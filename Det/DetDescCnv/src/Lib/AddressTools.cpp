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

// local
#include "AddressTools.h"

#include <cstdlib> // for getenv with gcc 4.3

//-----------------------------------------------------------------------------
// Implementation file for class : AddressTools
//
// 2005-06-13 : Juan PALACIOS
//-----------------------------------------------------------------------------

bool AddressTools::expandAddress( std::string& address ) {
  bool                   return_value = true;
  std::string::size_type start        = address.find( environmentKey );

  if ( start != std::string::npos ) {
    address        = address.substr( start );
    std::string ev = extractVariable( address );
    return_value   = resolvedEV( ev );
    address.replace( 0, address.find_first_of( delims ), ev );
  }

  return return_value;
}
//-----------------------------------------------------------------------------
std::string AddressTools::extractVariable( const std::string& address ) {
  std::string::size_type start = address.find( environmentKey );
  if ( start == std::string::npos ) return "";
  std::string::size_type length = address.find_first_of( delims, start ) - start;
  return cleanEV( address.substr( start, length ) );
}
//-----------------------------------------------------------------------------
std::string AddressTools::cleanEV( const std::string& environmentVariable ) {
  std::string dummy = environmentVariable;

  while ( dummy.find_first_of( brackets ) != std::string::npos ) {
    dummy.replace( dummy.find_first_of( brackets ), dummy.find_first_of( brackets ), "" );
  }
  return dummy.replace( dummy.find( environmentKey ), dummy.find( environmentKey ) + environmentKey.size(), "" );
}
//-----------------------------------------------------------------------------
bool AddressTools::resolvedEV( std::string& environmentVariable ) {
  bool        return_value   = false;
  const char* charEV         = environmentVariable.c_str();
  const char* charResolvedEV = getenv( charEV );

  if ( 0 != charResolvedEV ) {
    environmentVariable = std::string( charResolvedEV );
    return_value        = true;
  } else {
    environmentVariable = "";
    return_value        = false;
  }
  return return_value;
}
//-----------------------------------------------------------------------------
