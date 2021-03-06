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
#ifndef LHCB_CXX_SYNCHRONIZED_VALUE_H
#define LHCB_CXX_SYNCHRONIZED_VALUE_H
#include <functional>
#include <mutex>
#include <shared_mutex>
#include <tuple>
#include <type_traits>
#include <utility>

#include <boost/callable_traits.hpp>

namespace LHCb::cxx {

  namespace details {

    template <typename F, typename Value>
    using require_arg0_t =
        std::enable_if_t<std::is_same_v<std::tuple_element_t<0, boost::callable_traits::args_t<F>>, Value>>;

    template <typename Value, typename... Args>
    using require_constructible_t = std::enable_if_t<std::is_constructible_v<Value, Args...>>;

  } // namespace details

  // C++20: replace with http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0290r2.html
  //         http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2014/n4033.html

  template <typename Value, typename Mutex = std::mutex,
            typename ReadLock  = std::conditional_t<std::is_same_v<std::shared_mutex, Mutex>, std::shared_lock<Mutex>,
                                                   std::lock_guard<Mutex>>,
            typename WriteLock = std::lock_guard<Mutex>>
  class SynchronizedValue {
    static_assert( !std::is_reference_v<Value>, "Value must not be a reference" );
    Value         m_obj;
    mutable Mutex m_mtx;

  public:
    template <typename... Args, typename = details::require_constructible_t<Value, Args...>>
    SynchronizedValue( Args&&... args ) : m_obj{std::forward<Args>( args )...} {}

    template <typename F, typename... Args, typename = details::require_arg0_t<F, Value&>>
    decltype( auto ) with_lock( F&& f, Args&&... args ) {
      WriteLock _{m_mtx};
      return std::invoke( std::forward<F>( f ), m_obj, std::forward<Args>( args )... );
    }

    template <typename F, typename... Args, typename = details::require_arg0_t<F, const Value&>>
    decltype( auto ) with_lock( F&& f, Args&&... args ) const {
      ReadLock _{m_mtx};
      return std::invoke( std::forward<F>( f ), m_obj, std::forward<Args>( args )... );
    }
  };

  // transform an f(T,...) into an f(SynchronizedValue<T>,...)
  template <typename Fun>
  auto with_lock( Fun&& f ) {
    return [f = std::forward<Fun>( f )]( auto& p, auto&&... args ) -> decltype( auto ) {
      return p.with_lock( f, std::forward<decltype( args )>( args )... );
    };
  }
  // call f(T) for each element in a container of Synced<T>
  template <typename ContainerOfSynced, typename Fun>
  void for_each( ContainerOfSynced& c, Fun&& f ) {
    std::for_each( begin( c ), end( c ), with_lock( std::forward<Fun>( f ) ) );
  }

} // namespace LHCb::cxx
#endif
