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

#include <DetDesc/ConditionAccessor.h>
#include <DetDesc/ConditionContext.h>
#include <DetDesc/ConditionKey.h>

#include <GaudiKernel/DataObjectHandle.h>
#include <GaudiKernel/GaudiException.h>
#include <GaudiKernel/IUpdateManagerSvc.h>
#include <GaudiKernel/SmartIF.h>
#include <GaudiKernel/StatusCode.h>

#include <functional>
#include <list>

class EventContext;

namespace LHCb::DetDesc {
  // Wrapper around Algorithm or AlgTool that enables it to depend on
  // conditions via ConditionAccessors.
  //
  // Together with ConditionAccessor, this is the main interface through which
  // Algorithm and Tool writers interact with the condition management backend.
  // Everything else is, in a sense, an implementation detail of them.
  //
  template <typename Base>
  class ConditionAccessorHolder : public Base {
  public:
    // helper to access the base class type
    using base_class = ConditionAccessorHolder;

    // Forward the constructor from the Algorithm / AlgTool base class
    using Base::Base;

    // The base class exposes to the user all the other components of the
    // chosen back-end's interface: accessors, contexts and keys.
    template <typename T>
    using ConditionAccessor = LHCb::DetDesc::ConditionAccessor<T>;
    using ConditionContext  = LHCb::DetDesc::ConditionContext;
    using ConditionKey      = LHCb::DetDesc::ConditionKey;
    template <typename T>
    using DetectorAccessor = LHCb::DetDesc::ConditionAccessor<T>;

    // At initialize() time, we must declare our condition inputs (whose key
    // has been configured by now) to the condition management backend.
    StatusCode initialize() override {
      // Initialize the base (this will configure condition accessor keys)
      auto status = Base::initialize();
      if ( status.isFailure() ) return status;

      m_ums = this->service( "UpdateManagerSvc" );
      if ( !m_ums ) return StatusCode::FAILURE;

      while ( !m_delayedRegistrations.empty() ) {
        m_delayedRegistrations.front()();
        m_delayedRegistrations.pop_front();
      }

      return status;
    }

    // We must also provide a way for a reentrant algorithm to access the
    // condition handling context associated with this event.
    const ConditionContext& getConditionContext( const EventContext& /*ctx*/ ) const { return *m_ctxHandle.get(); }

  private:
    // This block of private declarations is an implementation detail of accessors
    template <typename T>
    friend class ConditionAccessor;

    // Track that a new ConditionAccessor has been declared
    template <typename T>
    void registerConditionAccessor( ConditionAccessor<T>& accessor ) {
      if ( m_ums ) {
        m_ums->registerCondition( this, accessor.key(), nullptr, accessor.m_ptr );
      } else {
        auto ptr = &accessor;
        m_delayedRegistrations.emplace_back( [this, ptr]() { registerConditionAccessor( *ptr ); } );
      }
    }

  private:
    // We must declare a dependency on the condition context
    // (which will be produced by the ConditionContextProducer alg).
    DataObjectReadHandle<ConditionContext> m_ctxHandle{DataObjID{ConditionContext::DefaultLocation}, this};

    SmartIF<IUpdateManagerSvc> m_ums;

    /// ConditionAccessors are usually registered before initialization, so we need
    /// to postpone the actual registration to the UMS to the initialization.
    std::list<std::function<void()>> m_delayedRegistrations;
  };
} // namespace LHCb::DetDesc
