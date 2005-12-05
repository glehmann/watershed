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
#include "itkSize.h"
#include "itkConnectedComponentAlgorithm.h"

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
    { return; }

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
  this->GetOutput()->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TLabelImage>
void
MorphologicalWatershedFromMarkersImageFilter<TInputImage, TLabelImage>
::GenerateData()
{
  // there is 2 possible cases: with or without watershed lines.
  // the algorithm with watershed lines is from Meyer
  // the algorithm without watershed lines is from beucher
  // The 2 algorithms are very similar and so are integrated in the same filter.

  //---------------------------------------------------------------------------
  // declare the vars common to the 2 algorithms: constants, iterators,
  // hierarchical queue, progress reporter, and status image
  // also allocate output images and verify preconditions
  //---------------------------------------------------------------------------

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
    { itkExceptionMacro( << "Marker and input must have the same size." ); }
  
  // create a temporary image to store the state of each pixel (processed or not)
  typedef Image< bool, ImageDimension > StatusImageType;
  typename StatusImageType::Pointer statusImage = StatusImageType::New();
  statusImage->SetRegions( this->GetMarkerImage()->GetLargestPossibleRegion() );
  statusImage->Allocate();

  // FAH (in french: File d'Attente Hierarchique)
  typedef typename std::queue< IndexType > QueueType;
  typedef typename std::map< InputImagePixelType, QueueType > MapType;
  MapType fah;

  // the radius which will be used for all the shaped iterators
  Size< ImageDimension > radius;
  radius.Fill(1);

  // iterator for the marker image
  typedef ConstShapedNeighborhoodIterator<LabelImageType> MarkerIteratorType;
  typename MarkerIteratorType::ConstIterator nmIt;
  MarkerIteratorType markerIt(radius, this->GetMarkerImage(), this->GetMarkerImage()->GetRequestedRegion());
  // add a boundary constant to avoid adding pixels on the border in the fah
  ConstantBoundaryCondition<LabelImageType> lcbc;
  lcbc.SetConstant( NumericTraits<LabelImagePixelType>::max() );
  markerIt.OverrideBoundaryCondition(&lcbc);
  setConnectivity( &markerIt, m_FullyConnected );

  // iterator for the status image
  typedef ShapedNeighborhoodIterator<StatusImageType> StatusIteratorType;
  typename StatusIteratorType::Iterator nsIt;
  StatusIteratorType statusIt(radius, statusImage, this->GetOutput()->GetRequestedRegion());
  ConstantBoundaryCondition< StatusImageType > bcbc;
  bcbc.SetConstant( true );  // outside pixel are already processed
  statusIt.OverrideBoundaryCondition(&bcbc);
  setConnectivity( &statusIt, m_FullyConnected );

  typedef ImageRegionIterator<StatusImageType> StatusIteratorType2;
  StatusIteratorType2 statusIt2( statusImage, this->GetOutput()->GetRequestedRegion() );
  
  // iterator for the input image
  typedef ConstShapedNeighborhoodIterator<InputImageType> InputIteratorType;
  InputIteratorType inputIt(radius, this->GetInput(), this->GetInput()->GetRequestedRegion());
  typename InputIteratorType::ConstIterator niIt;
  setConnectivity( &inputIt, m_FullyConnected );
  
  // iterator for the output image
  typedef ShapedNeighborhoodIterator<LabelImageType> OutputIteratorType;
  typedef typename OutputIteratorType::OffsetType OffsetType;
  typename OutputIteratorType::Iterator noIt;
  OutputIteratorType outputIt(radius, this->GetOutput(), this->GetOutput()->GetRequestedRegion());
  ConstantBoundaryCondition<LabelImageType> lcbc2;
  lcbc2.SetConstant( NumericTraits<LabelImagePixelType>::Zero ); // outside pixel are watershed so they won't be use to find real watershed  pixels
  outputIt.OverrideBoundaryCondition(&lcbc2);
  setConnectivity( &outputIt, m_FullyConnected );

  typedef ImageRegionIterator<LabelImageType> OutputIteratorType2;
  OutputIteratorType2 outputIt2( this->GetOutput(), this->GetOutput()->GetRequestedRegion() );
  

  //---------------------------------------------------------------------------
  // Meyer's algorithm
  //---------------------------------------------------------------------------
  if( m_MarkWatershed )
    {
    // first stage:
    //  - set markers pixels to already processed status
    //  - copy markers pixels to output image
    //  - init FAH with indexes of background pixels with marker pixel(s) in their neighborhood

    // the status image must be initialized before the first stage. In the first stage, the
    // set to true are the neighbors of the marker (and the marker) so it's difficult
    // (impossible ?)to init the status image at the same time
    // the overhead should be small
    statusImage->FillBuffer( false );
  
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
        // copy it to the output image
        outputIt2.Set( markerPixel );
        // and increase progress because this pixel will not be used in the flooding stage.
        progress.CompletedPixel();
  
        // search the background pixels in the neighborhood
        for ( nmIt= markerIt.Begin(), nsIt= statusIt.Begin(), niIt= inputIt.Begin();
              nmIt != markerIt.End();
              nmIt++, nsIt++, niIt++ )
          {
          if ( !nsIt.Get() && nmIt.Get() == bgLabel )
            {
              // this neighbor is a background pixel and is not already processed; add its
              // index to fah
              fah[ niIt.Get() ].push( markerIt.GetIndex() + nmIt.GetNeighborhoodOffset() );
              // mark it as already in the fah to avoid adding it several times
              nsIt.Set( true );
            }
          }
        }
      else
        {
        // Some pixels may be never processed so, by default, non marked pixels
        // must be marked as watershed
        outputIt2.Set( wsLabel );
        }
      // one more pixel done in the init stage
      progress.CompletedPixel();
      }
    // end of init stage
    
    // flooding
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
        for (noIt = outputIt.Begin(); noIt != outputIt.End(); noIt++)
          {
          LabelImagePixelType o = noIt.Get();
          if( o != wsLabel && !collision )
            {
            if( marker != wsLabel && o != marker )
              { collision = true; }
            else
              { marker = o; }
            }
          }
        if( !collision )
          {
          // set the marker value
          outputIt.SetCenterPixel( marker );
          // and propagate to the neighbors
          for (niIt = inputIt.Begin(), nsIt = statusIt.Begin();
            niIt != inputIt.End();
            niIt++, nsIt++)
            {
            if ( !nsIt.Get() )
              {
              // the pixel is not yet processed. add it to the fah
              if ( niIt.Get() <= currentValue )
                { currentQueue.push( inputIt.GetIndex() + niIt.GetNeighborhoodOffset() ); }
              else
                { fah[ niIt.Get() ].push( inputIt.GetIndex() + niIt.GetNeighborhoodOffset() ); }
              // mark it as already in the fah
              nsIt.Set( true );
              }
            }
          }
        // one more pixel in the flooding stage
        progress.CompletedPixel();
        }
      }
    }


  //---------------------------------------------------------------------------
  // Beucher's algorithm
  //---------------------------------------------------------------------------
  else
    {
    // first stage:
    //  - set markers pixels to already processed status
    //  - copy markers pixels to output image
    //  - init FAH with indexes of pixels with background pixel in their neighborhood
    for ( markerIt.GoToBegin(), statusIt.GoToBegin(), outputIt.GoToBegin(), inputIt.GoToBegin();
      !markerIt.IsAtEnd();
      ++markerIt, ++statusIt, ++outputIt, ++inputIt ) 
      {
      LabelImagePixelType markerPixel = markerIt.GetCenterPixel();
      if ( markerPixel != bgLabel )
        {
        // this pixels belongs to a marker
        // mark it as already processed
        statusIt2.Set( true );
        // and copy it to the output image
        outputIt2.Set( markerPixel );
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
            fah[ inputIt.GetCenterPixel() ].push( markerIt.GetIndex() );
          }
        else
          {
          // increase progress because this pixel will not be used in the flooding stage.
          progress.CompletedPixel();
          }
        }
      else
        {
        statusIt2.Set( false );
        outputIt2.Set( wsLabel );
        }
      progress.CompletedPixel();
      }
    // end of init stage
    
    // flooding
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
              { currentQueue.push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() ); }
            else
              { fah[ niIt.Get() ].push( inputIt.GetIndex() + noIt.GetNeighborhoodOffset() ); }
            nsIt.Set( true );
            progress.CompletedPixel();
            }
          }
        }
      }
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
