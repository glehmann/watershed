/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryWatershedImageFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBinaryWatershedImageFilter_h
#define __itkBinaryWatershedImageFilter_h

#include "itkImageToImageFilter.h"

namespace itk {

/** \class BinaryWatershedImageFilter
 * \brief TODO
 *
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa WatershedImageFilter, MorphologicalWatershedFromMarkersImageFilter, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TDistance=float>
class ITK_EXPORT BinaryWatershedImageFilter : 
    public ImageToImageFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef BinaryWatershedImageFilter Self;
  typedef ImageToImageFilter<TInputImage, TOutputImage>
  Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Some convenient typedefs. */
  typedef TInputImage InputImageType;
  typedef TOutputImage OutputImageType;
  typedef typename InputImageType::Pointer         InputImagePointer;
  typedef typename InputImageType::ConstPointer    InputImageConstPointer;
  typedef typename InputImageType::RegionType      InputImageRegionType;
  typedef typename InputImageType::PixelType       InputImagePixelType;
  typedef typename OutputImageType::Pointer        OutputImagePointer;
  typedef typename OutputImageType::ConstPointer   OutputImageConstPointer;
  typedef typename OutputImageType::RegionType     OutputImageRegionType;
  typedef typename OutputImageType::PixelType      OutputImagePixelType;
  typedef typename OutputImageType::SizeType       SizeType;
  
  /** ImageDimension constants */
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);

  typedef TDistance DistanceType;
  typedef itk::Image< DistanceType, ImageDimension > DistanceImageType;

  /** Standard New method. */
  itkNewMacro(Self);  

  /** Runtime information support. */
  itkTypeMacro(BinaryWatershedImageFilter, 
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
   * Set/Get whether the output is a binary image or a label image. Default
   * is false (label image). The watershed line is drawn in the binary
   * to separate the objects.
   */
  itkSetMacro(BinaryOutput, bool);
  itkGetConstReferenceMacro(BinaryOutput, bool);
  itkBooleanMacro(BinaryOutput);

  /**
   */
  itkSetMacro(Level, DistanceType);
  itkGetMacro(Level, DistanceType);

  /**
   * Set/Get whether the distance map will be computing using the image
   * spacing. Default is true.
   */
  itkSetMacro(UseImageSpacing, bool);
  itkGetConstReferenceMacro(UseImageSpacing, bool);
  itkBooleanMacro(UseImageSpacing);
  
  /**
   */
  itkSetMacro(BackgroundValue, OutputImagePixelType);
  itkGetMacro(BackgroundValue, OutputImagePixelType);

  /**
   */
  itkSetMacro(ForegroundValue, InputImagePixelType);
  itkGetMacro(ForegroundValue, InputImagePixelType);

protected:
  BinaryWatershedImageFilter();
  ~BinaryWatershedImageFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** BinaryWatershedImageFilter needs the entire input be
   * available. Thus, it needs to provide an implementation of
   * GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion() ;

  /** BinaryWatershedImageFilter will produce the entire output. */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));
  
  /** Single-threaded version of GenerateData.  This filter delegates
   * to GrayscaleGeodesicErodeImageFilter. */
  void GenerateData();
  

private:
  BinaryWatershedImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_FullyConnected;

  bool m_BinaryOutput;

  bool m_UseImageSpacing;

  DistanceType m_Level;

  InputImagePixelType m_ForegroundValue;

  OutputImagePixelType m_BackgroundValue;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryWatershedImageFilter.txx"
#endif

#endif


