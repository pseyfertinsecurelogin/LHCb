#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE test_CallableTraits
#include <boost/test/unit_test.hpp>
#include "Kernel/CallableTraits.h"

template <typename T>
using arg0_t = typename LHCb::cxx::CallableTraits<T>::template argument_type<0>;


// check member functions
struct S {
    void f();
    void i() noexcept;
    void g() const;
    void h() const noexcept;
    void k() && ;
};

static_assert( std::is_same_v<S&, arg0_t<decltype(&S::f)>> );
static_assert( std::is_same_v<S&, arg0_t<decltype(&S::i)>> );
static_assert( std::is_same_v<const S&, arg0_t<decltype(&S::g)>> );
static_assert( std::is_same_v<const S&, arg0_t<decltype(&S::h)>> );
static_assert( std::is_same_v<S&&, arg0_t<decltype(&S::k)>> );

// check lambdas -- with class
auto sl = [](S&){};
auto slc = [](S const&){};
auto rsl = [](S&&){};

static_assert( std::is_same_v<S&, arg0_t<decltype(sl)>> );
static_assert( std::is_same_v<S const&, arg0_t<decltype(slc)>> );
static_assert( std::is_same_v<S&&, arg0_t<decltype(rsl)>> );

// check lambdas -- with POD
auto l = [](int&){};
auto lc = [](int const&){};

static_assert( std::is_same_v<int&, arg0_t<decltype(l)>> );
static_assert( std::is_same_v<int const&, arg0_t<decltype(lc)>> );

// check functions
int sf(S&) noexcept;
int sfc(S const&);
int f(int&);
int fc(int const&);

static_assert( std::is_same_v<S&, arg0_t<decltype(sf)>> );
static_assert( std::is_same_v<S const&, arg0_t<decltype(sfc)>> );
static_assert( std::is_same_v<int&, arg0_t<decltype(f)>> );
static_assert( std::is_same_v<int const&, arg0_t<decltype(fc)>> );

BOOST_AUTO_TEST_CASE( test_ct )
{
    BOOST_CHECK( true ); // we just want the above to compile...
}
