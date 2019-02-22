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
// $Id: AddressTools.h,v 1.2 2006-04-10 16:26:46 marcocle Exp $
#ifndef STREAMS_ADDRESSTOOLS_H
#define STREAMS_ADDRESSTOOLS_H 1

// Include files
#include <string>

/** @class AddressTools AddressTools.h streams/AddressTools.h
 *
 *
 *  @author Juan PALACIOS
 *  @date   2005-06-13
 */

static const std::string environmentKey( "$" );
static const std::string delims( " \t,.;/#" );
static const std::string brackets( "{}" );
class AddressTools {
public:
  static bool expandAddress( std::string& address );
  static bool hasEnvironmentVariable( const std::string& address ) {
    return address.find( environmentKey ) != std::string::npos;
  }

private:
  static std::string extractVariable( const std::string& address );
  static std::string cleanEV( const std::string& environmentVariable );
  static bool        resolvedEV( std::string& environmentVariable );
};
#endif // STREAMS_ADDRESSTOOLS_H
