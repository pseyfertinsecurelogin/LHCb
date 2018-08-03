#ifndef TRACKMCINTERFACES_IIDEALSTATECREATOR_H
#define TRACKMCINTERFACES_IIDEALSTATECREATOR_H 1

// Include files
// -------------
#include "GaudiKernel/IAlgTool.h"

#include "Event/Track.h"
// forward declarations
namespace LHCb {
 class MCParticle;
 class MCHit;
 class State;
 class StateVector;
}

/** @class IIdealStateCreator IIdealStateCreator.h "TrackMCInterfaces/IIdealStateCreator.h"
 *
 *  Interface class for IdealStateCreator (in Tr/TrackMCTools).
 *
 *  @author Eduardo Rodrigues (adaptations to new track event model)
 *  @date   2005-04-04
 *
 *  @author Rutger van der Eijk
 *  @date   4-7-2002
 */


struct  IIdealStateCreator: extend_interfaces<IAlgTool> {

  /// Retrieve interface ID
  DeclareInterfaceID( IIdealStateCreator, 2, 0 );
  
  /// Create a state at a z-position from an MCParticle's entry/exit points
  virtual StatusCode createState( const LHCb::MCParticle* mcPart,
                                  double zrec,
                                  LHCb::State& pState ) const = 0;

  /// Create a state at a z-position from an MCHits entry/exit points
  virtual StatusCode createState(const LHCb::MCHit* mcHit,
                                  double zrec,
                                  LHCb::State& pState ) const = 0;

  /// Create a state at an MCParticle's vertex
  virtual StatusCode createStateVertex( const LHCb::MCParticle* mcPart,
                                        LHCb::State& pState ) const = 0;


  /// Create a statevector at a z-position from an MCParticle's entry/exit points
  virtual StatusCode createStateVector( const LHCb::MCParticle* mcPart,
                                  double zrec,
                                  LHCb::StateVector& pVec ) const = 0;

  /// Create a statevector at a z-position from an MCHits entry/exit points
  virtual StatusCode createStateVector(const LHCb::MCHit* mcHit,
                                  double zrec,
                                  LHCb::StateVector& pVec ) const = 0;

  /// Create a statevector at an MCParticle's vertex
  virtual StatusCode createStateVectorVertex( const LHCb::MCParticle* mcPart,
                                              LHCb::StateVector& pVec ) const = 0;

  /// Adds states for all related MCHits to the track
  virtual StatusCode addMCHitStates(const LHCb::MCParticle& mcPart, LHCb::Track& track) const = 0;
  
};

#endif // TRACKMCINTERFACES_IIDEALSTATECREATOR_H
