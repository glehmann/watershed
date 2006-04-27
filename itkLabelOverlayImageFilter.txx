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
  m_Opacity = 0.5;
  m_BackgroundValue = NumericTraits<LabelPixelType>::Zero;
  m_UseBackground = false;
}

template <class TInputImage, class TLabelImage, class TOutputImage>
void
LabelOverlayImageFilter<TInputImage, TLabelImage, TOutputImage>
::BeforeThreadedGenerateData()
{
  this->GetFunctor().SetOpacity(m_Opacity);
  this->GetFunctor().SetBackgroundValue(m_BackgroundValue);
  this->GetFunctor().SetUseBackground(m_UseBackground);
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
     << std::endl
     << indent << "BackgroundValue: "
     << static_cast<typename NumericTraits<LabelPixelType>::PrintType>(m_BackgroundValue)
     << std::endl
     << indent << "UseBackground: "
     << static_cast<typename NumericTraits<bool>::PrintType>(m_UseBackground)
     << std::endl;
}


} // end namespace itk

#endif
