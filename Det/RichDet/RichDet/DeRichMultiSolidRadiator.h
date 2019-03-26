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

/** @file DeRichMultiSolidRadiator.h
 *
 *  Header file for detector description class : DeRichMultiSolidRadiator
 *
 *  @author Antonis Papanestis  a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#pragma once

// Include files
#include "GaudiKernel/Transform3DTypes.h"
#include "RichDet/DeRichRadiator.h"

// External declarations
extern const CLID CLID_DeRichMultiSolidRadiator;

/** @class DeRichMultiSolidRadiator RichDet/DeRichMultiSolidRadiator.h
 *
 * Implementation of the DeRichRadiator for a radiator with multiple solids.
 * WARNING: The detector element is attached to the physical volume that
 * contain all the physical volumes of the radiator material, so the use of
 * geometryInfo should be done with care.
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 */

class DeRichMultiSolidRadiator : public DeRichRadiator {

public:
  /**
   * Constructor for this class
   */
  DeRichMultiSolidRadiator( const std::string& name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRichMultiSolidRadiator() = default;

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
   * @retval StatusCode::FAILURE Initialisation failed, program should
   * terminate
   */
  StatusCode initialize() override final;

  // Finds the next intersection point with radiator.
  StatusCode nextIntersectionPoint( const Gaudi::XYZPoint& pGlobal, const Gaudi::XYZVector& vGlobal,
                                    Gaudi::XYZPoint& returnPoint ) const override final;

  // Finds the entry and exit points of the radiator after looking
  // at all the solids to find the first and last intersection point.
  StatusCode intersectionPoints( const Gaudi::XYZPoint& position, const Gaudi::XYZVector& direction,
                                 Gaudi::XYZPoint& entryPoint, Gaudi::XYZPoint& exitPoint ) const override final;

  // Finds the intersection points with radiator after looking at all the solids
  unsigned int intersectionPoints( const Gaudi::XYZPoint& pGlobal, const Gaudi::XYZVector& vGlobal,
                                   std::vector<Gaudi::XYZPoint>& points ) const override final;

  // Finds the intersections (entry/exit) with radiator. For boolean solids there
  unsigned int intersections( const Gaudi::XYZPoint& pGlobal, const Gaudi::XYZVector& vGlobal,
                              Rich::RadIntersection::Vector& intersections ) const override final;

  // Returns the refractive index at the given photon energy for this radiator
  double refractiveIndex( const double energy, const bool hlt = false ) const override final;

public:
  /** Access to the list of single solid radiators.
   *  Will be either sub-tiles, or tiles, depending on which are active in the DB
   */
  inline const DeRichRadiator::Vector& radiators() const noexcept { return m_radiators; }

  /// Explicit Access to sub-tiles (if active)
  inline const DeRichRadiator::Vector& subTileRadiators() const noexcept { return radiators(); }

  /// Explicit Access to full tiles
  inline const DeRichRadiator::Vector& fullTileRadiators() const noexcept { return m_fullTiles; }

private:
  /// Add radiator volumes
  StatusCode addVolumes( const ILVolume* lv, const std::string& volName, const Gaudi::Transform3D& toUpperLevel,
                         const int VolIndex );

  StatusCode addFullTileVolumes( const ILVolume* lv, const std::string& volName,
                                 const Gaudi::Transform3D& toUpperLevel );

  StatusCode addSubTileVolumes( const ILVolume* lv, const std::string& volName,
                                const Gaudi::Transform3D& toUpperLevel );

  /// UMS update on geometry changes
  StatusCode geometryUpdate();

private:
  /// vector of solids
  typedef std::vector<const ISolid*> Solids;
  /// vector of physical volumes
  typedef std::vector<const IPVolume*> PVolumes;
  /// vector of transformation matrices
  typedef std::vector<Gaudi::Transform3D> Transforms;

  Solids     m_solids;     ///< The solids that make up this radiator
  PVolumes   m_pVolumes;   ///< The physical volumes
  Transforms m_toTopLevel; ///< Tranforms to top level of the detector element
  Transforms m_toLowLevel; ///< Tranforms to low (solid) level of the detector element

  /// The DeRichRadiators (sub-tiles) that make up this radiator
  DeRichRadiator::Vector m_radiators;

  /// Explicit list of full tiles
  DeRichRadiator::Vector m_fullTiles;

  /// First update flag
  bool m_firstUpdate{true};
};
