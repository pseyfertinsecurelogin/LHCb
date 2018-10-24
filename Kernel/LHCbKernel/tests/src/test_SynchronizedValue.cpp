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
#define BOOST_TEST_MODULE test_MultiSynchronizedValue
#include <boost/test/unit_test.hpp>

#include "Kernel/SynchronizedValue.h"


namespace dummy {

enum State { Available, Read, Write};
State state = State::Available;

struct mutex {
    void lock(State s) {
        BOOST_CHECK (state==State::Available) ;
        state = s;
    }
    void unlock() {
        BOOST_CHECK (state!=State::Available) ;
        state = State::Available;
    }
};

template <State target>
class lock {
    mutex& m_;
public:
    lock(mutex& m) :m_{m} { m_.lock(target); }
    ~lock() { m_.unlock(); }
};
}

BOOST_AUTO_TEST_CASE( test_sv )
{
   LHCb::cxx::SynchronizedValue<int,dummy::mutex,
                                    dummy::lock<dummy::State::Read>,
                                    dummy::lock<dummy::State::Write>> sv{ 9 };
   BOOST_CHECK( dummy::state == dummy::State::Available );
   sv.with_lock( [](int& i) {
       BOOST_CHECK( dummy::state == dummy::State::Write);
       BOOST_CHECK( i == 9 );
       i = 3;
   });
   BOOST_CHECK( dummy::state == dummy::State::Available );
   sv.with_lock( [](const int& i) {
       BOOST_CHECK( dummy::state == dummy::State::Read);
       BOOST_CHECK( i == 3 );
   } );
   BOOST_CHECK( dummy::state == dummy::State::Available );
   std::as_const(sv).with_lock( [](const int& i) {
       BOOST_CHECK( dummy::state == dummy::State::Read);
       BOOST_CHECK( i == 3 );
   } ) ;
   BOOST_CHECK( dummy::state == dummy::State::Available );
   //std::as_const(sv).with_lock( [](int&) { } ); // should not compile
   //sv.with_lock( [](int) { } ); // should not compile
}


struct HasFun {
    int f() { return 1;}
    int g() const { return 2;}
    int h(int i) const { return 3+i;}
};

BOOST_AUTO_TEST_CASE( sv2 )
{
    LHCb::cxx::SynchronizedValue< HasFun > aaa;
    BOOST_CHECK( aaa.with_lock( [](HasFun& h) { return h.f(); } ) == 1 );
    // aaa.with_lock( [](HasFun h) { return h.f(); } );   // should not compile!
    BOOST_CHECK( aaa.with_lock( &HasFun::f ) == 1 );
    //std::as_const(sv).with_lock( &HasFun::f ); // should not compile
    BOOST_CHECK( aaa.with_lock( &HasFun::g ) == 2 );
    BOOST_CHECK( std::as_const(aaa).with_lock( &HasFun::g ) == 2 );
    BOOST_CHECK( std::as_const(aaa).with_lock( [](const HasFun& hf) { return hf.h(5); }) == 8 );
    BOOST_CHECK( std::as_const(aaa).with_lock( &HasFun::h, 5 ) == 8 );

    auto fun = [](const HasFun& hf, int i) { return hf.h(i); };
    auto fun2 = LHCb::cxx::with_lock(fun);
    BOOST_CHECK( fun2( aaa, 9 ) == 12 );
}
