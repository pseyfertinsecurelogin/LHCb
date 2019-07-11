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

#include "SIMDWrapper.h"

/**
 * Vec3 template class for handling Vector math on any base type.
 *
 * For example, if the template parameter is one of the float_v defined
 * in SIMDWrapper.h the constructor can take any combination of float, float*
 * or float_v.
 *
 * This class is intended as a Proxy class to ease the usage of SoA PoD format.
 * This class should not serve as a storage class.
 *
 * @author: Arthur Hennequin
 */

template <typename T>
class Vec3 {
public:
  T x, y, z;

  Vec3() {}

  template <typename Tx, typename Ty, typename Tz>
  Vec3( Tx x, Ty y, Tz z ) : x( x ), y( y ), z( z ) {}

  friend Vec3<T> operator+( const Vec3<T>& a, const Vec3<T>& b ) { return {a.x + b.x, a.y + b.y, a.z + b.z}; }
  friend Vec3<T> operator-( const Vec3<T>& a, const Vec3<T>& b ) { return {a.x - b.x, a.y - b.y, a.z - b.z}; }
  friend Vec3<T> operator*( const Vec3<T>& a, const Vec3<T>& b ) { return {a.x * b.x, a.y * b.y, a.z * b.z}; }
  inline Vec3<T> cross( const Vec3<T>& b ) const { return {y * b.z - z * b.y, z * b.x - x * b.z, x * b.y - y * b.x}; }
  inline T       dot( const Vec3<T>& b ) const { return x * b.x + y * b.y + z * b.z; }
  inline T       mag2() const { return dot( *this ); }
  inline T       perp2() const { return x * x + y * y; }
  inline T       mag() const { return sqrt( mag2() ); }
  inline T       rho() const { return sqrt( perp2() ); }
  inline T       theta() const { return faster_atan2( rho(), z ); }
  inline T       phi() const { return faster_atan2( y, x ); }

  // Compatibility with ROOT::Math::XYZVector and friends
  T       X() const { return x; }
  T       Y() const { return y; }
  T       Z() const { return z; }
  Vec3<T> Cross( Vec3<T> const& b ) const { return cross( b ); }

  inline T eta() const {
    auto zs = z / rho();
    return approx_log<T>( zs + sqrt( zs * zs + 1 ) );
  }
};

#define VEC3_SOA_ACCESSOR( name, locX, locY, locZ )                                                                    \
  template <typename T>                                                                                                \
  inline Vec3<T> name( const int key ) const {                                                                         \
    return {locX + key, locY + key, locZ + key};                                                                       \
  }                                                                                                                    \
  template <typename T, typename I>                                                                                    \
  inline Vec3<T> gather_##name( const I& key ) const {                                                                 \
    return {gather( locX, key ), gather( locY, key ), gather( locZ, key )};                                            \
  }                                                                                                                    \
  template <typename T, typename I, typename MaskT>                                                                    \
  inline Vec3<T> maskgather_##name( const I& key, const MaskT& mask, const T& src ) const {                            \
    return {maskgather( locX, key, mask, src ), maskgather( locY, key, mask, src ),                                    \
            maskgather( locZ, key, mask, src )};                                                                       \
  }                                                                                                                    \
  template <typename T>                                                                                                \
  inline void store_##name( const int key, const Vec3<T>& v ) const {                                                  \
    v.x.store( locX + key );                                                                                           \
    v.y.store( locY + key );                                                                                           \
    v.z.store( locZ + key );                                                                                           \
  }                                                                                                                    \
  template <typename T, typename MaskT>                                                                                \
  inline void compressstore_##name( const int key, MaskT mask, const Vec3<T>& v ) const {                              \
    v.x.compressstore( mask, locX + key );                                                                             \
    v.y.compressstore( mask, locY + key );                                                                             \
    v.z.compressstore( mask, locZ + key );                                                                             \
  }

#define VEC3_XY_SOA_ACCESSOR( name, locX, locY, valZ )                                                                 \
  template <typename T>                                                                                                \
  inline Vec3<T> name( const int key ) const {                                                                         \
    return {locX + key, locY + key, valZ};                                                                             \
  }                                                                                                                    \
  template <typename T, typename I>                                                                                    \
  inline Vec3<T> gather_##name( const I& key ) const {                                                                 \
    return {gather( locX, key ), gather( locY, key ), valZ};                                                           \
  }                                                                                                                    \
  template <typename T, typename I, typename MaskT>                                                                    \
  inline Vec3<T> maskgather_##name( const I& key, const MaskT& mask, const T& src ) const {                            \
    return {maskgather( locX, key, mask, src ), maskgather( locY, key, mask, src ), valZ};                             \
  }                                                                                                                    \
  template <typename T>                                                                                                \
  inline void store_##name( const int key, const Vec3<T>& v ) const {                                                  \
    v.x.store( locX + key );                                                                                           \
    v.y.store( locY + key );                                                                                           \
  }                                                                                                                    \
  template <typename T, typename MaskT>                                                                                \
  inline void compressstore_##name( const int key, MaskT mask, const Vec3<T>& v ) const {                              \
    v.x.compressstore( mask, locX + key );                                                                             \
    v.y.compressstore( mask, locY + key );                                                                             \
  }

// Variadic accessors:

#define VEC3_SOA_ACCESSOR_VAR( name, locX, locY, locZ, ... )                                                           \
  template <typename T>                                                                                                \
  inline Vec3<T> name( const int key, __VA_ARGS__ ) const {                                                            \
    return {locX + key, locY + key, locZ + key};                                                                       \
  }                                                                                                                    \
  template <typename T, typename I>                                                                                    \
  inline Vec3<T> gather_##name( const I& key, __VA_ARGS__ ) const {                                                    \
    return {gather( locX, key ), gather( locY, key ), gather( locZ, key )};                                            \
  }                                                                                                                    \
  template <typename T, typename I, typename MaskT>                                                                    \
  inline Vec3<T> maskgather_##name( const I& key, const MaskT& mask, const T& src, __VA_ARGS__ ) const {               \
    return {maskgather( locX, key, mask, src ), maskgather( locY, key, mask, src ),                                    \
            maskgather( locZ, key, mask, src )};                                                                       \
  }                                                                                                                    \
  template <typename T>                                                                                                \
  inline void store_##name( const int key, __VA_ARGS__, const Vec3<T>& v ) const {                                     \
    v.x.store( locX + key );                                                                                           \
    v.y.store( locY + key );                                                                                           \
    v.z.store( locZ + key );                                                                                           \
  }                                                                                                                    \
  template <typename T, typename MaskT>                                                                                \
  inline void compressstore_##name( const int key, __VA_ARGS__, MaskT mask, const Vec3<T>& v ) const {                 \
    v.x.compressstore( mask, locX + key );                                                                             \
    v.y.compressstore( mask, locY + key );                                                                             \
    v.z.compressstore( mask, locZ + key );                                                                             \
  }

#define VEC3_XY_SOA_ACCESSOR_VAR( name, locX, locY, valZ, ... )                                                        \
  template <typename T>                                                                                                \
  inline Vec3<T> name( const int key, __VA_ARGS__ ) const {                                                            \
    return {locX + key, locY + key, valZ};                                                                             \
  }                                                                                                                    \
  template <typename T, typename I>                                                                                    \
  inline Vec3<T> gather_##name( const I& key, __VA_ARGS__ ) const {                                                    \
    return {gather( locX, key ), gather( locY, key ), valZ};                                                           \
  }                                                                                                                    \
  template <typename T, typename I, typename MaskT>                                                                    \
  inline Vec3<T> maskgather_##name( const I& key, const MaskT& mask, const T& src, __VA_ARGS__ ) const {               \
    return {maskgather( locX, key, mask, src ), maskgather( locY, key, mask, src ), valZ};                             \
  }                                                                                                                    \
  template <typename T>                                                                                                \
  inline void store_##name( const int key, __VA_ARGS__, const Vec3<T>& v ) const {                                     \
    v.x.store( locX + key );                                                                                           \
    v.y.store( locY + key );                                                                                           \
  }                                                                                                                    \
  template <typename T, typename MaskT>                                                                                \
  inline void compressstore_##name( const int key, __VA_ARGS__, MaskT mask, const Vec3<T>& v ) const {                 \
    v.x.compressstore( mask, locX + key );                                                                             \
    v.y.compressstore( mask, locY + key );                                                                             \
  }
