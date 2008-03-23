/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkHierarchicalQueue.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkHierarchicalQueue_h
#define __itkHierarchicalQueue_h

// #include <vector>
#include <list>
#include <map>

namespace itk
{
/** \class HierarchicalQueue
 *  \brief HierarchicalQueue class
 * 
 * This class implement a priority queue based on list and map or list and vector,
 * depending on the key type. Image analysis are making a particular
 * usage of priority queue: there is a restricted set of keys, but a
 * very high number of values. This particularity make classical priority
 * queue implementations (with heap and vector or deque, like in STL) highly inefficient.
 * Also, some algorithm like morphological watershed are requiring that the
 * values are returned in the same order they have been pushed in the queue.
 * This class gives both better performances for image analysis, and ensure
 * the output order of the values.
 */
template <typename TKey, typename TValue, typename TCompare=typename std::less<TKey> >
class HierarchicalQueue
{

public:

  /** Standard typedefs */
  typedef HierarchicalQueue      Self;

  typedef TValue ValueType;
  typedef TKey KeyType;
  typedef TCompare CompareType;

  typedef std::list<ValueType>      ValueListType;
  typedef std::map<KeyType, ValueListType, CompareType>  MapType;
//   typedef std::Vector<ValueListType>  VectorType;

  /** return the current key */
  inline const KeyType & FrontKey() const
    {
    assert(!this->Empty());
    return m_Map.begin()->first;
    }

  /** return the current value */
  inline const ValueType & FrontValue() const
    {
    assert(!this->Empty());
    return m_Map.begin()->second.front();
    }

  /** push a value in the queue */
  inline void Push( const KeyType & k, const ValueType & v)
    {
    m_Map[k].push_back( v );
    m_Size++;
    }

  /** return the size of the queue */
  inline const unsigned long & Size() const
    {
    return m_Size;
    }

  /** return true if the queue is empty */
  inline const bool Empty() const
    {
    return m_Size == 0;
    }

  /** remove the first element of the queue */
  inline void Pop()
    {
    assert(!this->Empty());
    ValueListType & valueList = m_Map.begin()->second;
    valueList.pop_front();
    if( valueList.empty() )
      {
      m_Map.erase( m_Map.begin() );
      }
    m_Size--;
    }

  HierarchicalQueue()
    {
    m_Size = 0;
    }


protected:

private:

  MapType m_Map;
//   VactorType m_Vector;
  unsigned long m_Size;

};



template <typename TKey, typename TValue, typename TCompare >
class VectorHierarchicalQueue
{

public:

  /** Standard typedefs */
  typedef VectorHierarchicalQueue      Self;

  typedef TValue ValueType;
  typedef TKey KeyType;
  typedef TCompare CompareType;

  typedef std::list<ValueType>      ValueListType;
  typedef std::vector<ValueListType>  VectorType;

  // for code conciseness
  typedef NumericTraits< TKey > NT;

  /** return the current key */
  inline const KeyType & FrontKey() const
    {
    assert(!this->Empty());
    return m_CurrentValue;
    }

  /** return the current value */
  inline const ValueType & FrontValue() const
    {
    assert(!this->Empty());
    return m_Vector[ m_CurrentValue  - NT::NonpositiveMin() ].front();
    }

  /** push a value in the queue */
  inline void Push( const KeyType & k, const ValueType & v)
    {
      assert( (int)(k  - NT::NonpositiveMin()) < (int)m_Vector.size() );
    assert( k  - NT::NonpositiveMin() >= 0 );

    m_Vector[ k  - NT::NonpositiveMin() ].push_back( v );
    if( this->Empty() || m_Compare( k, m_CurrentValue ) )
      {
      m_CurrentValue = k;
      }
    m_Size++;
    }

  /** return the size of the queue */
  inline const unsigned long & Size() const
    {
    return m_Size;
    }

  /** return true if the queue is empty */
  inline const bool Empty() const
    {
    return m_Size == 0;
    }

  /** remove the first element of the queue */
  inline void Pop()
    {
    assert(!this->Empty());
    ValueListType & valueList = m_Vector[ m_CurrentValue  - NT::NonpositiveMin() ];
    valueList.pop_front();
    m_Size--;

    if( valueList.empty() && !this->Empty() )
      {
      // update the current key to a new value
      while( m_Vector[ m_CurrentValue - NT::NonpositiveMin() ].empty() )
        {
        m_CurrentValue += m_Direction;
        }
      }

    }

  VectorHierarchicalQueue()
    {
    m_Vector.resize( NT::max() - NT::NonpositiveMin() + 1 );
    if( m_Compare( NT::max(), NT::NonpositiveMin() ) )
      {
      m_Direction = -1;
      }
    else
      {
      m_Direction = 1;
      }
    m_Size = 0;
    // initialized to make valgrind happy
    m_CurrentValue = 0;

    assert( m_Vector.size() != 0 );
    }


protected:

private:

  VectorType m_Vector;
  unsigned long m_Size;
  TKey m_CurrentValue;
  TCompare m_Compare;
  signed int m_Direction;

};

template <typename TValue, typename TCompare >
class HierarchicalQueue<unsigned char, TValue, TCompare>
: public VectorHierarchicalQueue<unsigned char, TValue, TCompare>
{
};

template <typename TValue, typename TCompare >
class HierarchicalQueue<unsigned short, TValue, TCompare>
: public VectorHierarchicalQueue<unsigned short, TValue, TCompare>
{
};

template <typename TValue, typename TCompare >
class HierarchicalQueue<signed char, TValue, TCompare>
: public VectorHierarchicalQueue<signed char, TValue, TCompare>
{
};

template <typename TValue, typename TCompare >
class HierarchicalQueue<signed short, TValue, TCompare>
: public VectorHierarchicalQueue<signed short, TValue, TCompare>
{
};

template <typename TValue, typename TCompare >
class HierarchicalQueue<bool, TValue, TCompare>
: public VectorHierarchicalQueue<bool, TValue, TCompare>
{
};


} // end namespace itk

#endif
