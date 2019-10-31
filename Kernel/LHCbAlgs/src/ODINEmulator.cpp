/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "boost/integer_traits.hpp"
#include <algorithm>
#include <map>

#include "GaudiAlg/Transformer.h"
#include "GaudiKernel/GaudiException.h"

#include "Event/ODIN.h"

#include "DeterministicPrescalerGenerator.h"

using namespace LHCb;
using EventTypes = LHCb::ODIN::EventTypes;

namespace {
  static const std::map<EventTypes, const char*> table = {{EventTypes::NoBias, "NoBias"}, {EventTypes::Lumi, "Lumi"}};

  using accfrac_t                          = std::map<std::string, std::vector<double>>;
  const accfrac_t DEFAULT_ACCEPT_FRACTIONS = {{table.at( EventTypes::NoBias ), {0, 0, 0, 0}},
                                              {table.at( EventTypes::Lumi ), {0, 0, 0, 0}}};

  EventTypes parse( const std::string& s ) {
    for ( const auto& p : table ) {
      if ( s == p.second ) return p.first;
    }
    throw GaudiException( "Could not parse string to EventTypes value.", "ODINEmulator", StatusCode::FAILURE );
    return EventTypes::Physics; // can never reach this line
  }
} // namespace

/** @class ODINEmulator ODINEmulator.cpp
 *
 * \brief Emulate ODIN
 *
 * The following parts of ODIN are emulated:
 * - eventNumber: incremented starting from 0 (not thread safe!)
 * - bunchCrossingType: either preserved (default) or set to a constant
 * - eventType: set according to the accept fractions.
 *   The deterministic prescaler machinery is used, with independent
 *   seeds for every combination of event type and bxtype.
 * All other fields are preserved.
 *
 * You must specify the location of the emulated ODIN with
 * OutputODINLocation. Optionally, you can configure
 * - the accept fractions for every combination of event type and
     bunch crossing type (default as for standard physics running),
 * - whether to override the bunch crossing type (no by default,
     use with care),
 * - whether to set filter passed based on the emulated event type
     (yes by default), and
 * - whether to overwrite the event number with a new monotonically
 *   increasing sequence. In multithreaded mode, the assignment of
 *   numbers is not deterministic.
 * - the ODIN input location.
 *
 */
class ODINEmulator : public Gaudi::Functional::Transformer<ODIN( const ODIN& )> {
public:
  ODINEmulator( const std::string& name, ISvcLocator* pSvcLocator )
      : Transformer( name, pSvcLocator, KeyValue{"InputODINLocation", ODINLocation::Default},
                     KeyValue{"OutputODINLocation", ""} ) {
    m_accFrac.declareUpdateHandler( [this]( Property& ) -> void {
      for ( auto& p : m_accFrac ) {
        for ( unsigned int i = 0; i < p.second.size(); ++i ) {
          auto accFrac = p.second[i];
          auto acc =
              ( accFrac <= 0 ? 0
                             : accFrac >= 1 ? boost::integer_traits<uint32_t>::const_max
                                            : boost::uint32_t( accFrac * boost::integer_traits<uint32_t>::const_max ) );
          m_acc[static_cast<uint32_t>( parse( p.first ) )][i] = acc;
          if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
            debug() << "frac: " << accFrac << " acc: 0x" << std::hex << acc << endmsg;
        }
      }
    } );
    m_accFrac.useUpdateHandler();
  };

  StatusCode initialize() override;
  StatusCode finalize() override;

  ODIN operator()( const ODIN& ) const override;

private:
  mutable Gaudi::Accumulators::Counter<uint64_t>  m_eventNumber{this, "Next event number."};
  std::map<unsigned int, std::array<uint32_t, 4>> m_initial;
  std::map<unsigned int, std::array<uint32_t, 4>> m_acc;
  mutable std::map<unsigned int, std::array<Gaudi::Accumulators::BinomialCounter<uint32_t>, 4>> m_counters;

  Gaudi::Property<accfrac_t> m_accFrac{
      this, "AcceptFractions", DEFAULT_ACCEPT_FRACTIONS,
      "Map {event_type: accept_fractions} with the latter is a list of the form [ee, be, eb, bb]."};
  Gaudi::Property<int>  m_overrideBXType{this, "OverrideBXType", -1,
                                        "If between 0 and 3, override the bunch crossing type for all events."};
  Gaudi::Property<bool> m_filter{this, "Filter", true, "Filter only selected events."};
  Gaudi::Property<bool> m_setEventNumber{this, "SetEventNumber", false,
                                         "Set event number using a new monitonically increasing sequence."};
};

StatusCode ODINEmulator::initialize() {
  const StatusCode sc = Transformer::initialize();
  if ( !sc ) return sc;

  for ( const auto& p : DEFAULT_ACCEPT_FRACTIONS ) {
    auto mask        = static_cast<uint32_t>( parse( p.first ) );
    m_counters[mask] = {};
    for ( unsigned int i = 0; i < 4; ++i ) {
      m_initial[mask][i] = mix32( mix32( mixString( name().size(), name() ), mask ), i );
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) debug() << " generated initial value " << m_initial << endmsg;

  return sc;
}

ODIN ODINEmulator::operator()( const ODIN& odin ) const {
  ODIN emulated{odin};
  if ( m_setEventNumber ) { emulated.setEventNumber( ( m_eventNumber++ ).value() ); }
  if ( UNLIKELY( m_overrideBXType >= 0 && m_overrideBXType < 4 ) ) {
    emulated.setBunchCrossingType( static_cast<ODIN::BXTypes>( m_overrideBXType.value() ) );
  }
  auto bx = emulated.bunchCrossingType();

  unsigned int eventType = std::accumulate(
      std::begin( m_initial ), std::end( m_initial ), 0u,
      [&]( unsigned int left, std::pair<unsigned int, std::array<uint32_t, 4>> p ) -> unsigned int {
        bool     acc = true;
        uint32_t x   = p.second[bx];
        x            = mix64( x, odin.gpsTime() );
        x            = mix32( x, odin.runNumber() );
        x            = mix64( x, odin.eventNumber() );
        auto accThld = m_acc.at( p.first )[bx];
        acc = acc && ( accThld == boost::integer_traits<uint32_t>::const_max || ( accThld > 0 && x < accThld ) );
        m_counters.at( p.first )[bx] += acc;
        return acc ? ( left | p.first ) : left;
      } );
  emulated.setEventType( eventType );

  setFilterPassed( eventType > 0 || !m_filter );

  return emulated;
}

StatusCode ODINEmulator::finalize() {
  for ( const auto& p : m_counters ) {
    for ( unsigned int i = 0; i < 4; ++i ) {
      info() << "#accept(" << p.first << "," << i << "): " << p.second[i] << endmsg;
    }
  }
  return Transformer::finalize();
}

DECLARE_COMPONENT( ODINEmulator )
