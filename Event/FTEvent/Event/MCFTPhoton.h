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
#include "Event/MCHit.h"
#include "GaudiKernel/ContainedObject.h"
#include "GaudiKernel/ObjectVector.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCFTPhoton = 14005;

  // Namespace for locations in TDS
  namespace MCFTPhotonLocation {
    inline const std::string Default = "MC/FT/Photons";
  }

  /** @class MCFTPhoton MCFTPhoton.h
   *
   * This class represents a photon in a fibre mat as a result of crossing
   * MCHits.
   *
   * @author Violaine Bellee, Julian Wishahi
   *
   */

  class MCFTPhoton final : public ContainedObject {
  public:
    /// typedef for ObjectVector of MCFTPhoton
    typedef ObjectVector<MCFTPhoton> Container;

    /// Constructor
    MCFTPhoton( int sensDetID, double nPhotons, const LHCb::MCHit* mcHit, double time, double wavelength, double posX,
                double posZ, double dXdY, double dZdY, bool isReflected )
        : m_sensDetID( sensDetID )
        , m_nPhotons( nPhotons )
        , m_mcHit( mcHit )
        , m_time( time )
        , m_wavelength( wavelength )
        , m_posX( posX )
        , m_posZ( posZ )
        , m_dXdY( dXdY )
        , m_dZdY( dZdY )
        , m_isReflected( isReflected ) {}

    /// Default Constructor
    MCFTPhoton()
        : m_sensDetID( 0 )
        , m_nPhotons( 0.0 )
        , m_mcHit( 0 )
        , m_time( 0.0 )
        , m_wavelength( 0.0 )
        , m_posX( 0.0 )
        , m_posZ( 0.0 )
        , m_dXdY( 0.0 )
        , m_dZdY( 0.0 )
        , m_isReflected() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Comparison for sorting by matID and x-position
    static bool lowerByMatID( MCFTPhoton* p1, MCFTPhoton* p2 );

    /// Retrieve const  ID of the mat
    int sensDetID() const;

    /// Update  ID of the mat
    void setSensDetID( int value );

    /// Retrieve const  Number of photons
    double nPhotons() const;

    /// Update  Number of photons
    void setNPhotons( double value );

    /// Retrieve const  Pointer to orignal MCHit
    const LHCb::MCHit* mcHit() const;

    /// Update  Pointer to orignal MCHit
    void setMcHit( const LHCb::MCHit* value );

    /// Retrieve const  Exit time of the photons at the fibre ends
    double time() const;

    /// Update  Exit time of the photons at the fibre ends
    void setTime( double value );

    /// Retrieve const  Wavelength of the photons at the fibre ends
    double wavelength() const;

    /// Update  Wavelength of the photons at the fibre ends
    void setWavelength( double value );

    /// Retrieve const  x coordinate (local) of the photons at the fibre ends
    double posX() const;

    /// Update  x coordinate (local) of the photons at the fibre ends
    void setPosX( double value );

    /// Retrieve const  z coordinate (local) of the photons at the fibre ends
    double posZ() const;

    /// Update  z coordinate (local) of the photons at the fibre ends
    void setPosZ( double value );

    /// Retrieve const  dx/dy direction (local) of the photons at the fibre ends
    double dXdY() const;

    /// Update  dx/dy direction (local) of the photons at the fibre ends
    void setDXdY( double value );

    /// Retrieve const  dz/dy direction (local) of the photons at the fibre ends
    double dZdY() const;

    /// Update  dz/dy direction (local) of the photons at the fibre ends
    void setDZdY( double value );

    /// Retrieve const  isReflected flags
    bool isReflected() const;

    /// Update  isReflected flags
    void setIsReflected( bool value );

  protected:
  private:
    int                m_sensDetID;   ///< ID of the mat
    double             m_nPhotons;    ///< Number of photons
    const LHCb::MCHit* m_mcHit;       ///< Pointer to orignal MCHit
    double             m_time;        ///< Exit time of the photons at the fibre ends
    double             m_wavelength;  ///< Wavelength of the photons at the fibre ends
    double             m_posX;        ///< x coordinate (local) of the photons at the fibre ends
    double             m_posZ;        ///< z coordinate (local) of the photons at the fibre ends
    double             m_dXdY;        ///< dx/dy direction (local) of the photons at the fibre ends
    double             m_dZdY;        ///< dz/dy direction (local) of the photons at the fibre ends
    bool               m_isReflected; ///< isReflected flags

  }; // class MCFTPhoton

  /// Definition of vector container type for MCFTPhoton
  typedef ObjectVector<MCFTPhoton> MCFTPhotons;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCFTPhoton::clID() const { return LHCb::MCFTPhoton::classID(); }

inline const CLID& LHCb::MCFTPhoton::classID() { return CLID_MCFTPhoton; }

inline int LHCb::MCFTPhoton::sensDetID() const { return m_sensDetID; }

inline void LHCb::MCFTPhoton::setSensDetID( int value ) { m_sensDetID = value; }

inline double LHCb::MCFTPhoton::nPhotons() const { return m_nPhotons; }

inline void LHCb::MCFTPhoton::setNPhotons( double value ) { m_nPhotons = value; }

inline const LHCb::MCHit* LHCb::MCFTPhoton::mcHit() const { return m_mcHit; }

inline void LHCb::MCFTPhoton::setMcHit( const LHCb::MCHit* value ) { m_mcHit = value; }

inline double LHCb::MCFTPhoton::time() const { return m_time; }

inline void LHCb::MCFTPhoton::setTime( double value ) { m_time = value; }

inline double LHCb::MCFTPhoton::wavelength() const { return m_wavelength; }

inline void LHCb::MCFTPhoton::setWavelength( double value ) { m_wavelength = value; }

inline double LHCb::MCFTPhoton::posX() const { return m_posX; }

inline void LHCb::MCFTPhoton::setPosX( double value ) { m_posX = value; }

inline double LHCb::MCFTPhoton::posZ() const { return m_posZ; }

inline void LHCb::MCFTPhoton::setPosZ( double value ) { m_posZ = value; }

inline double LHCb::MCFTPhoton::dXdY() const { return m_dXdY; }

inline void LHCb::MCFTPhoton::setDXdY( double value ) { m_dXdY = value; }

inline double LHCb::MCFTPhoton::dZdY() const { return m_dZdY; }

inline void LHCb::MCFTPhoton::setDZdY( double value ) { m_dZdY = value; }

inline bool LHCb::MCFTPhoton::isReflected() const { return m_isReflected; }

inline void LHCb::MCFTPhoton::setIsReflected( bool value ) { m_isReflected = value; }

inline bool LHCb::MCFTPhoton::lowerByMatID( MCFTPhoton* p1, MCFTPhoton* p2 ) {
  return ( p1->sensDetID() < p2->sensDetID() );
}
