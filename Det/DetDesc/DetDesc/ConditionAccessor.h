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

#include <DetDesc/ConditionContext.h>
#include <DetDesc/ConditionKey.h>
#include <DetDesc/ParamValidDataObject.h>

#include <GaudiKernel/Property.h>

#include <string>
#include <type_traits>

namespace LHCb::DetDesc {
  template <typename Base>
  class ConditionAccessorHolder;

  namespace details {
    template <typename T>
    inline constexpr bool is_conditon_type_v = std::is_convertible<T*, ParamValidDataObject*>::value;

    template <typename T>
    using is_conditon_type_t = std::conditional_t<is_conditon_type_v<T>, std::true_type, std::false_type>;

    template <typename T>
    using accessor_storage_t = std::conditional_t<is_conditon_type_v<T>, T*, ParamValidDataObject*>;

    template <typename T>
    const T& do_extract_payload( std::true_type, const accessor_storage_t<T> ptr ) {
      return *ptr;
    }
    template <typename T>
    const T& do_extract_payload( std::false_type, const accessor_storage_t<T> ptr ) {
      return *std::any_cast<T>( &ptr->payload );
    }

    template <typename T>
    const T& extract_payload( const accessor_storage_t<T> ptr ) {
      return do_extract_payload<T>( is_conditon_type_t<T>{}, ptr );
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
  public:
    // Constructor takes the "this" pointer of the owner and the usual triplet
    // of values needed to declare a property (in this case the condition's key)
    template <typename Owner>
    ConditionAccessor( Owner* owner, const std::string& keyName, const ConditionKey& keyDefault,
                       const std::string& keyDoc = "" )
        : m_key{owner, keyName, keyDefault, keyDoc} {
      owner->registerConditionAccessor( *this );
    }

    // Condition accessors can neither be moved nor copied
    ConditionAccessor( const ConditionAccessor& ) = delete;
    ConditionAccessor( ConditionAccessor&& )      = delete;
    ConditionAccessor& operator=( const ConditionAccessor& ) = delete;
    ConditionAccessor& operator=( ConditionAccessor&& ) = delete;

    // Access the key which this ConditionAccessor points to.
    // The key may change during configuration of the owner.
    const ConditionKey& key() const { return m_key; }

    // Access the value of the condition, for a given condition context
    const T& get( const ConditionContext& /*ctx*/ ) const { return details::extract_payload<T>( m_ptr ); }

  private:
    template <typename Base>
    friend class ConditionAccessorHolder;

    // Configurable key which this ConditionAccessor points to.
    Gaudi::Property<ConditionKey> m_key;

    // Pointer to the condition in the Detector Transien Store
    details::accessor_storage_t<T> m_ptr;
  };
} // namespace LHCb::DetDesc
