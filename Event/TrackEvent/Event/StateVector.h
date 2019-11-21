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
#include "Event/TrackTypes.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class StateVector StateVector.h
   *
   * StateVector is a simple track state vector with z position
   *
   * @author Jose Hernando, Eduardo Rodrigues, Wouter Hulsbergen
   *
   */

  class StateVector final {
  public:
    /// Constructor with assigned key
    StateVector( const Gaudi::TrackVector& stateVec, double z ) : m_parameters( stateVec ), m_z( z ) {}

    /// Constructor from a position and direction
    StateVector( const Gaudi::XYZPoint& position, const Gaudi::XYZVector& direction, double qOverP = 0 );

    /// Default Constructor
    StateVector() = default;

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve the number of state parameters
    [[nodiscard]] unsigned int nParameters() const;

    /// Retrieve the position and momentum vectors of the state
    void positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const;

    /// Retrieve the 3D-position vector (x,y,z) of the state
    [[nodiscard]] Gaudi::XYZPoint position() const;

    /// Retrieve the x-position of the state
    [[nodiscard]] double x() const;

    /// Retrieve the y-position of the state
    [[nodiscard]] double y() const;

    /// Retrieve the slopes (Tx=dx/dz,Ty=dy/dz,1.) of the state
    [[nodiscard]] Gaudi::XYZVector slopes() const;

    /// Retrieve the Tx=dx/dz slope of the state
    [[nodiscard]] double tx() const;

    /// Retrieve the Ty=dy/dz slope of the state
    [[nodiscard]] double ty() const;

    /// Retrieve the charge-over-momentum Q/P of the state
    [[nodiscard]] double qOverP() const;

    /// Retrieve the momentum of the state
    [[nodiscard]] double p() const;

    /// Retrieve the transverse momentum of the state
    [[nodiscard]] double pt() const;

    /// Retrieve the momentum vector (px,py,pz) of the state
    [[nodiscard]] Gaudi::XYZVector momentum() const;

    /// Retrieve the Q/Pperp (ratio of the charge to the component of the momentum transverse to the magnetic field) of
    /// the state
    [[nodiscard]] double qOverPperp() const;

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

    /// printOut method to Gaudi message stream
    MsgStream& fillStream( MsgStream& os ) const;

    /// Retrieve const  the parameter vector
    [[nodiscard]] const Gaudi::TrackVector& parameters() const;

    /// Retrieve  the parameter vector
    Gaudi::TrackVector& parameters();

    /// Retrieve const  the z-position of the state
    [[nodiscard]] double z() const;

    friend std::ostream& operator<<( std::ostream& str, const StateVector& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    Gaudi::TrackVector m_parameters; ///< the parameter vector
    double             m_z{0.0};     ///< the z-position of the state

  }; // class StateVector

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::StateVector::fillStream( std::ostream& s ) const {
  s << "{ "
    << "parameters :	" << m_parameters << std::endl
    << "z :	" << (float)m_z << std::endl
    << " }";
  return s;
}

inline const Gaudi::TrackVector& LHCb::StateVector::parameters() const { return m_parameters; }

inline Gaudi::TrackVector& LHCb::StateVector::parameters() { return m_parameters; }

inline double LHCb::StateVector::z() const { return m_z; }

inline unsigned int LHCb::StateVector::nParameters() const { return (unsigned int)m_parameters.Dim(); }

inline void LHCb::StateVector::positionAndMomentum( Gaudi::XYZPoint& pos, Gaudi::XYZVector& mom ) const {

  pos = position();
  mom = momentum();
}

inline Gaudi::XYZPoint LHCb::StateVector::position() const {

  return Gaudi::XYZPoint( m_parameters[0], m_parameters[1], m_z );
}

inline double LHCb::StateVector::x() const { return m_parameters[0]; }

inline double LHCb::StateVector::y() const { return m_parameters[1]; }

inline Gaudi::XYZVector LHCb::StateVector::slopes() const {

  return Gaudi::XYZVector( m_parameters[2], m_parameters[3], 1. );
}

inline double LHCb::StateVector::tx() const { return m_parameters[2]; }

inline double LHCb::StateVector::ty() const { return m_parameters[3]; }

inline double LHCb::StateVector::qOverP() const { return m_parameters[4]; }

inline Gaudi::XYZVector LHCb::StateVector::momentum() const {

  Gaudi::XYZVector mom = slopes();
  mom *= ( p() / mom.R() );
  return mom;
}

inline double LHCb::StateVector::qOverPperp() const {

  const double tx2 = tx() * tx();
  return ( qOverP() * sqrt( ( 1. + tx2 + ty() * ty() ) / ( 1. + tx2 ) ) );
}

inline void LHCb::StateVector::setX( double value ) { m_parameters[0] = value; }

inline void LHCb::StateVector::setY( double value ) { m_parameters[1] = value; }

inline void LHCb::StateVector::setZ( double value ) { m_z = value; }

inline void LHCb::StateVector::setTx( double value ) { m_parameters[2] = value; }

inline void LHCb::StateVector::setTy( double value ) { m_parameters[3] = value; }

inline void LHCb::StateVector::setQOverP( double value ) { m_parameters[4] = value; }

inline MsgStream& LHCb::StateVector::fillStream( MsgStream& os ) const {

  os << "{ "
     << "parameters :	" << m_parameters << std::endl
     << "z :	        " << (float)m_z << std::endl
     << " }";
  return os;
}
