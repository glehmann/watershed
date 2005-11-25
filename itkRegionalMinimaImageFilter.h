#ifndef __itkRegionalMinimaImageFilter_h
#define __itkRegionalMinimaImageFilter_h

#include "itkRegionalExtremaImageFilter.h"
#include "itkNumericTraits.h"

namespace itk {

namespace Function {
template <class TInput>
class LessThan
{
public:
  LessThan(){}
  ~LessThan(){}

  inline bool operator()(const TInput &A, TInput &B)
  {
    return (A<B);
  }
};
} // end namespace Function


template <class TInputImage, class TOutputImage>
class ITK_EXPORT RegionalMinimaImageFilter :
    public
    RegionalExtremaImageFilter<TInputImage, TOutputImage,
			       Function::LessThan<typename TInputImage::PixelType>,
			       Function::LessThan<typename TOutputImage::PixelType>
    >
{
public:
  typedef RegionalMinimaImageFilter Self;
  typedef RegionalExtremaImageFilter<TInputImage, TOutputImage,
				     Function::LessThan<typename TInputImage::PixelType>,
				     Function::LessThan<typename TOutputImage::PixelType>  > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  RegionalMinimaImageFilter() 
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::max());
  }
  virtual ~RegionalMinimaImageFilter() {}

private:
  RegionalMinimaImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end RegionalMinimaImageFilter

} //end namespace itk
#endif
