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
#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestPrUTHits
#include <boost/test/unit_test.hpp>
#include <type_traits>

#include "Event/PrUTHits.h"

// The test fixture: Create a container of hits
// This can be used as a common starting point for all tests
using namespace LHCb::Pr::UT;

struct ExampleHits {

  Hits hits; // collection of hits

  ExampleHits() {
    BOOST_TEST_MESSAGE( "setup example hits" );

    using dType1 = SIMDWrapper::scalar::types;
    bool mask    = true;
    hits.compressstore_channelID<dType1::int_v>( 0, mask, 12345 );

    using dType2  = SIMDWrapper::sse::types;
    auto sse_mask = dType2::loop_mask( 0, dType2::size );
    hits.compressstore_channelID<dType2::int_v>( 0, sse_mask, 12345 );

    using dType3   = SIMDWrapper::avx2::types;
    auto avx2_mask = dType3::loop_mask( 0, dType3::size );
    hits.compressstore_channelID<dType3::int_v>( 0, avx2_mask, 12345 );
  }

  ~ExampleHits() { BOOST_TEST_MESSAGE( "tear down example hits" ); }
};

BOOST_AUTO_TEST_CASE( test_hits_size ) {

  Hits myHits{};

  using dType1 = SIMDWrapper::scalar::types;
  using dType2 = SIMDWrapper::sse::types;
  using dType3 = SIMDWrapper::avx2::types;

  BOOST_CHECK( myHits.size() == dType1::size + dType2::size + dType3::size );
}

BOOST_AUTO_TEST_CASE( test_hits_constructors ) {
  static_assert( std::is_default_constructible_v<Hits> );
  static_assert( std::is_move_constructible_v<Hits> );
  static_assert( !std::is_copy_constructible_v<Hits> ); // this is disabled
}
