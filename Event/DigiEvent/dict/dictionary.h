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
#include "Event/BcmDigit.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#include "Event/CaloDigitStatus.h"
#include "Event/HCDigit.h"
#include "Event/MuonDigit.h"
#include "Event/OTTime.h"
#include "Event/PackMuonDigit.h"
#include "Event/RichDigit.h"
#include "Event/STCluster.h"
#include "Event/STDigit.h"
#include "Event/STLiteCluster.h"
#include "Event/UTCluster.h"
#include "Event/UTDigit.h"
#include "Event/UTLiteCluster.h"
#include "Event/VPCluster.h"
#include "Event/VPDigit.h"
#include "Event/VeloCluster.h"
#include "Event/VeloDigit.h"
#include "Event/VeloLiteCluster.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/FastClusterContainer.h"
#include <vector>

// end include files

namespace {
  struct DigiEvent_Instantiations {
    // begin instantiations
    FastClusterContainer<LHCb::STLiteCluster, int>   _i2;
    FastClusterContainer<LHCb::VeloLiteCluster, int> _i1;
    KeyedContainer<LHCb::BcmDigit>                   m_KeyedContainer_LHCb__BcmDigit;
    KeyedContainer<LHCb::CaloAdc>                    m_KeyedContainer_LHCb__CaloAdc;
    KeyedContainer<LHCb::CaloDigit>                  m_KeyedContainer_LHCb__CaloDigit;
    KeyedContainer<LHCb::HCDigit>                    m_KeyedContainer_LHCb__HCDigit;
    KeyedContainer<LHCb::MuonDigit>                  m_KeyedContainer_LHCb__MuonDigit;
    KeyedContainer<LHCb::OTTime>                     m_KeyedContainer_LHCb__OTTime;
    KeyedContainer<LHCb::RichDigit>                  m_KeyedContainer_LHCb__RichDigit;
    KeyedContainer<LHCb::STCluster>                  m_KeyedContainer_LHCb__STCluster;
    KeyedContainer<LHCb::STDigit>                    m_KeyedContainer_LHCb__STDigit;
    KeyedContainer<LHCb::UTCluster>                  m_KeyedContainer_LHCb__UTCluster;
    KeyedContainer<LHCb::UTDigit>                    m_KeyedContainer_LHCb__UTDigit;
    KeyedContainer<LHCb::VPCluster>                  m_KeyedContainer_LHCb__VPCluster;
    KeyedContainer<LHCb::VPDigit>                    m_KeyedContainer_LHCb__VPDigit;
    KeyedContainer<LHCb::VeloCluster>                m_KeyedContainer_LHCb__VeloCluster;
    KeyedContainer<LHCb::VeloDigit>                  m_KeyedContainer_LHCb__VeloDigit;
    KeyedObject<LHCb::CaloCellID>                    m_KeyedObject_LHCb__CaloCellID;
    KeyedObject<LHCb::HCCellID>                      m_KeyedObject_LHCb__HCCellID;
    KeyedObject<LHCb::MuonTileID>                    m_KeyedObject_LHCb__MuonTileID;
    KeyedObject<LHCb::RichSmartID>                   m_KeyedObject_LHCb__RichSmartID;
    KeyedObject<LHCb::STChannelID>                   m_KeyedObject_LHCb__STChannelID;
    KeyedObject<LHCb::UTChannelID>                   m_KeyedObject_LHCb__UTChannelID;
    KeyedObject<LHCb::VPChannelID>                   m_KeyedObject_LHCb__VPChannelID;
    KeyedObject<LHCb::VeloChannelID>                 m_KeyedObject_LHCb__VeloChannelID;
    ObjectVector<LHCb::BcmDigit>                     m_ObjectVector_LHCb__BcmDigit;
    ObjectVector<LHCb::CaloAdc>                      m_ObjectVector_LHCb__CaloAdc;
    ObjectVector<LHCb::CaloDigit>                    m_ObjectVector_LHCb__CaloDigit;
    ObjectVector<LHCb::HCDigit>                      m_ObjectVector_LHCb__HCDigit;
    ObjectVector<LHCb::MuonDigit>                    m_ObjectVector_LHCb__MuonDigit;
    ObjectVector<LHCb::OTTime>                       m_ObjectVector_LHCb__OTTime;
    ObjectVector<LHCb::RichDigit>                    m_ObjectVector_LHCb__RichDigit;
    ObjectVector<LHCb::STCluster>                    m_ObjectVector_LHCb__STCluster;
    ObjectVector<LHCb::STDigit>                      m_ObjectVector_LHCb__STDigit;
    ObjectVector<LHCb::UTCluster>                    m_ObjectVector_LHCb__UTCluster;
    ObjectVector<LHCb::UTDigit>                      m_ObjectVector_LHCb__UTDigit;
    ObjectVector<LHCb::VPCluster>                    m_ObjectVector_LHCb__VPCluster;
    ObjectVector<LHCb::VPDigit>                      m_ObjectVector_LHCb__VPDigit;
    ObjectVector<LHCb::VeloCluster>                  m_ObjectVector_LHCb__VeloCluster;
    ObjectVector<LHCb::VeloDigit>                    m_ObjectVector_LHCb__VeloDigit;
    SmartRef<LHCb::BcmDigit>                         m_SmartRef_LHCb__BcmDigit;
    SmartRef<LHCb::CaloAdc>                          m_SmartRef_LHCb__CaloAdc;
    SmartRef<LHCb::CaloDigit>                        m_SmartRef_LHCb__CaloDigit;
    SmartRef<LHCb::HCDigit>                          m_SmartRef_LHCb__HCDigit;
    SmartRef<LHCb::MuonDigit>                        m_SmartRef_LHCb__MuonDigit;
    SmartRef<LHCb::OTTime>                           m_SmartRef_LHCb__OTTime;
    SmartRef<LHCb::RichDigit>                        m_SmartRef_LHCb__RichDigit;
    SmartRef<LHCb::STCluster>                        m_SmartRef_LHCb__STCluster;
    SmartRef<LHCb::STDigit>                          m_SmartRef_LHCb__STDigit;
    SmartRef<LHCb::UTCluster>                        m_SmartRef_LHCb__UTCluster;
    SmartRef<LHCb::UTDigit>                          m_SmartRef_LHCb__UTDigit;
    SmartRef<LHCb::VPCluster>                        m_SmartRef_LHCb__VPCluster;
    SmartRef<LHCb::VPDigit>                          m_SmartRef_LHCb__VPDigit;
    SmartRef<LHCb::VeloCluster>                      m_SmartRef_LHCb__VeloCluster;
    SmartRef<LHCb::VeloDigit>                        m_SmartRef_LHCb__VeloDigit;
    SmartRefVector<LHCb::BcmDigit>                   m_SmartRefVector_LHCb__BcmDigit;
    SmartRefVector<LHCb::CaloAdc>                    m_SmartRefVector_LHCb__CaloAdc;
    SmartRefVector<LHCb::CaloDigit>                  m_SmartRefVector_LHCb__CaloDigit;
    SmartRefVector<LHCb::HCDigit>                    m_SmartRefVector_LHCb__HCDigit;
    SmartRefVector<LHCb::MuonDigit>                  m_SmartRefVector_LHCb__MuonDigit;
    SmartRefVector<LHCb::OTTime>                     m_SmartRefVector_LHCb__OTTime;
    SmartRefVector<LHCb::RichDigit>                  m_SmartRefVector_LHCb__RichDigit;
    SmartRefVector<LHCb::STCluster>                  m_SmartRefVector_LHCb__STCluster;
    SmartRefVector<LHCb::STDigit>                    m_SmartRefVector_LHCb__STDigit;
    SmartRefVector<LHCb::UTCluster>                  m_SmartRefVector_LHCb__UTCluster;
    SmartRefVector<LHCb::UTDigit>                    m_SmartRefVector_LHCb__UTDigit;
    SmartRefVector<LHCb::VPCluster>                  m_SmartRefVector_LHCb__VPCluster;
    SmartRefVector<LHCb::VPDigit>                    m_SmartRefVector_LHCb__VPDigit;
    SmartRefVector<LHCb::VeloCluster>                m_SmartRefVector_LHCb__VeloCluster;
    SmartRefVector<LHCb::VeloDigit>                  m_SmartRefVector_LHCb__VeloDigit;
    std::vector<LHCb::BcmDigit*>                     m_std_vector_LHCb__BcmDigitp;
    std::vector<LHCb::CaloAdc*>                      m_std_vector_LHCb__CaloAdcp;
    std::vector<LHCb::CaloDigit*>                    m_std_vector_LHCb__CaloDigitp;
    std::vector<LHCb::HCDigit*>                      m_std_vector_LHCb__HCDigitp;
    std::vector<LHCb::MuonDigit*>                    m_std_vector_LHCb__MuonDigitp;
    std::vector<LHCb::OTTime*>                       m_std_vector_LHCb__OTTimep;
    std::vector<LHCb::RichDigit*>                    m_std_vector_LHCb__RichDigitp;
    std::vector<LHCb::STCluster*>                    m_std_vector_LHCb__STClusterp;
    std::vector<LHCb::STDigit*>                      m_std_vector_LHCb__STDigitp;
    std::vector<LHCb::UTCluster*>                    m_std_vector_LHCb__UTClusterp;
    std::vector<LHCb::UTDigit*>                      m_std_vector_LHCb__UTDigitp;
    std::vector<LHCb::VPCluster*>                    m_std_vector_LHCb__VPClusterp;
    std::vector<LHCb::VPDigit*>                      m_std_vector_LHCb__VPDigitp;
    std::vector<LHCb::VeloCluster*>                  m_std_vector_LHCb__VeloClusterp;
    std::vector<LHCb::VeloDigit*>                    m_std_vector_LHCb__VeloDigitp;
    std::vector<SmartRef<LHCb::BcmDigit>>            m_std_vector_SmartRef_LHCb__BcmDigit;
    std::vector<SmartRef<LHCb::CaloAdc>>             m_std_vector_SmartRef_LHCb__CaloAdc;
    std::vector<SmartRef<LHCb::CaloDigit>>           m_std_vector_SmartRef_LHCb__CaloDigit;
    std::vector<SmartRef<LHCb::HCDigit>>             m_std_vector_SmartRef_LHCb__HCDigit;
    std::vector<SmartRef<LHCb::MuonDigit>>           m_std_vector_SmartRef_LHCb__MuonDigit;
    std::vector<SmartRef<LHCb::OTTime>>              m_std_vector_SmartRef_LHCb__OTTime;
    std::vector<SmartRef<LHCb::RichDigit>>           m_std_vector_SmartRef_LHCb__RichDigit;
    std::vector<SmartRef<LHCb::STCluster>>           m_std_vector_SmartRef_LHCb__STCluster;
    std::vector<SmartRef<LHCb::STDigit>>             m_std_vector_SmartRef_LHCb__STDigit;
    std::vector<SmartRef<LHCb::UTCluster>>           m_std_vector_SmartRef_LHCb__UTCluster;
    std::vector<SmartRef<LHCb::UTDigit>>             m_std_vector_SmartRef_LHCb__UTDigit;
    std::vector<SmartRef<LHCb::VPCluster>>           m_std_vector_SmartRef_LHCb__VPCluster;
    std::vector<SmartRef<LHCb::VPDigit>>             m_std_vector_SmartRef_LHCb__VPDigit;
    std::vector<SmartRef<LHCb::VeloCluster>>         m_std_vector_SmartRef_LHCb__VeloCluster;
    std::vector<SmartRef<LHCb::VeloDigit>>           m_std_vector_SmartRef_LHCb__VeloDigit;
    // end instantiations
  };
} // namespace
