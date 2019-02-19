/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// begin include files
#include "Event/ODIN.h"
#include "Event/RawEvent.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct _Instantiations {
    // begin instantiations
    SmartRef<LHCb::ODIN>              m_SmartRef_LHCb__ODIN;
    SmartRefVector<LHCb::ODIN>        m_SmartRefVector_LHCb__ODIN;
    std::vector<SmartRef<LHCb::ODIN>> m_std_vector_SmartRef_LHCb__ODIN;
    // end instantiations
  };
} // namespace
