#ifndef __itkRegionalMaximaImageFilter_h
#define __itkRegionalMaximaImageFilter_h

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
class ITK_EXPORT RegionalMaximaImageFilter :
    public
    RegionalExtremaImageFilter<TInputImage, TOutputImage,
			       Function::GreaterThan<typename TInputImage::PixelType>,
			       Function::GreaterThan<typename TOutputImage::PixelType>  >
{
public:
  typedef RegionalMaximaImageFilter Self;
  typedef RegionalExtremaImageFilter<TInputImage, TOutputImage,
				     Function::GreaterThan<typename TInputImage::PixelType>,
				     Function::GreaterThan<typename TOutputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  RegionalMaximaImageFilter() 
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::min());
  }
  virtual ~RegionalMaximaImageFilter() {}

private:
  RegionalMaximaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end RegionalMaximaImageFilter

} //end namespace itk
#endif
