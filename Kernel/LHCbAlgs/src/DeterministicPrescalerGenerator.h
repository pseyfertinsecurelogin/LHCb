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
/** @file LHCbMath/DeterministicPrescalerGenerator.h
 *
 *  Quasi-random generator for DeterministicPrescaler
 */

#include <string>

namespace {

  uint32_t mix( uint32_t state ) {
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
    state += ( state << 16 );
    state ^= ( state >> 13 );
    state += ( state << 4 );
    state ^= ( state >> 7 );
    state += ( state << 10 );
    state ^= ( state >> 5 );
    state += ( state << 8 );
    state ^= ( state >> 16 );
    return state;
  }

  // mix some 'extra' entropy into 'state' and return result
  uint32_t mix32( uint32_t state, uint32_t extra ) { return mix( state + extra ); }

  // mix some 'extra' entropy into 'state' and return result
  uint32_t mix64( uint32_t state, uint64_t extra ) {
    constexpr auto mask = ( ( ~uint64_t{0} ) >> 32 );
    state               = mix32( state, uint32_t( extra & mask ) );
    return mix32( state, uint32_t( ( extra >> 32 ) & mask ) );
  }

  // mix some 'extra' entropy into 'state' and return result
  uint32_t mixString( uint32_t state, std::string_view extra ) {
    // prefix name with ' ' until the size is a multiple of 4.
    std::string s = std::string( ( 4 - extra.size() % 4 ) % 4, ' ' ).append( extra );
    for ( size_t i = 0; i < s.size() / 4; ++i ) {
      // FIXME: this _might_ do something different on big endian vs. small endian machines...
      uint32_t x = uint32_t( s[i * 4] ) | uint32_t( s[i * 4 + 1] ) << 8 | uint32_t( s[i * 4 + 2] ) << 16 |
                   uint32_t( s[i * 4 + 3] ) << 24;
      state = mix32( state, x );
    }
    return state;
  }

} // namespace
