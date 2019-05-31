/** @file ve.h
 *
 * @author Manuel Schiller <Manuel.Schiller@glasgow.ac.uk>
 * @date 2018-08-08
 *
 * For copyright and license information, see the end of the file.
 */
#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstdint>
#include <initializer_list>
#include <iostream>
#include <tuple>
#include <type_traits>
#include <utility>

#include "compilerhints.h"

/// auto-vectorizable simd vector type
namespace ve {
  /// implementation details
  namespace detail {
    /// which type to use for masks of given size
    template <std::size_t SZ>
    struct _mask_type;
    template <>
    struct _mask_type<1> {
      using type = uint8_t;
    };
    template <>
    struct _mask_type<2> {
      using type = uint16_t;
    };
    template <>
    struct _mask_type<4> {
      using type = uint32_t;
    };
    template <>
    struct _mask_type<8> {
      using type = uint64_t;
    };

    /// array of T, size SZ
    template <typename T, size_t SZ>
    struct arr_type {
      static_assert( SZ <= 1024, "SZ too large" );
      using value_type = T;
      using self_type  = arr_type<T, SZ>;
      using mask_type  = arr_type<typename _mask_type<sizeof( T )>::type, SZ>;
      using size_type  = uint16_t;
      constexpr static size_type size() noexcept { return SZ; }
      T                          m_arr[SZ];
      constexpr T&               operator[]( size_type idx ) noexcept { return m_arr[idx]; }
      constexpr const T&         operator[]( size_type idx ) const noexcept { return m_arr[idx]; }
      constexpr self_type&       operator+=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] += other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator-=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] -= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator*=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] *= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator/=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] /= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator%=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] %= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator>>=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] >>= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator<<=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] <<= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator&=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] &= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator|=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] |= other.m_arr[i];
        return *this;
      }
      constexpr self_type& operator^=( const self_type& other ) noexcept {
        for ( size_type i = 0; SZ != i; ++i ) m_arr[i] ^= other.m_arr[i];
        return *this;
      }

      constexpr friend self_type operator-( const self_type& v ) noexcept {
        self_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -v.m_arr[i];
        return retVal;
      }
      constexpr friend self_type operator~( const self_type& v ) noexcept {
        self_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = ~v.m_arr[i];
        return retVal;
      }

      constexpr friend self_type operator+( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) += w;
      }
      constexpr friend self_type operator-( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) -= w;
      }
      constexpr friend self_type operator*( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) *= w;
      }
      constexpr friend self_type operator/( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) /= w;
      }
      constexpr friend self_type operator%( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) %= w;
      }
      constexpr friend self_type operator<<( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) <<= w;
      }
      constexpr friend self_type operator>>( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) >>= w;
      }
      constexpr friend self_type operator&( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) &= w;
      }
      constexpr friend self_type operator|( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) |= w;
      }
      constexpr friend self_type operator^( const self_type& v, const self_type& w ) noexcept {
        return self_type( v ) ^= w;
      }
      constexpr friend mask_type operator==( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] == w.m_arr[i] );
        return retVal;
      }
      constexpr friend mask_type operator!=( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] != w.m_arr[i] );
        return retVal;
      }

      constexpr friend mask_type operator<( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] < w.m_arr[i] );
        return retVal;
      }
      constexpr friend mask_type operator<=( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] <= w.m_arr[i] );
        return retVal;
      }
      constexpr friend mask_type operator>( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] > w.m_arr[i] );
        return retVal;
      }
      constexpr friend mask_type operator>=( const self_type& v, const self_type& w ) noexcept {
        mask_type retVal;
        for ( size_type i = 0; SZ != i; ++i ) retVal.m_arr[i] = -bool( v.m_arr[i] >= w.m_arr[i] );
        return retVal;
      }
    };

#if defined( __clang__ )
    /// vectorized type (clang)
    template <typename T, size_t SZ>
    struct __vectype;
    template <typename T>
    struct __vectype<T, 1> {
      using type __attribute__( ( ext_vector_type( 1 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 2> {
      using type __attribute__( ( ext_vector_type( 2 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 4> {
      using type __attribute__( ( ext_vector_type( 4 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 8> {
      using type __attribute__( ( ext_vector_type( 8 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 16> {
      using type __attribute__( ( ext_vector_type( 16 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 32> {
      using type __attribute__( ( ext_vector_type( 32 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 64> {
      using type __attribute__( ( ext_vector_type( 64 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 128> {
      using type __attribute__( ( ext_vector_type( 128 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 256> {
      using type __attribute__( ( ext_vector_type( 256 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 512> {
      using type __attribute__( ( ext_vector_type( 512 ) ) ) = T;
    };
    template <typename T>
    struct __vectype<T, 1024> {
      using type __attribute__( ( ext_vector_type( 1024 ) ) ) = T;
    };
#elif defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __INTEL_COMPILER )
    /// vectorized type (GCC)
    template <typename T, size_t SZ>
    struct __vectype {
      typedef T __attribute__( ( vector_size( sizeof( T ) * SZ ) ) ) type;
    };
#else
    /// vectorized type (hope for autovectorisation)
    template <typename T, size_t SZ>
    struct __vectype {
      using type = arr_type<T, SZ>;
    };
#endif

    /// vector type for arr_type
    template <typename T, size_t SZ>
    using vect_type = typename __vectype<T, SZ>::type;
    /// mask vector type for arr_type
    template <typename T, size_t SZ>
    using mask_type = typename __vectype<typename _mask_type<sizeof( T )>::type, SZ>::type;

    /// helper to detect if T is indexable
    template <typename T, typename = decltype( std::declval<const T&>()[0] )>
    static constexpr std::true_type __detect_indexable( int ) noexcept;
    /// helper to detect if T is indexable
    template <typename T>
    static constexpr std::false_type __detect_indexable( long ) noexcept;
    /// is T indexable, i.e. does T's operator[] exist
    template <typename T>
    struct is_indexable : decltype( __detect_indexable<T>( 0 ) ) {};

    /// helper to detect if T is linear
    template <typename T,
              typename = decltype( 2 * std::declval<const T&>() +
                                   std::declval<const T&>() * std::declval<const T&>() / std::declval<const T&>() )>
    static constexpr std::true_type __detect_linear( int ) noexcept;
    /// helper to detect if T is linear
    template <typename T>
    static constexpr std::false_type __detect_linear( long ) noexcept;
    namespace {
      /// helper struct to test function of __detect_linear
      struct empty_for_linear_test {};
    } // namespace
    /// is T linear, i.e. does it support operator+, -, *, /
    template <typename T>
    struct is_linear : decltype( __detect_linear<T>( 0 ) ) {};

    /// is T vector-like
    template <typename T>
    using is_vector_like = std::integral_constant<bool, is_indexable<T>::value && is_linear<T>::value>;

    // check correct implementation
    static_assert( !is_indexable<float>::value, "is_indexable not correct" );
    static_assert( is_indexable<vect_type<float, 4>>::value, "is_indexable not correct" );
    static_assert( !is_linear<empty_for_linear_test>::value, "is_linear not correct" );
    static_assert( is_linear<float>::value, "is_linear not correct" );
    static_assert( is_linear<vect_type<float, 4>>::value, "is_linear not correct" );
    static_assert( !is_vector_like<float>::value, "is_vector_like not correct" );
    static_assert( is_vector_like<vect_type<float, 4>>::value, "is_vector_like not correct" );

#if defined( __clang__ )
    // clang's ext_vector_type attribute takes care of the usual operators
    // for us build uniform interface to vector shuffles
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return __builtin_shufflevector( v, v, ( IDXs & ( SZ - 1 ) )... );
    }
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v, const vect_type<T, SZ>& w ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return __builtin_shufflevector( v, w, ( IDXs & ( 2 * SZ - 1 ) )... );
    }
#elif defined( __GNUC__ ) && !defined( __clang__ ) && !defined( __INTEL_COMPILER )
    // gcc's vector_size attribute takes care of the usual operators for
    // us build uniform interface to vector shuffles
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return __builtin_shuffle( v, mask_type<T, SZ>{( IDXs & ( SZ - 1 ) )...} );
    }
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v, const vect_type<T, SZ>& w ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return __builtin_shuffle( v, w, mask_type<T, SZ>{( IDXs & ( 2 * SZ - 1 ) )...} );
    }
#else
    // if we're not living in clang or gcc land, supply the default
    // assembly-level operators, and hope the compiler is smart enough to
    // autovectorize...
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return {v.m_arr[IDXs & ( SZ - 1 )]...};
    }
    template <typename T, size_t SZ, size_t... IDXs>
    constexpr vect_type<T, SZ> shuffle( const vect_type<T, SZ>& v, const vect_type<T, SZ>& w ) noexcept {
      static_assert( sizeof...( IDXs ) == SZ, "wrong number of indices" );
      return {( ( ( IDXs < SZ ) ? v : w )[IDXs & ( SZ - 1 )] )...};
    }
#endif
  } // namespace detail

  class simdv_base {
  public:
    /// dummy type which is used as a command in simdv's constructor
    struct iota {};
    /// dummy type which is used as a command in simdv's constructor
    struct from_mask {};
  };

  template <typename, size_t>
  class simdv;

  namespace detail {
    template <size_t M, typename T, std::size_t... IDXs>
    void _hadd( T& v, std::index_sequence<IDXs...> /* unused */ ) noexcept {
      v += shuffle<( IDXs ^ ( M & ( T::size() - 1 ) ) )...>( v );
    }
    template <size_t M, typename CMP, typename T, std::size_t... IDXs>
    void __hcmp( const CMP& cmp, T& v, std::index_sequence<IDXs...> /* unused */ ) noexcept {
      const auto vshuf = shuffle<( IDXs ^ ( M & ( T::size() - 1 ) ) )...>( v );
      v                = sel( cmp( v, vshuf ), v, vshuf );
    }
    template <typename CMP, typename T>
    T _hcmp( const CMP& cmp, const T& _v ) noexcept {
      T v( _v );
      using SEQ = decltype( std::make_index_sequence<T::size()>() );
      switch ( T::size() ) {
      case 1024:
        __hcmp<512>( cmp, v, SEQ() ); // fallthrough
      case 512:
        __hcmp<256>( cmp, v, SEQ() ); // fallthrough
      case 256:
        __hcmp<128>( cmp, v, SEQ() ); // fallthrough
      case 128:
        __hcmp<64>( cmp, v, SEQ() ); // fallthrough
      case 64:
        __hcmp<32>( cmp, v, SEQ() ); // fallthrough
      case 32:
        __hcmp<16>( cmp, v, SEQ() ); // fallthrough
      case 16:
        __hcmp<8>( cmp, v, SEQ() ); // fallthrough
      case 8:
        __hcmp<4>( cmp, v, SEQ() ); // fallthrough
      case 4:
        __hcmp<2>( cmp, v, SEQ() ); // fallthrough
      case 2:
        __hcmp<1>( cmp, v, SEQ() ); // fallthrough
      default:
        break;
      }
      return v;
    }
    template <std::size_t NROT, std::size_t N, typename T, std::size_t... IDXs>
    constexpr simdv<T, N> hrotate( const simdv<T, N>& v, std::index_sequence<IDXs...> ) noexcept {
      return shuffle<( ( IDXs + NROT ) & ( N - 1 ) )...>( v );
    }

    /// detect if T is a vector type
    template <typename T, typename = void>
    struct is_vector : std::false_type {};
    /// detect if T is a vector type (specialisation)
    template <typename T>
    struct is_vector<T, std::void_t<typename T::vector_tag>> : std::true_type {};
    /// detect if T is vector type
    template <typename T, typename RET = void, bool EXTRACOND = true>
    using enable_if_vector_t = typename std::enable_if<is_vector<T>::value && EXTRACOND, RET>::type;
    /// detect if T is proxy type
    template <typename T, typename = void>
    struct is_proxy : std::false_type {};
    /// detect if T is proxy type (specialisation)
    template <typename T>
    struct is_proxy<T, std::void_t<typename T::proxy_tag>> : std::true_type {};
    /// detect if T is proxy type
    template <typename T, typename RET = void>
    using enable_if_proxy_t = typename std::enable_if<is_proxy<T>::value, RET>::type;

    namespace {
      // implement just enough of the interface to test
      // is_vector(_like) and is_proxy
      struct dummy_vector {
        using vector_tag = void;
        using value_type = int;
        struct dummy_proxy {
          using proxy_tag  = void;
          using value_type = int;
        };
        dummy_proxy inline         operator[]( int ) const noexcept;
        friend inline dummy_vector operator+( const dummy_vector&, const dummy_vector& ) noexcept;
        friend inline dummy_vector operator-( const dummy_vector&, const dummy_vector& ) noexcept;
        friend inline dummy_vector operator*(const int, const dummy_vector&)noexcept;
        friend inline dummy_vector operator*(const dummy_vector&, const dummy_vector&)noexcept;
        friend inline dummy_vector operator/( const dummy_vector&, const dummy_vector& ) noexcept;
      };
    } // namespace
    static_assert( is_vector<dummy_vector>::value, "incorrect implementation of is_vector" );
    static_assert( !is_vector<int>::value, "incorrect implementation of is_vector" );
    static_assert( is_proxy<typename dummy_vector::dummy_proxy>::value, "incorrect implementation of is_proxy" );
    static_assert( !is_proxy<int>::value, "incorrect implementation of is_proxy" );
    static_assert( is_vector_like<dummy_vector>::value, "incorrect implementation of is_vector_like" );

    /// check if M is a mask-like vector type
    template <typename M>
    struct is_mask
        : std::integral_constant<bool, is_vector<M>::value && std::is_integral<typename M::value_type>::value> {};
    /// enable overload if M is a mask-like vector type
    template <typename M, typename RET = void>
    using enable_if_mask_t = typename std::enable_if<is_mask<M>::value, RET>::type;

    /// extract the value_type from vectors and scalars alike
    template <typename T,
              unsigned FLAGS = 4 * is_proxy<T>::value + 2 * is_vector<T>::value + 1 * is_vector_like<T>::value>
    struct value_type;
    /// extract value_type from vectors and scalars alike (scalar)
    template <typename T>
    struct value_type<T, 0> {
      using type = T;
    };
    /// extract value_type from vectors and scalars alike (vector extension)
    template <typename T>
    struct value_type<T, 1> {
      using type = decltype( 0 + std::declval<const T&>()[0] );
    };
    /// extract value_type from vectors and scalars alike (vector)
    template <typename T>
    struct value_type<T, 2> {
      using type = typename T::value_type;
    };
    /// extract value_type from vectors and scalars alike (vector)
    template <typename T>
    struct value_type<T, 3> {
      using type = typename T::value_type;
    };
    /// extract value_type from vectors and scalars alike (vector's proxy)
    template <typename T>
    struct value_type<T, 4> {
      using type = typename T::value_type;
    };
    /// extract the value_type from vectors and scalars alike
    template <typename T>
    using value_type_t = typename value_type<T>::type;

    static_assert( std::is_same<value_type_t<int>, int>::value, "incorrect implementation of value_type_t" );
    static_assert( std::is_same<value_type_t<dummy_vector>, int>::value, "incorrect implementation of value_type_t" );
    static_assert( std::is_same<value_type_t<typename dummy_vector::dummy_proxy>, int>::value,
                   "incorrect implementation of value_type_t" );

    /// version of std::make_unsigned: transparent on non-intergal types
    template <typename T, bool = std::is_integral<T>::value>
    struct make_unsigned {
      using type = T;
    };
    template <typename T>
    struct make_unsigned<T, true> : std::make_unsigned<T> {};

    /// given two types, check that the first is the common type of both
    template <typename S, typename T, bool BOTH_ARITH = std::is_arithmetic<S>::value&& std::is_arithmetic<T>::value>
    struct is_first_common_type : std::false_type {};
    /// given two types, check that the first is the common type of both
    template <typename S, typename T>
    struct is_first_common_type<S, T, true> : std::is_same<S, typename std::common_type<S, T>::type> {};

    /// given two types (vectors or scalars), return the common vector type
    template <typename S, typename T, typename SV = value_type_t<S>, typename TV = value_type_t<T>>
    using common_vector_type_t = typename std::conditional<
        is_vector<S>::value,
        typename std::conditional<is_vector<T>::value,
                                  typename std::conditional<is_first_common_type<SV, TV>::value, S, T>::type, S>::type,
        typename std::conditional<is_vector<T>::value, T, void>::type>::type;

    /// return v's value as a mask (for vectors)
    template <typename T>
    constexpr typename std::enable_if<is_vector<T>::value, typename T::mask_type>::type as_mask( const T& v ) noexcept {
      return v.as_mask();
    }
    /// return v's value as a mask (for scalars)
    template <typename T>
    constexpr typename std::enable_if<std::is_scalar<T>::value && std::is_integral<T>::value, T>::type
    as_mask( const T& v ) noexcept {
      return v;
    }

  } // namespace detail

  template <typename T, size_t N>
  class alignas( sizeof( T ) * N ) simdv : private simdv_base {
  private:
    static_assert( N > 0, "N must be positive" );
    static_assert( 0 == ( N & ( N - 1 ) ), "N must be a power of 2" );
    static_assert( std::is_arithmetic<T>::value, "T must be an arithmetic type" );

    template <size_t SZ>
    using _mask_type = detail::_mask_type<SZ>;
    using _V         = detail::vect_type<T, N>;
    using _M         = detail::mask_type<T, N>;
    template <typename S, size_t SZ>
    using __M = detail::mask_type<S, SZ>;
    using _A  = detail::arr_type<T, N>;
    union _U {
      _A _a;
      _V _v;
      _M _m;
      constexpr _U() noexcept : _a() {}
      ~_U() noexcept                     = default;
      constexpr _U( const _U& ) noexcept = default;
      constexpr _U( _U&& ) noexcept      = default;
      constexpr _U& operator=( const _U& ) noexcept = default;
      constexpr _U& operator=( _U&& ) noexcept = default;

      constexpr _U( const _V& v ) noexcept : _v( v ) {}
      constexpr _U( _V&& v ) noexcept : _v( std::move( v ) ) {}

      template <typename MM = _M,
                typename = typename std::enable_if<std::is_same<MM, _M>::value && !std::is_same<_M, _V>::value>::type>
      constexpr _U( const MM& m ) noexcept : _m( m ) {}
      template <typename MM = _M,
                typename = typename std::enable_if<std::is_same<MM, _M>::value && !std::is_same<_M, _V>::value>::type>
      constexpr _U( MM&& m ) noexcept : _m( std::move( m ) ) {}

      template <typename AA = _A,
                typename std::enable_if<std::is_same<AA, _A>::value && !std::is_same<_V, _A>::value>::type>
      constexpr _U( const AA& a ) noexcept : _a( a ) {}
      template <typename AA = _A,
                typename std::enable_if<std::is_same<AA, _A>::value && !std::is_same<_V, _A>::value>::type>
      constexpr _U( AA&& a ) noexcept : _a( std::move( a ) ) {}
    };
    _U m_arr;

    template <typename, size_t>
    friend class ::ve::simdv;

  public:
    // allow construction from underlying vector extension types, but
    // force explicit construction
    template <
        typename V,
        typename = typename std::enable_if<
            detail::is_vector_like<V>::value && !detail::is_vector<V>::value && std::is_convertible<V, _V>::value &&
            ( !std::is_same<V, _M>::value || ( std::is_same<V, _M>::value && std::is_same<_V, _M>::value ) )>::type>
    explicit constexpr simdv( const V& v ) noexcept : m_arr( _V( v ) ) {}
    template <
        typename V,
        typename = typename std::enable_if<
            detail::is_vector_like<V>::value && !detail::is_vector<V>::value && std::is_convertible<V, _V>::value &&
            ( !std::is_same<V, _M>::value || ( std::is_same<V, _M>::value && std::is_same<_V, _M>::value ) )>::type>
    explicit constexpr simdv( V&& v ) noexcept : m_arr( _V( std::move( v ) ) ) {}
    template <typename M, typename = M,
              typename = typename std::enable_if<detail::is_vector_like<M>::value && !detail::is_vector<M>::value &&
                                                 std::is_same<M, _M>::value && !std::is_same<_V, _M>::value>::type>
    explicit constexpr simdv( const M& m ) noexcept : m_arr( m ) {}
    template <typename M, typename = M,
              typename = typename std::enable_if<detail::is_vector_like<M>::value && !detail::is_vector<M>::value &&
                                                 std::is_same<M, _M>::value && !std::is_same<_V, _M>::value>::type>
    explicit constexpr simdv( M&& m ) noexcept : m_arr( std::move( m ) ) {}

    // typedefs
    using self_type = simdv;
    using mask_type = simdv<typename _mask_type<sizeof( T )>::type, N>;
    static_assert( sizeof( typename _mask_type<sizeof( T )>::type ) == sizeof( T ), "Incorrect mask type" );

    using value_type             = T;
    using reference              = T&;
    using const_reference        = const T&;
    using pointer                = T*;
    using const_pointer          = const T*;
    using size_type              = uint16_t;
    using difference_type        = int16_t;
    using iterator               = pointer;
    using const_iterator         = const_pointer;
    using reverse_iterator       = std::reverse_iterator<iterator>;
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    using vector_tag = void;
    using iota       = simdv_base::iota;
    using from_mask  = simdv_base::from_mask;

  private:
    template <typename S, typename TYPE = mask_type, typename U = S>
    using enable_if_mask_t = typename std::enable_if<std::is_scalar<S>::value && sizeof( value_type ) == sizeof( S ) &&
                                                         std::is_integral<S>::value && std::is_integral<U>::value,
                                                     TYPE>::type;

    template <typename U, typename V, typename RETVAL = U,
              bool                               ALLARITHMETIC =
                  std::is_arithmetic<U>::value&& std::is_arithmetic<V>::value&& std::is_arithmetic<RETVAL>::value,
              bool NONEVECTOR = !detail::is_vector<RETVAL>::value && !detail::is_vector<U>::value &&
                                !detail::is_vector<V>::value && !detail::is_vector_like<RETVAL>::value &&
                                !detail::is_vector_like<U>::value && !detail::is_vector_like<V>::value>
    struct _is_non_narrowing : std::false_type {};
    template <typename U, typename V, typename RETVAL>
    struct _is_non_narrowing<U, V, RETVAL, true, true>
        : std::integral_constant<bool,
                                 std::is_same<RETVAL, typename std::common_type<U, V>::type>::value ||
                                     std::is_same<RETVAL, typename detail::make_unsigned<
                                                              typename std::common_type<U, V>::type>::type>::value> {};

    template <typename U, typename V, typename RETVAL = U>
    using is_non_narrowing = _is_non_narrowing<U, V, RETVAL>;
    template <typename U, typename V, typename RETVAL = U>
    using is_narrowing = std::integral_constant<bool, !_is_non_narrowing<U, V, RETVAL>::value>;

    /** @brief operator +, -, *, / need suitable constraints
     *
     * This is the standard SFINAE constraint for operator+ and friends.
     * The basic requirements are:
     * - at least one of U and V must be a vector
     * - neither is a proxy
     * - at least one of U and V must derive from self_type
     * - the underlying types (value_type for vectors, the type itself for
     *   scalar types) must support arithmetic
     * - if U and V are both vectors, U must derive from self_type (to
     *   break ties where the compiler cannot decide which operator to
     *   call)
     * - scalar arguments are not narrowed
     */
    template <typename U, typename V, typename RETVAL = detail::common_vector_type_t<U, V>,
              typename UV = detail::value_type_t<U>, typename VV = detail::value_type_t<V>,
              bool EXTRACOND = is_non_narrowing<UV, VV, typename RETVAL::value_type>::value>
    using enable_if_non_narrowing2_t = typename std::enable_if<
        // at least one of the arguments is a vector
        ( detail::is_vector<U>::value || detail::is_vector<V>::value ) &&
            // neither is a proxy
            ( !detail::is_proxy<U>::value && !detail::is_proxy<V>::value ) &&
            // at least one of the arguments is derived from
            // self_type
            ( std::is_base_of<self_type, U>::value || std::is_base_of<self_type, V>::value ) &&
            // they value_types must be arithmetic ones
            std::is_arithmetic<UV>::value && std::is_arithmetic<VV>::value &&
            // if we have two different vector arguments, break
            // the tie
            ( ( detail::is_vector<U>::value + detail::is_vector<V>::value ) == 1 ||
              ( ( detail::is_vector<U>::value + detail::is_vector<V>::value ) == 2 &&
                std::is_base_of<self_type, U>::value ) ) &&
            // and whatever extra conditions are appropriate
            EXTRACOND,
        RETVAL>::type;

    /** @brief operator %, >>, << need suitable constraints
     *
     * This is the standard SFINAE constraint for operator% and friends.
     * The basic requirements are:
     * - the defaults from enable_if_non_narrowing2_t above
     * - the underlying value_types are both integral
     * - scalar arguments are not narrowed
     */
    template <typename U, typename V, typename RETVAL = detail::common_vector_type_t<U, V>,
              typename UV = detail::value_type_t<U>, typename VV = detail::value_type_t<V>>
    using enable_if_integral2_t =
        enable_if_non_narrowing2_t<U, V, RETVAL, UV, VV,
                                   std::is_integral<UV>::value && std::is_integral<VV>::value &&
                                       is_non_narrowing<UV, VV, typename RETVAL::value_type>::value>;

    /** @brief operator &, |, ^ need suitable constraints
     *
     * This is the standard SFINAE constraint for operator& and friends.
     * The basic requirements are:
     * - the defaults from enable_if_non_narrowing2_t above
     * - at least one of the underlying value_types is integral
     * - scalar arguments are not narrowed
     */
    template <typename U, typename V, typename RETVAL = typename detail::common_vector_type_t<U, V>::mask_type,
              typename UV = detail::value_type_t<U>, typename VV = detail::value_type_t<V>>
    using enable_if_mask2_t =
        enable_if_non_narrowing2_t<U, V, RETVAL, UV, VV,
                                   ( std::is_integral<UV>::value || std::is_integral<VV>::value ) &&
                                       sizeof( UV ) <= sizeof( typename RETVAL::value_type ) &&
                                       sizeof( VV ) <= sizeof( typename RETVAL::value_type )>;

    /** @brief operator ==, !=, <, <=, >, >= need suitable constraints
     *
     * This is the standard SFINAE constraint for operator== and friends.
     * The basic requirements are:
     * - the defaults from enable_if_non_narrowing2_t above
     * - scalar arguments are not narrowed
     */
    template <typename U, typename V, typename RETVAL = typename detail::common_vector_type_t<U, V>::mask_type,
              typename UV = detail::value_type_t<U>, typename VV = detail::value_type_t<V>>
    using enable_if_mask3_t = enable_if_non_narrowing2_t<U, V, RETVAL, UV, VV,
                                                         sizeof( UV ) <= sizeof( typename RETVAL::value_type ) &&
                                                             sizeof( VV ) <= sizeof( typename RETVAL::value_type )>;

    /// enable overload if S is a vector and derived from self_type
    template <typename S, typename RETVAL = S, bool EXTRACOND = true>
    using enable_if_derived_t =
        typename std::enable_if<detail::is_vector<S>::value && std::is_base_of<self_type, S>::value && EXTRACOND,
                                RETVAL>::type;

  public:
    constexpr simdv() noexcept                   = default;
    constexpr simdv( const self_type& ) noexcept = default;
    constexpr simdv( self_type&& ) noexcept      = default;

    /// constuct from mask_type
    constexpr simdv( from_mask /* unused */, const mask_type& m ) noexcept : simdv( m.m_arr._m ) {}

    constexpr mask_type as_mask() const noexcept { return mask_type( m_arr._m ); }

    /// initialise with incrementing values starting with val
    template <typename VAL>
    constexpr simdv( iota /* unused */, VAL val ) noexcept {
      for ( size_type i = 0; i != N; ++i, ++val ) m_arr._a[i] = val;
    }

    /// from scalar (non-narrowing)
    template <
        typename S, typename = S, typename = S,
        typename = typename std::enable_if<!detail::is_vector_like<S>::value && !detail::is_vector<S>::value &&
                                           ( !std::is_integral<value_type>::value || !std::is_same<S, bool>::value ) &&
                                           is_non_narrowing<value_type, S>::value>::type>
    constexpr simdv( const S& val ) noexcept {
      for ( size_type i = 0; N != i; ++i ) m_arr._a[i] = value_type( val );
    }
    /// from (scalar) bool for integer types (sets all bits)
    template <
        typename S, typename = S, typename = S, typename = S,
        typename = typename std::enable_if<std::is_same<bool, S>::value && std::is_integral<value_type>::value>::type>
    constexpr simdv( const S& val ) noexcept {
      for ( size_type i = 0; N != i; ++i ) m_arr._a[i] = -value_type( val );
    }
    /// from scalar (narrowing, requires explicit construction)
    template <
        typename S, typename = S, typename = S, typename = S, typename = S,
        typename = typename std::enable_if<!detail::is_vector_like<S>::value && !detail::is_vector<S>::value &&
                                           ( !std::is_integral<value_type>::value || !std::is_same<S, bool>::value ) &&
                                           is_narrowing<value_type, S>::value>::type>
    explicit constexpr simdv( const S& val ) noexcept {
      for ( size_type i = 0; N != i; ++i ) m_arr._a[i] = value_type( val );
    }
    /// from pair of iterators
    template <typename IT>
    constexpr simdv( IT first, IT last, const T& val = T() ) noexcept : simdv( val ) {
      const size_type jend = ( std::size_t( last - first ) < N ) ? size_type( last - first ) : N;
      for ( size_type j = 0; jend != j; ++j ) m_arr._a[j] = *first++;
    }

    /// from initializer_list (only if non-narrowing)
    template <typename S, typename = typename std::enable_if<!detail::is_vector_like<S>::value &&
                                                             std::is_convertible<S, value_type>::value>::type>
    explicit constexpr simdv( std::initializer_list<S> ilist, const T& val = T() ) noexcept
        : simdv( ilist.begin(), ilist.end(), val ) {}

    /// from other vector (non-narrowing)
    template <typename V, typename = V, typename = V, typename = V, typename = V, typename = V,
              typename = typename std::enable_if<
                  detail::is_vector<V>::value && std::is_convertible<typename V::value_type, value_type>::value &&
                  N == V::size() && is_non_narrowing<value_type, typename V::value_type>::value>::type>
    constexpr simdv( const V& v ) noexcept {
      std::size_t i{0};
      for ( auto& x : v ) {
        m_arr._a[i] = value_type( x );
        ++i;
      }
    }
    /// from other vector (potentially narrowing, therefore explicit)
    template <typename V, typename = V, typename = V, typename = V, typename = V, typename = V, typename = V,
              typename = typename std::enable_if<
                  detail::is_vector<V>::value && std::is_convertible<typename V::value_type, value_type>::value &&
                  N == V::size() && !is_non_narrowing<value_type, typename V::value_type>::value>::type>
    explicit constexpr simdv( const V& v ) noexcept {
      std::size_t i{0};
      for ( auto& x : v ) {
        m_arr._a[i] = value_type( x );
        ++i;
      }
    }

    ~simdv() noexcept = default;

    self_type& operator=( const self_type& ) noexcept = default;
    self_type& operator=( self_type&& ) noexcept = default;
    self_type& operator                          =( const value_type v ) noexcept {
      *this = self_type( v );
      return *this;
    }

    static constexpr size_type size() noexcept { return N; }
    static constexpr size_type max_size() noexcept { return N; }
    static constexpr size_type capacity() noexcept { return N; }
    static constexpr bool      empty() noexcept { return false; }

  private:
    /// base class for proxies (make sure we can do everything trivially)
    struct proxy_base {
      _U&             m_arr;
      const size_type m_idx;
    };

  public:
    /// clang does not like references to vector elements - proxy them
    class const_proxy : protected proxy_base {
    protected:
      constexpr const_proxy& operator=( const const_proxy& ) noexcept = delete;
      constexpr const_proxy& operator=( const_proxy&& ) noexcept = delete;
      using proxy_base::m_arr;
      using proxy_base::m_idx;

    public:
      using proxy_tag  = void;
      using value_type = typename self_type::value_type;
      explicit constexpr const_proxy( _U& arr, size_type idx ) noexcept : proxy_base{arr, idx} {}
      constexpr const_proxy( const const_proxy& ) noexcept = default;
      constexpr const_proxy( const_proxy&& ) noexcept      = default;

      constexpr                      operator value_type() const noexcept { return m_arr._v[m_idx]; }
      friend constexpr const_pointer operator&( const const_proxy& p ) noexcept { return &p.m_arr._a[p.m_idx]; }
    };
    static_assert( detail::is_proxy<const_proxy>::value, "const_proxy must be proxy" );

    constexpr const_proxy operator[]( size_type idx ) const noexcept {
      return const_proxy{const_cast<_U&>( m_arr ), idx};
    }

    /// clang does not like references to vector elements - proxy them
    class proxy : public const_proxy {
    private:
      using const_proxy::m_arr;
      using const_proxy::m_idx;

    public:
      constexpr explicit proxy( _U& arr, size_type idx ) noexcept : const_proxy( arr, idx ) {}
      constexpr proxy( const proxy& ) noexcept = default;
      constexpr proxy( proxy&& ) noexcept      = default;

      constexpr proxy& operator=( const value_type& val ) noexcept {
        m_arr._v[m_idx] = val;
        return *this;
      }
      constexpr proxy& operator=( value_type&& val ) noexcept {
        m_arr._v[m_idx] = std::move( val );
        return *this;
      }
      constexpr proxy& operator++() noexcept {
        ++m_arr._v[m_idx];
        return *this;
      }
      constexpr proxy& operator--() noexcept {
        --m_arr._v[m_idx];
        return *this;
      }
      constexpr value_type operator++( int ) noexcept {
        value_type retVal( *this );
        ++m_arr._v[m_idx];
        return retVal;
      }
      constexpr value_type operator--( int ) noexcept {
        value_type retVal( *this );
        --m_arr._v[m_idx];
        return retVal;
      }
      constexpr proxy& operator+=( const value_type& val ) noexcept {
        m_arr._v[m_idx] += val;
        return *this;
      }
      constexpr proxy& operator-=( const value_type& val ) noexcept {
        m_arr._v[m_idx] -= val;
        return *this;
      }
      constexpr proxy& operator*=( const value_type& val ) noexcept {
        m_arr._v[m_idx] *= val;
        return *this;
      }
      constexpr proxy& operator/=( const value_type& val ) noexcept {
        m_arr._v[m_idx] /= val;
        return *this;
      }
      template <typename S>
      constexpr enable_if_mask_t<S, proxy, value_type>& operator%=( const S& val ) noexcept {
        m_arr._v[m_idx] %= val;
        return *this;
      }
      template <typename S>
      constexpr enable_if_mask_t<S, proxy, value_type>& operator>>=( const S& val ) noexcept {
        m_arr._m[m_idx] >>= val;
        return *this;
      }
      template <typename S>
      constexpr enable_if_mask_t<S, proxy, value_type>& operator<<=( const S& val ) noexcept {
        m_arr._m[m_idx] <<= val;
        return *this;
      }
      template <typename S>
      constexpr enable_if_mask_t<S, typename mask_type::proxy>& operator&=( const S& val ) noexcept {
        m_arr._m[m_idx] &= typename mask_type::value_type( val );
        return reinterpret_cast<typename mask_type::proxy&>( *this );
      }
      template <typename S>
      constexpr enable_if_mask_t<S, typename mask_type::proxy>& operator|=( const S& val ) noexcept {
        m_arr._m[m_idx] |= typename mask_type::value_type( val );
        return reinterpret_cast<typename mask_type::proxy&>( *this );
      }
      template <typename S>
      constexpr enable_if_mask_t<S, typename mask_type::proxy>& operator^=( const S& val ) noexcept {
        m_arr._m[m_idx] ^= typename mask_type::value_type( val );
        return reinterpret_cast<typename mask_type::proxy&>( *this );
      }
      friend constexpr pointer operator&( const proxy& p ) noexcept { return &p.m_arr._a[p.m_idx]; }
    };
    static_assert( detail::is_proxy<proxy>::value, "proxy must be proxy" );

    constexpr proxy operator[]( size_type idx ) noexcept { return proxy{m_arr, idx}; }

    proxy at( size_type idx ) {
      if ( idx >= N ) throw std::out_of_range( __func__ );
      return operator[]( idx );
    }
    const_proxy at( size_type idx ) const {
      if ( idx >= N ) throw std::out_of_range( __func__ );
      return operator[]( idx );
    }

    constexpr proxy       front() noexcept { return operator[]( 0 ); }
    constexpr const_proxy front() const noexcept { return operator[]( 0 ); }
    constexpr proxy       back() noexcept { return operator[]( N - 1 ); }
    constexpr const_proxy back() const noexcept { return operator[]( N - 1 ); }

    constexpr pointer       data() noexcept { return &front(); }
    constexpr const_pointer data() const noexcept { return &front(); }

    constexpr iterator       begin() noexcept { return &front(); }
    constexpr iterator       end() noexcept { return ( &back() ) + 1; }
    constexpr const_iterator begin() const noexcept { return &front(); }
    constexpr const_iterator end() const noexcept { return ( &back() ) + 1; }
    constexpr const_iterator cbegin() const noexcept { return &front(); }
    constexpr const_iterator cend() const noexcept { return ( &back() ) + 1; }

    constexpr reverse_iterator       rbegin() noexcept { return reverse_iterator{end()}; }
    constexpr reverse_iterator       rend() noexcept { return reverse_iterator{begin()}; }
    constexpr const_reverse_iterator rbegin() const noexcept { return reverse_iterator{end()}; }
    constexpr const reverse_iterator rend() const noexcept { return reverse_iterator{begin()}; }
    constexpr const_reverse_iterator crbegin() const noexcept { return reverse_iterator{end()}; }
    constexpr const reverse_iterator crend() const noexcept { return reverse_iterator{begin()}; }

    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V, U>& operator+=( U& u, const V& v ) noexcept {
      u.m_arr._v += U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V, U>& operator-=( U& u, const V& v ) noexcept {
      u.m_arr._v -= U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V, U>& operator*=( U& u, const V& v ) noexcept {
      u.m_arr._v *= U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V, U>& operator/=( U& u, const V& v ) noexcept {
      u.m_arr._v /= U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V, U>& operator%=( U& u, const V& v ) noexcept {
      u.m_arr._v %= U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V, U>& operator>>=( U& u, const V& v ) noexcept {
      u.m_arr._v >>= U( v ).m_arr._v;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V, U>& operator<<=( U& u, const V& v ) noexcept {
      u.m_arr._v <<= U( v ).m_arr._v;
      return u;
    }

    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V, U>& operator&=( U& u, const V& v ) noexcept {
      u.m_arr._m &= U( from_mask(), detail::as_mask( v ) ).m_arr._m;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V, U>& operator|=( U& u, const V& v ) noexcept {
      u.m_arr._m |= U( from_mask(), detail::as_mask( v ) ).m_arr._m;
      return u;
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V, U>& operator^=( U& u, const V& v ) noexcept {
      u.m_arr._m ^= U( from_mask(), detail::as_mask( v ) ).m_arr._m;
      return u;
    }

    template <typename V>
    friend constexpr enable_if_derived_t<V>& operator++( V& v ) noexcept {
      v += T( 1 );
      return v;
    }
    template <typename V>
    friend constexpr enable_if_derived_t<V> operator++( V& v, int ) noexcept {
      auto retVal( v );
      v += T( 1 );
      return retVal;
    }
    template <typename V>
    friend constexpr enable_if_derived_t<V>& operator--( V& v ) noexcept {
      v -= T( 1 );
      return v;
    }
    template <typename V>
    friend constexpr enable_if_derived_t<V> operator--( V& v, int ) noexcept {
      auto retVal( v );
      v -= T( 1 );
      return retVal;
    }

    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V> operator+( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_non_narrowing2_t<U, V>;
      ret_type retVal( u );
      retVal += ret_type( v );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V> operator-( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_non_narrowing2_t<U, V>;
      ret_type retVal( u );
      retVal -= ret_type( v );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V> operator*( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_non_narrowing2_t<U, V>;
      ret_type retVal( u );
      retVal *= ret_type( v );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_non_narrowing2_t<U, V> operator/( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_non_narrowing2_t<U, V>;
      ret_type retVal( u );
      retVal /= ret_type( v );
      return retVal;
    }

    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V> operator%( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_integral2_t<U, V>;
      ret_type retVal( u );
      retVal %= ret_type( v );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V> operator>>( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_integral2_t<U, V>;
      ret_type retVal( u );
      retVal >>= ret_type( v );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_integral2_t<U, V> operator<<( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_integral2_t<U, V>;
      ret_type retVal( u );
      retVal <<= ret_type( v );
      return retVal;
    }

    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V> operator&( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_mask2_t<U, V>;
      ret_type retVal( from_mask(), detail::as_mask( u ) );
      retVal &= ret_type( from_mask(), detail::as_mask( v ) );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V> operator|( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_mask2_t<U, V>;
      ret_type retVal( from_mask(), detail::as_mask( u ) );
      retVal |= ret_type( from_mask(), detail::as_mask( v ) );
      return retVal;
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V> operator^( const U& u, const V& v ) noexcept {
      using ret_type = enable_if_mask2_t<U, V>;
      ret_type retVal( from_mask(), detail::as_mask( u ) );
      retVal ^= ret_type( from_mask(), detail::as_mask( v ) );
      return retVal;
    }

    template <std::size_t... IDXs>
    constexpr self_type shuffle() const noexcept {
      static_assert( sizeof...( IDXs ) == N, "wrong number of indices" );
      return self_type( detail::shuffle<T, N, IDXs...>( m_arr._v ) );
    }

    template <std::size_t... IDXs>
    constexpr self_type shuffle( const self_type& w ) const noexcept {
      static_assert( sizeof...( IDXs ) == N, "wrong number of indices" );
      return self_type( detail::shuffle<T, N, IDXs...>( m_arr._v, w.m_arr._v ) );
    }

    template <typename S>
    friend constexpr enable_if_derived_t<S> operator-( const S& v ) noexcept {
      return S( -v.m_arr._v );
    }
    template <typename S>
    friend constexpr enable_if_derived_t<S, typename S::mask_type> operator~( const S& v ) noexcept {
      return typename S::mask_type( ~v.m_arr._m );
    }

    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator==( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v == type( v ).m_arr._v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator!=( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v != type( v ).m_arr._v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator<( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v < type( v ).m_arr._v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator<=( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v <= type( v ).m_arr._v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator>( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v > type( v ).m_arr._v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask3_t<U, V> operator>=( const U& u, const V& v ) noexcept {
      using type          = detail::common_vector_type_t<U, V>;
      using mask          = typename type::mask_type;
      using internal_mask = __M<typename mask::value_type, mask::size()>;
      return mask( reinterpret_cast<internal_mask>( type( u ).m_arr._v >= type( v ).m_arr._v ) );
    }

    template <typename S>
    friend constexpr enable_if_derived_t<S, typename S::mask_type> operator!( const S& v ) noexcept {
      return 0 == v;
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V> operator&&( const U& u, const V& v ) noexcept {
      return 0 != ( detail::as_mask( u ) & detail::as_mask( v ) );
    }
    template <typename U, typename V>
    friend constexpr enable_if_mask2_t<U, V> operator||( const U& u, const V& v ) noexcept {
      return 0 != ( detail::as_mask( u ) | detail::as_mask( v ) );
    }

    template <typename M>
    static constexpr
        typename std::enable_if<std::is_integral<M>::value && sizeof( M ) == sizeof( uint32_t ), self_type>::type
        gather( const T* base, const simdv<M, N>& offsets ) noexcept {
      simdv<T, N> retVal;
      for ( unsigned i = 0; N != i; ++i ) retVal[i] = base[int32_t( offsets[i] )];
      return retVal;
    }

    template <typename M>
    typename std::enable_if<std::is_integral<M>::value && sizeof( M ) == sizeof( uint32_t )>::type
    scatter( T* base, const simdv<M, N>& offsets ) noexcept {
      for ( unsigned i = 0; N != i; ++i ) base[int32_t( offsets[i] )] = operator[]( i );
    }
  };

  template <size_t... IDXS, typename T>
  constexpr detail::enable_if_vector_t<T, T> shuffle( const T& v ) noexcept {
    return v.template shuffle<IDXS...>();
  }
  template <size_t... IDXS, typename T>
  constexpr detail::enable_if_vector_t<T, T> shuffle( const T& v, const T& w ) noexcept {
    return v.template shuffle<IDXS...>( w );
  }

  /// return true if all elements of v are nonzero
  template <typename V, typename = typename detail::enable_if_vector_t<V>>
  constexpr bool all_of( const V& v ) noexcept {
    return -hadd( v != 0 ) == V::size();
  }
  /// return true if any elements of v are nonzero
  template <typename V, typename = typename detail::enable_if_vector_t<V>>
  constexpr bool any_of( const V& v ) noexcept {
    return -hadd( v != 0 ) > 0;
  }
  /// return true if none of the elements of v are nonzero
  template <typename V, typename = typename detail::enable_if_vector_t<V>>
  constexpr bool none_of( const V& v ) noexcept {
    return -hadd( v != 0 ) == 0;
  }

  /// print vector to ostream
  template <typename T>
  detail::enable_if_vector_t<T, std::ostream>& operator<<( std::ostream& os, const T& v ) {
    for ( const auto& el : v ) os << ( ( &el == &v.front() ) ? '(' : ',' ) << el;
    return os << ')';
  }

  template <typename T>
  constexpr typename std::enable_if<!detail::is_vector<T>::value && std::is_pod<T>::value, T>::type
  sel( bool cond, const T& alt1, const T& alt2 ) noexcept {
    using VecT = simdv<T, 1>;
    using VecM = typename VecT::mask_type;
    return sel( VecM( 0 ) != cond, VecT( alt1 ), VecT( alt2 ) )[0];
  }
  template <typename M, typename T>
  constexpr typename std::enable_if<detail::is_mask<M>::value && detail::is_vector<T>::value &&
                                        ( sizeof( typename M::value_type ) == sizeof( typename T::value_type ) ) &&
                                        ( M::size() == T::size() ),
                                    T>::type
  sel( const M& cond, const T& alt1, const T& alt2 ) noexcept {
    return T( typename T::from_mask(), ( cond & alt1 ) | ( ~cond & alt2 ) );
  }
  template <typename M, typename T, typename S>
  constexpr typename std::enable_if<
      detail::is_mask<M>::value && detail::is_vector<S>::value && !detail::is_vector<T>::value &&
          ( sizeof( typename M::value_type ) == sizeof( typename S::value_type ) ) && ( M::size() == S::size() ) &&
          std::is_same<typename std::common_type<typename S::value_type, T>::type, typename S::value_type>::value,
      S>::type
  sel( const M& cond, const S& alt1, const T& alt2 ) noexcept {
    return sel( cond, alt1, S( alt2 ) );
  }
  template <typename M, typename T, typename S>
  constexpr typename std::enable_if<
      detail::is_mask<M>::value && detail::is_vector<T>::value && !detail::is_vector<S>::value &&
          ( sizeof( typename M::value_type ) == sizeof( typename T::value_type ) ) && ( M::size() == T::size() ) &&
          std::is_same<typename std::common_type<typename T::value_type, S>::type, typename T::value_type>::value,
      S>::type
  sel( const M& cond, const S& alt1, const T& alt2 ) noexcept {
    return sel( cond, T( alt1 ), alt2 );
  }

  /// element-wise minimum of two vectors
  template <typename T>
  detail::enable_if_vector_t<T, T> min( const T& a, const T& b ) noexcept {
    return sel( a < b, a, b );
  }
  /// element-wise maximum of two vectors
  template <typename T>
  detail::enable_if_vector_t<T, T> max( const T& a, const T& b ) noexcept {
    return sel( a < b, b, a );
  }

  /// horizontal rotate - rotate elements in vector
  template <std::size_t NROTATE, typename T>
  constexpr detail::enable_if_vector_t<T, T> hrotate( const T& v ) noexcept {
    static_assert( NROTATE < T::size(), "rotation count too high" );
    return detail::hrotate<NROTATE>( v, std::make_index_sequence<T::size()>() );
  }

  /// horizontal add - adds up all elements in the vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, T> haddv( const T& _v ) noexcept {
    using detail::_hadd;
    using SEQ = decltype( std::make_index_sequence<T::size()>() );
    T v( _v );
    switch ( T::size() ) {
    case 1024:
      _hadd<512>( v, SEQ() ); // fallthrough
    case 512:
      _hadd<256>( v, SEQ() ); // fallthrough
    case 256:
      _hadd<128>( v, SEQ() ); // fallthrough
    case 128:
      _hadd<64>( v, SEQ() ); // fallthrough
    case 64:
      _hadd<32>( v, SEQ() ); // fallthrough
    case 32:
      _hadd<16>( v, SEQ() ); // fallthrough
    case 16:
      _hadd<8>( v, SEQ() ); // fallthrough
    case 8:
      _hadd<4>( v, SEQ() ); // fallthrough
    case 4:
      _hadd<2>( v, SEQ() ); // fallthrough
    case 2:
      _hadd<1>( v, SEQ() ); // fallthrough
    case 1:
      break;
    default:
      UNREACHABLE();
    }
    return v;
  }

  /// horizontal add - adds up all elements in the vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, typename T::value_type> hadd( const T& _v ) noexcept {
    return haddv( _v )[0];
  }

  /// horizontal min - minimum of all elements in vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, T> hminv( const T& _v ) noexcept {
    return detail::_hcmp(
        []( const auto& v, const auto& w ) noexcept { return v < w; }, _v );
  }

  /// horizontal min - minimum of all elements in vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, typename T::value_type> hmin( const T& _v ) noexcept {
    return hminv( _v )[0];
  }

  /// horizontal max - maximum of all elements in vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, T> hmaxv( const T& _v ) noexcept {
    return detail::_hcmp(
        []( const auto& v, const auto& w ) noexcept { return v > w; }, _v );
  }

  /// horizontal max - maximum of all elements in vector v
  template <typename T>
  constexpr detail::enable_if_vector_t<T, typename T::value_type> hmax( const T& _v ) noexcept {
    return hmaxv( _v )[0];
  }

  template <std::size_t N, typename T, typename IT, typename OP>
  constexpr simdv<T, N> map( OP op, IT first, IT last,
                             T default_value = T() ) noexcept( noexcept( op( *first ) ) && noexcept( T( T() ) ) ) {
    const typename simdv<T, N>::size_type sz     = std::distance( first, last );
    simdv<T, N>                           retVal = default_value;
    auto                                  it     = retVal.begin();
    const auto                            itend  = it + ( sz < retVal.size() ? sz : retVal.size() );
    for ( ; itend != it; ++first, ++it ) *it = op( *first );
    return retVal;
  }

  namespace detail { ///< implementation details
    /// true if all arguments are true
    constexpr static bool ALL() noexcept { return true; }
    /// true if all arguments are true
    template <typename ARG, typename... ARGS>
    constexpr static bool ALL( ARG&& arg, ARGS&&... args ) noexcept {
      return bool( arg ) && ALL( std::forward<ARGS>( args )... );
    }

    /// convenience typedef
    template <std::size_t... IDXs>
    using indices = std::index_sequence<IDXs...>;
    /** @brief vector sorting stage
     *
     * compares a permutation of a vector to its original using a
     * comparison functor, and put elements in a pair for which the
     * comparison functor returned true first.
     *
     * @tparam CMP  type of comparison functor
     */
    template <typename CMP>
    struct SortStage {
      CMP m_cmp;
      /** @brief vector sorting stage
       *
       * compares a vector to a permutation of itself, and puts the
       * "smaller" element first
       *
       * @tparam IDXPERM  permutation of indices
       * @tparam IDXMASK  mask indicating which comparison results need
       *                  flipping
       * @tparam ARGS     type of any additional arguments that are
       *                  permuted the same way as the vector to be
       *                  sorted
       *
       * @param v         vector to be sorted
       * @param args...   other arguments that are permuted the same way
       *                  as v
       */
      template <std::size_t... IDXPERM, std::size_t... IDXMASK, typename T, std::size_t N, typename... ARGS>
      void operator()( indices<IDXPERM...> /* unused */, indices<IDXMASK...> /* unused */, ve::simdv<T, N>& v,
                       ARGS&... args ) const noexcept {
        using mask_type       = typename ve::simdv<T, N>::mask_type;
        using mask_value_type = typename mask_type::value_type;
        constexpr mask_type flip{mask_value_type( -mask_value_type( bool( IDXMASK ) ) )...};
        const auto          vperm = shuffle<IDXPERM...>( v );
        const auto          mask  = m_cmp( v, vperm ) ^ flip;
        v                         = sel( mask, v, vperm );
        std::forward_as_tuple( ( ( args = sel( mask, args, shuffle<IDXPERM...>( args ) ) ), 0 )... );
      }
    };
    /** @brief one step of bitonic merge sort, horizontally vectorized
     *
     * @tparam N            length of sublists to be sorted
     * @tparam STAGE        SortStage with suitable comparison functor
     * @tparam ARGS...      arguments given to SortStage to be sorted
     * @tparam IDXs...      consecutive index sequence starting at 0,
     *                      enumerating each element in the vector
     *
     * @param stage         sorting stage object
     * @param args...       vector (and other vectors) to be sorted
     */
    template <std::size_t N, typename STAGE, typename... ARGS, std::size_t... IDXs>
    void bitonicsort_step( const STAGE& stage, indices<IDXs...> /* unused */, ARGS&... args ) noexcept {
      static_assert( 0 == ( N & ( N - 1 ) ), "N must be a power of 2" );
      constexpr auto MASK = 2 * N - 1;
      stage( indices<( ( IDXs & ( ~MASK ) ) | ( MASK - ( IDXs & MASK ) ) )...>(), indices<bool( IDXs & N )...>(),
             args... );
      switch ( N ) {
      case 512:
        stage( indices<( IDXs ^ 256 )...>(), indices<bool( IDXs & 256 )...>(), args... ); /* fallthrough */
      case 256:
        stage( indices<( IDXs ^ 128 )...>(), indices<bool( IDXs & 128 )...>(), args... ); /* fallthrough */
      case 128:
        stage( indices<( IDXs ^ 64 )...>(), indices<bool( IDXs & 64 )...>(), args... ); /* fallthrough */
      case 64:
        stage( indices<( IDXs ^ 32 )...>(), indices<bool( IDXs & 32 )...>(), args... ); /* fallthrough */
      case 32:
        stage( indices<( IDXs ^ 16 )...>(), indices<bool( IDXs & 16 )...>(), args... ); /* fallthrough */
      case 16:
        stage( indices<( IDXs ^ 8 )...>(), indices<bool( IDXs & 8 )...>(), args... ); /* fallthrough */
      case 8:
        stage( indices<( IDXs ^ 4 )...>(), indices<bool( IDXs & 4 )...>(), args... ); /* fallthrough */
      case 4:
        stage( indices<( IDXs ^ 2 )...>(), indices<bool( IDXs & 2 )...>(), args... ); /* fallthrough */
      case 2:
        stage( indices<( IDXs ^ 1 )...>(), indices<bool( IDXs & 1 )...>(), args... ); /* fallthrough */
      default:
        break;
      };
    }

    /** @brief bitonic merge sort in a vector register
     *
     * @tparam STAGE        SortStage with suitable comparison functor
     * @tparam T            type of underlying vector's elements
     * @tparam N            number of elements in vector
     * @tparam ARGS...      types of other arguments to be permuted along
     *                      with vector to be sorted
     *
     * @param stage         sorting stage object
     * @param v             vector to be sorted
     * @param args...       other vector to be permuted along with v
     */
    template <typename STAGE, typename T, std::size_t N, typename... ARGS>
    void bitonicsort( const STAGE& stage, simdv<T, N>& v, ARGS&... args ) noexcept {
      static_assert( N <= 1024, "N must be <= 1024" );
      // size 1 array are always sorted
      if ( N >= 2 ) bitonicsort_step<1>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 4 ) bitonicsort_step<2>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 8 ) bitonicsort_step<4>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 16 ) bitonicsort_step<8>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 32 ) bitonicsort_step<16>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 64 ) bitonicsort_step<32>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 128 ) bitonicsort_step<64>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 256 ) bitonicsort_step<128>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 512 ) bitonicsort_step<256>( stage, std::make_index_sequence<N>(), v, args... );
      if ( N >= 1024 ) bitonicsort_step<512>( stage, std::make_index_sequence<N>(), v, args... );
    }
  } // namespace detail

  /** @brief horizontal sort
   *
   * Sorts a vector horizontally.
   *
   * @tparam T        type of vector's elements
   * @tparam N        length of vector
   * @tparam CMP      type of comparison functor
   * @tparam ARGS...  type(s) of other vectors to be permuted along with
   *                  vector
   *
   * @param v         vector to be sorted
   * @param cmp       comparison functor
   * @param args...   other vectors to be permuted along with v
   *
   * Example: Given a vector, sort it into descending order, and keep track
   * of the elements by recording at which indices in the array they were
   * originally:
   * @code
   * simdv<float, 8> v = get_vector_from_somewhere();
   * using mask_type = typename simdv<float, 8>::mask_type;
   * mask_type idxs(mask_type::iota(), 0); // {0, 1, 2, 3, 4, 5, 6, 7}
   * // sort v in descending order, permute idxs along with it
   * hsort(v, [] (const auto& a, const auto& b) noexcept { return a > b; },
   *       idxs);
   * // v is now in descending order, and for each element v[i], idxs[i]
   * contains the index at which that element was located before the sort
   * @endcode
   */
  template <typename T, typename CMP, typename... ARGS>
  detail::enable_if_vector_t<T> hsort( T& v, CMP cmp, ARGS&... args ) noexcept {
    static_assert( detail::ALL( ( T::size() == ARGS::size() )... ), "all arguments must be same size" );
    detail::bitonicsort( detail::SortStage<CMP>{std::move( cmp )}, v, args... );
  }

  namespace detail {
    /// a simple insertion sort to mop up when required
    template <typename iter, typename compar, typename value>
    void __insertionsort( const iter& start, const iter& end, const compar& cmp, value v ) {
      iter i = start + 1, j;
      do {
        j = i - 1;
        while ( ( start <= j ) && cmp( v, *j ) ) {
          *( j + 1 ) = *j;
          --j;
        }
        *( j + 1 ) = v;
        if ( end != ++i ) v = *i;
      } while ( end != i );
    }

    // generic shellsort driver - don't use directly
    template <typename iter, typename compar, typename val>
    void __shellsort_doit( iter start, iter end, compar cmp, val v, const size_t* incs ) {
      size_t h;
      iter   it, jt;
      // skip too large increments
      while ( ( size_t( 1 ) < *incs ) && ( size_t( end - start ) <= *incs ) ) ++incs;
      while ( ( h = *incs++ ) > 1 ) {
        for ( it = start + h; it < end; ++it ) {
          v  = *it;
          jt = it;
          while ( ( jt >= ( start + h ) ) && cmp( v, *( jt - h ) ) ) {
            *jt = *( jt - h );
            jt -= h;
          }
          *jt = v;
        }
      }
      if ( end - start > 1 ) { __insertionsort( start, end, cmp, *( start + 1 ) ); }
    }

    static constexpr std::size_t __shellsort_incs[] = {11117101561907004825ULL,
                                                       4940934027514224367ULL,
                                                       2195970678895210830ULL,
                                                       975986968397871480ULL,
                                                       433771985954609547ULL,
                                                       192787549313159799ULL,
                                                       85683355250293244ULL,
                                                       38081491222352553ULL,
                                                       16925107209934468ULL,
                                                       7522269871081986ULL,
                                                       3343231053814216ULL,
                                                       1485880468361874ULL,
                                                       660391319271944ULL,
                                                       293507253009753ULL,
                                                       130447668004335ULL,
                                                       57976741335260ULL,
                                                       11452195819311ULL,
                                                       25767440593449ULL,
                                                       5089864808583ULL,
                                                       2262162137148ULL,
                                                       1005405394288ULL,
                                                       446846841906ULL,
                                                       198598596403ULL,
                                                       88266042846ULL,
                                                       39229352376ULL,
                                                       17435267723ULL,
                                                       7749007877ULL,
                                                       3444003501ULL,
                                                       1530668223ULL,
                                                       680296988ULL,
                                                       302354217ULL,
                                                       134379652ULL,
                                                       59724290ULL,
                                                       26544129ULL,
                                                       11797391ULL,
                                                       5243285ULL,
                                                       2330349ULL,
                                                       1035711ULL,
                                                       460316ULL,
                                                       204585ULL,
                                                       90927ULL,
                                                       40412ULL,
                                                       17961ULL,
                                                       7983ULL,
                                                       3548ULL,
                                                       1577ULL,
                                                       701UL,
                                                       301UL,
                                                       132UL,
                                                       57UL,
                                                       23UL,
                                                       10UL,
                                                       4UL,
                                                       1UL,
                                                       0UL};

    // shell sort - usage is like std::sort
    // not terribly fast, but probably ok for larger arrays
    template <typename iter, typename compar>
    void shellsort( iter start, iter end, compar cmp ) {
      if ( ( end - start ) > 16 )
        __shellsort_doit( start, end, cmp, *start, __shellsort_incs );
      else if ( ( end - start ) > 1 )
        __insertionsort( start, end, cmp, *( start + 1 ) );
    }

#if 0
        /// mergesort implementation, does SIMDWIDTH-sized sublists vectorised
        template <std::size_t SIMDWIDTH, typename T, typename CMP>
        void mergesort(T* first, std::size_t sz, const CMP& cmp, T* wkspfirst)
        {
            using value_type = typename std::decay<T>::type;
            using vect_type = ve::simdv<value_type, SIMDWIDTH>;
            // sort sublists of length SIMDWIDTH or smaller
            auto it = first;
            for (auto itend = first + sz; it + SIMDWIDTH <= itend; it += SIMDWIDTH) {
                vect_type& v = *reinterpret_cast<vect_type*>(reinterpret_cast<value_type*>(&*it));
                hsort(v, cmp);
            }
            if (it + 1 < first + sz) __insertionsort(it, first + sz, cmp, *(it + 1));
            // merge sublists
            bool swapped = false;
            for (std::size_t incr = SIMDWIDTH; incr <= sz; incr *= 2) {
                auto jt = wkspfirst;
                auto itend = first + sz;
                for (it = first; it + incr < itend; it += 2 * incr) {
                    auto mid = it + incr;
                    auto last = std::min(mid + incr, itend);
                    jt = std::merge(it, mid, mid, last, jt, cmp);
                }
                if (it < itend) {
                    jt = std::copy(it, itend, jt);
                }
                std::swap(first, wkspfirst);
                swapped = !swapped;
            }
            // if needed, copy back from workspace
            if (swapped) {
                std::swap(first, wkspfirst);
                std::copy(wkspfirst, wkspfirst + sz, first);
            }
        }
#endif
    template <typename T, typename CMP>
    void shortsort_aligned( T* first, T* last, const CMP& cmp ) {
      using value_type  = typename std::decay<T>::type;
      auto     it       = first;
      unsigned sublists = 0;
      if ( it + 64 <= last ) {
        using vect_type = ve::simdv<value_type, 64>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 64;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      if ( it + 32 <= last ) {
        using vect_type = ve::simdv<value_type, 32>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 32;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      if ( it + 16 <= last ) {
        using vect_type = ve::simdv<value_type, 16>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 16;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      if ( it + 8 <= last ) {
        using vect_type = ve::simdv<value_type, 8>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 8;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      if ( it + 4 <= last ) {
        using vect_type = ve::simdv<value_type, 4>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 4;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      if ( it + 2 <= last ) {
        using vect_type = ve::simdv<value_type, 2>;
        vect_type& v    = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
        it += 2;
        ++sublists;
      }
      if ( it == last && 1 == sublists ) return;
      // more than one sublist, or not done - finish up with a pass of
      // insertion sort
      __insertionsort( first, last, cmp, *( first + 1 ) );
    }
    /// mergesort implementation, does SIMDWIDTH-sized sublists vectorised
    template <std::size_t SIMDWIDTH, typename T, typename CMP>
    void mergesort_inplace( T* first, std::size_t sz, const CMP& cmp ) {
      using value_type = typename std::decay<T>::type;
      using vect_type  = ve::simdv<value_type, SIMDWIDTH>;
      // sort sublists of length SIMDWIDTH or smaller
      auto it = first;
      for ( auto itend = first + sz; it + SIMDWIDTH <= itend; it += SIMDWIDTH ) {
        vect_type& v = *reinterpret_cast<vect_type*>( reinterpret_cast<value_type*>( &*it ) );
        hsort( v, cmp );
      }
      if ( it + 1 < first + sz ) shortsort_aligned( it, first + sz, cmp );
      // merge sublists
      for ( std::size_t incr = SIMDWIDTH; incr <= sz; incr *= 2 ) {
        auto itend = first + sz;
        for ( it = first; it + incr < itend; it += 2 * incr ) {
          auto mid  = it + incr;
          auto last = std::min( mid + incr, itend );
          std::inplace_merge( it, mid, last, cmp );
        }
      }
    }
  } // namespace detail

  // [FIXME]: This is very much work in progress - it's working, but not
  // faster
  template <std::size_t SIMDWIDTH, typename IT, typename CMP, typename... ARGS>
  void sort( IT first, IT last, const CMP& cmp ) __attribute__( ( noinline ) );
  template <std::size_t SIMDWIDTH, typename IT, typename CMP, typename... ARGS>
  void sort( IT first, IT last, const CMP& cmp ) {
    // check alignment...
    const std::size_t align_offs =
        static_cast<char*>( static_cast<void*>( &*first ) ) - static_cast<char*>( static_cast<void*>( nullptr ) );
    if ( 0 == ( align_offs & 63 ) ) {
      detail::shellsort( first, last, cmp );
      // detail::mergesort_inplace<SIMDWIDTH>(&*first, last - first, cmp);
    } else {
      // not aligned to cache line - this is bad, and we fall back to
      // std::sort
      std::sort( first, last, cmp );
    }
  }

  namespace detail { /// implementation details
    template <typename JT>
    using pointee_t = typename std::decay<decltype( *std::declval<JT>() )>::type;
    template <typename JT, std::size_t SIMDWIDTH>
    using vecttype_t = typename std::conditional<
        std::is_const<typename std::remove_reference<decltype( *std::declval<JT>() )>::type>::value,
        const simdv<pointee_t<JT>, SIMDWIDTH>, simdv<pointee_t<JT>, SIMDWIDTH>>::type;
  } // namespace detail

  /** @brief vectorised version of std::transform
   *
   * @tparam SIMDWIDTH        width of SIMD register to use
   * @tparam IT               type of iterator to first kind of element to
   *                          be transformed
   * @tparam OUTIT            type of iterator for output
   * @tparam OP               type of functor performing operation on
   *                          elements (care must be taken to formulate it
   *                          such that it can be called with both scalar
   *                          and vector arguments)
   * @tparam ITS...           types of other kinds of elements to be
   *                          transformed (if any)
   *
   * @param first             iterator pointing to first element in range
   *                          to be transformed
   * @param last              iterator pointing one past the last element in
   *                          range to be transformed
   * @param newfirst          iterator pointing to the first element of the
   *                          output range
   * @param op                operation to perform
   * @param others...         if op takes more than one argument(s), others
   *                          are the iterators pointing to the first element
   *                          of the ranges supplying the other arguments
   *
   * @returns iterator pointing one past the last element written to the
   *          output range
   *
   * The code should behave as if this code was used in the implementation:
   *
   * @code
   * for (; last != first;
   *         std::forward_as_tuple((++first, 0), (++newfirst, 0),
   *                               (++others, 0)...)) {
   *     *newfirst = op(*first, (*others)...);
   * }
   * @endcode
   *
   * If the ranges are longer than SIMDWIDTH, and if the alignment of all
   * ranges is suitable, the implementation will fill vector registers from
   * the ranges, and call op with the contents of those vector registers.
   * Thus, it is important that op can also be called with references to
   * objects of type simdv<T, SIMDWIDTH> for suitable types T.
   */
  template <std::size_t SIMDWIDTH, typename IT, typename OUTIT, typename OP, typename... ITS>
  __attribute__( ( noinline ) ) OUTIT
  vtransform( IT first, IT last, OUTIT newfirst, OP op,
              ITS... others ) noexcept( noexcept( *newfirst = op( *first, *others... ) ) ) {
    using detail::ALL;
    using detail::pointee_t;
    using detail::vecttype_t;
    // make sure we only deal with elements of the same size
    static_assert( sizeof( pointee_t<IT> ) == sizeof( pointee_t<OUTIT> ) &&
                       ALL( ( sizeof( pointee_t<IT> ) == sizeof( pointee_t<ITS> ) )... ),
                   "sizes of pointees must match" );
    // work out mask to check alignment constraints
    constexpr auto alignmask = sizeof( typename std::decay<decltype( *first )>::type ) * SIMDWIDTH - 1;
    // if we have enough elements, and if we're aligned correctly, do the
    // loop vectorised
    if ( std::size_t( std::distance( first, last ) ) >= SIMDWIDTH &&
         ( 0 == ( reinterpret_cast<std::size_t>( &*first ) & alignmask ) ) &&
         ( 0 == ( reinterpret_cast<std::size_t>( &*newfirst ) & alignmask ) ) &&
         ALL( ( 0 == ( reinterpret_cast<std::size_t>( &*others ) & alignmask ) )... ) ) {
      // the full vectorised crazyness
      for ( ; std::size_t( std::distance( first, last ) ) >= SIMDWIDTH; std::forward_as_tuple(
                ( first += SIMDWIDTH, 0 ), ( newfirst += SIMDWIDTH, 0 ), ( others += SIMDWIDTH, 0 )... ) ) {
        reinterpret_cast<vecttype_t<OUTIT, SIMDWIDTH>&>( *newfirst ) =
            op( reinterpret_cast<vecttype_t<IT, SIMDWIDTH>&>( *first ),
                reinterpret_cast<vecttype_t<ITS, SIMDWIDTH>&>( *others )... );
      }
    }
    for ( ; last != first; std::forward_as_tuple( ( ++first, 0 ), ( ++newfirst, 0 ), ( ++others, 0 )... ) ) {
      *newfirst = op( *first, *others... );
    }
    return newfirst;
  }

} // namespace ve

/* Copyright (C) CERN for the benefit of the LHCb collaboration
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 * In applying this licence, CERN does not waive the privileges and immunities
 * granted to it by virtue of its status as an Intergovernmental Organization
 * or submit itself to any jurisdiction.
 */

// vim: sw=4:tw=78:ft=cpp:et
