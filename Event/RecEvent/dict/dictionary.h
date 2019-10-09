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
#include "Event/CaloCluster.h"
#include "Event/CaloClusterEntry.h"
#include "Event/CaloHypo.h"
#include "Event/CaloPosition.h"
#include "Event/MuonCluster.h"
#include "Event/MuonCoord.h"
#include "Event/MuonPID.h"
#include "Event/PrimaryVertex.h"
#include "Event/PrimaryVertexTrack.h"
#include "Event/ProcStatus.h"
#include "Event/ProtoParticle.h"
#include "Event/RecHeader.h"
#include "Event/RecSummary.h"
#include "Event/RecVertex.h"
#include "Event/RichPID.h"
#include "Event/RichSummaryPhoton.h"
#include "Event/RichSummaryRadSegment.h"
#include "Event/RichSummaryTrack.h"
#include "Event/STSummary.h"
#include "Event/TwoProngVertex.h"
#include "Event/UTSummary.h"
#include "Event/VertexBase.h"
#include "Event/WeightsVector.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct RecEvent_Instantiations {
    // begin instantiations
    Gaudi::SymMatrix9x9                           _i5;
    KeyedContainer<LHCb::CaloCluster>             m_KeyedContainer_LHCb__CaloCluster;
    KeyedContainer<LHCb::CaloHypo>                m_KeyedContainer_LHCb__CaloHypo;
    KeyedContainer<LHCb::MuonCluster>             m_KeyedContainer_LHCb__MuonCluster;
    KeyedContainer<LHCb::MuonCoord>               m_KeyedContainer_LHCb__MuonCoord;
    KeyedContainer<LHCb::MuonPID>                 m_KeyedContainer_LHCb__MuonPID;
    KeyedContainer<LHCb::ProtoParticle>           m_KeyedContainer_LHCb__ProtoParticle;
    KeyedContainer<LHCb::RecVertex>               m_KeyedContainer_LHCb__RecVertex;
    KeyedContainer<LHCb::RichPID>                 m_KeyedContainer_LHCb__RichPID;
    KeyedContainer<LHCb::RichSummaryTrack>        m_KeyedContainer_LHCb__RichSummaryTrack;
    KeyedContainer<LHCb::TwoProngVertex>          m_KeyedContainer_LHCb__TwoProngVertex;
    KeyedContainer<LHCb::VertexBase>              m_KeyedContainer_LHCb__VertexBase;
    KeyedContainer<LHCb::WeightsVector>           m_KeyedContainer_LHCb__WeightsVector;
    ObjectVector<LHCb::CaloCluster>               m_ObjectVector_LHCb__CaloCluster;
    ObjectVector<LHCb::CaloHypo>                  m_ObjectVector_LHCb__CaloHypo;
    ObjectVector<LHCb::MuonCluster>               m_ObjectVector_LHCb__MuonCluster;
    ObjectVector<LHCb::MuonCoord>                 m_ObjectVector_LHCb__MuonCoord;
    ObjectVector<LHCb::MuonPID>                   m_ObjectVector_LHCb__MuonPID;
    ObjectVector<LHCb::ProtoParticle>             m_ObjectVector_LHCb__ProtoParticle;
    ObjectVector<LHCb::RecVertex>                 m_ObjectVector_LHCb__RecVertex;
    ObjectVector<LHCb::RichPID>                   m_ObjectVector_LHCb__RichPID;
    ObjectVector<LHCb::RichSummaryTrack>          m_ObjectVector_LHCb__RichSummaryTrack;
    ObjectVector<LHCb::TwoProngVertex>            m_ObjectVector_LHCb__TwoProngVertex;
    ObjectVector<LHCb::VertexBase>                m_ObjectVector_LHCb__VertexBase;
    ObjectVector<LHCb::WeightsVector>             m_ObjectVector_LHCb__WeightsVector;
    SmartRef<LHCb::CaloCluster>                   m_SmartRef_LHCb__CaloCluster;
    SmartRef<LHCb::CaloHypo>                      m_SmartRef_LHCb__CaloHypo;
    SmartRef<LHCb::MuonCluster>                   m_SmartRef_LHCb__MuonCluster;
    SmartRef<LHCb::MuonCoord>                     m_SmartRef_LHCb__MuonCoord;
    SmartRef<LHCb::MuonPID>                       m_SmartRef_LHCb__MuonPID;
    SmartRef<LHCb::ProcStatus>                    m_SmartRef_LHCb__ProcStatus;
    SmartRef<LHCb::ProtoParticle>                 m_SmartRef_LHCb__ProtoParticle;
    SmartRef<LHCb::RecHeader>                     m_SmartRef_LHCb__RecHeader;
    SmartRef<LHCb::RecSummary>                    m_SmartRef_LHCb__RecSummary;
    SmartRef<LHCb::RecVertex>                     m_SmartRef_LHCb__RecVertex;
    SmartRef<LHCb::RichPID>                       m_SmartRef_LHCb__RichPID;
    SmartRef<LHCb::RichSummaryTrack>              m_SmartRef_LHCb__RichSummaryTrack;
    SmartRef<LHCb::STSummary>                     m_SmartRef_LHCb__STSummary;
    SmartRef<LHCb::TwoProngVertex>                m_SmartRef_LHCb__TwoProngVertex;
    SmartRef<LHCb::UTSummary>                     m_SmartRef_LHCb__UTSummary;
    SmartRef<LHCb::VertexBase>                    m_SmartRef_LHCb__VertexBase;
    SmartRef<LHCb::WeightsVector>                 m_SmartRef_LHCb__WeightsVector;
    SmartRefVector<LHCb::CaloCluster>             m_SmartRefVector_LHCb__CaloCluster;
    SmartRefVector<LHCb::CaloHypo>                m_SmartRefVector_LHCb__CaloHypo;
    SmartRefVector<LHCb::MuonCluster>             m_SmartRefVector_LHCb__MuonCluster;
    SmartRefVector<LHCb::MuonCoord>               m_SmartRefVector_LHCb__MuonCoord;
    SmartRefVector<LHCb::MuonPID>                 m_SmartRefVector_LHCb__MuonPID;
    SmartRefVector<LHCb::ProcStatus>              m_SmartRefVector_LHCb__ProcStatus;
    SmartRefVector<LHCb::ProtoParticle>           m_SmartRefVector_LHCb__ProtoParticle;
    SmartRefVector<LHCb::RecHeader>               m_SmartRefVector_LHCb__RecHeader;
    SmartRefVector<LHCb::RecSummary>              m_SmartRefVector_LHCb__RecSummary;
    SmartRefVector<LHCb::RecVertex>               m_SmartRefVector_LHCb__RecVertex;
    SmartRefVector<LHCb::RichPID>                 m_SmartRefVector_LHCb__RichPID;
    SmartRefVector<LHCb::RichSummaryTrack>        m_SmartRefVector_LHCb__RichSummaryTrack;
    SmartRefVector<LHCb::STSummary>               m_SmartRefVector_LHCb__STSummary;
    SmartRefVector<LHCb::TwoProngVertex>          m_SmartRefVector_LHCb__TwoProngVertex;
    SmartRefVector<LHCb::UTSummary>               m_SmartRefVector_LHCb__UTSummary;
    SmartRefVector<LHCb::VertexBase>              m_SmartRefVector_LHCb__VertexBase;
    SmartRefVector<LHCb::WeightsVector>           m_SmartRefVector_LHCb__WeightsVector;
    std::pair<int, float>                         _i3;
    std::vector<LHCb::CaloCluster*>               m_std_vector_LHCb__CaloClusterp;
    std::vector<LHCb::CaloClusterEntry>           m_std_vector_LHCb__CaloClusterEntry;
    std::vector<LHCb::CaloHypo*>                  m_std_vector_LHCb__CaloHypop;
    std::vector<LHCb::MuonCluster*>               m_std_vector_LHCb__MuonClusterp;
    std::vector<LHCb::MuonCoord*>                 m_std_vector_LHCb__MuonCoordp;
    std::vector<LHCb::MuonPID*>                   m_std_vector_LHCb__MuonPIDp;
    std::vector<LHCb::ProtoParticle*>             m_std_vector_LHCb__ProtoParticlep;
    std::vector<LHCb::RecVertex*>                 m_std_vector_LHCb__RecVertexp;
    std::vector<LHCb::RichPID*>                   m_std_vector_LHCb__RichPIDp;
    std::vector<LHCb::RichSummaryPhoton>          m_std_vector_LHCb__RichSummaryPhoton;
    std::vector<LHCb::RichSummaryRadSegment>      m_std_vector_LHCb__RichSummaryRadSegment;
    std::vector<LHCb::RichSummaryTrack*>          m_std_vector_LHCb__RichSummaryTrackp;
    std::vector<LHCb::TwoProngVertex*>            m_std_vector_LHCb__TwoProngVertexp;
    std::vector<LHCb::VertexBase*>                m_std_vector_LHCb__VertexBasep;
    std::vector<LHCb::WeightsVector*>             m_std_vector_LHCb__WeightsVectorp;
    std::vector<SmartRef<LHCb::CaloCluster>>      m_std_vector_SmartRef_LHCb__CaloCluster;
    std::vector<SmartRef<LHCb::CaloHypo>>         m_std_vector_SmartRef_LHCb__CaloHypo;
    std::vector<SmartRef<LHCb::MuonCluster>>      m_std_vector_SmartRef_LHCb__MuonCluster;
    std::vector<SmartRef<LHCb::MuonCoord>>        m_std_vector_SmartRef_LHCb__MuonCoord;
    std::vector<SmartRef<LHCb::MuonPID>>          m_std_vector_SmartRef_LHCb__MuonPID;
    std::vector<SmartRef<LHCb::ProcStatus>>       m_std_vector_SmartRef_LHCb__ProcStatus;
    std::vector<SmartRef<LHCb::ProtoParticle>>    m_std_vector_SmartRef_LHCb__ProtoParticle;
    std::vector<SmartRef<LHCb::RecHeader>>        m_std_vector_SmartRef_LHCb__RecHeader;
    std::vector<SmartRef<LHCb::RecSummary>>       m_std_vector_SmartRef_LHCb__RecSummary;
    std::vector<SmartRef<LHCb::RecVertex>>        m_std_vector_SmartRef_LHCb__RecVertex;
    std::vector<SmartRef<LHCb::RichPID>>          m_std_vector_SmartRef_LHCb__RichPID;
    std::vector<SmartRef<LHCb::RichSummaryTrack>> m_std_vector_SmartRef_LHCb__RichSummaryTrack;
    std::vector<SmartRef<LHCb::STSummary>>        m_std_vector_SmartRef_LHCb__STSummary;
    std::vector<SmartRef<LHCb::TwoProngVertex>>   m_std_vector_SmartRef_LHCb__TwoProngVertex;
    std::vector<SmartRef<LHCb::UTSummary>>        m_std_vector_SmartRef_LHCb__UTSummary;
    std::vector<SmartRef<LHCb::VertexBase>>       m_std_vector_SmartRef_LHCb__VertexBase;
    std::vector<SmartRef<LHCb::WeightsVector>>    m_std_vector_SmartRef_LHCb__WeightsVector;
    std::vector<const LHCb::RecVertex*>           _i1;
    std::vector<const LHCb::VertexBase*>          _i2;
    std::vector<std::pair<int, float>>            _i4;
    // end instantiations
  };
} // namespace
