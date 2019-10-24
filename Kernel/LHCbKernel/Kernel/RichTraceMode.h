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
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class RichTraceMode RichTraceMode.h
   *
   * Helper class used to configure the ray tracing options in the
   * IRichRayTracing tools
   *
   * @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *
   */

  class RichTraceMode {
  public:
    /// The result of the ray tracing
    enum RayTraceResult {
      RayTraceFailed = 0, // The ray tracing failed
      OutsidePDPanel = 1, // The ray tracing succeeded and but was outside the acceptance of the PD panels
      InPDPanel      = 2, // The ray tracing succeeded and fell into the acceptance of an PD panel
      InPDTube       = 3  // The ray tracing succeeded and fell into the acceptance of an PD tube
    };
    /// Flag the precision mode to use when calculating the PD acceptance on the PD panels. Only applies if
    /// DetectorPlaneBoundary enum is set to RespectPDTubes
    enum DetectorPrecision {
      FlatPDs      = 0, // Approximate the PD window acceptance by circles on a plane (fastest)
      SphericalPDs = 1, // Models the PD entrance window by a simple sphere + circular acceptance (fast)
      FullPDs      = 2  // Use the full detector description for the PDs (slower)
    };
    /// Flag how to treat the PD panel and PD tube acceptances when performing the ray tracing
    enum DetectorPlaneBoundary {
      IgnorePDAcceptance = 1, // Ignore the PDs acceptance entirely (both the average PD panel and individual tubes)
      RespectPDPanel     = 2, // Respect the average physical boundaries of the PDs on the panel plane
      RespectPDTubes     = 3  // Respect the individual PD acceptances
    };

    /// Default Constructor
    RichTraceMode();

    /// Constructor with configuration values
    explicit RichTraceMode(
        const LHCb::RichTraceMode::DetectorPlaneBoundary bound,
        const LHCb::RichTraceMode::DetectorPrecision     detPrec = LHCb::RichTraceMode::DetectorPrecision::FlatPDs,
        const bool forcedSide = false, const bool respectOuter = false, const bool respectMirrSegs = false,
        const bool checkBeamPipeIntersects = false, const bool checkPDKaptonShadowing = false,
        const bool aeroRefraction = false );

    /// Default Destructor
    ~RichTraceMode() = default;

    /// Tests if the given ray tracing result was successful, within the bounds presented by the configured mode
    [[nodiscard]] bool traceWasOK( const LHCb::RichTraceMode::RayTraceResult& result ) const;

    /// Print this RichTraceMode in a human readable way
    std::ostream& fillStream( std::ostream& os ) const;

    /// Retrieve const  Bit packed field containing the ray tracing configuration options
    [[nodiscard]] unsigned int data() const;

    /// Update  Bit packed field containing the ray tracing configuration options
    void setData( unsigned int value );

    /// Retrieve The PD modelling precision to use
    [[nodiscard]] LHCb::RichTraceMode::DetectorPrecision detPrecision() const;

    /// Update The PD modelling precision to use
    void setDetPrecision( const LHCb::RichTraceMode::DetectorPrecision& value );

    /// Retrieve The PD tube and panel acceptance mode
    [[nodiscard]] LHCb::RichTraceMode::DetectorPlaneBoundary detPlaneBound() const;

    /// Update The PD tube and panel acceptance mode
    void setDetPlaneBound( const LHCb::RichTraceMode::DetectorPlaneBoundary& value );

    /// Retrieve Flag to force the side to use in the ray-tracing
    [[nodiscard]] bool forcedSide() const;

    /// Update Flag to force the side to use in the ray-tracing
    void setForcedSide( bool value );

    /// Retrieve Flag to respect the overall outer boundaries of the mirrors
    [[nodiscard]] bool outMirrorBoundary() const;

    /// Update Flag to respect the overall outer boundaries of the mirrors
    void setOutMirrorBoundary( bool value );

    /// Retrieve Flag to respect the individual mirror segment boundaries (i.e. the small gaps between them)
    [[nodiscard]] bool mirrorSegBoundary() const;

    /// Update Flag to respect the individual mirror segment boundaries (i.e. the small gaps between them)
    void setMirrorSegBoundary( bool value );

    /// Retrieve Flag to turn on rejection of trajectories which intersect the beam pipe inside the RICH detectors
    [[nodiscard]] bool beamPipeIntersects() const;

    /// Update Flag to turn on rejection of trajectories which intersect the beam pipe inside the RICH detectors
    void setBeamPipeIntersects( bool value );

    /// Retrieve Flag to turn on the checking for shadowing by the PD kapton
    [[nodiscard]] bool hpdKaptonShadowing() const;

    /// Update Flag to turn on the checking for shadowing by the PD kapton
    void setHpdKaptonShadowing( bool value );

    /// Retrieve Flag to turn on the refraction correction at the exit of the Aerogel volume into the Rich1Gas
    [[nodiscard]] bool aeroRefraction() const;

    /// Update Flag to turn on the refraction correction at the exit of the Aerogel volume into the Rich1Gas
    void setAeroRefraction( bool value );

    friend std::ostream& operator<<( std::ostream& str, const RichTraceMode& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield data
    enum dataBits {
      detPrecisionBits       = 0,
      detPlaneBoundBits      = 4,
      forcedSideBits         = 8,
      outMirrorBoundaryBits  = 9,
      mirrorSegBoundaryBits  = 10,
      beamPipeIntersectsBits = 11,
      hpdKaptonShadowingBits = 12,
      aeroRefractionBits     = 13
    };

    /// Bitmasks for bitfield data
    enum dataMasks {
      detPrecisionMask       = 0xfL,
      detPlaneBoundMask      = 0xf0L,
      forcedSideMask         = 0x100L,
      outMirrorBoundaryMask  = 0x200L,
      mirrorSegBoundaryMask  = 0x400L,
      beamPipeIntersectsMask = 0x800L,
      hpdKaptonShadowingMask = 0x1000L,
      aeroRefractionMask     = 0x2000L
    };

    unsigned int m_data{0}; ///< Bit packed field containing the ray tracing configuration options

  }; // class RichTraceMode

  inline std::ostream& operator<<( std::ostream& s, LHCb::RichTraceMode::RayTraceResult e ) {
    switch ( e ) {
    case LHCb::RichTraceMode::RayTraceFailed:
      return s << "RayTraceFailed";
    case LHCb::RichTraceMode::OutsidePDPanel:
      return s << "OutsidePDPanel";
    case LHCb::RichTraceMode::InPDPanel:
      return s << "InPDPanel";
    case LHCb::RichTraceMode::InPDTube:
      return s << "InPDTube";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RichTraceMode::RayTraceResult";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::RichTraceMode::DetectorPrecision e ) {
    switch ( e ) {
    case LHCb::RichTraceMode::FlatPDs:
      return s << "FlatPDs";
    case LHCb::RichTraceMode::SphericalPDs:
      return s << "SphericalPDs";
    case LHCb::RichTraceMode::FullPDs:
      return s << "FullPDs";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RichTraceMode::DetectorPrecision";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::RichTraceMode::DetectorPlaneBoundary e ) {
    switch ( e ) {
    case LHCb::RichTraceMode::IgnorePDAcceptance:
      return s << "IgnorePDAcceptance";
    case LHCb::RichTraceMode::RespectPDPanel:
      return s << "RespectPDPanel";
    case LHCb::RichTraceMode::RespectPDTubes:
      return s << "RespectPDTubes";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RichTraceMode::DetectorPlaneBoundary";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::RichTraceMode::RichTraceMode() {

  setDetPlaneBound( LHCb::RichTraceMode::DetectorPlaneBoundary::IgnorePDAcceptance );
  setDetPrecision( LHCb::RichTraceMode::DetectorPrecision::FlatPDs );
}

inline LHCb::RichTraceMode::RichTraceMode( const LHCb::RichTraceMode::DetectorPlaneBoundary bound,
                                           const LHCb::RichTraceMode::DetectorPrecision detPrec, const bool forcedSide,
                                           const bool respectOuter, const bool respectMirrSegs,
                                           const bool checkBeamPipeIntersects, const bool checkPDKaptonShadowing,
                                           const bool aeroRefraction )
    : m_data( 0 ) {

  setDetPlaneBound( bound );
  setDetPrecision( detPrec );
  setForcedSide( forcedSide );
  setOutMirrorBoundary( respectOuter );
  setMirrorSegBoundary( respectMirrSegs );
  setBeamPipeIntersects( checkBeamPipeIntersects );
  setHpdKaptonShadowing( checkPDKaptonShadowing );
  setAeroRefraction( aeroRefraction );
}

inline unsigned int LHCb::RichTraceMode::data() const { return m_data; }

inline void LHCb::RichTraceMode::setData( unsigned int value ) { m_data = value; }

inline LHCb::RichTraceMode::DetectorPrecision LHCb::RichTraceMode::detPrecision() const {
  return ( LHCb::RichTraceMode::DetectorPrecision )( ( m_data & detPrecisionMask ) >> detPrecisionBits );
}

inline void LHCb::RichTraceMode::setDetPrecision( const LHCb::RichTraceMode::DetectorPrecision& value ) {
  auto val = (unsigned int)value;
  m_data &= ~detPrecisionMask;
  m_data |= ( ( ( (unsigned int)val ) << detPrecisionBits ) & detPrecisionMask );
}

inline LHCb::RichTraceMode::DetectorPlaneBoundary LHCb::RichTraceMode::detPlaneBound() const {
  return ( LHCb::RichTraceMode::DetectorPlaneBoundary )( ( m_data & detPlaneBoundMask ) >> detPlaneBoundBits );
}

inline void LHCb::RichTraceMode::setDetPlaneBound( const LHCb::RichTraceMode::DetectorPlaneBoundary& value ) {
  auto val = (unsigned int)value;
  m_data &= ~detPlaneBoundMask;
  m_data |= ( ( ( (unsigned int)val ) << detPlaneBoundBits ) & detPlaneBoundMask );
}

inline bool LHCb::RichTraceMode::forcedSide() const { return 0 != ( ( m_data & forcedSideMask ) >> forcedSideBits ); }

inline void LHCb::RichTraceMode::setForcedSide( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~forcedSideMask;
  m_data |= ( ( ( (unsigned int)val ) << forcedSideBits ) & forcedSideMask );
}

inline bool LHCb::RichTraceMode::outMirrorBoundary() const {
  return 0 != ( ( m_data & outMirrorBoundaryMask ) >> outMirrorBoundaryBits );
}

inline void LHCb::RichTraceMode::setOutMirrorBoundary( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~outMirrorBoundaryMask;
  m_data |= ( ( ( (unsigned int)val ) << outMirrorBoundaryBits ) & outMirrorBoundaryMask );
}

inline bool LHCb::RichTraceMode::mirrorSegBoundary() const {
  return 0 != ( ( m_data & mirrorSegBoundaryMask ) >> mirrorSegBoundaryBits );
}

inline void LHCb::RichTraceMode::setMirrorSegBoundary( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~mirrorSegBoundaryMask;
  m_data |= ( ( ( (unsigned int)val ) << mirrorSegBoundaryBits ) & mirrorSegBoundaryMask );
}

inline bool LHCb::RichTraceMode::beamPipeIntersects() const {
  return 0 != ( ( m_data & beamPipeIntersectsMask ) >> beamPipeIntersectsBits );
}

inline void LHCb::RichTraceMode::setBeamPipeIntersects( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~beamPipeIntersectsMask;
  m_data |= ( ( ( (unsigned int)val ) << beamPipeIntersectsBits ) & beamPipeIntersectsMask );
}

inline bool LHCb::RichTraceMode::hpdKaptonShadowing() const {
  return 0 != ( ( m_data & hpdKaptonShadowingMask ) >> hpdKaptonShadowingBits );
}

inline void LHCb::RichTraceMode::setHpdKaptonShadowing( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~hpdKaptonShadowingMask;
  m_data |= ( ( ( (unsigned int)val ) << hpdKaptonShadowingBits ) & hpdKaptonShadowingMask );
}

inline bool LHCb::RichTraceMode::aeroRefraction() const {
  return 0 != ( ( m_data & aeroRefractionMask ) >> aeroRefractionBits );
}

inline void LHCb::RichTraceMode::setAeroRefraction( bool value ) {
  auto val = (unsigned int)value;
  m_data &= ~aeroRefractionMask;
  m_data |= ( ( ( (unsigned int)val ) << aeroRefractionBits ) & aeroRefractionMask );
}

inline bool LHCb::RichTraceMode::traceWasOK( const LHCb::RichTraceMode::RayTraceResult& result ) const {

  return ( (int)result >= (int)detPlaneBound() );
}
