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
#ifndef UT_UTLEXICALCAUTER_H
#define UT_UTLEXICALCAUTER_H

#include "boost/lexical_cast.hpp"
#include <iostream>

namespace UT{

  /**
   * Template converter from basic anytype to string
   * @param type the number you want to convert
   * @param digits the number of desired digits (ie '1' -> "001" if digits is 3
   * @return the wanted string
   *
   * @author Andy Beiter (based on code by Johan Luisier)
   * @date 2018-09-04
   */
  template <typename T>
  std::string toString(const T& type, const unsigned int& digits = 0)
  {
    std::string result(boost::lexical_cast<std::string>(type));
    if (digits != 0)
    {
      while(result.size() < digits)
        result = "0" + result;
    }
    return result;
  }

  /**
   * Template converter from string to any numeric type.
   * @param m_string the string which has to be converted
   * @param type the variable where the value will be stored
   * @return bool true if success
   * @author Andy Beiter based on code by:
   * @author Johan Luisier
   * @author M Needham
   * @date 2018-09-04
   */
  template <typename T>
  bool fromString(const std::string& mystring, T& type)
  {
    bool ok;
    try{
      ok = true;
      type = boost::lexical_cast<T>(mystring);
    } // try
    catch(boost::bad_lexical_cast& e){
      // catch expection if we fail...
      ok = false;
      std::cerr << "ERROR " << e.what() << "** " << mystring << " **" << std::endl;
    } // catch
    return ok;
  }
}

#endif //UT_UTLEXICALCAUTER_H
