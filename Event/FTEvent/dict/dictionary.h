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
#include "Event/FTCluster.h"
#include "Event/FTLiteCluster.h"
#include "Event/MCFTDeposit.h"
#include "Event/MCFTDigit.h"
#include "Event/MCFTPhoton.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct FTEvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::FTCluster>          m_KeyedContainer_LHCb__FTCluster;
    KeyedContainer<LHCb::MCFTDigit>          m_KeyedContainer_LHCb__MCFTDigit;
    KeyedObject<LHCb::FTChannelID>           m_KeyedObject_LHCb__FTChannelID;
    ObjectVector<LHCb::FTCluster>            m_ObjectVector_LHCb__FTCluster;
    ObjectVector<LHCb::MCFTDeposit>          m_ObjectVector_LHCb__MCFTDeposit;
    ObjectVector<LHCb::MCFTDigit>            m_ObjectVector_LHCb__MCFTDigit;
    ObjectVector<LHCb::MCFTPhoton>           m_ObjectVector_LHCb__MCFTPhoton;
    SmartRef<LHCb::FTCluster>                m_SmartRef_LHCb__FTCluster;
    SmartRef<LHCb::MCFTDeposit>              m_SmartRef_LHCb__MCFTDeposit;
    SmartRef<LHCb::MCFTDigit>                m_SmartRef_LHCb__MCFTDigit;
    SmartRef<LHCb::MCFTPhoton>               m_SmartRef_LHCb__MCFTPhoton;
    SmartRefVector<LHCb::FTCluster>          m_SmartRefVector_LHCb__FTCluster;
    SmartRefVector<LHCb::MCFTDeposit>        m_SmartRefVector_LHCb__MCFTDeposit;
    SmartRefVector<LHCb::MCFTDigit>          m_SmartRefVector_LHCb__MCFTDigit;
    SmartRefVector<LHCb::MCFTPhoton>         m_SmartRefVector_LHCb__MCFTPhoton;
    std::vector<LHCb::FTCluster*>            m_std_vector_LHCb__FTClusterp;
    std::vector<LHCb::MCFTDeposit*>          m_std_vector_LHCb__MCFTDepositp;
    std::vector<LHCb::MCFTDigit*>            m_std_vector_LHCb__MCFTDigitp;
    std::vector<LHCb::MCFTPhoton*>           m_std_vector_LHCb__MCFTPhotonp;
    std::vector<SmartRef<LHCb::FTCluster>>   m_std_vector_SmartRef_LHCb__FTCluster;
    std::vector<SmartRef<LHCb::MCFTDeposit>> m_std_vector_SmartRef_LHCb__MCFTDeposit;
    std::vector<SmartRef<LHCb::MCFTDigit>>   m_std_vector_SmartRef_LHCb__MCFTDigit;
    std::vector<SmartRef<LHCb::MCFTPhoton>>  m_std_vector_SmartRef_LHCb__MCFTPhoton;
    // end instantiations
  };
} // namespace
