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
#include "Event/CaloHypo.h"
#include "Event/Particle.h"
#include "Event/RelatedInfoMap.h"
#include "Event/Track.h"
#include "Event/VertexBase.h"
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
 * @date  2014-04-10 10:29:15.087139
 *
 */

namespace EventAssocDictPhys {
  struct __Instantiations {
    GaudiDict::Relation1DDict<LHCb::Particle, LHCb::VertexBase>                 _1;
    GaudiDict::Relation1DDict<LHCb::Particle, LHCb::RelatedInfoMap>             _2;
    GaudiDict::Relation1DDict<LHCb::Particle, double>                           _3;
    GaudiDict::Relation1DDict<LHCb::Particle, int>                              _4;
    GaudiDict::Relation1DDict<LHCb::Track, float>                               _5;
    GaudiDict::RelationWeighted1DDict<LHCb::Particle, LHCb::Particle, double>   _6;
    GaudiDict::RelationWeighted1DDict<LHCb::Particle, LHCb::VertexBase, double> _7;
    GaudiDict::RelationWeighted2DDict<LHCb::CaloCluster, LHCb::Track, float>    _8;
    GaudiDict::RelationWeighted2DDict<LHCb::CaloHypo, LHCb::Track, float>       _9;
    GaudiDict::RelationWeighted2DDict<LHCb::Track, LHCb::CaloCluster, float>    _10;
    GaudiDict::RelationWeighted2DDict<LHCb::Track, LHCb::CaloHypo, float>       _11;
  };
} // end of namespace EventAssocDictPhys

// ====================================================================
// The END ============================================================
// ====================================================================
