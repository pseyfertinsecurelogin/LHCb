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
/// STD and STL includes
#include <algorithm>
#include <memory>
/// GaudiKernel includes
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IValidity.h"
#include "GaudiKernel/Transform3DTypes.h"
/// DetDesc  includes
#include "DetDesc/DetDesc.h"
#include "DetDesc/IPVolume.h"
#include "DetDesc/IPVolume_predicates.h"
#include "DetDesc/ISolid.h"
#include "DetDesc/InstanceCounter.h"
#include "DetDesc/LogVolumeException.h"
#include "DetDesc/Services.h"
#include "DetDesc/Surface.h"
#include "DetDesc/ValidDataObject.h"

/// forward declarations
class IDataProviderSvc;
class IMessageSvc;

/** @class LogVolBase LogVolBase.h DetDesc/LogVolBase.h
 *
 *  The common base class for ILVolume implementations:
 *    - LVolume
 *    - LAssembly
 *
 *  @author  Vanya Belyaev Ivan.Belyaev
 *  @author  Sebastien Ponce
 *  @author  Marco Clemencic
 */

class LogVolBase : public virtual ILVolume, public ValidDataObject, private Details::InstanceCounter<LogVolBase> {

public:
  /** constructor
   *  @exception LVolumeException wrong paramaters value
   *  @param name name of logical volume
   *  @param sensitivity  name of sensitive detector object (for simulation)
   *  @param magnetic  nam eof magnetic field object (for simulation)
   */
  LogVolBase( const std::string& name = "", const std::string& sensitivity = "", const std::string& magnetic = "" );

public:
  /// destructor
  ~LogVolBase() override;

  /** retrieve  the name(identification)  of Logical Volume
   *  @see ILVolume
   *  @return    the name(identification)  of Logical Volume
   */
  inline const std::string& name() const override {
    static const std::string s_empty;
    IRegistry*               pReg = registry();
    return pReg ? pReg->identifier() : s_empty;
  }

  /** vector of physical volumes
   *  @see ILVolume
   *  @return vector of physical volumes
   */
  inline PVolumes& pvolumes() override { return m_pvolumes; }

  /** vector of physical volumes (const version)
   *  @see ILVolume
   *  @return vector of physical volumes
   */
  inline const PVolumes& pvolumes() const override { return m_pvolumes; }

  /** number of Physical(positioned) Volumes
   *  @see ILVolume
   *  @return number of Physical(positioned) Volumes
   */
  inline ILVolume::ReplicaType noPVolumes() const override { return m_pvolumes.size(); }

  /** daughter (Physical Volume) by index
   *  @see ILVolume
   *  @param  index    physical volume index
   *  @return pointer to daughter (Physical Volume)
   */
  const IPVolume* operator[]( const ILVolume::ReplicaType& index ) const override {
    return index < m_pvolumes.size() ? m_pvolumes[index] : nullptr;
  };

  /** daughter (Physical Volume) by name
   *  @see ILVolume
   *  @param  name    physical volume name
   *  @return pointer to daughter (Physical Volume)
   */
  inline const IPVolume* operator[]( std::string_view name ) const override {
    auto pvi = std::find_if( m_pvolumes.begin(), m_pvolumes.end(), IPVolume_byName( name ) );
    return pvi != m_pvolumes.end() ? *pvi : nullptr;
  };

  /** get daughter (Physical Volume) by index
   *  @param  index    physical volume index
   *  @return pointer to daughter (Physical Volume)
   */
  const IPVolume* pvolume( const ILVolume::ReplicaType& index ) const override {
    return index < m_pvolumes.size() ? m_pvolumes[index] : nullptr;
  }

  /** get daughter (Physical Volume) by name
   *  @param  name    physical volume name
   *  @return pointer to daughter (Physical Volume)
   */
  const IPVolume* pvolume( std::string_view name ) const override {
    auto pvi = std::find_if( m_pvolumes.begin(), m_pvolumes.end(), IPVolume_byName( name ) );
    return pvi != m_pvolumes.end() ? *pvi : nullptr;
  }

  /** begin iterator  for manipulation with daughters
   *  @see ILVolume
   *  @return begin iterator  for manipulation with daughters
   */
  inline ILVolume::PVolumes::iterator pvBegin() override { return m_pvolumes.begin(); }

  /** begin iterator  for manipulation with daughters (const version)
   *  @see ILVolume
   *  @return begin iterator  for manipulation with daughters
   */
  inline ILVolume::PVolumes::const_iterator pvBegin() const override { return m_pvolumes.begin(); }

  /** retrieve end iterator for manipulation with daughters
   *  @see ILVolume
   *  @return end iterator  for manipulation with daughters
   */
  inline ILVolume::PVolumes::iterator pvEnd() override { return m_pvolumes.end(); }

  /** retrieve end iterator for manipulation with daughters (const version)
   *  @see ILVolume
   *  @return end iterator  for manipulation with daughters
   */
  inline ILVolume::PVolumes::const_iterator pvEnd() const override { return m_pvolumes.end(); }

  /** traverse the sequence of paths  \n
   *  transform the sequence of replicas to sequence of  physical volumes
   *  @see ILVolume
   *  @param pathBegin    first replica-Path
   *  @param pathEnd      last  replica-Path
   *  @param pVolumePath  vector of physical volumes
   *  @return status code
   */
  StatusCode traverse( ILVolume::ReplicaPath::const_iterator pathBegin, ILVolume::ReplicaPath::const_iterator pathEnd,
                       ILVolume::PVolumePath& pVolumePath ) const override;

  /** traverse the sequence of paths  \n
   *  transform the sequence of replicas to sequence of  physical volumes
   *  @see ILVolume
   *  @param path         replica-Path
   *  @param pVolumePath  vector of physical volumes
   *  @return status code
   */
  inline StatusCode traverse( const ILVolume::ReplicaPath& path, ILVolume::PVolumePath& pVolumePath ) const override {
    return traverse( path.begin(), path.end(), pVolumePath );
  }

  /** name of sensitive "detector" - needed for simulation
   *  @see ILVolume
   *  @return name of sensitive "detector"
   */
  inline const std::string& sdName() const override { return m_sdName; }

  /** magnetic properties  (if needed for simulation)
   *  @see ILVolume
   *  @return name of magnetic field  object
   */
  inline const std::string& mfName() const override { return m_mfName; }

  /** accessors to surfaces
   *  @see ILVolume
   *  @return vector of surfaces
   */
  inline Surfaces& surfaces() override { return m_surfaces; }

  /** accessors to surfaces  (const version)
   *  @see ILVolume
   *  @return vector of surfaces
   */
  inline const Surfaces& surfaces() const override { return m_surfaces; }

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

  /** reset to initial state, clear chaches, etc...
   *  @see ILVolume
   *  @return self reference
   */
  inline ILVolume* reset() override {
    /// reset all physical volumes
    std::for_each( m_pvolumes.begin(), m_pvolumes.end(), []( IPVolume* v ) { v->reset(); } );
    /// return self-reference
    return this;
  }

  /// Clear (delete) all volumes
  inline void clearVolumes() override {
    for ( auto& ipv : m_pvolumes ) { delete ipv; }
    m_pvolumes.clear();
  }

  /** query the interface
   *  @see IInterface
   *  @param ID unique interface identifier
   *  @param ppI placeholder for returned interface
   *  @return status code
   */
  StatusCode queryInterface( const InterfaceID& ID, void** ppI ) override;

  /** add the reference
   *  @see IInterface
   *  @return reference counter
   */
  unsigned long addRef() override;

  /** release the interface
   *  @see IInterface
   *  @return reference counter
   */
  unsigned long release() override;

  /** create daughter physical volume
   *  @param PVname name of daughter volume
   *  @param LVnameForPV name of logical volume for the physical volume
   *  @param position position of logical volume
   *  @param rotation rotation of logical volume
   *  @return pointer to created physical volume
   */
  IPVolume* createPVolume( const std::string& PVname, const std::string& LVnameForPV,
                           const Gaudi::XYZPoint&   pos = Gaudi::XYZPoint(),
                           const Gaudi::Rotation3D& rot = Gaudi::Rotation3D() );

  /** create daughter physical volume
   *  @param PVname      name of daughter volume
   *  @param LVnameForPV name of logical volume for the physical volume
   *  @param Transform   tranformation
   *  @return pointer to created physical volume
   */
  IPVolume* createPVolume( const std::string& PVname, const std::string& LVnameForPV,
                           const Gaudi::Transform3D& Transform );

protected:
  /** create EMPTY daughter physical volume
   *  @return pointer to created physical volume
   */
  IPVolume* createPVolume();

  /// updates box cover integrating the new PVolume
  /// default implementation is empty
  virtual void updateCover( const IPVolume& ){};

  /** Assertion
   *  @param assertion condition
   *  @param name      exception message
   *  @param sc        status code
   */
  inline void Assert( bool assertion, const std::string& name, const StatusCode& sc = StatusCode::FAILURE ) const {
    if ( !assertion ) { throw LogVolumeException( name, this, sc ); }
  }

  /** Assertion
   *  @param assertion condition
   *  @param name      exception message
   *  @param Exception previous exception
   *  @param sc        status code
   */
  inline void Assert( bool assertion, const std::string& name, const GaudiException& Exception,
                      const StatusCode& sc = StatusCode::FAILURE ) const {
    if ( !assertion ) { throw LogVolumeException( name, Exception, this, sc ); }
  }

  /** Auxillary method  to calculate intersections with daughters
   *  @exception LVolumeException wrong parameters or geometry error
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param childIntersections output container
   *  @param tickMin minimum value of possible Tick
   *  @param tickMax maximum value of possible Tick
   *  @param Threshold threshold value
   *  @return number of intersections
   */
  unsigned int intersectDaughters( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                                   ILVolume::Intersections& childIntersections, const ISolid::Tick& tickMin,
                                   const ISolid::Tick& tickMax, const double Threshold ) const;

  /** Auxillary method  to calculate intersection with daughters
   *  @exception LVolumeException wrong parameters or geometry error
   *  @param Point initial point at the line
   *  @param Vector direction vector of the line
   *  @param childIntersections output container
   *  @param Threshold threshold value
   *  @return number of intersections
   */
  unsigned int intersectDaughters( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                                   ILVolume::Intersections& childIntersections, const double Threshold ) const;

  /** check for the given 3D-point inside daughter volume
   *  Point coordinates are in the local reference
   *  frame of the solid.
   *  @param LocalPoint point (in local reference system of the solid)
   *  @return true if the point is inside the daughter volume
   */
  bool isInsideDaughter( const Gaudi::XYZPoint& LocalPoint ) const {
    return insideDaughter( LocalPoint ).first != nullptr;
  }

  /** check for the given 3D-point inside daughter volume
   *  Point coordinates are in the local reference
   *  frame of the solid.
   *  @param LocalPoint point (in local reference system of the solid)
   *  @return pointer to the daughter volume
   */
  std::pair<const IPVolume*, int> insideDaughter( const Gaudi::XYZPoint& LocalPoint ) const {
    auto itp = std::find_if( m_pvolumes.begin(), m_pvolumes.end(), IPVolume_isInside( LocalPoint ) );
    return itp != m_pvolumes.end() ? std::pair{*itp, std::distance( m_pvolumes.begin(), itp )} : std::pair{nullptr, -1};
  }

protected:
  /**
   *  accessor to data service used for retriving of the material
   *  @return pointer to data service
   */
  IDataProviderSvc* dataSvc() const;

  /**
   *  accessor to massage service
   *  @return pointer to message service
   */
  IMessageSvc* msgSvc() const;

  /// copy constructor is private!
  LogVolBase( const LogVolBase& lvb ) = delete;
  /// assignment operator is private!
  LogVolBase& operator=( const LogVolBase& lvb ) = delete;

private:
  /// list of daughter physical volumes
  PVolumes m_pvolumes;
  /// list attached surfaces
  Surfaces m_surfaces;
  /// name of sensitive detector object
  std::string m_sdName;
  /// name of magnetic field source
  std::string m_mfName;
  /// reference to services
  DetDesc::ServicesPtr m_services = DetDesc::services();
};

// ============================================================================
// The End
// ============================================================================
