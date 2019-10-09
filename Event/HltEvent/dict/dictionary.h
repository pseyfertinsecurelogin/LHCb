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
//#include "Event/HltVertexReports.h"
#include "Event/HltDecReport.h"
#include "Event/HltDecReports.h"
#include "Event/HltLumiResult.h"
#include "Event/HltObjectSummary.h"
#include "Event/HltSelReports.h"
#include "Event/HltVertexReports.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct HltEvent_Instantiations {
    // begin instantiations
    GaudiUtils::VectorMap<std::string, LHCb::HltDecReport>                      _dr2;
    GaudiUtils::VectorMap<std::string, LHCb::HltObjectSummary>                  _ii2;
    GaudiUtils::VectorMap<std::string, LHCb::HltVertexReports::HltVertexReport> _vr0;
    LHCb::HltObjectSummary::Info                                                _ii5;
    ObjectVector<LHCb::HltObjectSummary>                                        m_ObjectVector_LHCb__HltObjectSummary;
    SmartRef<LHCb::HltDecReports>                                               m_SmartRef_LHCb__HltDecReports;
    SmartRef<LHCb::HltLumiResult>                                               m_SmartRef_LHCb__HltLumiResult;
    SmartRef<LHCb::HltObjectSummary>                                            m_SmartRef_LHCb__HltObjectSummary;
    SmartRef<LHCb::HltSelReports>                                               m_SmartRef_LHCb__HltSelReports;
    SmartRef<LHCb::HltVertexReports>                                            m_SmartRef_LHCb__HltVertexReports;
    SmartRefVector<LHCb::HltDecReports>                                         m_SmartRefVector_LHCb__HltDecReports;
    SmartRefVector<LHCb::HltLumiResult>                                         m_SmartRefVector_LHCb__HltLumiResult;
    SmartRefVector<LHCb::HltObjectSummary>                                      m_SmartRefVector_LHCb__HltObjectSummary;
    SmartRefVector<LHCb::HltSelReports>                                         m_SmartRefVector_LHCb__HltSelReports;
    SmartRefVector<LHCb::HltVertexReports>                                      m_SmartRefVector_LHCb__HltVertexReports;
    std::pair<std::string, LHCb::HltDecReport>                                  _dr0;
    std::pair<std::string, LHCb::HltObjectSummary>                              _ii0;
    std::vector<LHCb::HltObjectSummary*>                                        _ii4;
    std::vector<LHCb::HltObjectSummary*>                                        m_std_vector_LHCb__HltObjectSummaryp;
    std::vector<SmartRef<LHCb::HltDecReports>>                  m_std_vector_SmartRef_LHCb__HltDecReports;
    std::vector<SmartRef<LHCb::HltLumiResult>>                  m_std_vector_SmartRef_LHCb__HltLumiResult;
    std::vector<SmartRef<LHCb::HltObjectSummary>>               m_std_vector_SmartRef_LHCb__HltObjectSummary;
    std::vector<SmartRef<LHCb::HltSelReports>>                  m_std_vector_SmartRef_LHCb__HltSelReports;
    std::vector<SmartRef<LHCb::HltVertexReports>>               m_std_vector_SmartRef_LHCb__HltVertexReports;
    std::vector<const LHCb::HltObjectSummary*>                  _ii3;
    std::vector<std::pair<std::string, LHCb::HltDecReport>>     _dr1;
    std::vector<std::pair<std::string, LHCb::HltObjectSummary>> _ii1;
    // end instantiations
  };
} // namespace
