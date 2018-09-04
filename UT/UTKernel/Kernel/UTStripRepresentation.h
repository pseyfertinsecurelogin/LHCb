// $Id: UTStripRepresentation.h,v 1.2 2009-01-22 11:43:44 mneedham Exp $
#ifndef _UTDAQ_UTStripRepresentation_H
#define _UTDAQ_UTStripRepresentation_H 1


/** @class StripRepresentaiton UTStripRepresentation.h  UTDAQ/UTStripRepresentation
*
* Sometimes we number a strip on a tell1 board 0 - 3072
* This helper class does just that.
*
*  @author M.Needham
*  @date   17/8/2008
*/

#include <iostream>

namespace UTDAQ{

  class UTStripRepresentation{

  public:

    /** constructor  from int */
    explicit UTStripRepresentation(unsigned int value); 

    /** destructor */
    ~UTStripRepresentation(){}

    /** cast to int */
    operator int() const;

    /** return the value */
    unsigned int value() const;

    /** Operator overloading for stringoutput */
    friend std::ostream& operator<< (std::ostream& s, 
                                     const UTDAQ::UTStripRepresentation& obj)
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


inline UTDAQ::UTStripRepresentation::UTStripRepresentation(unsigned int value):m_value(value){
  // constructor
}

inline UTDAQ::UTStripRepresentation::operator int() const {
  return m_value;
}

inline unsigned int UTDAQ::UTStripRepresentation::value() const{
  return m_value;
}

inline std::ostream& UTDAQ::UTStripRepresentation::fillStream(std::ostream& s) const {

   s << "{ "
     << "Strip: " << value() << std::endl << "  } ";

  return s;
}

#include <sstream>

inline std::string  UTDAQ::UTStripRepresentation::toString() const{
  std::ostringstream o;
  fillStream(o);
  return o.str();
}


#endif //  _UTDAQ_UTStripRepresentation_H
 
