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
#include "BasicDQFilter.h"

// ----------------------------------------------------------------------------
// Implementation file for class: BasicDQFilter
//
// 04/11/2011: Marco Clemencic
// ----------------------------------------------------------------------------
DECLARE_COMPONENT( BasicDQFilter )

bool BasicDQFilter::accept( const FlagsType& flags ) const {
  return std::none_of( flags.begin(), flags.end(), [&]( const auto& f ) {
    // if the flag is not ignored
    if ( m_ignoredFlags.find( f.first ) != m_ignoredFlags.end() ) {
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Ignoring flag: " << f.first << endmsg;
      return false;
    }
    return f.second != 0;
  } );
}
// ============================================================================
