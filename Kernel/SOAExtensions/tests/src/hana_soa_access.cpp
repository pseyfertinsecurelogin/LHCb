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
#undef NDEBUG

#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE utestSelection
#include <boost/test/unit_test.hpp>

#include "LHCbMath/SIMDWrapper.h"
#include <boost/hana.hpp>
#include <functional>
#include <vector>

namespace hana = boost::hana;

using namespace SIMDWrapper::avx2;

template <typename T>
struct type_; // for type checking

template <typename T>
auto _get_members( T&& t ) {
  return hana::transform( hana::accessors<std::decay_t<T>>(),
                          [&t]( auto accessor ) -> decltype( auto ) { return &hana::second( accessor )( t ); } );
}

template <typename T>
struct _first_member_type {
  using type =
      std::remove_pointer_t<std::remove_reference_t<decltype( _get_members( std::declval<T>() )[hana::size_c<0>] )>>;
};

template <typename T>
using _first_member_type_t = typename _first_member_type<T>::type;

template <typename T>
struct _is_simd_wrapper { // to be extended by all simd types
  constexpr static bool value = std::is_same_v<int_v, T> or std::is_same_v<float_v, T>;
};

template <typename Out, typename T>
Out get( T&& t, int i ) {
  auto construct = []( auto&&... ts ) -> decltype( auto ) { return Out{ts...}; };

  using _out_mem_t = _first_member_type_t<Out>;
  auto aos         = hana::transform( _get_members( t ), [i]( auto* item ) -> decltype( auto ) {
    if constexpr ( std::is_pointer_v<_out_mem_t> ) {
      return &( *item )[i];
    } else if constexpr ( _is_simd_wrapper<_out_mem_t>::value ) {
      return &( *item )[i];
    } else {
      return ( *item )[i];
    }
  } );
  return hana::unpack( aos, construct );
}

struct muonpids {
  BOOST_HANA_DEFINE_STRUCT( muonpids, ( std::vector<int>, statuss ), ( std::vector<float>, chi2corrs ) );
};

struct muonpid_ptr { // with pointers to the original structure
  BOOST_HANA_DEFINE_STRUCT( muonpid_ptr, ( int*, status ), ( float*, chi2corr ) );
};

struct muonpid { // copies of original structure
  BOOST_HANA_DEFINE_STRUCT( muonpid, ( int, status ), ( float, chi2corr ) );
};

struct muonpid_soa { // now i want fancy soa types
  BOOST_HANA_DEFINE_STRUCT( muonpid_soa, ( int_v, status ), ( float_v, chi2corr ) );
};

BOOST_AUTO_TEST_CASE( all ) {
  muonpids m{{1, 2, 3, 4, 5, 6, 7, 0}, {3, 4, 5, 6, 7, 0, 1, 2}};

  // check ptr types
  auto a      = get<muonpid_ptr>( m, 1 );
  *a.status   = 42;
  *a.chi2corr = 100;
  BOOST_CHECK( m.statuss[1] == 42 and m.chi2corrs[1] == 100 );

  // check value types
  auto b = get<muonpid>( m, 2 );
  BOOST_CHECK( b.status == 3 and b.chi2corr == 5 );

// check vector types
#ifdef __AVX2__
  auto c = get<muonpid_soa>( m, 0 );
  BOOST_STATIC_ASSERT( std::is_same_v<decltype( c.status ), int_v> );
  BOOST_STATIC_ASSERT( std::is_same_v<decltype( c.chi2corr ), float_v> );
  int sum_chi2corr = c.chi2corr.hadd();

  BOOST_CHECK_MESSAGE( sum_chi2corr == 124, sizeof( c.status ) );
#endif
}
