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

// local
#include "Event/TwoProngVertex.h"
#include "Event/TrackParameters.h"

// STL
#include <cmath>

//-----------------------------------------------------------------------------
// Implementation file for class : TwoProngVertex
//
// 2007-10-16 : Wouter HULSBERGEN, Sean BRISBANE
//-----------------------------------------------------------------------------

namespace LHCb {

  void TwoProngVertex::GetArmenteros_Podolanski( double& qT, double& alpha ) const {
    const double           pA  = fabs( 1. / m_momA( 2 ) ); // correct
    const double           pB  = fabs( 1. / m_momB( 2 ) ); // correct
    const double           pAz = pA / std::sqrt( m_momA( 0 ) * m_momA( 0 ) + m_momA( 1 ) * m_momA( 1 ) + 1 );
    const double           pBz = pB / std::sqrt( m_momB( 0 ) * m_momB( 0 ) + m_momB( 1 ) * m_momB( 1 ) + 1 );
    const Gaudi::XYZVector p3A( m_momA( 0 ) * pAz, m_momA( 1 ) * pAz, pAz );
    const Gaudi::XYZVector p3B( m_momB( 0 ) * pBz, m_momB( 1 ) * pBz, pBz );
    const Gaudi::XYZVector p3V0 = p3A + p3B;
    const double           plA  = p3A.Dot( p3V0 );
    const double           plB  = p3B.Dot( p3V0 );
    const double           norm = 1.0 / ( p3V0.R() * p3V0.R() );

    alpha = ( plA - plB ) / ( plA + plB );
    qT    = .5 * ( std::sqrt( pA * pA - plA * plA * norm ) + std::sqrt( pB * pB - plB * plB * norm ) );
  }

  Gaudi::XYZVector TwoProngVertex::p3A() const {
    double tx = m_momA( 0 );
    double ty = m_momA( 1 );
    double p  = 1.0 / std::abs( m_momA( 2 ) );
    double pz = p / std::sqrt( 1.0 + tx * tx + ty * ty );
    return Gaudi::XYZVector( pz * tx, pz * ty, pz );
  }

  Gaudi::XYZVector TwoProngVertex::p3B() const {
    double tx = m_momB( 0 );
    double ty = m_momB( 1 );
    double p  = 1.0 / std::abs( m_momB( 2 ) );
    double pz = p / std::sqrt( 1 + tx * tx + ty * ty );
    return Gaudi::XYZVector( pz * tx, pz * ty, pz );
  }

  Gaudi::SymMatrix9x9 TwoProngVertex::covMatrix9x9() const {
    ///< Return the covariance matrix for position and track 1 & track 2 momenta
    Gaudi::SymMatrix9x9 covMatrix;
    covMatrix.Place_at( LHCb::VertexBase::covMatrix(), 0, 0 );
    covMatrix.Place_at( m_momcovA, 3, 3 );
    covMatrix.Place_at( m_momcovB, 6, 6 );

    // covMatrix.Place_at(m_mompposcovA,0,3);
    //  covMatrix.Place_at(m_momposcovB,0,6);
    //  covMatrix.Place_at(m_mommomcov,6,3);

    // now to copy off diagonal elements into full 9x9 matrix
    for ( int i = 0; i < 3; ++i ) {
      for ( int j = 0; j < 3; ++j ) { covMatrix( i, j + 3 ) = m_momposcovA( i, j ); }
    }

    for ( int i = 0; i < 3; ++i ) {
      for ( int j = 0; j < 3; ++j ) { covMatrix( i, j + 6 ) = m_momposcovB( i, j ); }
    }
    for ( int i = 0; i < 3; ++i ) {
      for ( int j = 0; j < 3; ++j ) { covMatrix( i + 3, j + 6 ) = m_mommomcov( i, j ); }
    }

    return covMatrix;
  }

  Gaudi::SymMatrix7x7 TwoProngVertex::covMatrix7x7( const double mass1, const double mass2 ) const {
    //< Return the covariance matrix for the V0

    // the Jacobians to change momenta of daughters from tx,ty,qop to PxPyPzE space
    Gaudi::Matrix4x3 JacobP1;
    Gaudi::Matrix4x3 JacobP2;
    Gaudi::Math::JacobdP4dMom( m_momA, mass1, JacobP1 );
    Gaudi::Math::JacobdP4dMom( m_momB, mass2, JacobP2 );

    // covmompos contains the transformed covariance matrix for the off diagonal part (position and momentum)
    Gaudi::Matrix4x3 covmompos;
    covmompos = JacobP1 * m_momposcovA;
    covmompos += JacobP2 * m_momposcovB;

    // covmommom contains the V0 symmetric covariance for the  momentum part
    Gaudi::Matrix4x4    covmommom12 = JacobP1 * m_mommomcov * ROOT::Math::Transpose( JacobP2 );
    Gaudi::SymMatrix4x4 covmommom;
    ROOT::Math::AssignSym::Evaluate( covmommom, covmommom12 + ROOT::Math::Transpose( covmommom12 ) );
    covmommom += ROOT::Math::Similarity( JacobP1, m_momcovA ); // J * C * JT
    covmommom += ROOT::Math::Similarity( JacobP2, m_momcovB );

    // finally, fill the returned matrix
    Gaudi::SymMatrix7x7 theMatrix;
    // position part from rec vertex
    theMatrix.Place_at( LHCb::VertexBase::covMatrix(), 0, 0 );
    // momentum part
    theMatrix.Place_at( covmommom, 3, 3 );
    // off diagonal elements
    for ( int i = 0; i < 4; ++i ) {
      for ( int j = 0; j < 3; ++j ) { theMatrix( i + 3, j ) = covmompos( i, j ); }
    }

    return theMatrix;
  }

  namespace {
    inline void JacobdP3dMom( const ROOT::Math::SVector<double, 3>& mom, ROOT::Math::SMatrix<double, 3, 3>& J ) {
      double tx  = mom( 0 );
      double ty  = mom( 1 );
      double qop = mom( 2 );
      double p   = 1 / std::abs( qop );
      double n2  = 1 + tx * tx + ty * ty;
      double n   = std::sqrt( n2 );
      double n3  = n2 * n;
      double pz  = p / n;
      double px  = pz * tx;
      double py  = pz * ty;
      J( 0, 0 )  = p * ( 1 + ty * ty ) / n3; // dpx/dtx
      J( 0, 1 )  = p * tx * -ty / n3;        // dpx/dty
      J( 0, 2 )  = -px / qop;                // dpx/dqop

      J( 1, 0 ) = p * ty * -tx / n3;        // dpy/dtx
      J( 1, 1 ) = p * ( 1 + tx * tx ) / n3; // dpy/dty
      J( 1, 2 ) = -py / qop;                // dpy/dqop

      J( 2, 0 ) = pz * -tx / n2; // dpz/dtx
      J( 2, 1 ) = pz * -ty / n2; // dpz/dtx
      J( 2, 2 ) = -pz / qop;     // dpz/dqop
    }
  } // namespace

  Gaudi::SymMatrix6x6 TwoProngVertex::covMatrix6x6() const {
    //< Return the covariance matrix for the V0

    // the Jacobians to change momenta of daughters from tx,ty,qop to PxPyPz space
    Gaudi::Matrix3x3 JacobP1;
    Gaudi::Matrix3x3 JacobP2;
    JacobdP3dMom( m_momA, JacobP1 );
    JacobdP3dMom( m_momB, JacobP2 );

    // covmompos contains the transformed covariance matrix for the off diagonal part (position and momentum)
    Gaudi::Matrix3x3 covmompos;
    covmompos = JacobP1 * m_momposcovA;
    covmompos += JacobP2 * m_momposcovB;

    // covmommom contains the V0 symmetric covariance for the  momentum part
    Gaudi::Matrix3x3    covmommom12 = JacobP1 * m_mommomcov * ROOT::Math::Transpose( JacobP2 );
    Gaudi::SymMatrix3x3 covmommom;
    ROOT::Math::AssignSym::Evaluate( covmommom, covmommom12 + ROOT::Math::Transpose( covmommom12 ) );
    covmommom += ROOT::Math::Similarity( JacobP1, m_momcovA ); // J * C * JT
    covmommom += ROOT::Math::Similarity( JacobP2, m_momcovB );

    // finally, fill the returned matrix
    Gaudi::SymMatrix6x6 theMatrix;
    // position part from rec vertex
    theMatrix.Place_at( LHCb::VertexBase::covMatrix(), 0, 0 );
    // momentum part
    theMatrix.Place_at( covmommom, 3, 3 );
    // off diagonal elements
    for ( int i = 0; i < 3; ++i ) {
      for ( int j = 0; j < 3; ++j ) { theMatrix( i + 3, j ) = covmompos( i, j ); }
    }

    return theMatrix;
  }

  Gaudi::Math::ValueWithError TwoProngVertex::massWithError( const double mass1, const double mass2 ) const {
    // we could implement this much more efficiently but I am lazy now
    auto                              p4cov = covMatrix7x7( mass1, mass2 ).Sub<Gaudi::SymMatrix4x4>( 3, 3 );
    ROOT::Math::SMatrix<double, 1, 4> dMdP4;
    Gaudi::LorentzVector              p4sum = momentum( mass1, mass2 );
    double                            m     = p4sum.M();
    dMdP4( 0, 0 )                           = -p4sum.Px() / m;
    dMdP4( 0, 1 )                           = -p4sum.Py() / m;
    dMdP4( 0, 2 )                           = -p4sum.Pz() / m;
    dMdP4( 0, 3 )                           = p4sum.E() / m;
    return Gaudi::Math::ValueWithError( m, std::sqrt( ROOT::Math::Similarity( dMdP4, p4cov )( 0, 0 ) ) );
  }

} // namespace LHCb
