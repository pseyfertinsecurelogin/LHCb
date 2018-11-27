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
#ifndef PACKSTCLUSTER_H
#define PACKSTCLUSTER_H 1

#include "Event/PackedCluster.h"

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class PackSTCluster PackSTCluster.h
 *
 *  Pack the LHCb::STCluster objects from a specified container.
 */
class PackSTCluster : public GaudiAlgorithm
{

public:

  PackSTCluster(const std::string& name, ISvcLocator* svcLocator);

  StatusCode execute() override;

private:

  Gaudi::Property<std::string> m_inputName{this, "InputName", LHCb::STClusterLocation::TTClusters};

  Gaudi::Property<std::string> m_outputName{this, "OutputName", LHCb::PackedClusterLocation::TT};

  /// Flag to turn on the removal of the input data after packing
  Gaudi::Property<bool> m_deleteInput{this, "DeleteInput", false};

  /// Flag to turn on the creation of output, even when input is missing
  Gaudi::Property<bool> m_alwaysOutput{this, "AlwaysCreateOutput", false};
};

#endif // PACKSTCLUSTER_H
