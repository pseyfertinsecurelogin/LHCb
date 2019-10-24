/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef KERNEL_INSTANCECOUNTER_H
#define KERNEL_INSTANCECOUNTER_H 1

// Include files
#include "GaudiKernel/System.h"
#include <iostream>

/** @class InstanceCounter InstanceCounter.h Kernel/InstanceCounter.h
 *  Static class used to instrument constructors and destructors to search
 *  for memory leaks
 *
 *  @author Markus Frank
 *  @date   2002-07-17
 */

template <class T>
class InstanceCounter {
  long m_count{0};

public:
  long               increment() { return ++m_count; }
  long               decrement() { return --m_count; }
  [[nodiscard]] long count() const { return m_count; }
  InstanceCounter() {}
  virtual ~InstanceCounter() {
    if ( 0 != m_count ) {
      std::cout << "Number of objects of type: " << System::typeinfoName( typeid( T ) )
                << " created, but not destroyed:" << m_count << std::endl;
    }
  }
};
#endif // KERNEL_INSTANCECOUNTER_H
