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
#ifndef COMPARETRACK_H
#define COMPARETRACK_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/PackedTrack.h"
#include "Event/Track.h"

/** @class CompareTrack CompareTrack.h
 *  Compare two containers of Tracks
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class CompareTrack : public GaudiAlgorithm {

public:
  /// Standard constructor
  CompareTrack( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  std::string m_inputName;
  std::string m_testName;
};

#endif // COMPARETRACK_H
