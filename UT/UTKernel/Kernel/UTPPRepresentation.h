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
#ifndef _UTDAQ_UTPPRepresentation_H
#define _UTDAQ_UTPPRepresentation_H 1

/** @class PPRepresentaiton UTPPRepresentation.h  UTDAQ/UTPPRepresentation
 *
 * Sometimes we number a strip on a tell1 board 0 - 3072
 * Sometimes we want to compose/decompose this into Beetles and ports
 * This helper class does just that.
 *
 *  @author Andy Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

#include "Kernel/LHCbConstants.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTStripRepresentation.h"
#include <iostream>

namespace UTDAQ {

  class UTPPRepresentation {

  public:
    /** constructor from:
     * @param unsigned int pp [0 - 3]
     * @param unsigned int beetle [0 - 5]
     * @param unsigned int port [0 - 3]
     * @param unsigned int strip [0 - 31]
     **/
    UTPPRepresentation( unsigned int pp, unsigned int beetle = 0, unsigned int port = 0, unsigned int strip = 0 );

    /** constructor  from int */
    explicit UTPPRepresentation( const UTDAQ::UTStripRepresentation& strip );

    /** destructor */
    ~UTPPRepresentation() {}

    /** decompose into beetle, port, strip **/
    void decompose( unsigned int& pp, unsigned int& beetle, unsigned int& port, unsigned int& strip ) const;

    /** cast to int */
    operator int() const;

    /** return the value */
    unsigned int value() const;

    /** value as UTStripRepresentation */
    UTDAQ::UTStripRepresentation toUTStripRepresentation() const;

    /** Operator overloading for stringoutput */
    friend std::ostream& operator<<( std::ostream& s, const UTDAQ::UTPPRepresentation& obj ) {
      return obj.fillStream( s );
    }

    /** Fill the ASCII output stream **/
    std::ostream& fillStream( std::ostream& s ) const;

    /** print method for python, not needed in C++ **/
    std::string toString() const;

  private:
    unsigned int m_value;
  };

} // namespace UTDAQ

//#include "Kernel/LHCbConstants.h"

inline UTDAQ::UTPPRepresentation::UTPPRepresentation( unsigned int pp, unsigned int beetle, unsigned int port,
                                                      unsigned int strip ) {
  m_value = ( UTDAQ::nStripPerPPx * pp ) + ( LHCbConstants::nStripsInBeetle * beetle ) +
            ( LHCbConstants::nStripsInPort * port ) + strip;
}

inline UTDAQ::UTPPRepresentation::UTPPRepresentation( const UTDAQ::UTStripRepresentation& strip )
    : m_value( strip.value() ) {
  // constructor
}

inline UTDAQ::UTPPRepresentation::operator int() const { return m_value; }

inline unsigned int UTDAQ::UTPPRepresentation::value() const { return m_value; }

inline void UTDAQ::UTPPRepresentation::decompose( unsigned int& pp, unsigned int& beetle, unsigned int& port,
                                                  unsigned int& strip ) const {
  pp                         = m_value / UTDAQ::nStripPerPPx;
  beetle                     = m_value / LHCbConstants::nStripsInBeetle;
  const unsigned int ppStrip = m_value % UTDAQ::nStripPerPPx;
  beetle                     = ppStrip / LHCbConstants::nStripsInBeetle;
  const unsigned beetleStrip = ppStrip % LHCbConstants::nStripsInBeetle;
  port                       = beetleStrip / LHCbConstants::nStripsInPort;
  strip                      = beetleStrip % LHCbConstants::nStripsInPort;
}

inline UTDAQ::UTStripRepresentation UTDAQ::UTPPRepresentation::toUTStripRepresentation() const {
  return UTDAQ::UTStripRepresentation( m_value );
}

inline std::ostream& UTDAQ::UTPPRepresentation::fillStream( std::ostream& s ) const {
  unsigned int pp, beetle, port, strip;
  decompose( pp, beetle, port, strip );
  s << "{ "
    << " UTPPRepresentation: \t" << value() << std::endl
    << " PP " << pp << " Beetle: " << beetle << " Port: " << port << "Strip: " << strip << std::endl
    << "  } ";

  return s;
}

#include <sstream>

inline std::string UTDAQ::UTPPRepresentation::toString() const {
  std::ostringstream o;
  fillStream( o );
  return o.str();
}

#endif //  _UTDAQ_UTPPRepresentation_H
