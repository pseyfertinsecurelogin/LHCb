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
#include "GaudiKernel/GenericVectorTypes.h"
#include "GaudiKernel/SymmetricMatrixTypes.h"
#include "GaudiKernel/SystemOfUnits.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class CaloPosition CaloPosition.h
   *
   * @brief Position of calorimeter cluster * * * The class represents the
   * position of calorimeter cluster, * center of gravity, spread and their
   * covariance matrices * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class CaloPosition final {
  public:
    /// 3-vector of parameters (E,X,Y)
    typedef Gaudi::Vector3 Parameters;
    /// 3x3 Covariance matrix (E,X,Y)
    typedef Gaudi::SymMatrix3x3 Covariance;
    /// 2-vector of parameters (X,Y)
    typedef Gaudi::Vector2 Center;
    /// 2x2 Covariance matrix (X,Y)
    typedef Gaudi::SymMatrix2x2 Spread;

    /// indices to access X,Y and E
    enum Index { X = 0, Y = 1, E = 2 };

    /// Default Constructor
    CaloPosition()
        : m_z( -1 * Gaudi::Units::km ), m_parameters( 0., 0., 0. ), m_covariance(), m_center( 0., 0. ), m_spread() {}

    /// retrieve the x-position @attention alias method!
    double x() const;

    /// retrieve the y-position @attention alias method!
    double y() const;

    /// retrieve the energy @attention alias method!
    double e() const;

    /// Print this CaloPosition data object in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    /// Retrieve const  Z-position where cluster parameters are estimated
    double z() const;

    /// Update  Z-position where cluster parameters are estimated
    void setZ( double value );

    /// Retrieve const  vector of major cluster parameters
    const Parameters& parameters() const;

    /// Retrieve  vector of major cluster parameters
    Parameters& parameters();

    /// Update  vector of major cluster parameters
    void setParameters( const Parameters& value );

    /// Retrieve const  covariance matrix of major cluster parameters (3x3)
    const Covariance& covariance() const;

    /// Retrieve  covariance matrix of major cluster parameters (3x3)
    Covariance& covariance();

    /// Update  covariance matrix of major cluster parameters (3x3)
    void setCovariance( const Covariance& value );

    /// Retrieve const  cluster center of gravity position (2D)
    const Center& center() const;

    /// Retrieve  cluster center of gravity position (2D)
    Center& center();

    /// Update  cluster center of gravity position (2D)
    void setCenter( const Center& value );

    /// Retrieve const  cluster spread matrix (2x2)
    const Spread& spread() const;

    /// Retrieve  cluster spread matrix (2x2)
    Spread& spread();

    /// Update  cluster spread matrix (2x2)
    void setSpread( const Spread& value );

    friend std::ostream& operator<<( std::ostream& str, const CaloPosition& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double     m_z;          ///< Z-position where cluster parameters are estimated
    Parameters m_parameters; ///< vector of major cluster parameters
    Covariance m_covariance; ///< covariance matrix of major cluster parameters (3x3)
    Center     m_center;     ///< cluster center of gravity position (2D)
    Spread     m_spread;     ///< cluster spread matrix (2x2)

  }; // class CaloPosition

  inline std::ostream& operator<<( std::ostream& s, LHCb::CaloPosition::Index e ) {
    switch ( e ) {
    case LHCb::CaloPosition::X:
      return s << "X";
    case LHCb::CaloPosition::Y:
      return s << "Y";
    case LHCb::CaloPosition::E:
      return s << "E";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::CaloPosition::Index";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline double LHCb::CaloPosition::z() const { return m_z; }

inline void LHCb::CaloPosition::setZ( double value ) { m_z = value; }

inline const LHCb::CaloPosition::Parameters& LHCb::CaloPosition::parameters() const { return m_parameters; }

inline LHCb::CaloPosition::Parameters& LHCb::CaloPosition::parameters() { return m_parameters; }

inline void LHCb::CaloPosition::setParameters( const Parameters& value ) { m_parameters = value; }

inline const LHCb::CaloPosition::Covariance& LHCb::CaloPosition::covariance() const { return m_covariance; }

inline LHCb::CaloPosition::Covariance& LHCb::CaloPosition::covariance() { return m_covariance; }

inline void LHCb::CaloPosition::setCovariance( const Covariance& value ) { m_covariance = value; }

inline const LHCb::CaloPosition::Center& LHCb::CaloPosition::center() const { return m_center; }

inline LHCb::CaloPosition::Center& LHCb::CaloPosition::center() { return m_center; }

inline void LHCb::CaloPosition::setCenter( const Center& value ) { m_center = value; }

inline const LHCb::CaloPosition::Spread& LHCb::CaloPosition::spread() const { return m_spread; }

inline LHCb::CaloPosition::Spread& LHCb::CaloPosition::spread() { return m_spread; }

inline void LHCb::CaloPosition::setSpread( const Spread& value ) { m_spread = value; }

inline double LHCb::CaloPosition::x() const { return parameters()( CaloPosition::Index::X ); }

inline double LHCb::CaloPosition::y() const { return parameters()( CaloPosition::Index::Y ); }

inline double LHCb::CaloPosition::e() const { return parameters()( CaloPosition::Index::E ); }
