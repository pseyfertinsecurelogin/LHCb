/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef    DETDESC_PVOLUME_H
#define    DETDESC_PVOLUME_H 1

//#include <atomic>
///  GaudiKernel includes
#include "GaudiKernel/Transform3DTypes.h"
/// DetDesc includes
#include "DetDesc/Services.h"
#include "DetDesc/IPVolume.h"
#include "DetDesc/InstanceCounter.h"

#include <optional>

/// forward declarations
class IDataProviderSvc;
class IMessageSvc;
class MsgStream;
class GaudiException;
class PVolumeException;

/** @class PVolume PVolume.h
 *
 *  simple implementation of IPVolume interface
 *  @see IPVolume
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @author Sebastien Ponce
 */


class PVolume: public IPVolume, private Details::InstanceCounter<PVolume>
{
public:

  /** constructor
   *  @param PhysVol_name name of phys volume
   *  @param LogVol_name name of log volume
   *  @param Position position of physical volume inside mother volume
   *  @param Rotation  rotation of physical volume with respect to mother
   */
  PVolume
  ( const std::string& PhysVol_name                  ,
    const std::string& LogVol_name                   ,
    //    const size_t       copynumber                    ,
    const Gaudi::XYZPoint&   Position     = Gaudi::XYZPoint  () ,
    const Gaudi::Rotation3D& Rotation     = Gaudi::Rotation3D () );

  /** constructor
   *  @param PhysVol_name name of phys volume
   *  @param LogVol_name name of log volume
   *  @param Transform transformation matrix
   */
  PVolume
  ( const std::string&     PhysVol_name ,
    const std::string&     LogVol_name  ,
    //    const size_t           copynumber   ,
    const Gaudi::Transform3D&  Transform    );

  /** retrieve name of the physical volume
   *  (unique within mother logical volume)
   *  @return name of physical volume
   */
  const std::string& name       () const override { return m_name ; }

  /**  retrieve the name of associated Logical Volume
   *  @return name of associated Logical Volume
   */
  const std::string& lvolumeName() const override { return m_lvname ; }

  /**  retrieve  the C++ pointer to Logical Volume
   *  @return pointer to Logical Volume
   */
  const ILVolume* lvolume () const override;

  /** get the tranformation matrix
   *  @return reference to transformation matrix
   */
  const Gaudi::Transform3D&  matrix      () const override { return m_matrix ; }

  /** get the inverse transformation matrix
   *  @return reference to inverse transformationmatrix
   */
  const Gaudi::Transform3D&  matrixInv  () const override;

  /** transform point from  Mother Reference System  to the Local one
   *  @param PointInMother point in Mother Reference System
   *  @return point in local reference system
   */
  Gaudi::XYZPoint toLocal  ( const Gaudi::XYZPoint& PointInMother ) const override;

  /** transform point in Local Reference System to the Mother Reference System
   *  @param PointInLocal point in Local Referency System
   *  @return point in mother reference system
   */
  Gaudi::XYZPoint toMother ( const Gaudi::XYZPoint& PointInLocal  ) const  override;

  /** check for 3D-point
   *  @param PointInMother pointin Mother Referency System
   *  @return true if point is inside physical volume
   */
  bool isInside       ( const Gaudi::XYZPoint& PointInMother ) const  override;

  /** printout to STD/STL stream
   *  @param os reference to STD/STL stream
   *  @return reference to STD/STL stream
   */
  std::ostream& printOut
  ( std::ostream& os = std::cout ) const override;

  /** printout to Gaudi stream
   *  @param os reference to Gaudi stream
   *  @return reference to Gaudi stream
   */
  MsgStream&    printOut
  ( MsgStream&    os             ) const override;

  /** reset to the initial state
   *  @return self-reference
   */
  IPVolume* reset ()  override;

  /** Intersection of the physical volume with with line.
   *  The line is parametrized in the local reference system of the mother
   *  logical volume ("Mother Reference System")
   *  with initial Point and direction Vector:
   *   - @f$ \vec{x}(t) = \vec{p} + t \times \vec{v} @f$ @n
   *
   * Method returns the number of intersection points("ticks") and
   * the container of pairs - ticks and pointer to the corresponding
   * material. @n
   * The simplification is determined by value of threshold
   * (in units of radiation length)
   *
   *  @see ILVolume
   *  @see ISolid
   *
   *  @exception PVolumeException wrong environment
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param intersections output container
   *  @param threshold threshold value
   */
  unsigned int intersectLine
  ( const Gaudi::XYZPoint        & Point         ,
    const Gaudi::XYZVector       & Vector        ,
    ILVolume::Intersections & intersections ,
    const double              threshold     ) const override;

  /** Intersection of the physical volume with with line.
   *  The line is parametrized in the local reference system of the mother
   *  logical volume ("Mother Reference System")
   *  with initial Point and direction Vector:
   *   - @f$ \vec{x}(t) = \vec{p} + t \times \vec{v} @f$ @n
   *
   * Method returns the number of intersection points("ticks") and
   * the container of pairs - ticks and pointer to the corresponding
   * material. @n
   * The simplification is determined by value of threshold
   * (in units of radiation length)
   *
   *  @see ILVolume
   *  @see ISolid
   *
   *  @exception PVolumeException wrong environment
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param intersections output container
   *  @param tickMin minimum value of possible Tick
   *  @param tickMax maximum value of possible Tick
   *  @param threshold threshold value
   */
  unsigned int intersectLine
  ( const Gaudi::XYZPoint        & Point ,
    const Gaudi::XYZVector       & Vector        ,
    ILVolume::Intersections & intersections ,
    const ISolid::Tick        tickMin       ,
    const ISolid::Tick        tickMax       ,
    const double              threshold     ) const override;


  /** apply the  misalignemnt to the transformation matrix
   *  @param ma misalignment matrix (assumed to be small!!!)
   *  @return the resulting transformation matrix
   */
  const Gaudi::Transform3D&
  applyMisAlignment ( const Gaudi::Transform3D& ma ) override;

  /** reset the  misalignemnt
   *  @return the "nominal" transformation matrix
   */
  const Gaudi::Transform3D&
  resetMisAlignment (                          ) override;

  /** query the interface
   *  @param ID unique interface identifier
   *  @param ppI placeholder for returned interface
   *  @return status code
   */
  StatusCode
  queryInterface( const InterfaceID& ID , void** ppI ) override;

  /** add the reference
   *  @return reference counter
   */
  unsigned long addRef  () override;

  /** release the interface
   *  @return reference counter
   */
  unsigned long release () override;

protected:

  /** find logical volume by name */
  void findLogical();

  /** Assertion
   *  @exception PVolumeException for wrong condition
   *  @param assertion condition
   *  @param name      exception message
   */
  void Assert
  ( bool               assertion ,
    const std::string& name      ) const;


  /** Assertion
   *  @exception PVolumeException for wrong condition
   *  @param assertion condition
   *  @param name       exception message
   *  @param Exception  previous exception
   */
  void Assert
  ( bool                  assertion ,
    const std::string&    name      ,
    const GaudiException& Exception ) const;

protected:

  /**
   * accessor to the data service
   * @return pointer to data service
   */
  IDataProviderSvc* dataSvc() const;


private:

  // name of physical volume
  std::string             m_name          ;
  // name of logical volume
  std::string             m_lvname        ;
  // nominal transformationmatrix
  Gaudi::Transform3D          m_nominal       ;
  // transformation matrix
  Gaudi::Transform3D          m_matrix;
  // pointer to inverse transformation matrix
  std::optional<Gaudi::Transform3D> m_imatrix;
  // pointer to logical volume
  ILVolume* m_lvolume = nullptr;
  // reference to dataSvc
  DetDesc::ServicesPtr    m_services;
};

/// ===========================================================================
#endif  ///< DETDESC_PVOLUME_H
/// ===========================================================================
