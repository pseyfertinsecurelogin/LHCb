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
// STD & STL
// ============================================================================
#include <utility>
#include <vector>
// ============================================================================
// Event
// ============================================================================
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"
#include "Event/ProtoParticle.h"
#include "Event/VertexBase.h"
#include "LHCbMath/Kinematics.h"
// ============================================================================
// Local
// ============================================================================
#include "CaloFutureUtils/CaloMomentum.h"
#include "CaloFutureUtils/Kinematics.h"
// ============================================================================
/** @file
 *  Implementation file for class LHCb::CaloMomentum
 *  @date 2006-08-08
 *  @author Olivier Deschamps
 */
// ============================================================================
// Default Constructor
// ============================================================================
LHCb::CaloMomentum::CaloMomentum() = default;
// ============================================================================
// From CaloPosition assume origin is (0,0,0)
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloPosition* calo ) { addCaloPosition( calo ); }
// ============================================================================
// From CaloPosition + reference point (null covariance)
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloPosition* calo, LHCb::CaloMomentum::Point point )
    : m_point( std::move( point ) ) {
  addCaloPosition( calo );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
}
// ============================================================================
// From CaloPosition + reference point + covariance
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloPosition* calo, LHCb::CaloMomentum::Point point,
                                  const LHCb::CaloMomentum::PointCovariance& cov )
    : m_point( std::move( point ) ), m_pointCovMatrix( cov ) {
  addCaloPosition( calo );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
// From ProtoParticle
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::ProtoParticle* proto ) { addCaloPosition( proto ); }
// ============================================================================
// From ProtoParticle +  XYZPoint
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::ProtoParticle* proto, LHCb::CaloMomentum::Point point )
    : m_point( std::move( point ) ) {
  addCaloPosition( proto );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
}
// ============================================================================
// From (ProtoParticle, XYZPoint, SymMatrix3x3)
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::ProtoParticle* proto, LHCb::CaloMomentum::Point point,
                                  const LHCb::CaloMomentum::PointCovariance& cov )
    : m_point( std::move( point ) ), m_pointCovMatrix( cov ) {
  addCaloPosition( proto );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
// From CaloHypo
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloHypo* hypo ) { addCaloPosition( hypo ); }

// vector of CaloHypos (useful to manage bremList from BremAdder tool)
LHCb::CaloMomentum::CaloMomentum( const std::vector<const LHCb::CaloHypo*> hypos ) {
  std::for_each( hypos.begin(), hypos.end(), [&]( const CaloHypo* h ) { addCaloPosition( h ); } );
}

// ============================================================================
// From (CaloHypo, XYZPoint)
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloHypo* hypo, LHCb::CaloMomentum::Point point )
    : m_point( std::move( point ) ) {
  addCaloPosition( hypo );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
}
// ============================================================================
// From (CaloHypo,XYZPoint, SymMatrix3x3)
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloHypo* hypo, LHCb::CaloMomentum::Point point,
                                  const LHCb::CaloMomentum::PointCovariance& cov )
    : m_point( std::move( point ) ), m_pointCovMatrix( cov ) {
  addCaloPosition( hypo );
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// ============================================================================
// From CaloCluster
// ============================================================================
LHCb::CaloMomentum::CaloMomentum( const LHCb::CaloCluster* cluster ) { addCaloPosition( cluster ); }
// ============================================================================
// CaloPosition
// ============================================================================
void LHCb::CaloMomentum::addCaloPosition( const LHCb::CaloPosition* calopos ) {
  m_flag |= LHCb::CaloMomentum::FromCaloPos;
  if ( calopos ) {
    m_caloPositions.push_back( *calopos );
  } else {
    m_status |= LHCb::CaloMomentum::NullCaloPos;
  }
}
// ============================================================================
// CaloCluster
// ============================================================================
void LHCb::CaloMomentum::addCaloPosition( const LHCb::CaloCluster* cluster ) {
  m_flag |= LHCb::CaloMomentum::FromCaloCluster;
  if ( cluster ) {
    m_caloPositions.push_back( cluster->position() );
  } else {
    m_status |= LHCb::CaloMomentum::NullCaloCluster;
  }
}
// ============================================================================
// CaloHypo
// ============================================================================
void LHCb::CaloMomentum::addCaloPosition( const LHCb::CaloHypo* hypo ) {
  m_flag |= LHCb::CaloMomentum::FromCaloHypo;

  if ( !hypo ) {
    m_status |= LHCb::CaloMomentum::NullCaloHypo;
    return;
  }

  // Check if the hypo has referenced hypos
  const SmartRefVector<LHCb::CaloHypo>& hypos = hypo->hypos();

  if ( hypos.empty() ) {
    // if no : photon, photonFromMerged, ...
    const LHCb::CaloPosition* calopos = hypo->position();
    this->addCaloPosition( calopos );
    m_caloHypos.push_back( hypo );
  } else {
    // if yes : mergedPi0
    for ( const auto& hypo : hypos ) {
      m_flag |= LHCb::CaloMomentum::FromCaloHypo;
      const LHCb::CaloPosition* calopos = hypo->position();
      this->addCaloPosition( calopos );
      m_caloHypos.push_back( hypo );
    }
  }
}
// ============================================================================
// ProtoParticle
// ============================================================================
void LHCb::CaloMomentum::addCaloPosition( const LHCb::ProtoParticle* proto ) {
  m_flag |= LHCb::CaloMomentum::FromProtoPart;

  if ( nullptr == proto ) {
    m_status |= LHCb::CaloMomentum::NullProtoPart;
    return;
  }

  if ( 0 != proto->charge() ) {
    m_status |= LHCb::CaloMomentum::ChargedProtoPart;
    return;
  }
  const SmartRefVector<LHCb::CaloHypo>& hypos = proto->calo();

  if ( hypos.empty() ) {
    m_status |= LHCb::CaloMomentum::EmptyHypoRef;
  } else if ( 1 == hypos.size() ) {
    // Each ProtoPart derive from a single CaloHypo so far
    auto ihypo = hypos.begin();
    this->addCaloPosition( *ihypo );
  } else {
    // If not the case the special configuration should be implemented here
    m_status |= LHCb::CaloMomentum::MultipleHypoRef;
  }
}
// ============================================================================
/// evaluate the parameters
// ============================================================================
bool LHCb::CaloMomentum::evaluate( int param ) {
  if ( LHCb::CaloMomentum::OK != m_status ) { return false; }

  // adapted from PhotonParam.h (thanks to Vanya Belyaev)

  Gaudi::LorentzVector p4;

  LHCb::CaloMomentum::MomCovariance      C11;                    // 4-moment 4x4 covariance matrix
  LHCb::CaloMomentum::PointCovariance    C22 = m_pointCovMatrix; // 3-point  3x3 covariance matrix
  LHCb::CaloMomentum::MomPointCovariance C12;                    // momentum-point 4x3 off-diagonal block

  for ( LHCb::CaloPosition calo : m_caloPositions ) {

    // Evaluate the 4-momentum
    if ( 0 != ( LHCb::CaloMomentum::Momentum & param ) ) {
      if ( calo.e() >= 0. ) p4 += CaloFuture::Kinematics::momentum( calo, m_point );
    }

    // Evaluate everithing the covariance
    if ( 0 != ( LHCb::CaloMomentum::Covariance & param ) ) {

      //  vector of "old" parameters
      //    vold( 1 ) = pos   -> x () ;   // x-position in Calorimeter
      //    vold( 2 ) = pos   -> y () ;   // y-position in Calorimeter
      //    vold( 3 ) = pos   -> e () ;   // energy     in Calorimeter
      //    vold( 4 ) = point .  x () ;   // x-position at production vertex
      //    vold( 5 ) = point .  y () ;   // y-position at production vertex
      //    vold( 6 ) = point .  z () ;   // z-position at production vertex

      // "old" covariance matrix has the block structure
      //     --------------
      //     |  S11 | S12 |
      //     --------------
      //     |  S21 | S22 |
      //     --------------
      // Where S11 is 3x3 covariance matrix of E  , xc and yc
      //   and S22 is 3x3 covariance matrix of xv , yv and zv
      //   matrices S12 and S21 both are equal to null matrix
      //
      const LHCb::CaloPosition::Covariance&      S11 = calo.covariance(); // (X,Y,E)
      const LHCb::CaloMomentum::PointCovariance& S22 = C22;               // (x,y,z)

      //    vnew ( 1 ) = px ;   // x-component of photon momentum
      //    vnew ( 2 ) = py ;   // y-component of photon momentum
      //    vnew ( 3 ) = pz ;   // z-component of photon momentum
      //    vnew ( 4 ) = e  ;   // photon energy
      //    vnew ( 5 ) = x0 ;   // x-component of photon production point
      //    vnew ( 6 ) = y0 ;   // y-component of photon production point
      //    vnew ( 7 ) = z0 ;   // z-component of photon production point

      //  evaluate the "transport matrix"

      //
      //  The transport matrix F = d(vnew) / d(vold)    (7x6)
      //
      //   has block structure
      //        --------------
      //        | F11 |  F12 |
      //        --------------
      //        | F21 |  F22 |
      //        --------------
      //  Where F11 = d( px , py , pz , e) / d ( xc , yc , e  )   4x3 matrix
      //        F12 = d( px , py , pz , e) / d ( xv , yv , zv )   4x3 matrix
      //        F21 = d( x0 , y0 , z0    ) / d ( xc , yc , e  )   3x3 matrix
      //        F22 = d( x0 , y0 , z0    ) / d ( xv , yv , zv )   3x3 matrix
      //
      //  From definition of parameters it is obvious that
      //  F21 is null matrix ans F22 is unit matrix

      LHCb::CaloMomentum::MomPointCovariance F11;
      LHCb::CaloMomentum::MomPointCovariance F12;

      // ok, now all blocks of transport matrix are
      // known and we could evaluate the covariance matrix
      // of new parameters

      // The covariance matrix of new parameters has block structure:
      //     -------------
      //     | C11 | C12 |
      //     -------------
      //     | C21 | C22 |
      //     -------------
      //
      // C = F * S * F^T
      //
      // Where:
      //
      // C11 = F11*S11*F11^T + F12*S21*F11^T + F11*S12*F12^T + F12*S22*F12^T
      // C12 = F11*S11*F21^T + F12*S21*F21^T + F11*S12*F22^T + F12*S22*F22^T
      // C21 = F21*S11*F11^T + F22*S21*F11^T + F21*S12*F12^T + F22*S22*F12^T
      // C22 = F21*S11*F21^T + F22*S21*F21^T + F21*S12*F22^T + F22*S22*F22^T
      //
      // taking into account that S12 = 0 , S21 = 0 , F21 = 0 , F22 = 1 on gets
      //
      // C11 = F11 * S11 * F11^T + F12 * S22 * F12^T
      // C12 = F12 * S22 * F22^T                     = F12 * S22
      // C21 = F22 * S22 * F12^T                     = S22 * F12^T
      // C22 = F22 * S22 * F22^T                     = S22

      // C11 += S11.similarity( F11 ) + S22.similarity( F12 ) ;

      // OK, get all jacobians:
      CaloFuture::Kinematics::jacobians( calo, m_point, F11, F12 );

      // -----------------------
      // Covariance are additive
      // Assume the various CaloPositions are NOT correlated
      // THIS IS NOT THE CASE FOR MERGED PI0
      // @ToDo : adapt to the case of correlated CaloPositions

      C11 += Similarity( F11, S11 ) + Similarity( F12, S22 );
      C12 += F12 * S22;
    }
  }

  if ( 0 != ( LHCb::CaloMomentum::Momentum & param ) ) {
    m_momentum = p4;
    m_flag |= LHCb::CaloMomentum::MomentumEvaluated;
  }

  if ( 0 != ( LHCb::CaloMomentum::Covariance & param ) ) {
    m_momCovMatrix      = C11;
    m_momPointCovMatrix = C12;
    m_flag |= LHCb::CaloMomentum::CovarianceEvaluated;
  }
  return true;
}
// ===========================================================================
// get the momentum
// ===========================================================================
const LHCb::CaloMomentum::Vector& LHCb::CaloMomentum::momentum() {
  evaluate( LHCb::CaloMomentum::Momentum );
  return m_momentum;
}
// ===========================================================================
// get the covariance
// ===========================================================================
const LHCb::CaloMomentum::MomCovariance& LHCb::CaloMomentum::momCovMatrix() {
  evaluate( LHCb::CaloMomentum::Covariance );
  return m_momCovMatrix;
}
// ===========================================================================
// get the covariance
// ===========================================================================
const LHCb::CaloMomentum::MomPointCovariance& LHCb::CaloMomentum::momPointCovMatrix() {
  evaluate( LHCb::CaloMomentum::Covariance );
  return m_momPointCovMatrix;
}
// =============================================================================
// reference point
// =============================================================================
void LHCb::CaloMomentum::setReferencePoint( const LHCb::VertexBase* vertex ) {
  m_point          = vertex->position();
  m_pointCovMatrix = vertex->covMatrix();
  addToFlag( LHCb::CaloMomentum::NewReferencePoint );
  addToFlag( LHCb::CaloMomentum::NewPointCovariance );
}
// =============================================================================
/** error on mass
 *  I suspect that this computation is invalid
 *  for the correct formula  @see LHCb::Math::sigmamass
 */
// =============================================================================
double LHCb::CaloMomentum::emass() {
  evaluate();

  return Gaudi::Math::sigmamass( m_momentum, m_momCovMatrix );
  //  Gaudi::Vector4 momentum( m_momentum.Px () ,
  //                         m_momentum.Py () ,
  //                         m_momentum.Pz () ,
  //                         m_momentum.E  () ) ;
  // return sqrt ( Similarity ( momentum, m_momCovMatrix ) / m_momentum.M2() );
}
