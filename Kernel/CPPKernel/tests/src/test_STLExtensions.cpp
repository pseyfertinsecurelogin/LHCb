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
#define BOOST_TEST_MODULE test_PlatformInfo
#include <boost/test/unit_test.hpp>

#include "Kernel/STLExtensions.h"
#include <numeric>
#include <type_traits>

template <typename ElementType, std::ptrdiff_t N>
auto sum( LHCb::span<ElementType, N> s ) {
  using std::begin;
  using std::end;
  return std::accumulate( begin( s ), end( s ), ElementType{0}, std::plus{} );
}

BOOST_AUTO_TEST_CASE( span ) {
  {
    std::array<int, 3> ai{9, 10, 11};
    BOOST_CHECK( sum( LHCb::make_span( ai ) ) == 30 );
    std::vector<int> vi{9, 10, 11};
    BOOST_CHECK( sum( LHCb::make_span( vi ) ) == 30 );

    const std::vector<int> cvi{9, 10, 11};
    BOOST_CHECK( sum( LHCb::make_span( cvi ) ) == 30 );

    // check that we can make a span from iterators over vector elements ...
    BOOST_CHECK( sum( LHCb::make_span( vi.begin(), vi.begin() + 2 ) ) == 19 );

    BOOST_CHECK( sum( LHCb::make_span( cvi.begin(), cvi.begin() + 2 ) ) == 19 );

    // check that we can make a span from iterators over span elements ...
    auto si = LHCb::make_span( vi );
    BOOST_CHECK( sum( LHCb::make_span( si.begin(), si.begin() + 2 ) ) == 19 );

    auto csi = LHCb::make_span( cvi );
    BOOST_CHECK( sum( LHCb::make_span( csi.begin(), csi.begin() + 2 ) ) == 19 );
  }
}

BOOST_AUTO_TEST_CASE( range_single ) {
  {
    auto single = LHCb::range::single{4.0};
    auto span   = LHCb::make_span( single );
    static_assert( span.extent == gsl::dynamic_extent ); // FIXME -- annoying that this isn't 1
    BOOST_CHECK( sum( span ) == single.value() );
    BOOST_CHECK( single.size() == 1 );
  }

  {
    // check conversion single<T> -> span<T>
    auto               single = LHCb::range::single{4.0};
    LHCb::span<double> d      = single;
    static_assert( d.extent == gsl::dynamic_extent ); // FIXME annoying, this has dynamic extent...
    LHCb::span<double, 1> e = single;
    static_assert( e.extent == 1 );
    auto f = static_cast<LHCb::span<double, 1>>( single );
    BOOST_CHECK( f[0] == single.value() );
    auto g = static_cast<LHCb::span<double>>( single );
    BOOST_CHECK( g[0] == single.value() );
  }

  // check two ways of moving out of single...

  {
    auto single_uptr = LHCb::range::single{std::make_unique<int>( 99 )};
    BOOST_CHECK( static_cast<bool>( single_uptr.value() ) );

    auto uptr = std::move( single_uptr.value() );

    BOOST_CHECK( !static_cast<bool>( single_uptr.value() ) );
    BOOST_CHECK( *uptr == 99 );
  }

  {
    auto single_uptr = LHCb::range::single{std::make_unique<int>( 99 )};
    BOOST_CHECK( static_cast<bool>( single_uptr.value() ) );

    auto uptr = std::move( single_uptr ).value();

    BOOST_CHECK( !static_cast<bool>( single_uptr.value() ) );
    BOOST_CHECK( *uptr == 99 );
  }

  // check in place construction
  {
    struct S {
      constexpr S( double, int, const char* ) {}
    };
    constexpr auto s1 = LHCb::range::single<S>{std::in_place, 2.5, 3, "HelloGoodbye"};
    static_assert( s1.size() == 1 );
    static_assert( !s1.empty() );
  }
}
