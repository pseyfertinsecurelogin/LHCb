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

// local
#include "Event/ProcStatus.h"

// STL
#include <algorithm>

//-----------------------------------------------------------------------------
// Implementation file for class : ProcStatus
//
// 2010-06-25 : Chris Jones
//-----------------------------------------------------------------------------

void LHCb::ProcStatus::addAlgorithmStatus( const std::string& name, int status ) {
  // Do we already have an entry for this alg ?
  auto ialg = std::find_if( m_algs.begin(), m_algs.end(), [&name]( const auto& S ) { return S.first == name; } );
  // If so, just set status, otherwise create an entry
  if ( ialg != m_algs.end() ) {
    ialg->second = status;
  } else {
    m_algs.emplace_back( AlgStatus( name, status ) );
  }
}

void LHCb::ProcStatus::addAlgorithmStatus( const std::string& algName, const std::string& subsystem,
                                           const std::string& reason, const int status, const bool eventAborted ) {
  // Add abort or OK string
  std::string message = ( eventAborted ? "ABORTED:" : "OK:" );

  // Add sub-system
  message += subsystem + ":";

  // Add reason for the message
  message += reason + ":";

  // Finally, the algorithm name
  message += algName;

  // Set the full message in the algs list
  addAlgorithmStatus( message, status );

  // if event was aborted, set this as well.
  // (Don't set if false, since this will overwrite pre-existing aborts
  if ( eventAborted ) setAborted( true );
}

bool LHCb::ProcStatus::subSystemAbort( const std::string& subsystem ) const {
  bool isaborted = false;
  if ( aborted() ) {
    // Loop over reports
    for ( const auto& S : m_algs ) {
      // Is this report an ABORT or OK ?
      auto colon = S.first.find_first_of( ":" );
      if ( colon > 0 && S.first.substr( 0, colon ) == "ABORTED" ) {
        // This is an abort, but is it the correct subsystem ?
        const std::string tmpS = S.first.substr( colon + 1 );
        colon                  = tmpS.find_first_of( ":" );
        if ( colon > 0 && tmpS.substr( 0, colon ) == subsystem ) {
          isaborted = true;
          break;
        }
      }
    }
  }
  return isaborted;
}

std::ostream& LHCb::ProcStatus::fillStream( std::ostream& s ) const {
  const char l_aborted = aborted() ? 'T' : 'F';
  s << "{ "
    << "aborted :	" << l_aborted << " Algorithm Status Reports : ";
  for ( const auto& S : m_algs ) { s << "[" << S.first << ":" << S.second << "] "; }
  return s << " }";
}
