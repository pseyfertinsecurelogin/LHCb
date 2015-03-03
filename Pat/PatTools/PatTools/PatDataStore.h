// $Id: PatDataStore.h,v 1.10 2006-07-20 16:00:31 cattanem Exp $
#ifndef PATTOOLS_PATDATASTORE_H 
#define PATTOOLS_PATDATASTORE_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"

#include "Event/Track.h"
#include "Event/RecVertex.h"
#include "PatTools/PatContainerMap.h"
#include "PatTools/PatDataContainer.h"
#include "PatTools/PatVeloCoord.h"
#include "PatTools/PatTTCoord.h"
#include "PatTools/PatTCoord.h"

namespace TrackKeys {
  enum flags { HasMissedVeloStations = 32,
	       AlreadyUsed           = 64 };
}

namespace PatDataStoreName {
  static const std::string& Hlt  = "HltPatDataStore";
}
static const InterfaceID IID_PatDataStore ( "PatDataStore", 1, 0 );

typedef LHCb::RecVertex TVertex;

typedef PatContainer<LHCb::RecVertex>        PatVertexContainer;
typedef PatContainer<LHCb::Track>            PatTrackContainer;
typedef PatDataContainer<LHCb::State>        PatStateContainer;
typedef PatDataContainer<PatVeloCoord> PatVeloContainer;
typedef PatDataContainer<PatTTCoord>   PatTTCoordContainer;
typedef PatDataContainer<PatTCoord>    PatTCoordContainer;

/** @class PatDataStore PatDataStore.h PatTools/PatDataStore.h
 *  This tool is used in algorithms wanting fast access to track and 
 *  vertices containers. It provides in fact PatContainer<T> which
 *  can be either on TES (standard, slower) or on local memory, which 
 *  is fast but non Gaudio standard. The type is decided at creation
 *  and can't be changed. The PatContainer object is either a layer
 *  interfacing the standard TES access, or its own data provider,
 *  based on the TrgContainer implementation developped in 2004-5
 *
 *  @author Olivier Callot
 *  @date   2005-05-20
 */
class PatDataStore : public GaudiTool {
public: 

  // Return the interface ID
  static const InterfaceID& interfaceID() { return IID_PatDataStore; }

  /// Standard constructor
  PatDataStore( const std::string& type, 
		const std::string& name,
		const IInterface* parent);

  virtual ~PatDataStore( ); ///< Destructor

  //! finalize method, to print and clean-up track's states

  virtual StatusCode finalize();

  //! Initialization, once per event
  void initEvent( ) {
    m_tracksMap.initEvent( );
    m_verticesMap.initEvent( );
    m_veloCoord->initEvent();
    m_ttCoord->initEvent();
    m_tCoord->initEvent();
    m_states->initEvent();
  }
  
  //! Update size, for last event
  void updateSize( ) {
    m_tracksMap.updateSize( );
    m_verticesMap.updateSize( );
    m_veloCoord->updateSize();
    m_ttCoord->updateSize();
    m_tCoord->updateSize();
    m_states->updateSize();
  }
  

  //=======================================================================
  //   Track containers.
  //=======================================================================

  //!  Create a container, with the specified mode
  PatTrackContainer* createTrackContainer( const std::string& myName, 
					   int   nsize = 200  ) {
    PatTrackContainer* pat = m_tracksMap.container(myName);
    if (pat == NULL) 
      pat = m_tracksMap.createContainer( myName, nsize, m_onTES, evtSvc() );

    return pat;
  }

  //! return the container of tracks with specified name
  PatTrackContainer* tracks(const std::string& myName) {
    PatTrackContainer* pat = m_tracksMap.container( myName );
    if (pat == NULL) {
      warning() << " No Track container " << myName 
		<< " in PatDataStore ==> creating it " << endreq;      
      pat = createTrackContainer( myName );
    }

    return pat;
  }

  //! iterator to the beginning of the list of track containers
  PatContainerMap<LHCb::Track>::iterator beginTracks() {
    return m_tracksMap.begin();
  }
  
  //! iterator to the end of the list of track containers
  PatContainerMap<LHCb::Track>::iterator endTracks() {
    return m_tracksMap.end();
  }  
  //=======================================================================
  //   Handling of vertices
  //=======================================================================
  //! create and return a container of PatVertexs
  PatVertexContainer* createVertexContainer( const std::string& myName, 
					     int  nsize = 20 ) {
    return m_verticesMap.createContainer( myName,
					  nsize, m_onTES, evtSvc() );
  }

  //! return the container of vertices with specified name
  PatVertexContainer* vertices(const std::string& myName) {
    PatVertexContainer* pat = m_verticesMap.container( myName );
    if (pat == NULL) {
      warning() << " No Vertex container " << myName 
		<< " in PatDataStore ==> creating it " << endreq;
      pat = createVertexContainer(myName);
    } 
    return pat;
  }

  //! Returns an iterator to the beginning of the list of vertex containers
  PatContainerMap<LHCb::RecVertex>::iterator beginVertices() {
    return m_verticesMap.begin();
  }
  
  //! Returns an iterator to the end of the list of vertex containers
  PatContainerMap<LHCb::RecVertex>::iterator endVertices() {
    return m_verticesMap.end();
  }  

  //! State container
  PatStateContainer* states()  { return m_states; }

  //! Velo Coordinates container
  PatVeloContainer* veloCoords()   { return m_veloCoord; }

  //! TT Coordinates container
  PatTTCoordContainer* ttCoords()  { return m_ttCoord; }

  //! T station Coordinates container
  PatTCoordContainer* tCoords()    { return m_tCoord; }

  //! Flag to indicate if data is on TES
  bool onTES()  const { return m_onTES; }
protected:

private:
  bool                         m_onTES;        ///< Containers on TES ?
  PatContainerMap<LHCb::Track>     m_tracksMap;   ///< Map of track containers
  PatContainerMap<LHCb::RecVertex> m_verticesMap; ///< Map of vertices containers
  PatStateContainer*           m_states;       ///< Storage for States
  PatVeloContainer*            m_veloCoord;    ///< Storage for PatVeloCoord
  PatTTCoordContainer*         m_ttCoord;      ///< Storage for PatTTCoord
  PatTCoordContainer*          m_tCoord;       ///< Storage for PatTCoord
};

#endif // PATTOOLS_PATDATASTORE_H
