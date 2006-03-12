#ifndef __itkReconstructionByErosionImageFilter_h
#define __itkReconstructionByErosionImageFilter_h

#include "itkReconstructionImageFilter.h"

#include "itkNumericTraits.h"

namespace itk {
/** \class ReconstructionByErosionImageFilter
 * \brief A grayscale geodesic dilation. No incremental option
 * available. Should be much faster than the current ITK version. Uses
 * the algorithm sescribed by Luc Vincent, "Morphological grayscale
 * reconstruction definition, efficient algorithm and applications in
 * image analysis". Proc IEEE Computer Vision and Pattern Recognition (1992)
 * \author Richard Beare. Department of Medicine, Monash University,
 * Melbourne, Australia.
 * \ingroup MathematicalMorphologyImageFilters
*/

template <class TInputImage, class TOutputImage>
class ITK_EXPORT ReconstructionByErosionImageFilter :
    public
    ReconstructionImageFilter<TInputImage, TOutputImage, std::less<typename TOutputImage::PixelType> >
{
public:
  typedef ReconstructionByErosionImageFilter Self;
  typedef ReconstructionImageFilter<TInputImage, TOutputImage, std::less<typename TOutputImage::PixelType> > Superclass;

  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);


protected:
  ReconstructionByErosionImageFilter()
  {
    SetMarkerValue(NumericTraits<typename TOutputImage::PixelType>::max());
  }
  virtual ~ReconstructionByErosionImageFilter() {}

private:
  ReconstructionByErosionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented



}; // end ReconstructionByErosionImageFilter



}

#endif
