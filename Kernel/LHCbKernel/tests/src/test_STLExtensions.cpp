#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_PlatformInfo
#include <boost/test/unit_test.hpp>

#include "Kernel/STLExtensions.h"
#include <numeric>
#include <type_traits>


template <typename ElementType, std::ptrdiff_t N>
auto sum( LHCb::span<ElementType,N> s ) {
    using std::begin; using std::end;
    return std::accumulate( begin(s), end(s), ElementType{0}, std::plus<>{} );
}

BOOST_AUTO_TEST_CASE( span )
{
  {
    std::array<int, 3> ai{9,10,11};
    BOOST_CHECK( sum( LHCb::make_span(ai) ) == 30 );
    std::vector<int> vi{ 9,10,11};
    BOOST_CHECK( sum( LHCb::make_span(vi) ) == 30 );
  }
}
