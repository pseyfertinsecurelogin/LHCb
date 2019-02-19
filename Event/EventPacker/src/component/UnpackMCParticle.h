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
#ifndef UNPACKMCPARTICLE_H
#define UNPACKMCPARTICLE_H 1

#include "Event/MCParticle.h"
#include "Event/PackedMCParticle.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/DataHandle.h"

/**
 * The algorithm provides access to previously packed MC particles at a TES location.
 * It reads a location of packed MC particles and converts them to LHCb::MCParticle
 * together with the associated LHCb::MCVertex which is unpacked by the algorithm UnpackMCVertex
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
class UnpackMCParticle : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  StatusCode execute() override;

private:
  DataObjectReadHandle<LHCb::PackedMCParticles> m_packedMCParticles{this, "InputName",
                                                                    LHCb::PackedMCParticleLocation::Default};
  DataObjectWriteHandle<LHCb::MCParticles>      m_MCParticles{this, "OutputName", LHCb::MCParticleLocation::Default};
  Gaudi::Property<bool>                         m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                       "Flag to turn on the creation of output, even when input is missing"};
};

#endif // UNPACKMCPARTICLE_H
