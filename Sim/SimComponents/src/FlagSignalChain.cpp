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
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "GaudiAlg/GaudiTool.h"
#include "MCInterfaces/IFlagSignalChain.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FlagSignalChain
//
// 2015-07-23 : Gloria Corti
//-----------------------------------------------------------------------------

/** @class FlagSignalChain FlagSignalChain.h
 *
 *
 *  @author Gloria Corti
 *  @date   2015-07-23
 */
struct FlagSignalChain : extends<GaudiTool, IFlagSignalChain> {

  using extends::extends;

  /// Set flag if particle is from signal
  void setFromSignalFlag( const LHCb::MCParticle* mother ) override {
    for ( auto& v : mother->endVertices() ) {
      for ( const auto& mcpc : v->products() ) {
        LHCb::MCParticle* mcp = const_cast<LHCb::MCParticle*>( mcpc.target() );
        mcp->setFromSignal( true );
        setFromSignalFlag( mcpc );
      }
    }
  }
};
// Declaration of the Tool Factory
DECLARE_COMPONENT( FlagSignalChain )
