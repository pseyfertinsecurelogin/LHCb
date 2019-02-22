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
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/Particle.h"
#include "Event/Vertex.h"
// ============================================================================
// CaloFutureUtils
// ============================================================================
#include "CaloFutureUtils/CaloFutureParticle.h"
// ============================================================================
/** @file
 *  Implementation file for class LHCb::CaloFutureParticle
 *  @author Olivier Deschamps
 */
// ============================================================================
// constructor from the particle
// ============================================================================
LHCb::CaloFutureParticle::CaloFutureParticle( LHCb::Particle* part ) : m_isCaloFuture( bool( part ) ) {
  addCaloFuturePosition( part );
}
// ============================================================================
// constructor from particle & origin point
// ============================================================================
LHCb::CaloFutureParticle::CaloFutureParticle( LHCb::Particle* part, const LHCb::CaloMomentum::Point& point )
    : m_isCaloFuture( bool( part ) ) {
  setReferencePoint( point );
  addCaloFuturePosition( part );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
}
// ============================================================================
// constructor from particle & origin point & covarinace
// ============================================================================
LHCb::CaloFutureParticle::CaloFutureParticle( LHCb::Particle* part, const LHCb::CaloMomentum::Point& point,
                                              const LHCb::CaloMomentum::PointCovariance& cov )
    : m_isCaloFuture( bool( part ) ) {
  setReferencePoint( point, cov );
  addCaloFuturePosition( part );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
// Particle
// ============================================================================
void LHCb::CaloFutureParticle::addCaloFuturePosition( LHCb::Particle* part ) {
  // --- Fill CaloFuturePosition vector from particle
  //  2 configuration :
  //    - the particle is basic (e.g. photon, mergedPi0)  : use proto->caloggggg
  //    - the particle is composite (e.g. pi0/eta->gg, Ks/B->pi0pi0->gggg, ...) : run along the decay tree

  // Some checks
  addToFlag( LHCb::CaloMomentum::FromPart );
  if ( !part ) {
    addToStatus( LHCb::CaloMomentum::NullPart );
    m_neutral = false;
    return;
  }
  if ( 0 != part->charge() ) {
    addToStatus( LHCb::CaloMomentum::ChargedParticle );
    m_neutral = false;
    return;
  }

  // Check the particle origin (protoParticle)
  if ( part->proto() ) {
    if ( part->proto()->calo().empty() ) {
      m_isCaloFuture = false;
      addToStatus( LHCb::CaloMomentum::PartIsNotCaloObject );
      return;
    }
    // particle derive from proto->caloHypo (-> photon, mergedPi0 )
    m_parts.push_back( part );
    m_caloEndTree.push_back( part );
    LHCb::CaloMomentum::addCaloPosition( part->proto() );
  } else if ( !part->isBasicParticle() ) {
    // particle is composite
    addToFlag( LHCb::CaloMomentum::FromPartDaughters );
    CaloFutureParticleTree( part );
    if ( m_isCaloFuture ) {
      m_parts.push_back( part );
      // the end-tree particles are pure calo objects
      for ( const auto& icalo : m_caloEndTree ) {
        if ( icalo->charge() != 0 ) m_neutral = false;
        LHCb::CaloMomentum::addCaloPosition( icalo->proto() );
      }
    } else {
      //      m_caloEndTree.clear();
      addToStatus( LHCb::CaloMomentum::PartIsNotCaloObject );
    }
  } else {
    //    m_caloEndTree.clear();
    addToStatus( LHCb::CaloMomentum::PartIsNotCaloObject );
  }
}
// ============================================================================
// update the tree
// ============================================================================
void LHCb::CaloFutureParticle::CaloFutureParticleTree( const LHCb::Particle* part ) {

  if ( !m_isCaloFuture ) {
    m_caloEndTree.clear();
    return;
  }
  if ( part->isBasicParticle() ) {
    if ( 0 == part->proto() ) {
      m_isCaloFuture = false;
    } else if ( part->proto()->calo().empty() ) {
      m_isCaloFuture = false;
    } else {
      m_caloEndTree.push_back( part );
    }
  } else {
    for ( const auto& dau : part->daughters() ) CaloFutureParticleTree( const_cast<LHCb::Particle*>( dau.target() ) );
  }
}
// ============================================================================
// update particle
// ============================================================================
void LHCb::CaloFutureParticle::updateParticle() {
  if ( LHCb::CaloMomentum::OK == status() ) {
    for ( LHCb::Particle* part : m_parts ) {
      part->setReferencePoint( referencePoint() );
      part->setPosCovMatrix( pointCovMatrix() );
      part->setMomentum( momentum() );
      part->setMomCovMatrix( momCovMatrix() );
      part->setPosMomCovMatrix( momPointCovMatrix() );
      addToFlag( LHCb::CaloMomentum::ParticleUpdated );
    }
  }
}
// ============================================================================
// update tree
// ============================================================================
void LHCb::CaloFutureParticle::updateTree() {
  updateParticle();

  for ( LHCb::Particle* part : m_parts ) {
    if ( !part->isBasicParticle() && LHCb::CaloMomentum::OK == status() ) {
      for ( const auto& daughter : part->daughters() ) {
        LHCb::CaloFutureParticle caloDau( const_cast<LHCb::Particle*>( daughter.target() ), referencePoint(),
                                          pointCovMatrix() );
        caloDau.updateTree();
      }
      addToFlag( LHCb::CaloMomentum::ParticleTreeUpdated );
    }
  }
}
// ============================================================================
// add vertex
// ============================================================================
void LHCb::CaloFutureParticle::addToVertex( LHCb::Vertex* vertex ) {
  m_vert = vertex;
  for ( const auto& part : m_parts ) vertex->addToOutgoingParticles( part );
  setReferencePoint( vertex );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
// get the particle
// ============================================================================
LHCb::Particle* LHCb::CaloFutureParticle::particle() const { return !m_parts.empty() ? m_parts.front() : nullptr; }
