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
#include "Event/StateVector.h"
#include "Event/TrackTypes.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_State = 10011;

  // Namespace for locations in TDS
  namespace StateLocation {
    inline const std::string Default = "Rec/Track/States";
  }

  /** @class State State.h
   *
   * State is the base class of offline and online track states.
   *
   * @author Jose Hernando, Eduardo Rodrigues
   *
   */

  class State final {
  public:
    /// typedef for std::vector of State
    typedef std::vector<State*>       Vector;
    typedef std::vector<const State*> ConstVector;

    /// typedef for KeyedContainer of State
    typedef KeyedContainer<State, Containers::HashMap> Container;

    /// State location enumerations
    enum Location {
      LocationUnknown,
      ClosestToBeam,
      FirstMeasurement,
      EndVelo,
      AtTT,
      AtT,
      BegRich1,
      EndRich1,
      BegRich2,
      EndRich2,
      Spd,
      Prs,
      BegECal,
      ECalShowerMax,
      MidECal,
      EndECal,
      BegHCal,
      MidHCal,
      EndHCal,
      Muon,
      LastMeasurement,
      Vertex,
      V0Vertex
    };

    /// Default constructor
    State();

    /// Constructor with arguments
    State( const Gaudi::TrackVector& stateVec, const Gaudi::TrackSymMatrix& cov, double z,
           const LHCb::State::Location& location );

    /// Constructor from a StateVector
    State( const LHCb::StateVector& stateVec )
        : m_flags( 0 ), m_stateVector( stateVec.parameters() ), m_covariance(), m_z( stateVec.z() ) {}

    /// Constructor without matrix
    State( const Gaudi::TrackVector& stateVec, double z, const LHCb::State::Location& location );

    /// Constructor with location
    State( const LHCb::State::Location& location );

    // Retrieve pointer to class definition structure
    const CLID&        clID() const;
    static const CLID& classID();

    /// conversion of string to enum for type Location
    static LHCb::State::Location LocationToType( const std::string& aName );

    /// conversion to string for enum type Location
    static const std::string& LocationToString( int aEnum );

    /// Retrieve the number of state parameters
    unsigned int nParameters() const;

    /// Retrieve the position and momentum vectors and the corresponding 6D covariance matrix (pos:0->2,mom:3-5) of the
    /// state
    void positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom, Gaudi::SymMatrix6x6& cov6D ) const;

    /// Retrieve the position and momentum vectors of the state
    void positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const;

    /// Retrieve the 3D-position vector (x,y,z) of the state
    Gaudi::XYZPoint position() const;

    /// Retrieve the x-position of the state
    double x() const;

    /// Retrieve the y-position of the state
    double y() const;

    /// Retrieve the z-position of the state
    double z() const;

    /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) of the state
    Gaudi::XYZVector slopes() const;

    /// Retrieve the Tx=dx/dz slope of the state
    double tx() const;

    /// Retrieve the Ty=dy/dz slope of the state
    double ty() const;

    /// Retrieve the charge-over-momentum Q/P of the state
    double qOverP() const;

    /// Retrieve the momentum of the state
    double p() const;

    /// Retrieve the transverse momentum of the state
    double pt() const;

    /// Retrieve the momentum vector (px,py,pz) of the state
    Gaudi::XYZVector momentum() const;

    /// Retrieve the Q/Pperp (ratio of the charge to the component of the momentum transverse to the magnetic field) of
    /// the state
    double qOverPperp() const;

    /// Retrieve the 6D (x,y,z,px,py,pz) covariance matrix of the state
    Gaudi::SymMatrix6x6 posMomCovariance() const;

    /// Retrieve the errors on the 3D-position vector (x,y,z) of the state
    Gaudi::SymMatrix3x3 errPosition() const;

    /// Retrieve the squared error on the x-position of the state
    double errX2() const;

    /// Retrieve the squared error on the y-position of the state
    double errY2() const;

    /// Retrieve the errors on the slopes (Tx=dx/dz,Ty=dy/dz,1.) of the state
    Gaudi::SymMatrix3x3 errSlopes() const;

    /// Retrieve the squared error on the x-slope Tx=dx/dz of the state
    double errTx2() const;

    /// Retrieve the squared error on the y-slope Ty=dy/dz of the state
    double errTy2() const;

    /// Retrieve the squared error on the charge-over-momentum Q/P of the state
    double errQOverP2() const;

    /// Retrieve the squared error on the momentum (px,py,pz) of the state
    double errP2() const;

    /// Retrieve the errors on the momentum vector (px,py,pz) of the state
    Gaudi::SymMatrix3x3 errMomentum() const;

    /// Retrieve the squared error on the Q/Pperp of the state
    double errQOverPperp2() const;

    /// Update the state vector
    void setState( const Gaudi::TrackVector& state );

    /// Update the state vector
    void setState( const LHCb::StateVector& state );

    /// Update the state vector
    void setState( double x, double y, double z, double tx, double ty, double qOverP );

    /// Update the state covariance matrix
    void setCovariance( const Gaudi::TrackSymMatrix& value );

    /// Update the x-position of the state
    void setX( double value );

    /// Update the y-position of the state
    void setY( double value );

    /// Update the z-position of the state
    void setZ( double value );

    /// Update the x-slope Tx=dx/dz slope of the state
    void setTx( double value );

    /// Update the y-slope Ty=dy/dz slope of the state
    void setTy( double value );

    /// Update the charge-over-momentum Q/P value of the state
    void setQOverP( double value );

    /// Update the squared error on the charge-over-momentum Q/P of the state
    void setErrQOverP2( double value );

    /// Check if the state is at a predefined location
    bool checkLocation( const LHCb::State::Location& value ) const;

    /// transport this state to a new z-position
    void linearTransportTo( double z );

    /// printOut method to Gaudi message stream
    std::ostream& fillStream( std::ostream& os ) const;

    /// Retrieve const  the variety of State flags
    unsigned int flags() const;

    /// Update  the variety of State flags
    void setFlags( unsigned int value );

    /// Retrieve state location
    LHCb::State::Location location() const;

    /// Update state location
    void setLocation( const LHCb::State::Location& value );

    /// Retrieve const  the state vector
    const Gaudi::TrackVector& stateVector() const;

    /// Retrieve  the state vector
    Gaudi::TrackVector& stateVector();

    /// Retrieve const  the state covariance matrix (indexes 0,...,4 for x, y, tx, ty, Q/p)
    const Gaudi::TrackSymMatrix& covariance() const;

    /// Retrieve  the state covariance matrix (indexes 0,...,4 for x, y, tx, ty, Q/p)
    Gaudi::TrackSymMatrix& covariance();

    friend std::ostream& operator<<( std::ostream& str, const State& obj ) { return obj.fillStream( str ); }

  protected:
    /// Offsets of bitfield flags
    enum flagsBits { locationBits = 0 };

    /// Bitmasks for bitfield flags
    enum flagsMasks { locationMask = 0xffffL };

    unsigned int          m_flags;       ///< the variety of State flags
    Gaudi::TrackVector    m_stateVector; ///< the state vector
    Gaudi::TrackSymMatrix m_covariance;  ///< the state covariance matrix (indexes 0,...,4 for x, y, tx, ty, Q/p)
    double                m_z;           ///< the z-position of the state

  private:
    static const GaudiUtils::VectorMap<std::string, Location>& s_LocationTypMap();

  }; // class State

  /// Definition of Keyed Container for State
  typedef KeyedContainer<State, Containers::HashMap> States;

  inline std::ostream& operator<<( std::ostream& s, LHCb::State::Location e ) {
    switch ( e ) {
    case LHCb::State::LocationUnknown:
      return s << "LocationUnknown";
    case LHCb::State::ClosestToBeam:
      return s << "ClosestToBeam";
    case LHCb::State::FirstMeasurement:
      return s << "FirstMeasurement";
    case LHCb::State::EndVelo:
      return s << "EndVelo";
    case LHCb::State::AtTT:
      return s << "AtTT";
    case LHCb::State::AtT:
      return s << "AtT";
    case LHCb::State::BegRich1:
      return s << "BegRich1";
    case LHCb::State::EndRich1:
      return s << "EndRich1";
    case LHCb::State::BegRich2:
      return s << "BegRich2";
    case LHCb::State::EndRich2:
      return s << "EndRich2";
    case LHCb::State::Spd:
      return s << "Spd";
    case LHCb::State::Prs:
      return s << "Prs";
    case LHCb::State::BegECal:
      return s << "BegECal";
    case LHCb::State::ECalShowerMax:
      return s << "ECalShowerMax";
    case LHCb::State::MidECal:
      return s << "MidECal";
    case LHCb::State::EndECal:
      return s << "EndECal";
    case LHCb::State::BegHCal:
      return s << "BegHCal";
    case LHCb::State::MidHCal:
      return s << "MidHCal";
    case LHCb::State::EndHCal:
      return s << "EndHCal";
    case LHCb::State::Muon:
      return s << "Muon";
    case LHCb::State::LastMeasurement:
      return s << "LastMeasurement";
    case LHCb::State::Vertex:
      return s << "Vertex";
    case LHCb::State::V0Vertex:
      return s << "V0Vertex";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::State::Location";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::State::State() : m_flags( 0 ), m_stateVector(), m_covariance(), m_z( 0.0 ) {

  setLocation( State::Location::LocationUnknown );
}

inline LHCb::State::State( const Gaudi::TrackVector& stateVec, const Gaudi::TrackSymMatrix& cov, double z,
                           const LHCb::State::Location& location )
    : m_flags( 0 ), m_stateVector( stateVec ), m_covariance( cov ), m_z( z ) {

  setLocation( location );
}

inline LHCb::State::State( const Gaudi::TrackVector& stateVec, double z, const LHCb::State::Location& location )
    : m_flags( 0 ), m_stateVector( stateVec ), m_covariance(), m_z( z ) {

  setLocation( location );
}

inline LHCb::State::State( const LHCb::State::Location& location )
    : m_flags( 0 ), m_stateVector(), m_covariance(), m_z( 0 ) {

  setLocation( location );
}

inline const CLID& LHCb::State::clID() const { return LHCb::State::classID(); }

inline const CLID& LHCb::State::classID() { return CLID_State; }

inline const GaudiUtils::VectorMap<std::string, LHCb::State::Location>& LHCb::State::s_LocationTypMap() {
  static const GaudiUtils::VectorMap<std::string, Location> m = {{"LocationUnknown", LocationUnknown},
                                                                 {"ClosestToBeam", ClosestToBeam},
                                                                 {"FirstMeasurement", FirstMeasurement},
                                                                 {"EndVelo", EndVelo},
                                                                 {"AtTT", AtTT},
                                                                 {"AtT", AtT},
                                                                 {"BegRich1", BegRich1},
                                                                 {"EndRich1", EndRich1},
                                                                 {"BegRich2", BegRich2},
                                                                 {"EndRich2", EndRich2},
                                                                 {"Spd", Spd},
                                                                 {"Prs", Prs},
                                                                 {"BegECal", BegECal},
                                                                 {"ECalShowerMax", ECalShowerMax},
                                                                 {"MidECal", MidECal},
                                                                 {"EndECal", EndECal},
                                                                 {"BegHCal", BegHCal},
                                                                 {"MidHCal", MidHCal},
                                                                 {"EndHCal", EndHCal},
                                                                 {"Muon", Muon},
                                                                 {"LastMeasurement", LastMeasurement},
                                                                 {"Vertex", Vertex},
                                                                 {"V0Vertex", V0Vertex}};
  return m;
}

inline LHCb::State::Location LHCb::State::LocationToType( const std::string& aName ) {
  auto iter = s_LocationTypMap().find( aName );
  return iter != s_LocationTypMap().end() ? iter->second : LocationUnknown;
}

inline const std::string& LHCb::State::LocationToString( int aEnum ) {
  static const std::string s_LocationUnknown = "LocationUnknown";
  auto                     iter              = std::find_if( s_LocationTypMap().begin(), s_LocationTypMap().end(),
                            [&]( const std::pair<const std::string, Location>& i ) { return i.second == aEnum; } );
  return iter != s_LocationTypMap().end() ? iter->first : s_LocationUnknown;
}

inline unsigned int LHCb::State::flags() const { return m_flags; }

inline void LHCb::State::setFlags( unsigned int value ) { m_flags = value; }

inline LHCb::State::Location LHCb::State::location() const {
  return ( LHCb::State::Location )( ( m_flags & locationMask ) >> locationBits );
}

inline void LHCb::State::setLocation( const LHCb::State::Location& value ) {
  unsigned int val = (unsigned int)value;
  m_flags &= ~locationMask;
  m_flags |= ( ( ( (unsigned int)val ) << locationBits ) & locationMask );
}

inline const Gaudi::TrackVector& LHCb::State::stateVector() const { return m_stateVector; }

inline Gaudi::TrackVector& LHCb::State::stateVector() { return m_stateVector; }

inline const Gaudi::TrackSymMatrix& LHCb::State::covariance() const { return m_covariance; }

inline Gaudi::TrackSymMatrix& LHCb::State::covariance() { return m_covariance; }

inline unsigned int LHCb::State::nParameters() const { return (unsigned int)m_stateVector.Dim(); }

inline void LHCb::State::positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom,
                                              Gaudi::SymMatrix6x6& cov6D ) const {

  pos   = position();
  mom   = momentum();
  cov6D = posMomCovariance();
}

inline void LHCb::State::positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const {

  pos = position();
  mom = momentum();
}

inline Gaudi::XYZPoint LHCb::State::position() const {

  return Gaudi::XYZPoint( m_stateVector[0], m_stateVector[1], m_z );
}

inline double LHCb::State::x() const { return m_stateVector[0]; }

inline double LHCb::State::y() const { return m_stateVector[1]; }

inline double LHCb::State::z() const { return m_z; }

inline Gaudi::XYZVector LHCb::State::slopes() const {

  return Gaudi::XYZVector( m_stateVector[2], m_stateVector[3], 1. );
}

inline double LHCb::State::tx() const { return m_stateVector[2]; }

inline double LHCb::State::ty() const { return m_stateVector[3]; }

inline Gaudi::XYZVector LHCb::State::momentum() const {

  Gaudi::XYZVector mom = slopes();
  mom *= ( p() / mom.R() );
  return mom;
}

inline double LHCb::State::qOverPperp() const {

  const double tx2 = tx() * tx();
  return ( qOverP() * sqrt( ( 1. + tx2 + ty() * ty() ) / ( 1. + tx2 ) ) );
}

inline double LHCb::State::errX2() const { return m_covariance( 0, 0 ); }

inline double LHCb::State::errY2() const { return m_covariance( 1, 1 ); }

inline double LHCb::State::errTx2() const { return m_covariance( 2, 2 ); }

inline double LHCb::State::errTy2() const { return m_covariance( 3, 3 ); }

inline void LHCb::State::setState( const Gaudi::TrackVector& state ) { m_stateVector = state; }

inline void LHCb::State::setState( const LHCb::StateVector& state ) {

  m_stateVector = state.parameters();
  m_z           = state.z();
}

inline void LHCb::State::setCovariance( const Gaudi::TrackSymMatrix& value ) { m_covariance = value; }

inline void LHCb::State::setX( double value ) { m_stateVector[0] = value; }

inline void LHCb::State::setY( double value ) { m_stateVector[1] = value; }

inline void LHCb::State::setZ( double value ) { m_z = value; }

inline void LHCb::State::setTx( double value ) { m_stateVector[2] = value; }

inline void LHCb::State::setTy( double value ) { m_stateVector[3] = value; }

inline bool LHCb::State::checkLocation( const LHCb::State::Location& value ) const { return location() == value; }
