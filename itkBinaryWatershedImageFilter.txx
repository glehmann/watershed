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
#include "itkLabelImageToLabelMapFilter.h"
#include "itkLabelMapToBinaryImageFilter.h"
#include "itkLabelMapToLabelImageFilter.h"
#include "itkLabelSelectionLabelMapFilter.h"
#include "itkAutoCropLabelMapFilter.h"
#include "itkLabelMapMaskImageFilter.h"

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
//   i2l->SetBackgroundValue( m_BackgroundValue );
  i2l->Update();
//   i2l->GetOutput()->PrintLabelObjects();

  typedef itk::LabelSelectionLabelMapFilter< LabelMapType > SelectType;
  typename SelectType::Pointer select = SelectType::New();
  select->SetInPlace( false );
  select->SetInput( i2l->GetOutput() );
  select->SetNumberOfThreads( this->GetNumberOfThreads() );

  typedef itk::AutoCropLabelMapFilter< LabelMapType > CropType;
  typename CropType::Pointer crop = CropType::New();
  crop->SetInput( select->GetOutput() );
  SizeType border;
  border.Fill( 1 );
  crop->SetCropBorder( border );
  crop->SetNumberOfThreads( this->GetNumberOfThreads() );

  typedef typename itk::Image< unsigned char, ImageDimension > InternalBinaryImageType;
  typedef itk::LabelMapToBinaryImageFilter< LabelMapType, InternalBinaryImageType> L2IType;
  typename L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( crop->GetOutput() );
  l2i->SetNumberOfThreads( this->GetNumberOfThreads() );

  // compute the distance map
  typedef itk::SignedMaurerDistanceMapImageFilter< InternalBinaryImageType, DistanceImageType > MapType;
  typename MapType::Pointer map = MapType::New();
  map->SetInput( l2i->GetOutput() );
  map->SetUseImageSpacing( m_UseImageSpacing );
  map->SetNumberOfThreads( this->GetNumberOfThreads() );
//   map->SetInsideIsPositive( true );
  if( m_Level != 0 )
    {
    map->SetSquaredDistance( false );
    }
  
  typedef unsigned long InternalLabelType;
  typedef typename itk::Image< InternalLabelType, ImageDimension > InternalLabelImageType;

  // run the watershed
  typedef MorphologicalWatershedImageFilter< DistanceImageType, InternalLabelImageType > WatershedType;
  typename WatershedType::Pointer watershed = WatershedType::New();
  watershed->SetInput( map->GetOutput() );
  watershed->SetLevel( m_Level );
  watershed->SetFullyConnected( m_FullyConnected );
  watershed->SetMarkWatershedLine( m_BinaryOutput );
  watershed->SetNumberOfThreads( this->GetNumberOfThreads() );
  
  // mask the output
  typedef typename itk::LabelMapMaskImageFilter< LabelMapType, InternalLabelImageType > MaskType;
  typename MaskType::Pointer mask = MaskType::New();
  mask->SetInput1( crop->GetOutput() );
  mask->SetInput2( watershed->GetOutput() );
//   mask->SetBackgroundValue( m_BackgroundValue );
  mask->SetNumberOfThreads( this->GetNumberOfThreads() );

  // and transform the objects to label map
  typedef itk::LabelImageToLabelMapFilter< InternalLabelImageType, LabelMapType> LI2LType;
  typename LI2LType::Pointer li2l = LI2LType::New();
  li2l->SetInput( mask->GetOutput() );
  li2l->SetNumberOfThreads( this->GetNumberOfThreads() );
//   li2l->SetBackgroundValue( m_BackgroundValue );

  // a label map to store the result
  typename LabelMapType::Pointer labelMap = LabelMapType::New();
  labelMap->SetBackgroundValue( m_BackgroundValue );
  labelMap->SetRegions( this->GetOutput()->GetLargestPossibleRegion() );
  labelMap->SetSpacing( this->GetOutput()->GetSpacing() );

  const typename LabelMapType::LabelObjectContainerType & labelObjectContainer = i2l->GetOutput()->GetLabelObjectContainer();
  typename LabelMapType::LabelObjectContainerType::const_iterator it = labelObjectContainer.begin();
  long currentLabel = 0;
  while( it != labelObjectContainer.end() )
    {
    const typename LabelObjectType::LabelType & label = it->first;
    const LabelObjectType * labelObject = it->second;

    select->SetLabel( label );
    mask->SetLabel( label );

    li2l->UpdateLargestPossibleRegion();

    const typename LabelMapType::LabelObjectContainerType & labelObjectContainer2 = li2l->GetOutput()->GetLabelObjectContainer();
    typename LabelMapType::LabelObjectContainerType::const_iterator it2 = labelObjectContainer2.begin();
    while( it2 != labelObjectContainer2.end() )
      {
      LabelObjectType * labelObject2 = const_cast<LabelObjectType *>( it2->second.GetPointer() );
      // avoid the background label
      if( currentLabel == m_BackgroundValue )
        {
        currentLabel++;
        }
      labelObject2->SetLabel( currentLabel );
      labelMap->AddLabelObject( labelObject2 );

      currentLabel++;
      it2++;
      }

    it++;
    }
  

  // Allocate the output
  this->AllocateOutputs();

  if( m_BinaryOutput )
    {
  
    typedef itk::LabelMapToBinaryImageFilter< LabelMapType, TOutputImage> L2IType2;
    typename L2IType2::Pointer l2i2 = L2IType2::New();
    l2i2->SetInput( labelMap );
    l2i2->SetForegroundValue( m_ForegroundValue );
    l2i2->SetBackgroundValue( m_BackgroundValue );
    l2i2->SetNumberOfThreads( this->GetNumberOfThreads() );

    l2i2->GraftOutput( this->GetOutput() );
    l2i2->Update();
    this->GraftOutput( l2i2->GetOutput() );

    }
  else
    {

    typedef itk::LabelMapToLabelImageFilter< LabelMapType, TOutputImage> L2LIType;
    typename L2LIType::Pointer l2li = L2LIType::New();
    l2li->SetInput( labelMap );
    l2li->SetNumberOfThreads( this->GetNumberOfThreads() );

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
