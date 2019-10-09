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
#include "Event/IOFSR.h"
#include "Event/ProcessFSR.h"
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
  struct FSREvent_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::ProcessFSR>        m_KeyedContainer_LHCb__ProcessFSR;
    ObjectVector<LHCb::ProcessFSR>          m_ObjectVector_LHCb__ProcessFSR;
    SmartRef<LHCb::ProcessFSR>              m_SmartRef_LHCb__ProcessFSR;
    SmartRefVector<LHCb::ProcessFSR>        m_SmartRefVector_LHCb__ProcessFSR;
    std::vector<LHCb::ProcessFSR*>          m_std_vector_LHCb__ProcessFSRp;
    std::vector<SmartRef<LHCb::ProcessFSR>> m_std_vector_SmartRef_LHCb__ProcessFSR;
    // end instantiations
  };
} // namespace
