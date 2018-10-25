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
#ifndef DUMPTRACKS_H
#define DUMPTRACKS_H 1

// Include files
#include "GaudiAlg/GaudiAlgorithm.h"
#include <string>

/** @class DumpTracks DumpTracks.h
 *  Dump all tracks in an event.
 *  Amount printed depends on OutputLevel:
 *    INFO: prints size of container
 *   DEBUG: prints also first "NumberOfObjectsToPrint" Tracks and States
 *          (default is 5)
 * VERBOSE: prints all Tracks and States
 *
 *  @author Marco Cattaneo
 *  @date   2004-07-14
 */
class DumpTracks : public GaudiAlgorithm
{

public:

  /// Standard constructor
  DumpTracks( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  unsigned int m_numObjects;    ///< Number of objects to print
  std::string m_tracksLocation; ///< Location of tracks container

};

#endif // DUMPTRACKS_H
