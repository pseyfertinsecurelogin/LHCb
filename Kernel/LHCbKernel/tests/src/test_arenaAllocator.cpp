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

#include "Kernel/ArenaAllocator.h"
#include <new>
#include <numeric>
#include <vector>

// Replace new and delete just for the purpose of demonstrating that
// they are not called.

std::size_t memory  = 0;
std::size_t alloc   = 0;
std::size_t dealloc = 0;

void* operator new( std::size_t s ) {
  memory += s;
  ++alloc;
  return malloc( s );
}
void operator delete( void* p ) {
  ++dealloc;
  free( p );
}
void operator delete( void* p, size_t ) {
  ++dealloc;
  free( p );
}

//   Note for vector it is possible to reduce the alignment requirements
//   down to alignof(T) because vector doesn't allocate anything but T's.
//   And if we're wrong about that guess, it is a compile-time error, not
//   a run time error.
template <class T, std::size_t BufSize = 200>
using SmallVector = std::vector<T, LHCb::Allocators::ArenaAllocator<T, BufSize, alignof( T )>>;

BOOST_AUTO_TEST_CASE( test_SmallVector ) {

  memory = 0;
  alloc  = 0;
  // Create the stack-based arena from which to allocate
  SmallVector<int>::allocator_type::arena_t a;
  BOOST_CHECK( a.used() == 0 );
  // Create the vector which uses that arena.
  SmallVector<int> v{&a};
  // Exercise the vector and note that new/delete are not getting called.
  v.push_back( 1 );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  BOOST_CHECK( a.used() == sizeof( int ) );
  v.push_back( 2 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  v.push_back( 3 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  v.push_back( 4 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  // Yes, the correct values are actually in the vector
  BOOST_CHECK( v[0] == 1 );
  BOOST_CHECK( v[1] == 2 );
  BOOST_CHECK( v[2] == 3 );
  BOOST_CHECK( v[3] == 4 );

  // continue to use the arena for a second vector...
  SmallVector<int> v2{&a};
  v2.reserve( 10 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 9 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 99 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 99 );
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );

  {
    // check what happens after a move
    auto vd = v.data();
    auto v3 = std::move( v );
    BOOST_CHECK( v3.data() == vd );
    BOOST_CHECK( v.empty() );
    BOOST_CHECK( v3[0] == 1 );
    BOOST_CHECK( v3[1] == 2 );
    BOOST_CHECK( v3[2] == 3 );
    BOOST_CHECK( v3[3] == 4 );

    BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
    BOOST_CHECK( memory == 0 );
    BOOST_CHECK( alloc == 0 );

    v3.push_back( 5 );
    BOOST_CHECK( a.used() ==
                 sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) + 8 * sizeof( int ) );
    BOOST_CHECK( memory == 0 );
    BOOST_CHECK( alloc == 0 );
  }
  // check deallocation by letting vd go out of scope, _after_ it allocated something...
  BOOST_CHECK( a.used() == sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
}

template <class T, std::size_t BufSize = 256>
using SmallList = std::list<T, LHCb::Allocators::ArenaAllocator<T, BufSize>>;

BOOST_AUTO_TEST_CASE( test_SmallList ) {

  memory = 0;
  alloc  = 0;
  SmallList<int>::allocator_type::arena_t a;
  SmallList<int>                          v{&a};
  constexpr int                           node_size = 32;
  BOOST_CHECK( a.used() == 0 );
  v.push_back( 1 );
  BOOST_CHECK( a.used() == node_size );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  v.push_back( 2 );
  BOOST_CHECK( a.used() == 2 * node_size );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  v.push_back( 3 );
  BOOST_CHECK( a.used() == 3 * node_size );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );
  v.push_back( 4 );
  BOOST_CHECK( a.used() == 4 * node_size );
  BOOST_CHECK( memory == 0 );
  BOOST_CHECK( alloc == 0 );

  BOOST_CHECK( v.front() == 1 );
  BOOST_CHECK( v.back() == 4 );
}

template <size_t MaxEntries>
class StaticContainer {
  constexpr static std::size_t c_size = MaxEntries * ( sizeof( int ) + sizeof( double ) + sizeof( char ) );
  using Arena_t                       = LHCb::Allocators::StaticArena<c_size>;
  template <typename T>
  using Allocator = LHCb::Allocators::ArenaAllocator<T, c_size>;

  Arena_t                                m_buffer;
  std::vector<int, Allocator<int>>       is{&m_buffer};
  std::vector<double, Allocator<double>> ds{&m_buffer};
  std::vector<char, Allocator<char>>     cs{&m_buffer};

public:
  StaticContainer() {
    cs.reserve( MaxEntries );
    is.reserve( MaxEntries );
    ds.reserve( MaxEntries );
  }

  struct Data {
    int         i;
    double      d;
    char        c;
    friend bool operator==( const Data& lhs, const Data& rhs ) {
      return lhs.i == rhs.i && lhs.d == rhs.d && lhs.c == rhs.c;
    }
  };

  Data operator[]( int i ) const { return {is[i], ds[i], cs[i]}; }

  void emplace_back( int i, double d, char c ) {
    is.push_back( i );
    ds.push_back( d );
    cs.push_back( c );
  }

  size_t size() const { return cs.size(); }
  bool   empty() const { return cs.empty(); }
};

BOOST_AUTO_TEST_CASE( test_StaticContainer ) {

  memory = 0;
  alloc  = 0;

  StaticContainer<32> c;
  BOOST_CHECK( c.empty() );
  c.emplace_back( 1, 1.5, 'a' );
  c.emplace_back( 2, 4.5, 'b' );
  BOOST_CHECK( c.size() == 2 );
  auto ex_1 = StaticContainer<32>::Data{1, 1.5, 'a'};
  BOOST_CHECK( c[0] == ex_1 );

  BOOST_CHECK( alloc == 0 );
  BOOST_CHECK( memory == 0 );
}

namespace details {
  template <std::size_t N, typename T>
  std::array<T, N> distribute( T t ) {
    std::array<T, N> a;
    a.fill( t );
    return a;
  }

  template <typename F, typename T1, typename T2, std::size_t... I>
  constexpr void apply_binary_impl( F f, T1&& t1, T2&& t2, std::index_sequence<I...> ) {
    ( std::invoke( f, std::get<I>( std::forward<T1>( t1 ) ), std::get<I>( std::forward<T2>( t2 ) ) ), ... );
  }
  template <typename F, typename T1, typename T2>
  constexpr void apply_binary( F&& f, T1&& t1, T2&& t2 ) {
    static_assert( std::tuple_size_v<std::remove_reference_t<T1>> == std::tuple_size_v<std::remove_reference_t<T2>> );
    apply_binary_impl( std::forward<F>( f ), std::forward<T1>( t1 ), std::forward<T2>( t2 ),
                       std::make_index_sequence<std::tuple_size_v<std::remove_reference_t<T1>>>{} );
  }
} // namespace details

template <typename... Args>
class Container {
  constexpr static auto N = sizeof...( Args );
  static_assert( N > 1 );
  constexpr static auto alignment = std::max( {sizeof( Args )...} );
  // constexpr static auto alignment = 64;
  template <typename T>
  using Allocator = LHCb::Allocators::DynamicArenaAllocator<T, alignment>;
  using Arena     = LHCb::Allocators::DynamicArena<alignment>;
  using Data      = std::tuple<std::vector<Args, Allocator<Args>>...>;

  static constexpr std::size_t n_bytes( std::size_t n ) { return ( Arena::template align_up<Args>( n ) + ... ); }

  static constexpr std::size_t n_bytes( std::array<std::size_t, N> n ) {
    auto sizes = std::array{Arena::template align_up<Args>( 1 )...};
    return std::inner_product( sizes.begin(), sizes.end(), n.begin(), 0 );
  }

  typename Arena::Handle m_buffer;
  Data                   m_data = std::make_from_tuple<Data>( details::distribute<N>( m_buffer.get() ) );

public:
  Container( std::size_t maxElements ) : m_buffer{Arena::create( n_bytes( maxElements ) )} {
    std::apply( [maxElements]( auto&... d ) { ( d.reserve( maxElements ), ... ); }, m_data );
  }

  Container( std::array<std::size_t, N> maxElements ) : m_buffer{Arena::create( n_bytes( maxElements ) )} {
    details::apply_binary( []( auto& c, auto n ) { c.reserve( n ); }, m_data, maxElements );
  }

  auto operator[]( int i ) const {
    return std::apply( [i]( const auto&... d ) { return std::make_tuple( d[i]... ); }, m_data );
  }

  template <typename T>
  const auto& column() const {
    return std::get<std::vector<T, Allocator<T>>>( m_data );
  }
  template <typename T>
  auto& column() {
    return std::get<std::vector<T, Allocator<T>>>( m_data );
  }

  template <typename T>
  const auto& column( int i ) const {
    return std::get<std::vector<T, Allocator<T>>>( m_data )[i];
  }

  template <size_t I>
  const auto& column() const {
    return std::get<I>( m_data );
  }
  template <size_t I>
  auto& column() {
    return std::get<I>( m_data );
  }

  template <size_t I>
  const auto& column( int i ) const {
    return std::get<I>( m_data )[i];
  }

  template <typename... U>
  void emplace_back( U&&... arg ) {
    // C++20: std::apply( [..arg=std::forward<U>(arg)](auto&... d) { (d.push_back(std::forward<U>(arg)),...); }, m_data
    // );
    std::apply( [&]( auto&... d ) { ( d.push_back( std::forward<U>( arg ) ), ... ); }, m_data );
  }

  size_t size() const { return std::get<0>( m_data ).size(); }
  bool   empty() const { return std::get<0>( m_data ).empty(); }
};

BOOST_AUTO_TEST_CASE( test_Container1 ) {
  memory = 0;
  alloc  = 0;

  std::size_t                  N = 64;
  Container<int, double, char> c{64};

  BOOST_CHECK( alloc == 1 );

  c.emplace_back( 1, 2.5, 'a' );
  BOOST_CHECK( alloc == 1 );

  BOOST_CHECK( c[0] == std::make_tuple( 1, 2.5, 'a' ) );
  BOOST_CHECK( c.column<0>( 0 ) == 1 );
  BOOST_CHECK( c.column<char>().at( 0 ) == 'a' );
  BOOST_CHECK( c.column<double>( 0 ) == 2.5 );

  c.emplace_back( 2, 4.5, 'b' );
  for ( int i = 2; i < static_cast<int>( N ); ++i ) {
    c.emplace_back( i * 3, i * 1.5, 'c' );
    BOOST_CHECK( alloc == 1 );
  }
  BOOST_CHECK( c.size() == N );

  auto c2 = std::move( c );
  BOOST_CHECK( c.empty() );
  BOOST_CHECK( c2.size() == N );
  BOOST_CHECK( c2[0] == std::make_tuple( 1, 2.5, 'a' ) );

  BOOST_CHECK( alloc == 1 );
}

BOOST_AUTO_TEST_CASE( test_Container2 ) {
  memory = 0;
  alloc  = 0;
  Container<LHCb::span<int>, int> c{{4, 100}}; // allocates, using a _single_ allocation,  memory for 4 spans, and
                                               // 100 doubles...
  auto& ints  = c.column<int>();               // get the column of doubles
  auto& spans = c.column<LHCb::span<int>>();   // get the column of spans

  // intersprerse writing to ints and span,
  // and demonstrate that the spans remain valid
  for ( int i = 0; i < 5; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 0, 5 ) );

  for ( int i = 5; i < 25; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 5, 20 ) );

  for ( int i = 25; i < 80; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 25, 55 ) );

  for ( int i = 80; i < 100; ++i ) ints.push_back( i );
  spans.emplace_back( LHCb::make_span( ints ).subspan( 80, 20 ) );

  BOOST_CHECK( alloc == 1 );

  auto c2 = std::move( c );  // move the container
  BOOST_CHECK( alloc == 1 ); // check that this doesn't allocate memory
  BOOST_CHECK( c.empty() );

  // and verify that the spans inside the 'moved' container are still valid
  std::vector<int> sums;
  for ( auto s : c2.column<LHCb::span<int>>() ) { sums.push_back( std::accumulate( s.begin(), s.end(), 0 ) ); }
  auto euler = []( int n ) { return n * ( n + 1 ) / 2; };
  BOOST_CHECK( sums.at( 0 ) == euler( 4 ) );
  BOOST_CHECK( sums.at( 1 ) == euler( 24 ) - euler( 4 ) );
  BOOST_CHECK( sums.at( 2 ) == euler( 79 ) - euler( 24 ) );
  BOOST_CHECK( sums.at( 3 ) == euler( 99 ) - euler( 79 ) );
}
