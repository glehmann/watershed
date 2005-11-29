#ifndef __itkRegionalExtremaImageFilter_txx
#define __itkRegionalExtremaImageFilter_txx

#include "itkImageRegionConstIterator.h"
#include "itkImageRegionIterator.h"
#include "itkNumericTraits.h"
#include "itkRegionalExtremaImageFilter.h"

namespace itk {

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2> 
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::RegionalExtremaImageFilter()
{
  m_FullyConnected = false;
}

template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void 
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::GenerateInputRequestedRegion()
{
  // call the superclass' implementation of this method
  Superclass::GenerateInputRequestedRegion();
  
  // We need all the input.
  InputImagePointer input = const_cast<InputImageType *>(this->GetInput());
  
  input->SetRequestedRegion( input->GetLargestPossibleRegion() );
}


template <class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void 
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::EnlargeOutputRequestedRegion(DataObject *)
{
  this->GetOutput()
    ->SetRequestedRegion( this->GetOutput()->GetLargestPossibleRegion() );
}


template<class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::GenerateData()
{
  // Allocate the output
  this->AllocateOutputs();

  // copy input to output - isn't there a better way?
  typedef ImageRegionConstIterator<TInputImage> InputIterator;
  typedef ImageRegionIterator<TOutputImage> OutputIterator;

  InputIterator inIt( this->GetInput(), 
		      this->GetOutput()->GetRequestedRegion() );  
  OutputIterator outIt( this->GetOutput(), 
			this->GetOutput()->GetRequestedRegion() );
  inIt = inIt.Begin();
  outIt = outIt.Begin();

  while ( !outIt.IsAtEnd() )
    {
    outIt.Set( static_cast<OutputImagePixelType>( inIt.Get() ) );
    ++inIt;
    ++outIt;
    }

  // Now for the real work!
  // More iterators - use shaped ones so that we can set connectivity

  ISizeType kernelRadius;
  kernelRadius.Fill(1);
  NOutputIterator outNIt(kernelRadius, 
			 this->GetOutput(), 
			 this->GetOutput()->GetRequestedRegion() );

  CNInputIterator inNIt(kernelRadius, 
		       this->GetInput(), 
		       this->GetOutput()->GetRequestedRegion() );
  

  typename CNInputIterator::OffsetType offsetIn;
  typename NOutputIterator::OffsetType offsetOut;

  if (!m_FullyConnected)
    {
    // only activate the neighbors that are face connected
    // to the current pixel. do not include the center pixel
    offsetIn.Fill(0);
    offsetOut.Fill(0);
    for (unsigned int d=0; d < InputImageType::ImageDimension; ++d)
      {
      offsetIn[d] = -1;
      offsetOut[d] = -1;
      inNIt.ActivateOffset(offsetIn);
      outNIt.ActivateOffset(offsetOut);
      offsetIn[d] = 1;
      offsetOut[d] = 1;
      inNIt.ActivateOffset(offsetIn);
      outNIt.ActivateOffset(offsetOut);
      offsetIn[d] = 0;
      offsetOut[d] = 0;
      }
    }
  else
    {
    // activate all neighbors that are face+edge+vertex
    // connected to the current pixel. do not include the center pixel
    unsigned int centerIndex = inNIt.GetCenterNeighborhoodIndex();
    for (unsigned int d=0; d < centerIndex*2 + 1; d++)
      {
      offsetIn = inNIt.GetOffset(d);
      inNIt.ActivateOffset(offsetIn);
      offsetOut = outNIt.GetOffset(d);
      outNIt.ActivateOffset(offsetOut);
      }
    offsetIn.Fill(0);
    offsetOut.Fill(0);
    inNIt.DeactivateOffset(offsetIn);
    outNIt.DeactivateOffset(offsetOut);
    }

  ConstantBoundaryCondition<OutputImageType> iBC;
  iBC.SetConstant(m_MarkerValue);
  inNIt.OverrideBoundaryCondition(&iBC);

  ConstantBoundaryCondition<OutputImageType> oBC;
  oBC.SetConstant(m_MarkerValue);
  outNIt.OverrideBoundaryCondition(&oBC);

  TFunction1 compareIn;
  TFunction2 compareOut;

  outIt = outIt.Begin();

  // set up the stack and neighbor list
  IndexStack IS;
  typename NOutputIterator::IndexListType IndexList;
  IndexList = outNIt.GetActiveIndexList();

  while ( !outIt.IsAtEnd() )
    {
    OutputImagePixelType V = outIt.Get();
    if (compareOut(V, m_MarkerValue)) 
      {
      inNIt.SetLocation(outIt.GetIndex());
      // Optimization should be possible - Cent should be same as V?
      //InputImagePixelType Cent = inNIt.GetCenterPixel(); 

      InputImagePixelType Cent = static_cast<InputImagePixelType>(V);

      //if (static_cast<OutputImagePixelType>(Cent) != V)
      //std::cout << "Not equal" << std::endl;

      typename CNInputIterator::ConstIterator sIt;
      for (sIt = inNIt.Begin(); !sIt.IsAtEnd(); ++sIt)
	{
	InputImagePixelType Adjacent = sIt.Get();
	if (compareIn(Adjacent, Cent))
	  {
	  // The centre pixel cannot be part of a regional minima
	  // because one of its neighbors is smaller.
	  // Set all pixels in the output image that are connected to
	  // the centre pixel and have the same value to m_MarkerValue
	  outNIt.SetLocation(outIt.GetIndex());
	  setConnectedPixels(outNIt, V, IS, IndexList);
	  break;
	  }
	}
      }
    ++outIt;
    }
  
 
}
template<class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void 
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::setConnectedPixels(NOutputIterator &OIt, OutputImagePixelType Val,
		     IndexStack &IS,
		     const typename NOutputIterator::IndexListType IndexList)
{
  // We can obviously look up Val inside this function. It is only
  // being passed as an optimization.

  // This flood fills connected pixels. We'll do this using a <stack>
  // Might consider passing the stack as an argument too.


  OutputImagePixelType NVal;
  //IndexStack IS;
  OutIndexType idx;
  IS.push(OIt.GetIndex());
  OIt.SetCenterPixel(m_MarkerValue);
  
  // Might consider passing this in as well
  //typename NOutputIterator::IndexListType IndexList;
  //IndexList = OIt.GetActiveIndexList();
  typename NOutputIterator::IndexListType::const_iterator LIt;

  while (!IS.empty())
    {
    idx = IS.top();
    IS.pop();
    OIt.SetLocation(idx);
    for (LIt = IndexList.begin(); LIt != IndexList.end(); ++LIt)
      {
      NVal = OIt.GetPixel(*LIt);
      if (NVal == Val)
	{
	// still in a flat zone
	IS.push(OIt.GetIndex(*LIt));
	OIt.SetPixel(*LIt, m_MarkerValue);
	}
      }
    }

}

template<class TInputImage, class TOutputImage, class TFunction1, class TFunction2>
void
RegionalExtremaImageFilter<TInputImage, TOutputImage, TFunction1, TFunction2>
::PrintSelf(std::ostream &os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "FullyConnected: "  << m_FullyConnected << std::endl;
}
  
} // end namespace itk

#endif
