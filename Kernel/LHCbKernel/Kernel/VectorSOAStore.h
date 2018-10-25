/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "AlignedAllocator.h"
#include "VectorConfiguration.h"
#include "assert.h"
#include "GaudiKernel/DataObject.h"

namespace LHCb {

namespace Vector {

namespace Mem {

/**
 * @brief      Store for a Vector SOA backend.
 *             Data is stored contiguously in std::vectors of the required
 *             precision, using an aligned allocator to guarantee vector alignment.
 *             New vectors can be requested with getNewVector. Once the size is exhausted,
 *             new contiguous backends are generated.
 *
 * @tparam     PRECISION  The data type of the backend.
 */
template<class PRECISION>
struct Store {
  typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>> backends;
  typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>>::iterator currentBackend;
  unsigned currentElement = 0;
  unsigned currentVector = 0;
  unsigned elementCapacity = 0;
  unsigned vectorCapacity = 0;
  size_t width = 21;

  Store () = default;

  /**
   * @brief      Constructs a Store for a datatype of
   *             "width" elements, with capacity for numberOfNodes.
   *
   * @param[in]  width          Width of stored elements.
   * @param[in]  numberOfNodes  Capacity of initial backend and successive backends.
   */
  inline Store (
    const size_t& width,
    const unsigned& numberOfNodes
  ) : width(width) {
    elementCapacity = numberOfNodes;
    backends.emplace_back(elementCapacity * width);
    vectorCapacity = elementCapacity / VectorConfiguration::width<PRECISION>();
    reset();
  }

  /**
   * @brief      Prints the state of the store.
   *
   * @param[in]  position  The position.
   */
  void printState (const std::string& position) {
    std::cout << "State at " << position << std::endl
      // << " currentBackend " << ((unsigned) (currentBackend - backends.begin()))
      << ", currentVector " << currentVector
      << ", currentElement " << currentElement << std::endl
      << " backend capacity " << backends.size()
      << ", element capacity " << elementCapacity
      << ", vector capacity " << vectorCapacity << std::endl;
  }

  /**
   * @brief      Resets the backends. The data is not erased, and the
   *             internal backends are not destroyed, but instead reused.
   */
  inline void reset () {
    currentBackend = backends.begin();
    currentVector = 0;
    currentElement = 0;
  }

  /**
   * @brief      Aligns to point to a new vector.
   */
  inline void align () {
    if (currentElement != 0) {
      currentElement = 0;
      ++currentVector;

      if (currentVector == vectorCapacity) {
        currentVector = 0;
        
        if (std::next(currentBackend) == backends.end()) {
          backends.emplace_back(elementCapacity * width);
        }
        
        ++currentBackend;
      }
    }
  }

  /**
   * @brief      Gets a pointer to a new vector. In case the
   *             current backend is exhausted, a new backend is generated.
   *
   * @return     The new vector.
   */
  inline PRECISION* getNewVector () {
    align();

    PRECISION* d = getPointer(currentBackend, currentVector);
    
    ++currentVector;
    if (currentVector == vectorCapacity) {
      currentVector = 0;

      if (std::next(currentBackend) == backends.end()) {
        backends.emplace_back(elementCapacity * width);
      }
      
      ++currentBackend;
    }

    return d;
  }

  /**
   * @brief      Gets a new element.
   *
   * @return     The next element.
   */
  inline PRECISION* getNextElement () {
    PRECISION* d = getPointer(currentBackend, currentVector, currentElement);

    ++currentElement;
    if (currentElement == VectorConfiguration::width<PRECISION>()) {
      ++currentVector;
      currentElement = 0;

      if (currentVector == vectorCapacity) {
        currentVector = 0;
        
        if (std::next(currentBackend) == backends.end()) {
          backends.emplace_back(elementCapacity * width);
        }
        
        ++currentBackend;
      }
    }

    return d;
  }

  /**
   * @brief      Gets a pointer to the last generated vector.
   *
   * @return     The last generated vector pointer.
   */
  inline PRECISION* getLastVector () {
    assert(currentVector!=0 || currentBackend!=backends.begin());

    if (currentVector == 0) {
      return getPointer(std::prev(currentBackend), vectorCapacity - 1);
    }
    return getPointer(currentBackend, currentVector - 1);
  }

  inline PRECISION* getFirstVector () {
    return backends.front().data();
  }

  /**
   * @brief      Gets a pointer to a backend / vector / element combination.
   *
   * @return     The pointer.
   */
  inline PRECISION* getPointer (
    const typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>>::iterator& backend,
    const unsigned& vector,
    const unsigned& element
  ) {
    assert(backend != backends.end() and vector < vectorCapacity and element < VectorConfiguration::width<PRECISION>());

    return backend->data() + vector * VectorConfiguration::width<PRECISION>() * width + element;
  }

  /**
   * @brief      Gets a pointer to a backend / vector combination.
   *
   * @return     The pointer.
   */
  inline PRECISION* getPointer (
    const typename std::list<std::vector<PRECISION, aligned_allocator<PRECISION, VectorConfiguration::bytewidth()>>>::iterator& backend,
    const unsigned& vector
  ) {
    return getPointer(backend, vector, 0);
  }
};

template<class PRECISION>
struct Stores : DataObject {
  std::unique_ptr<std::vector<Store<PRECISION>>> m_stores;

  Stores (std::unique_ptr<std::vector<Store<PRECISION>>> stores) : m_stores(std::move(stores)) {}
};

}

}

}
