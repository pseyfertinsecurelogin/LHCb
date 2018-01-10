#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_add_ref
#include <boost/test/unit_test.hpp>

#include "Event/TrackFunctor.h"


BOOST_AUTO_TEST_CASE( test_deref )
{
  {
    int a = 3;
    int b = 5;

    auto f = TrackFunctor::details::add_deref( [](int i, int j) { return i<j;} );

    BOOST_CHECK( f(a,b) == true );
    BOOST_CHECK( f(b,a) == false );
    BOOST_CHECK( f(&a,&b) == true );
    BOOST_CHECK( f(&b,&a) == false );

    auto g = [](int i) { return i+42;};
    auto h = TrackFunctor::details::add_deref( g );

    BOOST_CHECK( g(a) == 45 );
    BOOST_CHECK( h(a) == 45 );
    BOOST_CHECK( h(&a) == 45 );
  }
}

