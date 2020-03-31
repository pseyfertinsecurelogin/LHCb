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
#include "GaudiAlg/GaudiHistoAlg.h"
//-----------------------------------------------------------------------------
// Implementation file for class : EventCountHisto
//
// 2007-01-08 : Robert Lambert
//-----------------------------------------------------------------------------

/** @class EventCountHisto EventCountHisto.h
 *
 *
 *  @author Robert Lambert
 *  @date   2007-01-08
 */
class EventCountHisto final : public GaudiHistoAlg {

public:
  /// Standard constructor
  EventCountHisto( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;  ///< Algorithm execution
  StatusCode finalize() override; ///< Algorithm finalization

private:
  unsigned long long m_nEvents{0}; // number of events processed
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( EventCountHisto )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
EventCountHisto::EventCountHisto( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiHistoAlg( name, pSvcLocator ) {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode EventCountHisto::execute() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

  // code goes here
  ++m_nEvents;

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode EventCountHisto::finalize() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Finalize" << endmsg;

  info() << m_nEvents << " events processed" << endmsg;

  const std::string tag  = "Events";
  const std::string name = "N Events";
  plot( 1., tag, name, 0., 2, 3, m_nEvents );

  return GaudiHistoAlg::finalize();
}

//=============================================================================
