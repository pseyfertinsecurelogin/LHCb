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
// Include files
#include <string>
#include <cstdio>
#include "DetDesc/DetDesc.h"


/** print double value using format
 *  @param value   value to be printed
 *  @param format  desirable format
 *  @return string representation of the "value"
 */
std::string DetDesc::print (const double value, const char*  format) {
  constexpr unsigned int buflen = 128;
  char  buffer[buflen];
  return { buffer , buffer + snprintf( buffer , buflen, format , value ) };
}

/** print double value using format
 *  @param value   value to be printed
 *  @param format  desirable format
 *  @return string representation of the "value"
 */
std::string DetDesc::print (const long value, const char*  format) {
  constexpr unsigned int buflen = 128;
  char  buffer[buflen];
  return { buffer , buffer + snprintf( buffer , buflen, format , value ) };
}

/**
 * Gets an instance of Services
 */
DetDesc::ServicesPtr DetDesc::services() {
  return DetDesc::Services::services();
}
