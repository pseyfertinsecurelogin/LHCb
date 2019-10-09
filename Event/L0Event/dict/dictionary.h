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
#include "Event/L0CaloAdc.h"
#include "Event/L0CaloCandidate.h"
#include "Event/L0DUChannel.h"
#include "Event/L0DUConfig.h"
#include "Event/L0DUCounter.h"
#include "Event/L0DUDecision.h"
#include "Event/L0DUElementaryCondition.h"
#include "Event/L0DUElementaryData.h"
#include "Event/L0DUReport.h"
#include "Event/L0DUTemplateConfig.h"
#include "Event/L0DUTrigger.h"
#include "Event/L0MuonCandidate.h"
#include "Event/L0MuonCtrlError.h"
#include "Event/L0MuonData.h"
#include "Event/L0MuonError.h"
#include "Event/L0MuonInfo.h"
#include "Event/L0MuonProcError.h"
#include "Event/L0ProcessorData.h"
#include "Event/L0PrsSpdHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <map>
#include <string>
#include <utility>
#include <vector>
// end include files

namespace {
  struct L0Event_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::L0CaloAdc>                      m_KeyedContainer_LHCb__L0CaloAdc;
    KeyedContainer<LHCb::L0DUConfig>                     m_KeyedContainer_LHCb__L0DUConfig;
    KeyedContainer<LHCb::L0MuonCandidate>                m_KeyedContainer_LHCb__L0MuonCandidate;
    KeyedContainer<LHCb::L0MuonCtrlError>                m_KeyedContainer_LHCb__L0MuonCtrlError;
    KeyedContainer<LHCb::L0MuonData>                     m_KeyedContainer_LHCb__L0MuonData;
    KeyedContainer<LHCb::L0MuonError>                    m_KeyedContainer_LHCb__L0MuonError;
    KeyedContainer<LHCb::L0MuonProcError>                m_KeyedContainer_LHCb__L0MuonProcError;
    KeyedContainer<LHCb::L0ProcessorData>                m_KeyedContainer_LHCb__L0ProcessorData;
    KeyedContainer<LHCb::L0PrsSpdHit>                    m_KeyedContainer_LHCb__L0PrsSpdHit;
    ObjectVector<LHCb::L0CaloAdc>                        m_ObjectVector_LHCb__L0CaloAdc;
    ObjectVector<LHCb::L0CaloCandidate>                  m_ObjectVector_LHCb__L0CaloCandidate;
    ObjectVector<LHCb::L0DUChannel>                      m_ObjectVector_LHCb__L0DUChannel;
    ObjectVector<LHCb::L0DUConfig>                       m_ObjectVector_LHCb__L0DUConfig;
    ObjectVector<LHCb::L0DUElementaryCondition>          m_ObjectVector_LHCb__L0DUElementaryCondition;
    ObjectVector<LHCb::L0DUElementaryData>               m_ObjectVector_LHCb__L0DUElementaryData;
    ObjectVector<LHCb::L0DUTrigger>                      m_ObjectVector_LHCb__L0DUTrigger;
    ObjectVector<LHCb::L0MuonCandidate>                  m_ObjectVector_LHCb__L0MuonCandidate;
    ObjectVector<LHCb::L0MuonCtrlError>                  m_ObjectVector_LHCb__L0MuonCtrlError;
    ObjectVector<LHCb::L0MuonData>                       m_ObjectVector_LHCb__L0MuonData;
    ObjectVector<LHCb::L0MuonError>                      m_ObjectVector_LHCb__L0MuonError;
    ObjectVector<LHCb::L0MuonProcError>                  m_ObjectVector_LHCb__L0MuonProcError;
    ObjectVector<LHCb::L0ProcessorData>                  m_ObjectVector_LHCb__L0ProcessorData;
    ObjectVector<LHCb::L0PrsSpdHit>                      m_ObjectVector_LHCb__L0PrsSpdHit;
    SmartRef<LHCb::L0CaloAdc>                            m_SmartRef_LHCb__L0CaloAdc;
    SmartRef<LHCb::L0CaloCandidate>                      m_SmartRef_LHCb__L0CaloCandidate;
    SmartRef<LHCb::L0DUChannel>                          m_SmartRef_LHCb__L0DUChannel;
    SmartRef<LHCb::L0DUConfig>                           m_SmartRef_LHCb__L0DUConfig;
    SmartRef<LHCb::L0DUElementaryCondition>              m_SmartRef_LHCb__L0DUElementaryCondition;
    SmartRef<LHCb::L0DUElementaryData>                   m_SmartRef_LHCb__L0DUElementaryData;
    SmartRef<LHCb::L0DUReport>                           m_SmartRef_LHCb__L0DUReport;
    SmartRef<LHCb::L0DUTrigger>                          m_SmartRef_LHCb__L0DUTrigger;
    SmartRef<LHCb::L0MuonCandidate>                      m_SmartRef_LHCb__L0MuonCandidate;
    SmartRef<LHCb::L0MuonCtrlError>                      m_SmartRef_LHCb__L0MuonCtrlError;
    SmartRef<LHCb::L0MuonData>                           m_SmartRef_LHCb__L0MuonData;
    SmartRef<LHCb::L0MuonError>                          m_SmartRef_LHCb__L0MuonError;
    SmartRef<LHCb::L0MuonInfo>                           m_SmartRef_LHCb__L0MuonInfo;
    SmartRef<LHCb::L0MuonProcError>                      m_SmartRef_LHCb__L0MuonProcError;
    SmartRef<LHCb::L0ProcessorData>                      m_SmartRef_LHCb__L0ProcessorData;
    SmartRef<LHCb::L0PrsSpdHit>                          m_SmartRef_LHCb__L0PrsSpdHit;
    SmartRefVector<LHCb::L0CaloAdc>                      m_SmartRefVector_LHCb__L0CaloAdc;
    SmartRefVector<LHCb::L0CaloCandidate>                m_SmartRefVector_LHCb__L0CaloCandidate;
    SmartRefVector<LHCb::L0DUChannel>                    m_SmartRefVector_LHCb__L0DUChannel;
    SmartRefVector<LHCb::L0DUConfig>                     m_SmartRefVector_LHCb__L0DUConfig;
    SmartRefVector<LHCb::L0DUElementaryCondition>        m_SmartRefVector_LHCb__L0DUElementaryCondition;
    SmartRefVector<LHCb::L0DUElementaryData>             m_SmartRefVector_LHCb__L0DUElementaryData;
    SmartRefVector<LHCb::L0DUReport>                     m_SmartRefVector_LHCb__L0DUReport;
    SmartRefVector<LHCb::L0DUTrigger>                    m_SmartRefVector_LHCb__L0DUTrigger;
    SmartRefVector<LHCb::L0MuonCandidate>                m_SmartRefVector_LHCb__L0MuonCandidate;
    SmartRefVector<LHCb::L0MuonCtrlError>                m_SmartRefVector_LHCb__L0MuonCtrlError;
    SmartRefVector<LHCb::L0MuonData>                     m_SmartRefVector_LHCb__L0MuonData;
    SmartRefVector<LHCb::L0MuonError>                    m_SmartRefVector_LHCb__L0MuonError;
    SmartRefVector<LHCb::L0MuonInfo>                     m_SmartRefVector_LHCb__L0MuonInfo;
    SmartRefVector<LHCb::L0MuonProcError>                m_SmartRefVector_LHCb__L0MuonProcError;
    SmartRefVector<LHCb::L0ProcessorData>                m_SmartRefVector_LHCb__L0ProcessorData;
    SmartRefVector<LHCb::L0PrsSpdHit>                    m_SmartRefVector_LHCb__L0PrsSpdHit;
    std::vector<LHCb::L0CaloAdc*>                        m_std_vector_LHCb__L0CaloAdcp;
    std::vector<LHCb::L0CaloCandidate*>                  m_std_vector_LHCb__L0CaloCandidatep;
    std::vector<LHCb::L0DUChannel*>                      m_std_vector_LHCb__L0DUChannelp;
    std::vector<LHCb::L0DUConfig*>                       m_std_vector_LHCb__L0DUConfigp;
    std::vector<LHCb::L0DUElementaryCondition*>          m_std_vector_LHCb__L0DUElementaryConditionp;
    std::vector<LHCb::L0DUElementaryData*>               m_std_vector_LHCb__L0DUElementaryDatap;
    std::vector<LHCb::L0DUTrigger*>                      m_std_vector_LHCb__L0DUTriggerp;
    std::vector<LHCb::L0MuonCandidate*>                  m_std_vector_LHCb__L0MuonCandidatep;
    std::vector<LHCb::L0MuonCtrlError*>                  m_std_vector_LHCb__L0MuonCtrlErrorp;
    std::vector<LHCb::L0MuonData*>                       m_std_vector_LHCb__L0MuonDatap;
    std::vector<LHCb::L0MuonError*>                      m_std_vector_LHCb__L0MuonErrorp;
    std::vector<LHCb::L0MuonProcError*>                  m_std_vector_LHCb__L0MuonProcErrorp;
    std::vector<LHCb::L0ProcessorData*>                  m_std_vector_LHCb__L0ProcessorDatap;
    std::vector<LHCb::L0PrsSpdHit*>                      m_std_vector_LHCb__L0PrsSpdHitp;
    std::vector<SmartRef<LHCb::L0CaloAdc>>               m_std_vector_SmartRef_LHCb__L0CaloAdc;
    std::vector<SmartRef<LHCb::L0CaloCandidate>>         m_std_vector_SmartRef_LHCb__L0CaloCandidate;
    std::vector<SmartRef<LHCb::L0DUChannel>>             m_std_vector_SmartRef_LHCb__L0DUChannel;
    std::vector<SmartRef<LHCb::L0DUConfig>>              m_std_vector_SmartRef_LHCb__L0DUConfig;
    std::vector<SmartRef<LHCb::L0DUElementaryCondition>> m_std_vector_SmartRef_LHCb__L0DUElementaryCondition;
    std::vector<SmartRef<LHCb::L0DUElementaryData>>      m_std_vector_SmartRef_LHCb__L0DUElementaryData;
    std::vector<SmartRef<LHCb::L0DUReport>>              m_std_vector_SmartRef_LHCb__L0DUReport;
    std::vector<SmartRef<LHCb::L0DUTrigger>>             m_std_vector_SmartRef_LHCb__L0DUTrigger;
    std::vector<SmartRef<LHCb::L0MuonCandidate>>         m_std_vector_SmartRef_LHCb__L0MuonCandidate;
    std::vector<SmartRef<LHCb::L0MuonCtrlError>>         m_std_vector_SmartRef_LHCb__L0MuonCtrlError;
    std::vector<SmartRef<LHCb::L0MuonData>>              m_std_vector_SmartRef_LHCb__L0MuonData;
    std::vector<SmartRef<LHCb::L0MuonError>>             m_std_vector_SmartRef_LHCb__L0MuonError;
    std::vector<SmartRef<LHCb::L0MuonInfo>>              m_std_vector_SmartRef_LHCb__L0MuonInfo;
    std::vector<SmartRef<LHCb::L0MuonProcError>>         m_std_vector_SmartRef_LHCb__L0MuonProcError;
    std::vector<SmartRef<LHCb::L0ProcessorData>>         m_std_vector_SmartRef_LHCb__L0ProcessorData;
    std::vector<SmartRef<LHCb::L0PrsSpdHit>>             m_std_vector_SmartRef_LHCb__L0PrsSpdHit;
    // end instantiations
  };
} // namespace
