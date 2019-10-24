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
#ifndef CaloFutureUtils_CaloMomentum_H
#define CaloFutureUtils_CaloMomentum_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/GenericMatrixTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/Vector4DTypes.h"
// ============================================================================
// Eevnt
// ============================================================================
#include "Event/CaloPosition.h"
// ============================================================================
// Forward declarations
// ============================================================================
namespace LHCb {
  class CaloCluster;
  class CaloHypo;
  class ProtoParticle;
  class VertexBase;
} // namespace LHCb
// ============================================================================
namespace LHCb {
  // ==========================================================================
  /** @class CaloMomentum CaloFutureUtils/CaloMomentum.h
   *  Helper class to evaluate the parameters of "Calo"-particles
   *  @author Olivier Deschamps
   *  @date 2006-08-08
   */
  class CaloMomentum {
    // ========================================================================
  public:
    // ========================================================================
    /// 4-momentum (px,py,pz,e)
    using Vector = Gaudi::LorentzVector;
    ///  3D point (x0,y0,z0)
    using Point = Gaudi::XYZPoint;
    /// 4x4 Covariance matrix (px,py,pz,e)
    using MomCovariance = Gaudi::SymMatrix4x4;
    /// 3x3 Covariance matrix (x0,y0,z0)
    using PointCovariance = Gaudi::SymMatrix3x3;
    /// 4x3 pos-mom  covariance matrix off-diagonal block
    using MomPointCovariance = Gaudi::Matrix4x3;
    /// vector of CaloPositions
    using CaloPositions = std::vector<LHCb::CaloPosition>;
    /// vector of CaloHypos
    using CaloHypos = std::vector<const LHCb::CaloHypo*>;
    // ========================================================================
  public:
    // ========================================================================
    /// indices to access Px,Py,Pz,E
    enum Index { Px = 0, Py = 1, Pz = 2, E = 3 };
    // ========================================================================
    enum Parameter { Momentum = 1, Covariance = 2 };
    // ========================================================================
    enum Status {
      OK                  = 0,
      NullCaloCluster     = 1,
      NullCaloPos         = 2,
      NullCaloHypo        = 4,
      NullProtoPart       = 8,
      ChargedProtoPart    = 16,
      EmptyHypoRef        = 32,
      MultipleHypoRef     = 64,
      NullPart            = 128,
      ChargedParticle     = 256,
      PartIsNotCaloObject = 512
    };
    // ========================================================================
    enum Flag {
      Empty               = 0,
      MomentumEvaluated   = 1,
      CovarianceEvaluated = 2,
      FromCaloCluster     = 4,
      FromCaloPos         = 8,
      FromCaloHypoRef     = 16,
      FromCaloHypo        = 32,
      FromProtoPart       = 64,
      FromPartDaughters   = 128,
      FromPart            = 256,
      ParticleUpdated     = 512,
      ParticleTreeUpdated = 1024,
      NewPointCovariance  = 2048,
      NewReferencePoint   = 4096
    };
    // ========================================================================
  public: // constructors
    // ========================================================================
    /// Default Constructor
    CaloMomentum();
    // ========================================================================
    /// Special constructors from CaloPosition class
    CaloMomentum( const LHCb::CaloPosition* calo );
    /// From CaloPosition + reference point (null covariance)
    CaloMomentum( const LHCb::CaloPosition* calo, Point point );
    /// From CaloPosition + reference point + covariance
    CaloMomentum( const LHCb::CaloPosition* calo, Point point, const PointCovariance& cov );
    // ========================================================================
    /// Special constructors from ProtoParticle
    CaloMomentum( const LHCb::ProtoParticle* proto );
    CaloMomentum( const LHCb::ProtoParticle* proto, Point point );
    CaloMomentum( const LHCb::ProtoParticle* proto, Point point, const PointCovariance& cov );
    // ========================================================================
    /// Special constructors from CaloHypo
    CaloMomentum( const std::vector<const LHCb::CaloHypo*> hypos );
    CaloMomentum( const LHCb::CaloHypo* hypo );
    CaloMomentum( const LHCb::CaloHypo* hypo, Point point );
    CaloMomentum( const LHCb::CaloHypo* hypo, Point point, const PointCovariance& cov );
    // ========================================================================
    /// Special constrctor form the CaloCluster
    CaloMomentum( const LHCb::CaloCluster* cluster );
    // ========================================================================
    /// Destructor
    virtual ~CaloMomentum() = default;
    // ========================================================================
  public:
    // ========================================================================
    void addCaloPosition( const LHCb::CaloPosition* pos );
    void addCaloPosition( const LHCb::CaloCluster* clus );
    void addCaloPosition( const LHCb::CaloHypo* hypo );
    void addCaloPosition( const LHCb::ProtoParticle* proto );
    // ========================================================================
  public:
    // ========================================================================
    // Getters (const)
    // ========================================================================
    [[nodiscard]] const CaloPositions&   caloPositions() const { return m_caloPositions; }
    [[nodiscard]] const Point&           referencePoint() const { return m_point; }
    [[nodiscard]] const PointCovariance& pointCovMatrix() const { return m_pointCovMatrix; }
    [[nodiscard]] unsigned int           multiplicity() const { return m_caloPositions.size(); }
    [[nodiscard]] int                    status() const { return m_status; }
    [[nodiscard]] int                    flag() const { return m_flag; }
    // ========================================================================
  public:
    // ========================================================================
    // Getters (non-const)
    // ========================================================================
    const Vector&             momentum();
    const MomCovariance&      momCovMatrix();
    const MomPointCovariance& momPointCovMatrix();
    // ========================================================================
  public:
    // ========================================================================
    // Setters
    // ========================================================================
    void setCaloPositions( const CaloPositions& value ) { m_caloPositions = value; }
    void resetCaloPositions() { m_caloPositions.clear(); }
    //

    CaloHypos caloHypos();
    void      setReferencePoint( const Point& point );
    void      setPosCovMatrix( const PointCovariance& cov );
    void      setReferencePoint( const Point& point, const PointCovariance& cov );
    void      setReferencePoint( const LHCb::VertexBase* vertex );
    //
    void addToStatus( Status status ) { m_status |= status; }
    void addToFlag( Flag flag ) { m_flag |= flag; }
    // ========================================================================
  public:
    // ========================================================================
    // Shortcuts
    // ========================================================================
    double e() { return momentum().E(); }
    double px() { return momentum().Px(); }
    double py() { return momentum().Py(); }
    double pz() { return momentum().Pz(); }
    double pt() { return momentum().Pt(); }
    double mass() { return momentum().M(); }
    double emass();
    // ========================================================================
  public:
    // ========================================================================
    /// Evaluators
    bool evaluate( int param );
    bool evaluate() { return evaluate( Momentum & Covariance ); }
    // ========================================================================
  private:
    // ========================================================================
    /// vector of Calo-positions
    CaloPositions m_caloPositions; //     vector of Calo-positions
    /// vector of CaloHypos
    CaloHypos m_caloHypos; //     vector of CaloHypos
    /// origin point
    Point m_point; //                 origin point
    /// origin point covariance
    PointCovariance m_pointCovMatrix; //      origin point covariance
    /// 4-momentum
    Vector m_momentum; //                   4-momentum
    /// momentum covariance matrix
    MomCovariance m_momCovMatrix; //   momentum covariance matrix
    /// position-momentum correlation matrix
    MomPointCovariance m_momPointCovMatrix; //       the correlation matrix
    // ========================================================================
  private: // auxillary
    // ========================================================================
    int m_status = LHCb::CaloMomentum::OK;    //                   the status
    int m_flag   = LHCb::CaloMomentum::Empty; //                         flag
    // ========================================================================
  }; // class CaloMomentum
  // ==========================================================================
} // end of namespace LHCb
// ============================================================================

inline LHCb::CaloMomentum::CaloHypos LHCb::CaloMomentum::caloHypos() { return m_caloHypos; }

inline void LHCb::CaloMomentum::setReferencePoint( const LHCb::CaloMomentum::Point& point ) {
  m_point = point;
  this->addToFlag( LHCb::CaloMomentum::NewReferencePoint );
}
// ============================================================================
inline void LHCb::CaloMomentum::setPosCovMatrix( const LHCb::CaloMomentum::PointCovariance& cov ) {
  m_pointCovMatrix = cov;
  this->addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
inline void LHCb::CaloMomentum::setReferencePoint( const LHCb::CaloMomentum::Point&           point,
                                                   const LHCb::CaloMomentum::PointCovariance& cov ) {
  m_point          = point;
  m_pointCovMatrix = cov;
  this->addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  this->addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
#endif // CaloFutureUtils_CaloMomentum_H
