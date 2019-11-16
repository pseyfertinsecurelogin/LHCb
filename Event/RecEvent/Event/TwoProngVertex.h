/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/RecVertex.h"
#include "Event/Track.h"
#include "Event/TrackTypes.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/NamedRange.h"
#include "GaudiKernel/SerializeSTL.h"
#include "GaudiKernel/SharedObjectsContainer.h"
#include "GaudiKernel/Vector4DTypes.h"
#include "Kernel/ParticleID.h"
#include "LHCbMath/MatrixTransforms.h"
#include "LHCbMath/ValueWithError.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_TwoProngVertex = 10040;

  // Namespace for locations in TDS
  namespace TwoProngVertexLocation {
    inline const std::string Default = LHCb::RecVertexLocation::V0;
  }

  /** @class TwoProngVertex TwoProngVertex.h
   *
   * TwoProngVertex for use in V0 finding and tracking monitoring
   *
   * @author Wouter Hulsbergen, Sean Brisbane
   *
   */

  class TwoProngVertex : public LHCb::RecVertex {
  public:
    /// typedef for std::vector of TwoProngVertex
    using Vector      = std::vector<TwoProngVertex*>;
    using ConstVector = std::vector<const TwoProngVertex*>;

    /// typedef for KeyedContainer of TwoProngVertex
    typedef KeyedContainer<TwoProngVertex, Containers::HashMap> Container;

    /// For defining SharedObjectContainer
    using Selection = SharedObjectsContainer<LHCb::TwoProngVertex>;
    /// For accessing a list of L0CaloCandidates which is either a SharedObjectContainer, a KeyedContainer or a
    /// ConstVector
    using Range = Gaudi::NamedRange_<ConstVector>;

    /// Additional information assigned to this V0. Contains Impact parameter information
    enum additionalInfo {
      IPx   = 1,  // TheImpact parameter with respect to the primary vertex in x
      IPy   = 2,  // TheImpact parameter with respect to the primary vertex in y
      POCAx = 11, // The Closest apporach of the contained tracks in x direction
      POCAy = 12, // The Closest apporach of the contained tracks in y direction
      LastGlobal =
          10000 // The last official flag value. Private user variables should use flags greater than this value
    };

    /// Sets minimal data content for useable vertex. The rest we do with setters.
    TwoProngVertex( const Gaudi::XYZPoint& position ) : LHCb::RecVertex( position ) {}

    /// Default Constructor
    TwoProngVertex() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Clone the TwoProngVertex
    TwoProngVertex* clone() const override;

    /// add a PID to the vector
    void addPID( LHCb::ParticleID& value );

    /// fitted p4 of first track
    Gaudi::LorentzVector p4A( double mass ) const;

    /// fitted p4 of second track
    Gaudi::LorentzVector p4B( double mass ) const;

    /// fitted mass of two-prong given mass hypothesis for 2 daughters
    double mass( double massA, double massB ) const;

    /// error on fitted mass of two-prong given mass hypothesis for 2 daughters
    Gaudi::Math::ValueWithError massWithError( double mass1, double mass2 ) const;

    /// fitted momentum of two-prong given mass hypothesis for 2 daughters
    Gaudi::LorentzVector momentum( double massA, double massB ) const;

    /// fitted p3 of first track
    Gaudi::XYZVector p3A() const;

    /// fitted p3 of second track
    Gaudi::XYZVector p3B() const;

    /// fitted momentum of two-prong
    Gaudi::XYZVector p3() const;

    /// retrieve the  7x7 Covariance Martix, x, y, z, px, py, pz, E for the V0
    Gaudi::SymMatrix7x7 covMatrix7x7( double mass1, double mass2 ) const;

    /// retrieve the 6x6 covariance matrix, x, y, z, px, py, pz for the two-prong
    Gaudi::SymMatrix6x6 covMatrix6x6() const;

    /// retrieve the  9x9 full Covariance Martix, x, y, z, txA, tyA, qopA,txB, tyB, qopB for the V0 and tracks
    Gaudi::SymMatrix9x9 covMatrix9x9() const;

    /// Retrieve the first track easily
    const LHCb::Track* trackA() const;

    /// Retrieve the second track easily
    const LHCb::Track* trackB() const;

    /// error handling
    static void throwError( char error[] );

    /// return the track type of the track at trackIndex
    Track::Types TrackType( unsigned int trackIndex ) const;

    /// return the Armenteros_Podolanski variables - Pt(V0) and PPerp assymetry of the daughters
    void GetArmenteros_Podolanski( double& qT, double& alpha ) const;

    /// Retrieve const  fitted momentum (tx,ty,qop) of first track
    const ROOT::Math::SVector<double, 3>& momA() const;

    /// Retrieve  fitted momentum (tx,ty,qop) of first track
    ROOT::Math::SVector<double, 3>& momA();

    /// Update  fitted momentum (tx,ty,qop) of first track
    void setMomA( const ROOT::Math::SVector<double, 3>& value );

    /// Retrieve const  covariance matrix of momentum of first track
    const Gaudi::SymMatrix3x3& momcovA() const;

    /// Retrieve  covariance matrix of momentum of first track
    Gaudi::SymMatrix3x3& momcovA();

    /// Retrieve const  off-diagonal part of momentum-position covariance matrix of first track
    const ROOT::Math::SMatrix<double, 3, 3>& momposcovA() const;

    /// Retrieve  off-diagonal part of momentum-position covariance matrix of first track
    ROOT::Math::SMatrix<double, 3, 3>& momposcovA();

    /// Retrieve const  fitted momentum (tx,ty,qop) of second track
    const ROOT::Math::SVector<double, 3>& momB() const;

    /// Retrieve  fitted momentum (tx,ty,qop) of second track
    ROOT::Math::SVector<double, 3>& momB();

    /// Update  fitted momentum (tx,ty,qop) of second track
    void setMomB( const ROOT::Math::SVector<double, 3>& value );

    /// Retrieve const  covariance matrix of momentum of second track
    const Gaudi::SymMatrix3x3& momcovB() const;

    /// Retrieve  covariance matrix of momentum of second track
    Gaudi::SymMatrix3x3& momcovB();

    /// Retrieve const  off-diagonal part of momentum-position covariance matrix of second track
    const ROOT::Math::SMatrix<double, 3, 3>& momposcovB() const;

    /// Retrieve  off-diagonal part of momentum-position covariance matrix of second track
    ROOT::Math::SMatrix<double, 3, 3>& momposcovB();

    /// Retrieve const  off-diagonal part of the momentum-momentum covariance matrix
    const ROOT::Math::SMatrix<double, 3, 3>& mommomcov() const;

    /// Retrieve  off-diagonal part of the momentum-momentum covariance matrix
    ROOT::Math::SMatrix<double, 3, 3>& mommomcov();

    /// Retrieve const  Vector of compatible PIDs for the mother
    const std::vector<LHCb::ParticleID>& compatiblePIDs() const;

    /// Update  Vector of compatible PIDs for the mother
    void setCompatiblePIDs( const std::vector<LHCb::ParticleID>& value );

    friend std::ostream& operator<<( std::ostream& str, const TwoProngVertex& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    ROOT::Math::SVector<double, 3>    m_momA;       ///< fitted momentum (tx,ty,qop) of first track
    Gaudi::SymMatrix3x3               m_momcovA;    ///< covariance matrix of momentum of first track
    ROOT::Math::SMatrix<double, 3, 3> m_momposcovA; ///< off-diagonal part of momentum-position covariance matrix of
                                                    ///< first track
    ROOT::Math::SVector<double, 3>    m_momB;       ///< fitted momentum (tx,ty,qop) of second track
    Gaudi::SymMatrix3x3               m_momcovB;    ///< covariance matrix of momentum of second track
    ROOT::Math::SMatrix<double, 3, 3> m_momposcovB; ///< off-diagonal part of momentum-position covariance matrix of
                                                    ///< second track
    ROOT::Math::SMatrix<double, 3, 3> m_mommomcov;  ///< off-diagonal part of the momentum-momentum covariance matrix
    std::vector<LHCb::ParticleID>     m_compatiblePIDs; ///< Vector of compatible PIDs for the mother

  }; // class TwoProngVertex

  /// Definition of Keyed Container for TwoProngVertex
  typedef KeyedContainer<TwoProngVertex, Containers::HashMap> TwoProngVertices;

  inline std::ostream& operator<<( std::ostream& s, LHCb::TwoProngVertex::additionalInfo e ) {
    switch ( e ) {
    case LHCb::TwoProngVertex::IPx:
      return s << "IPx";
    case LHCb::TwoProngVertex::IPy:
      return s << "IPy";
    case LHCb::TwoProngVertex::POCAx:
      return s << "POCAx";
    case LHCb::TwoProngVertex::POCAy:
      return s << "POCAy";
    case LHCb::TwoProngVertex::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::TwoProngVertex::additionalInfo";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::TwoProngVertex::clID() const { return LHCb::TwoProngVertex::classID(); }

inline const CLID& LHCb::TwoProngVertex::classID() { return CLID_TwoProngVertex; }

inline std::ostream& LHCb::TwoProngVertex::fillStream( std::ostream& s ) const {
  LHCb::RecVertex::fillStream( s );
  s << "{ "
    << "momA :	" << m_momA << std::endl
    << "momcovA :	" << m_momcovA << std::endl
    << "momposcovA :	" << m_momposcovA << std::endl
    << "momB :	" << m_momB << std::endl
    << "momcovB :	" << m_momcovB << std::endl
    << "momposcovB :	" << m_momposcovB << std::endl
    << "mommomcov :	" << m_mommomcov << std::endl
    << "compatiblePIDs :	" << m_compatiblePIDs << std::endl
    << " }";
  return s;
}

inline const ROOT::Math::SVector<double, 3>& LHCb::TwoProngVertex::momA() const { return m_momA; }

inline ROOT::Math::SVector<double, 3>& LHCb::TwoProngVertex::momA() { return m_momA; }

inline void LHCb::TwoProngVertex::setMomA( const ROOT::Math::SVector<double, 3>& value ) { m_momA = value; }

inline const Gaudi::SymMatrix3x3& LHCb::TwoProngVertex::momcovA() const { return m_momcovA; }

inline Gaudi::SymMatrix3x3& LHCb::TwoProngVertex::momcovA() { return m_momcovA; }

inline const ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::momposcovA() const { return m_momposcovA; }

inline ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::momposcovA() { return m_momposcovA; }

inline const ROOT::Math::SVector<double, 3>& LHCb::TwoProngVertex::momB() const { return m_momB; }

inline ROOT::Math::SVector<double, 3>& LHCb::TwoProngVertex::momB() { return m_momB; }

inline void LHCb::TwoProngVertex::setMomB( const ROOT::Math::SVector<double, 3>& value ) { m_momB = value; }

inline const Gaudi::SymMatrix3x3& LHCb::TwoProngVertex::momcovB() const { return m_momcovB; }

inline Gaudi::SymMatrix3x3& LHCb::TwoProngVertex::momcovB() { return m_momcovB; }

inline const ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::momposcovB() const { return m_momposcovB; }

inline ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::momposcovB() { return m_momposcovB; }

inline const ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::mommomcov() const { return m_mommomcov; }

inline ROOT::Math::SMatrix<double, 3, 3>& LHCb::TwoProngVertex::mommomcov() { return m_mommomcov; }

inline const std::vector<LHCb::ParticleID>& LHCb::TwoProngVertex::compatiblePIDs() const { return m_compatiblePIDs; }

inline void LHCb::TwoProngVertex::setCompatiblePIDs( const std::vector<LHCb::ParticleID>& value ) {
  m_compatiblePIDs = value;
}

inline LHCb::TwoProngVertex* LHCb::TwoProngVertex::clone() const { return new LHCb::TwoProngVertex( *this ); }

inline void LHCb::TwoProngVertex::addPID( LHCb::ParticleID& value ) { m_compatiblePIDs.push_back( value ); }

inline Gaudi::LorentzVector LHCb::TwoProngVertex::p4A( double mass ) const {

  Gaudi::LorentzVector p4a;
  return Gaudi::Math::geo2LA( m_momA, mass, p4a );
}

inline Gaudi::LorentzVector LHCb::TwoProngVertex::p4B( double mass ) const {

  Gaudi::LorentzVector p4b;
  return Gaudi::Math::geo2LA( m_momB, mass, p4b );
}

inline double LHCb::TwoProngVertex::mass( double massA, double massB ) const {

  return ( p4A( massA ) + p4B( massB ) ).M();
}

inline Gaudi::LorentzVector LHCb::TwoProngVertex::momentum( double massA, double massB ) const {

  return ( p4A( massA ) + p4B( massB ) );
}

inline Gaudi::XYZVector LHCb::TwoProngVertex::p3() const { return p3A() + p3B(); }

inline const LHCb::Track* LHCb::TwoProngVertex::trackA() const { return ( tracks().at( 0 ) ).target(); }

inline const LHCb::Track* LHCb::TwoProngVertex::trackB() const { return ( tracks().at( 1 ) ).target(); }

inline void LHCb::TwoProngVertex::throwError( char error[] ) {

  std::cout << error << std::endl; // this is bad, how should I report error
}

inline LHCb::Track::Types LHCb::TwoProngVertex::TrackType( const unsigned int trackIndex ) const {

  return trackIndex < tracks().size() ? tracks().at( trackIndex )->type() : LHCb::Track::Types::TypeUnknown;
}
