/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPriorityQueue.h,v $
  Language:  C++
  Date:      $Date: 2005/01/21 20:13:31 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPriorityQueue_h
#define __itkPriorityQueue_h

// #include <vector>
#include <list>
#include <map>

namespace itk
{
/** \class PriorityQueue
 *  \brief PriorityQueue class
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
template <typename TKey, typename TValue, typename TCompare>
class PriorityQueue
{

public:

  /** Standard typedefs */
  typedef PriorityQueue      Self;

  typedef TValue ValueType;
  typedef TKey KeyType;
  typedef TCompare CompareType;

  typedef std::list<ValueType>      ValueListType;
  typedef std::map<KeyType, ValueListType, CompareType>  MapType;
//   typedef std::Vector<ValueListType>  VectorType;

  /** return the current key */
  inline const KeyType & FrontKey() const
    {
    return m_Map.begin()->first;
    }

  /** return the current value */
  inline const ValueType & FrontValue() const
    {
    return m_Map.begin()->second.front();
    }

  /** push a value in the queue */
  inline void Push( KeyType & k, ValueType & v)
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
    ValueListType & valueList = m_Map.begin()->second;
    valueList.pop_front();
    if( valueList.empty() )
      {
      m_Map.erase( m_Map.begin() );
      }
    m_Size--;
    }

  PriorityQueue()
    {
    m_Size = 0;
    }


protected:

private:

  MapType m_Map;
//   VactorType m_Vector;
  unsigned long m_Size;

};

} // end namespace itk

#endif
