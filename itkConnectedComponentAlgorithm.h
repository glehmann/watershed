#ifndef __itkConnectedComponentAlgorithm_h
#define __itkConnectedComponentAlgorithm_h

#include "itkImage.h"
#include "itkConstShapedNeighborhoodIterator.h"
#include "itkShapedNeighborhoodIterator.h"

namespace itk
{
template< class TIterator >
TIterator*
setConnectivity( TIterator* it, bool fullyConnected=false )
{
  typename TIterator::OffsetType offset;
  if( !fullyConnected) 
    {
    // only activate the neighbors that are face connected
    // to the current pixel. do not include the center pixel
    offset.Fill( 0 );
    for( unsigned int d=0; d < TIterator::Dimension; ++d )
      {
      offset[d] = -1;
      it->ActivateOffset( offset );
      offset[d] = 1;
      it->ActivateOffset( offset );
      offset[d] = 0;
      }
    }
  else
    {
    // activate all neighbors that are face+edge+vertex
    // connected to the current pixel. do not include the center pixel
    unsigned int centerIndex = it->GetCenterNeighborhoodIndex();
    for( unsigned int d=0; d < centerIndex*2 + 1; d++ )
      {
      offset = it->GetOffset( d );
      it->ActivateOffset( offset );
      }
    offset.Fill(0);
    it->DeactivateOffset( offset );
    }
  return it;
}

}


#endif
