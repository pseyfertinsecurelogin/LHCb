// Include files
#include <cstdio>
#include <string> 

#include "DetDesc/Services.h"


/** print double value using format
 *  @param value   value to be printed 
 *  @param format  desirable format
 *  @return string representation of the "value"
 */
std::string DetDesc::print (const double value, const char*  format) {
  static constexpr unsigned int buflen = 128;
  static char  buffer[buflen];
  return { buffer , buffer + snprintf( buffer , buflen, format , value ) };
}

/** print double value using format
 *  @param value   value to be printed 
 *  @param format  desirable format
 *  @return string representation of the "value"
 */
std::string DetDesc::print (const long value, const char*  format) {
  static constexpr unsigned int buflen = 128;
  static char  buffer[buflen];
  return { buffer , buffer + snprintf( buffer , buflen, format , value ) };
}

/**
 * Gets an instance of Services
 */
DetDesc::ServicesPtr DetDesc::services() {
  return DetDesc::Services::services();
}
