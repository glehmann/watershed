/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMorphologicalWatershedImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMorphologicalWatershedImageFilter_txx
#define __itkMorphologicalWatershedImageFilter_txx

#include "itkMorphologicalWatershedImageFilter.h"
#include "itkHConcaveImageFilter.h"
#include "itkConnectedComponentImageFilter.h"
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage>
MorphologicalWatershedImageFilter<TInputImage, TOutputImage>
::MorphologicalWatershedImageFilter()
{
  m_FullyConnected = false;
  m_MarkWatershed = true;
}

template <class TInputImage, class TOutputImage>
void 
MorphologicalWatershedImageFilter<TInputImage, TOutputImage>
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
MorphologicalWatershedImageFilter<TInputImage, TOutputImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage>
void
MorphologicalWatershedImageFilter<TInputImage, TOutputImage>
::GenerateData()
{
  // Create a process accumulator for tracking the progress of this minipipeline
  ProgressAccumulator::Pointer progress = ProgressAccumulator::New();
  progress->SetMiniPipelineFilter(this);

  // Allocate the output
  this->AllocateOutputs();
  
  // Delegate to a H-Concave filter to find the regional minima
  //
  typename HConcaveImageFilter<TInputImage, TOutputImage>::Pointer
    rmin = HConcaveImageFilter<TInputImage, TOutputImage>::New();
  rmin->SetInput( this->GetInput() );
  rmin->SetHeight( 1 );
  rmin->SetFullyConnected( m_FullyConnected );

  // label the components
  typename ConnectedComponentImageFilter< TOutputImage, TOutputImage >::Pointer
    label = ConnectedComponentImageFilter< TOutputImage, TOutputImage >::New();
  label->SetInput( rmin->GetOutput() );
  label->SetFullyConnected( m_FullyConnected );

  // the watershed
  typename MorphologicalWatershedFromMarkersImageFilter< TInputImage, TOutputImage >::Pointer
    wshed = MorphologicalWatershedFromMarkersImageFilter< TInputImage, TOutputImage >::New();
  wshed->SetInput( this->GetInput() );
  wshed->SetMarkerImage( label->GetOutput() );
  wshed->SetFullyConnected( m_FullyConnected );
  wshed->SetMarkWatershed( m_MarkWatershed );

  // graft our output to the watershed filter to force the proper regions
  // to be generated
  wshed->GraftOutput( this->GetOutput() );

  // run the algorithm
  progress->RegisterInternalFilter(rmin,0.35f);
  progress->RegisterInternalFilter(label,.3f);
  progress->RegisterInternalFilter(wshed,.35f);

  wshed->Update();

  // graft the output of the watershed filter back onto this filter's
  // output. this is needed to get the appropriate regions passed
  // back.
  this->GraftOutput( wshed->GetOutput() );
}


template<class TInputImage, class TOutputImage>
void
MorphologicalWatershedImageFilter<TInputImage, TOutputImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "MarkWatershed: "  << m_MarkWatershed << std::endl;
}
  
}// end namespace itk
#endif
