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
#ifndef UNPACKTRACK_H
#define UNPACKTRACK_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "Event/Track.h"
#include "Event/PackedTrack.h"

/** @class UnpackTrack UnpackTrack.h
 *
 *  Unpack the PackedTrack
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackTrack : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackTrack( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:

  std::string m_inputName;  ///< Input location
  std::string m_outputName; ///< Output location
  bool m_alwaysOutput;      ///< Flag to turn on the creation of output, even when input is missing
  std::string m_ancestorFor;  ///< Container name for which ancestors shoudl be built
  std::string m_ancestorSource; ///< Container where ancestors are.

};

#endif // UNPACKTRACK_H
