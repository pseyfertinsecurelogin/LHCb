/*****************************************************************************\
* (c) Copyright 2019-20 CERN for the benefit of the LHCb Collaboration        *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once
/** Set which allocator implementation we use. The options are:
 *  1. Polymorphic allocators from Boost (should be OK for gcc+clang)
 *  2. Polymorphic allocators from the STL (only OK for gcc as of 12/2019)
 *  3. Custom non-polymorphic solution. Has no virtual calls and deallocations
 *     can be inlined as [probable] no-ops.
 *  4. Custom non-polymorphic solution that is a very thin wrapper around
 *     std::allocator. This should generate almost equivalent code to not using
 *     custom allocators at all. One source of differences will be that
 *     LHCb-specific types do not do any layout optimisation for the case that
 *     the allocator is stateless.
 *  5. Custom non-polymorphic solution. No virtual calls and deallocations can
 *     be inlined as definite no-ops.
 */
#define LHCB_CUSTOM_ALLOCATOR_IMPL 3
#if LHCB_CUSTOM_ALLOCATOR_IMPL == 1
#  include <boost/container/pmr/global_resource.hpp>
#  include <boost/container/pmr/memory_resource.hpp>
#  include <boost/container/pmr/monotonic_buffer_resource.hpp>
#  include <boost/container/pmr/polymorphic_allocator.hpp>
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 2
#  include <memory_resource>
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 3
#  include "Kernel/MonotonicArena.h"
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 4
#  include <memory>
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 5
#  include "Gaudi/Arena/Monotonic.h"
#else
#  error "Unknown LHCB_CUSTOM_ALLOCATOR_IMPL value"
#endif

/** The following block defines:
 *    LHCb::Allocators::MemoryResource:
 *      A [potentially] abstract [base] class representing an event-local
 *      memory resource. MemoryResource* can be obtained from the EventContext.
 *
 *    LHCb::Allocators::MonotonicBufferResource:
 *      A concrete type representing a memory resource. Provides a constructor
 *      taking a std::size_t representing an estimate of the required capacity
 *      of the resource.
 *      MonotonicBufferResource* is convertible to MemoryResource*.
 */
namespace LHCb::Allocators {
  namespace Utils {
    template <typename>
    struct provides_stats : std::false_type {};

    template <typename T>
    inline constexpr bool provides_stats_v = provides_stats<T>::value;
  } // namespace Utils
#if LHCB_CUSTOM_ALLOCATOR_IMPL == 1
  using MemoryResource          = boost::container::pmr::memory_resource;
  using MonotonicBufferResource = boost::container::pmr::monotonic_buffer_resource;
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 2
  using MemoryResource          = std::pmr::memory_resource;
  using MonotonicBufferResource = std::pmr::monotonic_buffer_resource;
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 3
  using MemoryResource          = LHCb::Arena::Monotonic<64 /* alignment */>;
  using MonotonicBufferResource = MemoryResource;
  namespace Utils {
    template <>
    struct provides_stats<MonotonicBufferResource> : std::true_type {};
  } // namespace Utils
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 4
  // Dummy monotonic-buffer-resource-like implementation -- use as both the interface and impl.
  struct MemoryResource {
    constexpr MemoryResource( std::size_t ) noexcept {}
  };
  using MonotonicBufferResource = MemoryResource;
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 5
  using MemoryResource          = Gaudi::Arena::Monotonic<64 /* alignment */>;
  using MonotonicBufferResource = MemoryResource;
  namespace Utils {
    template <>
    struct provides_stats<MonotonicBufferResource> : std::true_type {};
  } // namespace Utils
#endif
} // namespace LHCb::Allocators
