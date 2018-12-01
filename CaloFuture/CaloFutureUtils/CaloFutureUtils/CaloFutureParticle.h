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
#ifndef RecEvent_CaloFutureParticle_H
#define RecEvent_CaloFutureParticle_H 1
// ============================================================================
// Include files
// ============================================================================
// CaloFutureUtils
// ============================================================================
#include "CaloFutureUtils/CaloMomentum.h"
// ============================================================================
// Event
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// Forward declarations
// ============================================================================
namespace LHCb { class Vertex  ; }
// ============================================================================
namespace LHCb{
  // ==========================================================================
  /** @class CaloMomentum CaloFutureUtils/CaloMomentum.h
   *  Helper class to evaluate the parameters of "Calo"-particles
   *  @author Olivier Deschamps
   */
  class CaloFutureParticle final : public LHCb::CaloMomentum {
    // ========================================================================
  public:
    // ========================================================================
    /// Constructor fom particle
    CaloFutureParticle( LHCb::Particle*                      part  ) ;
    /// Constructor fom particle & origin vertex
    CaloFutureParticle( LHCb::Particle*                      part  ,
                  const LHCb::CaloMomentum::Point&           point ) ;
    /// Constructor fom particle & origin vertex & covariance
    CaloFutureParticle( LHCb::Particle*                      part  ,
                  const LHCb::CaloMomentum::Point&           point ,
                  const LHCb::CaloMomentum::PointCovariance& cov   ) ;
    // ========================================================================
  public:
    // ========================================================================
    // Setters
    // ========================================================================
    void addCaloFuturePosition( LHCb::Particle*  part   ) ;
    void addToVertex    ( LHCb::Vertex*    vertex ) ;
    // ========================================================================
  public:
    // ========================================================================
    // Getters
    // ========================================================================
    const std::vector<LHCb::Particle*>& particles   () const
    { return m_parts      ; }
    const LHCb::Vertex*                originVertex () const
    { return m_vert       ; }
    const LHCb::Particle::ConstVector& caloEndTree  () const
    { return m_caloEndTree; }
    // get (the first) particle
    LHCb::Particle* particle() const ;
    // calo?
    bool isCaloFuture() const { return m_isCaloFuture; }
    bool isPureNeutralCaloFuture() const { return m_isCaloFuture && m_neutral; }
    // ========================================================================
  public:
    // ========================================================================
    // Update
    // ========================================================================
    void updateParticle   () ;
    void updateTree       () ;
    void CaloFutureParticleTree ( const LHCb::Particle*  part ) ;
    // ========================================================================
  private:
    // ========================================================================
    LHCb::Particle::Vector      m_parts       ;
    LHCb::Vertex*               m_vert        = nullptr;
    bool                        m_isCaloFuture      = true;
    bool                        m_neutral     = true;
    LHCb::Particle::ConstVector m_caloEndTree ;
    // ========================================================================
  }; // class CaloFutureParticle
  // ==========================================================================
} // end of namespace LHCb
#endif ///RecEvent_CaloFutureParticle_H
// ============================================================================
