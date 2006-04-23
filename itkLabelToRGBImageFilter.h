#ifndef __itkLabelToRGBImageFilter_h
#define __itkLabelToRGBImageFilter_h

#include "itkUnaryFunctorImageFilter.h"

namespace itk
{

namespace Functor {  
 
template< class TLabel, class TRGBPixel >
class LabelToRGBFunctor
{
public:
  LabelToRGBFunctor()
    {
    
    TRGBPixel rgbPixel;
    // the following colors are from "R", and named:
    // "red"             "green3"          "blue"            "cyan"           
    //"magenta"         "darkorange1"     "darkgreen"       "blueviolet"     
    //"brown4"          "navy"            "yellow4"         "violetred1"     
    //"salmon4"         "turquoise4"      "sienna3"         "darkorchid1"    
    //"springgreen4"    "mediumvioletred" "orangered3"      "lightseagreen"  
    //"slateblue"       "deeppink1"       "aquamarine4"     "royalblue1"     
    //"tomato3"         "mediumblue"      "violetred4"      "darkmagenta"    
    //"violet"          "red4"           
    // They are a good selection of distinct colours for plotting and
    // overlays.
    
    addColor( 255, 0, 0 );
    addColor( 0, 205, 0 );
    addColor( 0, 0, 255 );
    addColor( 0, 255, 255 );
    addColor( 255, 0, 255 );
    addColor( 255, 127, 0 );
    addColor( 0, 100, 0 );
    addColor( 138, 43, 226 );
    addColor( 139, 35, 35 );
    addColor( 0, 0, 128 );
    addColor( 139, 139, 0 );
    addColor( 255, 62, 150 );
    addColor( 139, 76, 57 );
    addColor( 0, 134, 139 );
    addColor( 205, 104, 57 );
    addColor( 191, 62, 255 );
    addColor( 0, 139, 69 );
    addColor( 199, 21, 133 );
    addColor( 205, 55, 0 );
    addColor( 32, 178, 170 );
    addColor( 106, 90, 205 );
    addColor( 255, 20, 147 );
    addColor( 69, 139, 116 );
    addColor( 72, 118, 255 );
    addColor( 205, 79, 57 );
    addColor( 0, 0, 205 );
    addColor( 139, 34, 82 );
    addColor( 139, 0, 139 );
    addColor( 238, 130, 238 );
    addColor( 139, 0, 0 );
    }

  inline TRGBPixel operator()( const TLabel & p)
    {
    return m_Colors[ p % m_Colors.size()];
    }

  void addColor(unsigned char r, unsigned char g, unsigned char b)
    {
    TRGBPixel rgbPixel;
    typename TRGBPixel::ValueType m = itk::NumericTraits< typename TRGBPixel::ValueType >::max();
    rgbPixel.Set( static_cast< typename TRGBPixel::ValueType >( static_cast< double >( r ) / 255 * m ),
                  static_cast< typename TRGBPixel::ValueType >( static_cast< double >( g ) / 255 * m ),
                  static_cast< typename TRGBPixel::ValueType >( static_cast< double >( b ) / 255 * m ) );
    m_Colors.push_back( rgbPixel );
    }

  bool operator != (const LabelToRGBFunctor &l) const
  { return true; }

  ~LabelToRGBFunctor() {}

  std::vector< TRGBPixel > m_Colors;
};
}  // end namespace functor


/** \class LabelToRGBImageFilter
 * \brief Apply a colormap to a label image and put it on top of the input image
 *
 * Apply a colormap to a label image and put it on top of the input image. The set of colors
 * is a good selection of distinct colors. The opacity of the label image
 * can be defined by the user.
 *
 * \author Gaëtan Lehmann. Biologie du Développement et de la Reproduction, INRA de Jouy-en-Josas, France.
 * \author Richard Beare. Department of Medicine, Monash University, Melbourne, Australia.
 *
 * \sa ScalarToRGBPixelFunctor
 * \ingroup Multithreaded
 *
 */
template <typename  TInputImage, class TLabelImage, typename  TOutputImage>
class ITK_EXPORT LabelToRGBImageFilter :
    public
UnaryFunctorImageFilter<TLabelImage, TOutputImage, 
                        Functor::LabelToRGBFunctor< 
  typename TLabelImage::PixelType, 
  typename TOutputImage::PixelType>   >
{
public:
  /** Standard class typedefs. */
  typedef LabelToRGBImageFilter  Self;
  typedef UnaryFunctorImageFilter<TLabelImage, TOutputImage, 
                        Functor::LabelToRGBFunctor< 
                            typename TLabelImage::PixelType, 
                            typename TOutputImage::PixelType>   >  Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  typedef TOutputImage OutputImageType;
  typedef TLabelImage  LabelImageType;
  typedef typename TOutputImage::PixelType OutputPixelType;
  typedef typename TLabelImage::PixelType  LabelPixelType;

  /** Runtime information support. */
  itkTypeMacro(LabelToRGBImageFilter, UnaryFunctorImageFilter);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
protected:
  LabelToRGBImageFilter() {};
  virtual ~LabelToRGBImageFilter() {};

private:
  LabelToRGBImageFilter(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};


  
} // end namespace itk
  
#endif

