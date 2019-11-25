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
#include "Kernel/Container.h"

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
void operator delete(void* p)_GLIBCXX_USE_NOEXCEPT {
  ++dealloc;
  free( p );
}
void operator delete(void* p, size_t)_GLIBCXX_USE_NOEXCEPT {
  ++dealloc;
  free( p );
}

//   Note for vector it is possible to reduce the alignment requirements
//   down to alignof(T) because vector doesn't allocate anything but T's.
//   And if we're wrong about that guess, it is a compile-time error, not
//   a run time error.
template <class T, std::size_t BufSize = 200>
using SmallVector = std::vector<T, LHCb::Allocators::StaticArenaAllocator<T, BufSize, alignof( T )>>;

BOOST_AUTO_TEST_CASE( test_SmallVector ) {

  memory = 0;
  alloc  = 0;
  // Create the stack-based arena from which to allocate
  SmallVector<int>::allocator_type::arena_t a;
  BOOST_CHECK_EQUAL( a.used(), 0 );
  // Create the vector which uses that arena.
  SmallVector<int> v{&a};
  // Exercise the vector and note that new/delete are not getting called.
  v.push_back( 1 );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) );
  v.push_back( 2 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  v.push_back( 3 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  v.push_back( 4 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  // Yes, the correct values are actually in the vector
  BOOST_CHECK_EQUAL( v[0], 1 );
  BOOST_CHECK_EQUAL( v[1], 2 );
  BOOST_CHECK_EQUAL( v[2], 3 );
  BOOST_CHECK_EQUAL( v[3], 4 );

  // continue to use the arena for a second vector...
  SmallVector<int> v2{&a};
  v2.reserve( 10 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 9 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 99 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
  v2.push_back( 99 );
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );

  {
    // check what happens after a move
    auto vd = v.data();
    auto v3 = std::move( v );
    BOOST_CHECK_EQUAL( v3.data(), vd );
    BOOST_CHECK( v.empty() );
    BOOST_CHECK_EQUAL( v3[0], 1 );
    BOOST_CHECK_EQUAL( v3[1], 2 );
    BOOST_CHECK_EQUAL( v3[2], 3 );
    BOOST_CHECK_EQUAL( v3[3], 4 );

    BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
    BOOST_CHECK_EQUAL( memory, 0 );
    BOOST_CHECK_EQUAL( alloc, 0 );

    v3.push_back( 5 );
    BOOST_CHECK_EQUAL( a.used(),
                       sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) + 8 * sizeof( int ) );
    BOOST_CHECK_EQUAL( memory, 0 );
    BOOST_CHECK_EQUAL( alloc, 0 );
  }
  // check deallocation by letting vd go out of scope, _after_ it allocated something...
  BOOST_CHECK_EQUAL( a.used(), sizeof( int ) + 2 * sizeof( int ) + 4 * sizeof( int ) + 10 * sizeof( int ) );
}

template <class T, std::size_t BufSize = 256>
using SmallList = std::list<T, LHCb::Allocators::StaticArenaAllocator<T, BufSize>>;

BOOST_AUTO_TEST_CASE( test_SmallList ) {

  memory = 0;
  alloc  = 0;
  SmallList<int>::allocator_type::arena_t a;
  SmallList<int>                          v{&a};
  constexpr int                           node_size = 32;
  BOOST_CHECK_EQUAL( a.used(), 0 );
  v.push_back( 1 );
  BOOST_CHECK_EQUAL( a.used(), node_size );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  v.push_back( 2 );
  BOOST_CHECK_EQUAL( a.used(), 2 * node_size );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  v.push_back( 3 );
  BOOST_CHECK_EQUAL( a.used(), 3 * node_size );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );
  v.push_back( 4 );
  BOOST_CHECK_EQUAL( a.used(), 4 * node_size );
  BOOST_CHECK_EQUAL( memory, 0 );
  BOOST_CHECK_EQUAL( alloc, 0 );

  BOOST_CHECK_EQUAL( v.front(), 1 );
  BOOST_CHECK_EQUAL( v.back(), 4 );
}

template <size_t MaxEntries>
class StaticContainer {
  constexpr static std::size_t c_size = MaxEntries * ( sizeof( int ) + sizeof( double ) + sizeof( char ) );
  using Arena_t                       = LHCb::Allocators::StaticArena<c_size>;
  template <typename T>
  using Allocator = LHCb::Allocators::StaticArenaAllocator<T, c_size>;

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

  [[nodiscard]] size_t size() const { return cs.size(); }
  [[nodiscard]] bool   empty() const { return cs.empty(); }
};

BOOST_AUTO_TEST_CASE( test_StaticContainer ) {

  memory = 0;
  alloc  = 0;

  StaticContainer<32> c;
  BOOST_CHECK( c.empty() );
  c.emplace_back( 1, 1.5, 'a' );
  c.emplace_back( 2, 4.5, 'b' );
  BOOST_CHECK_EQUAL( c.size(), 2 );
  auto ex_1 = StaticContainer<32>::Data{1, 1.5, 'a'};
  BOOST_CHECK( c[0] == ex_1 );

  BOOST_CHECK_EQUAL( alloc, 0 );
  BOOST_CHECK_EQUAL( memory, 0 );
}
