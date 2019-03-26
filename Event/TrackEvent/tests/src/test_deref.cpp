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
#define BOOST_TEST_MODULE test_add_ref
#include <boost/test/unit_test.hpp>

#include "Event/TrackFunctor.h"

struct odd_t {
  bool operator()( int i ) const { return i % 2 != 0; }
};

constexpr auto odd = odd_t{};

auto add( int i ) {
  return [=]( int j ) { return j + i; };
}

BOOST_AUTO_TEST_CASE( test_deref ) {
  {
    int a = 3;
    int b = 5;

    // pass a non-capturing, rvalue lambda
    auto f = TrackFunctor::details::add_deref( []( int i, int j ) { return i < j; } );

    BOOST_CHECK( f( a, b ) == true );
    BOOST_CHECK( f( b, a ) == false );
    BOOST_CHECK( f( &a, &b ) == true );
    BOOST_CHECK( f( &b, &a ) == false );

    // pass an lvalue lambda
    auto g = []( int i ) { return i + 42; };
    auto h = TrackFunctor::details::add_deref( g );

    BOOST_CHECK( g( a ) == 45 );
    BOOST_CHECK( h( a ) == 45 );
    BOOST_CHECK( h( &a ) == 45 );

    // pass a capturing rvalue lambda
    auto k = TrackFunctor::details::add_deref( add( 42 ) );
    BOOST_CHECK( k( a ) == 45 );
    BOOST_CHECK( k( &a ) == 45 );

    // pass an instance of a struct with operator()
    auto l = TrackFunctor::details::add_deref( odd );

    int c = 18;
    BOOST_CHECK( odd( c ) == false );
    BOOST_CHECK( l( c ) == false );
    BOOST_CHECK( l( &c ) == false );
    int d = 19;
    BOOST_CHECK( odd( d ) == true );
    BOOST_CHECK( l( d ) == true );
    BOOST_CHECK( l( &d ) == true );
  }
}
