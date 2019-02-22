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
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "vectorclass.h"
#include <x86intrin.h>
#ifdef NDEBUG
#  pragma GCC diagnostic push
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include "gsl/span"
#ifdef NDEBUG
#  pragma GCC diagnostic pop
#endif

// Local
#include "LHCbMath/AVXGuard.h"
#include "LHCbMath/Similarity.h"

namespace {
  using gsl::span;

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

  template <typename T>
  std::add_const_t<T>& as_const( T& t ) {
    return t;
  }
  template <typename T>
  void as_const( T&& t ) = delete;

  inline double dot5_avx( Vec4d f0, double f1, Vec4d r0, double r1 ) { return horizontal_add( r0 * f0 ) + r1 * f1; }

  inline double dot5_avx( span<const double, 5> f, Vec4d r0, double r2 ) {
    return dot5_avx( to_Vec4d( f.subspan<0, 4>() ), f[4], r0, r2 );
  }

  inline Vec2d dots2_5_avx( span<const double, 10> f, Vec4d r0, double r2 ) {
    const auto x0   = r0 * to_Vec4d( f.subspan<0, 4>() );
    const auto x1   = r0 * to_Vec4d( f.subspan<5, 4>() );
    const auto temp = Vec4d{_mm256_hadd_pd( x0, x1 )};
    return temp.get_low() + temp.get_high() + gather2d<4, 9>( f.data() ) * r2;
  }

  inline Vec4d madd4_5_avx( Vec4d f0, Vec4d f1, Vec4d f2, Vec4d f3, Vec4d f4, Vec4d r0, double r4 ) {
    return f0 * r0[0] + f1 * r0[1] + f2 * r0[2] + f3 * r0[3] + f4 * r4;
  }

  inline Vec4d dots4_5_avx( span<const double, 20> f, Vec4d r0, double r4 ) {
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
        , c44{d[14]} {};
    // return a column of a rhs product with column-major f (aka. tranpose of row major f)
    template <int i = 0, int j = i + 1, int k = j + 1, int l = k + 1, int m = l + 1, std::ptrdiff_t extent>
    inline Vec4d c0i( span<const double, extent> f ) const {
      return c0 * f[i] + c1 * f[j] + c2 * f[k] + c3 * f[l] + c4 * f[m];
    }
    inline double c4i( span<const double, 5> f ) const { return dot5_avx( f, c4, c44 ); }
  };

} // namespace

namespace LHCb::Math::detail::avx {

  void similarity_5_1( span<const double, 15> Ci, span<const double, 5> Fi, span<double, 1> Ti ) {
    LHCb::AVX::Guard guard{};

    const avx_5_t m{Ci};
    Ti[0] = dot5_avx( Fi, m.c0i( Fi ), m.c4i( Fi ) );
  }

  void similarity_5_5( span<const double, 15> Ci, span<const double, 25> Fi, span<double, 15> Ti ) {
    LHCb::AVX::Guard guard{};

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

    const auto r0 = dots4_5_avx( Fi.subspan<0, 20>(), _00, _04 );
    const auto r1 = dots4_5_avx( Fi.subspan<5, 20>(), _10, _14 );
    // is the layout of r2 & r3 optimal?
    const auto r2 = Vec4d{dots2_5_avx( Fi.subspan<10, 10>(), _20, _24 ), dots2_5_avx( Fi.subspan<15, 10>(), _30, _34 )};
    const auto r3 = Vec4d{dot5_avx( Fi.subspan<20, 5>(), _20, _24 ), 0., dot5_avx( Fi.subspan<20, 5>(), _40, _44 ),
                          dot5_avx( Fi.subspan<20, 5>(), _00, _04 )};

    // shuffle the result so that we can store the output 'in order'
    assign( Ti.subspan<0, 4>(), blend4d<0, 1, 4, 2>( r0, r1 ) );
    assign( Ti.subspan<4, 4>(), blend4d<0, 4, 2, 3>( blend4d<1, -256, 7, 2>( r1, r0 ), r2 ) );
    assign( Ti.subspan<8, 4>(), blend4d<0, 1, 2, 7>( blend4d<1, 2, 7, -256>( r2, r3 ), r1 ) );
    assign( Ti.subspan<12, 3>(), blend4d<0, 7, 2, -256>( r3, r2 ) );
  }

  void similarity_5_7( span<const double, 15> Ci, span<const double, 35> Fi, span<double, 28> Ti ) {
    LHCb::AVX::Guard guard{};

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

    const auto r0 = dots4_5_avx( Fi.subspan<0, 20>(), _00, _04 );
    const auto r1 = dots4_5_avx( Fi.subspan<5, 20>(), _10, _14 );
    const auto r2 = dots4_5_avx( Fi.subspan<10, 20>(), _20, _24 );
    const auto r3 = dots4_5_avx( Fi.subspan<15, 20>(), _30, _34 );
    const auto r4 = Vec4d{dots2_5_avx( Fi.subspan<20, 10>(), _40, _44 ), dots2_5_avx( Fi.subspan<20, 10>(), _00, _04 )};
    const auto r5 = Vec4d{dots2_5_avx( Fi.subspan<25, 10>(), _50, _54 ), dots2_5_avx( Fi.subspan<25, 10>(), _10, _14 )};
    const auto r6 = Vec4d{dot5_avx( Fi.subspan<30, 5>(), _00, _04 ), dot5_avx( Fi.subspan<30, 5>(), _20, _24 ),
                          dot5_avx( Fi.subspan<30, 5>(), _40, _44 ), dot5_avx( Fi.subspan<30, 5>(), _60, _64 )};

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
  }

  bool average( span<const double, 5> X1, span<const double, 15> C1, span<const double, 5> X2,
                span<const double, 15> C2, span<double, 5> X, span<double, 15> C ) {
    LHCb::AVX::Guard guard{};
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

    auto invRMs = to_span( as_const( invRM ) );
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
    const auto    r2 = madd4_5_avx( kc0, kc1, kc2, kc3, kc4, _C2.c0, _C2.c4[0] );
    const auto    r4 = madd4_5_avx( kc0, kc1, kc2, kc3, kc4, _C2.c1, _C2.c4[1] );
    const auto    r3 = madd4_5_avx( _C2.c0, _C2.c1, _C2.c2, _C2.c3, _C2.c4, kr4, k44 );
    // TODO: can we avoid the horizontal_add by transposing the following?? or use a variant of dots2 for the last
    // two???
    const auto r5 = Vec4d{dot5_avx( kr3, kc4[3], _C2.c2, _C2.c4[2] ), dot5_avx( kr4, k44, d0, d4 ),
                          dot5_avx( kr4, k44, _C2.c0, _C2.c4[0] ), dot5_avx( kr4, k44, _C2.c4, _C2.c44 )};
    assign( X.first<4>(), _00 );
    X[4] = X1[4] + r5[1];

    assign( C.subspan<0, 4>(), blend4d<0, 5, 1, 6>( r1, r2 ) );
    assign( C.subspan<4, 4>(), blend4d<2, 5, 6, 3>( r4, blend4d<-256, 2, 7, -256>( r1, r2 ) ) );
    assign( C.subspan<8, 4>(), blend4d<0, 5, 2, 7>( r5, blend4d<-256, 3, -256, 5>( r1, r3 ) ) );
    assign( C.subspan<12, 3>(), blend4d<2, 3, 7, -256>( r3, r5 ) );

    return success;
  }

  double filter( span<double, 5> X, span<double, 15> C, span<const double, 5> Xref, span<const double, 5> H,
                 double refResidual, double errorMeas2 ) {
    LHCb::AVX::Guard guard{};

    const avx_5_t c( C );
    const auto    cht0 = c.c0i( H );
    const auto    cht4 = c.c4i( H );
    // TODO: combine
    const auto res =
        refResidual + dot5_avx( H, to_Vec4d( Xref.first<4>() ) - to_Vec4d( X.first<4>() ), Xref[4] - X[4] );
    const auto errorRes2 = errorMeas2 + dot5_avx( H, cht0, cht4 );

    const auto w     = 1. / errorRes2;
    const auto chtw0 = cht0 * w;
    const auto chtw4 = cht4 * w;
    // update the state vector and cov matrix
#if 1
    // keep this version for now, as it doesn't change the results...
    const auto wres = res / errorRes2;
    // const auto wres = w*res; // even this already changes the results...
    const auto x0 = to_Vec4d( X.first<4>() ) + cht0 * wres;
    const auto x4 = X[4] + cht4 * wres;
#else
    // this version has one division less, but produces slightly different answers...
    // just by changing the equivalent of a*(b/c) into (a*b)/c...
    const auto x0 = Vec4d{}.load( X ) + chtw0 * res;
    const auto x4 = X[4] + chtw4 * res;
#endif

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
  }

} // namespace LHCb::Math::detail::avx
