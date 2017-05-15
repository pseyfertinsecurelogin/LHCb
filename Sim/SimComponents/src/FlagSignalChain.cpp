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

  for ( auto iv = mother->endVertices().begin();
        iv != mother->endVertices().end(); iv++ ) {
    for ( auto idau = (*iv)->products().begin();
          idau != (*iv)->products().end(); idau++ ) {
      const LHCb::MCParticle* mcpc = *idau;
      LHCb::MCParticle* mcp = const_cast< LHCb::MCParticle* >( mcpc ) ;
      mcp->setFromSignal(true);
      setFromSignalFlag( *idau );
    }
  }
}
//==============================================================================
