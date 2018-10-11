/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
class UnpackTrackFunctional final :
  public Gaudi::Functional::Transformer< LHCb::Tracks(const LHCb::PackedTracks&) >
{

public:

  /// Standard constructor
  UnpackTrackFunctional( const std::string& name, ISvcLocator* pSvcLocator );

  /// Functional operator
  LHCb::Tracks operator()( const LHCb::PackedTracks& pTracks ) const override;

private:
  mutable Gaudi::Accumulators::AveragingCounter<unsigned long> m_unpackedTracks{ this, "# Unpacked Tracks" };
};

#endif // UnpackTrackFunctional
