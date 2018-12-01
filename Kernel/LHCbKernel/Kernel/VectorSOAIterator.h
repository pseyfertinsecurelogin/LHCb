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
#pragma once

#include <vector>
#include <list>
#include "VectorSOAStore.h"
#include "AlignedAllocator.h"
#include "VectorConfiguration.h"
#include "assert.h"

namespace LHCb {

namespace Vector {

namespace Mem {

/**
 * @brief      Vector SOA iterator.
 *             Helper class that allows iteration over memory stores.
 *             It is aware of the backend structure of LHCb::Vector::Mem::Store.
 *
 * @tparam     PRECISION  Precision of backend.
 */
template<class PRECISION>
struct Iterator {
  Store<PRECISION>* store = nullptr;
  typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>>::iterator backend;
  unsigned vector = 0;
  unsigned element = 0;
  unsigned vectorCapacity = 0;

  Iterator () = default;
  
  Iterator (Store<PRECISION>& storeParam)
    : store(&storeParam) {
    backend        = store->backends.begin();
    vectorCapacity = store->vectorCapacity;
  }

  Iterator (Store<PRECISION>& storeParam, const bool& fromCurrent)
    : store(&storeParam) {
    if (fromCurrent) {
      backend = store->currentBackend;
      vector  = store->currentVector;
      element = store->currentElement;
    } else {
      backend = store->backends.begin();
    }
    vectorCapacity = store->vectorCapacity;
  }

  friend inline std::ostream& operator<< (
    std::ostream& cout,
    const Iterator& iterator
  ) {
    if (iterator.store != nullptr) {
      cout << "Iterator to store " << &(*(iterator.store)) << " backend " << &(*(iterator.backend))
           << " vector " << iterator.vector << " element " << iterator.element << std::endl;
    } else {
      cout << "Uninitialized Iterator" << std::endl;
    }
    return cout;
  }

  /**
   * @brief      Gets the next Store vector,
   *             and updates the internal state
   *
   * @return     Pointer to next vector
   */
  PRECISION* nextVector () {
    assert(
      (store->currentVector==0) ?
      (backend != std::prev(store->currentBackend) or vector != (vectorCapacity)) :
      (backend != (store->currentBackend) or vector != (store->currentVector))
    );

    PRECISION* d = store->getPointer(backend, vector);

    element = 0;
    ++vector;
    if (vector == vectorCapacity) {
      vector = 0;
      ++backend;
    }

    return d;
  }

  PRECISION* nextElement () {
    assert(backend != (store->currentBackend) or vector != (store->currentVector) or element != (store->currentElement));

    PRECISION* d = store->getPointer(backend, vector, element);

    ++element;
    if (element == VectorConfiguration::width<PRECISION>()) {
      element = 0;
      ++vector;
      if (vector == vectorCapacity) {
        vector = 0;
        ++backend;
      }
    }

    return d;
  }
};

/**
 * @brief      Vector SOA reverse iterator.
 *             Helper class that allows iteration over memory stores.
 *             It is aware of the backend structure of LHCb::Vector::Mem::Store.
 *
 * @tparam     PRECISION  Precision of backend.
 */
template<class PRECISION>
struct ReverseIterator {
  Store<PRECISION>* store = nullptr;
  typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>>::iterator backend;
  unsigned vector = 0;
  unsigned vectorCapacity = 0;

  ReverseIterator () = default;

  ReverseIterator (Store<PRECISION>& storeParam)
    : store(&storeParam) {
    backend = store->currentBackend;
    vector = store->currentVector;
    vectorCapacity = store->vectorCapacity;
  }

  friend inline std::ostream& operator<< (
    std::ostream& cout,
    const ReverseIterator& iterator
  ) {
    if (iterator.store != nullptr) {
      cout << "ReverseIterator to store " << &(*(iterator.store)) << " backend " << &(*(iterator.backend))
           << " vector " << iterator.vector << std::endl;
    } else {
      cout << "Uninitialized ReverseIterator" << std::endl;
    }
    return cout;
  }

  /**
   * @brief      Gets the previous Store vector,
   *             and updates the internal state
   *
   * @return     Pointer to previous vector
   */
  PRECISION* previousVector () {
    assert(backend != store->backends.begin() or vector != 0);

    if (vector == 0) {
      --backend;
      vector = vectorCapacity;
    }

    return store->getPointer(backend, --vector);
  }
};

}

}

}
