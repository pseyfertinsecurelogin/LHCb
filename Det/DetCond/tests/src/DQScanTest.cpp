/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Local custom parsers must be defined very early in the file.
#include "GaudiKernel/ParsersFactory.h"

namespace Gaudi {
  namespace Parsers {
    // Note: to be kept in sync with the property in DetCondTest::DQScanTest
    StatusCode parse(std::vector<std::pair<unsigned int, unsigned int> >& result, const std::string& input) {
      return Gaudi::Parsers::parse_(result, input);
    }
  }
}

// Include files

#include "DetCond/ICondDBReader.h"

// local
#include "DQScanTest.h"


namespace {
  inline long long s2ns(unsigned int s) {
    return static_cast<long long>(s) * 1000000000;
  }
}

// ----------------------------------------------------------------------------
// Implementation file for class: DQScanTest
//
// 31/01/2012: Marco Clemencic
// ----------------------------------------------------------------------------
DECLARE_COMPONENT( DetCondTest::DQScanTest )

namespace DetCondTest {
// ============================================================================
// Initialization
// ============================================================================
StatusCode DQScanTest::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_scanner = tool<IDQScanner>(m_DQScannerName);

  m_iovs.clear();
  for(IOVPropType &iov: m_iovsProp) {
    m_iovs.emplace_back(Gaudi::Time(s2ns(iov.first)), Gaudi::Time(s2ns(iov.second)));
  }

  return StatusCode::SUCCESS;
}

// ============================================================================
// Main execution
// ============================================================================
StatusCode DQScanTest::execute() {
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  info() << "Execute" << endmsg;

  for(const auto& iov: m_iovs) {
    always() << "Process IOV " << iov.since << " -> " << iov.until << endmsg;
    always() << "-> Flags: " << m_scanner->scan(iov.since, iov.until) << endmsg;
  }

  return StatusCode::SUCCESS;
}

// ============================================================================
// Finalize
// ============================================================================
StatusCode DQScanTest::finalize() {
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  if (release(m_scanner).isFailure()) {
    warning() << "Failed to release tool " << m_DQScannerName << endmsg;
  }
  m_scanner = nullptr;

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

// ============================================================================
} // namespace DetCondTest
