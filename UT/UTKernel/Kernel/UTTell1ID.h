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
#ifndef _UTTell1ID_H
#define _UTTell1ID_H 1

/** @class UTTell1ID UTTell1ID.h "UTDAQ/UTTell1ID.h"
 *
 *  Class to describe a UTTell1ID
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

#include <iostream>

class UTTell1ID final {

public:
  /// constructor with strip and waferIndex
  UTTell1ID( unsigned int aRegion, unsigned int aSubID ) {

    m_id   = ( aRegion << regionBits ) + ( aSubID << subIDBits );
    m_isUT = false;
  }

  UTTell1ID( unsigned int aRegion, unsigned int aSubID, bool aIsUT ) {
    m_isUT = aIsUT;
    if ( m_isUT )
      m_id = ( aRegion << regionBitsUT ) + ( aSubID << subIDBits );
    else
      m_id = ( aRegion << regionBits ) + ( aSubID << subIDBits );
  }

  explicit UTTell1ID( unsigned int id ) : m_id( id ) { m_isUT = false; }

  explicit UTTell1ID( unsigned int id, bool isUT ) : m_id( id ) { m_isUT = isUT; }

  /// Default Constructor
  UTTell1ID() = default;

  /// wafer
  unsigned int region() const;

  /// strip
  unsigned int subID() const;

  /// cast
  //  operator int() const;

  /// comparison equality
  friend bool operator==( const UTTell1ID& lhs, const UTTell1ID& rhs ) { return lhs.id() == rhs.id(); }

  /// comparison <
  friend bool operator<( const UTTell1ID& lhs, const UTTell1ID& rhs ) { return lhs.id() < rhs.id(); }

  /// Retrieve UT Channel ID
  unsigned int id() const;

  /// Retrieve if it is UT
  unsigned int isUT() const;

  /// Operator overloading for stringoutput
  friend std::ostream& operator<<( std::ostream& s, const UTTell1ID& obj ) { return obj.fillStream( s ); }

  // Fill the ASCII output stream
  std::ostream& fillStream( std::ostream& s ) const;

  /** print method for python Not needed in C++ */
  std::string toString() const;

  enum General { nullBoard = 0x0000ffff };

private:
  enum bits { subIDBits = 0, regionBits = 5, regionBitsUT = 6 }; /// Enumeration to store the bit packing offsets
  enum masks { subIDMask = 0x0000001f, regionMask = 0x000000e0, subIDMaskUT = 0x0000003f, regionMaskUT = 0x000000c0 };

  unsigned int m_id   = 0; /// UTell1ID
  bool         m_isUT = false;
};

#include <sstream>
#include <string>

inline std::string UTTell1ID::toString() const {
  std::ostringstream o;
  fillStream( o );
  return o.str();
}

inline unsigned int UTTell1ID::id() const { return m_id; }

inline unsigned int UTTell1ID::isUT() const { return m_isUT; }

inline unsigned int UTTell1ID::region() const {
  return isUT() ? ( ( m_id & regionMask ) >> regionBits ) : ( ( m_id & regionMaskUT ) >> regionBitsUT );
}

inline unsigned int UTTell1ID::subID() const {
  return isUT() ? ( ( m_id & subIDMask ) >> subIDBits ) : ( ( m_id & subIDMaskUT ) >> subIDBits );
}

inline std::ostream& UTTell1ID::fillStream( std::ostream& s ) const {
  s << "{ "
    << " UTTell1ID:\t" << id() << std::endl
    << " region:\t" << region() << std::endl
    << " subID:\t" << subID();
  if ( isUT() )
    s << " isUT } ";
  else
    s << " } ";

  return s;
}

#endif // _UTTell1ID_H
