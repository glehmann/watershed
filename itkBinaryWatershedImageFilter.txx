/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryWatershedImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryWatershedImageFilter_txx
#define __itkBinaryWatershedImageFilter_txx

#include "itkBinaryWatershedImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkSignedMaurerDistanceMapImageFilter.h"
#include "itkMorphologicalWatershedImageFilter.h"
#include "itkMaskImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TDistance>
BinaryWatershedImageFilter<TInputImage, TOutputImage, TDistance>
::BinaryWatershedImageFilter()
{
  m_FullyConnected = false;
  m_BinaryOutput = false;
  m_UseImageSpacing = true;
  m_Level = NumericTraits< InputImagePixelType >::Zero;
  m_ForegroundValue = NumericTraits< OutputImagePixelType >::max();
  m_BackgroundValue = NumericTraits< OutputImagePixelType >::Zero;
}

template <class TInputImage, class TOutputImage, class TDistance>
void 
BinaryWatershedImageFilter<TInputImage, TOutputImage, TDistance>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  if ( !input )
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TDistance>
void 
BinaryWatershedImageFilter<TInputImage, TOutputImage, TDistance>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage, class TDistance>
void
BinaryWatershedImageFilter<TInputImage, TOutputImage, TDistance>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  typedef typename itk::Image< unsigned char, ImageDimension > InternalBinaryImageType;

  // prepare the input image
  typedef typename itk::BinaryThresholdImageFilter< TInputImage, InternalBinaryImageType > ThresholdType;
  typename ThresholdType::Pointer th = ThresholdType::New();
  th->SetInput( this->GetInput() );
  th->SetLowerThreshold( m_ForegroundValue );
  th->SetUpperThreshold( m_ForegroundValue );
  th->SetNumberOfThreads( this->GetNumberOfThreads() );
/*  th->SetInsideValue(0);
  th->SetOutsideValue(255);*/
  
  // compute the distance map
  typedef itk::SignedMaurerDistanceMapImageFilter< InternalBinaryImageType, DistanceImageType > MapType;
  typename MapType::Pointer map = MapType::New();
  map->SetInput( th->GetOutput() );
  map->SetUseImageSpacing( m_UseImageSpacing );
  map->SetNumberOfThreads( this->GetNumberOfThreads() );
//   map->SetInsideIsPositive( true );
  if( m_Level != 0 )
    {
    map->SetSquaredDistance( false );
    }
  

  // mask the output
  typedef typename itk::MaskImageFilter< InternalBinaryImageType, TOutputImage, TOutputImage > MaskType;
  typename MaskType::Pointer mask = MaskType::New();
//  mask->SetInput1( watershed->GetOutput() );
  mask->SetInput2( th->GetOutput() );
  mask->SetOutsideValue( m_BackgroundValue );
  mask->SetNumberOfThreads( this->GetNumberOfThreads() );


  if( m_BinaryOutput )
    {
    typedef unsigned long InternalLabelType;
    typedef typename itk::Image< InternalLabelType, ImageDimension > InternalLabelImageType;

    // run the watershed
    typedef MorphologicalWatershedImageFilter< DistanceImageType, InternalLabelImageType > WatershedType;
    typename WatershedType::Pointer watershed = WatershedType::New();
    watershed->SetInput( map->GetOutput() );
    watershed->SetLevel( m_Level );
    watershed->SetFullyConnected( m_FullyConnected );
    watershed->SetMarkWatershedLine( true );
    watershed->SetNumberOfThreads( this->GetNumberOfThreads() );
  
    // switch back to a lighter type (and binarize the image)
    typedef itk::BinaryThresholdImageFilter< InternalLabelImageType, OutputImageType > Threshold2Type;
    typename Threshold2Type::Pointer th2 = Threshold2Type::New();
    th2->SetInput( watershed->GetOutput() );
    th2->SetLowerThreshold( 0 );
    th2->SetUpperThreshold( 0 );
    th2->SetInsideValue( m_BackgroundValue );
    th2->SetOutsideValue( m_ForegroundValue );
    th2->SetNumberOfThreads( this->GetNumberOfThreads() );

    mask->SetInput1( th2->GetOutput() );
  
    progress->RegisterInternalFilter(th,0.05f);
    progress->RegisterInternalFilter(map,0.4f);
    progress->RegisterInternalFilter(watershed,.4f);
    progress->RegisterInternalFilter(th2,0.05f);
    progress->RegisterInternalFilter(mask,0.05f);
  
    mask->GraftOutput( this->GetOutput() );
    mask->Update();
    this->GraftOutput( mask->GetOutput() );

    // finally copy background which should have been eroded
    //
    // iterator on input image
    ImageRegionConstIterator<InputImageType> inIt
              = ImageRegionConstIterator<InputImageType>( this->GetInput(),
                      this->GetOutput()->GetRequestedRegion() );
    // iterator on output image
    ImageRegionIterator<OutputImageType> outIt
              = ImageRegionIterator<OutputImageType>( this->GetOutput(),
                      this->GetOutput()->GetRequestedRegion() );
    outIt.GoToBegin();
    inIt.GoToBegin();
  
    ProgressReporter progress2(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels(), 20, 0.95, 0.05);
    while( !outIt.IsAtEnd() )
      {
      if( inIt.Get() != m_ForegroundValue )
        {
        outIt.Set( static_cast<OutputImagePixelType>( inIt.Get() ) );
        }
      ++outIt;
      ++inIt;
      progress2.CompletedPixel();
      }
    // the end !

    }
  else
    {
    // run the watershed
    typedef MorphologicalWatershedImageFilter< DistanceImageType, OutputImageType > WatershedType;
    typename WatershedType::Pointer watershed = WatershedType::New();
    watershed->SetInput( map->GetOutput() );
    watershed->SetLevel( m_Level );
    watershed->SetFullyConnected( m_FullyConnected );
    watershed->SetMarkWatershedLine( false );
    watershed->SetNumberOfThreads( this->GetNumberOfThreads() );
    watershed->SetWatershedLabel( m_BackgroundValue );
  
    mask->SetInput1( watershed->GetOutput() );
  
    progress->RegisterInternalFilter(th,0.1f);
    progress->RegisterInternalFilter(map,0.4f);
    progress->RegisterInternalFilter(watershed,0.4f);
    progress->RegisterInternalFilter(mask,.1f);

    mask->GraftOutput( this->GetOutput() );
    mask->Update();
    this->GraftOutput( mask->GetOutput() );
    
    }
}


template<class TInputImage, class TOutputImage, class TDistance>
void
BinaryWatershedImageFilter<TInputImage, TOutputImage, TDistance>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "UseImageSpacing: "  << m_UseImageSpacing << std::endl;
  os << indent << "BinaryOutput: "  << m_BinaryOutput << std::endl;
  os << indent << "Level: "  << static_cast<typename NumericTraits<DistanceType>::PrintType>(m_Level) << std::endl;
  os << indent << "ForegroundValue: "  << static_cast<typename NumericTraits<InputImagePixelType>::PrintType>(m_ForegroundValue) << std::endl;
  os << indent << "BackgroundValue: "  << static_cast<typename NumericTraits<OutputImagePixelType>::PrintType>(m_BackgroundValue) << std::endl;
}
  
}// end namespace itk
#endif
