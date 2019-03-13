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
  struct ConditionTransformation {
    using ConditionUpdateContext = std::unordered_map<ConditionKey, Condition*>;

    ConditionTransformation( std::vector<ConditionKey> inputs, ConditionKey output );

    virtual ~ConditionTransformation() = default; // required by IUpdateManagerSvc::registerCondition

    void registerTransformation( IUpdateManagerSvc* ums, IDataProviderSvc* dds );

    virtual void operator()( const ConditionKey& target, ConditionUpdateContext& ctx, Condition& output ) const = 0;

  private:
    StatusCode handler();

    ConditionUpdateContext m_condContext;
    ConditionKey           m_outputKey;
    Condition*             m_output = nullptr;
  };
} // namespace LHCb::DetDesc
