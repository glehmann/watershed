/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMorphologicalWatershedFromMarkersImageFilter.txx,v $
  Language:  C++
  Date:      $Date: 2004/12/21 22:47:30 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

    This software is distributed WITHOUT ANY WARRANTY; without even 
    the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
    PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMorphologicalWatershedFromMarkersImageFilter_txx
#define __itkMorphologicalWatershedFromMarkersImageFilter_txx

#include <algorithm>
#include <queue>
#include <list>
#include "itkMorphologicalWatershedFromMarkersImageFilter.h"
#include "itkProgressReporter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIteratorWithIndex.h"
#include "itkShapedNeighborhoodIterator.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkConstantBoundaryCondition.h"

namespace itk {

template <class TInputImage, class TLabelImage>
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::MorphologicalWatershedFromMarkersImageFilter()
{
  this->SetNumberOfRequiredInputs(2);
  m_FullyConnected = false;
  m_MarkWatershed = true;
}


template <class TInputImage, class TLabelImage>
void 
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // get pointers to the inputs
  LabelImagePointer  markerPtr = this->GetMarkerImage();

  InputImagePointer  inputPtr = 
    const_cast< InputImageType * >( this->GetInput() );
  
  if ( !markerPtr || !inputPtr )
    {
    return;
    }

  // We need to
  // configure the inputs such that all the data is available.
  //
  markerPtr->SetRequestedRegion(markerPtr->GetLargestPossibleRegion());
  inputPtr->SetRequestedRegion(inputPtr->GetLargestPossibleRegion());
}


template <class TInputImage, class TLabelImage>
void 
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TLabelImage>
void
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::GenerateData()
{
  // the label used to find background in the marker image
  static const LabelImagePixelType bgLabel = NumericTraits< LabelImagePixelType >::Zero;
  // the label used to mark the watershed line in the output image
  static const LabelImagePixelType wsLabel = NumericTraits< LabelImagePixelType >::Zero;

  this->AllocateOutputs();
  // Set up the progress reporter
  // we can't found the exact number of pixel to process in the 2nd pass, so we use the maximum number possible.
  ProgressReporter progress(this, 0, this->GetMarkerImage()->GetRequestedRegion().GetNumberOfPixels()*2);
  
  // mask and marker must have the same size
  if ( this->GetMarkerImage()->GetRequestedRegion().GetSize() != this->GetInput()->GetRequestedRegion().GetSize() )
    {
    itkExceptionMacro( << "Marker and input must have the same size." );
    }
  
  // create a temporary image to store the state of each pixel (processed or not)
  typedef Image< bool, ImageDimension > StatusImageType;
  typename StatusImageType::Pointer statusImage = StatusImageType::New();
  statusImage->SetRegions( this->GetMarkerImage()->GetLargestPossibleRegion() );
  statusImage->Allocate();

  // FAH (in french: File d'Attente Hierarchique)
  typedef typename std::queue< IndexType > QueueType;
  typedef typename std::map< InputImagePixelType, QueueType > MapType;
  MapType fah;
  
  if( m_MarkWatershed )
    {
    statusImage->FillBuffer( false );
  
    // first stage:
    //  - set markers pixels to already processed status
    //  - copy markers pixels to output image
    //  - init FAH with indexes of background pixels with marker pixel(s) in their neighborhood
    {
    // iterator for the marker image
    typedef ConstShapedNeighborhoodIterator<LabelImageType> MarkerIteratorType;
    typename MarkerIteratorType::RadiusType radius;
    radius.Fill(1);
    typename MarkerIteratorType::ConstIterator nmIt;
    MarkerIteratorType markerIt(radius, this->GetMarkerImage(), this->GetMarkerImage()->GetRequestedRegion());
    // iterator for the status image
    typedef ShapedNeighborhoodIterator<StatusImageType> StatusIteratorType;
    StatusIteratorType statusIt(radius, statusImage, this->GetMarkerImage()->GetRequestedRegion());
    typename StatusIteratorType::Iterator nsIt;
    // iterator for the input image
    typedef ConstShapedNeighborhoodIterator<InputImageType> InputIteratorType;
    InputIteratorType inputIt(radius, this->GetInput(), this->GetInput()->GetRequestedRegion());
    typename InputIteratorType::ConstIterator niIt;
    
    // activate neighbors 
    typename MarkerIteratorType::OffsetType offset;
    unsigned int d;
    typename MarkerIteratorType::OffsetValueType i;
    offset.Fill(0);
    if (!m_FullyConnected)
      {
      for (d=0; d < ImageDimension; ++d)
        {
        for (i=-1; i<=1; i+=2)
          {
          offset[d] = i;
          markerIt.ActivateOffset(offset); // a neighbor pixel in dimension d
          statusIt.ActivateOffset(offset);
          inputIt.ActivateOffset(offset);
          }
        offset[d] = 0;
        }
      }
    else
      {
      // activate all pixels excepted center pixel
      for (d=0; d < markerIt.GetCenterNeighborhoodIndex()*2+1; ++d)
        {
          markerIt.ActivateOffset( markerIt.GetOffset(d) );
          statusIt.ActivateOffset( statusIt.GetOffset(d) );
          inputIt.ActivateOffset( inputIt.GetOffset(d) );
        }
      offset.Fill(0);
      markerIt.DeactivateOffset(offset);
      statusIt.DeactivateOffset(offset);
      inputIt.DeactivateOffset(offset);
      }
    // add a boundary constant to avoid adding pixels on the border in the fah
    ConstantBoundaryCondition<LabelImageType> lcbc;
    lcbc.SetConstant( NumericTraits<LabelImagePixelType>::max() );
    markerIt.OverrideBoundaryCondition(&lcbc);
  
    // iterator for the output image
    typedef ImageRegionIterator<LabelImageType> OutputIteratorType;
    OutputIteratorType outputIt(this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    
    for ( markerIt.GoToBegin(), statusIt.GoToBegin(), outputIt.GoToBegin(), inputIt.GoToBegin();
      !markerIt.IsAtEnd();
      ++markerIt, ++statusIt, ++outputIt, ++inputIt ) 
      {
      LabelImagePixelType markerPixel = markerIt.GetCenterPixel();
      if ( markerPixel != bgLabel )
        {
        // this pixel belongs to a marker
        // mark it as already processed
        statusIt.SetCenterPixel( true );
        // and copy it to the output image
        outputIt.Set( markerPixel );
  
        // search the background pixels in the neighborhood
        for ( nmIt= markerIt.Begin(), nsIt= statusIt.Begin(), niIt= inputIt.Begin();
              nmIt != markerIt.End();
              nmIt++, nsIt++, niIt++ )
          {
          if ( !nsIt.Get() && nmIt.Get() == bgLabel )
            {
              // this neighbor is a background pixel and is not already processed; add to fah
              fah[ niIt.Get() ].push( markerIt.GetIndex() + nmIt.GetNeighborhoodOffset() );
              // already in the fah
              nsIt.Set( true );
            }
          }
          // increase progress because this pixel will not be used in the flooding stage.
          progress.CompletedPixel();
        }
      else
        {
        outputIt.Set( wsLabel );
        }
      progress.CompletedPixel();
      }
    // end of init stage
    }
    
    // flooding
    {
    // iterator for the output image
    typedef ShapedNeighborhoodIterator<LabelImageType> OutputIteratorType;
    typedef typename OutputIteratorType::OffsetType OffsetType;
    typename OutputIteratorType::RadiusType radius;
    radius.Fill(1);
    typename OutputIteratorType::Iterator noIt;
    OutputIteratorType outputIt(radius, this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    ConstantBoundaryCondition<LabelImageType> lcbc;
    lcbc.SetConstant( NumericTraits<LabelImagePixelType>::Zero ); // outside pixel are watershed so they won't be use to find real watershed  pixels
    outputIt.OverrideBoundaryCondition(&lcbc);
  
    // iterator for the status image
    typedef ShapedNeighborhoodIterator<StatusImageType> StatusIteratorType;
    typename StatusIteratorType::Iterator nsIt;
    StatusIteratorType statusIt(radius, statusImage, this->GetOutput()->GetRequestedRegion());
    ConstantBoundaryCondition< StatusImageType > bcbc;
    bcbc.SetConstant( true );  // outside pixel are already processed
    statusIt.OverrideBoundaryCondition(&bcbc);
  
    // iterator for the intput image
    typedef ConstShapedNeighborhoodIterator<InputImageType> InputIteratorType;
    typename InputIteratorType::ConstIterator niIt;
    InputIteratorType inputIt(radius, this->GetInput(), this->GetInput()->GetRequestedRegion());
  
    // activate neighbors 
    typename OutputIteratorType::OffsetType offset;
    unsigned int d;
    typename OutputIteratorType::OffsetValueType i;
    offset.Fill(0);
    if (!m_FullyConnected)
      {
      for (d=0; d < ImageDimension; ++d)
        {
        for (i=-1; i<=1; i+=2)
          {
          offset[d] = i;
          outputIt.ActivateOffset(offset); // a neighbor pixel in dimension d
          statusIt.ActivateOffset(offset);
          inputIt.ActivateOffset(offset);
          }
        offset[d] = 0;
        }
      }
    else
      {
      // activate all pixels excepted center pixel
      for (d=0; d < outputIt.GetCenterNeighborhoodIndex()*2+1; ++d)
        {
          outputIt.ActivateOffset( outputIt.GetOffset(d) );
          statusIt.ActivateOffset( statusIt.GetOffset(d) );
          inputIt.ActivateOffset( inputIt.GetOffset(d) );
        }
      offset.Fill(0);
      outputIt.DeactivateOffset(offset);
      statusIt.DeactivateOffset(offset);
      inputIt.DeactivateOffset(offset);
      }
  
    // init all the iterators
    outputIt.GoToBegin();
    statusIt.GoToBegin();
    inputIt.GoToBegin();
    

    // and start flooding
    while ( !fah.empty() )
      {
      // store the current vars
      InputImagePixelType currentValue = fah.begin()->first;
      QueueType currentQueue = fah.begin()->second;
      // and remove them from the fah
      fah.erase( fah.begin() );
    
      while ( !currentQueue.empty() )
        {
        IndexType idx = currentQueue.front();
        currentQueue.pop();
        
        // move the iterators to the right place
        OffsetType shift = idx - outputIt.GetIndex();
        outputIt += shift;
        statusIt += shift;
        inputIt += shift;
        
        // iterate over the neighbors. If there is only one marker value, give that value
        // to the pixel, else keep it as is (watershed line)
        LabelImagePixelType marker = wsLabel;
        bool collision = false;
        for (noIt = outputIt.Begin(), niIt = inputIt.Begin(), nsIt = statusIt.Begin();
          noIt != outputIt.End();
          noIt++, niIt++, nsIt++)
          {
          LabelImagePixelType o = noIt.Get();
          if( o != wsLabel && !collision )
            {
            if( marker != wsLabel && o != marker )
              { collision = true; }
            else
              { marker = o; }
            }
          if ( !nsIt.Get() )
            {
            // the pixel is not yet processed. add it to the fah
            if ( niIt.Get() <= currentValue )
              { currentQueue.push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() ); }
            else
              { fah[ niIt.Get() ].push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() ); }
            // mark it as already in the fah
            nsIt.Set( true );
            }
          }
        if( !collision )
          {
          // set the marker value
          outputIt.SetCenterPixel( marker );
          }
        progress.CompletedPixel();
        }
      }
    } // end of flooding
    }
  else
    {
    // first stage:
    //  - set markers pixels to already processed status
    //  - copy markers pixels to output image
    //  - init FAH with indexes of pixels with background pixel in their neighborhood
    {
    // iterator for the marker image
    typedef ConstShapedNeighborhoodIterator<LabelImageType> MarkerIteratorType;
    typename MarkerIteratorType::RadiusType radius;
    radius.Fill(1);
    typename MarkerIteratorType::ConstIterator nmIt;
    MarkerIteratorType markerIt(radius, this->GetMarkerImage(), this->GetMarkerImage()->GetRequestedRegion());
    // activate neighbors 
    typename MarkerIteratorType::OffsetType offset;
    unsigned int d;
    typename MarkerIteratorType::OffsetValueType i;
    offset.Fill(0);
    if (!m_FullyConnected)
      {
      for (d=0; d < ImageDimension; ++d)
        {
        for (i=-1; i<=1; i+=2)
          {
          offset[d] = i;
          markerIt.ActivateOffset(offset); // a neighbor pixel in dimension d
          }
        offset[d] = 0;
        }
      }
    else
      {
      // activate all pixels excepted center pixel
      for (d=0; d < markerIt.GetCenterNeighborhoodIndex()*2+1; ++d)
        {
          markerIt.ActivateOffset( markerIt.GetOffset(d) );
        }
      offset.Fill(0);
      markerIt.DeactivateOffset(offset);
      }
    // add a boundary constant to avoid adding pixels on the border in the fah
    ConstantBoundaryCondition<LabelImageType> lcbc;
    lcbc.SetConstant( NumericTraits<LabelImagePixelType>::max() );
    markerIt.OverrideBoundaryCondition(&lcbc);
  
    // iterator for the status image
    typedef ImageRegionIterator<StatusImageType> StatusIteratorType;
    StatusIteratorType statusIt(statusImage, this->GetMarkerImage()->GetRequestedRegion());
    
    // iterator for the output image
    typedef ImageRegionIterator<LabelImageType> OutputIteratorType;
    OutputIteratorType outputIt(this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    
    // iterator for the input image
    typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
    InputIteratorType inputIt(this->GetInput(), this->GetInput()->GetRequestedRegion());
    
    for ( markerIt.GoToBegin(), statusIt.GoToBegin(), outputIt.GoToBegin(), inputIt.GoToBegin();
      !markerIt.IsAtEnd();
      ++markerIt, ++statusIt, ++outputIt, ++inputIt ) 
      {
      LabelImagePixelType markerPixel = markerIt.GetCenterPixel();
      if ( markerPixel != bgLabel )
        {
        // this pixels belongs to a marker
        // mark it as already processed
        statusIt.Set( true );
        // and copy it to the output image
        outputIt.Set( markerPixel );
        // search if it has background pixel in its neighborhood
        bool haveBgNeighbor = false;
        for ( nmIt= markerIt.Begin(); nmIt != markerIt.End() && !haveBgNeighbor; nmIt++ )
          {
          if ( nmIt.Get() == bgLabel )
            { haveBgNeighbor = true; }
          }
        if ( haveBgNeighbor )
          {
            // there is a background pixel in the neighborhood; add to fah
            fah[ inputIt.Get() ].push( markerIt.GetIndex() );
          }
        else
          {
          // increase progress because this pixel will not be used in the flooding stage.
          progress.CompletedPixel();
          }
        }
      else
        {
        // this pixel does not belong to a marker
        // mark it as not processed and give it a default output value
        // the default output value must be the watershed marker because
        // in some particular cases, some pixels which should be marked as watershed
        // won't be processed and will have an undefinded value without that.
        // here is an example where the center pixel won't be processed:
        // 1 0 2
        // 0 0 0
        // 4 0 3
        //
        statusIt.Set( false );
        outputIt.Set( wsLabel );
        }
      progress.CompletedPixel();
      }
    // end of init stage
    }
    
    // flooding
    {
    // iterator for the output image
    typedef ShapedNeighborhoodIterator<LabelImageType> OutputIteratorType;
    typedef typename OutputIteratorType::OffsetType OffsetType;
    typename OutputIteratorType::RadiusType radius;
    radius.Fill(1);
    typename OutputIteratorType::Iterator noIt;
    OutputIteratorType outputIt(radius, this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    typename OutputIteratorType::Iterator noIt2;
    OutputIteratorType outputIt2(radius, this->GetOutput(), this->GetOutput()->GetRequestedRegion());
    ConstantBoundaryCondition<LabelImageType> lcbc;
    lcbc.SetConstant( NumericTraits<LabelImagePixelType>::Zero ); // outside pixel are watershed so they won't be use to find real watershed  pixels
    outputIt.OverrideBoundaryCondition(&lcbc);
    outputIt2.OverrideBoundaryCondition(&lcbc);
  
    // iterator for the status image
    typedef ShapedNeighborhoodIterator<StatusImageType> StatusIteratorType;
    typename StatusIteratorType::Iterator nsIt;
    StatusIteratorType statusIt(radius, statusImage, this->GetOutput()->GetRequestedRegion());
    typename StatusIteratorType::Iterator nsIt2;
    StatusIteratorType statusIt2(radius, statusImage, this->GetOutput()->GetRequestedRegion());
    ConstantBoundaryCondition< StatusImageType > bcbc;
    bcbc.SetConstant( true );  // outside pixel are already processed
    statusIt.OverrideBoundaryCondition(&bcbc);
    statusIt2.OverrideBoundaryCondition(&bcbc);
  
      // iterator for the intput image
    typedef ConstShapedNeighborhoodIterator<InputImageType> InputIteratorType;
    typename InputIteratorType::ConstIterator niIt;
    InputIteratorType inputIt(radius, this->GetInput(), this->GetInput()->GetRequestedRegion());
  
    // activate neighbors 
    typename OutputIteratorType::OffsetType offset;
    unsigned int d;
    typename OutputIteratorType::OffsetValueType i;
    offset.Fill(0);
    if (!m_FullyConnected)
      {
      for (d=0; d < ImageDimension; ++d)
        {
        for (i=-1; i<=1; i+=2)
          {
          offset[d] = i;
          outputIt.ActivateOffset(offset); // a neighbor pixel in dimension d
          statusIt.ActivateOffset(offset);
          inputIt.ActivateOffset(offset);
          outputIt2.ActivateOffset(offset);
          statusIt2.ActivateOffset(offset);
          }
        offset[d] = 0;
        }
      }
    else
      {
      // activate all pixels excepted center pixel
      for (d=0; d < outputIt.GetCenterNeighborhoodIndex()*2+1; ++d)
        {
          outputIt.ActivateOffset( outputIt.GetOffset(d) );
          statusIt.ActivateOffset( statusIt.GetOffset(d) );
          inputIt.ActivateOffset( inputIt.GetOffset(d) );
          outputIt2.ActivateOffset( inputIt.GetOffset(d) );
          statusIt2.ActivateOffset( inputIt.GetOffset(d) );
        }
      offset.Fill(0);
      outputIt.DeactivateOffset(offset);
      statusIt.DeactivateOffset(offset);
      inputIt.DeactivateOffset(offset);
      outputIt2.DeactivateOffset(offset);
      statusIt2.DeactivateOffset(offset);
      }
  
    // init all the iterators
    outputIt.GoToBegin();
    outputIt2.GoToBegin();
    statusIt.GoToBegin();
    statusIt2.GoToBegin();
    inputIt.GoToBegin();
      
  
    // and start flooding
    while ( !fah.empty() )
      {
      // store the current vars
      InputImagePixelType currentValue = fah.begin()->first;
      QueueType currentQueue = fah.begin()->second;
      // and remove them from the fah
      fah.erase( fah.begin() );
    
      while ( !currentQueue.empty() )
        {
        IndexType idx = currentQueue.front();
        currentQueue.pop();
        
        // move the iterators to the right place
        OffsetType shift = idx - outputIt.GetIndex();
        outputIt += shift;
        statusIt += shift;
        inputIt += shift;
        
        LabelImagePixelType currentMarker = outputIt.GetCenterPixel();
        // get the current value of the pixel
        // iterate over neighbors to propagate the marker
        for (noIt = outputIt.Begin(), niIt = inputIt.Begin(), nsIt = statusIt.Begin();
          noIt != outputIt.End();
          noIt++, niIt++, nsIt++)
          {
          if ( !nsIt.Get() )
            {
            // the pixel is not yet processed. It can be labeled with the current label
            noIt.Set( currentMarker );
            if ( niIt.Get() <= currentValue )
              {
              currentQueue.push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() );
              }
            else
              {
              fah[ niIt.Get() ].push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() );
              }
            nsIt.Set( true );
            progress.CompletedPixel();
            }
          }
        }
      }
    } // end of flooding
    }
}



template<class TInputImage, class TLabelImage>
void
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
  
  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "MarkWatershed: "  << m_MarkWatershed << std::endl;
}
  
}// end namespace itk
#endif
