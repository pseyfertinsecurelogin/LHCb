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
#ifndef UNPACKUTCLUSTER_H
#define UNPACKUTCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class UnpackUTCluster UnpackUTCluster.h
 *
 *  Unpack the LHCb::UTCluster objects from a specified container.
 *
 *  This is a single-container-type version of UnpackCluster, and assumes a
 *  single input -> output mapping.
 */
class UnpackUTCluster : public GaudiAlgorithm
{

public:

  UnpackUTCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::PackedClusterLocation::UT};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::UTClusterLocation::UTClusters};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // UNPACKUTCLUSTER_H
