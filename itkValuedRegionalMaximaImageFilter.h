#ifndef __itkValuedRegionalMaximaImageFilter_h
#define __itkValuedRegionalMaximaImageFilter_h

#include "itkRegionalExtremaImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ValuedRegionalMaximaImageFilter :
    public
    RegionalExtremaImageFilter<TInputImage, TOutputImage,
			       std::greater<typename TInputImage::PixelType>,
			       std::greater<typename TOutputImage::PixelType>  >
{
public:
  typedef ValuedRegionalMaximaImageFilter Self;
  typedef RegionalExtremaImageFilter<TInputImage, TOutputImage,
				     std::greater<typename TInputImage::PixelType>,
				     std::greater<typename TOutputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  ValuedRegionalMaximaImageFilter() 
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::min());
  }
  virtual ~ValuedRegionalMaximaImageFilter() {}

private:
  ValuedRegionalMaximaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end ValuedRegionalMaximaImageFilter

} //end namespace itk
#endif
