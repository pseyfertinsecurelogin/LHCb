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
#ifndef PACKPARTICLESANDVERTICES_H
#define PACKPARTICLESANDVERTICES_H 1

#include <sstream>

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/SmartIF.h"

#include "Event/StandardPacker.h"
#include "Kernel/Particle2LHCbIDs.h"

#include "Relations/Relation1D.h"
#include "Relations/RelationWeighted1D.h"

#include "Kernel/LHCbID.h"

#include "Event/PackedFlavourTag.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedParticle.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedRelations.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedTrack.h"
#include "Event/PackedVertex.h"

#include "Event/FlavourTag.h"
#include "Event/MCParticle.h"
#include "Event/MuonPID.h"
#include "Event/Particle.h"
#include "Event/ProtoParticle.h"
#include "Event/RecVertex.h"
#include "Event/RelatedInfoMap.h"
#include "Event/RichPID.h"
#include "Event/Track.h"
#include "Event/Vertex.h"

/** @class PackParticlesAndVertices PackParticlesAndVertices.h
 *
 *  Packs Particles, Vertices and related information.
 *
 *  If one always wants to create the packed locations, regardless of whether there
 *  are any related objects to be packed, one should set the `AlwaysCreateOutput` to
 *  true and set the `AlwaysCreateContainers` list to the desired locations.
 *
 *  For example, the following configures the tool to always create the packed
 *  particles location:
 *
 *  @code
 *  from Configurables import PackParticlesAndVertices
 *  input_stream = '/Event'
 *  ppvs = PackParticlesAndVertices()
 *  ppvs.InputStream = input_stream
 *  ppvs.AlwaysCreateOutput = True
 *  ppvs.AlwaysCreateContainers = [
 *      '{0}/pPhys/Particles'.format(input_stream)
 *  ]
 *  @endcode
 *
 *  @author Olivier Callot
 *  @date   2012-01-23
 */
class PackParticlesAndVertices : public GaudiAlgorithm {

public:
  /// Standard constructor
  PackParticlesAndVertices( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

  StatusCode initialize() override; ///< Initialize the algorithm instance.

private:
  // Relation types
  typedef LHCb::Relation1D<LHCb::Particle, LHCb::VertexBase>                      P2VRELATION;
  typedef LHCb::Relation1D<LHCb::Particle, LHCb::MCParticle>                      P2MCPRELATION;
  typedef LHCb::Relation1D<LHCb::Particle, int>                                   Part2IntRelations;
  typedef LHCb::Relation1D<LHCb::Particle, LHCb::RelatedInfoMap>                  Part2InfoRelations;
  typedef LHCb::RelationWeighted1D<LHCb::ProtoParticle, LHCb::MCParticle, double> Proto2MCPRelation;

  // things for working
  typedef std::vector<IRegistry*>                  Leaves;
  typedef std::map<CLID, std::vector<std::string>> ClassIDTESMap;

private:
  /// Test if a TES container is Veto'ed from being packed
  bool isVetoed( const std::string& id ) const;

  /// Test if an output location should always be created
  bool alwaysCreate( const std::string& id ) const;

  /// Build the ClassID to TES location(s) map
  void buildTESMap( const DataObject* obj, ClassIDTESMap& tesmap ) const;

  /// Pack a Particle container
  void packAParticleContainer( const LHCb::Particles* parts, LHCb::PackedParticles& pparts ) const;

  /// Pack a ProtoParticle container
  void packAProtoParticleContainer( const LHCb::ProtoParticles* protos, LHCb::PackedProtoParticles& pprotos ) const;

  /// Pack a MuonPID container
  void packAMuonPIDContainer( const LHCb::MuonPIDs* pids, LHCb::PackedMuonPIDs& ppids ) const;

  /// Pack a RichPID container
  void packARichPIDContainer( const LHCb::RichPIDs* pids, LHCb::PackedRichPIDs& ppids ) const;

  /// Pack a Track container
  void packATrackContainer( const LHCb::Tracks* tracks, LHCb::PackedTracks& ptracks ) const;

  /// pack a vertex container
  void packAVertexContainer( const LHCb::Vertices* verts, LHCb::PackedVertices& pverts ) const;

  /// Pack a FlavourTag container
  void packAFTContainer( const LHCb::FlavourTags* fts, LHCb::PackedFlavourTags& pfts ) const;

  /// Pack a RecVertex container
  void packARecVertexContainer( const LHCb::RecVertices* rverts, LHCb::PackedRecVertices& prverts ) const;

  /// Pack a 'SmartRef to SmartRef' relations container
  template <typename RELATION>
  void packAP2PRelationContainer( const RELATION* rels, LHCb::PackedRelations& prels ) const;

  /// Pack a 'SmartRef to SmartRef' weighted relations container
  template <typename RELATION>
  void packAP2PRelationContainer( const RELATION* rels, LHCb::PackedWeightedRelations& prels ) const;

  /// Pack a 'SmartRef to int' relations container
  template <typename RELATION>
  void packAP2IntRelationContainer( const RELATION* rels, LHCb::PackedRelations& prels ) const;

  /// Pack a 'SmartRef to RelatedInfoMap' relations container
  void packAP2RelatedInfoRelationContainer( const Part2InfoRelations* rels, LHCb::PackedRelatedInfoRelations& prels,
                                            const std::string& location ) const;

  /// Get an objects location in the TES
  inline std::string objectLocation( const DataObject& pObj ) const {
    return ( pObj.registry() ? pObj.registry()->identifier() : "" );
  }

  /// Copy data object version
  template <typename INPUT, typename OUTPUT>
  void saveVersion( const INPUT& in, OUTPUT& out ) const {
    const int i_ver = in.version();
    const int o_ver = out.version();
    // sanity check
    if ( UNLIKELY( o_ver != 0 && o_ver != i_ver ) ) {
      std::ostringstream mess;
      mess << objectLocation( in ) << " version " << i_ver << " != current packed version " << o_ver;
      Warning( mess.str() ).ignore();
    }
    out.setVersion( i_ver );
  }

private:
  std::string m_inputStream;  ///< Input stream root
  bool        m_alwaysOutput; ///< Flag to turn on the creation of output, even when input is missing
  bool        m_deleteInput;  ///< Delete the containers after packing if true.
  bool        m_enableCheck;  ///< Flag to turn on automatic unpacking and checking of the output post-packing
  std::vector<std::string> m_vetoedConts; ///< Vetoed containers. Will not be packed.
  std::vector<std::string> m_createConts; ///< Always create these containers
  StandardPacker           m_pack;        ///< Standard packer
};

//=========================================================================
//  Pack a 'SmartRef to SmartRef' relations container
//=========================================================================
template <typename RELATION>
inline void PackParticlesAndVertices::packAP2PRelationContainer( const RELATION*        rels,
                                                                 LHCb::PackedRelations& prels ) const {
  // Make a new packed data object and save
  prels.relations().emplace_back( LHCb::PackedRelation() );
  LHCb::PackedRelation& prel = prels.relations().back();

  // reference to original container and key
  prel.container = m_pack.reference64( &prels, rels, 0 );

  // First object
  prel.start = prels.sources().size();

  // reserve size
  const auto newSize = prels.sources().size() + rels->relations().size();
  prels.sources().reserve( newSize );
  prels.dests().reserve( newSize );

  // Loop over relations
  for ( const auto& R : rels->relations() ) {
    prels.sources().emplace_back( m_pack.reference64( &prels, R.from()->parent(), R.from()->key() ) );
    prels.dests().emplace_back( m_pack.reference64( &prels, R.to()->parent(), R.to()->key() ) );
  }

  // last object
  prel.end = prels.sources().size();

  // Clear the registry address of the unpacked container, to prevent reloading
  if ( !m_deleteInput ) rels->registry()->setAddress( 0 );
}

//=========================================================================
//  Pack a 'SmartRef to SmartRef' relations container
//=========================================================================
template <typename RELATION>
inline void PackParticlesAndVertices::packAP2PRelationContainer( const RELATION*                rels,
                                                                 LHCb::PackedWeightedRelations& prels ) const {
  // Make a new packed data object and save
  prels.relations().emplace_back( LHCb::PackedRelation() );
  LHCb::PackedRelation& prel = prels.relations().back();

  // reference to original container and key
  prel.container = m_pack.reference64( &prels, rels, 0 );

  // First object
  prel.start = prels.sources().size();

  // reserve size
  const auto newSize = prels.sources().size() + rels->relations().size();
  prels.sources().reserve( newSize );
  prels.dests().reserve( newSize );
  prels.weights().reserve( newSize );

  // Loop over relations
  for ( const auto& R : rels->relations() ) {
    prels.sources().emplace_back( m_pack.reference64( &prels, R.from()->parent(), R.from()->key() ) );
    prels.dests().emplace_back( m_pack.reference64( &prels, R.to()->parent(), R.to()->key() ) );
    prels.weights().emplace_back( R.weight() );
  }

  // last object
  prel.end = prels.sources().size();

  // Clear the registry address of the unpacked container, to prevent reloading
  if ( !m_deleteInput ) rels->registry()->setAddress( 0 );
}

//=========================================================================
//  Pack a 'SmartRef to int' relations container
//=========================================================================
template <typename RELATION>
inline void PackParticlesAndVertices::packAP2IntRelationContainer( const RELATION*        rels,
                                                                   LHCb::PackedRelations& prels ) const {
  // Make a new packed data object and save
  prels.relations().emplace_back( LHCb::PackedRelation() );
  LHCb::PackedRelation& prel = prels.relations().back();

  // reference to original container and key
  prel.container = m_pack.reference64( &prels, rels, 0 );

  // First object
  prel.start = prels.sources().size();

  // reserve size
  const auto newSize = prels.sources().size() + rels->relations().size();
  prels.sources().reserve( newSize );
  prels.dests().reserve( newSize );

  // Loop over relations
  for ( const auto& R : rels->relations() ) {
    prels.sources().emplace_back( m_pack.reference64( &prels, R.from()->parent(), R.from()->key() ) );
    prels.dests().emplace_back( R.to() );
  }

  // last object
  prel.end = prels.sources().size();

  // Clear the registry address of the unpacked container, to prevent reloading
  if ( !m_deleteInput ) rels->registry()->setAddress( 0 );
}

//==============================================================================
// Test if a TES location is veto'ed
//==============================================================================
inline bool PackParticlesAndVertices::isVetoed( const std::string& id ) const {
  const bool vetoed = ( std::find( m_vetoedConts.begin(), m_vetoedConts.end(), id ) != m_vetoedConts.end() );
  if ( msgLevel( MSG::DEBUG ) ) {
    if ( vetoed ) {
      debug() << "  --> VETO'ed  ... " << id << endmsg;
    } else {
      debug() << "  --> Selected ... " << id << endmsg;
    }
  }
  return vetoed;
}

//==============================================================================
// Test if an output TES location should always be created
//==============================================================================
inline bool PackParticlesAndVertices::alwaysCreate( const std::string& id ) const {
  return m_alwaysOutput && ( std::find( m_createConts.begin(), m_createConts.end(), id ) != m_createConts.end() );
}

#endif // PACKPARTICLESANDVERTICES_H
