/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// stdlib
#include <string>

// boost
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/range.hpp>
#include <boost/regex.hpp>

// local
#include <FileStager/FileStagerFunctions.h>

namespace ba = boost::algorithm;

namespace {
  using std::string;
}

namespace FileStager {
  //=============================================================================
  bool createPFN( string& remote, string& command, bool stageLocal ) {
    boost::match_flag_type                 flags = boost::match_default;
    boost::match_results<string::iterator> match;
    boost::regex                           re( "^(root:)((?:root:)|(?:dcap:))" );

    boost::iterator_range<string::iterator> result;
    if ( ( result = ba::find_first( remote, "gfal:" ) ) ) {
      // strip gfal:
      ba::erase_range( remote, result );
    } else if ( regex_search( remote.begin(), remote.end(), match, re, flags ) ) {
      // in case of root:root: or root:dcap, remove the first root
      boost::iterator_range<string::iterator> range( match[1].first, match[1].second );
      ba::erase_range( remote, range );
    } else if ( ( result = ba::find_first( remote, "mdf:" ) ) ) {
      // strip mdf:
      ba::erase_range( remote, result );
    }

    re = "^((?:rfio)|(?:castor):)//(?:\\w\\.\\w\\.\\w:\\d+/+castor)";
    boost::regex re_ssh{"^(ssh://)([a-zA-z0-9\\.-]+)(?::(\\d)+)?"};
    if ( regex_search( remote.begin(), remote.end(), match, re, flags ) ) {
      boost::iterator_range<string::iterator> range( match[1].first, match[1].second );
      ba::replace_range( remote, range, "rfio:" );
      command = "rfcp";
      return true;
    } else if ( ( result = ba::find_first( remote, "castor:" ) ) ) {
      // erase castor
      ba::erase_range( remote, result );
      command = "rfcp";
      return true;
    } else if ( ( result = ba::find_first( remote, "rfio:" ) ) ) {
      // erase rfio
      ba::erase_range( remote, result );
      command = "rfcp";
      return true;
    } else if ( ( result = ba::find_first( remote, "root:" ) ) ) {
      // xrootd needs no changes, command is xrdcp
      command = "xrdcp -s";
      return true;
    } else if ( ( result = ba::find_first( remote, "dcap:" ) ) ) {
      // gsidcap or dcap needs no changes, command is dccp
      command = "dccp -A";
      return true;
    } else if ( regex_search( begin( remote ), end( remote ), match, re_ssh, flags ) ) {
      command = "scp -oStrictHostKeyChecking=no -oForwardX11=no -oForwardX11Trusted=no -oForwardAgent=no -q";
      if ( match[3].length() > 0 ) { command += " -P " + match.str( 3 ); }
      remote = match[2].str() + string( match[0].second, end( remote ) );
      return true;
    } else if ( ( result = ba::find_first( remote, "/castor" ) ) ) {
      // castor file, no protocol specification
      command = "rfcp";
      return true;
    } else if ( stageLocal ) {
      if ( ( result = ba::find_first( remote, "file:" ) ) ) {
        // local file, perhaps nfs or other networked filesystem
        ba::erase_range( remote, result );
        ba::trim_left_if( remote, ba::is_any_of( "/" ) );
        remote = "/" + remote;
      }
      command = "cp";
      return true;
    } else {
      return false;
    }
  }
} // namespace FileStager
