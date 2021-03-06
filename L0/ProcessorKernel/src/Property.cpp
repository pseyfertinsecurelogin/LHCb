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

#include "ProcessorKernel/Property.h"

#include <cstring> // For strlen with gcc 4.3

/**
    Build vectors
*/
const std::vector<int> L0Muon::Property::getVectorOfInt() const {
  std::vector<int> vi;
  const char*      str    = m_value.c_str();
  int              length = m_value.size();
  char*            endptr;

  for ( int k = 0; k < length; k++ ) {
    int i = strtol( str, &endptr, 0 );
    vi.push_back( i );
    if ( strlen( endptr ) == 0 ) { break; }
    str = endptr + 1;
  }
  return vi;
}
const std::vector<long> L0Muon::Property::getVectorOfLong() const {
  std::vector<long> vl;
  const char*       str    = m_value.c_str();
  int               length = m_value.size();
  char*             endptr;

  for ( int k = 0; k < length; k++ ) {
    long l = strtol( str, &endptr, 0 );
    vl.push_back( l );
    if ( strlen( endptr ) == 0 ) { break; }
    str = endptr + 1;
  }
  return vl;
}
const std::vector<float> L0Muon::Property::getVectorOfFloat() const {
  std::vector<float> vf;
  const char*        str    = m_value.c_str();
  int                length = m_value.size();
  char*              endptr;

  for ( int k = 0; k < length; k++ ) {
    float f = (float)strtod( str, &endptr );
    vf.push_back( f );
    if ( strlen( endptr ) == 0 ) { break; }
    str = endptr + 1;
  }
  return vf;
}
const std::vector<double> L0Muon::Property::getVectorOfDouble() const {
  std::vector<double> vd;
  const char*         str    = m_value.c_str();
  int                 length = m_value.size();
  char*               endptr;

  for ( int k = 0; k < length; k++ ) {
    double d = strtod( str, &endptr );
    vd.push_back( d );
    if ( strlen( endptr ) == 0 ) { break; }
    str = endptr + 1;
  }
  return vd;
}
/**
    Define ouput operator.
    Output the internal string.
*/
std::ostream& operator<<( std::ostream& os, const L0Muon::Property& property ) {

  const std::string s = property;
  return os << s;
}
