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
#include <unordered_map>
#include <vector>

class IUpdateManagerSvc;
class IDataProviderSvc;
class Condition;

namespace LHCb::DetDesc {
  /// Base class for declaration of condition derivations.
  /// Users are supposed to specialize this class implementing `operator()`.
  ///
  /// An example of use can be found in `DetCond/examples/ConditionAccessor.cpp`
  struct ConditionDerivation {
    /// Class used to access the conditions accessible to the current transformation.
    using ConditionUpdateContext = std::unordered_map<ConditionKey, Condition*>;

    /// Construct a transformation object declaring the list of inputs ConditionKey
    /// and the output ConditionKey.
    ConditionDerivation( std::vector<ConditionKey> inputs, ConditionKey output );

    virtual ~ConditionDerivation() = default;

    /// Register this transformation callback to the IUpdateManagerSvc instance.
    void registerDerivation( IUpdateManagerSvc* ums, IDataProviderSvc* dds );

    /// Un-register this transformation callback to the IUpdateManagerSvc instance.
    void unregisterDerivation( IUpdateManagerSvc* ums );

    /// Callback invoked whe an update of the output Condition is required.
    /// the `ctx` ConditionUpdateContext will be filled with the input conditions,
    /// the `target` ConditionKey is used to be able to reuse a transformation class
    /// that behaves differently depending on the requested output, and `output` is
    /// the Condition instance to update.
    virtual void operator()( const ConditionKey& target, ConditionUpdateContext& ctx, Condition& output ) const = 0;

  private:
    /// Internal wrapper around `operator()` required by IUpdateManagerSvc.
    StatusCode i_handler();

    //@{
    /// Backend specific variable
    ConditionUpdateContext m_condContext;
    ConditionKey           m_outputKey;
    Condition*             m_output = nullptr;
    //@}
  };
} // namespace LHCb::DetDesc
