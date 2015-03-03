// $Id: $
#ifndef UNPACKPARTICLESANDVERTICES_H
#define UNPACKPARTICLESANDVERTICES_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Kernel/Particle2LHCbIDs.h"

#include "Event/StandardPacker.h"
#include "Relations/Relation1D.h"

#include "Event/PackedTrack.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedParticle.h"
#include "Event/PackedVertex.h"
#include "Event/PackedRelations.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedFlavourTag.h"
#include "Event/PackedPartToRelatedInfoRelation.h"

#include "Event/Track.h"
#include "Event/ProtoParticle.h"
#include "Event/MuonPID.h"
#include "Event/Particle.h"
#include "Event/Vertex.h"
#include "Event/FlavourTag.h"
#include "Event/RecVertex.h"
#include "Event/MCParticle.h"
#include "Event/RelatedInfoMap.h"

/** @class UnpackParticlesAndVertices UnpackParticlesAndVertices.h
 *
 *  Unpacks all Packed Particles and related objects
 *
 *  @author Olivier Callot
 *  @date   2012-01-23
 */
class UnpackParticlesAndVertices : public GaudiAlgorithm
{

public:

  /// Standard constructor
  UnpackParticlesAndVertices( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~UnpackParticlesAndVertices( ); ///< Destructor

  virtual StatusCode execute(); ///< Algorithm execution

private:

  template < class FROM, class TO, class FROMCONT, class TOCONT >
  void unpackP2PRelations( const std::string & location );

  template < class FROM, class TO, class FROMCONT >
  void unpackP2IntRelations( const std::string & location );

private:

  std::string m_inputStream; ///< Input stream root

  std::string m_postFix;

  StandardPacker m_pack; ///< Standard Packer

};

template < class FROM, class TO, class FROMCONT, class TOCONT >
inline void
UnpackParticlesAndVertices::unpackP2PRelations( const std::string & location )
{
  typedef LHCb::Relation1D<FROM,TO> RELATION;

  unsigned int nbRelContainer(0), nbRel(0);

  RELATION * rels = NULL;
  LHCb::PackedRelations* prels = getIfExists<LHCb::PackedRelations>( location );
  if ( NULL != prels )
  {
    for ( std::vector<LHCb::PackedRelation>::iterator itR = prels->relations().begin();
          prels->relations().end() != itR; ++itR )
    {
      const LHCb::PackedRelation& prel = *itR;
      int indx = prel.container >> 32;
      const std::string & containerName = prels->linkMgr()->link( indx )->path() + m_postFix;
      rels = new RELATION();
      put( rels, containerName );
      ++nbRelContainer;
      FROMCONT * srcContainer = NULL;
      int prevSrcLink = -1;
      DataObject* dstContainer = NULL;
      int prevDstLink = -1;
      for ( int kk = prel.start;  prel.end > kk; ++kk )
      {
        int srcLink(0), srcKey(0);
        m_pack.indexAndKey64( prels->sources()[kk], srcLink, srcKey );
        if ( srcLink != prevSrcLink )
        {
          prevSrcLink = srcLink;
          const std::string & srcName = prels->linkMgr()->link( srcLink )->path();
          srcContainer = get<FROMCONT>( srcName );
        }
        FROM* from = srcContainer->object( srcKey );
        int dstLink(0), dstKey(0);
        m_pack.indexAndKey64( prels->dests()[kk], dstLink, dstKey );
        if ( dstLink != prevDstLink )
        {
          prevDstLink = dstLink;
          const std::string & dstName = prels->linkMgr()->link( dstLink )->path();
          dstContainer = get<DataObject>( dstName );
        }
        TOCONT * _to = dynamic_cast<TOCONT*>(dstContainer);
        TO* to = ( _to ? _to->object(dstKey) : NULL );
        if ( !to ) info() << "Unknown objec: Container type " << (dstContainer->clID()>>16)
                          << "+" << (dstContainer->clID()&0xFFFF)
                          << " key " << dstKey << endmsg;
        rels->relate( from, to );
        ++nbRel;
      }
    }
  }

  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Retrieved " << nbRel << " relations in " << nbRelContainer << " containers"
            << " from " << location
            << endmsg;
  }

}

template < class FROM, class TO, class FROMCONT >
inline void
UnpackParticlesAndVertices::unpackP2IntRelations( const std::string & location )
{
  typedef LHCb::Relation1D<FROM,TO> RELATION;

  unsigned int nbRelContainer(0), nbRel(0);

  LHCb::PackedRelations* prels = getIfExists<LHCb::PackedRelations>( location );
  if ( NULL != prels )
  {
    for ( std::vector<LHCb::PackedRelation>::iterator itR = prels->relations().begin();
          prels->relations().end() != itR; ++itR )
    {
      const LHCb::PackedRelation& prel = *itR;
      const int indx = prel.container >> 32;
      const std::string & containerName = prels->linkMgr()->link(indx)->path() + m_postFix;
      RELATION * rels = new RELATION();
      put( rels, containerName );
      ++nbRelContainer;
      FROMCONT * srcContainer = NULL;
      int prevSrcLink = -1;
      for ( int kk = prel.start; prel.end > kk; ++kk )
      {
        int srcLink(0), srcKey(0);
        m_pack.indexAndKey64( prels->sources()[kk], srcLink, srcKey );
        if ( srcLink != prevSrcLink )
        {
          prevSrcLink = srcLink;
          const std::string & srcName = prels->linkMgr()->link( srcLink )->path();
          srcContainer = get<FROMCONT>( srcName );
        }
        FROM * from = srcContainer->object( srcKey );
        TO to       = (TO) prels->dests()[kk];
        rels->relate( from, to );
        ++nbRel;
      }
    }
  }

  if ( msgLevel(MSG::DEBUG) )
  {
    debug() << "Retrieved " << nbRel << " relations in " << nbRelContainer << " containers"
            << " from " << location
            << endmsg;
  }

}

#endif // UNPACKPARTICLESANDVERTICES_H
