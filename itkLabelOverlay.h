#ifndef __itkLabelOverlay_h
#define __itkLabelOverlay_h

template< class TInputPixel, class TLabel, class TRGBPixel >
class LabelOverlay
{
public:
  LabelOverlay()
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
    
    rgbPixel.Set(255, 0, 0);
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set(0, 205, 0);
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 0 , 255 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 255 , 255 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 255 , 0 , 255 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 255 , 127 , 0 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 100 , 0 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 138 , 43 , 226 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 35 , 35 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 0 , 128 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 139 , 0 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 255 , 62 , 150 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 76 , 57 );
    m_Colors.push_back( rgbPixel );
    rgbPixel.Set( 0 , 134 , 139 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 205 , 104 , 57 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 191 , 62 , 255 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 139 , 69 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 199 , 21 , 133 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 205 , 55 , 0 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 32 , 178 , 170 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 106 , 90 , 205 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 255 , 20 , 147 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 69 , 139 , 116 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 72 , 118 , 255 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 205 , 79 , 57 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 0 , 0 , 205 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 34 , 82 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 0 , 139 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 238 , 130 , 238 );
    m_Colors.push_back( rgbPixel );

    rgbPixel.Set( 139 , 0 , 0 );
    m_Colors.push_back( rgbPixel );

  }
  ~LabelOverlay() {}
  inline TRGBPixel operator()(  const TInputPixel & p1,
				const TLabel & p2)
  {
    if( p2 == itk::NumericTraits< TLabel >::Zero )
      {
      typename TRGBPixel::ValueType p = static_cast< typename TRGBPixel::ValueType >( p1 );
      TRGBPixel rgbPixel;
      rgbPixel.Set( p, p, p );
      return rgbPixel;
      }
    return m_Colors[ p2 % m_Colors.size() ];
  }
  bool operator != (const LabelOverlay&) const
  {
    return false;
  }
  std::vector< TRGBPixel > m_Colors;
};

#endif

