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
#include "Event/PackedProtoParticle.h"
#include "Event/ProtoParticle.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareProtoParticle
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareProtoParticle CompareProtoParticle.h
 *  Compare two containers of ProtoParticles
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
struct CompareProtoParticle
    : Gaudi::Functional::Consumer<void( LHCb::ProtoParticles const&, LHCb::ProtoParticles const& )> {

  /// Standard constructor
  CompareProtoParticle( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::ProtoParticleLocation::Charged},
                  KeyValue{"TestName", LHCb::ProtoParticleLocation::Charged + "Test"}}} {}

  void operator()( LHCb::ProtoParticles const& old, LHCb::ProtoParticles const& test ) const override {
    // check and return
    const LHCb::ProtoParticlePacker packer( this );
    packer.check( old, test )
        .orThrow( "CompareProtoParticle failed", "CompareProtoParticle" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
  }
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareProtoParticle )
