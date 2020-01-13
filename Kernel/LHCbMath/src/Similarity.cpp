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

#include <type_traits>
#include <utility>

#include "GaudiKernel/System.h"
#include "LHCbMath/Similarity.h"
#include "vectorclass.h"

namespace LHCb::Math::detail {

  using gsl::span;

  namespace {

    //----------------------------------------------------------------------
    // AVX details
    //----------------------------------------------------------------------
#if defined( __AVX__ )

    auto to_Vec4d( span<const double, 4> s ) { return Vec4d{}.load( s.data() ); }
    template <std::ptrdiff_t N>
    auto to_Vec4d( span<const double, N> s ) {
      static_assert( N < 4, "N<4" );
      return Vec4d{}.load_partial( N, s.data() );
    }

    void assign( span<double, 4> s, const Vec4d& v ) { v.store( s.data() ); }
    template <std::ptrdiff_t N>
    void assign( span<double, N> s, const Vec4d& v ) {
      static_assert( N < 4, "N<4" );
      v.store_partial( N, s.data() );
    }

    inline double dot5( Vec4d f0, double f1, Vec4d r0, double r1 ) { return horizontal_add( r0 * f0 ) + r1 * f1; }

    inline double dot5( span<const double, 5> f, Vec4d r0, double r2 ) {
      return dot5( to_Vec4d( f.subspan<0, 4>() ), f[4], r0, r2 );
    }

    inline Vec2d dots2_5( span<const double, 10> f, Vec4d r0, double r2 ) {
      const auto x0   = r0 * to_Vec4d( f.subspan<0, 4>() );
      const auto x1   = r0 * to_Vec4d( f.subspan<5, 4>() );
      const auto temp = Vec4d{_mm256_hadd_pd( x0, x1 )};
      return temp.get_low() + temp.get_high() + gather2d<4, 9>( f.data() ) * r2;
    }

    inline Vec4d madd4_5( Vec4d f0, Vec4d f1, Vec4d f2, Vec4d f3, Vec4d f4, Vec4d r0, double r4 ) {
      return f0 * r0[0] + f1 * r0[1] + f2 * r0[2] + f3 * r0[3] + f4 * r4;
    }

    inline Vec4d dots4_5( span<const double, 20> f, Vec4d r0, double r4 ) {
      const auto x0     = r0 * to_Vec4d( f.subspan<0, 4>() );
      const auto x1     = r0 * to_Vec4d( f.subspan<5, 4>() );
      const auto x2     = r0 * to_Vec4d( f.subspan<10, 4>() );
      const auto x3     = r0 * to_Vec4d( f.subspan<15, 4>() );
      const auto temp01 = Vec4d{_mm256_hadd_pd( x0, x1 )};
      const auto temp23 = Vec4d{_mm256_hadd_pd( x2, x3 )};
      return blend4d<2, 3, 4, 5>( temp01, temp23 ) + blend4d<0, 1, 6, 7>( temp01, temp23 ) +
             gather4d<4, 9, 14, 19>( f.data() ) * r4;
    }

    // reshuffle a symmetric, lower triangle, row major matrix for SIMD use...
    struct alignas( 32 ) avx_5_t {
      const Vec4d  c0, c1, c2, c3, c4; // r4 == c4...
      const double c44;
      avx_5_t( span<const double, 15> d )
          : c0{gather4d<0, 1, 3, 6>( d.data() )}
          , c1{gather4d<1, 2, 4, 7>( d.data() )}
          , c2{gather4d<3, 4, 5, 8>( d.data() )}
          , c3{gather4d<6, 7, 8, 9>( d.data() )}
          , c4{gather4d<10, 11, 12, 13>( d.data() )}
          , c44{d[14]} {}
      // return a column of a rhs product with column-major f (aka. tranpose of row major f)
      template <int i = 0, int j = i + 1, int k = j + 1, int l = k + 1, int m = l + 1, std::ptrdiff_t extent>
      inline Vec4d c0i( span<const double, extent> f ) const {
        return c0 * f[i] + c1 * f[j] + c2 * f[k] + c3 * f[l] + c4 * f[m];
      }
      inline double c4i( span<const double, 5> f ) const { return dot5( f, c4, c44 ); }
    };

    //----------------------------------------------------------------------
    // SSE3 details
    //----------------------------------------------------------------------
#elif defined( __SSE3__ )

    auto to_Vec2d( span<const double, 2> s ) { return Vec2d{}.load( s.data() ); }

    inline double dot5( span<const double, 5> f, Vec2d r0, Vec2d r1, double r2 ) {
      auto r = r0 * to_Vec2d( f.first<2>() ) + r1 * to_Vec2d( f.subspan<2, 2>() );
      return _mm_hadd_pd( r, r )[0] + r2 * f[4];
    }

    inline Vec2d dots2_5( span<const double, 10> f, Vec2d r0, Vec2d r1, double r2 ) {
      auto x0 = r0 * to_Vec2d( f.subspan<0, 2>() );
      auto x1 = r1 * to_Vec2d( f.subspan<2, 2>() );
      auto y0 = r0 * to_Vec2d( f.subspan<5, 2>() );
      auto y1 = r1 * to_Vec2d( f.subspan<7, 2>() );
      return Vec2d{_mm_hadd_pd( x0, y0 )} + Vec2d{_mm_hadd_pd( x1, y1 )} + r2 * Vec2d{f[4], f[9]};
    }

    // reshuffle the origin matrix for SIMD use...
    struct alignas( 16 ) sse_t {
      Vec2d r0, r10, r2, r12, r4, r14, r6, r16, r8, r18, r24;
      sse_t( span<const double, 15> d )
          : r0{d[0], d[1]}
          , r10{d[3], d[6]}
          , r2{d[1], d[2]}
          , r12{d[4], d[7]}
          , r4{d[3], d[4]}
          , r14{d[5], d[8]}
          , r6{d[6], d[7]}
          , r16{d[8], d[9]}
          , r8{d[10], d[11]}
          , r18{d[12], d[13]}
          , r24{d[14], 0.} {}
      inline Vec2d g0( span<const double, 5> f ) const {
        return r0 * f[0] + r2 * f[1] + r4 * f[2] + r6 * f[3] + r8 * f[4];
      }
      inline Vec2d g2( span<const double, 5> f ) const {
        return r10 * f[0] + r12 * f[1] + r14 * f[2] + r16 * f[3] + r18 * f[4];
      }
      inline double g4( span<const double, 5> f ) const { return dot5( f, r8, r18, r24[0] ); }
    };

#endif
  } // namespace
  //----------------------------------------------------------------------

  void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti ) {
#if defined( __AVX__ )

    const avx_5_t m{Ci};
    Ti[0] = dot5( Fi, m.c0i( Fi ), m.c4i( Fi ) );

#elif defined( __SSE3__ )

    sse_t m{Ci};
    Ti[0] = dot5( Fi, m.g0( Fi ), m.g2( Fi ), m.g4( Fi ) );

#else

    auto _0 = Ci[0] * Fi[0] + Ci[1] * Fi[1] + Ci[3] * Fi[2] + Ci[6] * Fi[3] + Ci[10] * Fi[4];
    auto _1 = Ci[1] * Fi[0] + Ci[2] * Fi[1] + Ci[4] * Fi[2] + Ci[7] * Fi[3] + Ci[11] * Fi[4];
    auto _2 = Ci[3] * Fi[0] + Ci[4] * Fi[1] + Ci[5] * Fi[2] + Ci[8] * Fi[3] + Ci[12] * Fi[4];
    auto _3 = Ci[6] * Fi[0] + Ci[7] * Fi[1] + Ci[8] * Fi[2] + Ci[9] * Fi[3] + Ci[13] * Fi[4];
    auto _4 = Ci[10] * Fi[0] + Ci[11] * Fi[1] + Ci[12] * Fi[2] + Ci[13] * Fi[3] + Ci[14] * Fi[4];
    Ti[0]   = Fi[0] * _0 + Fi[1] * _1 + Fi[2] * _2 + Fi[3] * _3 + Fi[4] * _4;

#endif
  }

  void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti ) {
#if defined( __AVX__ )

    // reshuffle the symmetric, lower diagonal, row-major Ci matrix for SIMD use...
    const avx_5_t m{Ci};

    const auto _00 = m.c0i( Fi.subspan<0, 5>() );
    const auto _04 = m.c4i( Fi.subspan<0, 5>() );
    const auto _10 = m.c0i( Fi.subspan<5, 5>() );
    const auto _14 = m.c4i( Fi.subspan<5, 5>() );
    const auto _20 = m.c0i( Fi.subspan<10, 5>() );
    const auto _24 = m.c4i( Fi.subspan<10, 5>() );
    const auto _30 = m.c0i( Fi.subspan<15, 5>() );
    const auto _34 = m.c4i( Fi.subspan<15, 5>() );
    const auto _40 = m.c0i( Fi.subspan<20, 5>() );
    const auto _44 = m.c4i( Fi.subspan<20, 5>() );

    const auto r0 = dots4_5( Fi.subspan<0, 20>(), _00, _04 );
    const auto r1 = dots4_5( Fi.subspan<5, 20>(), _10, _14 );
    // is the layout of r2 & r3 optimal?
    const auto r2 = Vec4d{dots2_5( Fi.subspan<10, 10>(), _20, _24 ), dots2_5( Fi.subspan<15, 10>(), _30, _34 )};
    const auto r3 = Vec4d{dot5( Fi.subspan<20, 5>(), _20, _24 ), 0., //
                          dot5( Fi.subspan<20, 5>(), _40, _44 ),     //
                          dot5( Fi.subspan<20, 5>(), _00, _04 )};

    // shuffle the result so that we can store the output 'in order'
    assign( Ti.subspan<0, 4>(), blend4d<0, 1, 4, 2>( r0, r1 ) );
    assign( Ti.subspan<4, 4>(), blend4d<0, 4, 2, 3>( blend4d<1, -256, 7, 2>( r1, r0 ), r2 ) );
    assign( Ti.subspan<8, 4>(), blend4d<0, 1, 2, 7>( blend4d<1, 2, 7, -256>( r2, r3 ), r1 ) );
    assign( Ti.subspan<12, 3>(), blend4d<0, 7, 2, -256>( r3, r2 ) );

#elif defined( __SSE3__ )

    // reshuffle the origin matrix for SIMD use...
    sse_t m{Ci};

    auto _0 = m.g0( Fi.subspan<0, 5>() );
    auto _2 = m.g2( Fi.subspan<0, 5>() );
    auto _4 = m.g4( Fi.subspan<0, 5>() );
    auto r  = dots2_5( Fi.subspan<0, 10>(), _0, _2, _4 );
    auto s  = dots2_5( Fi.subspan<10, 10>(), _0, _2, _4 );
    Ti[0]   = r[0];
    Ti[1]   = r[1];
    Ti[3]   = s[0];
    Ti[6]   = s[1];
    Ti[10]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<5, 5>() );
    _2      = m.g2( Fi.subspan<5, 5>() );
    _4      = m.g4( Fi.subspan<5, 5>() );
    r       = dots2_5( Fi.subspan<5, 10>(), _0, _2, _4 );
    s       = dots2_5( Fi.subspan<15, 10>(), _0, _2, _4 );
    Ti[2]   = r[0];
    Ti[4]   = r[1];
    Ti[7]   = s[0];
    Ti[11]  = s[1];
    _0      = m.g0( Fi.subspan<10, 5>() );
    _2      = m.g2( Fi.subspan<10, 5>() );
    _4      = m.g4( Fi.subspan<10, 5>() );
    r       = dots2_5( Fi.subspan<10, 10>(), _0, _2, _4 );
    Ti[5]   = r[0];
    Ti[8]   = r[1];
    Ti[12]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<15, 5>() );
    _2      = m.g2( Fi.subspan<15, 5>() );
    _4      = m.g4( Fi.subspan<15, 5>() );
    r       = dots2_5( Fi.subspan<15, 10>(), _0, _2, _4 );
    Ti[9]   = r[0];
    Ti[13]  = r[1];
    _0      = m.g0( Fi.subspan<20, 5>() );
    _2      = m.g2( Fi.subspan<20, 5>() );
    _4      = m.g4( Fi.subspan<20, 5>() );
    Ti[14]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );

#else

    if ( UNLIKELY( Ci.data() == Ti.data() ) ) { throw "target and source overlap -- do not do that"; }
    auto _0 = Ci[0] * Fi[0] + Ci[1] * Fi[1] + Ci[3] * Fi[2] + Ci[6] * Fi[3] + Ci[10] * Fi[4];
    auto _1 = Ci[1] * Fi[0] + Ci[2] * Fi[1] + Ci[4] * Fi[2] + Ci[7] * Fi[3] + Ci[11] * Fi[4];
    auto _2 = Ci[3] * Fi[0] + Ci[4] * Fi[1] + Ci[5] * Fi[2] + Ci[8] * Fi[3] + Ci[12] * Fi[4];
    auto _3 = Ci[6] * Fi[0] + Ci[7] * Fi[1] + Ci[8] * Fi[2] + Ci[9] * Fi[3] + Ci[13] * Fi[4];
    auto _4 = Ci[10] * Fi[0] + Ci[11] * Fi[1] + Ci[12] * Fi[2] + Ci[13] * Fi[3] + Ci[14] * Fi[4];
    Ti[0]   = Fi[0] * _0 + Fi[1] * _1 + Fi[2] * _2 + Fi[3] * _3 + Fi[4] * _4;
    Ti[1]   = Fi[5] * _0 + Fi[6] * _1 + Fi[7] * _2 + Fi[8] * _3 + Fi[9] * _4;
    Ti[3]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[6]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[10]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    _0      = Ci[0] * Fi[5] + Ci[1] * Fi[6] + Ci[3] * Fi[7] + Ci[6] * Fi[8] + Ci[10] * Fi[9];
    _1      = Ci[1] * Fi[5] + Ci[2] * Fi[6] + Ci[4] * Fi[7] + Ci[7] * Fi[8] + Ci[11] * Fi[9];
    _2      = Ci[3] * Fi[5] + Ci[4] * Fi[6] + Ci[5] * Fi[7] + Ci[8] * Fi[8] + Ci[12] * Fi[9];
    _3      = Ci[6] * Fi[5] + Ci[7] * Fi[6] + Ci[8] * Fi[7] + Ci[9] * Fi[8] + Ci[13] * Fi[9];
    _4      = Ci[10] * Fi[5] + Ci[11] * Fi[6] + Ci[12] * Fi[7] + Ci[13] * Fi[8] + Ci[14] * Fi[9];
    Ti[2]   = Fi[5] * _0 + Fi[6] * _1 + Fi[7] * _2 + Fi[8] * _3 + Fi[9] * _4;
    Ti[4]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[7]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[11]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    _0      = Ci[0] * Fi[10] + Ci[1] * Fi[11] + Ci[3] * Fi[12] + Ci[6] * Fi[13] + Ci[10] * Fi[14];
    _1      = Ci[1] * Fi[10] + Ci[2] * Fi[11] + Ci[4] * Fi[12] + Ci[7] * Fi[13] + Ci[11] * Fi[14];
    _2      = Ci[3] * Fi[10] + Ci[4] * Fi[11] + Ci[5] * Fi[12] + Ci[8] * Fi[13] + Ci[12] * Fi[14];
    _3      = Ci[6] * Fi[10] + Ci[7] * Fi[11] + Ci[8] * Fi[12] + Ci[9] * Fi[13] + Ci[13] * Fi[14];
    _4      = Ci[10] * Fi[10] + Ci[11] * Fi[11] + Ci[12] * Fi[12] + Ci[13] * Fi[13] + Ci[14] * Fi[14];
    Ti[5]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[8]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[12]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    _0      = Ci[0] * Fi[15] + Ci[1] * Fi[16] + Ci[3] * Fi[17] + Ci[6] * Fi[18] + Ci[10] * Fi[19];
    _1      = Ci[1] * Fi[15] + Ci[2] * Fi[16] + Ci[4] * Fi[17] + Ci[7] * Fi[18] + Ci[11] * Fi[19];
    _2      = Ci[3] * Fi[15] + Ci[4] * Fi[16] + Ci[5] * Fi[17] + Ci[8] * Fi[18] + Ci[12] * Fi[19];
    _3      = Ci[6] * Fi[15] + Ci[7] * Fi[16] + Ci[8] * Fi[17] + Ci[9] * Fi[18] + Ci[13] * Fi[19];
    _4      = Ci[10] * Fi[15] + Ci[11] * Fi[16] + Ci[12] * Fi[17] + Ci[13] * Fi[18] + Ci[14] * Fi[19];
    Ti[9]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[13]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    _0      = Ci[0] * Fi[20] + Ci[1] * Fi[21] + Ci[3] * Fi[22] + Ci[6] * Fi[23] + Ci[10] * Fi[24];
    _1      = Ci[1] * Fi[20] + Ci[2] * Fi[21] + Ci[4] * Fi[22] + Ci[7] * Fi[23] + Ci[11] * Fi[24];
    _2      = Ci[3] * Fi[20] + Ci[4] * Fi[21] + Ci[5] * Fi[22] + Ci[8] * Fi[23] + Ci[12] * Fi[24];
    _3      = Ci[6] * Fi[20] + Ci[7] * Fi[21] + Ci[8] * Fi[22] + Ci[9] * Fi[23] + Ci[13] * Fi[24];
    _4      = Ci[10] * Fi[20] + Ci[11] * Fi[21] + Ci[12] * Fi[22] + Ci[13] * Fi[23] + Ci[14] * Fi[24];
    Ti[14]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;

#endif
  }

  void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti ) {
#if defined( __AVX__ )

    // reshuffle the 5x5 symmetric Ci matrix for SIMD use...
    const avx_5_t m{Ci};

    const auto _00 = m.c0i( Fi.subspan<0, 5>() );
    const auto _04 = m.c4i( Fi.subspan<0, 5>() );
    const auto _10 = m.c0i( Fi.subspan<5, 5>() );
    const auto _14 = m.c4i( Fi.subspan<5, 5>() );
    const auto _20 = m.c0i( Fi.subspan<10, 5>() );
    const auto _24 = m.c4i( Fi.subspan<10, 5>() );
    const auto _30 = m.c0i( Fi.subspan<15, 5>() );
    const auto _34 = m.c4i( Fi.subspan<15, 5>() );
    const auto _40 = m.c0i( Fi.subspan<20, 5>() );
    const auto _44 = m.c4i( Fi.subspan<20, 5>() );
    const auto _50 = m.c0i( Fi.subspan<25, 5>() );
    const auto _54 = m.c4i( Fi.subspan<25, 5>() );
    const auto _60 = m.c0i( Fi.subspan<30, 5>() );
    const auto _64 = m.c4i( Fi.subspan<30, 5>() );

    const auto r0 = dots4_5( Fi.subspan<0, 20>(), _00, _04 );
    const auto r1 = dots4_5( Fi.subspan<5, 20>(), _10, _14 );
    const auto r2 = dots4_5( Fi.subspan<10, 20>(), _20, _24 );
    const auto r3 = dots4_5( Fi.subspan<15, 20>(), _30, _34 );
    const auto r4 = Vec4d{dots2_5( Fi.subspan<20, 10>(), _40, _44 ), dots2_5( Fi.subspan<20, 10>(), _00, _04 )};
    const auto r5 = Vec4d{dots2_5( Fi.subspan<25, 10>(), _50, _54 ), dots2_5( Fi.subspan<25, 10>(), _10, _14 )};
    const auto r6 = Vec4d{dot5( Fi.subspan<30, 5>(), _00, _04 ), dot5( Fi.subspan<30, 5>(), _20, _24 ),
                          dot5( Fi.subspan<30, 5>(), _40, _44 ), dot5( Fi.subspan<30, 5>(), _60, _64 )};

    // shuffle the result so that we can store the output 'in order'
    assign( Ti.subspan<0, 4>(), blend4d<0, 1, 4, 2>( r0, r1 ) );
    assign( Ti.subspan<4, 4>(), blend4d<0, 4, 2, 3>( blend4d<5, -256, 3, 6>( r0, r1 ), r2 ) );
    assign( Ti.subspan<8, 4>(),
            blend4d<0, 1, 6, 7>( blend4d<1, 4, -256, -256>( r2, r3 ), blend4d<-256, -256, 6, 3>( r1, r4 ) ) );
    assign( Ti.subspan<12, 4>(), blend4d<0, 1, 4, 7>( blend4d<2, 5, -256, -256>( r2, r3 ), r4 ) );
    assign( Ti.subspan<16, 4>(),
            blend4d<0, 1, 6, 7>( blend4d<6, 3, -256, -256>( r2, r5 ), blend4d<-256, -256, 2, 5>( r3, r4 ) ) );
    assign( Ti.subspan<20, 4>(), blend4d<0, 4, 3, 5>( r5, r6 ) );
    assign( Ti.subspan<24, 4>(), blend4d<7, 1, 2, 3>( blend4d<-256, 6, 1, 7>( r5, r6 ), r3 ) );

#elif defined( __SSE3__ )

    // reshuffle the 5x5 symmetric Ci matrix for SIMD use...
    sse_t m{Ci};

    auto _0 = m.g0( Fi.subspan<0, 5>() );
    auto _2 = m.g2( Fi.subspan<0, 5>() );
    auto _4 = m.g4( Fi.subspan<0, 5>() );
    Ti[0]   = dot5( Fi.subspan<0, 5>(), _0, _2, _4 );
    Ti[1]   = dot5( Fi.subspan<5, 5>(), _0, _2, _4 );
    Ti[3]   = dot5( Fi.subspan<10, 5>(), _0, _2, _4 );
    Ti[6]   = dot5( Fi.subspan<15, 5>(), _0, _2, _4 );
    Ti[10]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    Ti[15]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[21]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<5, 5>() );
    _2      = m.g2( Fi.subspan<5, 5>() );
    _4      = m.g4( Fi.subspan<5, 5>() );
    Ti[2]   = dot5( Fi.subspan<5, 5>(), _0, _2, _4 );
    Ti[4]   = dot5( Fi.subspan<10, 5>(), _0, _2, _4 );
    Ti[7]   = dot5( Fi.subspan<15, 5>(), _0, _2, _4 );
    Ti[11]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    Ti[16]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[22]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<10, 5>() );
    _2      = m.g2( Fi.subspan<10, 5>() );
    _4      = m.g4( Fi.subspan<10, 5>() );
    Ti[5]   = dot5( Fi.subspan<10, 5>(), _0, _2, _4 );
    Ti[8]   = dot5( Fi.subspan<15, 5>(), _0, _2, _4 );
    Ti[12]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    Ti[17]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[23]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<15, 5>() );
    _2      = m.g2( Fi.subspan<15, 5>() );
    _4      = m.g4( Fi.subspan<15, 5>() );
    Ti[9]   = dot5( Fi.subspan<15, 5>(), _0, _2, _4 );
    Ti[13]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    Ti[18]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[24]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<20, 5>() );
    _2      = m.g2( Fi.subspan<20, 5>() );
    _4      = m.g4( Fi.subspan<20, 5>() );
    Ti[14]  = dot5( Fi.subspan<20, 5>(), _0, _2, _4 );
    Ti[19]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[25]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<25, 5>() );
    _2      = m.g2( Fi.subspan<25, 5>() );
    _4      = m.g4( Fi.subspan<25, 5>() );
    Ti[20]  = dot5( Fi.subspan<25, 5>(), _0, _2, _4 );
    Ti[26]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );
    _0      = m.g0( Fi.subspan<30, 5>() );
    _2      = m.g2( Fi.subspan<30, 5>() );
    _4      = m.g4( Fi.subspan<30, 5>() );
    Ti[27]  = dot5( Fi.subspan<30, 5>(), _0, _2, _4 );

#else

    if ( UNLIKELY( Ci.data() == Ti.data() ) ) { throw "target and source overlap -- do not do that"; }
    auto _0 = Ci[0] * Fi[0] + Ci[1] * Fi[1] + Ci[3] * Fi[2] + Ci[6] * Fi[3] + Ci[10] * Fi[4];
    auto _1 = Ci[1] * Fi[0] + Ci[2] * Fi[1] + Ci[4] * Fi[2] + Ci[7] * Fi[3] + Ci[11] * Fi[4];
    auto _2 = Ci[3] * Fi[0] + Ci[4] * Fi[1] + Ci[5] * Fi[2] + Ci[8] * Fi[3] + Ci[12] * Fi[4];
    auto _3 = Ci[6] * Fi[0] + Ci[7] * Fi[1] + Ci[8] * Fi[2] + Ci[9] * Fi[3] + Ci[13] * Fi[4];
    auto _4 = Ci[10] * Fi[0] + Ci[11] * Fi[1] + Ci[12] * Fi[2] + Ci[13] * Fi[3] + Ci[14] * Fi[4];
    Ti[0]   = Fi[0] * _0 + Fi[1] * _1 + Fi[2] * _2 + Fi[3] * _3 + Fi[4] * _4;
    Ti[1]   = Fi[5] * _0 + Fi[6] * _1 + Fi[7] * _2 + Fi[8] * _3 + Fi[9] * _4;
    Ti[3]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[6]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[10]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    Ti[15]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[21]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[5] + Ci[1] * Fi[6] + Ci[3] * Fi[7] + Ci[6] * Fi[8] + Ci[10] * Fi[9];
    _1      = Ci[1] * Fi[5] + Ci[2] * Fi[6] + Ci[4] * Fi[7] + Ci[7] * Fi[8] + Ci[11] * Fi[9];
    _2      = Ci[3] * Fi[5] + Ci[4] * Fi[6] + Ci[5] * Fi[7] + Ci[8] * Fi[8] + Ci[12] * Fi[9];
    _3      = Ci[6] * Fi[5] + Ci[7] * Fi[6] + Ci[8] * Fi[7] + Ci[9] * Fi[8] + Ci[13] * Fi[9];
    _4      = Ci[10] * Fi[5] + Ci[11] * Fi[6] + Ci[12] * Fi[7] + Ci[13] * Fi[8] + Ci[14] * Fi[9];
    Ti[2]   = Fi[5] * _0 + Fi[6] * _1 + Fi[7] * _2 + Fi[8] * _3 + Fi[9] * _4;
    Ti[4]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[7]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[11]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    Ti[16]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[22]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[10] + Ci[1] * Fi[11] + Ci[3] * Fi[12] + Ci[6] * Fi[13] + Ci[10] * Fi[14];
    _1      = Ci[1] * Fi[10] + Ci[2] * Fi[11] + Ci[4] * Fi[12] + Ci[7] * Fi[13] + Ci[11] * Fi[14];
    _2      = Ci[3] * Fi[10] + Ci[4] * Fi[11] + Ci[5] * Fi[12] + Ci[8] * Fi[13] + Ci[12] * Fi[14];
    _3      = Ci[6] * Fi[10] + Ci[7] * Fi[11] + Ci[8] * Fi[12] + Ci[9] * Fi[13] + Ci[13] * Fi[14];
    _4      = Ci[10] * Fi[10] + Ci[11] * Fi[11] + Ci[12] * Fi[12] + Ci[13] * Fi[13] + Ci[14] * Fi[14];
    Ti[5]   = Fi[10] * _0 + Fi[11] * _1 + Fi[12] * _2 + Fi[13] * _3 + Fi[14] * _4;
    Ti[8]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[12]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    Ti[17]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[23]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[15] + Ci[1] * Fi[16] + Ci[3] * Fi[17] + Ci[6] * Fi[18] + Ci[10] * Fi[19];
    _1      = Ci[1] * Fi[15] + Ci[2] * Fi[16] + Ci[4] * Fi[17] + Ci[7] * Fi[18] + Ci[11] * Fi[19];
    _2      = Ci[3] * Fi[15] + Ci[4] * Fi[16] + Ci[5] * Fi[17] + Ci[8] * Fi[18] + Ci[12] * Fi[19];
    _3      = Ci[6] * Fi[15] + Ci[7] * Fi[16] + Ci[8] * Fi[17] + Ci[9] * Fi[18] + Ci[13] * Fi[19];
    _4      = Ci[10] * Fi[15] + Ci[11] * Fi[16] + Ci[12] * Fi[17] + Ci[13] * Fi[18] + Ci[14] * Fi[19];
    Ti[9]   = Fi[15] * _0 + Fi[16] * _1 + Fi[17] * _2 + Fi[18] * _3 + Fi[19] * _4;
    Ti[13]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    Ti[18]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[24]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[20] + Ci[1] * Fi[21] + Ci[3] * Fi[22] + Ci[6] * Fi[23] + Ci[10] * Fi[24];
    _1      = Ci[1] * Fi[20] + Ci[2] * Fi[21] + Ci[4] * Fi[22] + Ci[7] * Fi[23] + Ci[11] * Fi[24];
    _2      = Ci[3] * Fi[20] + Ci[4] * Fi[21] + Ci[5] * Fi[22] + Ci[8] * Fi[23] + Ci[12] * Fi[24];
    _3      = Ci[6] * Fi[20] + Ci[7] * Fi[21] + Ci[8] * Fi[22] + Ci[9] * Fi[23] + Ci[13] * Fi[24];
    _4      = Ci[10] * Fi[20] + Ci[11] * Fi[21] + Ci[12] * Fi[22] + Ci[13] * Fi[23] + Ci[14] * Fi[24];
    Ti[14]  = Fi[20] * _0 + Fi[21] * _1 + Fi[22] * _2 + Fi[23] * _3 + Fi[24] * _4;
    Ti[19]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[25]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[25] + Ci[1] * Fi[26] + Ci[3] * Fi[27] + Ci[6] * Fi[28] + Ci[10] * Fi[29];
    _1      = Ci[1] * Fi[25] + Ci[2] * Fi[26] + Ci[4] * Fi[27] + Ci[7] * Fi[28] + Ci[11] * Fi[29];
    _2      = Ci[3] * Fi[25] + Ci[4] * Fi[26] + Ci[5] * Fi[27] + Ci[8] * Fi[28] + Ci[12] * Fi[29];
    _3      = Ci[6] * Fi[25] + Ci[7] * Fi[26] + Ci[8] * Fi[27] + Ci[9] * Fi[28] + Ci[13] * Fi[29];
    _4      = Ci[10] * Fi[25] + Ci[11] * Fi[26] + Ci[12] * Fi[27] + Ci[13] * Fi[28] + Ci[14] * Fi[29];
    Ti[20]  = Fi[25] * _0 + Fi[26] * _1 + Fi[27] * _2 + Fi[28] * _3 + Fi[29] * _4;
    Ti[26]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;
    _0      = Ci[0] * Fi[30] + Ci[1] * Fi[31] + Ci[3] * Fi[32] + Ci[6] * Fi[33] + Ci[10] * Fi[34];
    _1      = Ci[1] * Fi[30] + Ci[2] * Fi[31] + Ci[4] * Fi[32] + Ci[7] * Fi[33] + Ci[11] * Fi[34];
    _2      = Ci[3] * Fi[30] + Ci[4] * Fi[31] + Ci[5] * Fi[32] + Ci[8] * Fi[33] + Ci[12] * Fi[34];
    _3      = Ci[6] * Fi[30] + Ci[7] * Fi[31] + Ci[8] * Fi[32] + Ci[9] * Fi[33] + Ci[13] * Fi[34];
    _4      = Ci[10] * Fi[30] + Ci[11] * Fi[31] + Ci[12] * Fi[32] + Ci[13] * Fi[33] + Ci[14] * Fi[34];
    Ti[27]  = Fi[30] * _0 + Fi[31] * _1 + Fi[32] * _2 + Fi[33] * _3 + Fi[34] * _4;

#endif
  }

  void similarity_2_5( span<const double, 3> Ci, span<const double, 10> Fi, span<double, 15> Ti ) {
    if ( UNLIKELY( Ci.data() == Ti.data() ) ) { throw "target and source overlap -- do not do that"; }
    auto _0 = Ci[0] * Fi[0] + Ci[1] * Fi[1];
    auto _1 = Ci[1] * Fi[0] + Ci[2] * Fi[1];
    Ti[0]   = Fi[0] * _0 + Fi[1] * _1;
    Ti[1]   = Fi[2] * _0 + Fi[3] * _1;
    Ti[3]   = Fi[4] * _0 + Fi[5] * _1;
    Ti[6]   = Fi[6] * _0 + Fi[7] * _1;
    Ti[10]  = Fi[8] * _0 + Fi[9] * _1;
    _0      = Ci[0] * Fi[2] + Ci[1] * Fi[3];
    _1      = Ci[1] * Fi[2] + Ci[2] * Fi[3];
    Ti[2]   = Fi[2] * _0 + Fi[3] * _1;
    Ti[4]   = Fi[4] * _0 + Fi[5] * _1;
    Ti[7]   = Fi[6] * _0 + Fi[7] * _1;
    Ti[11]  = Fi[8] * _0 + Fi[9] * _1;
    _0      = Ci[0] * Fi[4] + Ci[1] * Fi[5];
    _1      = Ci[1] * Fi[4] + Ci[2] * Fi[5];
    Ti[5]   = Fi[4] * _0 + Fi[5] * _1;
    Ti[8]   = Fi[6] * _0 + Fi[7] * _1;
    Ti[12]  = Fi[8] * _0 + Fi[9] * _1;
    _0      = Ci[0] * Fi[6] + Ci[1] * Fi[7];
    _1      = Ci[1] * Fi[6] + Ci[2] * Fi[7];
    Ti[9]   = Fi[6] * _0 + Fi[7] * _1;
    Ti[13]  = Fi[8] * _0 + Fi[9] * _1;
    _0      = Ci[0] * Fi[8] + Ci[1] * Fi[9];
    _1      = Ci[1] * Fi[8] + Ci[2] * Fi[9];
    Ti[14]  = Fi[8] * _0 + Fi[9] * _1;
  }

  bool average( span<const double, 5> X1, span<const double, 15> C1, //
                span<const double, 5> X2, span<const double, 15> C2, //
                span<double, 5> X, span<double, 15> C ) {

#if defined( __AVX__ )

    // compute the inverse of the covariance (i.e. weight) of the difference: R=(C1+C2)
    Gaudi::SymMatrix5x5 invRM;
    auto                invR = to_span( invRM );
    const auto          y0   = to_Vec4d( C1.subspan<0, 4>() ) + to_Vec4d( C2.subspan<0, 4>() );
    const auto          y1   = to_Vec4d( C1.subspan<4, 4>() ) + to_Vec4d( C2.subspan<4, 4>() );
    const auto          y2   = to_Vec4d( C1.subspan<8, 4>() ) + to_Vec4d( C2.subspan<8, 4>() );
    const auto          y3   = to_Vec4d( C1.subspan<12, 3>() ) + to_Vec4d( C2.subspan<12, 3>() );
    assign( invR.subspan<0, 4>(), y0 );
    assign( invR.subspan<4, 4>(), y1 );
    assign( invR.subspan<8, 4>(), y2 );
    assign( invR.subspan<12, 3>(), y3 );

    bool success = invRM.InvertChol();
    // compute the gain matrix

    auto invRMs = to_span( std::as_const( invRM ) );
    // K <- C1*inverse(C1+C2) = C1*invR
    const avx_5_t _invR( invRMs );

    const auto kr0 = _invR.c0i<0, 1, 3, 6, 10>( C1 );
    const auto kr1 = _invR.c0i<1, 2, 4, 7, 11>( C1 );
    const auto kr2 = _invR.c0i<3, 4, 5, 8, 12>( C1 );
    const auto kr3 = _invR.c0i<6, 7, 8, 9, 13>( C1 );
    const auto kr4 = _invR.c0i<10, 11, 12, 13, 14>( C1 );

    // transpose (kr0,kr1,kr2,kr3) -> (kc0,kc1,kc2,kc3)
    const auto _0 = blend4d<0, 1, 4, 5>( kr0, kr2 );
    const auto _1 = blend4d<0, 1, 4, 5>( kr1, kr3 );
    const auto _2 = blend4d<2, 3, 6, 7>( kr0, kr2 );
    const auto _3 = blend4d<2, 3, 6, 7>( kr1, kr3 );

    const auto kc0 = blend4d<0, 4, 2, 6>( _0, _1 );
    const auto kc1 = blend4d<1, 5, 3, 7>( _0, _1 );
    const auto kc2 = blend4d<0, 4, 2, 6>( _2, _3 );
    const auto kc3 = blend4d<1, 5, 3, 7>( _2, _3 );

    const avx_5_t _C1( C1 );
    const auto    kc4 = _C1.c0i<10, 11, 12, 13, 14>( invRMs );
    const auto    k44 = _C1.c4i( invRMs.subspan<10, 5>() );

    //       kc0[0] kc1[0] kc2[0] kc3[0]  kc4[0]     kr0[0] kr0[1] kr0[2] kr0[3]  .         0  1  2  3  4
    //       kc0[1] kc1[1] kc2[1] kc3[1]  kc4[1]     kr1[0] kr1[1] kr1[2] kr1[3]  .         5  6  7  8  9
    //  K =  kc0[2] kc1[2] kc2[2] kc3[2]  kc4[2]  =  kr2[0] kr2[1] kr2[2] kr2[3]  .    =   10 11 12 13 14
    //       kc0[3] kc1[3] kc2[3] kc3[3]  kc4[3]     kr3[0] kr3[1] kr3[2] kr3[3]  .        15 16 17 18 19
    //        .      .      .      .      k44        kr4[0] kr4[1] kr4[2] kr4[3]  k44      20 21 22 23 24

    // X <- X1 + C1*inverse(C1+C2)*(X2-X1) =  X1 + K*(X2-X1) = X1 + K*d
    const auto _x20 = to_Vec4d( X2.first<4>() );
    const auto _x10 = to_Vec4d( X1.first<4>() );
    const auto d0   = _x20 - _x10;
    double     d4   = X2[4] - X1[4];

    const auto _00 = _x10 + kc0 * d0[0] + kc1 * d0[1] + kc2 * d0[2] + kc3 * d0[3] + kc4 * d4;

    // C <-  C1 * inverse(C1+C2)  * C2 =  K * C2
    const avx_5_t _C2( C2 );
    const auto    r1 = kc0 * _C2.c0 + kc1 * _C2.c1 + kc2 * _C2.c2 + kc3 * _C2.c3 + kc4 * _C2.c4;
    const auto    r2 = madd4_5( kc0, kc1, kc2, kc3, kc4, _C2.c0, _C2.c4[0] );
    const auto    r4 = madd4_5( kc0, kc1, kc2, kc3, kc4, _C2.c1, _C2.c4[1] );
    const auto    r3 = madd4_5( _C2.c0, _C2.c1, _C2.c2, _C2.c3, _C2.c4, kr4, k44 );
    // TODO: can we avoid the horizontal_add by transposing the following?? or use a variant of dots2 for the last
    // two???
    const auto r5 = Vec4d{dot5( kr3, kc4[3], _C2.c2, _C2.c4[2] ), dot5( kr4, k44, d0, d4 ),
                          dot5( kr4, k44, _C2.c0, _C2.c4[0] ), dot5( kr4, k44, _C2.c4, _C2.c44 )};
    assign( X.first<4>(), _00 );
    X[4] = X1[4] + r5[1];

    assign( C.subspan<0, 4>(), blend4d<0, 5, 1, 6>( r1, r2 ) );
    assign( C.subspan<4, 4>(), blend4d<2, 5, 6, 3>( r4, blend4d<-256, 2, 7, -256>( r1, r2 ) ) );
    assign( C.subspan<8, 4>(), blend4d<0, 5, 2, 7>( r5, blend4d<-256, 3, -256, 5>( r1, r3 ) ) );
    assign( C.subspan<12, 3>(), blend4d<2, 3, 7, -256>( r3, r5 ) );

    return success;

#else

    // compute the inverse of the covariance (i.e. weight) of the difference: R=(C1+C2)
    Gaudi::SymMatrix5x5 invRM;
    auto                invR = invRM.Array();
    for ( int i = 0; i < 15; ++i ) invR[i] = C1[i] + C2[i];
    bool success = invRM.InvertChol();
    // compute the gain matrix

    // K <- C1*inverse(C1+C2) = C1*invR
    double K[25];
    K[0] = C1[0] * invR[0] + C1[1] * invR[1] + C1[3] * invR[3] + C1[6] * invR[6] + C1[10] * invR[10];
    K[1] = C1[0] * invR[1] + C1[1] * invR[2] + C1[3] * invR[4] + C1[6] * invR[7] + C1[10] * invR[11];
    K[2] = C1[0] * invR[3] + C1[1] * invR[4] + C1[3] * invR[5] + C1[6] * invR[8] + C1[10] * invR[12];
    K[3] = C1[0] * invR[6] + C1[1] * invR[7] + C1[3] * invR[8] + C1[6] * invR[9] + C1[10] * invR[13];
    K[4] = C1[0] * invR[10] + C1[1] * invR[11] + C1[3] * invR[12] + C1[6] * invR[13] + C1[10] * invR[14];

    K[5] = C1[1] * invR[0] + C1[2] * invR[1] + C1[4] * invR[3] + C1[7] * invR[6] + C1[11] * invR[10];
    K[6] = C1[1] * invR[1] + C1[2] * invR[2] + C1[4] * invR[4] + C1[7] * invR[7] + C1[11] * invR[11];
    K[7] = C1[1] * invR[3] + C1[2] * invR[4] + C1[4] * invR[5] + C1[7] * invR[8] + C1[11] * invR[12];
    K[8] = C1[1] * invR[6] + C1[2] * invR[7] + C1[4] * invR[8] + C1[7] * invR[9] + C1[11] * invR[13];
    K[9] = C1[1] * invR[10] + C1[2] * invR[11] + C1[4] * invR[12] + C1[7] * invR[13] + C1[11] * invR[14];

    K[10] = C1[3] * invR[0] + C1[4] * invR[1] + C1[5] * invR[3] + C1[8] * invR[6] + C1[12] * invR[10];
    K[11] = C1[3] * invR[1] + C1[4] * invR[2] + C1[5] * invR[4] + C1[8] * invR[7] + C1[12] * invR[11];
    K[12] = C1[3] * invR[3] + C1[4] * invR[4] + C1[5] * invR[5] + C1[8] * invR[8] + C1[12] * invR[12];
    K[13] = C1[3] * invR[6] + C1[4] * invR[7] + C1[5] * invR[8] + C1[8] * invR[9] + C1[12] * invR[13];
    K[14] = C1[3] * invR[10] + C1[4] * invR[11] + C1[5] * invR[12] + C1[8] * invR[13] + C1[12] * invR[14];

    K[15] = C1[6] * invR[0] + C1[7] * invR[1] + C1[8] * invR[3] + C1[9] * invR[6] + C1[13] * invR[10];
    K[16] = C1[6] * invR[1] + C1[7] * invR[2] + C1[8] * invR[4] + C1[9] * invR[7] + C1[13] * invR[11];
    K[17] = C1[6] * invR[3] + C1[7] * invR[4] + C1[8] * invR[5] + C1[9] * invR[8] + C1[13] * invR[12];
    K[18] = C1[6] * invR[6] + C1[7] * invR[7] + C1[8] * invR[8] + C1[9] * invR[9] + C1[13] * invR[13];
    K[19] = C1[6] * invR[10] + C1[7] * invR[11] + C1[8] * invR[12] + C1[9] * invR[13] + C1[13] * invR[14];

    K[20] = C1[10] * invR[0] + C1[11] * invR[1] + C1[12] * invR[3] + C1[13] * invR[6] + C1[14] * invR[10];
    K[21] = C1[10] * invR[1] + C1[11] * invR[2] + C1[12] * invR[4] + C1[13] * invR[7] + C1[14] * invR[11];
    K[22] = C1[10] * invR[3] + C1[11] * invR[4] + C1[12] * invR[5] + C1[13] * invR[8] + C1[14] * invR[12];
    K[23] = C1[10] * invR[6] + C1[11] * invR[7] + C1[12] * invR[8] + C1[13] * invR[9] + C1[14] * invR[13];
    K[24] = C1[10] * invR[10] + C1[11] * invR[11] + C1[12] * invR[12] + C1[13] * invR[13] + C1[14] * invR[14];

    // X <- X1 + C1*inverse(C1+C2)*(X2-X1) =  X1 + K*(X2-X1) = X1 + K*d
    double d[5]{X2[0] - X1[0], X2[1] - X1[1], X2[2] - X1[2], X2[3] - X1[3], X2[4] - X1[4]};
    X[0] = X1[0] + K[0] * d[0] + K[1] * d[1] + K[2] * d[2] + K[3] * d[3] + K[4] * d[4];
    X[1] = X1[1] + K[5] * d[0] + K[6] * d[1] + K[7] * d[2] + K[8] * d[3] + K[9] * d[4];
    X[2] = X1[2] + K[10] * d[0] + K[11] * d[1] + K[12] * d[2] + K[13] * d[3] + K[14] * d[4];
    X[3] = X1[3] + K[15] * d[0] + K[16] * d[1] + K[17] * d[2] + K[18] * d[3] + K[19] * d[4];
    X[4] = X1[4] + K[20] * d[0] + K[21] * d[1] + K[22] * d[2] + K[23] * d[3] + K[24] * d[4];

    // C <-  C1 * inverse(C1+C2)  * C2 =  K * C2
    C[0]  = K[0] * C2[0] + K[1] * C2[1] + K[2] * C2[3] + K[3] * C2[6] + K[4] * C2[10];
    C[1]  = K[5] * C2[0] + K[6] * C2[1] + K[7] * C2[3] + K[8] * C2[6] + K[9] * C2[10];
    C[3]  = K[10] * C2[0] + K[11] * C2[1] + K[12] * C2[3] + K[13] * C2[6] + K[14] * C2[10];
    C[6]  = K[15] * C2[0] + K[16] * C2[1] + K[17] * C2[3] + K[18] * C2[6] + K[19] * C2[10];
    C[10] = K[20] * C2[0] + K[21] * C2[1] + K[22] * C2[3] + K[23] * C2[6] + K[24] * C2[10];

    C[2]  = K[5] * C2[1] + K[6] * C2[2] + K[7] * C2[4] + K[8] * C2[7] + K[9] * C2[11];
    C[4]  = K[10] * C2[1] + K[11] * C2[2] + K[12] * C2[4] + K[13] * C2[7] + K[14] * C2[11];
    C[7]  = K[15] * C2[1] + K[16] * C2[2] + K[17] * C2[4] + K[18] * C2[7] + K[19] * C2[11];
    C[11] = K[20] * C2[1] + K[21] * C2[2] + K[22] * C2[4] + K[23] * C2[7] + K[24] * C2[11];

    C[5]  = K[10] * C2[3] + K[11] * C2[4] + K[12] * C2[5] + K[13] * C2[8] + K[14] * C2[12];
    C[8]  = K[15] * C2[3] + K[16] * C2[4] + K[17] * C2[5] + K[18] * C2[8] + K[19] * C2[12];
    C[12] = K[20] * C2[3] + K[21] * C2[4] + K[22] * C2[5] + K[23] * C2[8] + K[24] * C2[12];

    C[9]  = K[15] * C2[6] + K[16] * C2[7] + K[17] * C2[8] + K[18] * C2[9] + K[19] * C2[13];
    C[13] = K[20] * C2[6] + K[21] * C2[7] + K[22] * C2[8] + K[23] * C2[9] + K[24] * C2[13];

    C[14] = K[20] * C2[10] + K[21] * C2[11] + K[22] * C2[12] + K[23] * C2[13] + K[24] * C2[14];
    // the following used to be more stable, but isn't any longer, it seems:
    // ROOT::Math::AssignSym::Evaluate(C, -2 * K * C1) ;
    // C += C1 + ROOT::Math::Similarity(K,R) ;
    return success;

#endif
  }

  double filter( span<double, 5> X, span<double, 15> C,               //
                 span<const double, 5> Xref, span<const double, 5> H, //
                 double refResidual, double errorMeas2 ) {

#if defined( __AVX__ )

    const avx_5_t c( C );
    const auto    cht0 = c.c0i( H );
    const auto    cht4 = c.c4i( H );
    // TODO: combine
    const auto res = refResidual + dot5( H, to_Vec4d( Xref.first<4>() ) - to_Vec4d( X.first<4>() ), Xref[4] - X[4] );
    const auto errorRes2 = errorMeas2 + dot5( H, cht0, cht4 );

    const auto w     = 1. / errorRes2;
    const auto chtw0 = cht0 * w;
    const auto chtw4 = cht4 * w;
    // update the state vector and cov matrix
#  if 1
    // keep this version for now, as it doesn't change the results...
    const auto wres = res / errorRes2;
    // const auto wres = w*res; // even this already changes the results...
    const auto x0 = to_Vec4d( X.first<4>() ) + cht0 * wres;
    const auto x4 = X[4] + cht4 * wres;
#  else
    // this version has one division less, but produces slightly different answers...
    // just by changing the equivalent of a*(b/c) into (a*b)/c...
    const auto x0 = Vec4d{}.load( X ) + chtw0 * res;
    const auto x4 = X[4] + chtw4 * res;
#  endif

    const auto _0 = c.c0 - cht0 * chtw0[0];
    const auto _1 =
        blend4d<4, 1, 2, 3>( c.c1, permute4d<1, 0, 2, 2>( c.c4 ) ) - Vec4d{cht4, cht0[1], cht0[2], cht0[3]} * chtw0[1];
    const auto _2 =
        blend4d<2, 3, 6, 4>( c.c2, c.c4 ) - Vec4d{cht0[2], cht0[3], cht4, cht4} * permute4d<2, 2, 2, 0>( chtw0 );
    const auto _3 =
        Vec4d{c.c3[3], c.c4[3], c.c44, 0.} - Vec4d{cht0[3], cht4, cht4, 0.} * Vec4d{chtw0[3], chtw0[3], chtw4, 0.};

    assign( X.first<4>(), x0 );
    X[4] = x4;

    assign( C.subspan<0, 4>(), blend4d<0, 1, 5, 2>( _0, _1 ) );
    assign( C.subspan<4, 4>(), blend4d<2, 5, 6, 3>( _1, blend4d<-256, 0, 7, -256>( _2, _0 ) ) );
    assign( C.subspan<8, 4>(), blend4d<1, 5, 3, 7>( _2, blend4d<-256, 0, -256, 4>( _3, _1 ) ) );
    assign( C.subspan<12, 3>(), blend4d<6, 1, 2, -256>( _3, _2 ) );

    return w * res * res;

#else

    // The ugly code below makes the filter step about 20% faster
    // than SMatrix would do it.
    auto res = refResidual + H[0] * ( Xref[0] - X[0] ) + H[1] * ( Xref[1] - X[1] ) + H[2] * ( Xref[2] - X[2] ) +
               H[3] * ( Xref[3] - X[3] ) + H[4] * ( Xref[4] - X[4] );
    double CHT[5]    = {C[0] * H[0] + C[1] * H[1] + C[3] * H[2] + C[6] * H[3] + C[10] * H[4],
                     C[1] * H[0] + C[2] * H[1] + C[4] * H[2] + C[7] * H[3] + C[11] * H[4],
                     C[3] * H[0] + C[4] * H[1] + C[5] * H[2] + C[8] * H[3] + C[12] * H[4],
                     C[6] * H[0] + C[7] * H[1] + C[8] * H[2] + C[9] * H[3] + C[13] * H[4],
                     C[10] * H[0] + C[11] * H[1] + C[12] * H[2] + C[13] * H[3] + C[14] * H[4]};
    auto   errorRes2 = errorMeas2 + H[0] * CHT[0] + H[1] * CHT[1] + H[2] * CHT[2] + H[3] * CHT[3] + H[4] * CHT[4];

    // update the state vector and cov matrix
    auto w = res / errorRes2;
    X[0] += CHT[0] * w;
    X[1] += CHT[1] * w;
    X[2] += CHT[2] * w;
    X[3] += CHT[3] * w;
    X[4] += CHT[4] * w;

    w = 1. / errorRes2;
    C[0] -= w * CHT[0] * CHT[0];
    C[1] -= w * CHT[1] * CHT[0];
    C[3] -= w * CHT[2] * CHT[0];
    C[6] -= w * CHT[3] * CHT[0];
    C[10] -= w * CHT[4] * CHT[0];

    C[2] -= w * CHT[1] * CHT[1];
    C[4] -= w * CHT[2] * CHT[1];
    C[7] -= w * CHT[3] * CHT[1];
    C[11] -= w * CHT[4] * CHT[1];

    C[5] -= w * CHT[2] * CHT[2];
    C[8] -= w * CHT[3] * CHT[2];
    C[12] -= w * CHT[4] * CHT[2];

    C[9] -= w * CHT[3] * CHT[3];
    C[13] -= w * CHT[4] * CHT[3];

    C[14] -= w * CHT[4] * CHT[4];

    return res * res / errorRes2;

#endif
  }

} // namespace LHCb::Math::detail
