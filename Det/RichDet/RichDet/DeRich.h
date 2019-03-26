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
/** @file DeRich.h
 *
 *  Header file for detector description class : DeRich
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#pragma once

// STL
#include <array>
#include <memory>
#include <vector>

// Mathcore
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

// LHCbKernel
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"

// Local
#include "RichDet/DeRichBase.h"
#include "RichDet/DeRichLocations.h"
#include "RichDet/Rich1DTabProperty.h"
#include "RichDet/RichDetConfigType.h"
#include "RichDet/RichMirrorSegPosition.h"

// Utils
#include "RichUtils/RichSIMDRayTracing.h"

class DeRichPDPanel;
class DeRichPD;

/** @class DeRich DeRich.h
 *
 * Class for generic info about the Rich(1/2) system.
 * DeRich is an abstract class contaning the common parts of the DeRich1
 * and DeRich2 classes.
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 */
class DeRich : public DeRichBase {

public:
  /**
   * Constructor for this class
   */
  DeRich( const std::string& name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRich() = default;

  /**
   * This is where most of the geometry is read and variables initialised
   * @return Status of initialisation
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override;

  /**
   * Returns the nominal centre of curvature of the spherical mirror for
   * this Rich for the given side
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal centre of curvature
   */
  inline const Gaudi::XYZPoint& nominalCentreOfCurvature( const Rich::Side side ) const noexcept {
    return m_nominalCentresOfCurvature[side];
  }

  /**
   * Returns the nominal centres of curvature of the spherical mirror for
   * this Rich for the given side
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal centre of curvature
   */
  inline const Rich::SIMD::Point<Rich::SIMD::DefaultScalarFP>&
  nominalCentreOfCurvatureSIMD( const Rich::Side side ) const noexcept {
    return m_nominalCentresOfCurvatureSIMD[side];
  }

  /**
   * Returns the nominal centres of curvature of the spherical mirror for
   * this Rich for the given sides
   *
   * @param sides Which sides : top, bottom (Rich1), left, right (Rich2)
   * @return The nominal centre of curvature
   */
  inline Rich::SIMD::Point<Rich::SIMD::DefaultScalarFP> nominalCentreOfCurvature( const Rich::SIMD::Sides& sides ) const
      noexcept {
    using namespace Rich::SIMD;
    using MaskType = Point<DefaultScalarFP>::Scalar::MaskType;
    // Start by making CoCs for each side
    const auto& CoC1( nominalCentreOfCurvatureSIMD( Rich::firstSide ) );
    const auto& CoC2( nominalCentreOfCurvatureSIMD( Rich::secondSide ) );
    // local copy of X,Y,Z for first side
    auto X = CoC1.X();
    auto Y = CoC1.Y();
    auto Z = CoC1.Z();
    // mask for side 2
    const auto m = LHCb::SIMD::simd_cast<MaskType>( sides == Sides( Rich::secondSide ) );
    // update values for side 2
    X( m ) = CoC2.X();
    Y( m ) = CoC2.Y();
    Z( m ) = CoC2.Z();
    // return the final result
    return Point<DefaultScalarFP>( X, Y, Z );
  }

  /**
   * Returns the nominal normal vector of the flat mirror plane for this Rich
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal normal vector
   */
  inline const Gaudi::XYZVector& nominalNormal( const Rich::Side side ) const noexcept {
    return m_nominalNormals[side];
  }

  /**
   * Returns the SIMD nominal normal vector of the flat mirror plane for this Rich
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal normal vector
   */
  inline const Rich::SIMD::Vector<Rich::SIMD::DefaultScalarFP>& nominalNormalSIMD( const Rich::Side side ) const
      noexcept {
    return m_nominalNormalsSIMD[side];
  }

  /**
   * Returns the nominal flat mirror plane for this Rich
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal flat mirror plane
   */
  inline const Gaudi::Plane3D& nominalPlane( const Rich::Side side ) const noexcept { return m_nominalPlanes[side]; }

  /**
   * Returns the nominal flat mirror plane for this Rich
   *
   * @param side Which side: top, bottom (Rich1), left, right (Rich2)
   * @return The nominal flat mirror plane
   */
  inline const Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>& nominalPlaneSIMD( const Rich::Side side ) const
      noexcept {
    return m_nominalPlanesSIMD[side];
  }

  /**
   * Returns the nominal flat mirror plane for this Rich
   *
   * @param sides Which sides : top, bottom (Rich1), left, right (Rich2)
   * @return The nominal flat mirror plane
   */
  inline const Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP> nominalPlane( const Rich::SIMD::Sides& sides ) const
      noexcept {
    using namespace Rich::SIMD;
    using MaskType = Point<DefaultScalarFP>::Scalar::MaskType;
    // start with the SIMD planes for each side
    const auto& P1( nominalPlaneSIMD( Rich::firstSide ) );
    const auto& P2( nominalPlaneSIMD( Rich::secondSide ) );
    // local copy of A,B,C,D for first side
    auto A = P1.A();
    auto B = P1.B();
    auto C = P1.C();
    auto D = P1.D();
    // mask for side 2
    const auto m = LHCb::SIMD::simd_cast<MaskType>( sides == Sides( Rich::secondSide ) );
    // update values for side 2
    A( m ) = P2.A();
    B( m ) = P2.B();
    C( m ) = P2.C();
    D( m ) = P2.D();
    // return the SIMD plane object
    return Plane<DefaultScalarFP>( A, B, C, D );
  }

  /**
   * Check on which side of this Rich lies this point
   *
   * @param point A point in the global coordinate system
   * @return The side for this point
   */
  template <typename POINT>
  inline decltype( auto ) side( const POINT& point ) const noexcept {
    return side( point.x(), point.y() );
  }

  /**
   * Check on which side of this Rich lies this (x,y) point
   *
   * @param point A point in the global coordinate system
   * @return The side for this point
   */
  template <typename TYPE>
  inline decltype( auto ) side( const TYPE x, const TYPE y ) const noexcept {
    if constexpr ( std::is_arithmetic<TYPE>::value ) {
      // Scalar
      return ( Rich::Rich1 == rich() ? ( y < 0 ? Rich::bottom : Rich::top ) : ( x < 0 ? Rich::right : Rich::left ) );
    } else {
      // SIMD
      using namespace Rich::SIMD;
      Sides sides( Rich::firstSide ); // R1 top or R2 left
      // update as needed to R1 bottom or R2 right
      // Is there a better way to do ??
      if ( Rich::Rich1 == rich() ) {
        sides( LHCb::SIMD::simd_cast<Sides::MaskType>( y < TYPE::Zero() ) ) = Sides( Rich::secondSide );
      } else {
        sides( LHCb::SIMD::simd_cast<Sides::MaskType>( x < TYPE::Zero() ) ) = Sides( Rich::secondSide );
      }
      return sides;
    }
  }

  /**
   * Returns the detector type for this Rich
   *
   * @return The detector type
   */
  inline Rich::DetectorType rich() const noexcept { return m_rich; }

  /**
   * Returns the RichSystem
   *
   * @return The DeRichSystem object
   */
  inline DeRichSystem* deRichSystem() noexcept { return deRichSys(); }

  /**
   * Returns the nominal spherical mirror radius for this Rich
   *
   * @return The nominal spherical mirror radius
   */
  inline double sphMirrorRadius() const noexcept { return m_sphMirrorRadius; }

  /**
   * Returns the nominal spherical mirror radius for this Rich
   *
   * @return The nominal spherical mirror radius
   */
  inline Rich::SIMD::FP<Rich::SIMD::DefaultScalarFP> sphMirrorRadiusSIMD() const noexcept {
    return m_sphMirrorRadiusSIMD;
  }

  /// Returns the Photon Detector config type 0=hpd, 1=pmt
  inline Rich::RichPhDetConfigType RichPhotoDetConfig() const noexcept { return m_RichPhotoDetConfig; }

  /** Returns the Rich Geometry config type 0=current optics,
   *  1 upgrade optics, 2 horizontal rich1-upgrade optics.
   *
   *  CRJ - This should be an enum, not an int ....
   *        Should also be cleaned up now the horizontal RICH1 is no longer an option
   */
  inline int RichGeometryConfig() const noexcept { return m_RichGeometryConfig; }

  // returns phodetector array config 0=standardpmt, 1=grandpmt, 2= mixture of stand+grandpmt
  inline int Rich2PhotoDetectorArrayConfig() const noexcept { return m_Rich2PhotoDetectorArrayConfig; }

  /// Use large PMTs
  inline bool Rich2UseGrandPmt() const noexcept { return m_Rich2UseGrandPmt; }

  /// Use large+small PMTs
  inline bool Rich2UseMixedPmt() const noexcept { return m_Rich2UseMixedPmt; }

  /**
   * Returns a pointer to the tabulated property that holds the refractive
   * index for the gas window for this Rich
   *
   * @return Pointer to gas window refIndex
   */
  inline const Rich::TabulatedProperty1D* gasWinRefIndex() const noexcept { return m_gasWinRefIndex.get(); }

  /**
   * Returns a pointer to the tabulated property that holds the absorption
   * length of the gas window for this Rich
   *
   * @return Pointer gas window absorption length
   */
  inline const Rich::TabulatedProperty1D* gasWinAbsLength() const noexcept { return m_gasWinAbsLength.get(); }

  /**
   * Returns a pointer to the tabulated property that holds the nominal quantum
   * efficiency of an Photodetector
   *
   * @return Pointer to quantum efficiency (can be null)
   */
  inline const std::shared_ptr<const Rich::TabulatedProperty1D>& nominalPDQuantumEff() const noexcept {
    return m_nominalPDQuantumEff;
  }

  /**
   * Returns a pointer to the tabulated property that holds the nominal reflectivity
   * of the spherical mirror.
   *
   * @return Pointer to nominal spherical mirror reflectivity
   */
  inline const Rich::TabulatedProperty1D* nominalSphMirrorRefl() const noexcept { return m_nominalSphMirrorRefl.get(); }

  /**
   * Returns a pointer to the tabulated property that holds the nominal reflectivity
   * of the flat mirror.
   *
   * @return Pointer to nominal flat mirror reflectivity
   */
  inline const Rich::TabulatedProperty1D* nominalSecMirrorRefl() const noexcept { return m_nominalSecMirrorRefl.get(); }

  /**
   * Method to find the row/column of a spherical mirror segment.  It can
   * be used to test if the mirror segment is at the edge or not
   * @return Position (row/column) for this spherical mirror segment
   */
  Rich::MirrorSegPosition sphMirrorSegPos( const int mirrorNumber ) const;

  /**
   * Method to find the row/column of a flat mirror segment. It can be used to
   * test if the mirror segment is at the edge or not
   *
   * @return Position (row/column) for this flat mirror segment
   */
  Rich::MirrorSegPosition secMirrorSegPos( const int mirrorNumber ) const;

  /// sensitive volume identifier for hpd version. to be phased out
  int sensitiveVolumeID( const Gaudi::XYZPoint& globalPoint ) const override;

  /// Access PD Panels
  inline const DeRichPDPanel* pdPanel( const Rich::Side panel ) const noexcept { return m_PDPanels[panel]; }

  /// Ray trace a given direction with the given PD panel (scalar)
  LHCb::RichTraceMode::RayTraceResult rayTrace( const Rich::Side side, const Gaudi::XYZPoint& pGlobal,
                                                const Gaudi::XYZVector& vGlobal, Gaudi::XYZPoint& hitPosition,
                                                LHCb::RichSmartID& smartID, const DeRichPD*& dePD,
                                                const LHCb::RichTraceMode mode ) const;

  /// type for SIMD ray tracing result
  using SIMDRayTResult = Rich::RayTracingUtils::SIMDResult;
  /// scalar FP type for SIMD objects
  using FP = Rich::SIMD::DefaultScalarFP;
  /// SIMD float type
  using SIMDFP = Rich::SIMD::FP<FP>;

  /// Ray trace a given direction with the given PD panel (SIMD)
  SIMDRayTResult::Results rayTrace( const Rich::Side side, const Rich::SIMD::Point<FP>& pGlobal,
                                    const Rich::SIMD::Vector<FP>& vGlobal, Rich::SIMD::Point<FP>& hitPosition,
                                    SIMDRayTResult::SmartIDs& smartID, SIMDRayTResult::PDs& PDs,
                                    const LHCb::RichTraceMode mode ) const;

  /// Ray trace a given direction with the correct PD panel (SIMD)
  SIMDRayTResult::Results rayTrace( const Rich::SIMD::Sides& sides, const Rich::SIMD::Point<FP>& pGlobal,
                                    const Rich::SIMD::Vector<FP>& vGlobal, Rich::SIMD::Point<FP>& hitPosition,
                                    SIMDRayTResult::SmartIDs& smartID, SIMDRayTResult::PDs& PDs,
                                    const LHCb::RichTraceMode mode ) const;

protected:
  /// Load on demand the nominal PD Q.E.
  void loadNominalQuantumEff();

  /// Load the PD panels
  void loadPDPanels();

  /// Fill SIMD caches
  void fillSIMDTypes();

private:
  /// Access the name for a given panel
  virtual const std::string panelName( const Rich::Side panel ) const;

  /// Load on demand the nominal HPD Q.E.
  std::shared_ptr<const Rich::TabulatedProperty1D> loadNominalHPDQuantumEff();

  /// Load on demand the nominal PMT Q.E.
  std::shared_ptr<const Rich::TabulatedProperty1D> loadNominalPMTQuantumEff();

protected:
  /// rich type
  Rich::DetectorType m_rich = Rich::InvalidDetector;

  /// The nominal radius of the spherical mirror
  double m_sphMirrorRadius{0};

  /// RICH PhotoDetector Configuration
  Rich::RichPhDetConfigType m_RichPhotoDetConfig = Rich::HPDConfig;

  /// Rich Geometry Configuration
  int m_RichGeometryConfig{0};

  /** RICH2 PhotoDetector Configuration
   * This is specific to RICH2, but is part of the overall RICH geometry config.
   */
  int m_Rich2PhotoDetectorArrayConfig{0};

  /** Use large PMTs in RICH2
   *  This is specific to RICH2, but the flags are in RICH1 part of DB since
   *  all the pmts are created there together. So at the moment giving the capability to access it
   * from RICH1 or RICH2 detector elements as per convenience and saving cpu time. There is also the
   * possibility of different array config in RICH1 in future. So keeping all the flags here makes
   * it easier to navigate through them. The mixed pmt means some pmts are large and others are with
   * standard size.
   */
  bool m_Rich2UseGrandPmt{false};

  /// Use mixed PMTs
  bool m_Rich2UseMixedPmt{false};

  /// refractive index of the quartz gas window
  std::unique_ptr<const Rich::TabulatedProperty1D> m_gasWinRefIndex;

  /// absorption length of the quartz gas window
  std::unique_ptr<const Rich::TabulatedProperty1D> m_gasWinAbsLength;

  /// spherical mirror reflectivity
  std::unique_ptr<const Rich::TabulatedProperty1D> m_nominalSphMirrorRefl;

  /// flat mirror reflectivity
  std::unique_ptr<const Rich::TabulatedProperty1D> m_nominalSecMirrorRefl;

  /// Nominal planes for each panel
  Rich::PanelArray<Gaudi::Plane3D> m_nominalPlanes = {{}};

  /// The nominal normal vector of the flat mirror planes
  Rich::PanelArray<Gaudi::XYZVector> m_nominalNormals = {{}};

  /// The nominal centres of curvature of the spherical mirrors
  Rich::PanelArray<Gaudi::XYZPoint> m_nominalCentresOfCurvature = {{}};

private: // data
  /// Pointers to the PD panels of this Rich detector
  Rich::DetectorArray<const DeRichPDPanel*> m_PDPanels{{nullptr, nullptr}};

  /// flag to test if the xml supports mirror position info
  bool m_positionInfo{false};

  int m_sphMirrorSegRows{0}; ///< number of spherical mirror rows
  int m_sphMirrorSegCols{0}; ///< number of spherical mirror columns
  int m_secMirrorSegRows{0}; ///< number of secondary mirror rows
  int m_secMirrorSegCols{0}; ///< number of secondary mirror columns

  /// PD quantum efficiency
  std::shared_ptr<const Rich::TabulatedProperty1D> m_nominalPDQuantumEff;

private:
  // SIMD copies of various types

  /// SIMD Nominal planes for each panel
  Rich::PanelArray<Rich::SIMD::Plane<Rich::SIMD::DefaultScalarFP>> m_nominalPlanesSIMD = {{}};

  /// SIMD nominal normal vector of the flat mirror planes
  Rich::PanelArray<Rich::SIMD::Vector<Rich::SIMD::DefaultScalarFP>> m_nominalNormalsSIMD = {{}};

  /// The nominal centres of curvature of the spherical mirrors
  Rich::PanelArray<Rich::SIMD::Point<Rich::SIMD::DefaultScalarFP>> m_nominalCentresOfCurvatureSIMD = {{}};

  /// The nominal radius of the spherical mirror
  Rich::SIMD::FP<Rich::SIMD::DefaultScalarFP> m_sphMirrorRadiusSIMD;
};
