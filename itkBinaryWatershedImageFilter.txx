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

#include "itkLabelObject.h"
#include "itkLabelMap.h"
#include "itkBinaryImageToLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkMorphologicalWatershedLabelMapFilter.h"

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

  // find the objects to split
  typedef itk::LabelObject< unsigned long, ImageDimension > LabelObjectType;
  typedef itk::LabelMap< LabelObjectType > LabelMapType;
  
  typedef itk::BinaryImageToLabelMapFilter< TInputImage, LabelMapType> I2LType;
  typename I2LType::Pointer i2l = I2LType::New();
  i2l->SetInput( this->GetInput() );
  i2l->SetFullyConnected( m_FullyConnected );
  i2l->SetForegroundValue( m_ForegroundValue );
  i2l->SetNumberOfThreads( this->GetNumberOfThreads() );
  i2l->SetBackgroundValue( m_BackgroundValue );
  progress->RegisterInternalFilter(i2l,0.1f);
  i2l->UpdateLargestPossibleRegion();
//   i2l->GetOutput()->PrintLabelObjects();

  typedef itk::MorphologicalWatershedLabelMapFilter< LabelMapType, LabelMapType, DistanceType > WatershedType;
  typename WatershedType::Pointer watershed = WatershedType::New();
  watershed->SetInput( i2l->GetOutput() );
  watershed->SetUseImageSpacing( m_UseImageSpacing );
  watershed->SetLevel( m_Level );
  watershed->SetFullyConnected( m_FullyConnected );
  watershed->SetMarkWatershedLine( m_BinaryOutput );
  watershed->SetNumberOfThreads( this->GetNumberOfThreads() );
  progress->RegisterInternalFilter(watershed,0.8f);
  watershed->UpdateLargestPossibleRegion();

  // Allocate the output
  this->AllocateOutputs();

  if( m_BinaryOutput )
    {
  
    typedef itk::LabelMapToBinaryImageFilter< LabelMapType, TOutputImage> L2IType2;
    typename L2IType2::Pointer l2i2 = L2IType2::New();
    l2i2->SetInput( watershed->GetOutput() );
    l2i2->SetForegroundValue( m_ForegroundValue );
    l2i2->SetBackgroundValue( m_BackgroundValue );
    l2i2->SetNumberOfThreads( this->GetNumberOfThreads() );
    progress->RegisterInternalFilter(l2i2,0.1f);

    l2i2->GraftOutput( this->GetOutput() );
    l2i2->Update();
    this->GraftOutput( l2i2->GetOutput() );

    }
  else
    {

    typedef itk::LabelMapToLabelImageFilter< LabelMapType, TOutputImage> L2LIType;
    typename L2LIType::Pointer l2li = L2LIType::New();
    l2li->SetInput( watershed->GetOutput() );
    l2li->SetNumberOfThreads( this->GetNumberOfThreads() );
    progress->RegisterInternalFilter(l2li,0.1f);

    l2li->GraftOutput( this->GetOutput() );
    l2li->Update();
    this->GraftOutput( l2li->GetOutput() );
    
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
