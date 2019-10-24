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
#define BOOST_TEST_MODULE test_SynchronizedValue
#include <boost/test/unit_test.hpp>

#include "Kernel/SynchronizedValue.h"

namespace dummy {

  enum State { Available, Read, Write };
  State state = State::Available;

  struct mutex {
    void lock( State s ) {
      BOOST_CHECK( state == State::Available );
      state = s;
    }
    void unlock() {
      BOOST_CHECK( state != State::Available );
      state = State::Available;
    }
  };

  template <State target>
  class lock {
    mutex& m_;

  public:
    lock( mutex& m ) : m_{m} { m_.lock( target ); }
    ~lock() { m_.unlock(); }
  };
} // namespace dummy

BOOST_AUTO_TEST_CASE( test_sv ) {
  LHCb::cxx::SynchronizedValue<int, dummy::mutex, dummy::lock<dummy::State::Read>, dummy::lock<dummy::State::Write>> sv{
      9};
  BOOST_CHECK( dummy::state == dummy::State::Available );
  sv.with_lock( []( int& i ) {
    BOOST_CHECK( dummy::state == dummy::State::Write );
    BOOST_CHECK( i == 9 );
    i = 3;
  } );
  BOOST_CHECK( dummy::state == dummy::State::Available );
  sv.with_lock( []( const int& i ) {
    BOOST_CHECK( dummy::state == dummy::State::Read );
    BOOST_CHECK( i == 3 );
  } );
  BOOST_CHECK( dummy::state == dummy::State::Available );
  std::as_const( sv ).with_lock( []( const int& i ) {
    BOOST_CHECK( dummy::state == dummy::State::Read );
    BOOST_CHECK( i == 3 );
  } );
  BOOST_CHECK( dummy::state == dummy::State::Available );
  // std::as_const(sv).with_lock( [](int&) { } ); // should not compile
  // sv.with_lock( [](int) { } ); // should not compile
}

struct HasFun {
  int f() {
    BOOST_CHECK( dummy::state == dummy::State::Write );
    return 1;
  }
  [[nodiscard]] int g() const {
    BOOST_CHECK( dummy::state == dummy::State::Read );
    return 2;
  }
  [[nodiscard]] int h( int i ) const {
    BOOST_CHECK( dummy::state == dummy::State::Read );
    return 3 + i;
  }
  int i() noexcept {
    BOOST_CHECK( dummy::state == dummy::State::Write );
    return 4;
  }
  [[nodiscard]] int j() const noexcept {
    BOOST_CHECK( dummy::state == dummy::State::Read );
    return 5;
  }
};

int fun_noexcept( HasFun& ) noexcept {
  BOOST_CHECK( dummy::state == dummy::State::Write );
  return 9;
}
int fun_except( HasFun& ) {
  BOOST_CHECK( dummy::state == dummy::State::Write );
  return 10;
}

int constfun_noexcept( const HasFun& ) noexcept {
  BOOST_CHECK( dummy::state == dummy::State::Read );
  return 11;
}
int constfun_except( const HasFun& ) {
  BOOST_CHECK( dummy::state == dummy::State::Read );
  return 12;
}

BOOST_AUTO_TEST_CASE( svmem_fun ) {
  LHCb::cxx::SynchronizedValue<HasFun, dummy::mutex, dummy::lock<dummy::State::Read>, dummy::lock<dummy::State::Write>>
      aaa;
  BOOST_CHECK( aaa.with_lock( []( HasFun& h ) { return h.f(); } ) == 1 );
  // aaa.with_lock( [](HasFun h) { return h.f(); } );   // should not compile!
  BOOST_CHECK( aaa.with_lock( &HasFun::f ) == 1 );
  // std::as_const(sv).with_lock( &HasFun::f ); // should not compile
  BOOST_CHECK( aaa.with_lock( &HasFun::g ) == 2 );
  BOOST_CHECK( aaa.with_lock( &HasFun::i ) == 4 );
  BOOST_CHECK( std::as_const( aaa ).with_lock( &HasFun::j ) == 5 );
  BOOST_CHECK( std::as_const( aaa ).with_lock( &HasFun::g ) == 2 );
  BOOST_CHECK( std::as_const( aaa ).with_lock( []( const HasFun& hf ) { return hf.h( 5 ); } ) == 8 );
  BOOST_CHECK( std::as_const( aaa ).with_lock( &HasFun::h, 5 ) == 8 );

  auto fun  = []( const HasFun& hf, int i ) { return hf.h( i ); };
  auto fun2 = LHCb::cxx::with_lock( fun );
  BOOST_CHECK( fun2( aaa, 9 ) == 12 );

  auto lfun102 = LHCb::cxx::with_lock( []( const HasFun& hf, int i ) { return hf.h( 99 + i ); } );
  BOOST_CHECK( lfun102( aaa, 12 ) == ( 3 + 99 + 12 ) );

  BOOST_CHECK( aaa.with_lock( &fun_noexcept ) == 9 );
  BOOST_CHECK( aaa.with_lock( &fun_except ) == 10 );
  BOOST_CHECK( aaa.with_lock( &constfun_noexcept ) == 11 );
  BOOST_CHECK( aaa.with_lock( &constfun_except ) == 12 );
}
