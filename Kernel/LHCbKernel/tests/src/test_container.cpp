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
#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>

#include "Kernel/ArenaAllocator.h"
#include "Kernel/Container.h"

// Replace new and delete just for the purpose of demonstrating that
// they are called once and once only for each container.

std::size_t memory  = 0;
std::size_t alloc   = 0;
std::size_t dealloc = 0;

void* operator new( std::size_t s ) {
  memory += s;
  ++alloc;
  return malloc( s );
}
void operator delete( void* p ) {
  ++dealloc;
  free( p );
}
void operator delete( void* p, size_t ) {
  ++dealloc;
  free( p );
}

BOOST_AUTO_TEST_CASE( test_MultiVector ) {
  memory = 0;
  alloc  = 0;

  std::size_t                                  N = 64;
  LHCb::Kernel::MultiVector<int, double, char> c{64};

  BOOST_CHECK_EQUAL( alloc, 1 );
  BOOST_CHECK_EQUAL( c.size(), 0 );
  BOOST_CHECK_EQUAL( c.capacity(), N );
  BOOST_CHECK( c.empty() );

  auto idc = c.emplace_back( 1, 2.5, 'a' );
  BOOST_CHECK_EQUAL( std::get<0>( idc ), 1 );
  BOOST_CHECK_EQUAL( std::get<1>( idc ), 2.5 );
  BOOST_CHECK_EQUAL( std::get<2>( idc ), 'a' );

  BOOST_CHECK_EQUAL( std::get<int&>( idc ), 1 );
  BOOST_CHECK_EQUAL( std::get<double&>( idc ), 2.5 );
  BOOST_CHECK_EQUAL( std::get<char&>( idc ), 'a' );

  auto&& [ii, dd, cc] = c.emplace_back( 1, 2.5, 'a' );
  BOOST_CHECK_EQUAL( ii, 1 );
  BOOST_CHECK_EQUAL( dd, 2.5 );
  BOOST_CHECK_EQUAL( cc, 'a' );
  cc = 'X';
  BOOST_CHECK_EQUAL( c.column<char>().back(), 'X' );

  auto&& [iii, ddd] = c.get<int, double>( 1 );
  BOOST_CHECK_EQUAL( iii, 1 );
  BOOST_CHECK_EQUAL( ddd, 2.5 );

  BOOST_CHECK_EQUAL( alloc, 1 );

  BOOST_CHECK( c[0] == std::make_tuple( 1, 2.5, 'a' ) );
  BOOST_CHECK( c[0] == c.front() );
  BOOST_CHECK_EQUAL( c.column<0>()[0], 1 );
  BOOST_CHECK_EQUAL( c.column<char>().at( 0 ), 'a' );
  BOOST_CHECK_EQUAL( c.column<double>()[0], 2.5 );

  BOOST_CHECK_EQUAL( c.size(), 2 );
  BOOST_CHECK( c[1] == c.back() );

  for ( int i = c.size(); i < static_cast<int>( N ); ++i ) {
    c.emplace_back( i * 3, i * 1.5, 'A' + i );
    BOOST_CHECK_EQUAL( alloc, 1 );
  }
  BOOST_CHECK_EQUAL( c.size(), N );
  BOOST_CHECK_EQUAL( alloc, 1 );

  auto c2 = std::move( c );
  BOOST_CHECK( c.empty() );
  BOOST_CHECK_EQUAL( c2.size(), N );
  BOOST_CHECK( c2[0] == std::make_tuple( 1, 2.5, 'a' ) );

  BOOST_CHECK_EQUAL( alloc, 1 );
}

BOOST_AUTO_TEST_CASE( test_MultiContainer ) {
  memory = 0;
  alloc  = 0;
  LHCb::Kernel::MultiContainer<LHCb::span<int>, int> c{4, 100}; // allocates, using a _single_ allocation,  memory for 4
                                                                // spans, and 100 ints...
  BOOST_CHECK_EQUAL( alloc, 1 );
  auto& ints  = c.column<int>();             // get the column of ints
  auto& spans = c.column<LHCb::span<int>>(); // get the column of spans

  // intersperse writing to ints and span,
  // and demonstrate that the spans remain valid
  for ( int i = 0; i < 5; ++i ) ints.push_back( i );
  spans.emplace_back( ints );

  for ( int i = 5; i < 25; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 5 ) );

  for ( int i = 25; i < 80; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 25 ) );

  for ( int i = 80; i < 100; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 80 ) );

  BOOST_CHECK_EQUAL( alloc, 1 );

  auto c2 = std::move( c );      // move the container
  BOOST_CHECK_EQUAL( alloc, 1 ); // check that this doesn't allocate memory
  BOOST_CHECK( c.empty() );

  // and verify that the spans inside the 'moved' container are still valid
  std::vector<int> sums;
  for ( auto s : c2.column<LHCb::span<int>>() ) { sums.push_back( std::accumulate( s.begin(), s.end(), 0 ) ); }
  auto euler = []( int n ) { return n * ( n + 1 ) / 2; };
  BOOST_CHECK_EQUAL( sums.at( 0 ), euler( 4 ) );
  BOOST_CHECK_EQUAL( sums.at( 1 ), euler( 24 ) - euler( 4 ) );
  BOOST_CHECK_EQUAL( sums.at( 2 ), euler( 79 ) - euler( 24 ) );
  BOOST_CHECK_EQUAL( sums.at( 3 ), euler( 99 ) - euler( 79 ) );
}

struct Pieces {
  Pieces( int i, float f, double d, char c ) // C++20: remove the c-tor, and let Pieces be an aggregate...
      : i{i}, f{f}, d{d}, c{c} {} // Pre-C++20 does not allow aggegrates to be constructed using emplace_back...
  int    i;
  float  f;
  double d;
  char   c;

  friend bool operator==( const Pieces& lhs, const Pieces& rhs ) // C++20: just opt-in to operator<=>
  {
    return lhs.i == rhs.i && lhs.f == rhs.f && lhs.d == rhs.d && lhs.c == rhs.c;
  }
};

BOOST_AUTO_TEST_CASE( test_MultiVector_piecewise_construct ) {
  memory = 0;
  alloc  = 0;
  LHCb::Kernel::MultiVector<int, Pieces> c{24};

  c.emplace_back( 3, Pieces{1, 1.5F, 2.5, 'a'} );
  auto&& [ii, pp] = c.emplace_back( 3, std::forward_as_tuple( 2, 2.5F, 3.5, 'b' ) );
  BOOST_CHECK_EQUAL( ii, 3 );
  ii = 9;
  BOOST_CHECK_EQUAL( c.column<int>().back(), 9 );
  BOOST_CHECK_EQUAL( &c.column<int>().back(), &ii );
  BOOST_CHECK_EQUAL( &c.column<0>().back(), &ii );

  auto p1 = Pieces{2, 2.5F, 3.5, 'b'};
  BOOST_CHECK( pp == p1 );
  BOOST_CHECK( c.column<Pieces>().at( 1 ) == p1 );
  BOOST_CHECK( &pp.d == &c.column<Pieces>().at( 1 ).d );

  BOOST_CHECK_EQUAL( alloc, 1 );
  BOOST_CHECK_EQUAL( c.size(), 2 );

  // c.emplace_back( 3 );

  for ( auto i = c.size(); i < c.capacity(); ++i ) {
    c.emplace_back( i, std::forward_as_tuple( 2 * i, 2.5F * i, 3.5 * i, 'a' + i ) );
  }

  BOOST_CHECK_EQUAL( alloc, 1 );
  BOOST_CHECK_EQUAL( c.size(), 24 );
  BOOST_CHECK_EQUAL( c.capacity(), 24 );
}

BOOST_AUTO_TEST_CASE( test_MultiVector_clear_pop_back ) {
  memory = 0;
  alloc  = 0;

  LHCb::Kernel::MultiVector<int, double, char> c{64};

  c.emplace_back( 1, 2.5, 'c' );
  c.emplace_back( 2, 3.5, 'd' );
  c.emplace_back( 3, 4.5, 'e' );

  BOOST_CHECK_EQUAL( alloc, 1 );
  BOOST_CHECK( !c.empty() );
  BOOST_CHECK_EQUAL( c.size(), 3 );

  c.pop_back();
  BOOST_CHECK_EQUAL( c.size(), 2 );
  c.clear();
  BOOST_CHECK_EQUAL( c.size(), 0 );
  BOOST_CHECK( c.empty() );

  // auto c2 = c;
  // c = {};
}

BOOST_AUTO_TEST_CASE( test_MultiVector_algorithms ) {
  LHCb::Kernel::MultiVector<int, char> c{64};
  c.emplace_back( 1, 'H' );
  c.emplace_back( 2, 'e' );
  c.emplace_back( 3, 'l' );
  c.emplace_back( 4, 'l' );
  c.emplace_back( 5, 'o' );

  std::string s;
  transform( c, std::back_inserter( s ), []( char c ) { return c; } );
  BOOST_CHECK_EQUAL( s, "Hello" );

  std::string s2 = transform_reduce(
      c, std::string{},
      []( std::string s, char c ) {
        s.push_back( c );
        return std::move( s );
      },
      []( char c ) { return c; } );
  BOOST_CHECK_EQUAL( s2, "Hello" );

  int sum = accumulate( c, 0, []( int i, int j ) { return i + j; } );
  BOOST_CHECK_EQUAL( sum, 15 );

  int sum2 = transform_reduce( c, 0, std::plus<>{}, []( int i ) { return i; } );
  BOOST_CHECK_EQUAL( sum2, 15 );
}
