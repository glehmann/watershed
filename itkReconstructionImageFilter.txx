#ifndef __itkReconstructionImageFilter_txx
#define __itkReconstructionImageFilter_txx

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkReconstructionImageFilter.h"
#include "itkProgressReporter.h"
#include "itkConstantBoundaryCondition.h"
#include "itkConnectedComponentAlgorithm.h"

#ifdef COPY
#include "itkConstantPadImageFilter.h"
#include "itkCropImageFilter.h"
#endif
#ifdef FACES
#include "itkImageDuplicator.h"
#endif
namespace itk {

template <class TInputImage, class TOutputImage, class TCompare>
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::ReconstructionImageFilter()
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();

  // get pointers to the inputs
  MarkerImagePointer  markerPtr =
    const_cast< MarkerImageType * >( this->GetInput(0) );

  MaskImagePointer  maskPtr =
    const_cast< MaskImageType * >( this->GetInput(1) );

  if ( !markerPtr || !maskPtr )
    {
    return;
    }
  markerPtr->SetRequestedRegion(markerPtr->GetLargestPossibleRegion());
  maskPtr->SetRequestedRegion(maskPtr->GetLargestPossibleRegion());
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::SetMarkerImage(const MarkerImageType* markerImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(0, const_cast<MarkerImageType *>( markerImage ));
}

template <class TInputImage, class TOutputImage, class TCompare>
const typename ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>::MarkerImageType *
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>::GetMarkerImage()
{
  return this->GetInput(0);
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>::SetMaskImage(const MaskImageType* maskImage)
{
  // Process object is not const-correct so the const casting is required.
  this->SetNthInput(1, const_cast<MaskImageType *>( maskImage ));
}

template <class TInputImage, class TOutputImage, class TCompare>
const typename ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>::MaskImageType *
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>::GetMaskImage()
{
  return this->GetInput(1);
}

#ifdef BASIC
// this is the basic version - it works and is a lot faster than the
// existing reconstruction routines in itk
template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  // there are 2 passes that use all pixels and a 3rd that uses some
  // subset of the pixels. We'll just pretend that the third pass
  // takes the same as each of the others. Is it OK to update more
  // often than pixels?
  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*3);

  typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef ImageRegionIterator<OutputImageType> OutputIteratorType;
  
  MarkerImageConstPointer markerImage = this->GetMarkerImage();
  MaskImageConstPointer   maskImage = this->GetMaskImage();
  OutputImagePointer      output = this->GetOutput();

  TCompare compare;

  InputIteratorType inIt( markerImage,
			  output->GetRequestedRegion() );
  OutputIteratorType outIt( output,
			    output->GetRequestedRegion() );
  inIt.GoToBegin();
  outIt.GoToBegin();

  // declare our queue type
  typedef typename std::queue<OutputImageIndexType> FifoType;
  FifoType IndexFifo;

  // copy marker to output - isn't there a better way?
  while ( !outIt.IsAtEnd() )
    {
    MarkerImagePixelType currentValue = inIt.Get();
    outIt.Set( static_cast<OutputImagePixelType>( currentValue ) );
    ++inIt;
    ++outIt;
    }
  ISizeType kernelRadius;
  kernelRadius.Fill(1);
  NOutputIterator outNIt(kernelRadius,
			 output,
			 output->GetRequestedRegion() );
  setConnectivityPrev( &outNIt, m_FullyConnected );
  
  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);
  outNIt.OverrideBoundaryCondition(&oBC);
  
  InputIteratorType mskIt( maskImage,
			   output->GetRequestedRegion() );
  
  mskIt.GoToBegin();
  // scan in forward raster order
  for (outNIt.GoToBegin(),mskIt.GoToBegin();!outNIt.IsAtEnd(); ++outNIt,++mskIt)
    {
    OutputImagePixelType V = outNIt.GetCenterPixel();
    // visit the previous neighbours
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskIt.Get());
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      }
    progress.CompletedPixel();
    }
  // now for the reverse raster order pass
  // reset the neighborhood
  setConnectivityLater( &outNIt, m_FullyConnected );
  outNIt.OverrideBoundaryCondition(&oBC);
  outNIt.GoToEnd();
  //mskIt.GoToEnd();
  
  ConstantBoundaryCondition<InputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);
  CNInputIterator mskNIt(kernelRadius,
			 maskImage,
			 output->GetRequestedRegion() );
  
  setConnectivityLater( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  
  typename NOutputIterator::IndexListType oIndexList, mIndexList;
  typename NOutputIterator::IndexListType::const_iterator oLIt, mLIt;
  
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  
  mskNIt.GoToEnd();
  while (!outNIt.IsAtBegin())
    {
    --outNIt;
    --mskNIt;
    OutputImagePixelType V = outNIt.GetCenterPixel();
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    //std::cout << (int)V << " " << mskNIt.GetIndex() <<std::endl;
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskNIt.GetCenterPixel());
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      V = iV;
      }

    // now put indexes in the fifo
    //typename CNInputIterator::ConstIterator mIt;
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); oLIt != oIndexList.end();++oLIt, ++mLIt)
      {
      
      //std::cout << " " << outNIt.GetIndex(*oLIt);
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      if (compare(V, VN) && compare(iN, VN)) 
	{
	IndexFifo.push(outNIt.GetIndex());
	break;
	}
      }
    progress.CompletedPixel();
    }

  
  // Now we want to check the full neighborhood
  setConnectivity( &outNIt, m_FullyConnected );
  setConnectivity( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  outNIt.OverrideBoundaryCondition(&oBC);
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  // now process the fifo - this fill the parts that weren't dealt
  // with by the raster and anti-raster passes
  //typename NOutputIterator::Iterator sIt;
  typename CNInputIterator::ConstIterator mIt;
  
  while (!IndexFifo.empty())
    {
    OutputImageIndexType I = IndexFifo.front();
    IndexFifo.pop();
    // reposition the iterators
    outNIt += I - outNIt.GetIndex();
    mskNIt += I - mskNIt.GetIndex();
    OutputImagePixelType V = outNIt.GetCenterPixel();
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); 
	 oLIt != oIndexList.end();
	 ++oLIt, ++mLIt)
      {
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      // candidate for dilation via flooding
      if (compare(V, VN) && (iN != VN))
	{
	if (compare(iN, V)) 
	  {
	  // not clamped by the mask, propogate the center value
	  outNIt.SetPixel(*oLIt, V);
	  }
	else
	  {
	  // apply the clamping
	  outNIt.SetPixel(*oLIt, iN);
	  }
	 IndexFifo.push(outNIt.GetIndex(*oLIt));
	}
      }     
    progress.CompletedPixel();
    }
  
}
#endif
#ifdef FACES
// this is the version which we use to experiment with the face
// calculator to optimize the performance. The basic idea will be to
// operate on the inside region in the normal way and then put all of
// the faces on the fifo
template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  // there are 2 passes that use all pixels and a 3rd that uses some
  // subset of the pixels. We'll just pretend that the third pass
  // takes the same as each of the others. Is it OK to update more
  // often than pixels?
  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*3);
  
  MarkerImageConstPointer markerImage = this->GetMarkerImage();
  MaskImageConstPointer   maskImage = this->GetMaskImage();
  OutputImagePointer      output = this->GetOutput();
  
  typename ImageDuplicator<MaskImageType>::Pointer dup = ImageDuplicator<MaskImageType>::New();

  dup->SetInputImage(maskImage);
  dup->Update();
  MaskImagePointer maskImageCopy = dup->GetOutput();
  MaskImageConstPointer maskImageConstCopy = dup->GetOutput();
  // seems that we need a copy of the mask image for the face based
  // optimization to work -- basically because we need to set the
  // borders of the mask too

  FifoType IndexFifo;

  ISizeType kernelRadius;
  kernelRadius.Fill(1);

  FaceCalculatorType faceCalculator;

  FaceListType faceList;
  FaceListTypeIt fit;

  faceList = faceCalculator(output, output->GetRequestedRegion(),
			    kernelRadius);
  
  // We will process the centre "block" first. This will require
  // setting the face regions to the marker value and copying the real
  // values in later
  InputIteratorType inIt( markerImage,
			  output->GetRequestedRegion());
  OutputIteratorType outIt( output,
			    output->GetRequestedRegion());
  // copy the body
  while ( !outIt.IsAtEnd() )
    {
    MarkerImagePixelType currentValue = inIt.Get();
    outIt.Set( static_cast<OutputImagePixelType>( currentValue ) );
    ++inIt;
    ++outIt;
    }


  // now do the raster passes over the faces to build the fifo
  for (fit = faceList.begin();fit != faceList.end();++fit)
    {  
    processRegion(progress, *fit, kernelRadius,
		  markerImage, maskImage, output, IndexFifo);
    }
  // process the fifo with boundary checks re-enabled
  processFifo(progress, output->GetRequestedRegion(), kernelRadius,
	      markerImage, maskImage, output, IndexFifo);


}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::fillFaces(FaceListType faceList,
	    OutputImagePointer &output)
{
  typename FaceCalculatorType::FaceListType::iterator fit;
  fit = faceList.begin();
  ++fit;  // don't fill the body

  for (;fit != faceList.end();++fit)
    {
      OutputIteratorType outIt(output,
			       *fit);
      for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
	{
	outIt.Set(m_MarkerValue);
	}
    }
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::copyFaces(FaceListType faceList,
	    MarkerImageConstPointer markerImage,
	    OutputImagePointer &output)
{
  typename FaceCalculatorType::FaceListType::iterator fit;
  fit = faceList.begin();
  ++fit;  // don't fill the body

  for (;fit != faceList.end();++fit)
    {
      OutputIteratorType outIt(output,
			       *fit);
      InputIteratorType inIt(markerImage,
			     *fit);
      for (outIt.GoToBegin(), inIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt, ++inIt)
	{
	outIt.Set(inIt.Get());
	}
    }
}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::processRegion(ProgressReporter &progress,
		const OutputImageRegionType thisRegion,
		const ISizeType kernelRadius,
		MarkerImageConstPointer markerImage,
		MaskImageConstPointer   maskImage,
		OutputImagePointer &output,
		FifoType &IndexFifo)
{
  NOutputIterator outNIt(kernelRadius,
			 output,
			 thisRegion );

  setConnectivityPrev( &outNIt, m_FullyConnected );
  
  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);
  outNIt.OverrideBoundaryCondition(&oBC);
  
  InputIteratorType mskIt( maskImage,
			   thisRegion );
  
  mskIt.GoToBegin();
  // scan in forward raster order
  for (outNIt.GoToBegin(),mskIt.GoToBegin();!outNIt.IsAtEnd(); ++outNIt,++mskIt)
    {
    OutputImagePixelType V = outNIt.GetCenterPixel();
    // visit the previous neighbours
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskIt.Get());
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      }
    progress.CompletedPixel();
    }
  // now for the reverse raster order pass
  // reset the neighborhood
  setConnectivityLater( &outNIt, m_FullyConnected );
  outNIt.OverrideBoundaryCondition(&oBC);
  outNIt.GoToEnd();
  //mskIt.GoToEnd();
  
  ConstantBoundaryCondition<InputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);
  CNInputIterator mskNIt(kernelRadius,
			 maskImage,
			 thisRegion );
  
  setConnectivityLater( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  
  typename NOutputIterator::IndexListType oIndexList, mIndexList;
  typename NOutputIterator::IndexListType::const_iterator oLIt, mLIt;
  
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  
  mskNIt.GoToEnd();
  while (!outNIt.IsAtBegin())
    {
    --outNIt;
    --mskNIt;
    OutputImagePixelType V = outNIt.GetCenterPixel();
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      OutputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    //std::cout << (int)V << " " << mskNIt.GetIndex() <<std::endl;
    // this step clamps to the mask 
    OutputImagePixelType iV = static_cast<OutputImagePixelType>(mskNIt.GetCenterPixel());
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      V = iV;
      }

    // now put indexes in the fifo
    //typename CNInputIterator::ConstIterator mIt;
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); oLIt != oIndexList.end();++oLIt, ++mLIt)
      {
      
      //std::cout << " " << outNIt.GetIndex(*oLIt);
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      if (compare(V, VN) && compare(iN, VN)) 
	{
	IndexFifo.push(outNIt.GetIndex());
	break;
	}
      }
    progress.CompletedPixel();
    }

}

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::buildFifo(ProgressReporter &progress,
	    const OutputImageRegionType thisRegion,
	    OutputImagePointer &output,
	    FifoType &IndexFifo)
{
  OutputIteratorType outIt( output,
			    thisRegion );
  for (outIt.GoToBegin(); !outIt.IsAtEnd(); ++outIt)
    {
    IndexFifo.push(outIt.GetIndex());
    progress.CompletedPixel();
    }
}
template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::processFifo(ProgressReporter &progress,
	      const OutputImageRegionType thisRegion,
	      const ISizeType kernelRadius,
	      MarkerImageConstPointer markerImage,
	      MaskImageConstPointer   maskImage,
	      OutputImagePointer &output,
	      FifoType &IndexFifo)
{
  NOutputIterator outNIt(kernelRadius,
			 output,
			 thisRegion );
  CNInputIterator mskNIt(kernelRadius,
			 maskImage,
			 thisRegion );
  ConstantBoundaryCondition<InputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);

  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);

  // Now we want to check the full neighborhood
  setConnectivity( &outNIt, m_FullyConnected );
  setConnectivity( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  outNIt.OverrideBoundaryCondition(&oBC);

  typename NOutputIterator::IndexListType oIndexList, mIndexList;
  typename NOutputIterator::IndexListType::const_iterator oLIt, mLIt;

  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  // now process the fifo - this fill the parts that weren't dealt
  // with by the raster and anti-raster passes
  //typename NOutputIterator::Iterator sIt;
  typename CNInputIterator::ConstIterator mIt;
  
  while (!IndexFifo.empty())
    {
    OutputImageIndexType I = IndexFifo.front();
    IndexFifo.pop();
    // reposition the iterators
    outNIt += I - outNIt.GetIndex();
    mskNIt += I - mskNIt.GetIndex();
    OutputImagePixelType V = outNIt.GetCenterPixel();
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); 
	 oLIt != oIndexList.end();
	 ++oLIt, ++mLIt)
      {
      OutputImagePixelType VN = outNIt.GetPixel(*oLIt);
      OutputImagePixelType iN = static_cast<OutputImagePixelType>(mskNIt.GetPixel(*mLIt));
      // candidate for dilation via flooding
      if (compare(V, VN) && (iN != VN))
	{
	if (compare(iN, V)) 
	  {
	  // not clamped by the mask, propogate the center value
	  outNIt.SetPixel(*oLIt, V);
	  }
	else
	  {
	  // apply the clamping
	  outNIt.SetPixel(*oLIt, iN);
	  }
	 IndexFifo.push(outNIt.GetIndex(*oLIt));
	}
      }     
    progress.CompletedPixel();
    }
  
}

#endif
#ifdef COPY
// a version that takes a padded copy of mask and marker
template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();
  // there are 2 passes that use all pixels and a 3rd that uses some
  // subset of the pixels. We'll just pretend that the third pass
  // takes the same as each of the others. Is it OK to update more
  // often than pixels?
  ProgressReporter progress(this, 0, this->GetOutput()->GetRequestedRegion().GetNumberOfPixels()*3);

  typedef ImageRegionConstIterator<InputImageType> InputIteratorType;
  typedef ImageRegionIterator<OutputImageType> OutputIteratorType;
  
  MarkerImageConstPointer markerImage = this->GetMarkerImage();
  MaskImageConstPointer   maskImage = this->GetMaskImage();
  OutputImagePointer      output = this->GetOutput();



  // create padded versions of the marker image and the mask image
  typedef typename itk::ConstantPadImageFilter<InputImageType, InputImageType> PadType;
  
  typename PadType::Pointer MaskPad = PadType::New();
  typename PadType::Pointer MarkerPad = PadType::New();

  ISizeType padSize;
  padSize.Fill( 1 );

  MaskPad->SetConstant(m_MarkerValue);
  MarkerPad->SetConstant(m_MarkerValue);
  MaskPad->SetPadLowerBound( padSize.m_Size );
  MaskPad->SetPadUpperBound( padSize.m_Size );
  MarkerPad->SetPadLowerBound( padSize.m_Size );
  MarkerPad->SetPadUpperBound( padSize.m_Size );

  MaskPad->SetInput(maskImage);
  MarkerPad->SetInput(markerImage);
  MaskPad->Update();
  MarkerPad->Update();

  MarkerImageConstPointer markerImageP = MarkerPad->GetOutput();
  MaskImageConstPointer   maskImageP = MaskPad->GetOutput();

  TCompare compare;

  FaceCalculatorType faceCalculator;

  FaceListType faceList;
  FaceListTypeIt fit;

  ISizeType kernelRadius;
  kernelRadius.Fill(1);

  faceList = faceCalculator(maskImageP, maskImageP->GetLargestPossibleRegion(),
			    kernelRadius);
  // we will only be processing the body region
  fit = faceList.begin();

  // declare our queue type
  typedef typename std::queue<OutputImageIndexType> FifoType;
  FifoType IndexFifo;

  NOutputIterator outNIt(kernelRadius,
			 markerImageP,
			 *fit );
  setConnectivityPrev( &outNIt, m_FullyConnected );
  
  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);
  outNIt.OverrideBoundaryCondition(&oBC);
  
  InputIteratorType mskIt( maskImageP,
			   *fit );
  
  mskIt.GoToBegin();
  // scan in forward raster order
  for (outNIt.GoToBegin(),mskIt.GoToBegin();!outNIt.IsAtEnd(); ++outNIt,++mskIt)
    {
    InputImagePixelType V = outNIt.GetCenterPixel();
    // visit the previous neighbours
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      InputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    // this step clamps to the mask 
    InputImagePixelType iV = static_cast<OutputImagePixelType>(mskIt.Get());
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      }
    progress.CompletedPixel();
    }
  // now for the reverse raster order pass
  // reset the neighborhood
  setConnectivityLater( &outNIt, m_FullyConnected );
  outNIt.OverrideBoundaryCondition(&oBC);
  outNIt.GoToEnd();
  //mskIt.GoToEnd();
  
  ConstantBoundaryCondition<InputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);
  CNInputIterator mskNIt(kernelRadius,
			 maskImageP,
			 *fit );
  
  setConnectivityLater( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  
  typename NOutputIterator::IndexListType oIndexList, mIndexList;
  typename NOutputIterator::IndexListType::const_iterator oLIt, mLIt;
  
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  
  mskNIt.GoToEnd();
  while (!outNIt.IsAtBegin())
    {
    --outNIt;
    --mskNIt;
    InputImagePixelType V = outNIt.GetCenterPixel();
    typename NOutputIterator::ConstIterator sIt;
    for (sIt = outNIt.Begin(); !sIt.IsAtEnd();++sIt)
      {
      InputImagePixelType VN = sIt.Get();
      if (compare(VN, V)) 
	{
	outNIt.SetCenterPixel(VN);
	V = VN;
	}
      }
    InputImagePixelType iV = mskNIt.GetCenterPixel();
    if (compare(V, iV))
      {
      outNIt.SetCenterPixel(iV);
      V = iV;
      }

    // now put indexes in the fifo
    //typename CNInputIterator::ConstIterator mIt;
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); oLIt != oIndexList.end();++oLIt, ++mLIt)
      {
      
      //std::cout << " " << outNIt.GetIndex(*oLIt);
      InputImagePixelType VN = outNIt.GetPixel(*oLIt);
      InputImagePixelType iN = mskNIt.GetPixel(*mLIt);
      if (compare(V, VN) && compare(iN, VN)) 
	{
	IndexFifo.push(outNIt.GetIndex());
	break;
	}
      }
    progress.CompletedPixel();
    }

  
  // Now we want to check the full neighborhood
  setConnectivity( &outNIt, m_FullyConnected );
  setConnectivity( &mskNIt, m_FullyConnected );
  mskNIt.OverrideBoundaryCondition(&iBC);
  outNIt.OverrideBoundaryCondition(&oBC);
  oIndexList = outNIt.GetActiveIndexList();
  mIndexList = mskNIt.GetActiveIndexList();
  // now process the fifo - this fill the parts that weren't dealt
  // with by the raster and anti-raster passes
  //typename NOutputIterator::Iterator sIt;
  typename CNInputIterator::ConstIterator mIt;
  
  while (!IndexFifo.empty())
    {
    InputImageIndexType I = IndexFifo.front();
    IndexFifo.pop();
    // reposition the iterators
    outNIt += I - outNIt.GetIndex();
    mskNIt += I - mskNIt.GetIndex();
    InputImagePixelType V = outNIt.GetCenterPixel();
    for (oLIt = oIndexList.begin(), mLIt = mIndexList.begin(); 
	 oLIt != oIndexList.end();
	 ++oLIt, ++mLIt)
      {
      InputImagePixelType VN = outNIt.GetPixel(*oLIt);
      InputImagePixelType iN = mskNIt.GetPixel(*mLIt);
      // candidate for dilation via flooding
      if (compare(V, VN) && (iN != VN))
	{
	if (compare(iN, V)) 
	  {
	  // not clamped by the mask, propogate the center value
	  outNIt.SetPixel(*oLIt, V);
	  }
	else
	  {
	  // apply the clamping
	  outNIt.SetPixel(*oLIt, iN);
	  }
	 IndexFifo.push(outNIt.GetIndex(*oLIt));
	}
      }     
    progress.CompletedPixel();
    }

    typedef typename itk::CropImageFilter<InputImageType, OutputImageType> CropType;
    typename CropType::Pointer crop = CropType::New();
    crop->SetInput( markerImageP );
    crop->SetUpperBoundaryCropSize( padSize );
    crop->SetLowerBoundaryCropSize( padSize );
    crop->GraftOutput( this->GetOutput() );
    /** execute the minipipeline */
    crop->Update();

    /** graft the minipipeline output back into this filter's output */
    this->GraftOutput( crop->GetOutput() );
  
}
#endif

template <class TInputImage, class TOutputImage, class TCompare>
void
ReconstructionImageFilter<TInputImage, TOutputImage, TCompare>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
  os << indent << "boundary value: " << m_MarkerValue << std::endl;
}
}
#endif
