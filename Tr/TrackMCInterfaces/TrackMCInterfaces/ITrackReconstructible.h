// $Id: ITrackReconstructible.h,v 1.1.1.1 2006-03-06 18:40:48 erodrigu Exp $
#ifndef TRACKMCINTERFACES_ITRACKRECONSTRUCTIBLE_H 
#define TRACKMCINTERFACES_ITRACKRECONSTRUCTIBLE_H 1

// Include files
// -------------
// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Forward declarations
namespace LHCb {
 class MCParticle;
};

static const InterfaceID IID_ITrackReconstructible( "ITrackReconstructible", 0, 0 );

/** @class ITrackReconstructible TrackMCInterfaces/ITrackReconstructible.h
 *  
 *
 *  @author Eduardo Rodrigues, Jose Hernando
 *  @date   2005-05-20
 */

class ITrackReconstructible : virtual public IAlgTool {
public:

  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITrackReconstructible; }

  virtual bool hasVelo        ( LHCb::MCParticle* tr ) = 0;
  virtual bool hasTT          ( LHCb::MCParticle* tr ) = 0;
  virtual bool hasSeed        ( LHCb::MCParticle* tr ) = 0;
  virtual bool hasVeloAndSeed ( LHCb::MCParticle* tr ) = 0;
  
};
#endif // TRACKMCINTERFACES_ITRACKRECONSTRUCTIBLE_H 
