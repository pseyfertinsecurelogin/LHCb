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
#include "EventContextExt.h"    // LHCb::getMemResource( EventContext const& )
#include "EventLocalResource.h" // Defines/documents LHCB_CUSTOM_ALLOCATOR_IMPL
#if LHCB_CUSTOM_ALLOCATOR_IMPL == 5
// TODO remove this once the default resource is nullptr in this case
#  include "GaudiKernel/ThreadLocalContext.h" // Gaudi::Hive::currentContext()
#endif
#include "STLExtensions.h"

// Includes for LHCb::make_span
#include <iterator>
#include <type_traits>
#include <vector>

/** The following block defines:
 *    LHCb::Allocators::MemoryResource* LHCb::defaultMemResource():
 *      Returns a handle to the default memory resource. This is called when an
 *      event-local allocator is default-constructed and needs a resource.
 *
 *    LHCb::Allocators::EventLocal<T>:
 *      A stateful allocator that uses an event-local memory resource. This can
 *      either be given directly or obtained from LHCb::defaultMemResource().
 */
namespace LHCb {
#if LHCB_CUSTOM_ALLOCATOR_IMPL == 1
  [[nodiscard]] inline Allocators::MemoryResource* defaultMemResource() {
    return boost::container::pmr::get_default_resource();
  }

  namespace Allocators {
    template <typename T>
    using EventLocal = boost::container::pmr::polymorphic_allocator<T>;
  } // namespace Allocators
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 2
  [[nodiscard]] inline Allocators::MemoryResource* defaultMemResource() { return std::pmr::get_default_resource(); }

  namespace Allocators {
    template <typename T>
    using EventLocal = std::pmr::polymorphic_allocator<T>;
  } // namespace Allocators
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 3
  [[nodiscard]] constexpr Allocators::MemoryResource* defaultMemResource() { return nullptr; }

  namespace Allocators {
    template <typename T>
    struct EventLocal {
      using value_type                             = T;
      using propagate_on_container_swap            = std::true_type;
      using propagate_on_container_copy_assignment = std::true_type;
      using propagate_on_container_move_assignment = std::true_type;

      constexpr EventLocal( MemoryResource* resource = defaultMemResource() ) noexcept : m_resource{resource} {}

      template <typename U>
      constexpr EventLocal( EventLocal<U> const& other ) noexcept : m_resource{other.m_resource} {}

      [[nodiscard]] __attribute__( ( always_inline ) ) T* allocate( std::size_t n ) {
        if ( LIKELY( m_resource != defaultMemResource() ) ) {
          return reinterpret_cast<T*>( m_resource->template allocate<alignof( T )>( n * sizeof( T ) ) );
        } else {
          return default_allocate( n );
        }
      }

      __attribute__( ( always_inline ) ) void deallocate( T* p, std::size_t n ) noexcept {
        if ( LIKELY( m_resource != defaultMemResource() ) ) {
          m_resource->deallocate( reinterpret_cast<std::byte*>( p ), n * sizeof( T ) );
        } else {
          default_deallocate( p, n );
        }
      }

      template <typename U>
      friend constexpr bool operator==( EventLocal const& lhs, EventLocal<U> const& rhs ) {
        return lhs.m_resource == rhs.m_resource;
      }

      [[nodiscard]] MemoryResource* resource() const noexcept { return m_resource; }

    private:
      [[nodiscard]] __attribute__( ( noinline ) ) T* default_allocate( std::size_t n ) {
        return std::allocator<T>{}.allocate( n );
      }

      __attribute__( ( noinline ) ) void default_deallocate( T* p, std::size_t n ) noexcept {
        std::allocator<T>{}.deallocate( p, n );
      }

      template <typename>
      friend struct EventLocal;

      MemoryResource* m_resource{defaultMemResource()}; //! transient (ROOT should not persist the value)
    };

    template <typename T, typename U>
    inline constexpr bool operator!=( EventLocal<T> const& lhs, EventLocal<U> const& rhs ) {
      return !( lhs == rhs );
    }
  } // namespace Allocators
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 4
  namespace Allocators {
    // Wraps std::allocator<T> but adds a constructor taking MemoryResource*
    template <typename T>
    struct EventLocal {
      using value_type       = T;
      constexpr EventLocal() = default;
      constexpr EventLocal( MemoryResource* ) noexcept {}
      template <typename U>
      constexpr EventLocal( EventLocal<U> const& ) noexcept {}
      [[nodiscard]] T* allocate( std::size_t n ) { return std::allocator<T>().allocate( n ); }
      void             deallocate( T* p, std::size_t n ) noexcept { std::allocator<T>().deallocate( p, n ); }
      [[nodiscard]] MemoryResource* resource() const noexcept { return nullptr; }
    };

    template <typename T, typename U>
    inline constexpr bool operator==( EventLocal<T> const&, EventLocal<U> const& ) {
      return true;
    }

    template <typename T, typename U>
    inline constexpr bool operator!=( EventLocal<T> const&, EventLocal<U> const& ) {
      return false;
    }
  } // namespace Allocators

  [[nodiscard]] inline Allocators::MemoryResource* defaultMemResource() {
    // The allocator is stateless and this value will just be discarded
    return nullptr;
  }
#elif LHCB_CUSTOM_ALLOCATOR_IMPL == 5
  [[nodiscard]] inline Allocators::MemoryResource* defaultMemResource() {
    // Get a memory resource from the thread-local EventContext
    return getMemResource( Gaudi::Hive::currentContext() );
  }

  namespace Allocators {
    namespace detail {
      struct DefaultResource {
        MemoryResource* operator()() { return defaultMemResource(); }
      };
    } // namespace detail

    // Depends on Gaudi!1026
    template <typename T>
    using EventLocal = Gaudi::Allocator::Arena<MemoryResource, T, detail::DefaultResource>;
  } // namespace Allocators
#endif

  // Fix make_span for iterators into std::vector<T, LHCb::Allocators::EventLocal<T>>
  namespace details_se {
    template <typename Iterator>
    constexpr bool is_contiguous_iterator() {
      using Value = typename std::iterator_traits<Iterator>::value_type;
      return (std::is_same_v<typename std::vector<Value, Allocators::EventLocal<Value>>::iterator, Iterator> ||
              std::is_same_v<typename std::vector<Value, Allocators::EventLocal<Value>>::const_iterator,
                             Iterator>)&&!std::is_same_v<bool, Value>;
    }
  } // namespace details_se

  template <typename Iterator, typename std::enable_if_t<details_se::is_contiguous_iterator<Iterator>(), int> = 0>
  [[nodiscard]] auto make_span( Iterator firstElem, Iterator lastElem ) {
    return make_span( firstElem.operator->(), std::distance( firstElem, lastElem ) );
  }
} // namespace LHCb
