/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "VectorClass/vectorclass.h"
#include <x86intrin.h>
#ifdef NDEBUG
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#define GSL_UNENFORCED_ON_CONTRACT_VIOLATION
#endif
#include "gsl/span"
#ifdef NDEBUG
#pragma GCC diagnostic pop
#endif


namespace {

using gsl::span;

auto to_Vec2d( span<const double,2> s ) { return Vec2d{}.load( s.data() ); }

inline double dot5_sse3(span<const double,5> f, Vec2d  r0, Vec2d  r1, double r2) {
    auto r = r0*to_Vec2d(f.first<2>()) + r1*to_Vec2d(f.subspan<2,2>());
    return _mm_hadd_pd(r,r)[0]+r2*f[4];
}

inline Vec2d dots2_5_sse3(span<const double,10> f, Vec2d  r0, Vec2d  r1, double r2) {
    auto x0 = r0*to_Vec2d(f.subspan<0,2>());
    auto x1 = r1*to_Vec2d(f.subspan<2,2>());
    auto y0 = r0*to_Vec2d(f.subspan<5,2>());
    auto y1 = r1*to_Vec2d(f.subspan<7,2>());
    return Vec2d{_mm_hadd_pd(x0,y0)} + Vec2d{_mm_hadd_pd(x1,y1)} + r2*Vec2d{ f[4],f[9] };
}

// reshuffle the origin matrix for SIMD use...
struct alignas(16) sse_t {
    Vec2d r0,r10,r2,r12,r4,r14,r6,r16,r8,r18,r24;
    sse_t(span<const double,15> d) :
      r0  { d[ 0], d[ 1] }, r10 { d[ 3], d[ 6] },
      r2  { d[ 1], d[ 2] }, r12 { d[ 4], d[ 7] },
      r4  { d[ 3], d[ 4] }, r14 { d[ 5], d[ 8] },
      r6  { d[ 6], d[ 7] }, r16 { d[ 8], d[ 9] },
      r8  { d[10], d[11] }, r18 { d[12], d[13] }, r24 { d[14], 0. }
    { };
 inline Vec2d  g0( span<const double,5> f ) const {
      return r0*f[0]  + r2*f[1] + r4*f[2] + r6*f[3] + r8*f[4] ;
 }
 inline Vec2d  g2( span<const double,5> f ) const {
      return r10*f[0] + r12*f[1] + r14*f[2] + r16*f[3] + r18*f[4] ;
 }
 inline double g4( span<const double,5> f ) const {
     return dot5_sse3( f, r8, r18, r24[0] );
 }
};

}

namespace LHCb {
namespace Math {
namespace detail {
namespace sse3 {

// origin: 5x5 input symmetric matrix, in row-major version,i.e.
//  1
//  2  3
//  4  5  6
//  7  8  9 10
// 11 12 13 14 15
// F: transformation, row-major
// ti: output 5x5 matrix: F * origin * Transpose(F)

    void similarity_5_1(span<const double,15> Ci, span<const double,5> Fi, span<double,1> ti)  {
        sse_t m { Ci };
        ti[0] = dot5_sse3(Fi   ,m.g0(Fi),m.g2(Fi),m.g4(Fi));
    }

    void similarity_5_5(span<const double,15> Ci, span<const double,25> Fi, span<double,15> ti)  {
      // std::cout << "using similarity_5_5_sse3" << std::endl;
      // reshuffle the origin matrix for SIMD use...
      sse_t m { Ci };

      auto _0 = m.g0(Fi.subspan<0,5>());
      auto _2 = m.g2(Fi.subspan<0,5>());
      auto _4 = m.g4(Fi.subspan<0,5>());
      auto r = dots2_5_sse3(Fi.subspan< 0,10>(),_0,_2,_4);
      auto s = dots2_5_sse3(Fi.subspan<10,10>(),_0,_2,_4);
      ti[ 0] = r[0];
      ti[ 1] = r[1];
      ti[ 3] = s[0];
      ti[ 6] = s[1];
      ti[10] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<5,5>());
      _2 = m.g2(Fi.subspan<5,5>());
      _4 = m.g4(Fi.subspan<5,5>());
      r = dots2_5_sse3(Fi.subspan< 5,10>(),_0,_2,_4);
      s = dots2_5_sse3(Fi.subspan<15,10>(),_0,_2,_4);
      ti[ 2] = r[0];
      ti[ 4] = r[1];
      ti[ 7] = s[0];
      ti[11] = s[1];
      _0 = m.g0(Fi.subspan<10,5>());
      _2 = m.g2(Fi.subspan<10,5>());
      _4 = m.g4(Fi.subspan<10,5>());
      r = dots2_5_sse3(Fi.subspan<10,10>(),_0,_2,_4);
      ti[ 5] = r[0];
      ti[ 8] = r[1];
      ti[12] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<15,5>());
      _2 = m.g2(Fi.subspan<15,5>());
      _4 = m.g4(Fi.subspan<15,5>());
      r = dots2_5_sse3(Fi.subspan<15,10>(),_0,_2,_4);
      ti[ 9] = r[0];
      ti[13] = r[1];
      _0 = m.g0(Fi.subspan<20,5>());
      _2 = m.g2(Fi.subspan<20,5>());
      _4 = m.g4(Fi.subspan<20,5>());
      ti[14] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
    }

    void similarity_5_7(span<const double,15> Ci, span<const double,35> Fi, span<double,28> Ti)  {

      // reshuffle the 5x5 symmetric Ci matrix for SIMD use...
      sse_t m { Ci };

      auto _0 = m.g0(Fi.subspan<0,5>());
      auto _2 = m.g2(Fi.subspan<0,5>());
      auto _4 = m.g4(Fi.subspan<0,5>());
      Ti[ 0] = dot5_sse3(Fi.subspan< 0,5>(),_0,_2,_4);
      Ti[ 1] = dot5_sse3(Fi.subspan< 5,5>(),_0,_2,_4);
      Ti[ 3] = dot5_sse3(Fi.subspan<10,5>(),_0,_2,_4);
      Ti[ 6] = dot5_sse3(Fi.subspan<15,5>(),_0,_2,_4);
      Ti[10] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      Ti[15] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[21] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<5,5>());
      _2 = m.g2(Fi.subspan<5,5>());
      _4 = m.g4(Fi.subspan<5,5>());
      Ti[ 2] = dot5_sse3(Fi.subspan< 5,5>(),_0,_2,_4);
      Ti[ 4] = dot5_sse3(Fi.subspan<10,5>(),_0,_2,_4);
      Ti[ 7] = dot5_sse3(Fi.subspan<15,5>(),_0,_2,_4);
      Ti[11] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      Ti[16] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[22] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<10,5>());
      _2 = m.g2(Fi.subspan<10,5>());
      _4 = m.g4(Fi.subspan<10,5>());
      Ti[ 5] = dot5_sse3(Fi.subspan<10,5>(),_0,_2,_4);
      Ti[ 8] = dot5_sse3(Fi.subspan<15,5>(),_0,_2,_4);
      Ti[12] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      Ti[17] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[23] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<15,5>());
      _2 = m.g2(Fi.subspan<15,5>());
      _4 = m.g4(Fi.subspan<15,5>());
      Ti[ 9] = dot5_sse3(Fi.subspan<15,5>(),_0,_2,_4);
      Ti[13] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      Ti[18] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[24] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<20,5>());
      _2 = m.g2(Fi.subspan<20,5>());
      _4 = m.g4(Fi.subspan<20,5>());
      Ti[14] = dot5_sse3(Fi.subspan<20,5>(),_0,_2,_4);
      Ti[19] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[25] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<25,5>());
      _2 = m.g2(Fi.subspan<25,5>());
      _4 = m.g4(Fi.subspan<25,5>());
      Ti[20] = dot5_sse3(Fi.subspan<25,5>(),_0,_2,_4);
      Ti[26] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
      _0 = m.g0(Fi.subspan<30,5>());
      _2 = m.g2(Fi.subspan<30,5>());
      _4 = m.g4(Fi.subspan<30,5>());
      Ti[27] = dot5_sse3(Fi.subspan<30,5>(),_0,_2,_4);
    }
} } } }
