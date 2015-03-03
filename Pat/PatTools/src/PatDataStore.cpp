// $Id: PatDataStore.cpp,v 1.5 2006-06-01 13:43:25 ocallot Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/ToolFactory.h" 

// local
#include "PatTools/PatDataStore.h"

using namespace LHCb;

//-----------------------------------------------------------------------------
// Implementation file for class : PatDataStore
//
// 2005-05-23 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
static const  ToolFactory<PatDataStore>          s_factory ;
const        IToolFactory& PatDataStoreFactory = s_factory ; 


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
PatDataStore::PatDataStore( const std::string& type,
                            const std::string& name,
                            const IInterface* parent )
  : GaudiTool ( type, name , parent )
{
  declareInterface<PatDataStore>(this);
  declareProperty( "onTES", m_onTES = true );
  
  m_states    = new PatStateContainer( 500 );
  m_veloCoord = new PatVeloContainer( 5000 );
  m_ttCoord   = new PatTTCoordContainer( 5000 );
  m_tCoord    = new PatTCoordContainer( 10000 );
}
//=============================================================================
// Destructor
//=============================================================================
PatDataStore::~PatDataStore() {
  delete m_tCoord;
  delete m_ttCoord;
  delete m_veloCoord;
  delete m_states;
}

//=========================================================================
//  Finalize, remove the states on the tracks.
//=========================================================================
StatusCode PatDataStore::finalize ( ) {
  updateSize();  // to get the size of the container for the last event.
  info() << "Maximum number of Velo coordinates " << m_veloCoord->maxSize() << endreq;
  if ( 0 < m_ttCoord->maxSize() ) {
    info() << "Maximum number of TT coordinates   " << m_ttCoord->maxSize() << endreq;
  }
  if ( 0 < m_tCoord->maxSize() ) {
    info() << "Maximum number of T coordinates    " << m_tCoord->maxSize() << endreq;
  }
  if ( 0 < m_states->maxSize() ) {
    info() << "Maximum number of States in store  " << m_states->maxSize() << endreq;
    //== remove all states from the tracks, so that the state vector can be deleted
    for ( PatContainerMap<Track>::iterator itP = beginTracks();
          endTracks() != itP; ++itP ) {
      std::vector<Track*>::iterator itT = (*itP).second->begin();
      for ( int kk = 0; (*itP).second->maxSize() > kk; ++kk ) {
        Track* tr = *(itT+kk);
        tr->clearStates();
      }
    }  
  }
  return GaudiTool::finalize();
}
//=============================================================================
