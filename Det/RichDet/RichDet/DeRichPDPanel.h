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

//=============================================================================
/** @file DeRichPDPanel.h
 *
 *  Header file for detector description class : DeRichPDPanel
 *
 *  @author Chris Jones   christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================

#pragma once

// Gaudi
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Transform3DTypes.h"

// RichDet
#include "RichDet/DeRichBase.h"
#include "RichDet/DeRichPD.h"

// LHCbKernel
#include "Kernel/RichSmartID.h"
#include "Kernel/RichTraceMode.h"

// RichUtils
#include "RichUtils/RichDAQDefinitions.h"
#include "RichUtils/RichSIMDRayTracing.h"

//=============================================================================
/** @class DeRichPDPanel DeRichPDPanel.h RichDet/DeRichPDPanel.h
 *
 *  Base class for all photon detector panels (HPDs, MaPMTs).
 *  Used by Brunel, Boole etc., so they are neutral to the PD technology.
 *
 *  @author Chris Jones  christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================
class DeRichPDPanel : public DeRichBase {

public:
  /// Standard constructor
  explicit DeRichPDPanel( const std::string& name = "" );

  /// Destructor
  virtual ~DeRichPDPanel() = default;

public:
  /// Access the RICH detector type
  inline Rich::DetectorType rich() const noexcept { return m_rich; }

  /// Access the PD panel side
  inline Rich::Side side() const noexcept { return m_side; }

  /// Access the PD type
  inline LHCb::RichSmartID::IDType pdType() const noexcept { return m_pdType; }

  /// Access the RichSmartID for this panel
  inline LHCb::RichSmartID panelID() const noexcept { return m_panelID; }

  /// The number of PD columns in this panel
  inline unsigned int nPDColumns() const noexcept { return m_PDColumns; }

  /// The number of PDs in each columns
  inline unsigned int nPDsPerCol() const noexcept { return m_PDNumInCol; }

  /// The total number of PDs in this panel
  inline unsigned int nPDs() const noexcept { return m_PDMax; }

  /** @brief Retrieves the detection plane of the PD panel.
   *
   *  The plane is defined at the top of the PDs
   *
   *  @return The detection plane
   */
  inline const Gaudi::Plane3D& detectionPlane() const noexcept { return m_detectionPlane; }

  /**
   * Returns the Z coordinate of the detection plane in the local coordinate system.
   *
   * @return The detection plane Z coordinate (local system).
   */
  inline double detectPlaneZcoord() const noexcept { return m_detPlaneZ; }

  /**
   * Returns the offset (y in Rich1, x in Rich2) so that the two panels of
   * each detector appear side-by-side using the globalToPanel method.
   *
   * @return The offset for the globalToPanel method
   */
  inline double localOffset() const noexcept { return m_localOffset; }

  /** @brief Converts a RichSmartID to a point in global coordinates.
   *
   *  The point is given on the inside of the PD window, on the photocathode.
   *
   *  @param[in]  smartID     The HPD channel ID
   *  @param[out] detectPoint The detection point
   *  @param[in]  photoCathodeSide  If false use the outside of the PD window and correct
   *                                for refraction. If true use the photocathode side
   *
   *  @return StatusCode indicating if the conversion was successful or not
   *  @retval true  Conversion to photocathode was OK
   *  @retval false Impossible conversion to photocathode
   */
  inline bool detectionPoint( const LHCb::RichSmartID smartID,     //
                              Gaudi::XYZPoint&        detectPoint, //
                              const bool              photoCathodeSide = false ) const {
    return this->dePD( smartID )->detectionPoint( smartID, detectPoint, photoCathodeSide );
  }

  /** Get the global to local (PD panel) transform. The local frame is defined with an offset
   *  to accommodate both detector panels in one histogram and the x=0 in Rich1 and y=0 in
   *  Rich2 are aligned with the global x=0 and y=0.
   */
  inline const Gaudi::Transform3D& globalToPDPanelMatrix() const noexcept { return m_globalToPDPanelTransform; }

  /** Get the global to local (PD panel) transform. The local frame is defined with an offset
   *  to accommodate both detector panels in one histogram and the x=0 in Rich1 and y=0 in
   *  Rich2 are aligned with the global x=0 and y=0.
   */
  inline const Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP>& globalToPDPanelMatrixSIMD() const noexcept {
    return m_globalToPDPanelTransformSIMD;
  }

  /// Get the transform from the local to global frame. See comments at the globalToPDPanelMatrix
  /// method
  inline const Gaudi::Transform3D& PDPanelToGlobalMatrix() const noexcept { return m_PDPanelToGlobalTransform; }

public:
  /// type for SIMD ray tracing result
  using SIMDRayTResult = Rich::RayTracingUtils::SIMDResult;
  /// scalar FP type for SIMD objects
  using FP = Rich::SIMD::DefaultScalarFP;
  /// SIMD float type
  using SIMDFP = Rich::SIMD::FP<FP>;
  /// SIMD Int32 type
  using SIMDINT32 = Rich::SIMD::Int32;
  /// SIMD Point
  using SIMDPoint = Rich::SIMD::Point<FP>;
  /// SIMD Vector
  using SIMDVector = Rich::SIMD::Vector<FP>;

public: // virtual methods. Derived classes must implement these
  /** Access the DeRichPD object for a given PD RichSmartID
   *  @param[in] pdID The PD RichSmartID identifier
   *  @return Pointer to the associated DeRichPD object
   */
  virtual const DeRichPD* dePD( const LHCb::RichSmartID pdID ) const = 0;

  /** @brief Returns the intersection point with an HPD window given a vector
   *  and a point (scalar)
   *
   *  With the "circle" option a quick check is performed
   *  to test if there would be an intersection with a flat circle instead
   *  of the PD window.
   *
   *  @param[in]  pGlobal           The intersection start point
   *  @param[in]  vGlobal           The intersection direction
   *  @param[out] windowPointGlobal The return point on the PD window
   *  @param[out] smartID           The returned smartID with hit PD info
   *  @param[out] dePD              Pointer to the associated DePD object
   *  @param[in]  mode              The ray-tracing configuration mode
   *
   *  @return Status of intersection
   */
  virtual LHCb::RichTraceMode::RayTraceResult PDWindowPoint( const Gaudi::XYZPoint&    pGlobal,           //
                                                             const Gaudi::XYZVector&   vGlobal,           //
                                                             Gaudi::XYZPoint&          windowPointGlobal, //
                                                             LHCb::RichSmartID&        smartID,           //
                                                             const DeRichPD*&          dePD,              //
                                                             const LHCb::RichTraceMode mode ) const = 0;

  /** @brief Returns the intersection point with the detector plane given a vector
   *         and a point (Scalar).
   *
   * If mode is tight, returns true only if point is within
   * the detector coverage.
   *
   * @param[in]  vGlobal     The intersection direction
   * @param[in]  pGlobal     The intersection start point
   * @param[out] hitPosition The intersection point on the PD plane
   * @param[out] smartID     The returned smartID with hit PD info
   * @param[out] dePD        Pointer to the associated DePD object
   * @param[in]  mode        The ray-tracing configuration mode
   *
   * @return Intersection status
   */
  virtual LHCb::RichTraceMode::RayTraceResult detPlanePoint( const Gaudi::XYZPoint&    pGlobal,     //
                                                             const Gaudi::XYZVector&   vGlobal,     //
                                                             Gaudi::XYZPoint&          hitPosition, //
                                                             LHCb::RichSmartID&        smartID,     //
                                                             const DeRichPD*&          dePD,        //
                                                             const LHCb::RichTraceMode mode ) const = 0;

  /** @brief Returns the intersection point with an HPD window given a vector
   *  and a point (SIMD)
   *
   *  With the "circle" option a quick check is performed
   *  to test if there would be an intersection with a flat circle instead
   *  of the PD window.
   *
   *  @param[in]  pGlobal           The intersection start point
   *  @param[in]  vGlobal           The intersection direction
   *  @param[out] windowPointGlobal The return point on the PD window
   *  @param[out] smartID           The returned smartID with hit PD info
   *  @param[out] dePD              Pointer to the associated DePD object
   *  @param[in]  mode              The ray-tracing configuration mode
   *
   *  @return Status of intersection
   */
  virtual SIMDRayTResult::Results PDWindowPointSIMD( const SIMDPoint&          pGlobal,     //
                                                     const SIMDVector&         vGlobal,     //
                                                     SIMDPoint&                hitPosition, //
                                                     SIMDRayTResult::SmartIDs& smartID,     //
                                                     SIMDRayTResult::PDs&      PDs,         //
                                                     const LHCb::RichTraceMode mode ) const = 0;

  /** @brief Returns the intersection point with the detector plane given a vector
   *         and a point (SIMD).
   *
   * If mode is tight, returns true only if point is within
   * the detector coverage.
   *
   * @param[in]  vGlobal     The intersection direction
   * @param[in]  pGlobal     The intersection start point
   * @param[out] hitPosition The intersection point on the PD plane
   * @param[out] smartID     The returned smartID with hit PD info
   * @param[out] dePD        Pointer to the associated DePD object
   * @param[in]  mode        The ray-tracing configuration mode
   *
   * @return Intersection status
   */
  virtual SIMDRayTResult::Results detPlanePointSIMD( const SIMDPoint&          pGlobal,     //
                                                     const SIMDVector&         vGlobal,     //
                                                     SIMDPoint&                hitPosition, //
                                                     SIMDRayTResult::SmartIDs& smartID,     //
                                                     SIMDRayTResult::PDs&      PDs,         //
                                                     const LHCb::RichTraceMode mode ) const = 0;

  /** @brief Converts a Gaudi::XYZPoint in global coordinates to a RichSmartID.
   *
   *  The point is assumed to be on the actual detection volume (silicon pixel sensor).
   *
   *  @param[in]  globalPoint The point in global coordinates
   *  @param[out] id          The RichSmartID for the given point
   *
   *  @return Status of conversion
   *  @retval false Point outside silicon pixel sensor or in a position that
   *          could not have originated from the photocathode.
   */
  virtual bool smartID( const Gaudi::XYZPoint& globalPoint, LHCb::RichSmartID& id ) const = 0;

  /// Returns the PD number for the given RichSmartID
  virtual Rich::DAQ::PDPanelIndex pdNumber( const LHCb::RichSmartID& smartID ) const = 0;

  /**
   * Adds to the given vector all the available readout channels in this PD panel
   *
   * @param readoutChannels Vector of valid pixel IDs
   *
   * @return Status code
   * @retval true  Readout channel list was successfully determined
   * @retval false An error occured whilst determining the readout channels
   */
  virtual bool readoutChannelList( LHCb::RichSmartID::Vector& readoutChannels ) const = 0;

  /// Is a 'large' PD
  virtual bool isLargePD( const LHCb::RichSmartID smartID ) const = 0;

protected:
  /// Set the Rich and side
  void setRichSide( const Rich::DetectorType rich, const Rich::Side side ) noexcept {
    // set the Rich and Side enums
    m_rich = rich;
    m_side = side;
    // cache the panel ID
    m_panelID = LHCb::RichSmartID( rich, side, pdType() );
  }

protected:
  // Parameters that must be properly configured in the derived classes

  unsigned int m_PDColumns{0};  ///< Number of PD columns in the panel
  unsigned int m_PDNumInCol{0}; ///< Number of PDs in each column
  unsigned int m_PDMax{0};      ///< Total number of PDs in this panel

  /// The PD technology type (HPD or MaPMT)
  LHCb::RichSmartID::IDType m_pdType = LHCb::RichSmartID::Undefined;

  double m_localOffset{0}; ///< offset applied in the global to panel coordinate transformation

  double m_detPlaneZ{0}; ///< The z position of the detection plane in an PD panel

  Gaudi::Plane3D m_detectionPlane; ///< detection plane in global coordinates

  Gaudi::Transform3D m_globalToPDPanelTransform; ///< global to PD plane (local) transform
  Gaudi::Transform3D m_PDPanelToGlobalTransform; ///< local (PD plane) to global transform

  Rich::SIMD::Transform3D<Rich::SIMD::DefaultScalarFP> m_globalToPDPanelTransformSIMD;

private:
  // data

  /// SmartID for this panel
  LHCb::RichSmartID m_panelID;

  Rich::DetectorType m_rich = Rich::InvalidDetector; ///< The RICH detector type
  Rich::Side         m_side = Rich::InvalidSide;     ///< The RICH PD panel (up, down, left or right)
};
