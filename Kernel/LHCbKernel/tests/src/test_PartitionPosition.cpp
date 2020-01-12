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

#include "Kernel/PartitionPosition.h"
#include "Kernel/STLExtensions.h"
#include <array>
#include <bitset>

BOOST_AUTO_TEST_CASE( masked_stable_partition_array ) {

  int  a[] = {1, 2, 3, 4, 5, 5, 4, 3, 2, 1};
  bool b[] = {0, 1, 0, 1, 0, 0, 1, 0, 1, 0};

  auto s = LHCb::span( a );

  auto n = LHCb::cxx::stablePartitionByMask( s, b );
  BOOST_CHECK( n == 4 );
  auto expected_first  = {2, 4, 4, 2};
  auto expected_second = {1, 3, 5, 5, 3, 1};
  auto expected_third  = {2, 4, 4, 2, 1, 3, 5, 5, 3, 1};
  BOOST_TEST( s.first( n ) == expected_first, boost::test_tools::per_element() );
  BOOST_TEST( s.subspan( n ) == expected_second, boost::test_tools::per_element() );
  BOOST_TEST( a == expected_third, boost::test_tools::per_element() );
}

BOOST_AUTO_TEST_CASE( masked_stable_partition_bitset ) {
  auto            a = std::array{1, 2, 3, 4, 5, 5, 4, 3, 2, 1};
  std::bitset<10> b{0b0101001010};

  auto s = LHCb::make_span( a );

  auto n = LHCb::cxx::stablePartitionByMask( s, b );
  BOOST_CHECK( n == 4 );
  auto expected_first  = {2, 4, 4, 2};
  auto expected_second = {1, 3, 5, 5, 3, 1};
  auto expected_third  = {2, 4, 4, 2, 1, 3, 5, 5, 3, 1};
  BOOST_TEST( s.first( n ) == expected_first, boost::test_tools::per_element() );
  BOOST_TEST( s.subspan( n ) == expected_second, boost::test_tools::per_element() );
  BOOST_TEST( a == expected_third, boost::test_tools::per_element() );
}

BOOST_AUTO_TEST_CASE( masked_partition_bitset ) {
  auto            a = std::array{1, 2, 3, 4, 5, 6, 7, 8, 9};
  std::bitset<10> b{0b010101010};

  auto s = LHCb::make_span( a );

  auto n = LHCb::cxx::partitionByMask( s, b );
  BOOST_CHECK( n == 4 );
  auto expected_first  = {8, 2, 6, 4};
  auto expected_second = {5, 3, 7, 1, 9};
  auto expected_third  = {8, 2, 6, 4, 5, 3, 7, 1, 9};
  BOOST_TEST( s.first( n ) == expected_first, boost::test_tools::per_element() );
  BOOST_TEST( s.subspan( n ) == expected_second, boost::test_tools::per_element() );
  BOOST_TEST( a == expected_third, boost::test_tools::per_element() );
}
