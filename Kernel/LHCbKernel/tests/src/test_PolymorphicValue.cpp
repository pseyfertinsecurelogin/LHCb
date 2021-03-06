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

#include "Kernel/PolymorphicValue.h"
#include <atomic>
#include <iostream>

template <typename T>
class InstanceCounter {
  static std::atomic<std::size_t> s_counter;

public:
  InstanceCounter() { ++s_counter; }
  InstanceCounter( const InstanceCounter& ) { ++s_counter; }
  ~InstanceCounter() { --s_counter; }
  static std::size_t count() { return s_counter; }
};

template <typename T>
std::atomic<std::size_t> InstanceCounter<T>::s_counter = {0};

namespace unique {
  struct IFace {
    virtual ~IFace()                                           = default;
    [[nodiscard]] virtual std::unique_ptr<IFace> clone() const = 0;
    [[nodiscard]] virtual int                    f() const     = 0;
    virtual int                                  f()           = 0;
  };

  class A : public IFace, private InstanceCounter<A> {

  public:
    ~A() override = default;
    [[nodiscard]] std::unique_ptr<IFace> clone() const override { return std::make_unique<A>( *this ); }
    [[nodiscard]] int                    f() const override { return 1; }
    int                                  f() override { return 2; }
  };

  class B : public IFace, private InstanceCounter<B> {

  public:
    ~B() override = default;
    [[nodiscard]] std::unique_ptr<IFace> clone() const override { return std::make_unique<B>( *this ); }
    [[nodiscard]] int                    f() const override { return 3; }
    int                                  f() override { return 4; }
  };
} // namespace unique

namespace plain {
  struct IFace {
    virtual ~IFace()                           = default;
    [[nodiscard]] virtual IFace* clone() const = 0;
    [[nodiscard]] virtual int    f() const     = 0;
    virtual int                  f()           = 0;
  };

  class A : public IFace, private InstanceCounter<A> {

  public:
    ~A() override = default;
    [[nodiscard]] A*  clone() const override { return new A( *this ); }
    [[nodiscard]] int f() const override { return 1; }
    int               f() override { return 2; }
  };

  class B : public IFace, private InstanceCounter<B> {

  public:
    ~B() override = default;
    [[nodiscard]] B*  clone() const override { return new B( *this ); }
    [[nodiscard]] int f() const override { return 3; }
    int               f() override { return 4; }
  };
} // namespace plain

// Composite exists to demonstrate that the default, compiler generated copy/move constructor
// and destructor are all that is needed...
template <typename I>
class Composite {
  LHCb::cxx::PolymorphicValue<I> iface;

public:
  Composite( std::unique_ptr<I> up ) : iface{std::move( up )} {}
  [[nodiscard]] int f() const { return iface ? iface->f() : -1; }
  int               f() { return iface ? iface->f() : -2; }
  void              setIFace( std::unique_ptr<I> up ) { iface = LHCb::cxx::PolymorphicValue{std::move( up )}; }

  friend void swap( Composite& lhs, Composite& rhs ) noexcept {
    swap( lhs.iface, rhs.iface );
    ;
  }
};

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_plain ) {
  BOOST_CHECK( InstanceCounter<plain::A>::count() == 0 );
  BOOST_CHECK( InstanceCounter<plain::B>::count() == 0 );
  {
    auto c = Composite<plain::IFace>{std::make_unique<plain::A>()};
    BOOST_CHECK( InstanceCounter<plain::A>::count() == 1 );
    auto d = c;
    BOOST_CHECK( InstanceCounter<plain::A>::count() == 2 );
    BOOST_CHECK( c.f() == 2 );
    BOOST_CHECK( std::as_const( c ).f() == 1 );
    c.setIFace( std::make_unique<plain::B>() );
    BOOST_CHECK( InstanceCounter<plain::A>::count() == 1 );
    BOOST_CHECK( InstanceCounter<plain::B>::count() == 1 );
    BOOST_CHECK( c.f() == 4 );
    BOOST_CHECK( std::as_const( c ).f() == 3 );
    BOOST_CHECK( d.f() == 2 );
    BOOST_CHECK( std::as_const( d ).f() == 1 );
    // std::as_const(d).g(); // should not, and does not, compile
    c = std::as_const( d );
    BOOST_CHECK( InstanceCounter<plain::A>::count() == 2 );
    BOOST_CHECK( InstanceCounter<plain::B>::count() == 0 );
    BOOST_CHECK( c.f() == 2 );
    BOOST_CHECK( std::as_const( c ).f() == 1 );
    c = std::move( d );
    BOOST_CHECK( InstanceCounter<plain::A>::count() == 1 );
    BOOST_CHECK( InstanceCounter<plain::B>::count() == 0 );
  }
  BOOST_CHECK( InstanceCounter<plain::A>::count() == 0 );
  BOOST_CHECK( InstanceCounter<plain::B>::count() == 0 );
}

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_unique ) {
  BOOST_CHECK( InstanceCounter<unique::A>::count() == 0 );
  BOOST_CHECK( InstanceCounter<unique::B>::count() == 0 );
  {
    auto c = Composite<unique::IFace>{std::make_unique<unique::A>()};
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 1 );
    auto d = c;
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 2 );
    c.f();
    c.setIFace( std::make_unique<unique::B>() );
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 1 );
    BOOST_CHECK( InstanceCounter<unique::B>::count() == 1 );
    c.f();
    d.f();
    // std::as_const(d).g(); // should not, and does not, compile
    swap( c, d );
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 1 );
    BOOST_CHECK( InstanceCounter<unique::B>::count() == 1 );

    c = std::as_const( d );
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 0 );
    BOOST_CHECK( InstanceCounter<unique::B>::count() == 2 );
    c.f();

    c = std::move( d );
    BOOST_CHECK( InstanceCounter<unique::A>::count() == 0 );
    BOOST_CHECK( InstanceCounter<unique::B>::count() == 1 );
  }
  BOOST_CHECK( InstanceCounter<unique::A>::count() == 0 );
  BOOST_CHECK( InstanceCounter<unique::B>::count() == 0 );
}

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_compare ) {
  auto iface_99 = LHCb::cxx::PolymorphicValue<unique::IFace>{std::make_unique<unique::A>()};

  BOOST_CHECK( iface_99 == iface_99 );
  BOOST_CHECK( !( iface_99 != iface_99 ) );

  auto iface_11 = LHCb::cxx::PolymorphicValue<unique::IFace>{std::make_unique<unique::B>()};
  BOOST_CHECK( iface_99 != iface_11 );
  BOOST_CHECK( !( iface_99 == iface_11 ) );

  BOOST_CHECK( ( iface_99 < iface_11 ) == ( iface_99.ptr() < iface_11.ptr() ) );
  BOOST_CHECK( ( iface_99 > iface_11 ) == ( iface_99.ptr() > iface_11.ptr() ) );

  BOOST_CHECK( ( iface_99 <= iface_11 ) == ( iface_99.ptr() <= iface_11.ptr() ) );
  BOOST_CHECK( ( iface_99 >= iface_11 ) == ( iface_99.ptr() >= iface_11.ptr() ) );

  BOOST_CHECK( iface_99 <= iface_99 );
  BOOST_CHECK( iface_99 >= iface_99 );

  BOOST_CHECK( !( iface_99 < iface_99 ) );
  BOOST_CHECK( !( iface_99 > iface_99 ) );

  BOOST_CHECK( ( iface_99 < iface_11 ) != ( iface_99 >= iface_11 ) );
  BOOST_CHECK( ( iface_99 > iface_11 ) != ( iface_99 <= iface_11 ) );

  BOOST_CHECK( ( iface_99 < iface_11 ) == ( iface_11 >= iface_99 ) );
  BOOST_CHECK( ( iface_99 > iface_11 ) == ( iface_11 <= iface_99 ) );
}

#include <functional>

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_hash ) {
  auto iface_a = LHCb::cxx::PolymorphicValue<unique::IFace>{std::make_unique<unique::A>()};

  auto ptr_a = iface_a.ptr();
  auto hash1 = std::hash<decltype( ptr_a )>{}( ptr_a );
  auto hash2 = std::hash<decltype( iface_a )>{}( iface_a );
  BOOST_CHECK( hash1 == hash2 );
}

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_deref ) {
  auto iface_a = LHCb::cxx::PolymorphicValue<unique::IFace>{std::make_unique<unique::A>()};

  const auto&           r  = *iface_a;
  [[maybe_unused]] auto rf = r.f();

  iface_a->f();
}

BOOST_AUTO_TEST_CASE( test_PolymorphicValue_def_const ) {
  LHCb::cxx::PolymorphicValue<unique::IFace> iface_a;
  BOOST_CHECK( !iface_a );
}

#if 0
// check error messages when passing a non-conforming interface

struct BadIFace1 {}; // no clone() at all
LHCb::cxx::PolymorphicValue<BadIFace1> bad{ std::make_unique<BadIFace1>() };

struct BadIFace2 { void clone() const {} }; //clone with wrong return type
LHCb::cxx::PolymorphicValue<BadIFace2> bad2{ std::make_unique<BadIFace2>() };

struct BadIFace3 { BadIFace3* clone() {} }; // non-const clone
LHCb::cxx::PolymorphicValue<BadIFace3> bad3{ std::make_unique<BadIFace3>() };
#endif
