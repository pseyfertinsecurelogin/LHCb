// $Id: ITrackCriteriaSelector.h,v 1.1.1.1 2006-03-06 18:40:48 erodrigu Exp $
#ifndef TRACKMCINTERFACES_ITRACKCRITERIASELECTOR_H 
#define TRACKMCINTERFACES_ITRACKCRITERIASELECTOR_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Forward declarations
namespace LHCb {
 class MCParticle;
 class Track;
};

/** @class ITrackCriteriaSelector ITrackCriteriaSelector.h "TrackMCInterfaces/ITrackCriteriaSelector.h"
 *
 *  Interface class for the TrackCriteriaSelector tool.
 *
 *  @author Jeroen van Tilburg
 *  @date   2003-07-28
 */

static const InterfaceID IID_ITrackCriteriaSelector( "ITrackCriteriaSelector", 1, 0 );

class ITrackCriteriaSelector : virtual public IAlgTool
{
public:
  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_ITrackCriteriaSelector; }

  virtual bool select( LHCb::Track* track ) const = 0;
  virtual bool select( LHCb::MCParticle* mcParticle ) = 0;

  virtual bool selectByTrackType( LHCb::Track* track ) const = 0;
  virtual bool selectByTrackType( LHCb::MCParticle* mcParticle ) = 0;

  virtual unsigned int trackType( LHCb::MCParticle* mcPart ) = 0;

  virtual StatusCode setTrackType( LHCb::MCParticle* mcPart,
                                   LHCb::Track*& track ) = 0;

};

#endif // TRACKMCINTERFACES_ITRACKCRITERIASELECTOR_H
