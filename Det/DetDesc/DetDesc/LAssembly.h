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
#ifndef DETDESC_LASSEMBLY_H
#define DETDESC_LASSEMBLY_H 1
// Include files
// from DetDesc
#include "DetDesc/CLIDLAssembly.h"
#include "DetDesc/DetDesc.h"
#include "DetDesc/IBoxCover.h"
#include "DetDesc/LogVolBase.h"

template <class TYPE>
class DataObjectFactory;

static const std::string EmptyString( "" );

/** @class LAssembly LAssembly.h DetDesc/LAssembly.h
 *
 *  The implementation of "Logical Assembly" notion.
 *  It is a logicla voluem without shape and material.
 *  It is just a placeholder for its child volumes.
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   18/11/2001
 */

class LAssembly : public virtual IBoxCover, public LogVolBase {
  /// friend factory for instantiation
  friend class DataObjectFactory<LAssembly>;

public:
  /** constructor
   *  @exception LVolumeException wrong paramaters value
   *  @param name name of logical volume
   *  @param sensitivity  name of sensitive detector object (for simulation)
   *  @param magnetic  name of magnetic field object (for simulation)
   */
  LAssembly( const std::string& name = "", const std::string& sensitivity = "", const std::string& magnetic = "" )
      : LogVolBase( name, sensitivity, magnetic ) {}

public:
  /** is this volume "Assembly" of other volumes?
   *  @see ILVolume
   *  notion of Assembly Volume is imported from Geant4.
   *  "Assembly Volume" has no associated material and shape,
   *  thus material name shoudl me empty, pointer to solid
   *  and pointer to material should be both nulls
   *  @return true if volume is Assembly
   */
  inline bool isAssembly() const override { return true; }

  /** the solid, associated with the Logical Volume
   *  @see ILVolume
   *  @return the solid, associated with the Logical Volume
   */
  inline const ISolid* solid() const override { return nullptr; }

  /** the material, associated with the Logical Volume
   *  For Assembly Volumes material pointes to NULL!
   *  @see ILVolume
   *  @exception LVolumeException no material
   *  @return the material, associated with the Logical Volume
   */
  inline const Material* material() const override { return nullptr; }

  /** the material(by name), associated with the Logical Volume
   *  @see ILVolume
   *  @return the material(by name), associated with the Logical Volume
   */
  inline const std::string& materialName() const override { return EmptyString; }

  /** check for the given 3D-point. Point coordinates are in the
   *  local reference frame of the logical volume
   *  @see ILVolume
   *  @param LocalPoint point (in local reference system of the solid)
   *  @return true if the point is inside the solid
   */
  inline bool isInside( const Gaudi::XYZPoint& LocalPoint ) const override { return isInsideDaughter( LocalPoint ); }

  /** calculate the daughter path containing the Point in Local frame ,
   *  can be VERY slow for complex geometry,
   *  therefore use the appropriate Level for usage
   *  @see ILVolume
   *  @param  LocalPoint point in local reference system of logical volume
   *  @param  Level depth level
   *  @param  pVolumePath  vector of physical volumes
   *  @return status code
   */
  StatusCode belongsTo( const Gaudi::XYZPoint& LocalPoint, const int Level,
                        ILVolume::PVolumePath& pVolumePath ) const override;

  /** calculate the daughter path containing the Point in Local frame ,
   *  can be VERY slow for complex geometry,
   *  therefore use the appropriate Level for usage
   *  @see ILVolume
   *  @param  LocalPoint point in local reference system of logical volume
   *  @param  Level depth level
   *  @param  replicaPath  vector of physical volumes
   *  @return status code
   */
  StatusCode belongsTo( const Gaudi::XYZPoint& LocalPoint, const int Level,
                        ILVolume::ReplicaPath& replicaPath ) const override;

  /** intersection of the logical volume with with the line \n
   *  The line is parametrized in the local reference system
   *  of the logical volume by initial Point and direction Vector \n
   *  @f$ \vec{x}(t) = \vec{p} + t\times \vec{v} @f$  \n
   *
   *   Method returns the number of intersection points("ticks") and
   *   the container of pairs - ticks and pointer to the corresponding
   *   material. The simplification is determined by value of threshold
   *   (in units of radiation length)
   *   Method throws LVolumeException in the case, then
   *   solid is not defined or material is not accessible.
   *
   *  @see ILVolume
   *  @exception LogVolumeException solid or/and matherial problems
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param intersections output container
   *  @param threshold threshold value
   *  @return number of intersections
   */
  unsigned int intersectLine( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                              ILVolume::Intersections& intersections, const double threshold ) const override;

  /** intersection of the logical volume with with the line \n
   *  Theine is parametrized in the local reference system
   *  of the logical volume by initial Point and direction Vector \n
   *  @f$ \vec{x}(t) = \vec{p} + t\times \vec{v} @f$  \n
   *
   *   Method returns the number of intersection points("ticks") and
   *   the container of pairs - ticks and pointer to the corresponding
   *   material. The simplification is determined by value of threshold
   *   (in units of radiation length)
   *   Method throws LVolumeException in the case, then
   *   solid is not defined or material is not accessible.
   *
   *  @see ILVolume
   *  @exception LogVolumeException solid or/and matherial problems
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param intersections output container
   *  @param tickMin minimum value of possible Tick
   *  @param tickMax maximum value of possible Tick
   *  @param Threshold threshold value
   *  @return number of intersections
   */
  unsigned int intersectLine( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                              ILVolume::Intersections& intersections, const ISolid::Tick tickMin,
                              const ISolid::Tick tickMax, const double Threshold ) const override;

  /** class/object identifier (static method)
   *  @return unique class identifier
   */
  static const CLID& classID();

  /** class/object identification (virtual method)
   *  @return unique class identifier
   */
  const CLID& clID() const override;

  /** printout to STD/STL stream
   *  @see ILVolume
   *  @param os STD/STL stream
   *  @return reference to the stream
   */
  std::ostream& printOut( std::ostream& os = std::cout ) const override;

  /** printout to Gaudi MsgStream stream
   *  @see ILVolume
   *  @param os Gaudi MsgStream  stream
   *  @return reference to the stream
   */
  MsgStream& printOut( MsgStream& os ) const override;

protected:
  /// updates box cover integrating the new BoxCover
  void updateCover( const IPVolume& box ) override;
};

// ============================================================================
// The End
// ============================================================================
#endif // DETDESC_LASSEMBLY_H
