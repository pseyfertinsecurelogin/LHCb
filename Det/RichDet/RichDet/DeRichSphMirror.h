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

/** @file DeRichSphMirror.h
 *
 *  Header file for detector description class : DeRichSphMirror
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */

#pragma once

// STL
#include <memory>

// Gaudi
#include "GaudiKernel/Plane3DTypes.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

#include "DetDesc/ISolid.h"
#include "RichDet/DeRichBase.h"
#include "RichDet/Rich1DTabProperty.h"

// Utils
#include "RichUtils/RichSIMDTypes.h"

// External declarations
extern const CLID CLID_DeRichSphMirror;

/** @class DeRichSphMirror RichDet/DeRichSphMirror.h
 *
 * Detector element for spherical mirrors providing geometry info
 *
 * @author Antonis Papanestis a.papanestis@rl.ac.uk
 * @date   2004-06-18
 */
class DeRichSphMirror : public DeRichBase {

public:
  /**
   * Constructor for this class
   */
  DeRichSphMirror( const std::string& name = "" );

  /**
   * Default destructor
   */
  virtual ~DeRichSphMirror() = default;

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override { return classID(); }

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
  StatusCode initialize() override;

  /**
   * Retrieves the centre of curvarute of this mirror
   * @return The centre of curvature of this mirror as a Gaudi::XYZPoint
   */
  inline const Gaudi::XYZPoint& centreOfCurvature() const noexcept { return m_centreOfCurvature; }

  /**
   * Retrieves the centre this mirror on the reflective surface.
   * @return The mirror centre as Gaudi::XYZPoint
   */
  inline const Gaudi::XYZPoint& mirrorCentre() const noexcept { return m_mirrorCentre; }

  /**
   * Retrieves the segment number of this spherical mirror
   * @return The number of this mirror
   */
  inline int mirrorNumber() const noexcept { return m_mirrorNumber; }

  /**
   * Retrieves the radius of this spherical mirror
   * @return The radius of this spherical mirror
   */
  inline decltype( auto ) radius() const noexcept { return m_radius; }

  /**
   * Checks if the direction intersects with the mirror
   * @return Status of intersection
   * @retval StatusCode::FAILURE No intersection
   */
  StatusCode intersects( const Gaudi::XYZPoint& globalP, const Gaudi::XYZVector& globalV ) const;

  /**
   * Checks if the direction intersects with the mirror and returns the
   * intersction point
   * @return Intersection point
   */
  StatusCode intersects( const Gaudi::XYZPoint& globalP, const Gaudi::XYZVector& globalV,
                         Gaudi::XYZPoint& intersectionPoint ) const;

  /**
   * Returns a pointer to the tabulated property that holds the reflectivity
   * of the mirror.
   *
   * @return Pointer to mirror reflectivity
   */
  inline const Rich::TabulatedProperty1D* reflectivity() const noexcept { return m_reflectivity.get(); }

  /**
   * Retrieves the normal vector at the centre of the mirror
   * @return Normal vector at the mirror centre
   */
  inline const Gaudi::XYZVector& centreNormal() const noexcept { return m_centreNormal; }

  /**
   * Retrieves the plane defined by the centre normal and the centre
   * of the mirror
   * @return Plane perpendicular to the normal vector
   */
  inline const Gaudi::Plane3D& centreNormalPlane() const noexcept { return m_centreNormalPlane; }

private: // methods
  /**
   * Update the geometrical parameters.
   * @return SUCCESS if everything OK
   */
  StatusCode updateGeometry();

public:
  /// Basic cache of mirror data needed by SIMD reconstruction as required type
  template <typename TYPE>
  struct MirrorData {
    /// radius of curvature
    TYPE radius{0};
    /// Plane parameters
    TYPE planeA{0}, planeB{0}, planeC{0}, planeD{0};
    /// Centre of curvature
    TYPE cocX{0}, cocY{0}, cocZ{0};
  };

  /// Access the mirror data
  inline decltype( auto ) mirrorData() const noexcept { return m_mirrorData; }

private: // data
  /// floating point mirror data
  MirrorData<Rich::SIMD::DefaultScalarFP> m_mirrorData;

  /// mirror reflectivity
  std::unique_ptr<const Rich::TabulatedProperty1D> m_reflectivity;

  const ISolid* m_solid = nullptr; ///< The mirror solid

  Gaudi::XYZPoint m_centreOfCurvature; ///< The centre of curvature
  Gaudi::XYZPoint m_mirrorCentre;      ///< The mirror centre

  Gaudi::XYZPoint m_localOrigin;       ///< The local centre of curvature
  Gaudi::XYZPoint m_localMirrorCentre; ///< The local mirror centre

  double m_radius{0};        ///< Spherical mirror radius
  int    m_mirrorNumber{-1}; ///< mirror (segment) number

  /// The normal vector at the centre of the mirror
  Gaudi::XYZVector m_centreNormal;

  /// The plane normal to the normal vector at the centre of the mirror
  Gaudi::Plane3D m_centreNormalPlane;

  /// Flag for first update
  bool m_firstUpdate{true};
};
