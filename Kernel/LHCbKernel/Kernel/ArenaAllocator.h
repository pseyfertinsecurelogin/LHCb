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

namespace LHCb::Allocators {

  template <std::size_t N, std::size_t alignment = alignof( std::max_align_t )>
  class StaticArena {
    std::byte* m_ptr = nullptr;
    alignas( alignment ) std::byte m_buf[N];

    static std::size_t align_up( std::size_t n ) noexcept { return ( n + ( alignment - 1 ) ) & ~( alignment - 1 ); }
    bool               is_inside( std::byte* p ) const noexcept { return m_buf <= p && p <= m_buf + size(); }

  public:
    ~StaticArena() { m_ptr = nullptr; }
    StaticArena() noexcept : m_ptr{m_buf} {}
    StaticArena( const StaticArena& ) = delete;
    StaticArena& operator=( const StaticArena& ) = delete;
    StaticArena( StaticArena&& )                 = delete;
    StaticArena& operator=( StaticArena&& ) = delete;

    static constexpr std::size_t size() noexcept { return N; }
    std::size_t                  used() const noexcept { return static_cast<std::size_t>( m_ptr - m_buf ); }

    template <std::size_t ReqAlign>
    std::byte* allocate( std::size_t n ) {
      static_assert( ReqAlign <= alignment, "alignment is too small for this arena" );
      auto const aligned_n = align_up( n );
      if ( static_cast<decltype( aligned_n )>( m_buf + size() - m_ptr ) < aligned_n ) throw std::bad_alloc{};
      return std::exchange( m_ptr, m_ptr + aligned_n );
    }

    void deallocate( std::byte* p, std::size_t n ) noexcept {
      assert( is_inside( m_ptr ) );
      assert( is_inside( p ) );
      if ( p + align_up( n ) == m_ptr ) m_ptr = p;
    }

    void reset() noexcept { m_ptr = m_buf; }
  };

  template <class T, std::size_t N, std::size_t Align = alignof( std::max_align_t )>
  class ArenaAllocator {
  public:
    using value_type                = T;
    static auto constexpr alignment = Align;
    static auto constexpr size      = N;
    using arena_t                   = StaticArena<size, alignment>;

  private:
    arena_t* m_arena;

  public:
    ArenaAllocator( const ArenaAllocator& ) = default;
    ArenaAllocator& operator=( const ArenaAllocator& ) = delete;

    ArenaAllocator( arena_t* a ) noexcept : m_arena( a ) {
      static_assert( size % alignment == 0, "size N needs to be a multiple of alignment Align" );
    }

    template <class U>
    ArenaAllocator( const ArenaAllocator<U, N, alignment>& a ) noexcept : m_arena{a.m_arena} {}
    template <class _Up>
    struct rebind {
      using other = ArenaAllocator<_Up, N, alignment>;
    };

    T* allocate( std::size_t n ) {
      return reinterpret_cast<T*>( m_arena->template allocate<alignof( T )>( n * sizeof( T ) ) );
    }
    void deallocate( T* p, std::size_t n ) noexcept {
      m_arena->deallocate( reinterpret_cast<std::byte*>( p ), n * sizeof( T ) );
    }

    template <class U, std::size_t M, std::size_t A>
    friend class ArenaAllocator;
  };

  template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
  inline bool operator==( const ArenaAllocator<T, N, A1>& x, const ArenaAllocator<U, M, A2>& y ) noexcept {
    return N == M && A1 == A2 && &x.m_arena == &y.m_arena;
  }

  template <class T, std::size_t N, std::size_t A1, class U, std::size_t M, std::size_t A2>
  inline bool operator!=( const ArenaAllocator<T, N, A1>& x, const ArenaAllocator<U, M, A2>& y ) noexcept {
    return !( x == y );
  }

} // namespace LHCb::Allocators
