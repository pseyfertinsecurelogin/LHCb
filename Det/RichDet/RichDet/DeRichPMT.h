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

#pragma once

// STL
#include <array>
#include <cmath>
#include <cstdint>

// DetDesc
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/IPVolume.h"

// LHCbKernel
#include "Kernel/ILHCbMagnetSvc.h"
#include "Kernel/RichSmartID.h"

// RichDet
#include "RichDet/DeRichPD.h"

// External declarations
extern const CLID CLID_DERichPMT;

//=============================================================================
/** @class DeRichPMT DeRichPMT.h RichDet/DeRichPMT.h
 *
 *  PMT Photodetector
 *
 *  @author Sajan Easo
 *  @date   2011-10-10
 */
//=============================================================================

class DeRichPMT : public DeRichPD {

public:
  /// Standard constructor
  explicit DeRichPMT( const std::string& name = "" ) : DeRichPD( name ) {}

  /// Destructor
  virtual ~DeRichPMT() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override final { return classID(); }

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID();

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should terminate
   */
  StatusCode initialize() override final;

  // @brief Converts a RichSmartID to a point in global coordinates.
  bool detectionPoint( const LHCb::RichSmartID smartID, Gaudi::XYZPoint& detectPoint,
                       bool photoCathodeSide = false ) const override final;

  // (SIMD) Converts RichSmartIDs to an SIMD point in global coordinates.
  DeRichPD::SIMDFP::mask_type detectionPoint( const SmartIDs& smartID, SIMDPoint& detectPoint,
                                              bool photoCathodeSide = false ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID& smartID ) const;

  void setPmtIsGrandFlag( const bool isGrand );

  inline bool PmtIsGrand() const noexcept { return m_PmtIsGrand; }

  /// Set the PD ID
  inline void setPDSmartID( const LHCb::RichSmartID id ) noexcept { m_pdSmartID = id; }

  /// Access the PD ID
  inline LHCb::RichSmartID pdSmartID() const noexcept { return m_pdSmartID; }

  /// Shortcut to the 'to local' matrix
  inline const Gaudi::Transform3D& toLocalMatrix() const noexcept { return m_toLocM; }

  /// Shortcut to the 'to global' matrix
  inline const Gaudi::Transform3D& toGlobalMatrix() const noexcept { return geometry()->toGlobalMatrix(); }

  /// Set the zeroInPanelLocal position
  inline void setZeroInPanelLocal( const Gaudi::XYZPoint& p ) noexcept { m_zeroInPanelLocal = p; }

  /// Access the zeroInPanelLocal position
  inline const Gaudi::XYZPoint& zeroInPanelLocal() const noexcept { return m_zeroInPanelLocal; }

private:
  /// Floating point to use internal. Use float for reduced memory footprint.
  using FType = float;
  /// int type
  using IPix = LHCb::RichSmartID::DataType;

private:
  DetectorElement* getFirstRich();

  StatusCode getPMTParameters();
  StatusCode initPMTQuantumEff();
  StatusCode updateGeometry();

private:
  inline Gaudi::XYZPoint getAnodeHitCoordFromPixelNum( const IPix fracPixelCol, const IPix fracPixelRow ) const
      noexcept {
    const auto xh = ( (FType)fracPixelCol - m_PmtNumPixColFrac ) * m_PmtEffectivePixelXSize;
    const auto yh = ( (FType)fracPixelRow - m_PmtNumPixRowFrac ) * m_PmtEffectivePixelYSize;
    return {xh, yh, m_PmtAnodeHalfThickness};
  }

  inline SIMDPoint getAnodeHitCoordFromPixelNum( const SIMDUINT& fracPixelCol, const SIMDUINT& fracPixelRow ) const
      noexcept {
    using namespace LHCb::SIMD;
    const auto xh =
        ( simd_cast<SIMDFP>( fracPixelCol ) - SIMDFP( m_PmtNumPixColFrac ) ) * SIMDFP( m_PmtEffectivePixelXSize );
    const auto yh =
        ( simd_cast<SIMDFP>( fracPixelRow ) - SIMDFP( m_PmtNumPixRowFrac ) ) * SIMDFP( m_PmtEffectivePixelYSize );
    return {xh, yh, SIMDFP( m_PmtAnodeHalfThickness )};
  }

  inline Gaudi::XYZPoint getAnodeHitCoordFromGrandPixelNum( const IPix fracPixelCol, const IPix fracPixelRow ) const
      noexcept {
    const auto aXEffPixel =
        ( fracPixelCol == 0 || fracPixelCol == ( m_PmtNumPixCol - 1 ) ? m_GrandPmtEdgePixelXSize
                                                                      : m_GrandPmtEffectivePixelXSize );
    const auto aYEffPixel =
        ( fracPixelRow == 0 || fracPixelRow == ( m_PmtNumPixRow - 1 ) ? m_GrandPmtEdgePixelYSize
                                                                      : m_GrandPmtEffectivePixelYSize );
    const auto xh = ( (FType)fracPixelCol - m_PmtNumPixColFrac ) * aXEffPixel;
    const auto yh = ( (FType)fracPixelRow - m_PmtNumPixRowFrac ) * aYEffPixel;
    return {xh, yh, m_GrandPmtAnodeHalfThickness};
  }

  inline SIMDPoint getAnodeHitCoordFromGrandPixelNum( const SIMDUINT& fracPixelCol, const SIMDUINT& fracPixelRow ) const
      noexcept {
    using namespace LHCb::SIMD;
    const auto xmask      = ( fracPixelCol == SIMDUINT::Zero() ) || ( fracPixelCol == SIMDUINT( m_PmtNumPixCol - 1 ) );
    const auto ymask      = ( fracPixelRow == SIMDUINT::Zero() ) || ( fracPixelRow == SIMDUINT( m_PmtNumPixRow - 1 ) );
    auto       aXEffPixel = SIMDFP( m_GrandPmtEffectivePixelXSize );
    aXEffPixel( simd_cast<SIMDFP::mask_type>( xmask ) ) = SIMDFP( m_GrandPmtEdgePixelXSize );
    auto aYEffPixel                                     = SIMDFP( m_GrandPmtEffectivePixelYSize );
    aYEffPixel( simd_cast<SIMDFP::mask_type>( ymask ) ) = SIMDFP( m_GrandPmtEdgePixelYSize );

    const auto xh = ( simd_cast<SIMDFP>( fracPixelCol ) - SIMDFP( m_PmtNumPixColFrac ) ) * aXEffPixel;
    const auto yh = ( simd_cast<SIMDFP>( fracPixelRow ) - SIMDFP( m_PmtNumPixRowFrac ) ) * aYEffPixel;
    return {xh, yh, SIMDFP( m_GrandPmtAnodeHalfThickness )};
  }

  inline Gaudi::XYZPoint getAnodeHitCoordFromMultTypePixelNum( const IPix fracPixelCol, const IPix fracPixelRow ) const
      noexcept {
    return ( PmtIsGrand() ? getAnodeHitCoordFromGrandPixelNum( fracPixelCol, fracPixelRow )
                          : getAnodeHitCoordFromPixelNum( fracPixelCol, fracPixelRow ) );
  }

  inline SIMDPoint getAnodeHitCoordFromMultTypePixelNum( const SIMDUINT& fracPixelCol,
                                                         const SIMDUINT& fracPixelRow ) const noexcept {
    return ( PmtIsGrand() ? getAnodeHitCoordFromGrandPixelNum( fracPixelCol, fracPixelRow )
                          : getAnodeHitCoordFromPixelNum( fracPixelCol, fracPixelRow ) );
  }

private:
  // CRJ - To minimise memory footprint do not define data members
  //       that are not needed (outside of getPMTParameters() setup)
  //       Also, data members should be arranged so most commonly
  //       accessed are listed first.

  /// PD SmartID
  LHCb::RichSmartID m_pdSmartID;

  /// Cache 'to local' matrix
  Gaudi::Transform3D m_toLocM;

  /// SIMD To global transform
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toGlobalMatrixSIMD;

  /* Cache the point where {0,0,0} in this PD local frame appears
   * in the local frame of its parent PD panel */
  Gaudi::XYZPoint m_zeroInPanelLocal;

  FType m_zShift{0};

  FType m_PmtQwZSize{0};

  FType        m_PmtEffectivePixelXSize{0};
  FType        m_PmtEffectivePixelYSize{0};
  FType        m_PmtAnodeHalfThickness{0};
  unsigned int m_PmtNumPixCol{0};
  unsigned int m_PmtNumPixRow{0};
  FType        m_PmtNumPixColFrac{0};
  FType        m_PmtNumPixRowFrac{0};

  FType m_GrandPmtEdgePixelXSize{0};
  FType m_GrandPmtEdgePixelYSize{0};
  FType m_GrandPmtEffectivePixelXSize{0};
  FType m_GrandPmtEffectivePixelYSize{0};
  FType m_GrandPmtAnodeHalfThickness{0};

  bool m_PmtIsGrand{false};

  /// The PMT Anode detector element
  IDetectorElement* m_dePmtAnode = nullptr;
};
