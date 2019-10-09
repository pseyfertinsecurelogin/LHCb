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
#include "Event/UTTELL1BoardErrorBank.h"
#include "Event/UTTELL1Data.h"
#include "Event/UTTELL1Error.h"
#include "Event/UTTELL1EventInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct UTTELL1Event_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::UTTELL1BoardErrorBank>        m_KeyedContainer_LHCb__UTTELL1BoardErrorBank;
    KeyedContainer<LHCb::UTTELL1Data>                  m_KeyedContainer_LHCb__UTTELL1Data;
    ObjectVector<LHCb::UTTELL1BoardErrorBank>          m_ObjectVector_LHCb__UTTELL1BoardErrorBank;
    ObjectVector<LHCb::UTTELL1Data>                    m_ObjectVector_LHCb__UTTELL1Data;
    SmartRef<LHCb::UTTELL1BoardErrorBank>              m_SmartRef_LHCb__UTTELL1BoardErrorBank;
    SmartRef<LHCb::UTTELL1Data>                        m_SmartRef_LHCb__UTTELL1Data;
    SmartRefVector<LHCb::UTTELL1BoardErrorBank>        m_SmartRefVector_LHCb__UTTELL1BoardErrorBank;
    SmartRefVector<LHCb::UTTELL1Data>                  m_SmartRefVector_LHCb__UTTELL1Data;
    std::vector<LHCb::UTTELL1BoardErrorBank*>          m_std_vector_LHCb__UTTELL1BoardErrorBankp;
    std::vector<LHCb::UTTELL1Data*>                    m_std_vector_LHCb__UTTELL1Datap;
    std::vector<SmartRef<LHCb::UTTELL1BoardErrorBank>> m_std_vector_SmartRef_LHCb__UTTELL1BoardErrorBank;
    std::vector<SmartRef<LHCb::UTTELL1Data>>           m_std_vector_SmartRef_LHCb__UTTELL1Data;
    // end instantiations
  };
} // namespace
