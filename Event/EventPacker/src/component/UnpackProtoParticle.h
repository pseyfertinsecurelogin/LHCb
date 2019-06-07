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
#ifndef UNPACKPROTOPARTICLE_H
#define UNPACKPROTOPARTICLE_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"

/** @brief Unpack a PackedProtoParticle container to ProtoParticles.
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
class UnpackProtoParticle : public GaudiAlgorithm {

public:
  using GaudiAlgorithm::GaudiAlgorithm;
  StatusCode execute() override;

private:
  DataObjectReadHandle<LHCb::PackedProtoParticles> m_packedProtos{this, "InputName",
                                                                  LHCb::PackedProtoParticleLocation::Charged};
  DataObjectWriteHandle<LHCb::ProtoParticles>      m_protos{this, "OutputName", LHCb::ProtoParticleLocation::Charged};
  Gaudi::Property<bool>                            m_alwaysOutput{this, "AlwaysCreateOutput", false,
                                       "Flag to turn on the creation of output, even when input is missing"};
};

#endif // UNPACKPROTOPARTICLE_H
