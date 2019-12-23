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
#include "Event/ODIN.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : PrintHeader
//
// 2003-03-16 : Gloria Corti
//-----------------------------------------------------------------------------

/** @class PrintHeader PrintHeader.h cmt/PrintHeader.h
 *
 *  Print event and run number in debug mode
 *
 *  @author Gloria Corti
 *  @date   2003-03-16
 */

class PrintHeader final : public Gaudi::Functional::Consumer<void( const LHCb::ODIN& )> {
  mutable Gaudi::Accumulators::Counter<> m_nEvents{this, "EventCount"}; ///< Counter of events processed
public:
  /// Standard constructor
  PrintHeader( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name, pSvcLocator, {"ODINLocation", LHCb::ODINLocation::Default}} {}

  void operator()( const LHCb::ODIN& odin ) const override {
    ++m_nEvents;
    info() << "Run " << odin.runNumber() << ", Event " << odin.eventNumber() << endmsg;
  }
};

// Declaration of the Algorithm Factory

DECLARE_COMPONENT( PrintHeader )
