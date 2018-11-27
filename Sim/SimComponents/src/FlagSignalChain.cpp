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

// from LHCb
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"

// local
#include "FlagSignalChain.h"

//-----------------------------------------------------------------------------
// Implementation file for class : FlagSignalChain
//
// 2015-07-23 : Gloria Corti
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( FlagSignalChain )

//==============================================================================
// Set from signal flag
//==============================================================================
void FlagSignalChain::setFromSignalFlag( const LHCb::MCParticle* mother ) {

  for ( auto& v : mother->endVertices() ) {
    for ( const auto& mcpc : v->products() ) {
      LHCb::MCParticle* mcp = const_cast< LHCb::MCParticle* >( mcpc.target() ) ;
      mcp->setFromSignal(true);
      setFromSignalFlag( mcpc );
    }
  }
}
//==============================================================================
