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
#ifndef _STTell1ID_H
#define _STTell1ID_H 1

/** @class STTell1ID STTell1ID.h "STDAQ/STTell1ID.h"
 *
 *  Class to describe a STTell1ID
 *
 *  @author M.Needham
 *  @date   7/01/2004
 */

#include <iostream>

class STTell1ID final {

public:
  /// constructor with strip and waferIndex
  STTell1ID( unsigned int aRegion, unsigned int aSubID ) {

    m_id   = ( aRegion << regionBits ) + ( aSubID << subIDBits );
    m_isUT = false;
  }

  STTell1ID( unsigned int aRegion, unsigned int aSubID, bool aIsUT ) {
    m_isUT = aIsUT;
    if ( m_isUT )
      m_id = ( aRegion << regionBitsUT ) + ( aSubID << subIDBits );
    else
      m_id = ( aRegion << regionBits ) + ( aSubID << subIDBits );
  }

  explicit STTell1ID( unsigned int id ) : m_id( id ) { m_isUT = false; }

  explicit STTell1ID( unsigned int id, bool isUT ) : m_id( id ) { m_isUT = isUT; }

  /// Default Constructor
  STTell1ID() = default;

  /// wafer
  unsigned int region() const;

  /// strip
  unsigned int subID() const;

  /// cast
  //  operator int() const;

  /// comparison equality
  friend bool operator==( const STTell1ID& lhs, const STTell1ID& rhs ) { return lhs.id() == rhs.id(); }

  /// comparison <
  friend bool operator<( const STTell1ID& lhs, const STTell1ID& rhs ) { return lhs.id() < rhs.id(); }

  /// Retrieve IT Channel ID
  unsigned int id() const;

  /// Retrieve if it is UT
  unsigned int isUT() const;

  /// Operator overloading for stringoutput
  friend std::ostream& operator<<( std::ostream& s, const STTell1ID& obj ) { return obj.fillStream( s ); }

  // Fill the ASCII output stream
  std::ostream& fillStream( std::ostream& s ) const;

  /** print method for python Not needed in C++ */
  std::string toString() const;

  enum General { nullBoard = 0x0000ffff };

private:
  enum bits { subIDBits = 0, regionBits = 5, regionBitsUT = 6 }; /// Enumeration to store the bit packing offsets
  enum masks { subIDMask = 0x0000001f, regionMask = 0x000000e0, subIDMaskUT = 0x0000003f, regionMaskUT = 0x000000c0 };

  unsigned int m_id   = 0; /// STell1ID
  bool         m_isUT = false;
};

#include <sstream>
#include <string>

inline std::string STTell1ID::toString() const {
  std::ostringstream o;
  fillStream( o );
  return o.str();
}

inline unsigned int STTell1ID::id() const { return m_id; }

inline unsigned int STTell1ID::isUT() const { return m_isUT; }

inline unsigned int STTell1ID::region() const {
  return isUT() ? ( ( m_id & regionMask ) >> regionBits ) : ( ( m_id & regionMaskUT ) >> regionBitsUT );
}

inline unsigned int STTell1ID::subID() const {
  return isUT() ? ( ( m_id & subIDMask ) >> subIDBits ) : ( ( m_id & subIDMaskUT ) >> subIDBits );
}

inline std::ostream& STTell1ID::fillStream( std::ostream& s ) const {
  s << "{ "
    << " STTell1ID:\t" << id() << std::endl
    << " region:\t" << region() << std::endl
    << " subID:\t" << subID();
  if ( isUT() )
    s << " isUT } ";
  else
    s << " } ";

  return s;
}

#endif // _STTell1ID_H
