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

#pragma once

#include <immintrin.h>
#include <limits>

#include <cstdint>
#include <cmath>
//#include <climits>
#include <algorithm>

namespace SIMDWrapper {
namespace scalar {

  class mask_v {
    public:
      mask_v();
      mask_v(int m) : data(m) {}

      inline mask_v& operator=( const int& m) {
        data = m;
        return *this;
      }

      inline operator int() const { return data; }

      friend inline mask_v operator&&(const mask_v& lhs, const mask_v& rhs) { return lhs.data & rhs.data; }
      friend inline mask_v operator||(const mask_v& lhs, const mask_v& rhs) { return lhs.data | rhs.data; }
      friend inline mask_v operator!(const mask_v& x) { return ~x.data; }

      friend inline bool all(const mask_v& mask) { return mask==1; }
      friend inline bool none(const mask_v& mask) { return mask==0; }
      friend inline bool any(const mask_v& mask) { return mask!=0; }
    private:
      int data;
  };

  class int_v;

  class float_v {
    public:
      float_v();
      float_v( float  f ) : data( f ) { }
      float_v( const float* f ) : data( *f ) { }

      inline float_v& operator=( const float& f ) {
        data = f;
        return *this;
      }

      inline operator int_v() const;

      inline float cast() const { return data; }

      inline void store( float* ptr ) const {
        *ptr = data;
      }

      inline void compressstore( int mask, float* ptr ) const {
        if (mask) *ptr = data;
      }

      inline float hmax() const { return data; }
      inline float hmin() const { return data; }
      inline float hadd() const { return data; }
      inline float hmax(float mask) const { return (mask) ? data : std::numeric_limits<float>::min(); }
      inline float hmin(float mask) const { return (mask) ? data : std::numeric_limits<float>::max(); }
      inline float hadd(float mask) const { return (mask) ? data : 0; }

      friend inline float_v operator+(const float_v& lhs, const float_v& rhs) { return (lhs.data + rhs.data); }
      friend inline float_v operator-(const float_v& lhs, const float_v& rhs) { return (lhs.data - rhs.data); }
      friend inline float_v operator*(const float_v& lhs, const float_v& rhs) { return (lhs.data * rhs.data); }
      friend inline float_v operator/(const float_v& lhs, const float_v& rhs) { return (lhs.data / rhs.data); }
      friend inline float_v operator-(const float_v& x) { return -1.f * x; }

      friend inline float_v abs(const float_v& v) { return std::abs(v.data); }
      friend inline float_v copysign(const float_v& x, const float_v& y) { return std::copysign(x.data, y.data) ; }

      friend inline float_v sqrt(const float_v& v) { return std::sqrt(v.data); }

      friend inline float_v min(const float_v& lhs, const float_v& rhs) { return std::min(lhs, rhs); }
      friend inline float_v max(const float_v& lhs, const float_v& rhs) { return std::max(lhs, rhs); }

      friend inline float_v signselect(const float_v& s, const float_v& a, const float_v&b) { return (s.data>0)?a:b; }
      friend inline float_v select(int mask, const float_v& a, const float_v&b) { return (mask)?a:b; }

      friend inline mask_v operator<(const float_v& lhs, const float_v& rhs) { return lhs.data < rhs.data; }
      friend inline mask_v operator>(const float_v& lhs, const float_v& rhs) { return lhs.data > rhs.data; }
      friend inline mask_v operator==(const float_v& lhs, const float_v& rhs) { return lhs.data == rhs.data; }

    private:
      float data;
  };

  class int_v {
    public:
      template<int d = 0>
      constexpr int_v() : data ( d ) { }
      constexpr int_v( int  f ) : data( f ) { }
      int_v( const int* f ) : data( *f ) { }

      inline int_v& operator=( const int& f ) {
        data = f;
        return *this;
      }

      inline operator float_v() const { return float_v(float(data)); }
      inline int cast() const { return data; } // don't cast directly to avoid conflict

      inline void store( int* ptr ) const {
        *ptr = data;
      }

      inline void compressstore( int mask, int* ptr ) const {
        if (mask) *ptr = data;
      }

      inline int hmax() const { return data; }
      inline int hmin() const { return data; }
      inline int hadd() const { return data; }
      inline int hmax(int mask) const { return (mask) ? data : std::numeric_limits<int>::min(); }
      inline int hmin(int mask) const { return (mask) ? data : std::numeric_limits<int>::max(); }
      inline int hadd(int mask) const { return (mask) ? data : 0; }

      friend inline int_v operator+(const int_v& lhs, const int_v& rhs) { return lhs.data + rhs.data; }
      friend inline int_v operator-(const int_v& lhs, const int_v& rhs) { return lhs.data - rhs.data; }
      friend inline int_v operator*(const int_v& lhs, const int_v& rhs) { return lhs.data * rhs.data; }

      friend inline int_v operator&(const int_v& lhs, const int_v& rhs) { return lhs.data & rhs.data; }
      friend inline int_v operator|(const int_v& lhs, const int_v& rhs) { return lhs.data | rhs.data; }

      friend inline int_v operator<<(const int_v& lhs, const int_v& rhs) { return lhs.data << rhs.data; }
      friend inline int_v operator>>(const int_v& lhs, const int_v& rhs) { return lhs.data >> rhs.data; }

      friend inline int_v signselect(const float_v& s, const int_v& a, const int_v&b) { return (s>float_v(0.f))?a:b; }
      friend inline int_v select(int mask, const int_v& a, const int_v&b) { return (mask)?a:b; }

      friend inline mask_v operator<(const int_v& lhs, const int_v& rhs) { return lhs.data < rhs.data; }
      friend inline mask_v operator>(const int_v& lhs, const int_v& rhs) { return lhs.data > rhs.data; }
      friend inline mask_v operator==(const int_v& lhs, const int_v& rhs) { return lhs.data == rhs.data; }

    private:
      int data;
  };


  inline float_v::operator int_v() const { return int_v(int(data)); }

  inline int_v castToInt(const float_v& x) {
    union {
        float f;
        int i;
    } v;
    v.f = x.cast();
    return v.i;
  }

  inline float_v castToFloat(const int_v& x) {
    union {
        float f;
        int i;
    } v;
    v.i = x.cast();
    return v.f;
  }

  inline int_v gather(const int*base, const int_v& idx) {
    return base[idx.cast()];
  }

  inline float_v gather(const float*base, const int_v& idx) {
    return base[idx.cast()];
  }


  struct types {
    static const size_t size = 1;
    using int_v = scalar::int_v;
    using float_v = scalar::float_v;
    using mask_v = scalar::mask_v;
    constexpr static int_v indices() { return 0; }
    constexpr static int_v indices(int start) { return start; }
    template<typename T>
    static int popcount(  T mask ) {
      return (int) mask;
    }
    static mask_v loop_mask( int, int ) {
      return true;
    }
  };
}

#ifndef __AVX2__
namespace avx2 {
  using float_v = scalar::float_v;
  using int_v   = scalar::int_v;
  using types   = scalar::types;
}
#else
namespace avx2 {
  // Permutation for avx2 compress, from https://github.com/lemire/simdprune
  alignas( 32 ) const uint32_t compress_mask256_epi32[] = {
    0, 1, 2, 3, 4, 5, 6, 7, 1, 2, 3, 4, 5, 6, 7, 7, 0, 2, 3, 4, 5, 6, 7, 7, 2, 3, 4, 5, 6, 7, 7, 7, 0, 1, 3, 4, 5, 6,
    7, 7, 1, 3, 4, 5, 6, 7, 7, 7, 0, 3, 4, 5, 6, 7, 7, 7, 3, 4, 5, 6, 7, 7, 7, 7, 0, 1, 2, 4, 5, 6, 7, 7, 1, 2, 4, 5,
    6, 7, 7, 7, 0, 2, 4, 5, 6, 7, 7, 7, 2, 4, 5, 6, 7, 7, 7, 7, 0, 1, 4, 5, 6, 7, 7, 7, 1, 4, 5, 6, 7, 7, 7, 7, 0, 4,
    5, 6, 7, 7, 7, 7, 4, 5, 6, 7, 7, 7, 7, 7, 0, 1, 2, 3, 5, 6, 7, 7, 1, 2, 3, 5, 6, 7, 7, 7, 0, 2, 3, 5, 6, 7, 7, 7,
    2, 3, 5, 6, 7, 7, 7, 7, 0, 1, 3, 5, 6, 7, 7, 7, 1, 3, 5, 6, 7, 7, 7, 7, 0, 3, 5, 6, 7, 7, 7, 7, 3, 5, 6, 7, 7, 7,
    7, 7, 0, 1, 2, 5, 6, 7, 7, 7, 1, 2, 5, 6, 7, 7, 7, 7, 0, 2, 5, 6, 7, 7, 7, 7, 2, 5, 6, 7, 7, 7, 7, 7, 0, 1, 5, 6,
    7, 7, 7, 7, 1, 5, 6, 7, 7, 7, 7, 7, 0, 5, 6, 7, 7, 7, 7, 7, 5, 6, 7, 7, 7, 7, 7, 7, 0, 1, 2, 3, 4, 6, 7, 7, 1, 2,
    3, 4, 6, 7, 7, 7, 0, 2, 3, 4, 6, 7, 7, 7, 2, 3, 4, 6, 7, 7, 7, 7, 0, 1, 3, 4, 6, 7, 7, 7, 1, 3, 4, 6, 7, 7, 7, 7,
    0, 3, 4, 6, 7, 7, 7, 7, 3, 4, 6, 7, 7, 7, 7, 7, 0, 1, 2, 4, 6, 7, 7, 7, 1, 2, 4, 6, 7, 7, 7, 7, 0, 2, 4, 6, 7, 7,
    7, 7, 2, 4, 6, 7, 7, 7, 7, 7, 0, 1, 4, 6, 7, 7, 7, 7, 1, 4, 6, 7, 7, 7, 7, 7, 0, 4, 6, 7, 7, 7, 7, 7, 4, 6, 7, 7,
    7, 7, 7, 7, 0, 1, 2, 3, 6, 7, 7, 7, 1, 2, 3, 6, 7, 7, 7, 7, 0, 2, 3, 6, 7, 7, 7, 7, 2, 3, 6, 7, 7, 7, 7, 7, 0, 1,
    3, 6, 7, 7, 7, 7, 1, 3, 6, 7, 7, 7, 7, 7, 0, 3, 6, 7, 7, 7, 7, 7, 3, 6, 7, 7, 7, 7, 7, 7, 0, 1, 2, 6, 7, 7, 7, 7,
    1, 2, 6, 7, 7, 7, 7, 7, 0, 2, 6, 7, 7, 7, 7, 7, 2, 6, 7, 7, 7, 7, 7, 7, 0, 1, 6, 7, 7, 7, 7, 7, 1, 6, 7, 7, 7, 7,
    7, 7, 0, 6, 7, 7, 7, 7, 7, 7, 6, 7, 7, 7, 7, 7, 7, 7, 0, 1, 2, 3, 4, 5, 7, 7, 1, 2, 3, 4, 5, 7, 7, 7, 0, 2, 3, 4,
    5, 7, 7, 7, 2, 3, 4, 5, 7, 7, 7, 7, 0, 1, 3, 4, 5, 7, 7, 7, 1, 3, 4, 5, 7, 7, 7, 7, 0, 3, 4, 5, 7, 7, 7, 7, 3, 4,
    5, 7, 7, 7, 7, 7, 0, 1, 2, 4, 5, 7, 7, 7, 1, 2, 4, 5, 7, 7, 7, 7, 0, 2, 4, 5, 7, 7, 7, 7, 2, 4, 5, 7, 7, 7, 7, 7,
    0, 1, 4, 5, 7, 7, 7, 7, 1, 4, 5, 7, 7, 7, 7, 7, 0, 4, 5, 7, 7, 7, 7, 7, 4, 5, 7, 7, 7, 7, 7, 7, 0, 1, 2, 3, 5, 7,
    7, 7, 1, 2, 3, 5, 7, 7, 7, 7, 0, 2, 3, 5, 7, 7, 7, 7, 2, 3, 5, 7, 7, 7, 7, 7, 0, 1, 3, 5, 7, 7, 7, 7, 1, 3, 5, 7,
    7, 7, 7, 7, 0, 3, 5, 7, 7, 7, 7, 7, 3, 5, 7, 7, 7, 7, 7, 7, 0, 1, 2, 5, 7, 7, 7, 7, 1, 2, 5, 7, 7, 7, 7, 7, 0, 2,
    5, 7, 7, 7, 7, 7, 2, 5, 7, 7, 7, 7, 7, 7, 0, 1, 5, 7, 7, 7, 7, 7, 1, 5, 7, 7, 7, 7, 7, 7, 0, 5, 7, 7, 7, 7, 7, 7,
    5, 7, 7, 7, 7, 7, 7, 7, 0, 1, 2, 3, 4, 7, 7, 7, 1, 2, 3, 4, 7, 7, 7, 7, 0, 2, 3, 4, 7, 7, 7, 7, 2, 3, 4, 7, 7, 7,
    7, 7, 0, 1, 3, 4, 7, 7, 7, 7, 1, 3, 4, 7, 7, 7, 7, 7, 0, 3, 4, 7, 7, 7, 7, 7, 3, 4, 7, 7, 7, 7, 7, 7, 0, 1, 2, 4,
    7, 7, 7, 7, 1, 2, 4, 7, 7, 7, 7, 7, 0, 2, 4, 7, 7, 7, 7, 7, 2, 4, 7, 7, 7, 7, 7, 7, 0, 1, 4, 7, 7, 7, 7, 7, 1, 4,
    7, 7, 7, 7, 7, 7, 0, 4, 7, 7, 7, 7, 7, 7, 4, 7, 7, 7, 7, 7, 7, 7, 0, 1, 2, 3, 7, 7, 7, 7, 1, 2, 3, 7, 7, 7, 7, 7,
    0, 2, 3, 7, 7, 7, 7, 7, 2, 3, 7, 7, 7, 7, 7, 7, 0, 1, 3, 7, 7, 7, 7, 7, 1, 3, 7, 7, 7, 7, 7, 7, 0, 3, 7, 7, 7, 7,
    7, 7, 3, 7, 7, 7, 7, 7, 7, 7, 0, 1, 2, 7, 7, 7, 7, 7, 1, 2, 7, 7, 7, 7, 7, 7, 0, 2, 7, 7, 7, 7, 7, 7, 2, 7, 7, 7,
    7, 7, 7, 7, 0, 1, 7, 7, 7, 7, 7, 7, 1, 7, 7, 7, 7, 7, 7, 7, 0, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7, 0, 1,
    2, 3, 4, 5, 6, 6, 1, 2, 3, 4, 5, 6, 6, 6, 0, 2, 3, 4, 5, 6, 6, 6, 2, 3, 4, 5, 6, 6, 6, 6, 0, 1, 3, 4, 5, 6, 6, 6,
    1, 3, 4, 5, 6, 6, 6, 6, 0, 3, 4, 5, 6, 6, 6, 6, 3, 4, 5, 6, 6, 6, 6, 6, 0, 1, 2, 4, 5, 6, 6, 6, 1, 2, 4, 5, 6, 6,
    6, 6, 0, 2, 4, 5, 6, 6, 6, 6, 2, 4, 5, 6, 6, 6, 6, 6, 0, 1, 4, 5, 6, 6, 6, 6, 1, 4, 5, 6, 6, 6, 6, 6, 0, 4, 5, 6,
    6, 6, 6, 6, 4, 5, 6, 6, 6, 6, 6, 6, 0, 1, 2, 3, 5, 6, 6, 6, 1, 2, 3, 5, 6, 6, 6, 6, 0, 2, 3, 5, 6, 6, 6, 6, 2, 3,
    5, 6, 6, 6, 6, 6, 0, 1, 3, 5, 6, 6, 6, 6, 1, 3, 5, 6, 6, 6, 6, 6, 0, 3, 5, 6, 6, 6, 6, 6, 3, 5, 6, 6, 6, 6, 6, 6,
    0, 1, 2, 5, 6, 6, 6, 6, 1, 2, 5, 6, 6, 6, 6, 6, 0, 2, 5, 6, 6, 6, 6, 6, 2, 5, 6, 6, 6, 6, 6, 6, 0, 1, 5, 6, 6, 6,
    6, 6, 1, 5, 6, 6, 6, 6, 6, 6, 0, 5, 6, 6, 6, 6, 6, 6, 5, 6, 6, 6, 6, 6, 6, 6, 0, 1, 2, 3, 4, 6, 6, 6, 1, 2, 3, 4,
    6, 6, 6, 6, 0, 2, 3, 4, 6, 6, 6, 6, 2, 3, 4, 6, 6, 6, 6, 6, 0, 1, 3, 4, 6, 6, 6, 6, 1, 3, 4, 6, 6, 6, 6, 6, 0, 3,
    4, 6, 6, 6, 6, 6, 3, 4, 6, 6, 6, 6, 6, 6, 0, 1, 2, 4, 6, 6, 6, 6, 1, 2, 4, 6, 6, 6, 6, 6, 0, 2, 4, 6, 6, 6, 6, 6,
    2, 4, 6, 6, 6, 6, 6, 6, 0, 1, 4, 6, 6, 6, 6, 6, 1, 4, 6, 6, 6, 6, 6, 6, 0, 4, 6, 6, 6, 6, 6, 6, 4, 6, 6, 6, 6, 6,
    6, 6, 0, 1, 2, 3, 6, 6, 6, 6, 1, 2, 3, 6, 6, 6, 6, 6, 0, 2, 3, 6, 6, 6, 6, 6, 2, 3, 6, 6, 6, 6, 6, 6, 0, 1, 3, 6,
    6, 6, 6, 6, 1, 3, 6, 6, 6, 6, 6, 6, 0, 3, 6, 6, 6, 6, 6, 6, 3, 6, 6, 6, 6, 6, 6, 6, 0, 1, 2, 6, 6, 6, 6, 6, 1, 2,
    6, 6, 6, 6, 6, 6, 0, 2, 6, 6, 6, 6, 6, 6, 2, 6, 6, 6, 6, 6, 6, 6, 0, 1, 6, 6, 6, 6, 6, 6, 1, 6, 6, 6, 6, 6, 6, 6,
    0, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 6, 0, 1, 2, 3, 4, 5, 5, 5, 1, 2, 3, 4, 5, 5, 5, 5, 0, 2, 3, 4, 5, 5,
    5, 5, 2, 3, 4, 5, 5, 5, 5, 5, 0, 1, 3, 4, 5, 5, 5, 5, 1, 3, 4, 5, 5, 5, 5, 5, 0, 3, 4, 5, 5, 5, 5, 5, 3, 4, 5, 5,
    5, 5, 5, 5, 0, 1, 2, 4, 5, 5, 5, 5, 1, 2, 4, 5, 5, 5, 5, 5, 0, 2, 4, 5, 5, 5, 5, 5, 2, 4, 5, 5, 5, 5, 5, 5, 0, 1,
    4, 5, 5, 5, 5, 5, 1, 4, 5, 5, 5, 5, 5, 5, 0, 4, 5, 5, 5, 5, 5, 5, 4, 5, 5, 5, 5, 5, 5, 5, 0, 1, 2, 3, 5, 5, 5, 5,
    1, 2, 3, 5, 5, 5, 5, 5, 0, 2, 3, 5, 5, 5, 5, 5, 2, 3, 5, 5, 5, 5, 5, 5, 0, 1, 3, 5, 5, 5, 5, 5, 1, 3, 5, 5, 5, 5,
    5, 5, 0, 3, 5, 5, 5, 5, 5, 5, 3, 5, 5, 5, 5, 5, 5, 5, 0, 1, 2, 5, 5, 5, 5, 5, 1, 2, 5, 5, 5, 5, 5, 5, 0, 2, 5, 5,
    5, 5, 5, 5, 2, 5, 5, 5, 5, 5, 5, 5, 0, 1, 5, 5, 5, 5, 5, 5, 1, 5, 5, 5, 5, 5, 5, 5, 0, 5, 5, 5, 5, 5, 5, 5, 5, 5,
    5, 5, 5, 5, 5, 5, 0, 1, 2, 3, 4, 4, 4, 4, 1, 2, 3, 4, 4, 4, 4, 4, 0, 2, 3, 4, 4, 4, 4, 4, 2, 3, 4, 4, 4, 4, 4, 4,
    0, 1, 3, 4, 4, 4, 4, 4, 1, 3, 4, 4, 4, 4, 4, 4, 0, 3, 4, 4, 4, 4, 4, 4, 3, 4, 4, 4, 4, 4, 4, 4, 0, 1, 2, 4, 4, 4,
    4, 4, 1, 2, 4, 4, 4, 4, 4, 4, 0, 2, 4, 4, 4, 4, 4, 4, 2, 4, 4, 4, 4, 4, 4, 4, 0, 1, 4, 4, 4, 4, 4, 4, 1, 4, 4, 4,
    4, 4, 4, 4, 0, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 0, 1, 2, 3, 3, 3, 3, 3, 1, 2, 3, 3, 3, 3, 3, 3, 0, 2,
    3, 3, 3, 3, 3, 3, 2, 3, 3, 3, 3, 3, 3, 3, 0, 1, 3, 3, 3, 3, 3, 3, 1, 3, 3, 3, 3, 3, 3, 3, 0, 3, 3, 3, 3, 3, 3, 3,
    3, 3, 3, 3, 3, 3, 3, 3, 0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 2, 0, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
    2, 2, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

  class int_v;

  class float_v {
    public:
      float_v() {}
      float_v( scalar::float_v& f) : data(  _mm256_set1_ps( f.cast() ) ) {}
      float_v( float  f ) : data( _mm256_set1_ps(f) ) { }
      float_v( const float* f ) : data( _mm256_loadu_ps(f) ) { }
      float_v( __m256 f ) : data( f ) { }

      inline float_v& operator=( const __m256& f ) {
        data = f;
        return *this;
      }

      inline float_v& operator=( const float_v& f ) {
        data = f.data;
        return *this;
      }

      inline operator __m256() const { return data; }

      inline operator int_v() const;

      inline void store(float* ptr) const {
        _mm256_storeu_ps(ptr, data);
      }

      inline void compressstore( __m256 mask, float* ptr ) const {
        __m256i perm = _mm256_load_si256( (const __m256i *)compress_mask256_epi32 + ( _mm256_movemask_ps(mask) ^ 0xFF ) );
        _mm256_storeu_ps( ptr, _mm256_permutevar8x32_ps( data, perm ) );
      }

      friend inline float_v operator+(const float_v& lhs, const float_v& rhs) { return _mm256_add_ps(lhs,rhs); }
      friend inline float_v operator-(const float_v& lhs, const float_v& rhs) { return _mm256_sub_ps(lhs,rhs); }
      friend inline float_v operator*(const float_v& lhs, const float_v& rhs) { return _mm256_mul_ps(lhs,rhs); }
      friend inline float_v operator/(const float_v& lhs, const float_v& rhs) { return _mm256_div_ps(lhs,rhs); }
      friend inline float_v operator-(const float_v& x) { return -1.f * x; }

      friend inline float_v operator&(const float_v& lhs, const float_v& rhs) { return _mm256_and_ps(lhs,rhs); }
      friend inline float_v operator|(const float_v& lhs, const float_v& rhs) { return _mm256_or_ps(lhs,rhs); }
      friend inline float_v operator^(const float_v& lhs, const float_v& rhs) { return _mm256_xor_ps(lhs,rhs); }

      friend inline float_v operator&&(const float_v& lhs, const float_v& rhs) { return _mm256_and_ps(lhs,rhs); }
      friend inline float_v operator||(const float_v& lhs, const float_v& rhs) { return _mm256_or_ps(lhs,rhs); }
      friend inline float_v operator!(const float_v& x) { return x ^ _mm256_castsi256_ps( _mm256_set1_epi32( -1 ) ); }

      friend inline float_v min(const float_v& lhs, const float_v& rhs) { return _mm256_min_ps(lhs, rhs); }
      friend inline float_v max(const float_v& lhs, const float_v& rhs) { return _mm256_max_ps(lhs, rhs); }
      friend inline float_v abs(const float_v& v) { return v & _mm256_castsi256_ps( _mm256_set1_epi32( 0x7FFFFFFF ) ); }
      friend inline float_v copysign(const float_v& x, const float_v& y) { return x ^ ( y & _mm256_castsi256_ps( _mm256_set1_epi32( 0x80000000 ) ) ) ; }

      friend inline float_v signselect(const float_v& s, const float_v& a, const float_v&b) { return _mm256_blendv_ps( a, b, s ); }
      friend inline float_v select(__m256 mask, const float_v& a, const float_v&b) { return _mm256_blendv_ps( b, a, mask ); }

      friend inline float_v sqrt(const float_v& v) { return _mm256_sqrt_ps(v); }

      friend inline float_v operator<(const float_v& lhs, const float_v& rhs) { return _mm256_cmp_ps(lhs, rhs, _CMP_LT_OS); }
      friend inline float_v operator>(const float_v& lhs, const float_v& rhs) { return _mm256_cmp_ps(lhs, rhs, _CMP_GT_OS); }
      friend inline float_v operator==(const float_v& lhs, const float_v& rhs) { return _mm256_cmp_ps(lhs, rhs, _CMP_EQ_OS); }

      friend inline bool all(const float_v& mask) { return _mm256_movemask_ps(mask)==0xFF; }
      friend inline bool none(const float_v& mask) { return _mm256_movemask_ps(mask)==0x00; }
      friend inline bool any(const float_v& mask) { return _mm256_movemask_ps(mask)!=0x00; }

      inline float hmax() const { 
        __m128 r = _mm_max_ps( _mm256_extractf128_ps( data, 0 ), _mm256_extractf128_ps( data, 1 ) );
        r        = _mm_max_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r        = _mm_max_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_cvtss_f32( r );
      }
      inline float hmin() const {
        __m128 r = _mm_min_ps( _mm256_extractf128_ps( data, 0 ), _mm256_extractf128_ps( data, 1 ) );
        r        = _mm_min_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r        = _mm_min_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_cvtss_f32( r );
      }
      inline float hadd() const {
        __m128 r = _mm_add_ps( _mm256_extractf128_ps( data, 0 ), _mm256_extractf128_ps( data, 1 ) );
        r        = _mm_add_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r        = _mm_add_ps( r, _mm_shuffle_ps( r, r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_cvtss_f32( r );
      }
      inline float hmax(const __m256 mask) const { return select(mask, *this, std::numeric_limits<float>::min()).hmax(); }
      inline float hmin(const __m256 mask) const { return select(mask, *this, std::numeric_limits<float>::max()).hmin(); }
      inline float hadd(const __m256 mask) const { return select(mask, *this, 0.f).hadd(); }

      private:
      __m256 data;
  };

  class int_v {
    public:
      template<int d = 0>
      constexpr int_v() : data(_mm256_set1_epi32(d))  {}
      int_v( int  f ) : data( _mm256_set1_epi32(f) ) { }
      int_v( const int* f ) : data( _mm256_loadu_si256((__m256i*)f) ) { }
      constexpr int_v( __m256i f ) : data( f ) { }

      inline int_v& operator=( const __m256i& f ) {
          data = f;
          return *this;
      }

      inline operator __m256i() const { return data; }
      inline operator float_v() const { return float_v(_mm256_cvtepi32_ps(data)); }

      inline void store(int* ptr) const {
        _mm256_storeu_si256((__m256i*)ptr, data);
      }

      inline void compressstore( __m256 mask, int* ptr ) const {
        __m256i perm = _mm256_load_si256( (const __m256i*)compress_mask256_epi32 + ( _mm256_movemask_ps(mask) ^ 0xFF ) );
        _mm256_storeu_si256( (__m256i*)ptr, _mm256_permutevar8x32_epi32( data, perm ) );
      }

      inline int hmax() const {
        __m128i r = _mm_max_epi32( _mm256_extractf128_si256( data, 0 ), _mm256_extractf128_si256( data, 1 ) );
        r         = _mm_max_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r         = _mm_max_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_extract_epi32( r, 0 );
      }
      inline int hmin() const {
        __m128i r = _mm_min_epi32( _mm256_extractf128_si256( data, 0 ), _mm256_extractf128_si256( data, 1 ) );
        r         = _mm_min_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r         = _mm_min_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_extract_epi32( r, 0 );
      }
      inline int hadd() const {
        __m128i r = _mm_add_epi32( _mm256_extractf128_si256( data, 0 ), _mm256_extractf128_si256( data, 1 ) );
        r         = _mm_add_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 2, 3, 0, 1 ) ) );
        r         = _mm_add_epi32( r, _mm_shuffle_epi32( r, _MM_SHUFFLE( 1, 0, 3, 2 ) ) );
        return _mm_extract_epi32( r, 0 );
      }
      inline int hmax(const __m256 mask) const { return select(mask, *this, std::numeric_limits<int>::min()).hmax(); }
      inline int hmin(const __m256 mask) const { return select(mask, *this, std::numeric_limits<int>::max()).hmin(); }
      inline int hadd(const __m256 mask) const { return select(mask, *this, 0).hadd(); }

      friend inline int_v operator+(const int_v& lhs, const int_v& rhs) { return _mm256_add_epi32(lhs,rhs); }
      friend inline int_v operator-(const int_v& lhs, const int_v& rhs) { return _mm256_sub_epi32(lhs,rhs); }
      friend inline int_v operator*(const int_v& lhs, const int_v& rhs) { return _mm256_mul_epi32(lhs,rhs); }

      friend inline int_v operator&(const int_v& lhs, const int_v& rhs) { return _mm256_and_si256(lhs,rhs); }
      friend inline int_v operator|(const int_v& lhs, const int_v& rhs) { return _mm256_or_si256(lhs,rhs); }

      friend inline int_v operator<<(const int_v& lhs, const int_v& rhs) { return _mm256_sllv_epi32(lhs, rhs); }
      friend inline int_v operator>>(const int_v& lhs, const int_v& rhs) { return _mm256_srlv_epi32(lhs, rhs); }

      friend inline int_v signselect(const float_v& s, const int_v& a, const int_v&b) {
          return _mm256_castps_si256(_mm256_blendv_ps( _mm256_castsi256_ps(a), _mm256_castsi256_ps(b), s ));
      }
      friend inline int_v select(const __m256 mask, const int_v& a, const int_v&b) {
          return _mm256_castps_si256(_mm256_blendv_ps( _mm256_castsi256_ps(b), _mm256_castsi256_ps(a), mask ));
      }

      friend inline float_v operator<(const int_v& lhs, const int_v& rhs) { return _mm256_castsi256_ps(_mm256_cmpgt_epi32(rhs, lhs)); }
      friend inline float_v operator>(const int_v& lhs, const int_v& rhs) { return _mm256_castsi256_ps(_mm256_cmpgt_epi32(lhs, rhs)); }
      friend inline float_v operator==(const int_v& lhs, const int_v& rhs) { return _mm256_castsi256_ps(_mm256_cmpeq_epi32(lhs, rhs)); }

      private:
      __m256i data;
  };

  inline float_v::operator int_v() const { return int_v(_mm256_cvttps_epi32(data)); }


  inline int_v castToInt(const float_v& x) {
    return int_v(_mm256_castps_si256(x));
  }

  inline float_v castToFloat(const int_v& x) {
    return float_v(_mm256_castsi256_ps(x));
  }

  inline int_v gather(const int*base, const int_v& idx) {
    return _mm256_i32gather_epi32(base, idx, sizeof(int));
  }

  inline float_v gather(const float*base, const int_v& idx) {
    return _mm256_i32gather_ps(base, idx, sizeof(float));
  }


  struct types {
    static const size_t size = 8;
    using int_v = avx2::int_v;
    using float_v = avx2::float_v;
    using mask_v = avx2::float_v;
    static int_v indices() { return _mm256_setr_epi32(0,1,2,3,4,5,6,7); }
    static int_v indices(int start) { return indices() + start; }
    static int popcount( __m256 mask )  {
      return _mm_popcnt_u32( _mm256_movemask_ps( mask ) );
    }
    static float_v loop_mask( int i, int n ) {
      return _mm256_cmp_ps( _mm256_setr_ps(0,1,2,3,4,5,6,7), _mm256_set1_ps( n - i ), _CMP_LT_OS );
    }
  };
}
#endif

#ifndef __AVX512F__
namespace avx512 {
  using float_v = avx2::float_v;
  using int_v   = avx2::int_v;
  using types   = avx2::types;
}
#else
namespace avx512 {
  class mask_v {
    public:
      mask_v();
      mask_v(__mmask16 m) : data(m) {}

      inline mask_v& operator=( const __mmask16& m) {
        data = m;
        return *this;
      }

      inline operator __mmask16() const { return data; }

      friend inline mask_v operator&&(const mask_v& lhs, const mask_v& rhs) { return lhs & rhs; }
      friend inline mask_v operator||(const mask_v& lhs, const mask_v& rhs) { return lhs | rhs; }
      friend inline mask_v operator!(const mask_v& x) { return ~x; }

      friend inline bool all(const mask_v& mask) { return mask==0xFFFF; }
      friend inline bool none(const mask_v& mask) { return mask==0x0000; }
      friend inline bool any(const mask_v& mask) { return mask!=0x0000; }
    private:
      __mmask16 data;
  };

  class int_v;

  class float_v {
    public:
      float_v();
      float_v( scalar::float_v& f) : data(  _mm512_set1_ps( f.cast() ) ) {}
      float_v( float  f ) : data( _mm512_set1_ps(f) ) { }
      float_v( const float* f ) : data( _mm512_load_ps(f) ) { }
      float_v( __m512 f ) : data( f ) { }

      inline float_v& operator=( const __m512& f ) {
        data = f;
        return *this;
      }

      inline operator __m512() const { return data; }
      inline operator int_v() const;

      inline void store(float* ptr) const {
        _mm512_storeu_ps(ptr, data);
      }

      inline void compressstore(const mask_v& mask, float* ptr) const {
        _mm512_mask_compressstoreu_ps(ptr, mask, data);
      }

      inline float hmax() const { return _mm512_reduce_max_ps(data); }
      inline float hmin() const { return _mm512_reduce_min_ps(data); }
      inline float hadd() const { return _mm512_reduce_add_ps(data); }
      inline float hmax(const mask_v& mask) const { return _mm512_mask_reduce_max_ps(mask, data); }
      inline float hmin(const mask_v& mask) const { return _mm512_mask_reduce_min_ps(mask, data); }
      inline float hadd(const mask_v& mask) const { return _mm512_mask_reduce_add_ps(mask, data); }

      friend inline float_v operator+(const float_v& lhs, const float_v& rhs) { return _mm512_add_ps(lhs,rhs); }
      friend inline float_v operator-(const float_v& lhs, const float_v& rhs) { return _mm512_sub_ps(lhs,rhs); }
      friend inline float_v operator*(const float_v& lhs, const float_v& rhs) { return _mm512_mul_ps(lhs,rhs); }
      friend inline float_v operator/(const float_v& lhs, const float_v& rhs) { return _mm512_div_ps(lhs,rhs); }
      friend inline float_v operator-(const float_v& x) { return -1.f * x; }

      friend inline float_v operator&(const float_v& lhs, const float_v& rhs) { return _mm512_and_ps(lhs,rhs); }
      friend inline float_v operator|(const float_v& lhs, const float_v& rhs) { return _mm512_or_ps(lhs,rhs); }
      friend inline float_v operator^(const float_v& lhs, const float_v& rhs) { return _mm512_xor_ps(lhs,rhs); }

      friend inline float_v operator&&(const float_v& lhs, const float_v& rhs) { return _mm512_and_ps(lhs,rhs); }
      friend inline float_v operator||(const float_v& lhs, const float_v& rhs) { return _mm512_or_ps(lhs,rhs); }
      friend inline float_v operator!(const float_v& x) { return x ^ _mm512_castsi512_ps( _mm512_set1_epi32( -1 ) ); }

      friend inline float_v min(const float_v& lhs, const float_v& rhs) { return _mm512_min_ps(lhs, rhs); }
      friend inline float_v max(const float_v& lhs, const float_v& rhs) { return _mm512_max_ps(lhs, rhs); }
      friend inline float_v abs(const float_v& v) { return _mm512_abs_ps(v); }
      friend inline float_v copysign(const float_v& x, const float_v& y) { return x ^ ( y & _mm512_castsi512_ps( _mm512_set1_epi32( 0x80000000 ) ) ); }

      friend inline auto signselect(const float_v& s, const float_v& a, const float_v&b) { return _mm512_mask_mov_ps( a, s < float_v(0.f), b ); }
      friend inline auto select(const mask_v& mask, const float_v& a, const float_v&b) { return _mm512_mask_mov_ps( b, mask, a ); }

      friend inline float_v sqrt(const float_v& v) { return _mm512_sqrt_ps(v); }

      friend inline mask_v operator<(const float_v& lhs, const float_v& rhs) { return _mm512_cmp_ps_mask(lhs, rhs, _CMP_LT_OS); }
      friend inline mask_v operator>(const float_v& lhs, const float_v& rhs) { return _mm512_cmp_ps_mask(lhs, rhs, _CMP_GT_OS); }

      private:
        __m512 data;
  };

  class int_v {
    public:
      template<int d = 0>
      constexpr int_v() : data(_mm512_set1_epi32(d))  {}
      int_v( int  f ) : data( _mm512_set1_epi32(f) ) { }
      int_v( const int* f ) : data( _mm512_load_si512((__m512i*)f) ) { }
      constexpr int_v( __m512i f ) : data( f ) { }

      inline int_v& operator=( const __m512i& f ) {
        data = f;
        return *this;
      }

      inline operator __m512i() const { return data; }
      inline operator float_v() const { return float_v(_mm512_cvtepi32_ps(data)); }

      inline void store(int* ptr) const {
        _mm512_storeu_si512(ptr, data);
      }

      inline void compressstore(const mask_v& mask, int* ptr) const {
        _mm512_mask_compressstoreu_epi32(ptr, mask, data);
      }

      inline int hmax() const { return _mm512_reduce_max_epi32(data); }
      inline int hmin() const { return _mm512_reduce_min_epi32(data); }
      inline int hadd() const { return _mm512_reduce_add_epi32(data); }
      inline int hmax(const mask_v& mask) const { return _mm512_mask_reduce_max_epi32(mask, data); }
      inline int hmin(const mask_v& mask) const { return _mm512_mask_reduce_min_epi32(mask, data); }
      inline int hadd(const mask_v& mask) const { return _mm512_mask_reduce_add_epi32(mask, data); }

      friend inline int_v operator+(const int_v& lhs, const int_v& rhs) { return _mm512_add_epi32(lhs,rhs); }
      friend inline int_v operator-(const int_v& lhs, const int_v& rhs) { return _mm512_sub_epi32(lhs,rhs); }
      friend inline int_v operator*(const int_v& lhs, const int_v& rhs) { return _mm512_mul_epi32(lhs,rhs); }

      friend inline int_v operator&(const int_v& lhs, const int_v& rhs) { return _mm512_and_si512(lhs,rhs); }
      friend inline int_v operator|(const int_v& lhs, const int_v& rhs) { return _mm512_or_si512(lhs,rhs); }

      friend inline int_v signselect(const float_v& s, const int_v& a, const int_v&b) { return _mm512_mask_mov_epi32( a, s < float_v(0.f), b ); }
      friend inline int_v select(const mask_v& mask, const int_v& a, const int_v&b) { return _mm512_mask_mov_epi32( b, mask, a ); }

      friend inline mask_v operator<(const int_v& lhs, const int_v& rhs) { return _mm512_cmplt_epi32_mask(rhs, lhs); }
      friend inline mask_v operator>(const int_v& lhs, const int_v& rhs) { return _mm512_cmpgt_epi32_mask(lhs, rhs); }
      friend inline mask_v operator==(const int_v& lhs, const int_v& rhs) { return _mm512_cmpeq_epi32_mask(lhs, rhs); }

      private:
      __m512i data;
  };

  inline float_v::operator int_v() const { return int_v(_mm512_cvttps_epi32(data)); }

  inline int_v castToInt(const float_v& x) {
    return int_v(_mm512_castps_si512(x));
  }

  inline float_v castToFloat(const int_v& x) {
    return float_v(_mm512_castsi512_ps(x));
  }

  inline int_v gather(const int*base, const int_v& idx) {
    return _mm512_i32gather_epi32(idx, base, sizeof(int));
  }

  inline float_v gather(const float*base, const int_v& idx) {
    return _mm512_i32gather_ps(idx, base, sizeof(float));
  }


  struct types {
    static const size_t size = 16;
    using int_v = avx512::int_v;
    using float_v = avx512::float_v;
    using mask_v = avx512::mask_v;
    static int_v indices() { return _mm512_setr_epi32(0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15); }
    static int_v indices(int start) { return indices() + start; }
    static int popcount(mask_v mask)  {
      return _mm_popcnt_u32(mask);
    }
    static mask_v loop_mask(int i, int n) { return ( ( i + 16 ) > n ) ? ~(0xFFFF << ( n & 15 )) : 0xFFFF; }
  };
}
#endif
}

// Helper to build SIMDWrapper accessors:
#define SOA_ACCESSOR( name, location ) \
  template<typename T> \
  inline auto name( const int key ) const { return T(location + key); }\
  template<typename T> \
  inline auto gather_##name( const T& key ) const { return gather(location, key); }\
  template<typename T> \
  inline auto store_##name( const int key, const T& v) { v.store(location + key); }\
  template<typename T, typename MaskT> \
  inline auto compressstore_##name( const int key, MaskT mask, const T& v) { v.compressstore(mask, location + key); }

// With variadic parameters:
#define SOA_ACCESSOR_VAR( name, location, ... ) \
  template<typename T> \
  inline auto name( const int key, __VA_ARGS__ ) const { return T(location + key); }\
  template<typename T> \
  inline auto gather_##name( const T& key, __VA_ARGS__ ) const { return gather(location, key); }\
  template<typename T> \
  inline auto store_##name( const int key, __VA_ARGS__, const T& v) { v.store(location + key); }\
  template<typename T, typename MaskT> \
  inline auto compressstore_##name( const int key, __VA_ARGS__, MaskT mask, const T& v) { v.compressstore(mask, location + key); }

// Align size to AVX512 boundary
constexpr int align_size(int n) {
  return (n / 16 + 1) * 16;
}

// Maths :

template<typename T>
inline T faster_atan2(const T& y, const T& x ) { // error < 0.07 rad, no 0/0 security
  const T c1 = M_PI / 4.0;
  const T c2 = 3.0 * M_PI / 4.0;
  T abs_y    = abs( y );

  T x_plus_y = x + abs_y;
  T x_sub_y  = x - abs_y;
  T y_sub_x  = abs_y - x;

  T nom = signselect( x, x_sub_y, x_plus_y );
  T den = signselect( x, x_plus_y, y_sub_x );

  T r     = nom / den;
  T angle = signselect( x, c1, c2 ) - c1 * r;

  return copysign(angle, y);
}

template<typename T>
inline T approx_log2(const T& x) {
  auto vx = castToInt(x);
  auto mx = castToFloat((vx & 0x007FFFFF) | 0x3F000000);
  auto y = T(vx) * 1.1920928955078125e-7f;
  return y - 124.22551499f - 1.498030302f * mx - 1.72587999f / ( 0.3520887068f + mx );
}

template<typename T>
inline T approx_log(const T& x) {
  return approx_log2(x) * 0.69314718f;
}

template<typename T>
inline T vapprox_log(const T& x) {
  return castToInt(x) * 8.2629582881927490e-8f - 87.989971088f;
}

template<typename M, typename T>
inline void swap(const M& mask, T& a, T& b) {
  T tmp = select(mask, b, a);
  b     = select(mask, a, b);
  a     = tmp;
}

