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
#include <mutex>

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
  bool detectionPoint( const LHCb::RichSmartID smartID,                 //
                       Gaudi::XYZPoint&        detectPoint,             //
                       bool                    photoCathodeSide = false //
                       ) const override final;

  // (SIMD) Converts RichSmartIDs to an SIMD point in global coordinates.
  DeRichPD::SIMDFP::mask_type detectionPoint( const SmartIDs& smartID,                 //
                                              SIMDPoint&      detectPoint,             //
                                              bool            photoCathodeSide = false //
                                              ) const override final;

  /** Converts a RichSmartID to a point on the anode in global coordinates.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return The detection point on the anode in global coordinates
   */
  Gaudi::XYZPoint detPointOnAnode( const LHCb::RichSmartID& smartID ) const;

  /** Converts a RichSmartID to the pixel number in a PMT as in the hardware readout scheme.
   *  @param[in] smartID The RichSmartID for the PMT channel
   *  @return Pixel nr in a PMT as in the hardware readout scheme
   */
  int getPixelNumberInPmt( const LHCb::RichSmartID& smartID ) const;

  void setPmtIsGrandFlag( const bool isGrand );

  inline bool PmtIsGrand() const noexcept { return m_PmtIsGrand; }

  /// Shortcut to the 'to local' matrix
  inline const Gaudi::Transform3D& toLocalMatrix() const noexcept { return m_toLocM; }

  /// Shortcut to the 'to global' matrix
  inline const Gaudi::Transform3D& toGlobalMatrix() const noexcept { return geometry()->toGlobalMatrix(); }

  /// Set the zeroInPanelLocal position
  inline void setZeroInPanelLocal( const Gaudi::XYZPoint& p ) noexcept { m_zeroInPanelLocal = p; }

  /// Access the zeroInPanelLocal position
  inline const Gaudi::XYZPoint& zeroInPanelLocal() const noexcept { return m_zeroInPanelLocal; }

  // Access the PMT channel properties
  inline double PmtChannelGainMean( const LHCb::RichSmartID& smartID ) const noexcept {
    return m_PmtChannelGainMean[getPixelNumberInPmt( smartID )];
  }
  inline double PmtChannelGainRms( const LHCb::RichSmartID& smartID ) const noexcept {
    return m_PmtChannelGainRms[getPixelNumberInPmt( smartID )];
  }
  inline double PmtChannelThreshold( const LHCb::RichSmartID& smartID ) const noexcept {
    return m_PmtChannelThreshold[getPixelNumberInPmt( smartID )];
  }
  inline double PmtChannelSinProbability( const LHCb::RichSmartID& smartID ) const noexcept {
    return m_PmtChannelSinProbability[getPixelNumberInPmt( smartID )];
  }
  inline double PmtAverageOccupancy() const noexcept { return m_PmtAverageOccupancy; }

private:
  // definitions

  /// int type
  using IPix = LHCb::RichSmartID::DataType;

private:
  // initialisation

  DetectorElement* getFirstRich();

  StatusCode getPMTParameters();
  StatusCode readPMTPropertiesFromDB();
  StatusCode initPMTQuantumEff();
  StatusCode updateGeometry();

private:
  inline Gaudi::XYZPoint getAnodeHitCoordFromPixelNum( const IPix fracPixelCol, //
                                                       const IPix fracPixelRow  //
                                                       ) const noexcept {
    const auto xh =
        ( (FP)fracPixelCol - scalar( m_pddata.PmtNumPixColFrac ) ) * scalar( m_pddata.PmtEffectivePixelXSize );
    const auto yh =
        ( (FP)fracPixelRow - scalar( m_pddata.PmtNumPixRowFrac ) ) * scalar( m_pddata.PmtEffectivePixelYSize );
    return {xh, yh, scalar( m_pddata.PmtAnodeHalfThickness )};
  }

  inline SIMDPoint getAnodeHitCoordFromPixelNum( const SIMDUINT& fracPixelCol, //
                                                 const SIMDUINT& fracPixelRow  //
                                                 ) const noexcept {
    using namespace LHCb::SIMD;
    const auto xh = ( simd_cast<SIMDFP>( fracPixelCol ) - m_pddata.PmtNumPixColFrac ) * m_pddata.PmtEffectivePixelXSize;
    const auto yh = ( simd_cast<SIMDFP>( fracPixelRow ) - m_pddata.PmtNumPixRowFrac ) * m_pddata.PmtEffectivePixelYSize;
    return {xh, yh, m_pddata.PmtAnodeHalfThickness};
  }

  inline Gaudi::XYZPoint getAnodeHitCoordFromGrandPixelNum( const IPix fracPixelCol, //
                                                            const IPix fracPixelRow  //
                                                            ) const noexcept {
    const auto aXEffPixel = ( fracPixelCol == 0 || fracPixelCol == ( scalar( m_pddata.PmtNumPixCol ) - 1 )
                                  ? scalar( m_pddata.GrandPmtEdgePixelXSize )
                                  : scalar( m_pddata.GrandPmtEffectivePixelXSize ) );
    const auto aYEffPixel = ( fracPixelRow == 0 || fracPixelRow == ( scalar( m_pddata.PmtNumPixRow ) - 1 )
                                  ? scalar( m_pddata.GrandPmtEdgePixelYSize )
                                  : scalar( m_pddata.GrandPmtEffectivePixelYSize ) );
    const auto xh         = ( (FP)fracPixelCol - scalar( m_pddata.PmtNumPixColFrac ) ) * aXEffPixel;
    const auto yh         = ( (FP)fracPixelRow - scalar( m_pddata.PmtNumPixRowFrac ) ) * aYEffPixel;
    return {xh, yh, scalar( m_pddata.GrandPmtAnodeHalfThickness )};
  }

  inline SIMDPoint getAnodeHitCoordFromGrandPixelNum( const SIMDUINT& fracPixelCol, //
                                                      const SIMDUINT& fracPixelRow  //
                                                      ) const noexcept {
    using namespace LHCb::SIMD;

    const auto xmask =
        ( fracPixelCol == SIMDUINT::Zero() ) || ( fracPixelCol == ( m_pddata.PmtNumPixCol - SIMDUINT::One() ) );
    const auto ymask =
        ( fracPixelRow == SIMDUINT::Zero() ) || ( fracPixelRow == ( m_pddata.PmtNumPixRow - SIMDUINT::One() ) );

    auto aXEffPixel                                     = m_pddata.GrandPmtEffectivePixelXSize;
    aXEffPixel( simd_cast<SIMDFP::mask_type>( xmask ) ) = m_pddata.GrandPmtEdgePixelXSize;
    auto aYEffPixel                                     = m_pddata.GrandPmtEffectivePixelYSize;
    aYEffPixel( simd_cast<SIMDFP::mask_type>( ymask ) ) = m_pddata.GrandPmtEdgePixelYSize;

    const auto xh = ( simd_cast<SIMDFP>( fracPixelCol ) - m_pddata.PmtNumPixColFrac ) * aXEffPixel;
    const auto yh = ( simd_cast<SIMDFP>( fracPixelRow ) - m_pddata.PmtNumPixRowFrac ) * aYEffPixel;
    return {xh, yh, m_pddata.GrandPmtAnodeHalfThickness};
  }

  inline Gaudi::XYZPoint getAnodeHitCoordFromMultTypePixelNum( const IPix fracPixelCol, //
                                                               const IPix fracPixelRow  //
                                                               ) const noexcept {
    return ( PmtIsGrand() ? getAnodeHitCoordFromGrandPixelNum( fracPixelCol, fracPixelRow )
                          : getAnodeHitCoordFromPixelNum( fracPixelCol, fracPixelRow ) );
  }

  inline SIMDPoint getAnodeHitCoordFromMultTypePixelNum( const SIMDUINT& fracPixelCol, //
                                                         const SIMDUINT& fracPixelRow  //
                                                         ) const noexcept {
    return ( PmtIsGrand() ? getAnodeHitCoordFromGrandPixelNum( fracPixelCol, fracPixelRow )
                          : getAnodeHitCoordFromPixelNum( fracPixelCol, fracPixelRow ) );
  }

private:
  // definitions

  /// PMT data storage
  class PMTData {

  public:
    void initialise( const DetectorElement* deRich );

  public:
    // parameter data

    SIMDFP PmtEffectivePixelXSize = SIMDFP::Zero();
    SIMDFP PmtEffectivePixelYSize = SIMDFP::Zero();

    SIMDFP PmtAnodeHalfThickness = SIMDFP::Zero();

    SIMDUINT PmtNumPixCol = SIMDUINT::Zero();
    SIMDUINT PmtNumPixRow = SIMDUINT::Zero();

    SIMDFP PmtNumPixColFrac = SIMDFP::Zero();
    SIMDFP PmtNumPixRowFrac = SIMDFP::Zero();

    SIMDFP PmtQwZSize = SIMDFP::Zero();

    SIMDFP zShift = SIMDFP::Zero();

    SIMDFP GrandPmtEdgePixelXSize = SIMDFP::Zero();
    SIMDFP GrandPmtEdgePixelYSize = SIMDFP::Zero();

    SIMDFP GrandPmtEffectivePixelXSize = SIMDFP::Zero();
    SIMDFP GrandPmtEffectivePixelYSize = SIMDFP::Zero();

    SIMDFP GrandPmtAnodeHalfThickness = SIMDFP::Zero();

  private:
    // implementation data
    std::mutex m_initLock;
    bool       m_initialised{false};
  };

private:
  // CRJ - To minimise memory footprint do not define data members
  //       that are not needed (outside of getPMTParameters() setup)
  //       Also, data members should be arranged so most commonly
  //       accessed are listed first.

  // shared PD data. Same for every PMT.
  static PMTData m_pddata;

  /// Cache 'to local' matrix
  Gaudi::Transform3D m_toLocM;

  /// SIMD To global transform
  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_toGlobalMatrixSIMD;

  /* Cache the point where {0,0,0} in this PD local frame appears
   * in the local frame of its parent PD panel */
  Gaudi::XYZPoint m_zeroInPanelLocal;

  /// Flag to say if this is a large or small PMT
  bool m_PmtIsGrand{false};

  /// The PMT Anode detector element
  IDetectorElement* m_dePmtAnode = nullptr;

  // PMT properties (different for each PMT)
  using PMTChannelPropertyVector = std::vector<double>;
  PMTChannelPropertyVector m_PmtChannelGainMean;
  PMTChannelPropertyVector m_PmtChannelGainRms;
  PMTChannelPropertyVector m_PmtChannelThreshold;
  PMTChannelPropertyVector m_PmtChannelSinProbability;
  double                   m_PmtAverageOccupancy{0};
};
