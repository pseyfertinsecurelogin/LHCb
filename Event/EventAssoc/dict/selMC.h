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
// ====================================================================
// Include files
// ====================================================================
// STD & STL
// ====================================================================
#include <string>
#include <vector>
// ====================================================================
// Relations
// ====================================================================
#include "Relations/RelationsDict.h"
// ====================================================================
// Provided include files:
// ====================================================================
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/CaloHypo.h"
#include "Event/GenCollision.h"
#include "Event/L0CaloCandidate.h"
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
#include "Event/Particle.h"
#include "Event/ProtoParticle.h"
#include "Event/RecVertex.h"
#include "Event/Track.h"
#include "HepMC/GenParticle.h"
// ====================================================================
// Other include files: put your own includes here:
// ====================================================================
//#include ...
//#include ...
//#include ...
// ====================================================================

/** @file
 *
 * Helper file for building Reflex dictionaries for Relations
 * @author poluekt
 * @date  2014-04-10 10:29:15.158461
 *
 */

namespace EventAssocDictMC {
  struct __Instantiations {
    GaudiDict::Relation1DDict<LHCb::MCVertex, LHCb::GenCollision>                                          _1;
    GaudiDict::Relation1DDict<LHCb::Particle, LHCb::MCParticle>                                            _2;
    GaudiDict::Relation2DDict<HepMC::GenParticle*, LHCb::MCParticle>                                       _3;
    GaudiDict::Relation2DDict<HepMC::GenParticle*, LHCb::Particle>                                         _4;
    GaudiDict::Relation2DDict<LHCb::MCParticle, HepMC::GenParticle*>                                       _5;
    GaudiDict::Relation2DDict<LHCb::MCParticle, LHCb::Particle>                                            _6;
    GaudiDict::Relation2DDict<LHCb::Particle, HepMC::GenParticle*>                                         _7;
    GaudiDict::Relation2DDict<LHCb::Particle, LHCb::MCParticle>                                            _8;
    GaudiDict::RelationWeighted1DDict<LHCb::CaloCluster, LHCb::MCParticle, float>                          _9;
    GaudiDict::RelationWeighted1DDict<LHCb::CaloDigit, LHCb::MCParticle, float>                            _10;
    GaudiDict::RelationWeighted1DDict<LHCb::MCParticle, LHCb::CaloCluster, float>                          _11;
    GaudiDict::RelationWeighted1DDict<LHCb::MCParticle, LHCb::ProtoParticle, double>                       _12;
    GaudiDict::RelationWeighted1DDict<LHCb::Particle, LHCb::MCParticle, double>                            _13;
    GaudiDict::RelationWeighted1DDict<LHCb::ProtoParticle, LHCb::MCParticle, double>                       _14;
    GaudiDict::RelationWeighted2DDict<LHCb::GenCollision, LHCb::RecVertex, std::pair<unsigned int, float>> _15;
    GaudiDict::RelationWeighted2DDict<LHCb::L0CaloCandidate, LHCb::MCParticle, double>                     _16;
    GaudiDict::RelationWeighted2DDict<LHCb::MCParticle, LHCb::L0CaloCandidate, double>                     _17;
    GaudiDict::RelationWeighted2DDict<LHCb::MCParticle, LHCb::Particle, double>                            _18;
    GaudiDict::RelationWeighted2DDict<LHCb::MCParticle, LHCb::Track, double>                               _19;
    GaudiDict::RelationWeighted2DDict<LHCb::MCVertex, LHCb::RecVertex, std::pair<unsigned int, float>>     _20;
    GaudiDict::RelationWeighted2DDict<LHCb::Particle, LHCb::MCParticle, double>                            _21;
    GaudiDict::RelationWeighted2DDict<LHCb::RecVertex, LHCb::GenCollision, std::pair<unsigned int, float>> _22;
    GaudiDict::RelationWeighted2DDict<LHCb::RecVertex, LHCb::MCVertex, std::pair<unsigned int, float>>     _23;
    GaudiDict::RelationWeighted2DDict<LHCb::Track, LHCb::MCParticle, double>                               _24;
  };
} // end of namespace EventAssocDictMC

// ====================================================================
// The END ============================================================
// ====================================================================
