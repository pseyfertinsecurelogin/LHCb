/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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

struct memfun_t {
    int j = 0;
    int memfun(int i) const { return i+j; }
};

struct callable_t {
    int j = 0;
    int operator()(int i) const { return i+j; }
};


int fun(int i, int j) { return i+j; }

template <typename ElementType, std::ptrdiff_t N>
auto sum( LHCb::span<ElementType,N> s ) {
    using std::begin; using std::end;
    return std::accumulate( begin(s), end(s), ElementType{0}, std::plus<>{} );
}

BOOST_AUTO_TEST_CASE( invoke )
{
  {
    BOOST_CHECK( LHCb::invoke( fun, 1, 9 ) == 10 );

    memfun_t obj{9};
    BOOST_CHECK( LHCb::invoke( &memfun_t::memfun, obj, 1 ) == 10 );

    callable_t call{1};
    BOOST_CHECK( LHCb::invoke( call, 19 ) == 20 );

    BOOST_CHECK( LHCb::invoke( [](int i) { return i+3; }, 2 )== 5 );

  }
}

BOOST_AUTO_TEST_CASE( make_array )
{
  {
      auto i = LHCb::make_array( 0,1,2,3 ) ;
      // BOOST_CHECK( std::is_same< decltype(i), std::array<int,4> >::value );
      static_assert(std::is_same< decltype(i), std::array<int,4> >::value,"wrong type");
      BOOST_CHECK( i[0] == 0 );
      BOOST_CHECK( i[1] == 1 );
      BOOST_CHECK( i[2] == 2 );
      BOOST_CHECK( i[3] == 3 );
      //auto d = LHCb::make_array( 0,1,2.7,3 ) ;
      //static_assert(std::is_same< decltype(i), std::array<double,4> >::value,"wrong type");
  }
}

BOOST_AUTO_TEST_CASE( span )
{
  {
    std::array<int, 3> ai{9,10,11};
    BOOST_CHECK( sum( LHCb::make_span(ai) ) == 30 );
    std::vector<int> vi{ 9,10,11};
    BOOST_CHECK( sum( LHCb::make_span(vi) ) == 30 );
  }
}
