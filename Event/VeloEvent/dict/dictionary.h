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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "VeloEvent/InternalVeloCluster.h"
#include "VeloEvent/VeloDecodeStatus.h"
#include "VeloEvent/VeloFullFPGADigit.h"
#include "VeloEvent/VeloTELL1Data.h"
#include <vector>
// end include files

namespace {
  struct VeloEvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::InternalVeloCluster>        m_KeyedContainer_LHCb__InternalVeloCluster;
    KeyedContainer<LHCb::VeloFullFPGADigit>          m_KeyedContainer_LHCb__VeloFullFPGADigit;
    KeyedContainer<LHCb::VeloTELL1Data>              m_KeyedContainer_LHCb__VeloTELL1Data;
    KeyedObject<LHCb::VeloChannelID>                 m_KeyedObject_LHCb__VeloChannelID;
    ObjectVector<LHCb::InternalVeloCluster>          m_ObjectVector_LHCb__InternalVeloCluster;
    ObjectVector<LHCb::VeloFullFPGADigit>            m_ObjectVector_LHCb__VeloFullFPGADigit;
    ObjectVector<LHCb::VeloTELL1Data>                m_ObjectVector_LHCb__VeloTELL1Data;
    SmartRef<LHCb::InternalVeloCluster>              m_SmartRef_LHCb__InternalVeloCluster;
    SmartRef<LHCb::VeloFullFPGADigit>                m_SmartRef_LHCb__VeloFullFPGADigit;
    SmartRef<LHCb::VeloTELL1Data>                    m_SmartRef_LHCb__VeloTELL1Data;
    SmartRefVector<LHCb::InternalVeloCluster>        m_SmartRefVector_LHCb__InternalVeloCluster;
    SmartRefVector<LHCb::VeloFullFPGADigit>          m_SmartRefVector_LHCb__VeloFullFPGADigit;
    SmartRefVector<LHCb::VeloTELL1Data>              m_SmartRefVector_LHCb__VeloTELL1Data;
    std::vector<LHCb::InternalVeloCluster*>          m_std_vector_LHCb__InternalVeloClusterp;
    std::vector<LHCb::VeloFullFPGADigit*>            m_std_vector_LHCb__VeloFullFPGADigitp;
    std::vector<LHCb::VeloTELL1Data*>                m_std_vector_LHCb__VeloTELL1Datap;
    std::vector<SmartRef<LHCb::InternalVeloCluster>> m_std_vector_SmartRef_LHCb__InternalVeloCluster;
    std::vector<SmartRef<LHCb::VeloFullFPGADigit>>   m_std_vector_SmartRef_LHCb__VeloFullFPGADigit;
    std::vector<SmartRef<LHCb::VeloTELL1Data>>       m_std_vector_SmartRef_LHCb__VeloTELL1Data;
    // end instantiations
  };
} // namespace
