/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include "DetDesc/ConditionContext.h"
#include "DetDesc/ConditionKey.h"
#include "DetDesc/ParamValidDataObject.h"

#include "GaudiKernel/EventContext.h"
#include "GaudiKernel/Property.h"

#include "GaudiAlg/FixTESPath.h"
#include "GaudiAlg/FunctionalUtilities.h"

#include <string>
#include <type_traits>

namespace LHCb::DetDesc {
  template <typename Base>
  class ConditionAccessorHolder;

  namespace details {
    template <typename T>
    inline constexpr bool is_condition_type_v = std::is_base_of_v<ParamValidDataObject, T>;

    template <typename T>
    using accessor_storage_t = std::conditional_t<is_condition_type_v<T>, T*, ParamValidDataObject*>;

    template <typename T>
    const T& extract_payload( const accessor_storage_t<T> ptr ) {
      if constexpr ( is_condition_type_v<T> ) {
        return *ptr;
      } else {
        return *std::any_cast<T>( &ptr->payload );
      }
    }
  } // namespace details

  // A condition accessor is to condition data what an EventReadHandle is to
  // event data : it notifies the framework that an Algorithm or AlgTool depends
  // on a certain condition, enables configuring the target condition from
  // Python, and allows accessing that condition during event processing.
  //
  // ConditionAccessors are meant to be declared as members of the Algorithm or
  // AlgTool that owns them. Declaring them in any other place (on the stack,
  // on a separate heap location...) is likely to result in undefined behaviour.
  //
  template <typename T>
  class ConditionAccessor {

    template <typename... Args, std::size_t... Is>
    ConditionAccessor( const std::tuple<Args...>& args, std::index_sequence<Is...> )
        : ConditionAccessor( std::get<Is>( args )... ) {}

  public:
    // Constructor takes the "this" pointer of the owner and the usual triplet
    // of values needed to declare a property (in this case the condition's key)
    template <typename Owner>
    ConditionAccessor( Owner* owner, const std::string& keyName, const ConditionKey& keyDefault,
                       const std::string& keyDoc = "" )
        : m_key{owner, keyName, keyDefault, keyDoc} {
      owner->registerConditionAccessor( *this );
    }

    template <typename... Args>
    ConditionAccessor( const std::tuple<Args...>& args )
        : ConditionAccessor( args, std::index_sequence_for<Args...>{} ) {}

    // Condition accessors can neither be moved nor copied
    ConditionAccessor( const ConditionAccessor& ) = delete;
    ConditionAccessor( ConditionAccessor&& )      = delete;
    ConditionAccessor& operator=( const ConditionAccessor& ) = delete;
    ConditionAccessor& operator=( ConditionAccessor&& ) = delete;

    // Access the key which this ConditionAccessor points to.
    // The key may change during configuration of the owner.
    const ConditionKey& key() const { return m_key; }

    // Access the value of the condition, for a given condition context
    const T& get( const ConditionContext& /*ctx*/ ) const {
      if ( !m_ptr )
        throw GaudiException( "payload not present: " + m_key.toString(), "ConditionAccessor::get",
                              StatusCode::FAILURE );
      return details::extract_payload<T>( m_ptr );
    }

  private:
    template <typename Base>
    friend class ConditionAccessorHolder;

    // Configurable key which this ConditionAccessor points to.
    Gaudi::Property<ConditionKey> m_key;

    // Pointer to the condition in the Detector Transien Store
    details::accessor_storage_t<T> m_ptr;
  };

  template <typename C, typename A>
  const C& get( const ConditionAccessor<C>& handle, const ConditionAccessorHolder<A>& algo, const EventContext& ctx ) {
    return handle.get( algo.getConditionContext( ctx ) );
  }

  template <typename C>
  const ConditionKey& getKey( const ConditionAccessor<C>& handle ) {
    return handle.key();
  }

  template <typename... C>
  struct useConditionHandleFor {
    template <typename T>
    using InputHandle = std::enable_if_t<std::disjunction_v<std::is_same<std::decay_t<T>, std::decay_t<C>>...>,
                                         ConditionAccessor<std::decay_t<T>>>;
  };

  template <typename Algorithm = FixTESPath<Gaudi::Algorithm>>
  using AlgorithmWithCondition = ConditionAccessorHolder<Algorithm>;

  template <typename... C>
  using usesConditions =
      Gaudi::Functional::Traits::use_<useConditionHandleFor<C...>,
                                      Gaudi::Functional::Traits::BaseClass_t<AlgorithmWithCondition<>>>;

  template <typename B, typename... C>
  using usesBaseAndConditions =
      Gaudi::Functional::Traits::use_<useConditionHandleFor<C...>,
                                      Gaudi::Functional::Traits::BaseClass_t<AlgorithmWithCondition<B>>>;

} // namespace LHCb::DetDesc
