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
#ifndef PACKCLUSTER_H
#define PACKCLUSTER_H 1

#include <sstream>

#include "Event/PackedCluster.h"
#include "GaudiAlg/GaudiAlgorithm.h"

/** @class PackCluster PackCluster.h
 *
 *  Pack the cluster on the tracks of a specified container.
 *
 *  @author Olivier Callot
 *  @date   2012-03-05
 */
class PackCluster : public GaudiAlgorithm {

public:
  /// Standard constructor
  PackCluster( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  std::string m_inputName;   ///< Input Track location
  std::string m_outputName;  ///< Output location for packed clusters
  std::string m_veloClusLoc; ///< Velo clusters location
  std::string m_ttClusLoc;   ///< TT clusters location
  std::string m_utClusLoc;   ///< UT clusters location
  std::string m_itClusLoc;   ///< IT clusters location

  bool m_alwaysOutput; ///< Flag to turn on the creation of output, even when input is missing
};

#endif // PACKCLUSTER_H
