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

#include <malloc.h>
#include <cstdint>
#include <vector>
#include <iostream>
#include <stdexcept>

/**
 * Allocator for aligned data.
 *
 * Modified from the Mallocator from Stephan T. Lavavej.
 * <http://blogs.msdn.com/b/vcblog/archive/2008/08/28/the-mallocator.aspx>
 */
template <typename T, std::size_t Alignment>
class aligned_allocator
{
public:

  typedef T value_type;

  T * address(T& r) const
  {
    return &r;
  }

  const T * address(const T& s) const
  {
    return &s;
  }

  std::size_t max_size() const
  {
    // The following has been carefully written to be independent of
    // the definition of size_t and to avoid signed/unsigned warnings.
    return (static_cast<std::size_t>(0) - static_cast<std::size_t>(1)) / sizeof(T);
  }


  // The following must be the same for all allocators.
  template <typename U>
  struct rebind
  {
    typedef aligned_allocator<U, Alignment> other;
  };

  bool operator!=(const aligned_allocator& other) const
  {
    return !(*this == other);
  }

  // Returns true if and only if storage allocated from *this
  // can be deallocated from other, and vice versa.
  // Always returns true for stateless allocators.
  bool operator==(const aligned_allocator& other) const
  {
    return true;
  }


  // Default constructor, copy constructor, rebinding constructor, and destructor.
  // Empty for stateless allocators.
  aligned_allocator() { }

  aligned_allocator(const aligned_allocator&) { }

  template <typename U> aligned_allocator(const aligned_allocator<U, Alignment>&) { }

  ~aligned_allocator() { }


  // The following will be different for each allocator.
  T * allocate(const std::size_t n) const
  {
    // The return value of allocate(0) is unspecified.
    // Mallocator returns NULL in order to avoid depending
    // on malloc(0)'s implementation-defined behavior
    // (the implementation can define malloc(0) to return NULL,
    // in which case the bad_alloc check below would fire).
    // All allocators can return NULL in this case.
    if (n == 0) {
      return NULL;
    }

    // All allocators should contain an integer overflow check.
    // The Standardization Committee recommends that std::length_error
    // be thrown in the case of integer overflow.
    if (n > max_size())
    {
      throw std::length_error("aligned_allocator<T>::allocate() - Integer overflow.");
    }

    // Mallocator wraps malloc().
    void * const pv = memalign(Alignment, n * sizeof(T));

    // Allocators should throw std::bad_alloc in the case of memory allocation failure.
    if (pv == NULL)
    {
      throw std::bad_alloc();
    }

    return static_cast<T *>(pv);
  }

  void deallocate(T * const p, const std::size_t) const
  {
    free(p);
  }


  // The following will be the same for all allocators that ignore hints.
  template <typename U>
  T * allocate(const std::size_t n, const U * /* const hint */) const
  {
    return allocate(n);
  }
 
  aligned_allocator& operator=(const aligned_allocator&) = delete;
};
