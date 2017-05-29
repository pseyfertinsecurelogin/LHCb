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
DECLARE_TOOL_FACTORY( FlagSignalChain )

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
