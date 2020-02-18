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
// from LHCb core
#include "Event/ODIN.h"
// STL
#include <cmath>
#include <cstdint>
#include <limits>
#include <string>

#include "DeterministicPrescalerGenerator.h"

/** @class DeterministicPrescaler DeterministicPrescaler.cpp
 *
 * @brief Return a positive decision for a configurable fraction of executions.
 *
 * A random seed is set based on the SeedName property, and the accept fraction
 * is governed by the AcceptFraction property. For each execution a random
 * number distributed in [0, 1] is drawn based on the seed and the ODIN event
 * and run numbers. The filter passes if this number is smaller than the accept
 * fraction. An accept fraction of zero results in the filter always returning
 * false.
 *
 */
class DeterministicPrescaler final : public Gaudi::Functional::FilterPredicate<bool( const LHCb::ODIN& )> {
public:
  DeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator )
      : FilterPredicate( name, pSvcLocator, KeyValue{"ODINLocation", LHCb::ODINLocation::Default} ) {}

  StatusCode initialize() override {
    const StatusCode sc = FilterPredicate::initialize();
    if ( !sc ) return sc;
    if ( m_seed_name.empty() ) { return Error( "Required property SeedName is empty" ); }
    if ( msgLevel( MSG::DEBUG ) ) debug() << " generated initial value " << m_initial << endmsg;
    if ( m_acc != std::numeric_limits<uint32_t>::max() ) {
      info() << "Prescaling events; keeping " << m_accFrac.value() << " of events " << endmsg;
    }
    return sc;
  }

  bool operator()( const LHCb::ODIN& odin ) const override {
    auto accept = [&]( const LHCb::ODIN& odin ) {
      auto x = mix64( mix32( mix64( m_initial, odin.gpsTime() ), odin.runNumber() ), odin.eventNumber() );
      if ( msgLevel( MSG::DEBUG ) )
        debug() << "  gpsTime: " << odin.gpsTime() << "  run#: " << odin.runNumber() << "  evt#: " << odin.eventNumber()
                << "  -->  " << x << endmsg;
      // at this point, we assume 'x' to be uniformly distributed in [0,0xffffffff]
      // (and yes, this was verified to be sufficiently true on a sample of 10K MC events ;-)
      return x < m_acc;
    };
    const bool acc = ( ( m_acc == std::numeric_limits<uint32_t>::max() ) || ( m_acc != 0 && accept( odin ) ) );
    m_accept += acc;
    if ( msgLevel( MSG::DEBUG ) ) debug() << ( acc ? "Accepted" : "Rejected" ) << endmsg;
    return acc;
  }

private:
  Gaudi::Property<std::string> m_seed_name{this, "SeedName", "",
                                           [=]( Property& ) {
                                             if ( m_seed_name.empty() ) {
                                               throw GaudiException( "Required property SeedName is empty", name(),
                                                                     StatusCode::FAILURE );
                                             }
                                             m_initial = mixString( m_seed_name.size(), m_seed_name );
                                           },
                                           "String used to seed pseudo-random number generation"};

  /// initial seed unique to this instance (computed from the name)
  std::uint32_t m_initial{0};
  /// integer representation of the above
  std::uint32_t m_acc{std::numeric_limits<uint32_t>::max()};

  mutable Gaudi::Accumulators::BinomialCounter<> m_accept{this, "#accept"};

  /// fraction of input events to accept...
  Gaudi::Property<double> m_accFrac{
      this, "AcceptFraction", 1, [this]( const Property& ) {
        m_acc = ( m_accFrac <= 0 ? 0
                                 : m_accFrac >= 1 ? std::numeric_limits<uint32_t>::max()
                                                  : uint32_t( m_accFrac * std::numeric_limits<uint32_t>::max() ) );
        if ( msgLevel( MSG::DEBUG ) ) debug() << "frac: " << m_accFrac << " acc: 0x" << std::hex << m_acc << endmsg;
      }};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DeterministicPrescaler )
