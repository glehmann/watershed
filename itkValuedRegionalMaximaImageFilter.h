#ifndef __itkValuedRegionalMaximaImageFilter_h
#define __itkValuedRegionalMaximaImageFilter_h

#include "itkRegionalExtremaImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {

namespace Function {
template <class TInput>
class GreaterThan
{
public:
  GreaterThan(){}
  ~GreaterThan(){}

  inline bool operator()(const TInput &A, TInput &B)
  {
    return (A>B);
  }
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT ValuedRegionalMaximaImageFilter :
    public
    RegionalExtremaImageFilter<TInputImage, TOutputImage,
			       Function::GreaterThan<typename TInputImage::PixelType>,
			       Function::GreaterThan<typename TOutputImage::PixelType>  >
{
public:
  typedef ValuedRegionalMaximaImageFilter Self;
  typedef RegionalExtremaImageFilter<TInputImage, TOutputImage,
				     Function::GreaterThan<typename TInputImage::PixelType>,
				     Function::GreaterThan<typename TOutputImage::PixelType> > Superclass;

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
