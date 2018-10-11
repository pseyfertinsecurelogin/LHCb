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

#include "Kernel/MultiIndexedContainer.h"

using LHCb::Container::detail::Extents;

static_assert( Extents<2,3,4>::offset(0) ==  0 );
static_assert( Extents<2,3,4>::offset(1) == 12 );

static_assert( Extents<2,3,4>::offset(0,0) == 0);
static_assert( Extents<2,3,4>::offset(0,1) == 4);
static_assert( Extents<2,3,4>::offset(0,2) == 8);
static_assert( Extents<2,3,4>::offset(1,0) == 12);
static_assert( Extents<2,3,4>::offset(1,1) == 16);
static_assert( Extents<2,3,4>::offset(1,2) == 20);

static_assert( Extents<2,3,4>::offset(0,0,0) == 0);
static_assert( Extents<2,3,4>::offset(0,0,1) == 1);
static_assert( Extents<2,3,4>::offset(0,0,2) == 2);

static_assert( Extents<2,3,4>::offset(0,1,0) == 4);
static_assert( Extents<2,3,4>::offset(0,1,1) == 5);
static_assert( Extents<2,3,4>::offset(0,1,2) == 6);

static_assert( Extents<2,3,4>::offset(0,2,0) == 8);
static_assert( Extents<2,3,4>::offset(0,2,1) == 9);
static_assert( Extents<2,3,4>::offset(0,2,2) == 10);

static_assert( Extents<2,3,4>::offset(1,2,0) == 20);
static_assert( Extents<2,3,4>::offset(1,2,1) == 21);
static_assert( Extents<2,3,4>::offset(1,2,2) == 22);

static_assert( Extents<9,12,9>::offset(3,8,2) == 3*(12*9) + 8*(9) + 2 );
static_assert( Extents<9,12,10>::offset(3,8,2) == 3*Extents<9,12,10>::stride<0>()
                                                + 8*Extents<9,12,10>::stride<1>()
                                                + 2*Extents<9,12,10>::stride<2>() );

static_assert( Extents<9,12,10>::stride<0>() == 12* Extents<9,12,10>::stride<1>() );
static_assert( Extents<9,12,10>::stride<1>() == 10* Extents<9,12,10>::stride<2>() );
static_assert( Extents<9,12,10>::stride<2>() == 1 );

BOOST_AUTO_TEST_CASE( mic )
{
  {
    auto i0 = Extents<3,8,9,12>::offset( 1,2,3,4 ); BOOST_CHECK( i0 == 1*(8*9*12) + 2*(9*12) + 3*(12) + 4*(1) );
    auto i1 = Extents<3,8,9,12>::offset( 1,2,3,4 ); BOOST_CHECK( i1 == 1*(8*9*12) + 2*(9*12) + 3*(12) + 4*(1) );

    int ioffset = 0;
    std::array<int,4> wrong = {0,0,0,0 };
    for (int i=0;i<Extents<3,8,9,10>::size<0>(); ++i) {
        if (Extents<3,8,9,10>::offset(i) != ioffset) ++wrong[0];
        for (int j=0;j<Extents<3,8,9,10>::size<1>(); ++j) {
            if (Extents<3,8,9,10>::offset(i,j) != ioffset) ++wrong[1];
            for (int k=0;k<Extents<3,8,9,10>::size<2>(); ++k) {
                if (Extents<3,8,9,10>::offset(i,j,k) != ioffset) ++wrong[2];
                for (int l=0;l<Extents<3,8,9,10>::size<3>(); ++l) {
                    if ( Extents<3,8,9,10>::offset(i,j,k,l) != ioffset ) ++wrong[3];
                    ++ioffset;
    } } } }
    BOOST_CHECK( wrong[0] == 0 );
    BOOST_CHECK( wrong[1] == 0 );
    BOOST_CHECK( wrong[2] == 0 );
    BOOST_CHECK( wrong[3] == 0 );

  }
}

