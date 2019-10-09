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
#include "Event/GhostTrackInfo.h"
#include "Event/MCBcmDigit.h"
#include "Event/MCCaloDigit.h"
#include "Event/MCCaloHit.h"
#include "Event/MCExtendedHit.h"
#include "Event/MCHeader.h"
#include "Event/MCHit.h"
#include "Event/MCMuonDigit.h"
#include "Event/MCMuonDigitInfo.h"
#include "Event/MCMuonHitHistory.h"
#include "Event/MCOTDeposit.h"
#include "Event/MCOTTime.h"
#include "Event/MCParticle.h"
#include "Event/MCProperty.h"
#include "Event/MCRichDigit.h"
#include "Event/MCRichDigitHistoryCode.h"
#include "Event/MCRichDigitHit.h"
#include "Event/MCRichDigitSummary.h"
#include "Event/MCRichHit.h"
#include "Event/MCRichOpticalPhoton.h"
#include "Event/MCRichSegment.h"
#include "Event/MCRichTrack.h"
#include "Event/MCSTDeposit.h"
#include "Event/MCSTDigit.h"
#include "Event/MCSensPlaneHit.h"
#include "Event/MCTrackInfo.h"
#include "Event/MCUTDeposit.h"
#include "Event/MCUTDigit.h"
#include "Event/MCVPDigit.h"
#include "Event/MCVeloFE.h"
#include "Event/MCVertex.h"
#include "Event/MuonBXFlag.h"
#include "Event/MuonOriginFlag.h"
#include "Event/PackMCMuonDigitHistory.h"
#include "Event/PackMCMuonDigitInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/Map.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <utility>
#include <vector>

// end include files

namespace {
  struct MCEvent_Instantiations {
    // begin instantiations
    GaudiUtils::Map<LHCb::MCParticle*, unsigned int, std::map<LHCb::MCParticle*, unsigned int>> _i4;
    KeyedContainer<LHCb::MCBcmDigit>                 m_KeyedContainer_LHCb__MCBcmDigit;
    KeyedContainer<LHCb::MCCaloDigit>                m_KeyedContainer_LHCb__MCCaloDigit;
    KeyedContainer<LHCb::MCMuonDigit>                m_KeyedContainer_LHCb__MCMuonDigit;
    KeyedContainer<LHCb::MCOTTime>                   m_KeyedContainer_LHCb__MCOTTime;
    KeyedContainer<LHCb::MCParticle>                 m_KeyedContainer_LHCb__MCParticle;
    KeyedContainer<LHCb::MCRichDigit>                m_KeyedContainer_LHCb__MCRichDigit;
    KeyedContainer<LHCb::MCRichOpticalPhoton>        m_KeyedContainer_LHCb__MCRichOpticalPhoton;
    KeyedContainer<LHCb::MCRichSegment>              m_KeyedContainer_LHCb__MCRichSegment;
    KeyedContainer<LHCb::MCRichTrack>                m_KeyedContainer_LHCb__MCRichTrack;
    KeyedContainer<LHCb::MCSTDeposit>                m_KeyedContainer_LHCb__MCSTDeposit;
    KeyedContainer<LHCb::MCSTDigit>                  m_KeyedContainer_LHCb__MCSTDigit;
    KeyedContainer<LHCb::MCUTDeposit>                m_KeyedContainer_LHCb__MCUTDeposit;
    KeyedContainer<LHCb::MCUTDigit>                  m_KeyedContainer_LHCb__MCUTDigit;
    KeyedContainer<LHCb::MCVPDigit>                  m_KeyedContainer_LHCb__MCVPDigit;
    KeyedContainer<LHCb::MCVeloFE>                   m_KeyedContainer_LHCb__MCVeloFE;
    KeyedContainer<LHCb::MCVertex>                   m_KeyedContainer_LHCb__MCVertex;
    KeyedObject<LHCb::CaloCellID>                    m_KeyedObject_LHCb__CaloCellID;
    KeyedObject<LHCb::OTChannelID>                   m_KeyedObject_LHCb__OTChannelID;
    KeyedObject<LHCb::STChannelID>                   m_KeyedObject_LHCb__STChannelID;
    KeyedObject<LHCb::UTChannelID>                   m_KeyedObject_LHCb__UTChannelID;
    KeyedObject<LHCb::VPChannelID>                   m_KeyedObject_LHCb__VPChannelID;
    KeyedObject<LHCb::VeloChannelID>                 m_KeyedObject_LHCb__VeloChannelID;
    ObjectVector<LHCb::MCBcmDigit>                   m_ObjectVector_LHCb__MCBcmDigit;
    ObjectVector<LHCb::MCCaloDigit>                  m_ObjectVector_LHCb__MCCaloDigit;
    ObjectVector<LHCb::MCCaloHit>                    m_ObjectVector_LHCb__MCCaloHit;
    ObjectVector<LHCb::MCExtendedHit>                m_ObjectVector_LHCb__MCExtendedHit;
    ObjectVector<LHCb::MCHit>                        m_ObjectVector_LHCb__MCHit;
    ObjectVector<LHCb::MCMuonDigit>                  m_ObjectVector_LHCb__MCMuonDigit;
    ObjectVector<LHCb::MCOTDeposit>                  m_ObjectVector_LHCb__MCOTDeposit;
    ObjectVector<LHCb::MCOTTime>                     m_ObjectVector_LHCb__MCOTTime;
    ObjectVector<LHCb::MCParticle>                   m_ObjectVector_LHCb__MCParticle;
    ObjectVector<LHCb::MCRichDigit>                  m_ObjectVector_LHCb__MCRichDigit;
    ObjectVector<LHCb::MCRichDigitSummary>           m_ObjectVector_LHCb__MCRichDigitSummary;
    ObjectVector<LHCb::MCRichHit>                    m_ObjectVector_LHCb__MCRichHit;
    ObjectVector<LHCb::MCRichOpticalPhoton>          m_ObjectVector_LHCb__MCRichOpticalPhoton;
    ObjectVector<LHCb::MCRichSegment>                m_ObjectVector_LHCb__MCRichSegment;
    ObjectVector<LHCb::MCRichTrack>                  m_ObjectVector_LHCb__MCRichTrack;
    ObjectVector<LHCb::MCSTDeposit>                  m_ObjectVector_LHCb__MCSTDeposit;
    ObjectVector<LHCb::MCSTDigit>                    m_ObjectVector_LHCb__MCSTDigit;
    ObjectVector<LHCb::MCSensPlaneHit>               m_ObjectVector_LHCb__MCSensPlaneHit;
    ObjectVector<LHCb::MCUTDeposit>                  m_ObjectVector_LHCb__MCUTDeposit;
    ObjectVector<LHCb::MCUTDigit>                    m_ObjectVector_LHCb__MCUTDigit;
    ObjectVector<LHCb::MCVPDigit>                    m_ObjectVector_LHCb__MCVPDigit;
    ObjectVector<LHCb::MCVeloFE>                     m_ObjectVector_LHCb__MCVeloFE;
    ObjectVector<LHCb::MCVertex>                     m_ObjectVector_LHCb__MCVertex;
    SmartRef<LHCb::MCBcmDigit>                       m_SmartRef_LHCb__MCBcmDigit;
    SmartRef<LHCb::MCCaloDigit>                      m_SmartRef_LHCb__MCCaloDigit;
    SmartRef<LHCb::MCCaloHit>                        m_SmartRef_LHCb__MCCaloHit;
    SmartRef<LHCb::MCExtendedHit>                    m_SmartRef_LHCb__MCExtendedHit;
    SmartRef<LHCb::MCHeader>                         m_SmartRef_LHCb__MCHeader;
    SmartRef<LHCb::MCHit>                            m_SmartRef_LHCb__MCHit;
    SmartRef<LHCb::MCMuonDigit>                      m_SmartRef_LHCb__MCMuonDigit;
    SmartRef<LHCb::MCOTDeposit>                      m_SmartRef_LHCb__MCOTDeposit;
    SmartRef<LHCb::MCOTTime>                         m_SmartRef_LHCb__MCOTTime;
    SmartRef<LHCb::MCParticle>                       m_SmartRef_LHCb__MCParticle;
    SmartRef<LHCb::MCProperty>                       m_SmartRef_LHCb__MCProperty;
    SmartRef<LHCb::MCRichDigit>                      m_SmartRef_LHCb__MCRichDigit;
    SmartRef<LHCb::MCRichDigitSummary>               m_SmartRef_LHCb__MCRichDigitSummary;
    SmartRef<LHCb::MCRichHit>                        m_SmartRef_LHCb__MCRichHit;
    SmartRef<LHCb::MCRichOpticalPhoton>              m_SmartRef_LHCb__MCRichOpticalPhoton;
    SmartRef<LHCb::MCRichSegment>                    m_SmartRef_LHCb__MCRichSegment;
    SmartRef<LHCb::MCRichTrack>                      m_SmartRef_LHCb__MCRichTrack;
    SmartRef<LHCb::MCSTDeposit>                      m_SmartRef_LHCb__MCSTDeposit;
    SmartRef<LHCb::MCSTDigit>                        m_SmartRef_LHCb__MCSTDigit;
    SmartRef<LHCb::MCSensPlaneHit>                   m_SmartRef_LHCb__MCSensPlaneHit;
    SmartRef<LHCb::MCUTDeposit>                      m_SmartRef_LHCb__MCUTDeposit;
    SmartRef<LHCb::MCUTDigit>                        m_SmartRef_LHCb__MCUTDigit;
    SmartRef<LHCb::MCVPDigit>                        m_SmartRef_LHCb__MCVPDigit;
    SmartRef<LHCb::MCVeloFE>                         m_SmartRef_LHCb__MCVeloFE;
    SmartRef<LHCb::MCVertex>                         m_SmartRef_LHCb__MCVertex;
    SmartRefVector<LHCb::MCBcmDigit>                 m_SmartRefVector_LHCb__MCBcmDigit;
    SmartRefVector<LHCb::MCCaloDigit>                m_SmartRefVector_LHCb__MCCaloDigit;
    SmartRefVector<LHCb::MCCaloHit>                  m_SmartRefVector_LHCb__MCCaloHit;
    SmartRefVector<LHCb::MCExtendedHit>              m_SmartRefVector_LHCb__MCExtendedHit;
    SmartRefVector<LHCb::MCHeader>                   m_SmartRefVector_LHCb__MCHeader;
    SmartRefVector<LHCb::MCHit>                      m_SmartRefVector_LHCb__MCHit;
    SmartRefVector<LHCb::MCMuonDigit>                m_SmartRefVector_LHCb__MCMuonDigit;
    SmartRefVector<LHCb::MCOTDeposit>                m_SmartRefVector_LHCb__MCOTDeposit;
    SmartRefVector<LHCb::MCOTTime>                   m_SmartRefVector_LHCb__MCOTTime;
    SmartRefVector<LHCb::MCParticle>                 m_SmartRefVector_LHCb__MCParticle;
    SmartRefVector<LHCb::MCProperty>                 m_SmartRefVector_LHCb__MCProperty;
    SmartRefVector<LHCb::MCRichDigit>                m_SmartRefVector_LHCb__MCRichDigit;
    SmartRefVector<LHCb::MCRichDigitSummary>         m_SmartRefVector_LHCb__MCRichDigitSummary;
    SmartRefVector<LHCb::MCRichHit>                  m_SmartRefVector_LHCb__MCRichHit;
    SmartRefVector<LHCb::MCRichOpticalPhoton>        m_SmartRefVector_LHCb__MCRichOpticalPhoton;
    SmartRefVector<LHCb::MCRichSegment>              m_SmartRefVector_LHCb__MCRichSegment;
    SmartRefVector<LHCb::MCRichTrack>                m_SmartRefVector_LHCb__MCRichTrack;
    SmartRefVector<LHCb::MCSTDeposit>                m_SmartRefVector_LHCb__MCSTDeposit;
    SmartRefVector<LHCb::MCSTDigit>                  m_SmartRefVector_LHCb__MCSTDigit;
    SmartRefVector<LHCb::MCSensPlaneHit>             m_SmartRefVector_LHCb__MCSensPlaneHit;
    SmartRefVector<LHCb::MCUTDeposit>                m_SmartRefVector_LHCb__MCUTDeposit;
    SmartRefVector<LHCb::MCUTDigit>                  m_SmartRefVector_LHCb__MCUTDigit;
    SmartRefVector<LHCb::MCVPDigit>                  m_SmartRefVector_LHCb__MCVPDigit;
    SmartRefVector<LHCb::MCVeloFE>                   m_SmartRefVector_LHCb__MCVeloFE;
    SmartRefVector<LHCb::MCVertex>                   m_SmartRefVector_LHCb__MCVertex;
    std::pair<LHCb::MCParticle*, double>             _i2;
    std::vector<LHCb::MCBcmDigit*>                   m_std_vector_LHCb__MCBcmDigitp;
    std::vector<LHCb::MCCaloDigit*>                  m_std_vector_LHCb__MCCaloDigitp;
    std::vector<LHCb::MCCaloHit*>                    m_std_vector_LHCb__MCCaloHitp;
    std::vector<LHCb::MCExtendedHit*>                m_std_vector_LHCb__MCExtendedHitp;
    std::vector<LHCb::MCHit*>                        m_std_vector_LHCb__MCHitp;
    std::vector<LHCb::MCMuonDigit*>                  m_std_vector_LHCb__MCMuonDigitp;
    std::vector<LHCb::MCMuonDigitInfo>               m_std_vector_LHCb__MCMuonDigitInfo;
    std::vector<LHCb::MCMuonHitHistory>              m_std_vector_LHCb__MCMuonHitHistory;
    std::vector<LHCb::MCOTDeposit*>                  m_std_vector_LHCb__MCOTDepositp;
    std::vector<LHCb::MCOTTime*>                     m_std_vector_LHCb__MCOTTimep;
    std::vector<LHCb::MCParticle*>                   m_std_vector_LHCb__MCParticlep;
    std::vector<LHCb::MCRichDigit*>                  m_std_vector_LHCb__MCRichDigitp;
    std::vector<LHCb::MCRichDigitSummary*>           m_std_vector_LHCb__MCRichDigitSummaryp;
    std::vector<LHCb::MCRichHit*>                    m_std_vector_LHCb__MCRichHitp;
    std::vector<LHCb::MCRichOpticalPhoton*>          m_std_vector_LHCb__MCRichOpticalPhotonp;
    std::vector<LHCb::MCRichSegment*>                m_std_vector_LHCb__MCRichSegmentp;
    std::vector<LHCb::MCRichTrack*>                  m_std_vector_LHCb__MCRichTrackp;
    std::vector<LHCb::MCSTDeposit*>                  m_std_vector_LHCb__MCSTDepositp;
    std::vector<LHCb::MCSTDigit*>                    m_std_vector_LHCb__MCSTDigitp;
    std::vector<LHCb::MCSensPlaneHit*>               m_std_vector_LHCb__MCSensPlaneHitp;
    std::vector<LHCb::MCUTDeposit*>                  m_std_vector_LHCb__MCUTDepositp;
    std::vector<LHCb::MCUTDigit*>                    m_std_vector_LHCb__MCUTDigitp;
    std::vector<LHCb::MCVPDigit*>                    m_std_vector_LHCb__MCVPDigitp;
    std::vector<LHCb::MCVeloFE*>                     m_std_vector_LHCb__MCVeloFEp;
    std::vector<LHCb::MCVertex*>                     m_std_vector_LHCb__MCVertexp;
    std::vector<SmartRef<LHCb::MCBcmDigit>>          m_std_vector_SmartRef_LHCb__MCBcmDigit;
    std::vector<SmartRef<LHCb::MCCaloDigit>>         m_std_vector_SmartRef_LHCb__MCCaloDigit;
    std::vector<SmartRef<LHCb::MCCaloHit>>           m_std_vector_SmartRef_LHCb__MCCaloHit;
    std::vector<SmartRef<LHCb::MCExtendedHit>>       m_std_vector_SmartRef_LHCb__MCExtendedHit;
    std::vector<SmartRef<LHCb::MCHeader>>            m_std_vector_SmartRef_LHCb__MCHeader;
    std::vector<SmartRef<LHCb::MCHit>>               m_std_vector_SmartRef_LHCb__MCHit;
    std::vector<SmartRef<LHCb::MCMuonDigit>>         m_std_vector_SmartRef_LHCb__MCMuonDigit;
    std::vector<SmartRef<LHCb::MCOTDeposit>>         m_std_vector_SmartRef_LHCb__MCOTDeposit;
    std::vector<SmartRef<LHCb::MCOTTime>>            m_std_vector_SmartRef_LHCb__MCOTTime;
    std::vector<SmartRef<LHCb::MCParticle>>          m_std_vector_SmartRef_LHCb__MCParticle;
    std::vector<SmartRef<LHCb::MCProperty>>          m_std_vector_SmartRef_LHCb__MCProperty;
    std::vector<SmartRef<LHCb::MCRichDigit>>         m_std_vector_SmartRef_LHCb__MCRichDigit;
    std::vector<SmartRef<LHCb::MCRichDigitSummary>>  m_std_vector_SmartRef_LHCb__MCRichDigitSummary;
    std::vector<SmartRef<LHCb::MCRichHit>>           m_std_vector_SmartRef_LHCb__MCRichHit;
    std::vector<SmartRef<LHCb::MCRichOpticalPhoton>> m_std_vector_SmartRef_LHCb__MCRichOpticalPhoton;
    std::vector<SmartRef<LHCb::MCRichSegment>>       m_std_vector_SmartRef_LHCb__MCRichSegment;
    std::vector<SmartRef<LHCb::MCRichTrack>>         m_std_vector_SmartRef_LHCb__MCRichTrack;
    std::vector<SmartRef<LHCb::MCSTDeposit>>         m_std_vector_SmartRef_LHCb__MCSTDeposit;
    std::vector<SmartRef<LHCb::MCSTDigit>>           m_std_vector_SmartRef_LHCb__MCSTDigit;
    std::vector<SmartRef<LHCb::MCSensPlaneHit>>      m_std_vector_SmartRef_LHCb__MCSensPlaneHit;
    std::vector<SmartRef<LHCb::MCUTDeposit>>         m_std_vector_SmartRef_LHCb__MCUTDeposit;
    std::vector<SmartRef<LHCb::MCUTDigit>>           m_std_vector_SmartRef_LHCb__MCUTDigit;
    std::vector<SmartRef<LHCb::MCVPDigit>>           m_std_vector_SmartRef_LHCb__MCVPDigit;
    std::vector<SmartRef<LHCb::MCVeloFE>>            m_std_vector_SmartRef_LHCb__MCVeloFE;
    std::vector<SmartRef<LHCb::MCVertex>>            m_std_vector_SmartRef_LHCb__MCVertex;
    std::vector<const LHCb::MCParticle*>             _i1;
    std::vector<const LHCb::MCVertex*>               _i3;
    // end instantiations
  };
} // namespace
