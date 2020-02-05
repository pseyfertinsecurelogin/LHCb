/*****************************************************************************\
* (c) Copyright 2020 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#define BOOST_TEST_MODULE MatVecTest
#include "LHCbMath/MatVec.h"
#include <boost/test/included/unit_test.hpp>
#include <limits>

using namespace LHCb::LinAlg;

BOOST_AUTO_TEST_CASE( test_Vec ) {
  Vec unit_z{0, 0, 1}, unit_x{1, 0, 0}, unit_y{0, 1, 0};

  auto sum = Vec{1, 1, 1};
  BOOST_CHECK_EQUAL( unit_x + unit_y + unit_z, sum ); // operator+

  auto x_minus_y = Vec{1, -1, 0};
  BOOST_CHECK_EQUAL( unit_x - unit_y, x_minus_y ); // operator-

  BOOST_CHECK_EQUAL( unit_x.dot( unit_y ), 0 ); // dot

  BOOST_CHECK_EQUAL( unit_x * 2 + unit_y * 2 + unit_z * 2, sum * 2 ); // operator* with number

  auto zero = initialize_with_zeros<Vec<int, 3>>();
  BOOST_CHECK_EQUAL( unit_y * unit_z, zero ); // operator* elementwise

  BOOST_CHECK_EQUAL( Vec{1}.cast_to_value(), 1 ); // cast_to_value (only works when 1 dim

  Vec unit_x_2d{1, 0};
  // sub gets you a subvector of the specified type
  auto should_be_unit_x_2d = unit_y.sub<Vec<int, 2>, 1>();
  BOOST_CHECK_EQUAL( should_be_unit_x_2d, unit_x_2d );
}

BOOST_AUTO_TEST_CASE( test_Mat ) {
  Mat<float, 2> mat{};
  mat( 0, 0 ) = 4;
  mat( 0, 1 ) = 3;
  mat( 1, 0 ) = 2;
  mat( 1, 1 ) = 1;

  Mat<float, 2> mat_times_2;
  mat_times_2( 0, 0 ) = 8;
  mat_times_2( 0, 1 ) = 6;
  mat_times_2( 1, 0 ) = 4;
  mat_times_2( 1, 1 ) = 2;

  BOOST_CHECK_EQUAL( mat * 2, mat_times_2 );               // operator*
  BOOST_CHECK_EQUAL( mat + mat + mat - mat, mat_times_2 ); // operator+, operator-

  Mat<float, 2, 3> other{0, 1, 2, /*<-first_row   second_row->*/ 3, 4, 5};
  Mat<float, 2, 3> mat_times_other{9, 16, 23 /**/, 3, 6, 9};
  BOOST_CHECK_EQUAL( mat * other, mat_times_other ); // operator*

  Mat<float, 3, 2> other_T{0, 3, 1, 4, 2, 5};
  BOOST_CHECK_EQUAL( other.transpose(), other_T ); // transpose()

  auto             sub = other.sub<Mat<float, 2, 2>, 0, 0>();
  Mat<float, 2, 2> should_be_sub{0, 1, 3, 4};
  BOOST_CHECK_EQUAL( sub, should_be_sub );

  Vec<float, 2> unit_y{0, 1};
  Vec<float, 2> sub_times_unit_y{1, 4};
  BOOST_CHECK_EQUAL( sub * unit_y, sub_times_unit_y );
}

BOOST_AUTO_TEST_CASE( test_MatSym ) {
  Mat<float, 2> _id2d{};
  _id2d( 0, 0 ) = 1;
  _id2d( 0, 1 ) = 0;
  _id2d( 1, 0 ) = 0;
  _id2d( 1, 1 ) = 1;

  auto sym_id_2d = _id2d.cast_to_sym();

  MatSym<float, 2> also_sym_id_2d{/*row0*/ 1, /*row1*/ 0, 1};

  BOOST_CHECK_EQUAL( sym_id_2d, also_sym_id_2d );                                    // filling
  BOOST_CHECK_EQUAL( ( sym_id_2d * sym_id_2d ).cast_to_sym(), sym_id_2d );           // operator*
  BOOST_CHECK_EQUAL( sym_id_2d + sym_id_2d + sym_id_2d - sym_id_2d, sym_id_2d * 2 ); // operator+, operator-

  Vec<float, 2> unit_y{0, 1};
  BOOST_CHECK_EQUAL( sym_id_2d * unit_y, unit_y ); // operator*

  MatSym<float, 2> random{}; // the values are not necessarily 0, the underyling array isn't filled
  BOOST_CHECK_EQUAL( random.cast_to_mat().cast_to_sym(), random ); // cast_to_mat

  //clang-format off
  MatSym<float, 3> someMatrix{2, -1, 2, 0, -1, 2};
  MatSym<float, 3> someMatrixInverse{0.75f, 0.5f, 1.f, 0.25f, 0.5f, 0.75f};
  //clang-format on
  bool isEqual                   = true;
  auto shouldBeSomeMatrixInverse = someMatrix.invChol();
  for ( auto i = 0u; i < someMatrixInverse.m.size(); ++i ) {
    if ( fabs( someMatrixInverse.m[i] - shouldBeSomeMatrixInverse.m[i] ) > std::numeric_limits<float>::epsilon() ) {
      isEqual = false;
      break;
    }
  }
  BOOST_CHECK( isEqual );

  Mat<float, 2> subOfSomeMatrix{-1, 2, 0, -1};
  auto          alsoSubOfSomeMatrix = someMatrix.sub<Mat<float, 2>, 1, 0>();
  BOOST_CHECK_EQUAL( subOfSomeMatrix, alsoSubOfSomeMatrix );
}
