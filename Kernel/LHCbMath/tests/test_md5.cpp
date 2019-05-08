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

#include "LHCbMath/MD5.h"

BOOST_AUTO_TEST_CASE( test_md5 ) {

  auto md5 = Gaudi::Math::MD5{};
  BOOST_CHECK( md5.invalid() );
  BOOST_CHECK( !md5.valid() );
  BOOST_CHECK( !md5 );

  auto invalid = Gaudi::Math::MD5::createInvalid();
  BOOST_CHECK( invalid.invalid() );
  BOOST_CHECK( !invalid.valid() );
  BOOST_CHECK( !invalid );

  auto hello_world = Gaudi::Math::MD5::compute( "Hello Word!" );
  BOOST_CHECK( !hello_world.invalid() );
  BOOST_CHECK( hello_world.valid() );
  BOOST_CHECK( static_cast<bool>( hello_world ) );
  BOOST_CHECK( hello_world.str() == "7f25944055201790c6cc0c88bbb362fc" );

  auto fromString = Gaudi::Math::MD5::createFromStringRep( "7f25944055201790c6cc0c88bbb362fc" );
  BOOST_CHECK( fromString.str() == "7f25944055201790c6cc0c88bbb362fc" );
  BOOST_CHECK( fromString == hello_world );
  BOOST_CHECK( !( fromString != hello_world ) );
  BOOST_CHECK( !( fromString < hello_world ) );
  BOOST_CHECK( !( fromString > hello_world ) );
  BOOST_CHECK( fromString >= hello_world );
  BOOST_CHECK( fromString <= hello_world );

  auto big_brother = Gaudi::Math::MD5::compute(
      "It was a bright cold day in April, and the clocks were striking thirteen. "
      "Winston Smith, his chin nuzzled into his breast in an effort to escape the vile wind, slipped quickly "
      "through the glass doors of Victory Mansions, though not quickly enough to prevent a swirl of gritty dust "
      "from entering along with him. The hallway smelt of boiled cabbage and old rag mats. "
      "At one end of it a coloured poster, too large for indoor display, had been tacked to the wall. "
      "It depicted simply an enormous face, more than a metre wide: the face of a man of about forty-five, "
      "with a heavy black moustache and ruggedly handsome features. Winston made for the stairs. "
      "It was no use trying the lift. Even at the best of times it was seldom working, and at present "
      "the electric current was cut off during daylight hours. It was part of the economy drive in "
      "preparation for Hate Week. The flat was seven flights up, and Winston, who was thirty-nine and "
      "had a varicose ulcer above his right ankle, went slowly, resting several times on the way. "
      "On each landing, opposite the lift-shaft, the poster with the enormous face gazed from the wall. "
      "It was one of those pictures which are so contrived that the eyes follow you about when you move. "
      "BIG BROTHER IS WATCHING YOU, the caption beneath it ran." );
  BOOST_CHECK( big_brother.str() == "493f4ab946d981aa0e02c5cffee2f6a2" );

  BOOST_CHECK( big_brother != hello_world );
  BOOST_CHECK( big_brother < hello_world );
  BOOST_CHECK( big_brother <= hello_world );
  BOOST_CHECK( !( big_brother > hello_world ) );
  BOOST_CHECK( !( big_brother >= hello_world ) );
}