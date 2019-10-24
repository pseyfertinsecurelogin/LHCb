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

#include "Kernel/PlatformInfo.h"

#include "GaudiKernel/System.h"

#include <boost/test/unit_test.hpp>

// special trick to add quotes around a macro value
// see https://stackoverflow.com/a/20632065
#define STR_( X ) #X
#define STR( X ) STR_( X )

struct Shadow {
  std::uint16_t id;
  std::uint16_t instr;
};

using namespace LHCb;

BOOST_AUTO_TEST_CASE( constructor ) {
  {
    PlatformInfo info;

    BOOST_CHECK( info.binaryTag() == STR( BINARY_TAG ) );
    BOOST_CHECK( info.hostInstrSetLevel() == System::instructionsetLevel() );
  }
}

BOOST_AUTO_TEST_CASE( string_conversion ) {
  {
    PlatformInfo info;
    auto*        s = reinterpret_cast<Shadow*>( &info );

    s->instr = 0;
    BOOST_CHECK( info.hostInstrSet() == "80386" );

    {
      std::stringstream o;
      o << info;
      BOOST_CHECK( o.str() == ( STR( BINARY_TAG ) ":80386" ) );
    }

    s->id    = 0;
    s->instr = 8;
    {
      std::stringstream o;
      o << info;
      BOOST_CHECK( o.str() == "unknown:AVX2" );
    }

    s->id    = 1234;
    s->instr = 1234;
    {
      std::stringstream o;
      o << info;
      BOOST_CHECK( o.str() == "invalid:unknown" );
    }
  }
}

BOOST_AUTO_TEST_CASE( copy ) {
  {
    PlatformInfo info;
    auto*        s = reinterpret_cast<Shadow*>( &info );

    BOOST_CHECK( info.binaryTag() == STR( BINARY_TAG ) );
    BOOST_CHECK( info.hostInstrSetLevel() == System::instructionsetLevel() );

    s->id = ( s->id != 10 ) ? 10 : 11;
    s->instr++;

    PlatformInfo copy = info;

    BOOST_CHECK( copy.binaryId() == info.binaryId() );
    BOOST_CHECK( copy.hostInstrSetLevel() == info.hostInstrSetLevel() );
    BOOST_CHECK( copy.binaryTag() != STR( BINARY_TAG ) );
    BOOST_CHECK( copy.hostInstrSetLevel() != System::instructionsetLevel() );
  }
}
