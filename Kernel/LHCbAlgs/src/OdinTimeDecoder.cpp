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

// from Gaudi
#include "GaudiKernel/Incident.h"
#include "GaudiKernel/IIncidentSvc.h"

// data model
#include "Event/RawEvent.h"
#include "Event/RawBank.h"
#include "Event/ODIN.h"

// detector description
#include "DetDesc/RunChangeIncident.h"

// local
#include "OdinTimeDecoder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : OdinTimeDecoder
//
// 2006-09-21 : Marco Clemencic
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( OdinTimeDecoder )

#define ON_DEBUG if ( UNLIKELY( msgLevel(MSG::DEBUG) ) )
#define DEBUG_MSG ON_DEBUG debug()

//=============================================================================
// Initialize
//=============================================================================
StatusCode OdinTimeDecoder::initialize()
{
  StatusCode sc = GaudiTool::initialize();
  if (sc.isFailure()) return sc;
  // try to get the tool to decode the ODIN bank
  return m_odinDecoder.retrieve();
}

//=========================================================================
//  Get the ODIN object from T.E.S.
//=========================================================================
LHCb::ODIN *OdinTimeDecoder::getODIN() const
{
  // Check if the root of the transient store is available before calling the
  // ODIN decoder. (e. g. during the initialize)
  DataObject * tmp = nullptr;
  if ( LIKELY( evtSvc()->findObject("", tmp).isSuccess() ) )
  {
    // Decode the ODIN bank.
    /// \fixme because of https://gitlab.cern.ch/gaudi/Gaudi/merge_requests/217 IGenericTool
    /// cannot be used in a const method. The proper fix can be implemented only in the context
    /// of transformer algorithms of Gaudi::Functional.
    const_cast<IGenericTool&>(*m_odinDecoder).execute();
    // @FIXME: we must get the ODIN object from where the Tool created it
    return getIfExists<LHCb::ODIN>(LHCb::ODINLocation::Default);
  }
  return nullptr;
}

//=========================================================================
//  Return the time of current event
//=========================================================================
Gaudi::Time OdinTimeDecoder::getTime ( ) const
{

  static Gaudi::Time last_time(0);

  LHCb::ODIN *odin = getODIN();

  if (odin)
  {
    DEBUG_MSG << "GPS Time = " << odin->gpsTime() << endmsg;

    try {
      last_time = odin->eventTime();
    } catch(TimeException &e) {
      Warning("Problem with ODIN GPS Time '" + e.message() + "', ignored",
              StatusCode::SUCCESS).ignore();
    }

    // We need to trigger a RunChange incident if the run number changes or
    // we switch from flagging mode to filtering mode.
    if ((m_currentRun != odin->runNumber()) ||
        (m_flaggingMode && !odin->isFlagging()) )
    {
      ON_DEBUG {
        debug() << "Firing " << IncidentType::RunChange << " incident. ";
        if (m_currentRun != odin->runNumber())
          debug() << " Run change " << m_currentRun
                  << " -> " << odin->runNumber();
        else
          debug() << " Switched from flagging to filtering mode.";
        debug() << endmsg;
      }
      m_currentRun = odin->runNumber();
      m_flaggingMode = odin->isFlagging();
      incSvc()->fireIncident(RunChangeIncident(name(), m_currentRun, last_time));
    }
  }

  return last_time;
}
//=============================================================================
