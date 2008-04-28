/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMorphologicalWatershedLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMorphologicalWatershedLabelMapFilter_txx
#define __itkMorphologicalWatershedLabelMapFilter_txx

#include "itkMorphologicalWatershedLabelMapFilter.h"
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
MorphologicalWatershedLabelMapFilter<TInputImage, TOutputImage, TDistance>
::MorphologicalWatershedLabelMapFilter()
{
  m_FullyConnected = false;
  m_UseImageSpacing = true;
  m_MarkWatershedLine = false;
  m_Level = NumericTraits< InputImagePixelType >::Zero;
}

template<class TInputImage, class TOutputImage, class TDistance>
void
MorphologicalWatershedLabelMapFilter<TInputImage, TOutputImage, TDistance>
::BeforeThreadedGenerateData()
{
  Superclass::BeforeThreadedGenerateData();
  // a label map to store the result
  typename LabelMapType::Pointer labelMap = this->GetOutput();
  labelMap->SetBackgroundValue( this->GetInput()->GetBackgroundValue() );
}

template<class TInputImage, class TOutputImage, class TDistance>
void
MorphologicalWatershedLabelMapFilter<TInputImage, TOutputImage, TDistance>
::ThreadedGenerateData( LabelObjectType * labelObject )
{
  typedef itk::LabelSelectionLabelMapFilter< LabelMapType > SelectType;
  typename SelectType::Pointer select = SelectType::New();
  select->SetInPlace( false );
  select->SetInput( this->GetInput() );
  select->SetNumberOfThreads( 1 );
  select->SetLabel( labelObject->GetLabel() );

  typedef itk::AutoCropLabelMapFilter< LabelMapType > CropType;
  typename CropType::Pointer crop = CropType::New();
  crop->SetInput( select->GetOutput() );
  SizeType border;
  border.Fill( 1 );
  crop->SetCropBorder( border );
  crop->SetNumberOfThreads( 1 );

  typedef typename itk::Image< unsigned char, ImageDimension > InternalBinaryImageType;
  typedef itk::LabelMapToBinaryImageFilter< LabelMapType, InternalBinaryImageType> L2IType;
  typename L2IType::Pointer l2i = L2IType::New();
  l2i->SetInput( crop->GetOutput() );
  l2i->SetNumberOfThreads( 1 );

  // compute the distance map
  typedef itk::SignedMaurerDistanceMapImageFilter< InternalBinaryImageType, DistanceImageType > MapType;
  typename MapType::Pointer map = MapType::New();
  map->SetInput( l2i->GetOutput() );
  map->SetUseImageSpacing( m_UseImageSpacing );
  map->SetNumberOfThreads( 1 );
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
  watershed->SetMarkWatershedLine( m_MarkWatershedLine );
  watershed->SetNumberOfThreads( 1 );
  
  // mask the output
  typedef typename itk::LabelMapMaskImageFilter< LabelMapType, InternalLabelImageType > MaskType;
  typename MaskType::Pointer mask = MaskType::New();
  mask->SetInput1( crop->GetOutput() );
  mask->SetInput2( watershed->GetOutput() );
//   mask->SetBackgroundValue( m_BackgroundValue );
  mask->SetNumberOfThreads( 1 );
  mask->SetLabel( labelObject->GetLabel() );

  // and transform the objects to label map
  typedef itk::LabelImageToLabelMapFilter< InternalLabelImageType, LabelMapType> LI2LType;
  typename LI2LType::Pointer li2l = LI2LType::New();
  li2l->SetInput( mask->GetOutput() );
  li2l->SetNumberOfThreads( 1 );
//   li2l->SetBackgroundValue( m_BackgroundValue );

  // a label map to store the result
  typename LabelMapType::Pointer labelMap = this->GetOutput();


  li2l->UpdateLargestPossibleRegion();

  this->m_LabelObjectContainerLock->Lock();
  const typename LabelMapType::LabelObjectContainerType & labelObjectContainer2 = li2l->GetOutput()->GetLabelObjectContainer();
  typename LabelMapType::LabelObjectContainerType::const_iterator it2 = labelObjectContainer2.begin();
  while( it2 != labelObjectContainer2.end() )
    {
    LabelObjectType * labelObject2 = const_cast<LabelObjectType *>( it2->second.GetPointer() );
    labelMap->PushLabelObject( labelObject2 );
    it2++;
    }
  this->m_LabelObjectContainerLock->Unlock();
}


template<class TInputImage, class TOutputImage, class TDistance>
void
MorphologicalWatershedLabelMapFilter<TInputImage, TOutputImage, TDistance>
::AfterThreadedGenerateData()
{
  Superclass::AfterThreadedGenerateData();
}


template<class TInputImage, class TOutputImage, class TDistance>
void
MorphologicalWatershedLabelMapFilter<TInputImage, TOutputImage, TDistance>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "MarkWatershedLine: "  << m_MarkWatershedLine << std::endl;
  os << indent << "UseImageSpacing: "  << m_UseImageSpacing << std::endl;
  os << indent << "Level: "  << static_cast<typename NumericTraits<DistanceType>::PrintType>(m_Level) << std::endl;
}
  
}// end namespace itk
#endif
