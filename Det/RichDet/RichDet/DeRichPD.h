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
/** @file DeRichPD.h
 *
 *  Header file for detector description class : DeRichPD
 *
 *  @author Chris Jones   christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================

#pragma once

// STL
#include <memory>

// LHCbKernel
#include "Kernel/RichSmartID.h"

// RichDet
#include "RichDet/DeRichBase.h"
#include "RichDet/Rich1DTabFunc.h"
#include "RichDet/Rich1DTabProperty.h"

// RichUtils
#include "RichUtils/RichSIMDTypes.h"

//=============================================================================
/** @class DeRichPD DeRichPD.h RichDet/DeRichPD.h
 *
 *  Base class for all photon detectors (HPDs, MaPMTs).
 *  Used by Brunel, Boole etc., so they are neutral to the PD technology.
 *
 *  @author Chris Jones  christopher.rob.jones@cern.ch
 *  @date   2011-09-23
 */
//=============================================================================
class DeRichPD : public DeRichBase {

public:
  /// Standard constructor
  explicit DeRichPD( const std::string& name = "" ) : DeRichBase( name ) {}

  /// Destructor
  virtual ~DeRichPD() = default;

  /// Common initialisation
  StatusCode initialize() override;

public:
  /** Retrieves the HPD Quantum Efficiency interpolation function.
   *  For a given photon momentum (eV) returns the PD Quantum Efficiency percentage.
   *  @return pointer to the interpolation function for QuantumEff(PhotMom)
   */
  inline const Rich::TabulatedProperty1D* pdQuantumEff() const noexcept { return m_pdQuantumEffFunc.get(); }

public:
  /// scalar FP type for SIMD objects
  using FP = Rich::SIMD::DefaultScalarFP;
  /// SIMD float type
  using SIMDFP = Rich::SIMD::FP<FP>;
  /// SIMD Point
  using SIMDPoint = Rich::SIMD::Point<FP>;
  /// Type for SmartIDs container.
  using SmartIDs = Rich::SIMD::STDArray<LHCb::RichSmartID>;
  /// SIMD uint32 type
  using SIMDUINT = Rich::SIMD::UInt32;

public: // virtual methods to be implemented by derived classes
  /** @brief Converts a RichSmartID to a point in global coordinates.
   *
   *  The point can be given either on the inside of the PD window (photocathode) if
   *  photoCathodeSide=true or on the outside including refraction correction if
   *  photoCathodeSide=false
   *
   *  @param[in]  smartID     The RichSmartID pixel channel ID
   *  @param[out] detectPoint The position in global coordinates
   *  @param[in]  photoCathodeSide Set to false to include refraction on PD window
   *
   *  @return StatusCode indicating if the conversion was successful or not
   *  @retval true  Conversion was successful
   *  @retval false Conversion failed
   */
  virtual bool detectionPoint( const LHCb::RichSmartID smartID, Gaudi::XYZPoint& detectPoint,
                               bool photoCathodeSide = false ) const = 0;

  /** @brief Converts an 'SIMD' array of RichSmartIDs to an SIMD point in
   *         global coordinates.
   *
   *  The point can be given either on the inside of the PD window (photocathode) if
   *  photoCathodeSide=true or on the outside including refraction correction if
   *  photoCathodeSide=false
   *
   *  @param[in]  smartID     The RichSmartID pixel channel IDs
   *  @param[out] detectPoint The SIMD position(s) in global coordinates
   *  @param[in]  photoCathodeSide Set to false to include refraction on PD window
   *
   *  @return Mask indicating if the conversion was successful or not for each scalar entry
   */
  virtual SIMDFP::mask_type detectionPoint( const SmartIDs& smartID, SIMDPoint& detectPoint,
                                            bool photoCathodeSide = false ) const = 0;

public:
  /// Access the RICH
  inline Rich::DetectorType rich() const noexcept { return m_rich; }

  /// Access the actual physical area of the pixels in the PD (in mm^2)
  inline float pixelArea() const noexcept { return m_pixelArea; }

  /// Access the effective pixel area (in mm^2) including any demagnification factors
  inline float effectivePixelArea() const noexcept { return m_effPixelArea; }

  /// Access the effective number of active pixels
  inline float effectiveNumActivePixels() const noexcept { return m_effNumActivePixs; }

private:
  /// The RICH this PD is in
  Rich::DetectorType m_rich = Rich::InvalidDetector;

protected: // to be initialised by derived classes
  /// Interpolated property for HPD quantum efficiency
  std::shared_ptr<const Rich::TabulatedProperty1D> m_pdQuantumEffFunc;

  /// The pixel area (in mm^2)
  float m_pixelArea{0};

  /// The effective pixel area (in mm^2) including any demagnification factors
  float m_effPixelArea{0};

  /// The effective number of active pixels (including acceptance effects).
  float m_effNumActivePixs{0};
};
