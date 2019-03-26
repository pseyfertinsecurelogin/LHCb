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
#include "Kernel/ReadRoutingBits.h"

//-----------------------------------------------------------------------------
// Implementation file for class : ReadRoutingBits
//
// 2009-11-06 : Patrick Koppenburg
//-----------------------------------------------------------------------------
namespace Hlt {
  //=========================================================================
  //  return bits if they are in a range
  //=========================================================================
  std::vector<unsigned int> firedRoutingBits( const LHCb::RawEvent*            rawEvent,
                                              const std::vector<unsigned int>& relevantBits ) {
    std::vector<unsigned int> yes;
    yes.reserve( relevantBits.size() );
    std::copy_if( relevantBits.begin(), relevantBits.end(), std::back_inserter( yes ),
                  [bits = readRoutingBits( rawEvent )]( unsigned int j ) { return bits[j]; } );
    return yes;
  }
  //=========================================================================
  //  return bits if they are in a list
  //=========================================================================
  std::vector<unsigned int> firedRoutingBits( const LHCb::RawEvent* rawEvent, const unsigned int relevantMin,
                                              const unsigned int relevantMax ) {
    assert( relevantMax >= relevantMin );
    std::vector<unsigned int> yes;
    yes.reserve( relevantMax - relevantMin );
    auto bits = readRoutingBits( rawEvent );
    for ( unsigned int j = relevantMin; j <= relevantMax; ++j ) {
      if ( bits[j] ) yes.push_back( j ); // accepted
    }
    return yes;
  }
  //=========================================================================
  //  read routing bits and return as bitset
  //=========================================================================
  boost::dynamic_bitset<unsigned int> readRoutingBits( const LHCb::RawEvent* rawEvent ) {
    if ( !rawEvent ) { throw GaudiException( "Cannot find RawEvent", "Hlt::readRoutingBits", StatusCode::FAILURE ); }
    const unsigned int size = 3;

    const auto& banks = rawEvent->banks( LHCb::RawBank::HltRoutingBits );
    if ( banks.empty() ) {
      throw GaudiException( "There is no routing bits banks. Please make sure you run the trigger",
                            "Hlt::readRoutingBits", StatusCode::FAILURE );
    }
    if ( banks.size() > 1 ) {
      throw GaudiException( "There are >1 routing bits banks. Please remove the original before re-running the trigger",
                            "Hlt::readRoutingBits", StatusCode::FAILURE );
    }
    if ( LHCb::RawBank::MagicPattern != banks[0]->magic() ) {
      throw GaudiException( "Wrong magic in HltRoutingBits", "Hlt::readRoutingBits", StatusCode::FAILURE );
    }
    //    std::cout << banks[0]->size() << std::endl ;
    if ( banks[0]->size() != size * sizeof( unsigned int ) ) {
      throw GaudiException( "RawBank wrong size", "Hlt::readRoutingBits", StatusCode::FAILURE );
    }
    return boost::dynamic_bitset<unsigned int>( banks[0]->begin<unsigned int>(), banks[0]->end<unsigned int>() );
  }
} // namespace Hlt
