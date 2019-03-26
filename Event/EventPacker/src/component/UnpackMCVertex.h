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
#ifndef UNPACKMCVERTEX_H
#define UNPACKMCVERTEX_H 1

#include "Event/MCVertex.h"
#include "Event/PackedMCVertex.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/DataHandle.h"

/**
 * The algorithm provides access to previously packed MC vertices at a TES location.
 * It reads a location of packed MC vertices and converts them to LHCb::MCVertex
 * together with the associated LHCb::MCParticle which is unpacked by the algorithm UnpackMCParticle
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class UnpackMCVertex : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  StatusCode execute() override;

private:
  DataObjectReadHandle<LHCb::PackedMCVertices> m_packedMCVertices{this, "InputName",
                                                                  LHCb::PackedMCVertexLocation::Default};
  DataObjectWriteHandle<LHCb::MCVertices>      m_MCVertices{this, "OutputName", LHCb::MCVertexLocation::Default};
  Gaudi::Property<bool>                        m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                       "Flag to turn on the creation of output, even when input is missing"};
};

#endif // UNPACKMCVERTEX_H
