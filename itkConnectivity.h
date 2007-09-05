#ifndef itkConnectivity_h
#define itkConnectivity_h

#include <itkObject.h>
#include <itkIndex.h>
#include <itkOffset.h>
#include <itkImage.h>
#include <vector>

namespace itk
{

/**
 * @brief Connectivity information.
 *
 * This class describes the k-neighbors of a point in a digital space of 
 * dimension n.
 * The definition used for this connectivity is the one using cell 
 * decomposition, as defined by Malandain in "On Topology in Multidimensional 
 * Discrete Spaces", ftp://ftp.inria.fr/INRIA/publication/RR/RR-2098.ps.gz
 *
 * The advantage of this definition instead of the 4- or 8-connectivity (in 2D)
 * or 6-, 18- and 26-connectivity (in 3D) is that it is consistent and 
 * extensible in n-dimensions.
 *
 * In 2D, 4- and 8-connectivity are respectively corresponding to 1- and
 * 0-connectivities. In 3D, 6-, 18- and 26-connectivity are respectively
 * corresponding to 2-, 1- and 0-connectivity.
 */
template<unsigned int VDimension>
class ITK_EXPORT Connectivity  : public LightObject
{
public :

  /// Standard typedefs
  typedef Connectivity Self;
  typedef LightObject         Superclass;
  typedef SmartPointer<Self>  Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(Connectivity, LightObject);

  /// @brief The dimension of the space.
  itkStaticConstMacro(Dimension, unsigned int, VDimension);
    
  /// @brief Type for a point in n-D.
  typedef Offset< Dimension >  OffsetType;
  typedef Index< Dimension >   IndexType;

  typedef std::vector< OffsetType > OffsetContainerType;
  
  /// @brief Test if two points are neighbors
  bool AreNeighbors(IndexType const & p1, IndexType const & p2) const;
  
  /// @brief Test if a point is a neighbor of 0
  bool IsInNeighborhood( OffsetType const & p ) const;

  const OffsetContainerType & GetNeighbors() const;

  static int GetNeighborhoodSize();


  /** Set/Get the cell dimension. The FullyConnected methods are their
    * to make easier the implementation of the backward compatibility,
    * and the manipulation of the cell dimension.
    * SetGetNumberOfNeighbors() allow an more usual way to access the connectivity
    * (8-connected, 26-connected, ...) but dependent of the image dimension.
    */
  void SetCellDimension( int dim );

  const int & GetCellDimension() const;

  void SetFullyConnected( bool value );

  bool GetFullyConnected() const;

  void SetNumberOfNeighbors( int );

  int GetNumberOfNeighbors() const;

  /** helper methods */
  static int factorial( int );

  static OffsetType IntToOffset( int );

  static int OffsetToInt( const OffsetType & offset );

  static int ComputeNumberOfNeighbors( int cellDimension );


  /** Set/Get the global default cell dimension */
  static const int & GetGlobalDefaultCellDimension();

  static void SetGlobalDefaultCellDimension( int nb );

  static void SetGlobalDefaultFullyConnected( bool value );

  static bool GetGlobalDefaultFullyConnected();

  static void SetGlobalDefaultNumberOfNeighbors( int );

  static int GetGlobalDefaultNumberOfNeighbors();

  /** creation of equivalent iterators */
  template < class IteratorType >
  IteratorType Iterator( typename IteratorType::ImageType * img, const typename IteratorType::RegionType & region ) const
    {
    typename IteratorType::SizeType radius;
    radius.Fill( 1 );
    IteratorType it( radius, img, region );
    for( int i=0; i<m_Neighbors.size(); i++ )
      {
      it.ActivateOffset( m_Neighbors[i] );
      }
    return it;
    }

  template < class ConstIteratorType >
  ConstIteratorType ConstIterator( const typename ConstIteratorType::ImageType * img, const typename ConstIteratorType::RegionType & region ) const
    {
    typename ConstIteratorType::SizeType radius;
    radius.Fill( 1 );
    ConstIteratorType it( radius, img, region );
    for( int i=0; i<m_Neighbors.size(); i++ )
      {
      it.ActivateOffset( m_Neighbors[i] );
      }
    return it;
    }


  template < class IteratorType >
  IteratorType LaterIterator( typename IteratorType::ImageType * img, const typename IteratorType::RegionType & region ) const
    {
    typename IteratorType::SizeType radius;
    radius.Fill( 1 );
    IteratorType it( radius, img, region );
    unsigned int centerIndex = it.GetCenterNeighborhoodIndex();
    OffsetType offset;
    for( unsigned int d=centerIndex+1; d < 2*centerIndex+1; d++ )
      {
      offset = it.GetOffset( d );
      if( this->IsInNeighborhood( offset ) )
        {
        it.ActivateOffset( offset );
        }
      }
    offset.Fill(0);
    it.DeactivateOffset( offset );
    return it;
    }

  template < class ConstIteratorType >
  ConstIteratorType ConstLaterIterator( const typename ConstIteratorType::ImageType * img, const typename ConstIteratorType::RegionType & region ) const
    {
    typename ConstIteratorType::SizeType radius;
    radius.Fill( 1 );
    ConstIteratorType it( radius, img, region );
    unsigned int centerIndex = it.GetCenterNeighborhoodIndex();
    OffsetType offset;
    for( unsigned int d=centerIndex+1; d < 2*centerIndex+1; d++ )
      {
      offset = it.GetOffset( d );
      if( this->IsInNeighborhood( offset ) )
        {
        it.ActivateOffset( offset );
        }
      }
    offset.Fill(0);
    it.DeactivateOffset( offset );
    return it;
    }


  template < class IteratorType >
  IteratorType PreviousIterator( typename IteratorType::ImageType * img, const typename IteratorType::RegionType & region ) const
    {
    typename IteratorType::SizeType radius;
    radius.Fill( 1 );
    IteratorType it( radius, img, region );
    unsigned int centerIndex = it.GetCenterNeighborhoodIndex();
    OffsetType offset;
    for( unsigned int d=0; d < centerIndex; d++ )
      {
      offset = it.GetOffset( d );
      if( this->IsInNeighborhood( offset ) )
        {
        it.ActivateOffset( offset );
        }
      }
    offset.Fill(0);
    it.DeactivateOffset( offset );
    return it;
    }

  template < class ConstIteratorType >
  ConstIteratorType ConstPreviousIterator( const typename ConstIteratorType::ImageType * img, const typename ConstIteratorType::RegionType & region ) const
    {
    typename ConstIteratorType::SizeType radius;
    radius.Fill( 1 );
    ConstIteratorType it( radius, img, region );
    unsigned int centerIndex = it.GetCenterNeighborhoodIndex();
    OffsetType offset;
    for( unsigned int d=0; d < centerIndex; d++ )
      {
      offset = it.GetOffset( d );
      if( this->IsInNeighborhood( offset ) )
        {
        it.ActivateOffset( offset );
        }
      }
    offset.Fill(0);
    it.DeactivateOffset( offset );
    return it;
    }


protected:

  void PrintSelf(std::ostream& os, Indent indent) const;


private :
  // Purposedly not implemeted
  Connectivity(Self const & other);
  Connectivity & operator=(Self const & other);
  
  Connectivity();
  ~Connectivity() {};
  
  /// @brief Neighbors.
  OffsetContainerType m_Neighbors;

  int m_CellDimension;

  static int m_GlobalDefaultCellDimension;
};

}

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConnectivity.txx"
#endif

#endif // itkConnectivity_h
