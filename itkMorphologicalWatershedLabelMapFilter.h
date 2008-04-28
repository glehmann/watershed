/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMorphologicalWatershedLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2005/08/23 15:09:03 $
  Version:   $Revision: 1.4 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMorphologicalWatershedLabelMapFilter_h
#define __itkMorphologicalWatershedLabelMapFilter_h

#include "itkLabelMapFilter.h"

namespace itk {

/** \class MorphologicalWatershedLabelMapFilter
 * \brief TODO
 *
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa WatershedImageFilter, MorphologicalWatershedFromMarkersImageFilter, RelabelComponentImageFilter
 * \ingroup ImageEnhancement  MathematicalMorphologyImageFilters
 */
template<class TInputImage, class TOutputImage, class TDistance=float>
class ITK_EXPORT MorphologicalWatershedLabelMapFilter : 
    public LabelMapFilter<TInputImage, TOutputImage>
{
public:
  /** Standard class typedefs. */
  typedef MorphologicalWatershedLabelMapFilter Self;
  typedef LabelMapFilter<TInputImage, TOutputImage>
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
  typedef OutputImageType                          LabelMapType;
  typedef typename LabelMapType::LabelObjectType   LabelObjectType;
  
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
  itkTypeMacro(MorphologicalWatershedLabelMapFilter, 
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
   * Set/Get whether the watershed pixel must be marked or not. Default
   * is true. Set it to false do not only avoid writing watershed pixels,
   * it also decrease algorithm complexity.
   */
  itkSetMacro(MarkWatershedLine, bool);
  itkGetConstReferenceMacro(MarkWatershedLine, bool);
  itkBooleanMacro(MarkWatershedLine);

protected:
  MorphologicalWatershedLabelMapFilter();
  ~MorphologicalWatershedLabelMapFilter() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  virtual void ThreadedGenerateData( LabelObjectType * labelObject );

  virtual void BeforeThreadedGenerateData();

  virtual void AfterThreadedGenerateData();


private:
  MorphologicalWatershedLabelMapFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented

  bool m_FullyConnected;

  bool m_UseImageSpacing;

  DistanceType m_Level;

  bool m_MarkWatershedLine;

} ; // end of class

} // end namespace itk
  
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMorphologicalWatershedLabelMapFilter.txx"
#endif

#endif


