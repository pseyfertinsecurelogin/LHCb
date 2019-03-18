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

#include <DetDesc/ConditionKey.h>
#include <GaudiKernel/StatusCode.h>
#include <functional>
#include <unordered_map>
#include <vector>

class Condition;
class IDataProviderSvc;
class IUpdateManagerSvc;
class UpdateManagerSvc;

namespace LHCb::DetDesc {
  /// Base class for declaration of condition derivations.
  /// Users are supposed to specialize this class implementing `operator()`.
  ///
  /// An example of use can be found in `DetCond/examples/ConditionAccessor.cpp`
  struct ConditionDerivation {
    /// Class used to access the conditions accessible to the current transformation.
    using ConditionUpdateContext = std::unordered_map<ConditionKey, Condition*>;

    /// Type for a user provided callback function.
    /// The first argument is the ConditionKey of the target and is used to be
    /// able to reuse a transformation function that behaves differently depending
    /// on the requested output, The ConditionUpdateContext will be filled with the
    /// input conditions, and the last argument is the Condition instance to update.
    using CallbackFunction = std::function<void( const ConditionKey& /* target */, ConditionUpdateContext& /* ctx */,
                                                 Condition& /* output */ )>;

    /// Construct a transformation object declaring the list of inputs ConditionKey
    /// and the output ConditionKey, and a callback function to invoke when an
    /// update of the output Condition is required.
    ConditionDerivation( std::vector<ConditionKey> inputs, ConditionKey output, CallbackFunction func );

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
    CallbackFunction m_func;

    //@{
    /// Backend specific variable
    ConditionUpdateContext m_condContext;
    ConditionKey           m_outputKey;
    Condition*             m_output = nullptr;
    //@}
  };
} // namespace LHCb::DetDesc
