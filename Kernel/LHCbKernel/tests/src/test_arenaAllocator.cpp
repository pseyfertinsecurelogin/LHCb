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
#include <vector>

// Replace new and delete just for the purpose of demonstrating that
// they are not called.

std::size_t memory = 0;
std::size_t alloc  = 0;

void* operator new( std::size_t s ) {
  memory += s;
  ++alloc;
  return malloc( s );
}
void operator delete( void* p ) {
  --alloc;
  free( p );
}

void operator delete( void* p, size_t ) {
  --alloc;
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
class Container {

  constexpr static std::size_t c_size = MaxEntries * ( sizeof( int ) + sizeof( double ) + sizeof( char ) );
  using Arena_t                       = LHCb::Allocators::StaticArena<c_size>;
  template <typename T>
  using Allocator = LHCb::Allocators::ArenaAllocator<T, c_size>;

  std::unique_ptr<Arena_t>               m_buffer = std::make_unique<Arena_t>();
  std::vector<int, Allocator<int>>       is{m_buffer.get()};
  std::vector<double, Allocator<double>> ds{m_buffer.get()};
  std::vector<char, Allocator<char>>     cs{m_buffer.get()};

public:
  Container() {
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

BOOST_AUTO_TEST_CASE( test_Container ) {

  memory = 0;
  alloc  = 0;

  Container<32> c;
  BOOST_CHECK( c.empty() );
  c.emplace_back( 1, 1.5, 'a' );
  c.emplace_back( 2, 4.5, 'b' );
  BOOST_CHECK( c.size() == 2 );
  auto ex_1 = Container<32>::Data{1, 1.5, 'a'};
  BOOST_CHECK( c[0] == ex_1 );

  BOOST_CHECK( alloc == 1 );
  // std::cout << memory << '\n';
  // BOOST_CHECK(memory==32*(4+8+1));

  auto c2 = std::move( c );
  BOOST_CHECK( alloc == 1 );
  BOOST_CHECK( c.empty() );
  BOOST_CHECK( c2.size() == 2 );
  BOOST_CHECK( c2[0] == ex_1 );
}
