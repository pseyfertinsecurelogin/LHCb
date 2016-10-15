// $Id: UnpackTrackFunctional.h,v 1.2 2009-11-06 18:34:34 jonrob Exp $
#ifndef UnpackTrackFunctional_H 
#define UnpackTrackFunctional_H 1

// Include files
// from Gaudi
#include "GaudiAlg/Transformer.h"
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/Track.h"
#include "Event/PackedTrack.h"

/** @class UnpackTrackFunctional UnpackTrackFunctional.h
 *
 *  Unpack the PackedTrack
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackTrackFunctional : public Gaudi::Functional::Transformer< LHCb::Tracks(const LHCb::PackedTracks&) >
{

public: 

  /// Standard constructor
  UnpackTrackFunctional( const std::string& name, ISvcLocator* pSvcLocator );

  /// Functional operator
  LHCb::Tracks operator()( const LHCb::PackedTracks& pTracks ) const;

};

#endif // UnpackTrackFunctional
