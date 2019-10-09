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
#include "Event/BeamParameters.h"
#include "Event/CrossSectionsFSR.h"
#include "Event/GenCollision.h"
#include "Event/GenCountersFSR.h"
#include "Event/GenFSR.h"
#include "Event/GenHeader.h"
#include "Event/HepMCEvent.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include "HepMC/GenVertex.h"
#include <vector>

// end include files

namespace {
  struct GenEvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::GenCollision>            m_KeyedContainer_LHCb__GenCollision;
    KeyedContainer<LHCb::HepMCEvent>              m_KeyedContainer_LHCb__HepMCEvent;
    ObjectVector<LHCb::GenCollision>              m_ObjectVector_LHCb__GenCollision;
    ObjectVector<LHCb::HepMCEvent>                m_ObjectVector_LHCb__HepMCEvent;
    SmartRef<LHCb::BeamParameters>                m_SmartRef_LHCb__BeamParameters;
    SmartRef<LHCb::CrossSectionsFSR>              m_SmartRef_LHCb__CrossSectionsFSR;
    SmartRef<LHCb::GenCollision>                  m_SmartRef_LHCb__GenCollision;
    SmartRef<LHCb::GenCountersFSR>                m_SmartRef_LHCb__GenCountersFSR;
    SmartRef<LHCb::GenHeader>                     m_SmartRef_LHCb__GenHeader;
    SmartRef<LHCb::HepMCEvent>                    m_SmartRef_LHCb__HepMCEvent;
    SmartRefVector<LHCb::BeamParameters>          m_SmartRefVector_LHCb__BeamParameters;
    SmartRefVector<LHCb::CrossSectionsFSR>        m_SmartRefVector_LHCb__CrossSectionsFSR;
    SmartRefVector<LHCb::GenCollision>            m_SmartRefVector_LHCb__GenCollision;
    SmartRefVector<LHCb::GenCountersFSR>          m_SmartRefVector_LHCb__GenCountersFSR;
    SmartRefVector<LHCb::GenHeader>               m_SmartRefVector_LHCb__GenHeader;
    SmartRefVector<LHCb::HepMCEvent>              m_SmartRefVector_LHCb__HepMCEvent;
    std::vector<LHCb::GenCollision*>              m_std_vector_LHCb__GenCollisionp;
    std::vector<LHCb::HepMCEvent*>                m_std_vector_LHCb__HepMCEventp;
    std::vector<SmartRef<LHCb::BeamParameters>>   m_std_vector_SmartRef_LHCb__BeamParameters;
    std::vector<SmartRef<LHCb::CrossSectionsFSR>> m_std_vector_SmartRef_LHCb__CrossSectionsFSR;
    std::vector<SmartRef<LHCb::GenCollision>>     m_std_vector_SmartRef_LHCb__GenCollision;
    std::vector<SmartRef<LHCb::GenCountersFSR>>   m_std_vector_SmartRef_LHCb__GenCountersFSR;
    std::vector<SmartRef<LHCb::GenHeader>>        m_std_vector_SmartRef_LHCb__GenHeader;
    std::vector<SmartRef<LHCb::HepMCEvent>>       m_std_vector_SmartRef_LHCb__HepMCEvent;
    // end instantiations
  };
} // namespace
