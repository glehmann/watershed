/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimaImpositionImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimaImpositionImageFilter_txx
#define __itkMinimaImpositionImageFilter_txx

#include "itkMinimaImpositionImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkReconstructionByErosionImageFilter.h"
#include "itkMinimumImageFilter.h"
#include "itkShiftScaleImageFilter.h"
#include "itkProgressAccumulator.h"

namespace itk {

template <class TInputImage, class TLabelImage>
MinimaImpositionImageFilter<TInputImage, TLabelImage>
::MinimaImpositionImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_FullyConnected = false;
  m_Shift = 1;
}

template <class TInputImage, class TLabelImage>
void 
MinimaImpositionImageFilter<TInputImage, TLabelImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  LabelImagePointer label = this->GetMarkerImage();

  if ( !input || !label)
    { return; }
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
  label->SetRequestedRegion( label->GetLargestPossibleRegion() );
}


template <class TInputImage, class TLabelImage>
void 
MinimaImpositionImageFilter<TInputImage, TLabelImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TLabelImage>
void
MinimaImpositionImageFilter<TInputImage, TLabelImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // transform the marker image in an image with the smallest possible value
  // where there is some markers, and with the highest possible elsewhere
  //
  typedef BinaryThresholdImageFilter<TLabelImage, TInputImage> ThresholdFilterType;
  typename ThresholdFilterType::Pointer th = ThresholdFilterType::New();
  th->SetInput( this->GetMarkerImage() );
  th->SetLowerThreshold( 0 );
  th->SetUpperThreshold( 0 );
  th->SetOutsideValue( NumericTraits< InputPixelType >::NonpositiveMin() );
  th->SetInsideValue( NumericTraits< InputPixelType >::max() );

  // to avoid having 2 markers in the same minima, the input image must be encreased
  typedef ShiftScaleImageFilter< TInputImage, TInputImage > ShiftFilterType;
  typename ShiftFilterType::Pointer shift = ShiftFilterType::New();
  shift->SetInput( this->GetInput() );
  shift->SetShift( m_Shift );

  // to respect the precondition of the reconstruction, marker image must be above the
  // mask image
  typedef MinimumImageFilter< TInputImage, TInputImage, TInputImage > MinimumFilterType;
  typename MinimumFilterType::Pointer min = MinimumFilterType::New();
  min->SetInput( 0, th->GetOutput() );
  min->SetInput( 1, shift->GetOutput() );
  
  // now the reconstruction can be done
  typedef ReconstructionByErosionImageFilter< TInputImage, TInputImage > ReconstructionFilterType;
  typename ReconstructionFilterType::Pointer recons = ReconstructionFilterType::New();
  recons->SetMarkerImage( th->GetOutput() );
  recons->SetMaskImage( min->GetOutput() );
  recons->SetFullyConnected( m_FullyConnected );

  // graft our output to the reconstruction filter to force the proper regions
  // to be generated
  recons->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter( th, 0.1f );
  progress->RegisterInternalFilter( shift, 0.1f );
  progress->RegisterInternalFilter( min, 0.1f );
  progress->RegisterInternalFilter( recons, 0.7f );

  recons->Update();
  // graft the output of the reconstruction filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( recons->GetOutput() );
}


template<class TInputImage, class TLabelImage>
void
MinimaImpositionImageFilter<TInputImage, TLabelImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "Shift: "  << static_cast<typename NumericTraits<InputPixelType>::PrintType>(m_Shift) << std::endl;
}
  
}// end namespace itk
#endif
