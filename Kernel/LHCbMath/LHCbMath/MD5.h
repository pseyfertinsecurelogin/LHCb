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
#ifndef MD5_H
#define MD5_H 1
#include <algorithm>
#include <array>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <sstream>
// ============================================================================
/** @file MD5.h
 *
 *
 *  @author Gerhard Raven Gerhard.Raven@cern.ch
 *  @date 2008-05-22
 */
// ============================================================================

namespace Gaudi::Math {

  class MD5 {
    class md5_engine;

  public:
    using value_type = std::array<uint8_t, 16>;

    /// create an (recognizable!) invalid digest...
    constexpr MD5() = default;
    static MD5 createInvalid() { return MD5(); }

    /// compute MD5 digest for the specified string
    static MD5 compute( const std::string& s );

    /// compute MD5 digest for the provided object by first streaming it to a string
    template <typename T>
    static MD5 compute( const T& t ) {
      std::ostringstream x;
      x << t;
      return compute( x.str() );
    }

    /// provide a hex string representation of the (internal) 128 bit value of the computed digest
    std::string str() const;

    /// convert the hex string representation back to a full-blown instance
    static MD5 createFromStringRep( const std::string& s );

    /// check whether the digest is the (valid) result of an MD5 computation
    bool invalid() const {
      return std::all_of( std::begin( m_value ), std::end( m_value ), []( uint8_t i ) { return i == 0; } );
    }
    bool valid() const { return !invalid(); }

    explicit operator bool() const { return valid(); }

    /// binary comparisons
    friend bool operator<( const MD5& lhs, const MD5& rhs ) { return lhs.m_value < rhs.m_value; }
    friend bool operator>( const MD5& lhs, const MD5& rhs ) { return lhs.m_value > rhs.m_value; }
    friend bool operator==( const MD5& lhs, const MD5& rhs ) { return lhs.m_value == rhs.m_value; }

    friend bool operator<=( const MD5& lhs, const MD5& rhs ) { return !operator>( lhs, rhs ); }
    friend bool operator>=( const MD5& lhs, const MD5& rhs ) { return !operator<( lhs, rhs ); }
    friend bool operator!=( const MD5& lhs, const MD5& rhs ) { return !operator==( lhs, rhs ); }

  private:
    explicit MD5( const value_type& val )
        : m_value{val[0], val[1], val[2],  val[3],  val[4],  val[5],  val[6],  val[7],
                  val[8], val[9], val[10], val[11], val[12], val[13], val[14], val[15]} {}

    value_type m_value{0};
  };

  std::ostream& operator<<( std::ostream& os, const Gaudi::Math::MD5& x );

} // namespace Gaudi::Math

#endif
