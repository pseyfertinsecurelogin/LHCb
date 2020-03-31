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
#include "EmptyEventNodeCleaner.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FSRCleaner
//
// 2012-08-01 : Rob Lambert
//-----------------------------------------------------------------------------

/** @class FSRCleaner FSRCleaner.h
 *
 *  Searches for and removes empty data nodes in the FSR tree
 *
 *  @author Rob Lambert
 *  @date   2012-08-01
 */
class FSRCleaner final : public EmptyEventNodeCleaner {

public:
  /// Standard constructor
  FSRCleaner( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override { return StatusCode::SUCCESS; } ///< Algorithm execution, do nothing
  StatusCode finalize() override;                               ///< Real algorithm execution in finalize
};

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
  StatusCode sc = EmptyEventNodeCleaner::execute();

  // return, try to avoid shortcutting the usual finalize
  return EmptyEventNodeCleaner::finalize().andThen( [sc = std::move( sc )] { return sc; } );
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( FSRCleaner )

//=============================================================================
