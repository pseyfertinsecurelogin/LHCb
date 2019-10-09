/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// begin include files
#include "Event/EventCountFSR.h"
#include "Event/HltLumiSummary.h"
#include "Event/LumiCounters.h"
#include "Event/LumiFSR.h"
#include "Event/LumiIntegral.h"
#include "Event/LumiMethods.h"
#include "Event/TimeSpanFSR.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>

// end include files

namespace {
  struct LumiEvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::LumiFSR>               m_KeyedContainer_LHCb__LumiFSR;
    KeyedContainer<LHCb::LumiIntegral>          m_KeyedContainer_LHCb__LumiIntegral;
    KeyedContainer<LHCb::TimeSpanFSR>           m_KeyedContainer_LHCb__TimeSpanFSR;
    ObjectVector<LHCb::LumiFSR>                 m_ObjectVector_LHCb__LumiFSR;
    ObjectVector<LHCb::LumiIntegral>            m_ObjectVector_LHCb__LumiIntegral;
    ObjectVector<LHCb::TimeSpanFSR>             m_ObjectVector_LHCb__TimeSpanFSR;
    SmartRef<LHCb::HltLumiSummary>              m_SmartRef_LHCb__HltLumiSummary;
    SmartRef<LHCb::LumiCounters>                m_SmartRef_LHCb__LumiCounters;
    SmartRef<LHCb::LumiFSR>                     m_SmartRef_LHCb__LumiFSR;
    SmartRef<LHCb::LumiIntegral>                m_SmartRef_LHCb__LumiIntegral;
    SmartRef<LHCb::LumiMethods>                 m_SmartRef_LHCb__LumiMethods;
    SmartRef<LHCb::TimeSpanFSR>                 m_SmartRef_LHCb__TimeSpanFSR;
    SmartRefVector<LHCb::HltLumiSummary>        m_SmartRefVector_LHCb__HltLumiSummary;
    SmartRefVector<LHCb::LumiCounters>          m_SmartRefVector_LHCb__LumiCounters;
    SmartRefVector<LHCb::LumiFSR>               m_SmartRefVector_LHCb__LumiFSR;
    SmartRefVector<LHCb::LumiIntegral>          m_SmartRefVector_LHCb__LumiIntegral;
    SmartRefVector<LHCb::LumiMethods>           m_SmartRefVector_LHCb__LumiMethods;
    SmartRefVector<LHCb::TimeSpanFSR>           m_SmartRefVector_LHCb__TimeSpanFSR;
    std::vector<LHCb::LumiFSR*>                 m_std_vector_LHCb__LumiFSRp;
    std::vector<LHCb::LumiIntegral*>            m_std_vector_LHCb__LumiIntegralp;
    std::vector<LHCb::TimeSpanFSR*>             m_std_vector_LHCb__TimeSpanFSRp;
    std::vector<SmartRef<LHCb::HltLumiSummary>> m_std_vector_SmartRef_LHCb__HltLumiSummary;
    std::vector<SmartRef<LHCb::LumiCounters>>   m_std_vector_SmartRef_LHCb__LumiCounters;
    std::vector<SmartRef<LHCb::LumiFSR>>        m_std_vector_SmartRef_LHCb__LumiFSR;
    std::vector<SmartRef<LHCb::LumiIntegral>>   m_std_vector_SmartRef_LHCb__LumiIntegral;
    std::vector<SmartRef<LHCb::LumiMethods>>    m_std_vector_SmartRef_LHCb__LumiMethods;
    std::vector<SmartRef<LHCb::TimeSpanFSR>>    m_std_vector_SmartRef_LHCb__TimeSpanFSR;
    // end instantiations
  };
} // namespace
