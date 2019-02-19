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
#ifndef STXMLUtils_H
#define STXMLUtils_H 1

/** @namespace STXMLUtils STXMLUtils.h
 *
 *  class to help writing XML comments
 *
 *
 *  @author M Needham
 *  @date   2009-07-22
 */

#include <sstream>
#include <string>

namespace ST {
  namespace XMLUtils {

    /** add a comment for the author
     *  <!-- Author: author --!> */
    void authorComment( std::ostringstream& stream, const std::string& author );

    /** add a comment for the Version Tag
    <!-- Version: tag --!> */
    void tagComment( std::ostringstream& stream, const std::string& tag );

    /** add a comment for the description
     *  <!-- Descrption: desc --!> */
    void descriptionComment( std::ostringstream& stream, const std::string& description );

    /** Pipe all the comments together
     *  <!-- Author: author --!>
     *  <!-- Version: tag --!>
     *  <!-- Descrption: desc --!> */
    void fullComment( std::ostringstream& stream, const std::string& author, const std::string& tag,
                      const std::string desc );

    /** Generic comment
     *  <!-- Name value --!>
     */
    void comment( std::ostringstream& stream, const std::string& name, const std::string& value );

    /** remove nasty characters
    The characters , [ ] mapped to whitespace
    / to \
    **/
    void replaceChars( std::string& conString );

    /** replace in with out in conString */
    void replace( std::string& conString, std::string in, std::string out );

  } // namespace XMLUtils
} // namespace ST

#endif // ST::XMLUtils
