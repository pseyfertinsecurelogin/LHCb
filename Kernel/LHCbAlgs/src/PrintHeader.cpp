/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// Include files 

// from EventSys
#include "Event/ODIN.h"

// local
#include "PrintHeader.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintHeader
//
// 2003-03-16 : Gloria Corti
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory

DECLARE_COMPONENT( PrintHeader )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PrintHeader::PrintHeader( const std::string& name,
                          ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("PrintFileName",  m_printFile = false ); 
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode PrintHeader::execute() 
{
  // count events
  ++m_nEvents;

  // Get event number from the ODIN bank
  auto * odin = getIfExists<LHCb::ODIN> ( LHCb::ODINLocation::Default );
  if ( odin )
  {
    info() << "Run "     << odin->runNumber()
           << ", Event " << odin->eventNumber() << endmsg;
  } else { Warning("No ODIN Header").ignore(); }
       
  setFilterPassed(true);
  
  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode PrintHeader::finalize()
{
  info() << "Filtered " << m_nEvents << " events" << endmsg;
  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
