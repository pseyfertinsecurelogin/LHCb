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
#include "Event/STTELL1BoardErrorBank.h"
#include "Event/STTELL1Data.h"
#include "Event/STTELL1Error.h"
#include "Event/STTELL1EventInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/ObjectVector.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/SmartRefVector.h"
#include <vector>
// end include files

namespace {
  struct STTELL1Event_Instantiations {
    // begin instantiations
    KeyedContainer<LHCb::STTELL1BoardErrorBank>        m_KeyedContainer_LHCb__STTELL1BoardErrorBank;
    KeyedContainer<LHCb::STTELL1Data>                  m_KeyedContainer_LHCb__STTELL1Data;
    ObjectVector<LHCb::STTELL1BoardErrorBank>          m_ObjectVector_LHCb__STTELL1BoardErrorBank;
    ObjectVector<LHCb::STTELL1Data>                    m_ObjectVector_LHCb__STTELL1Data;
    SmartRef<LHCb::STTELL1BoardErrorBank>              m_SmartRef_LHCb__STTELL1BoardErrorBank;
    SmartRef<LHCb::STTELL1Data>                        m_SmartRef_LHCb__STTELL1Data;
    SmartRefVector<LHCb::STTELL1BoardErrorBank>        m_SmartRefVector_LHCb__STTELL1BoardErrorBank;
    SmartRefVector<LHCb::STTELL1Data>                  m_SmartRefVector_LHCb__STTELL1Data;
    std::vector<LHCb::STTELL1BoardErrorBank*>          m_std_vector_LHCb__STTELL1BoardErrorBankp;
    std::vector<LHCb::STTELL1Data*>                    m_std_vector_LHCb__STTELL1Datap;
    std::vector<SmartRef<LHCb::STTELL1BoardErrorBank>> m_std_vector_SmartRef_LHCb__STTELL1BoardErrorBank;
    std::vector<SmartRef<LHCb::STTELL1Data>>           m_std_vector_SmartRef_LHCb__STTELL1Data;
    // end instantiations
  };
} // namespace
