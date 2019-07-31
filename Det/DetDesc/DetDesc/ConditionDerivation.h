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

#include "DetDesc/ConditionKey.h"
#include "DetDesc/IConditionDerivationMgr.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/STLExtensions.h"
#include <functional>
#include <unordered_map>
#include <vector>

struct Condition;
class IDataProviderSvc;
class IUpdateManagerSvc;
class UpdateManagerSvc;

namespace LHCb::DetDesc {
  /// Base class for declaration of condition derivations.
  /// Users are supposed to specialize this class implementing `operator()`.
  ///
  /// An example of use can be found in `DetCond/examples/ConditionAccessor.cpp`
  struct ConditionDerivation {
    /// Construct a transformation object declaring the list of inputs ConditionKey
    /// and the output ConditionKey, and a callback function to invoke when an
    /// update of the output Condition is required.
    ConditionDerivation( LHCb::span<const ConditionKey> inputs, ConditionKey output, ConditionCallbackFunction func );

    /// Althogh one should not specialize this class, a virtual destructor is needed
    /// to comply to IUpdateManagerSvc requirements.
    virtual ~ConditionDerivation() = default;

    /// Return the ConditionKey for the target object.
    const ConditionKey& target() const { return m_outputKey; }

  private:
    // Register and unregister functions are meant to be called only by
    // UpdateManagerSvc (the derivations manager).
    friend UpdateManagerSvc;

    /// Register this transformation callback to the IUpdateManagerSvc instance.
    void registerDerivation( IUpdateManagerSvc* ums, IDataProviderSvc* dds );

    /// Un-register this transformation callback to the IUpdateManagerSvc instance.
    void unregisterDerivation( IUpdateManagerSvc* ums );

    /// Internal wrapper around the provided function, required by IUpdateManagerSvc.
    StatusCode i_handler();

    /// User provided callback function.
    ConditionCallbackFunction m_func;

    //@{
    /// Backend specific variable
    ConditionUpdateContext m_condContext;
    ConditionKey           m_outputKey;
    Condition*             m_output = nullptr;
    //@}
  };
} // namespace LHCb::DetDesc
