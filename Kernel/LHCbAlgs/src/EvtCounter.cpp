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

// from Gaudi
#include "GaudiKernel/IIncidentSvc.h"

// local
#include "EvtCounter.h"

//-----------------------------------------------------------------------------
// Implementation file for class : EvtCounter
//
// 2004-06-15 : Patrick KOPPENBURG
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory

DECLARE_COMPONENT( EvtCounter )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EvtCounter::EvtCounter( const std::string& type,
                        const std::string& name,
                        const IInterface* parent )
: base_class ( type, name , parent )
{
  declareProperty("InitialCount", m_initialCount = 1,
                  "Value to be used for the first event.");
  declareProperty("UseIncident", m_useIncident = true,
                  "Whether to increment the counter at every BeginEvent incident.");
}

//=============================================================================
// Initialize method, retrieve necessary services
//=============================================================================
StatusCode EvtCounter::initialize() 
{

  // Mandatory initialization of GaudiTool
  StatusCode sc = GaudiTool::initialize();
  if( sc.isFailure() ) { return sc; }

  m_eventCounter = m_initialCount;
  if (m_useIncident) {
    // Register to the Incident service to be notified at the begin of one event
    incSvc()->addListener(this,"BeginEvent",101);
    // take into account that the increment will be called once before the first event
    --m_eventCounter;
  }

  return sc;
}

//=============================================================================
