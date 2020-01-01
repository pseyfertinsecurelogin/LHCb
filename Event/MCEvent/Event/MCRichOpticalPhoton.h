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
#include "Event/MCRichHit.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/Vector3DTypes.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichRadiatorType.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MCRichOpticalPhoton = 12412;

  // Namespace for locations in TDS
  namespace MCRichOpticalPhotonLocation {
    inline const std::string Default = "MC/Rich/OpticalPhotons";
  }

  /** @class MCRichOpticalPhoton MCRichOpticalPhoton.h
   *
   * Auxially MC class containing specialist Rich data complimentary to the
   * MCRichHit class
   *
   * @author Chris Jones   (Christopher.Rob.Jones@cern.ch)
   *
   */

  class MCRichOpticalPhoton : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MCRichOpticalPhoton
    typedef KeyedContainer<MCRichOpticalPhoton, Containers::HashMap> Container;

    /// Default Constructor
    MCRichOpticalPhoton() = default;

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Point of incidence of the photon on the HPD window
    const Gaudi::XYZPoint& pdIncidencePoint() const;

    /// Update  Point of incidence of the photon on the HPD window
    void setPdIncidencePoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  Spherical mirror reflection point
    const Gaudi::XYZPoint& sphericalMirrorReflectPoint() const;

    /// Update  Spherical mirror reflection point
    void setSphericalMirrorReflectPoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  Flat mirror reflection point
    const Gaudi::XYZPoint& flatMirrorReflectPoint() const;

    /// Update  Flat mirror reflection point
    void setFlatMirrorReflectPoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  If applicable, exit point of the photon from the aerogel radiator
    const Gaudi::XYZPoint& aerogelExitPoint() const;

    /// Update  If applicable, exit point of the photon from the aerogel radiator
    void setAerogelExitPoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  Cherenkov angle theta at production
    float cherenkovTheta() const;

    /// Update  Cherenkov angle theta at production
    void setCherenkovTheta( float value );

    /// Retrieve const  Cherenkov angle phi at production
    float cherenkovPhi() const;

    /// Update  Cherenkov angle phi at production
    void setCherenkovPhi( float value );

    /// Retrieve const  Photon emission point
    const Gaudi::XYZPoint& emissionPoint() const;

    /// Update  Photon emission point
    void setEmissionPoint( const Gaudi::XYZPoint& value );

    /// Retrieve const  Photon energy at production
    float energyAtProduction() const;

    /// Update  Photon energy at production
    void setEnergyAtProduction( float value );

    /// Retrieve const  The momentum of the photon parent at production
    const Gaudi::XYZVector& parentMomentum() const;

    /// Update  The momentum of the photon parent at production
    void setParentMomentum( const Gaudi::XYZVector& value );

    /// Retrieve const  Photon incidence point on the external surface of the HPD Quartz Window
    const Gaudi::XYZPoint& hpdQWIncidencePoint() const;

    /// Update  Photon incidence point on the external surface of the HPD Quartz Window
    void setHpdQWIncidencePoint( const Gaudi::XYZPoint& value );

    /// Retrieve (const)  Associated MCRichHit
    const LHCb::MCRichHit* mcRichHit() const;

    /// Update  Associated MCRichHit
    void setMcRichHit( const SmartRef<LHCb::MCRichHit>& value );

    /// Update (pointer)  Associated MCRichHit
    void setMcRichHit( const LHCb::MCRichHit* value );

    friend std::ostream& operator<<( std::ostream& str, const MCRichOpticalPhoton& obj ) {
      return obj.fillStream( str );
    }

  private:
    Gaudi::XYZPoint  m_pdIncidencePoint{};            ///< Point of incidence of the photon on the HPD window
    Gaudi::XYZPoint  m_sphericalMirrorReflectPoint{}; ///< Spherical mirror reflection point
    Gaudi::XYZPoint  m_flatMirrorReflectPoint{};      ///< Flat mirror reflection point
    Gaudi::XYZPoint  m_aerogelExitPoint{};    ///< If applicable, exit point of the photon from the aerogel radiator
    float            m_cherenkovTheta{0};     ///< Cherenkov angle theta at production
    float            m_cherenkovPhi{0};       ///< Cherenkov angle phi at production
    Gaudi::XYZPoint  m_emissionPoint{};       ///< Photon emission point
    float            m_energyAtProduction{0}; ///< Photon energy at production
    Gaudi::XYZVector m_parentMomentum{};      ///< The momentum of the photon parent at production
    Gaudi::XYZPoint  m_hpdQWIncidencePoint{}; ///< Photon incidence point on the external surface of the HPD Quartz
                                              ///< Window
    SmartRef<LHCb::MCRichHit> m_mcRichHit;    ///< Associated MCRichHit

  }; // class MCRichOpticalPhoton

  /// Definition of Keyed Container for MCRichOpticalPhoton
  typedef KeyedContainer<MCRichOpticalPhoton, Containers::HashMap> MCRichOpticalPhotons;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MCRichOpticalPhoton::clID() const { return LHCb::MCRichOpticalPhoton::classID(); }

inline const CLID& LHCb::MCRichOpticalPhoton::classID() { return CLID_MCRichOpticalPhoton; }

inline std::ostream& LHCb::MCRichOpticalPhoton::fillStream( std::ostream& s ) const {
  s << "{ "
    << "pdIncidencePoint :	" << m_pdIncidencePoint << std::endl
    << "sphericalMirrorReflectPoint :	" << m_sphericalMirrorReflectPoint << std::endl
    << "flatMirrorReflectPoint :	" << m_flatMirrorReflectPoint << std::endl
    << "aerogelExitPoint :	" << m_aerogelExitPoint << std::endl
    << "cherenkovTheta :	" << m_cherenkovTheta << std::endl
    << "cherenkovPhi :	" << m_cherenkovPhi << std::endl
    << "emissionPoint :	" << m_emissionPoint << std::endl
    << "energyAtProduction :	" << m_energyAtProduction << std::endl
    << "parentMomentum :	" << m_parentMomentum << std::endl
    << "hpdQWIncidencePoint :	" << m_hpdQWIncidencePoint << std::endl
    << " }";
  return s;
}

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::pdIncidencePoint() const { return m_pdIncidencePoint; }

inline void LHCb::MCRichOpticalPhoton::setPdIncidencePoint( const Gaudi::XYZPoint& value ) {
  m_pdIncidencePoint = value;
}

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::sphericalMirrorReflectPoint() const {
  return m_sphericalMirrorReflectPoint;
}

inline void LHCb::MCRichOpticalPhoton::setSphericalMirrorReflectPoint( const Gaudi::XYZPoint& value ) {
  m_sphericalMirrorReflectPoint = value;
}

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::flatMirrorReflectPoint() const {
  return m_flatMirrorReflectPoint;
}

inline void LHCb::MCRichOpticalPhoton::setFlatMirrorReflectPoint( const Gaudi::XYZPoint& value ) {
  m_flatMirrorReflectPoint = value;
}

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::aerogelExitPoint() const { return m_aerogelExitPoint; }

inline void LHCb::MCRichOpticalPhoton::setAerogelExitPoint( const Gaudi::XYZPoint& value ) {
  m_aerogelExitPoint = value;
}

inline float LHCb::MCRichOpticalPhoton::cherenkovTheta() const { return m_cherenkovTheta; }

inline void LHCb::MCRichOpticalPhoton::setCherenkovTheta( float value ) { m_cherenkovTheta = value; }

inline float LHCb::MCRichOpticalPhoton::cherenkovPhi() const { return m_cherenkovPhi; }

inline void LHCb::MCRichOpticalPhoton::setCherenkovPhi( float value ) { m_cherenkovPhi = value; }

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::emissionPoint() const { return m_emissionPoint; }

inline void LHCb::MCRichOpticalPhoton::setEmissionPoint( const Gaudi::XYZPoint& value ) { m_emissionPoint = value; }

inline float LHCb::MCRichOpticalPhoton::energyAtProduction() const { return m_energyAtProduction; }

inline void LHCb::MCRichOpticalPhoton::setEnergyAtProduction( float value ) { m_energyAtProduction = value; }

inline const Gaudi::XYZVector& LHCb::MCRichOpticalPhoton::parentMomentum() const { return m_parentMomentum; }

inline void LHCb::MCRichOpticalPhoton::setParentMomentum( const Gaudi::XYZVector& value ) { m_parentMomentum = value; }

inline const Gaudi::XYZPoint& LHCb::MCRichOpticalPhoton::hpdQWIncidencePoint() const { return m_hpdQWIncidencePoint; }

inline void LHCb::MCRichOpticalPhoton::setHpdQWIncidencePoint( const Gaudi::XYZPoint& value ) {
  m_hpdQWIncidencePoint = value;
}

inline const LHCb::MCRichHit* LHCb::MCRichOpticalPhoton::mcRichHit() const { return m_mcRichHit; }

inline void LHCb::MCRichOpticalPhoton::setMcRichHit( const SmartRef<LHCb::MCRichHit>& value ) { m_mcRichHit = value; }

inline void LHCb::MCRichOpticalPhoton::setMcRichHit( const LHCb::MCRichHit* value ) { m_mcRichHit = value; }
