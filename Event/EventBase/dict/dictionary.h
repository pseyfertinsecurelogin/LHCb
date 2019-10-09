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
#include "Event/IntLink.h"
#include "Event/ProcessHeader.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct EventBase_Instantiations {
    // begin instantiations
    SmartRef<LHCb::IntLink>                    m_SmartRef_LHCb__IntLink;
    SmartRef<LHCb::ProcessHeader>              m_SmartRef_LHCb__ProcessHeader;
    SmartRefVector<LHCb::IntLink>              m_SmartRefVector_LHCb__IntLink;
    SmartRefVector<LHCb::ProcessHeader>        m_SmartRefVector_LHCb__ProcessHeader;
    std::vector<SmartRef<LHCb::IntLink>>       m_std_vector_SmartRef_LHCb__IntLink;
    std::vector<SmartRef<LHCb::ProcessHeader>> m_std_vector_SmartRef_LHCb__ProcessHeader;
    // end instantiations
  };
} // namespace
