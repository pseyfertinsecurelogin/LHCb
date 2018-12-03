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
#include <string>
#include <cmath>
#include <cstdint>
#include <limits>

namespace {

uint32_t mix(uint32_t state)
{
    // note: the constants below are _not_ arbitrary, but are picked
    //       carefully such that the bit shuffling has a large 'avalanche' effect...
    //       See http://bretm.home.comcast.net/~bretm/hash/
    //
    // note: as a result, you might call this a quasi-random (not to be confused
    //       with psuedo-random!) number generator, in that it generates an output
    //       which satisfies a requirement on the uniformity of its output distribution.
    //       (and not on the predictability of the next number in the sequence,
    //       based on knowledge of the preceding numbers)
    //
    // note: another way to look at this is is as an (approximation of an) evaporating
    //       black hole: whatever you dump in to it, you get something uniformly
    //       distributed back ;-)
    //
    state += (state << 16); state ^= (state >> 13);
    state += (state <<  4); state ^= (state >>  7);
    state += (state << 10); state ^= (state >>  5);
    state += (state <<  8); state ^= (state >> 16);
    return state;
}

// mix some 'extra' entropy into 'state' and return result
uint32_t mix32(uint32_t state, uint32_t extra)
{
    return mix( state + extra );
}

// mix some 'extra' entropy into 'state' and return result
uint32_t mix64(uint32_t state, uint64_t extra)
{
    constexpr auto mask = ( ( ~uint64_t{0} ) >> 32 );
    state = mix32( state , uint32_t( extra        & mask ));
    return  mix32( state , uint32_t((extra >> 32) & mask ));
}

// mix some 'extra' entropy into 'state' and return result
uint32_t mixString(uint32_t state, std::string_view extra)
{
  // prefix name with ' ' until the size is a multiple of 4.
  std::string s = std::string((4-extra.size()%4)%4,' ').append( extra );
  for (size_t i=0; i < s.size()/4; ++i) {
     //FIXME: this _might_ do something different on big endian vs. small endian machines...
     uint32_t x = uint32_t(s[i*4  ])
                | uint32_t(s[i*4+1])<< 8
                | uint32_t(s[i*4+2])<<16
                | uint32_t(s[i*4+3])<<24 ;
     state = mix32( state , x );
  }
  return state;
}

}

///////////////////////////////////

class DeterministicPrescaler final : public Gaudi::Functional::FilterPredicate<bool(const LHCb::ODIN&)>
{
public:
  DeterministicPrescaler( const std::string& name, ISvcLocator* pSvcLocator )
  : FilterPredicate(name, pSvcLocator,
                    KeyValue{ "ODINLocation", LHCb::ODINLocation::Default } ),
    m_initial{ mixString( name.size(), name ) }
  { }

  StatusCode initialize() override {
    const StatusCode sc = FilterPredicate::initialize();
    if (!sc) return sc;
    if (msgLevel(MSG::DEBUG)) debug() << " generated initial value " << m_initial << endmsg;
    if (m_acc!=std::numeric_limits<uint32_t>::max())  {
        info() << "Prescaling events; keeping " << m_accFrac.value() << " of events " << endmsg;
    }
    return sc;
  }

  bool operator()(const LHCb::ODIN& odin) const override {
    auto accept = [&](const LHCb::ODIN& odin) {
        auto x = mix64( mix32( mix64( m_initial,
                                      odin.gpsTime() ),
                                      odin.runNumber() ),
                                      odin.eventNumber() );
        if (msgLevel(MSG::DEBUG)) debug() << "  gpsTime: " << odin.gpsTime()
                                          << "  run#: "    << odin.runNumber()
                                          << "  evt#: "    << odin.eventNumber()
                                          << "  -->  "     << x
                                          << endmsg;
        // at this point, we assume 'x' to be uniformly distributed in [0,0xffffffff]
        // (and yes, this was verified to be sufficiently true on a sample of 10K MC events ;-)
        return x < m_acc;
    };
    const bool acc = ( ( m_acc == std::numeric_limits<uint32_t>::max() )
                       || ( m_acc !=0 && accept( odin ) ) );
    m_accept += acc;
    if (msgLevel(MSG::DEBUG)) debug() << (acc?"Accepted":"Rejected") << endmsg ;
    return acc;
  }

private:
  /// initial seed unique to this instance (computed from the name)
  std::uint32_t         m_initial{0};
  /// integer representation of the above
  std::uint32_t         m_acc{std::numeric_limits<uint32_t>::max()};

  mutable Gaudi::Accumulators::BinomialCounter<> m_accept{ this, "#accept" };

  /// fraction of input events to accept...
  Gaudi::Property<double>                  m_accFrac
  { this,  "AcceptFraction" , 1,  [this](const Property&) {
    m_acc = ( m_accFrac<=0 ? 0
            : m_accFrac>=1 ? std::numeric_limits<uint32_t>::max()
            : uint32_t( m_accFrac*std::numeric_limits<uint32_t>::max()));
    if( msgLevel(MSG::DEBUG) )
      debug() << "frac: " << m_accFrac << " acc: 0x" << std::hex << m_acc << endmsg;
  }};
};


// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DeterministicPrescaler )
