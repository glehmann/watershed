#ifndef _itkLabelOverlayImageFilter_txx
#define _itkLabelOverlayImageFilter_txx

#include "itkLabelOverlayImageFilter.h"

namespace itk
{

/**
 *
 */
template <class TInputImage, class TLabelImage, class TOutputImage>
LabelOverlayImageFilter<TInputImage, TLabelImage, TOutputImage>
::LabelOverlayImageFilter()
{
  m_Opacity = 1.0;
}

template <class TInputImage, class TLabelImage, class TOutputImage>
void
LabelOverlayImageFilter<TInputImage, TLabelImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  this->GetFunctor().SetOpacity(m_Opacity);
}

/**
 *
 */
template <class TInputImage, class TLabelImage, class TOutputImage>
void 
LabelOverlayImageFilter<TInputImage, TLabelImage, TOutputImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os,indent);

  os << indent << "Opacity: "
     << static_cast<typename NumericTraits<double>::PrintType>(m_Opacity)
     << std::endl;
}


} // end namespace itk

#endif
