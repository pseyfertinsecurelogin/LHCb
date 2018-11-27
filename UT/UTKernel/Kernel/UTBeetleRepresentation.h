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
#ifndef _UTDAQ_UTBeetleRepresentation_H
#define _UTDAQ_UTBeetleRepresentation_H 1


/** @class BeetleRepresentaiton UTBeetleRepresentation.h  UTDAQ/UTBeetleRepresentation
*
* Sometimes we number a strip on a tell1 board 0 - 3072
* Sometimes we want to compose/decompose this into Beetles and ports
* This helper class does just that.
*
*  @author A. Beiter (based on code by M.Needham)
*  @date   2018-09-04
*/

#include "Kernel/LHCbConstants.h"
#include <iostream>
#include "UTStripRepresentation.h"

namespace UTDAQ{

  class UTBeetleRepresentation{

  public:

    /** constructor from:
    * @param unsigned int beetle [0 - 23]
    * @param unsigned int port [0 - 3]
    * @param unsigned int strip [0 - 31]  
    **/
    UTBeetleRepresentation(unsigned int beetle,
                         unsigned int port = 0,
                         unsigned int strip = 0); 

    /** constructor  from int */
    explicit UTBeetleRepresentation(const UTStripRepresentation& strip); 

    /** destructor */
    ~UTBeetleRepresentation(){}

    /** decompose into beetle, port, strip **/
    void decompose(unsigned int& beetle,
                   unsigned int& port,
                   unsigned int& strip) const;

    /** decompose into beetle and strip **/
    void decompose(unsigned int& beetle,
                   unsigned int& strip) const;

    /** cast to int */
    operator int() const;

    /** return the value */
    unsigned int value() const;

    /** value as UTStripRepresentation */
    UTStripRepresentation toUTStripRepresentation() const;

    /** Operator overloading for stringoutput */
    friend std::ostream& operator<< (std::ostream& s, 
                                     const UTDAQ::UTBeetleRepresentation& obj)
    {
      return obj.fillStream(s);
    }

    /** Fill the ASCII output stream **/ 
    std::ostream& fillStream(std::ostream& s) const;

    /** print method for python, not needed in C++ **/
    std::string toString() const;

  private:

    unsigned int m_value;

  };

}

//#include "Kernel/LHCbConstants.h"

inline UTDAQ::UTBeetleRepresentation::UTBeetleRepresentation(unsigned int beetle,
                                                         unsigned int port,
				                         unsigned int strip){
  m_value = (LHCbConstants::nStripsInBeetle*beetle) 
	  + (LHCbConstants::nStripsInPort *port) + strip; 
}

inline UTDAQ::UTBeetleRepresentation::UTBeetleRepresentation(const UTDAQ::UTStripRepresentation& strip):m_value(strip.value()){
  // constructor
}

inline UTDAQ::UTBeetleRepresentation::operator int() const {
  return m_value;
}

inline unsigned int UTDAQ::UTBeetleRepresentation::value() const{
  return m_value;
}

inline UTDAQ::UTStripRepresentation UTDAQ::UTBeetleRepresentation::toUTStripRepresentation() const{
  return UTDAQ::UTStripRepresentation(m_value);
}


inline void UTDAQ::UTBeetleRepresentation::decompose(unsigned int& beetle,
                                              unsigned int& port,
                                              unsigned int& strip) const{

  unsigned beetleStrip;
  decompose(beetle,beetleStrip); // turn into Beetle and strip

  port = beetleStrip/LHCbConstants::nStripsInPort;
  strip = beetleStrip%LHCbConstants::nStripsInPort;
} 

inline void UTDAQ::UTBeetleRepresentation::decompose(unsigned int& beetle,
                                                   unsigned int& strip) const{
  beetle = m_value/LHCbConstants::nStripsInBeetle;
  strip = m_value%LHCbConstants::nStripsInBeetle;
} 

inline std::ostream& UTDAQ::UTBeetleRepresentation::fillStream(std::ostream& s) const {
   unsigned int beetle, port, strip;
   decompose(beetle, port, strip);
   s << "{ "
    << " UTBeetleRepresentation: \t" << value() << std::endl
    << " Beetle: " << beetle << " Port: " << port 
    << "Strip: " << strip << std::endl << "  } ";

  return s;
}


#include <sstream>

inline std::string  UTDAQ::UTBeetleRepresentation::toString() const{
  std::ostringstream o;
  fillStream(o);
  return o.str();
}

#endif //  _UTDAQ_UTBeetleRepresentation_H
 
