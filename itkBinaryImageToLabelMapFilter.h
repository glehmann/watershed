/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBinaryImageToLabelMapFilter.h,v $
  Language:  C++
  Date:      $Date: 2007/02/18 20:03:03 $
  Version:   $Revision: 1.16 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkBinaryImageToLabelMapFilter_h
#define __itkBinaryImageToLabelMapFilter_h

#include "itkImageToImageFilter.h"
#include "itkImage.h"
#include "itkConceptChecking.h"
#include <vector>
#include <map>
#include "itkProgressReporter.h"
#include "itkBarrier.h"

namespace itk
{

/**
 * \class BinaryImageToLabelMapFilter
 * \brief Label the connected components in a binary image and produce a collection of label objects
 *
 * BinaryImageToLabelMapFilter labels the objects in a binary image.
 * Each distinct object is assigned a unique label. 
 * The final object labels start with 1 and are consecutive. Objects
 * that are reached earlier by a raster order scan have a lower
 * label.
 *
 * \author Ga�tan Lehmann. Biologie du D�veloppement et de la Reproduction, INRA de Jouy-en-Josas, France.
 *
 * \sa ConnectedComponentImageFilter, LabelImageToLabelMapFilter, LabelMap
 */

template <class TInputImage, class TOutputImage=LabelMap< LabelObject< unsigned long, TInputImage::ImageDimension > > >
class ITK_EXPORT BinaryImageToLabelMapFilter : 
    public ImageToImageFilter< TInputImage, TOutputImage > 
{
public:
  /**
   * Standard "Self" & Superclass typedef.
   */
  typedef BinaryImageToLabelMapFilter                   Self;
  typedef ImageToImageFilter< TInputImage, TOutputImage > Superclass;

  /**
   * Types from the Superclass
   */
  typedef typename Superclass::InputImagePointer InputImagePointer;

  /**
   * Extract some information from the image types.  Dimensionality
   * of the two images is assumed to be the same.
   */
  typedef typename TOutputImage::PixelType         OutputPixelType;
  typedef typename TInputImage::PixelType          InputPixelType;
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(OutputImageDimension, unsigned int,
                      TOutputImage::ImageDimension);
  itkStaticConstMacro(InputImageDimension, unsigned int,
                      TInputImage::ImageDimension);
  
  /**
   * Image typedef support
   */
  typedef TInputImage                       InputImageType;
  typedef typename TInputImage::IndexType   IndexType;
  typedef typename TInputImage::SizeType    SizeType;
  typedef typename TInputImage::OffsetType  OffsetType;

  typedef TOutputImage                      OutputImageType;
  typedef typename TOutputImage::RegionType RegionType;
  typedef typename TOutputImage::IndexType  OutputIndexType;
  typedef typename TOutputImage::SizeType   OutputSizeType;
  typedef typename TOutputImage::OffsetType OutputOffsetType;
  typedef typename TOutputImage::PixelType  OutputImagePixelType;

  typedef std::list<IndexType>              ListType;

  /** 
   * Smart pointer typedef support 
   */
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /**
   * Run-time type information (and related methods)
   */
  itkTypeMacro(BinaryImageToLabelMapFilter, ImageToImageFilter);
  
  /**
   * Method for creation through the object factory.
   */
  itkNewMacro(Self);

  /**
   * Set/Get whether the connected components are defined strictly by
   * face connectivity or by face+edge+vertex connectivity.  Default is
   * FullyConnectedOff.  For objects that are 1 pixel wide, use
   * FullyConnectedOn.
   */
  itkSetMacro(FullyConnected, bool);
  itkGetConstReferenceMacro(FullyConnected, bool);
  itkBooleanMacro(FullyConnected);
  
  // only set after completion
  itkGetConstReferenceMacro(ObjectCount, unsigned long);

  // Concept checking -- input and output dimensions must be the same
  itkConceptMacro(SameDimension,
    (Concept::SameDimension<itkGetStaticConstMacro(InputImageDimension),
       itkGetStaticConstMacro(OutputImageDimension)>));


  /**
   * Set/Get the value used as "background" in the output image.
   * Defaults to NumericTraits<PixelType>::NonpositiveMin().
   */
  itkSetMacro(BackgroundValue, OutputPixelType);
  itkGetConstMacro(BackgroundValue, OutputPixelType);

  /**
   * Set/Get the value used as "foreground" in the output image.
   * Defaults to NumericTraits<PixelType>::max().
   */
  itkSetMacro(ForegroundValue, InputPixelType);
  itkGetConstMacro(ForegroundValue, InputPixelType);

protected:
  BinaryImageToLabelMapFilter() 
    {
    m_FullyConnected = false;
    m_ObjectCount = 0;
    m_BackgroundValue = NumericTraits<OutputPixelType>::NonpositiveMin();
    m_ForegroundValue = NumericTraits<InputPixelType>::max();
    }
  virtual ~BinaryImageToLabelMapFilter() {}
  BinaryImageToLabelMapFilter(const Self&) {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /**
   * Standard pipeline method. 
   */
  void BeforeThreadedGenerateData ();
  void AfterThreadedGenerateData ();
  void ThreadedGenerateData (const RegionType& outputRegionForThread, int threadId) ;

  /** BinaryImageToLabelMapFilter needs the entire input. Therefore
   * it must provide an implementation GenerateInputRequestedRegion().
   * \sa ProcessObject::GenerateInputRequestedRegion(). */
  void GenerateInputRequestedRegion();

  /** BinaryImageToLabelMapFilter will produce all of the output.
   * Therefore it must provide an implementation of
   * EnlargeOutputRequestedRegion().
   * \sa ProcessObject::EnlargeOutputRequestedRegion() */
  void EnlargeOutputRequestedRegion(DataObject *itkNotUsed(output));

  bool m_FullyConnected;
  
private:
  OutputPixelType m_BackgroundValue;
  InputPixelType m_ForegroundValue;

  unsigned long m_ObjectCount;
  // some additional types
  typedef typename TOutputImage::RegionType::SizeType OutSizeType;

  // types to support the run length encoding of lines
  class runLength
    {
    public:
    // run length information - may be a more type safe way of doing this
    long int length;
    typename InputImageType::IndexType where; // Index of the start of the run
    unsigned long int label; // the initial label of the run
    };

  typedef std::vector<runLength> lineEncoding;

  // the map storing lines
  typedef std::vector<lineEncoding> LineMapType;
  
  typedef std::vector<long> OffsetVec;

  // the types to support union-find operations
  typedef std::vector<unsigned long int> UnionFindType;
  UnionFindType m_UnionFind;
  UnionFindType m_Consecutive;
  // functions to support union-find operations
  void InitUnion(const unsigned long int size) 
    {
    m_UnionFind = UnionFindType(size + 1);
    }
  void InsertSet(const unsigned long int label);
  unsigned long int LookupSet(const unsigned long int label);
  void LinkLabels(const unsigned long int lab1, const unsigned long int lab2);
  unsigned long int CreateConsecutive();
  //////////////////
  bool CheckNeighbors(const OutputIndexType &A, 
                      const OutputIndexType &B);

  void CompareLines(lineEncoding &current, const lineEncoding &Neighbour);

  void FillOutput(const LineMapType &LineMap,
                  ProgressReporter &progress);

  void SetupLineOffsets(OffsetVec &LineOffsets);

  void Wait()
    {
    long nbOfThreads = this->GetNumberOfThreads();
    if( itk::MultiThreader::GetGlobalMaximumNumberOfThreads() != 0 )
      {
      nbOfThreads = std::min( this->GetNumberOfThreads(), itk::MultiThreader::GetGlobalMaximumNumberOfThreads() );
      }
    if( nbOfThreads > 1 )
      {
      m_Barrier->Wait();
      }
    }

  typename std::vector< long > m_NumberOfLabels;
  typename std::vector< long > m_FirstLineIdToJoin;
  typename Barrier::Pointer m_Barrier;
  LineMapType m_LineMap;
};
  
} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkBinaryImageToLabelMapFilter.txx"
#endif

#endif
