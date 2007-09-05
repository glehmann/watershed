#ifndef itkConnectivity_txx
#define itkConnectivity_txx

#include "itkConnectivity.h"

namespace itk
{

template<unsigned int VDimension>
int
Connectivity<VDimension>
::m_GlobalDefaultCellDimension = VDimension - 1;


template<unsigned int VDimension>
Connectivity<VDimension>
::Connectivity()
{
  this->SetCellDimension( m_GlobalDefaultCellDimension );
}


template<unsigned int VDimension>
bool
Connectivity<VDimension>
::AreNeighbors(IndexType const & p1, IndexType const & p2) const
{
  OffsetType diff;
  for(unsigned int i=0; i<VDimension; ++i)
    {
    diff[i] = p1[i] - p2[i];
    }
  
  return this->IsInNeighborhood( diff );
}


template<unsigned int VDimension>
bool 
Connectivity<VDimension>
::IsInNeighborhood(OffsetType const & o) const
{
  typename OffsetContainerType::const_iterator iterator = 
    std::find(m_Neighbors.begin(), m_Neighbors.end(), o);
  return (iterator != m_Neighbors.end());
}


template<unsigned int VDimension>
const typename Connectivity<VDimension>::OffsetContainerType &
Connectivity<VDimension>
::GetNeighbors() const
{
  return m_Neighbors;
}


template<unsigned int VDimension>
int
Connectivity<VDimension>
::GetNeighborhoodSize()
{
  return static_cast<int>(vcl_pow( 3.0, static_cast<double>(VDimension) ) );
}


template<unsigned int VDimension>
int
Connectivity<VDimension>
::GetNumberOfNeighbors() const
{
  return m_Neighbors.size();
}


template<unsigned int VDimension>
void
Connectivity<VDimension>
::SetNumberOfNeighbors( int nb )
{
  // and search a matching cell dimension. It should be quite
  // fast with usual dimensions.
  bool found = false;
  for( int i=0; i<=VDimension && !found; i++ )
    {
    if( ComputeNumberOfNeighbors( i ) == nb )
      {
      this->SetCellDimension( i );
      found = true;
      }
    }
  if( !found )
    {
    // send an exception
    itkExceptionMacro( << nb << " is not a valid number of neighbors for dimension " << VDimension << ".");
    }
}


template<unsigned int VDimension>
int 
Connectivity<VDimension>
::ComputeNumberOfNeighbors( int cellDimension )
{
  int numberOfNeighbors = 0;
  for(unsigned int i = cellDimension; i <= VDimension-1; ++i)
    {
    numberOfNeighbors += 
      factorial(VDimension)/(factorial(VDimension-i)*factorial(i)) * 1<<(VDimension-i);
    }
  
  return numberOfNeighbors;
}


template<unsigned int VDimension>
void 
Connectivity<VDimension>
::SetCellDimension( int dim )
{
  // check the validity of the requested cell dimension
  if( dim < 0 || dim > VDimension )
    {
    itkExceptionMacro( << dim << " is not a valid cell dimension for dimension " << VDimension << "." );
    }

  m_CellDimension = dim;

  m_Neighbors.clear();
  m_Neighbors.reserve( ComputeNumberOfNeighbors( dim ) );

  int neighborhoodSize = this->GetNeighborhoodSize();
  for(int i=0; i< neighborhoodSize; ++i)
    {
    OffsetType const offset = IntToOffset( i );
        
    unsigned int numberOfZeros = 0;
    for( int j=0; j<Dimension; j++ )
      {
      if( offset[j] == 0 )
        {
        numberOfZeros++;
        }
      }
    
    if( numberOfZeros!=VDimension && numberOfZeros >= m_CellDimension)
      {
      m_Neighbors.push_back( offset );
      }
    }
}


template<unsigned int VDimension>
const int &
Connectivity<VDimension>
::GetCellDimension() const
{
  return m_CellDimension;
}


template<unsigned int VDimension>
void 
Connectivity<VDimension>
::SetFullyConnected( bool value )
{
  if( value )
    {
    this->SetCellDimension( 0 );
    }
  else
    {
    this->SetCellDimension( Dimension - 1 );
    }
}


template<unsigned int VDimension>
bool 
Connectivity<VDimension>
::GetFullyConnected() const
{
  if( m_CellDimension == 0 )
    {
    return true;
    }
  // not really true, but return false otherwise
  return false;
}


template<unsigned int VDimension>
typename Connectivity<VDimension>::OffsetType
Connectivity<VDimension>
::IntToOffset( int i )
{
  OffsetType o;
  
  for( int d=0; d<Dimension; d++ )
    {
    o[d] = i % 3;
    i -= o[d];
    i /= 3;
    --o[d];
    }
  
  return o;
}


template<unsigned int VDimension>
int
Connectivity<VDimension>
::OffsetToInt( const OffsetType & o )
{
  int i=0;
  int factor=1;
  for(unsigned int d=0; d<Dimension; ++d)
    {
    d += factor * (o[d]+1);
    factor *= 3;
    }
  
  return i;
}


template<unsigned int VDimension>
int
Connectivity<VDimension>
::factorial( int n )
  {
  if( n<=1 )
    {
    return 1;
    }
  return n * factorial( n-1 );
  }


template<unsigned int VDimension>
void 
Connectivity<VDimension>
::SetGlobalDefaultCellDimension( int dim )
{
  // check the validity of the requested cell dimension
  if( dim < 0 || dim > VDimension )
    {
    itkGenericExceptionMacro( << dim << " is not a valid cell dimension for dimension " << VDimension << "." );
    }

  m_GlobalDefaultCellDimension = dim;
}


template<unsigned int VDimension>
const int &
Connectivity<VDimension>
::GetGlobalDefaultCellDimension()
{
  return m_GlobalDefaultCellDimension;
}


template<unsigned int VDimension>
void 
Connectivity<VDimension>
::SetGlobalDefaultFullyConnected( bool value )
{
  if( value )
    {
    m_GlobalDefaultCellDimension = 0;
    }
  else
    {
    m_GlobalDefaultCellDimension = Dimension - 1;
    }
}


template<unsigned int VDimension>
bool 
Connectivity<VDimension>
::GetGlobalDefaultFullyConnected()
{
  if( m_GlobalDefaultCellDimension == 0 )
    {
    return true;
    }
  // not really true, but return false otherwise
  return false;
}


template<unsigned int VDimension>
int
Connectivity<VDimension>
::GetGlobalDefaultNumberOfNeighbors()
{
  return ComputeNumberOfNeighbors( m_GlobalDefaultCellDimension );
}


template<unsigned int VDimension>
void
Connectivity<VDimension>
::SetGlobalDefaultNumberOfNeighbors( int nb )
{
  // and search a matching cell dimension. It should be quite
  // fast with usual dimensions.
  bool found = false;
  for( int i=0; i<=VDimension && !found; i++ )
    {
    if( ComputeNumberOfNeighbors( i ) == nb )
      {
      SetGlobalDefaultCellDimension( i );
      found = true;
      }
    }
  if( !found )
    {
    // send an exception
    itkGenericExceptionMacro( << nb << " is not a valid number of neighbors for dimension " << VDimension << ".");
    }
}


template<unsigned int VDimension>
void 
Connectivity<VDimension>
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf( os, indent );
  os << indent << "CellDimension: " << m_CellDimension << std::endl;
  os << indent << "NumberOfNeighbors: " << this->GetNumberOfNeighbors() << std::endl;
  os << indent << "FullyConnected: " << this->GetFullyConnected() << std::endl;
  os << indent << "Neighbors: [ ";
  for( int i=0; i<m_Neighbors.size(); i++ )
    {
    os << m_Neighbors[i] << " ";
    }
  os << "]" << std::endl;
  os << indent << "GlobalDefaultCellDimension: " << m_GlobalDefaultCellDimension << std::endl;
  os << indent << "GlobalDefaultNumberOfNeighbors: " << GetGlobalDefaultNumberOfNeighbors() << std::endl;
  os << indent << "GlobalDefaultFullyConnected: " << GetGlobalDefaultFullyConnected() << std::endl;

}



};
#endif // itkConnectivity_txx
