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
#include "Linker/LinkedFrom.h"
#include "Linker/LinkedTo.h"

#include "Event/CaloDigit.h"
#include "Event/FTCluster.h"
#include "Event/MCHit.h"
#include "Event/MCParticle.h"
#include "Event/MuonCoord.h"
#include "Event/MuonDigit.h"
#include "Event/Particle.h"
#include "Event/STCluster.h"
#include "Event/Track.h"
#include "Event/VPCluster.h"
#include "Event/VeloCluster.h"
#include "Kernel/OTChannelID.h"

namespace {
  struct _Instantiations {
    LinkedTo<LHCb::MCParticle, LHCb::VeloCluster> i1;
    LinkedTo<LHCb::MCParticle, LHCb::VPCluster>   i31;
    LinkedTo<LHCb::MCParticle, LHCb::FTCluster>   i41;
    LinkedTo<LHCb::MCParticle, LHCb::STCluster>   i2;
    LinkedTo<LHCb::MCParticle, ContainedObject>   i3;
    LinkedTo<LHCb::MCParticle, LHCb::MuonCoord>   i4;
    LinkedTo<LHCb::MCParticle, LHCb::MuonDigit>   i7;
    LinkedTo<LHCb::MCParticle, LHCb::CaloDigit>   i8;
    LinkedTo<LHCb::MCParticle, LHCb::Track>       i5;
    LinkedTo<LHCb::MCParticle, LHCb::MCHit>       i6;
    LinkedTo<LHCb::MCParticle, LHCb::Particle>    i10;
    LinkedTo<LHCb::MCParticle>                    i9;

    LinkedFrom<LHCb::VeloCluster, LHCb::MCParticle> i101;
    LinkedFrom<LHCb::VPCluster, LHCb::MCParticle>   i301;
    LinkedFrom<LHCb::FTCluster, LHCb::MCParticle>   i401;
    LinkedFrom<LHCb::STCluster, LHCb::MCParticle>   i102;
    LinkedFrom<ContainedObject, LHCb::MCParticle>   i103;
    LinkedFrom<LHCb::MuonCoord, LHCb::MCParticle>   i104;
    LinkedFrom<LHCb::MuonDigit, LHCb::MCParticle>   i107;
    LinkedFrom<LHCb::CaloDigit, LHCb::MCParticle>   i108;
    LinkedFrom<LHCb::Track, LHCb::MCParticle>       i105;
    LinkedFrom<LHCb::MCHit, LHCb::MCParticle>       i106;
    LinkedFrom<LHCb::Particle, LHCb::MCParticle>    i110;
    LinkedFrom<LHCb::MCParticle>                    i109;
  };
} // namespace
