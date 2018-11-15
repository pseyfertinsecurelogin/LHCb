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
#define BOOST_TEST_MODULE utestMetaEnum
#include <boost/test/unit_test.hpp>

#include "GaudiKernel/StatusCode.h"
#include "Kernel/meta_enum.h"
#include <sstream>

meta_enum_class(MyEnum, int, Unknown,
                Unknown=0,
                Case1,
                Case2,
                Case3,
                CaseLarge=10000)

BOOST_AUTO_TEST_CASE(test_meta_enum_printing) {
  BOOST_CHECK(static_cast<int>(MyEnum::Unknown) == 0);
  BOOST_CHECK(static_cast<int>(MyEnum::Case1) == 1);
  BOOST_CHECK(static_cast<int>(MyEnum::Case3) == 3);
  BOOST_CHECK(static_cast<int>(MyEnum::CaseLarge) == 10000);
  std::ostringstream output;
  output << MyEnum::Case1 << " " << MyEnum::CaseLarge;
  BOOST_CHECK(output.str()=="Case1 CaseLarge");
  MyEnum value;
  auto sc = parse(value, "Case2");
  BOOST_CHECK(value == MyEnum::Case2 && sc == StatusCode::SUCCESS);
  sc = parse(value, "non existing");
  BOOST_CHECK(value == MyEnum::Unknown && sc == StatusCode::FAILURE);
}
