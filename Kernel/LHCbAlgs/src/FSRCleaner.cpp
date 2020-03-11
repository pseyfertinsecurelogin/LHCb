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
#include "FSRCleaner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FSRCleaner
//
// 2012-08-01 : Rob Lambert
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
FSRCleaner::FSRCleaner( const std::string& name, ISvcLocator* pSvcLocator )
    : EmptyEventNodeCleaner( name, pSvcLocator ) {
  // Set sensible FSR defaults
  setProperty( "InputStream", "/FileRecords" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  setProperty( "DataService", "FileRecordDataSvc" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode FSRCleaner::finalize() {
  // Clean performed in execute of EmptyEventNodeCleaner... so let's call it!
  const StatusCode sc = EmptyEventNodeCleaner::execute();

  // return, try to avoid shortcutting the usual finalize
  return StatusCode{EmptyEventNodeCleaner::finalize() && sc};
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FSRCleaner )

//=============================================================================
