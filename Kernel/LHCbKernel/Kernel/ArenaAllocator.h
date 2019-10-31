// The MIT License (MIT)
//
// Copyright (c) 2015 Howard Hinnant
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
#pragma once

#include <cassert>
#include <cstddef>
#include <memory>
#include <new>
#include <utility>

#include "STLExtensions.h"

namespace LHCb::Allocators {

  namespace details {
    template <std::size_t Alignment>
    constexpr std::size_t align_up( std::size_t n ) {
      return ( n + ( Alignment - 1 ) ) & ~( Alignment - 1 );
    }
  } // namespace details

  template <std::size_t Alignment>
  class DynamicArena {

    std::byte*      m_current;
    span<std::byte> m_buffer;

    bool is_inside( std::byte* p ) const noexcept { return m_buffer.data() <= p && p <= m_buffer.data() + capacity(); }

    DynamicArena( span<std::byte> buffer ) noexcept : m_current{buffer.data()}, m_buffer{buffer} {
      assert( reinterpret_cast<std::byte*>( this ) + details::align_up<Alignment>( sizeof( DynamicArena ) ) ==
              buffer.data() );
    }

    struct DynamicArenaDeleter {
      void operator()( DynamicArena<Alignment>* p ) {
        p->~DynamicArena<Alignment>();
        delete[] reinterpret_cast<std::byte*>( p );
      }
    };

  public:
    template <typename T>
    static constexpr std::size_t align_up( std::size_t N ) {
      return details::align_up<Alignment>( N * sizeof( T ) );
    }

    using Handle = std::unique_ptr<DynamicArena<Alignment>, DynamicArenaDeleter>;

    // create a dynamic arena with (at least) N bytes of storage...
    static auto create( size_t N ) {
      constexpr auto daSize = details::align_up<Alignment>( sizeof( DynamicArena<Alignment> ) );
      auto           M      = details::align_up<Alignment>( N );
      auto           buf    = new std::byte[M + daSize];
      return Handle( new ( buf )
                         DynamicArena<Alignment>( {buf + daSize, static_cast<span<std::byte>::index_type>( M )} ) );
    }

    ~DynamicArena()                     = default;
    DynamicArena( const DynamicArena& ) = delete;
    DynamicArena& operator=( const DynamicArena& ) = delete;
    DynamicArena( DynamicArena&& rhs )             = delete;
    DynamicArena& operator=( DynamicArena&& rhs ) = delete;

    static constexpr std::size_t alignment = Alignment;
    [[nodiscard]] std::size_t    capacity() const noexcept { return m_buffer.size(); }

    // TODO: use CRTP to share the code below...
    [[nodiscard]] std::size_t used() const noexcept { return static_cast<std::size_t>( m_current - m_buffer.data() ); }

    template <std::size_t ReqAlign>
    std::byte* allocate( std::size_t n ) {
      static_assert( ReqAlign <= alignment, "alignment is too small for this arena" );
      auto const aligned_n = details::align_up<Alignment>( n );
      if ( static_cast<decltype( aligned_n )>( m_buffer.data() + capacity() - m_current ) < aligned_n )
        throw std::bad_alloc{};
      return std::exchange( m_current, m_current + aligned_n );
    }

    void deallocate( std::byte* p, std::size_t n ) noexcept {
      assert( is_inside( m_current ) );
      assert( is_inside( p ) );
      if ( p + details::align_up<Alignment>( n ) == m_current ) m_current = p;
    }

    void reset() noexcept { m_current = m_buffer; } // FIXME
  };

  template <std::size_t N, std::size_t Alignment = alignof( std::max_align_t )>
  class StaticArena {
    std::byte* m_current = nullptr;
    alignas( Alignment ) std::byte m_buffer[N]{};

    bool is_inside( std::byte* p ) const noexcept { return m_buffer <= p && p <= m_buffer + capacity(); }

  public:
    ~StaticArena() { m_current = nullptr; }
    StaticArena() noexcept : m_current{m_buffer} {}
    StaticArena( const StaticArena& ) = delete;
    StaticArena& operator=( const StaticArena& ) = delete;
    StaticArena( StaticArena&& )                 = delete;
    StaticArena& operator=( StaticArena&& ) = delete;

    static constexpr std::size_t alignment = Alignment;
    static constexpr std::size_t capacity() noexcept { return N; }

    // TODO: use CRTP to share the code below...
    [[nodiscard]] std::size_t used() const noexcept { return static_cast<std::size_t>( m_current - m_buffer ); }

    template <std::size_t ReqAlign>
    std::byte* allocate( std::size_t n ) {
      static_assert( ReqAlign <= alignment, "alignment is too small for this arena" );
      auto const aligned_n = details::align_up<Alignment>( n );
      if ( static_cast<decltype( aligned_n )>( m_buffer + capacity() - m_current ) < aligned_n ) throw std::bad_alloc{};
      return std::exchange( m_current, m_current + aligned_n );
    }

    void deallocate( std::byte* p, std::size_t n ) noexcept {
      assert( is_inside( m_current ) );
      assert( is_inside( p ) );
      if ( p + details::align_up<Alignment>( n ) == m_current ) m_current = p;
    }

    void reset() noexcept { m_current = m_buffer; }
  };

  template <typename T, typename Arena>
  class ArenaAllocator {
  public:
    using value_type                = T;
    static auto constexpr alignment = Arena::alignment;
    using arena_t                   = Arena;

  private:
    arena_t* m_arena;

  public:
    ArenaAllocator( const ArenaAllocator& ) = default;
    ArenaAllocator& operator=( const ArenaAllocator& ) = delete;

    ArenaAllocator( arena_t* a ) : m_arena( a ) {
      if ( a->capacity() % alignment != 0 )
        throw std::runtime_error( "ArenaAllocator: size must be a multiple of alignment" );
    }

    template <typename U>
    ArenaAllocator( const ArenaAllocator<U, arena_t>& a ) noexcept : m_arena{a.m_arena} {}

    template <typename U>
    struct rebind {
      using other = ArenaAllocator<U, arena_t>;
    };

    T* allocate( std::size_t n ) {
      return reinterpret_cast<T*>( m_arena->template allocate<alignof( T )>( n * sizeof( T ) ) );
    }
    void deallocate( T* p, std::size_t n ) noexcept {
      m_arena->deallocate( reinterpret_cast<std::byte*>( p ), n * sizeof( T ) );
    }

    template <typename, typename>
    friend class ArenaAllocator;
  };

  template <typename T, typename Arena1, typename U, typename Arena2>
  inline bool operator==( const ArenaAllocator<T, Arena1>& lhs, const ArenaAllocator<U, Arena2>& rhs ) noexcept {
    return std::is_same_v<Arena1, Arena2> && lhs.m_arena == rhs.m_arena;
  }

  template <typename T, typename Arena1, typename U, typename Arena2>
  inline bool operator!=( const ArenaAllocator<T, Arena1>& lhs, const ArenaAllocator<U, Arena2>& rhs ) noexcept {
    return !( lhs == rhs );
  }

  template <typename T, std::size_t N, std::size_t Align = alignof( std::max_align_t )>
  using StaticArenaAllocator = ArenaAllocator<T, StaticArena<N, Align>>;

  template <typename T, std::size_t Align = alignof( std::max_align_t )>
  using DynamicArenaAllocator = ArenaAllocator<T, DynamicArena<Align>>;

} // namespace LHCb::Allocators
