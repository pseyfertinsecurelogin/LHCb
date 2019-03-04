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
#ifndef _STDAQ_StripRepresentation_H
#define _STDAQ_StripRepresentation_H 1

/** @class StripRepresentaiton StripRepresentation.h  STDAQ/StripRepresentation
 *
 * Sometimes we number a strip on a tell1 board 0 - 3072
 * This helper class does just that.
 *
 *  @author M.Needham
 *  @date   17/8/2008
 */

#include <iostream>

namespace STDAQ {

  class StripRepresentation {

  public:
    /** constructor  from int */
    explicit StripRepresentation( unsigned int value );

    /** destructor */
    ~StripRepresentation() {}

    /** cast to int */
    operator int() const;

    /** return the value */
    unsigned int value() const;

    /** Operator overloading for stringoutput */
    friend std::ostream& operator<<( std::ostream& s, const STDAQ::StripRepresentation& obj ) {
      return obj.fillStream( s );
    }

    /** Fill the ASCII output stream **/
    std::ostream& fillStream( std::ostream& s ) const;

    /** print method for python, not needed in C++ **/
    std::string toString() const;

  private:
    unsigned int m_value;
  };

} // namespace STDAQ

inline STDAQ::StripRepresentation::StripRepresentation( unsigned int value ) : m_value( value ) {
  // constructor
}

inline STDAQ::StripRepresentation::operator int() const { return m_value; }

inline unsigned int STDAQ::StripRepresentation::value() const { return m_value; }

inline std::ostream& STDAQ::StripRepresentation::fillStream( std::ostream& s ) const {

  s << "{ "
    << "Strip: " << value() << std::endl
    << "  } ";

  return s;
}

#include <sstream>

inline std::string STDAQ::StripRepresentation::toString() const {
  std::ostringstream o;
  fillStream( o );
  return o.str();
}

#endif //  _STDAQ_StripRepresentation_H
