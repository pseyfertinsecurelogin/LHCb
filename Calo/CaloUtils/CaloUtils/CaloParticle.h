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
#ifndef RecEvent_CaloParticle_H
#define RecEvent_CaloParticle_H 1
// ============================================================================
// Include files
// ============================================================================
// CaloUtils
// ============================================================================
#include "CaloUtils/CaloMomentum.h"
// ============================================================================
// Event
// ============================================================================
#include "Event/Particle.h"
// ============================================================================
// Forward declarations
// ============================================================================
namespace LHCb {
  class Vertex;
}
// ============================================================================
namespace LHCb {
  // ==========================================================================
  /** @class CaloMomentum CaloUtils/CaloMomentum.h
   *  Helper class to evaluate the parameters of "Calo"-particles
   *  @author Olivier Deschamps
   */
  class CaloParticle final : public LHCb::CaloMomentum {
    // ========================================================================
  public:
    // ========================================================================
    /// Constructor fom particle
    CaloParticle( LHCb::Particle* part );
    /// Constructor fom particle & origin vertex
    CaloParticle( LHCb::Particle* part, const LHCb::CaloMomentum::Point& point );
    /// Constructor fom particle & origin vertex & covariance
    CaloParticle( LHCb::Particle* part, const LHCb::CaloMomentum::Point& point,
                  const LHCb::CaloMomentum::PointCovariance& cov );
    // ========================================================================
  public:
    // ========================================================================
    // Setters
    // ========================================================================
    void addCaloPosition( LHCb::Particle* part );
    void addToVertex( LHCb::Vertex* vertex );
    // ========================================================================
  public:
    // ========================================================================
    // Getters
    // ========================================================================
    const std::vector<LHCb::Particle*>& particles() const { return m_parts; }
    const LHCb::Vertex*                 originVertex() const { return m_vert; }
    const LHCb::Particle::ConstVector&  caloEndTree() const { return m_caloEndTree; }
    // get (the first) particle
    LHCb::Particle* particle() const;
    // calo?
    bool isCalo() const { return m_isCalo; }
    bool isPureNeutralCalo() const { return m_isCalo && m_neutral; }
    // ========================================================================
  public:
    // ========================================================================
    // Update
    // ========================================================================
    void updateParticle();
    void updateTree();
    void CaloParticleTree( const LHCb::Particle* part );
    // ========================================================================
  private:
    // ========================================================================
    LHCb::Particle::Vector      m_parts;
    LHCb::Vertex*               m_vert    = nullptr;
    bool                        m_isCalo  = true;
    bool                        m_neutral = true;
    LHCb::Particle::ConstVector m_caloEndTree;
    // ========================================================================
  }; // class CaloParticle
  // ==========================================================================
} // end of namespace LHCb
#endif /// RecEvent_CaloParticle_H
