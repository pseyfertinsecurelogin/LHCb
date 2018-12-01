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
// Gaudi
#include "GaudiAlg/FilterPredicate.h"

// STL
#include <string>
#include <cmath>
#include <cstdint>
#include <limits>

// from LHCb core
#include "Event/ODIN.h"

class DeterministicPrescaler final : public Gaudi::Functional::FilterPredicate<bool(const LHCb::ODIN&)>
{
public:
  DeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;
  bool operator()(const LHCb::ODIN&) const override;

private:

  /// fraction of input events to accept...
  Gaudi::Property<double>                  m_accFrac
  { this,  "AcceptFraction" , 1,  [this](const Property&) {
    m_acc = ( m_accFrac<=0 ? 0
            : m_accFrac>=1 ? std::numeric_limits<uint32_t>::max()
            : uint32_t( m_accFrac*std::numeric_limits<uint32_t>::max())
            );
    if( msgLevel(MSG::DEBUG) )
      debug() << "frac: " << m_accFrac << " acc: 0x" << std::hex << m_acc << endmsg;
  }};

  /// integer representation of the above
  std::uint32_t         m_acc{std::numeric_limits<uint32_t>::max()};

  /// initial seed unique to this instance (computed from the name)
  std::uint32_t         m_initial{0};

  // acahe pointer to counter, to avoid map look ups
  StatEntity*             m_counter = nullptr;

};
