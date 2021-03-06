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
#ifndef UNPACKPARTICLESANDVERTICES_H
#define UNPACKPARTICLESANDVERTICES_H 1

#include <memory>

#include "GaudiAlg/GaudiAlgorithm.h"

#include "Kernel/Particle2LHCbIDs.h"

#include "Relations/Relation1D.h"
#include "Relations/RelationWeighted1D.h"

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
#include "Event/StandardPacker.h"

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

/** @class UnpackParticlesAndVertices UnpackParticlesAndVertices.h
 *
 *  Unpacks all Packed Particles and related objects
 *
 *  @author Olivier Callot
 *  @date   2012-01-23
 */
class UnpackParticlesAndVertices : public GaudiAlgorithm {

public:
  /// Standard constructor
  UnpackParticlesAndVertices( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  template <class FROM, class TO, class FROMCONT, class TOCONT>
  void unpackP2PRelations( const std::string& location );

  template <class FROM, class TO, class FROMCONT, class TOCONT, class WEIGHT>
  void unpackP2PWeightedRelations( const std::string& location );

  template <class FROM, class TO, class FROMCONT>
  void unpackP2IntRelations( const std::string& location );

private:
  std::string m_inputStream; ///< Input stream root

  std::string m_postFix;

  StandardPacker m_pack{this}; ///< Standard Packer

  /// Track packer
  const LHCb::TrackPacker m_trackPacker{this};
  /// Muon PID packer
  const LHCb::MuonPIDPacker m_muonPacker{this};
  /// Rich PID packer
  const LHCb::RichPIDPacker m_richPacker{this};
  /// ProtoParticle packer
  const LHCb::ProtoParticlePacker m_protoPacker{this};
  /// Particle Packer
  const LHCb::ParticlePacker m_partPacker{this};
  /// Vertex Packer
  const LHCb::VertexPacker m_vertPacker{this};
  /// Flavour Tag Packer
  const LHCb::FlavourTagPacker m_ftPacker{this};
  /// Rec Vertex Packer
  const LHCb::RecVertexPacker m_rvPacker{this};
  /// Related Info Packer
  const LHCb::RelatedInfoRelationsPacker m_rInfoPacker{this};
};

template <class FROM, class TO, class FROMCONT, class TOCONT>
inline void UnpackParticlesAndVertices::unpackP2PRelations( const std::string& location ) {
  typedef LHCb::Relation1D<FROM, TO> RELATION;

  unsigned int nbRelContainer( 0 ), nbRel( 0 );

  RELATION*                    rels  = nullptr;
  const LHCb::PackedRelations* prels = getIfExists<LHCb::PackedRelations>( location );
  if ( nullptr != prels ) {
    for ( const auto& prel : prels->relations() ) {
      int                indx          = prel.container >> 32;
      const std::string& containerName = prels->linkMgr()->link( indx )->path() + m_postFix;
      rels                             = new RELATION();
      rels->setVersion( prels->version() );
      put( rels, containerName );
      ++nbRelContainer;
      FROMCONT*   srcContainer = nullptr;
      int         prevSrcLink  = -1;
      DataObject* dstContainer = nullptr;
      int         prevDstLink  = -1;
      for ( int kk = prel.start; prel.end > kk; ++kk ) {
        int srcLink( 0 ), srcKey( 0 );
        m_pack.indexAndKey64( prels->sources()[kk], srcLink, srcKey );
        if ( srcLink != prevSrcLink || !srcContainer ) {
          prevSrcLink                = srcLink;
          const std::string& srcName = prels->linkMgr()->link( srcLink )->path();
          // srcContainer = get<FROMCONT>( srcName );
          srcContainer = getIfExists<FROMCONT>( srcName );
          if ( !srcContainer ) {
            Error( "Unpack('" + location + "'): missing source '" + srcName + "', skip link" ).ignore();
            continue;
          }
        }
        FROM* from = srcContainer->object( srcKey );
        int   dstLink( 0 ), dstKey( 0 );
        m_pack.indexAndKey64( prels->dests()[kk], dstLink, dstKey );
        if ( dstLink != prevDstLink || !dstContainer ) {
          prevDstLink                = dstLink;
          const std::string& dstName = prels->linkMgr()->link( dstLink )->path();
          // dstContainer = get<DataObject>( dstName );
          dstContainer = getIfExists<DataObject>( dstName );
          if ( !dstContainer ) {
            Error( "Unpack('" + location + "'): missing destination '" + dstName + "', skip link" ).ignore();
            continue;
          }
        }
        TOCONT* _to = dynamic_cast<TOCONT*>( dstContainer );
        TO*     to  = ( _to ? _to->object( dstKey ) : nullptr );
        if ( !to )
          info() << "Unknown objec: Container type " << ( dstContainer->clID() >> 16 ) << "+"
                 << ( dstContainer->clID() & 0xFFFF ) << " key " << dstKey << endmsg;
        auto sc = rels->relate( from, to );
        if ( !sc ) { Error( "Problem forming relation" ).ignore(); }
        ++nbRel;
      }
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Retrieved " << nbRel << " relations in " << nbRelContainer << " containers"
            << " from " << location << endmsg;
  }
}

template <class FROM, class TO, class FROMCONT, class TOCONT, class WEIGHT>
inline void UnpackParticlesAndVertices::unpackP2PWeightedRelations( const std::string& location ) {
  typedef LHCb::RelationWeighted1D<FROM, TO, WEIGHT> RELATION;

  unsigned int nbRelContainer( 0 ), nbRel( 0 );

  RELATION*                            rels  = nullptr;
  const LHCb::PackedWeightedRelations* prels = getIfExists<LHCb::PackedWeightedRelations>( location );
  if ( nullptr != prels ) {
    for ( const auto& prel : prels->relations() ) {
      int                indx          = prel.container >> 32;
      const std::string& containerName = prels->linkMgr()->link( indx )->path() + m_postFix;
      rels                             = new RELATION();
      rels->setVersion( prels->version() );
      put( rels, containerName );
      ++nbRelContainer;
      FROMCONT*   srcContainer = nullptr;
      int         prevSrcLink  = -1;
      DataObject* dstContainer = nullptr;
      int         prevDstLink  = -1;
      for ( int kk = prel.start; prel.end > kk; ++kk ) {
        int srcLink( 0 ), srcKey( 0 );
        m_pack.indexAndKey64( prels->sources()[kk], srcLink, srcKey );
        if ( srcLink != prevSrcLink || !srcContainer ) {
          prevSrcLink                = srcLink;
          const std::string& srcName = prels->linkMgr()->link( srcLink )->path();
          srcContainer               = getIfExists<FROMCONT>( srcName );
          if ( !srcContainer ) {
            Error( "Unpack('" + location + "'): missing source '" + srcName + "', skip link" ).ignore();
            continue;
          }
        }
        FROM* from = srcContainer->object( srcKey );
        int   dstLink( 0 ), dstKey( 0 );
        m_pack.indexAndKey64( prels->dests()[kk], dstLink, dstKey );
        if ( dstLink != prevDstLink || !dstContainer ) {
          prevDstLink                = dstLink;
          const std::string& dstName = prels->linkMgr()->link( dstLink )->path();
          dstContainer               = getIfExists<DataObject>( dstName );
          if ( !dstContainer ) {
            Error( "Unpack('" + location + "'): missing destination '" + dstName + "', skip link" ).ignore();
            continue;
          }
        }
        const WEIGHT wgt = prels->weights()[kk];
        TOCONT*      _to = dynamic_cast<TOCONT*>( dstContainer );
        TO*          to  = ( _to ? _to->object( dstKey ) : nullptr );
        if ( !to )
          info() << "Unknown objec: Container type " << ( dstContainer->clID() >> 16 ) << "+"
                 << ( dstContainer->clID() & 0xFFFF ) << " key " << dstKey << endmsg;
        auto sc = rels->relate( from, to, wgt );
        if ( !sc ) { Error( "Problem forming weighted relation" ).ignore(); }
        ++nbRel;
      }
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Retrieved " << nbRel << " relations in " << nbRelContainer << " containers"
            << " from " << location << endmsg;
  }
}

template <class FROM, class TO, class FROMCONT>
inline void UnpackParticlesAndVertices::unpackP2IntRelations( const std::string& location ) {
  typedef LHCb::Relation1D<FROM, TO> RELATION;

  unsigned int nbRelContainer( 0 ), nbRel( 0 );

  LHCb::PackedRelations* prels = getIfExists<LHCb::PackedRelations>( location );
  if ( nullptr != prels ) {
    for ( auto itR = prels->relations().begin(); prels->relations().end() != itR; ++itR ) {
      const LHCb::PackedRelation& prel          = *itR;
      const int                   indx          = prel.container >> 32;
      const std::string&          containerName = prels->linkMgr()->link( indx )->path() + m_postFix;
      RELATION*                   rels          = new RELATION();
      rels->setVersion( prels->version() );
      put( rels, containerName );
      ++nbRelContainer;
      FROMCONT* srcContainer = nullptr;
      int       prevSrcLink  = -1;
      for ( int kk = prel.start; prel.end > kk; ++kk ) {
        int srcLink( 0 ), srcKey( 0 );
        m_pack.indexAndKey64( prels->sources()[kk], srcLink, srcKey );
        if ( srcLink != prevSrcLink ) {
          prevSrcLink                = srcLink;
          const std::string& srcName = prels->linkMgr()->link( srcLink )->path();
          srcContainer               = get<FROMCONT>( srcName );
        }
        FROM* from = srcContainer->object( srcKey );
        TO    to   = (TO)prels->dests()[kk];
        auto  sc   = rels->relate( from, to );
        if ( !sc ) { Error( "Problem forming relation" ).ignore(); }
        ++nbRel;
      }
    }
  }

  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Retrieved " << nbRel << " relations in " << nbRelContainer << " containers"
            << " from " << location << endmsg;
  }
}

#endif // UNPACKPARTICLESANDVERTICES_H
