/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMinimaImpositionImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMinimaImpositionImageFilter_h
#define __itkMinimaImpositionImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class MinimaImpositionImageFilter
 * \brief Impose the minima defined in a marker image to the input image
 *
 * TLabelImage should be an integer type. All non zero pixels of the image are
 * considered as markers.
 *
 * The minima imposition algorithm is described in
 * Chapter 6.3.6 of Pierre Soille's book "Morphological Image Analysis:
 * Principles and Applications", Second Edition, Springer, 2003.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa WatershedImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TLabelImage>
class ITK_EXPORT MinimaImpositionImageFilter : 
    public ImageToImageFilter<TInputImage, TLabelImage>
{
public:
  /** Standard class typedefs. */
  typedef MinimaImpositionImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TLabelImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TLabelImage LabelImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputPixelType;
  typedef typename LabelImageType::Pointer        LabelImagePointer;
  typedef typename LabelImageType::ConstPointer   LabelImageConstPointer;
  typedef typename LabelImageType::RegionType     LabelImageRegionType;
  typedef typename LabelImageType::PixelType      LabelPixelType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(LabelImageDimension, unsigned int,
                      TLabelImage::ImageDimension);

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(MinimaImpositionImageFilter, 
               ImageToImageFilter);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);
  
  /**
   * Set/Get the shift applied to the input image to avoid having 2 makers
   * is the same minima. The default value (1) may not be relevant for real
   * type pixels and may cause problems if the image contains some pixels
   * with the maximum possible value for the pixel type.
   */
  itkSetMacro(Shift, InputPixelType);
  itkGetMacro(Shift, InputPixelType);
  
   /** Set the marker image */
  void SetMarkerImage(TLabelImage *input)
     {
     // Process object is not const-correct so the const casting is required.
     this->SetNthInput(1, const_cast<TLabelImage *>(input) );
     }

  /** Get the marker image */
  LabelImageType * GetMarkerImage()
    {
    return static_cast<LabelImageType*>(const_cast<DataObject *>(this->ProcessObject::GetInput(1)));
    }

protected:
  MinimaImpositionImageFilter();
  ~MinimaImpositionImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** MinimaImpositionImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** MinimaImpositionImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  MinimaImpositionImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_FullyConnected;

  InputPixelType m_Shift;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMinimaImpositionImageFilter.txx"
#endif

#endif


