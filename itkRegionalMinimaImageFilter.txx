/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkRegionalMinimaImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkRegionalMinimaImageFilter_txx
#define __itkRegionalMinimaImageFilter_txx

#include "itkRegionalMinimaImageFilter.h"
#include "itkValuedRegionalMinimaImageFilter.h"
#include "itkProgressAccumulator.h"
#include "itkNumericTraits.h"
#include "itkBinaryThresholdImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
RegionalMinimaImageFilter<TInputImage, TOutputImage>
::RegionalMinimaImageFilter()
{
  m_FullyConnected = false;
  m_ForegroundValue = NumericTraits<OutputImagePixelType>::max();
  m_BackgroundValue = NumericTraits<OutputImagePixelType>::NonpositiveMin();
}

template <class TInputImage, class TOutputImage>
void 
RegionalMinimaImageFilter<TInputImage, TOutputImage>
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


template <class TInputImage, class TOutputImage>
void 
RegionalMinimaImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
RegionalMinimaImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();

  // Delegate to the valued filter to find the minima
  typename ValuedRegionalMinimaImageFilter<TInputImage, TInputImage>::Pointer
    rmin = ValuedRegionalMinimaImageFilter<TInputImage, TInputImage>::New();
  rmin->SetInput( this->GetInput() );
  rmin->SetFullyConnected( m_FullyConnected );
  progress->RegisterInternalFilter( rmin, 0.67f );
  rmin->Update();

  if( rmin->GetFlat() )
    {
    this->GetOutput()->FillBuffer( m_ForegroundValue );
    }
  else
    {
    typedef BinaryThresholdImageFilter< InputImageType, OutputImageType > ThresholdType;
    typename ThresholdType::Pointer th = ThresholdType::New();
    th->SetInput( rmin->GetOutput() );
    th->SetUpperThreshold( rmin->GetMarkerValue() );
    th->SetLowerThreshold( rmin->GetMarkerValue() );
    th->SetOutsideValue( m_ForegroundValue );
    th->SetInsideValue( m_BackgroundValue );
    progress->RegisterInternalFilter( th, 0.33f );

    th->GraftOutput( this->GetOutput() );
    th->Update();
    this->GraftOutput( th->GetOutput() );
    }


}


template<class TInputImage, class TOutputImage>
void
RegionalMinimaImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
}// end namespace itk
#endif
