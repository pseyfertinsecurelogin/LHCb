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
#ifndef LIB_GEOMETRYINFOPLUS_H
#define LIB_GEOMETRYINFOPLUS_H 1

// Include files
/** STD and STL includes */
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <string>
/* GaudiKernel includes */
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/StatusCode.h"
/** DetDesc includes */
#include "DetDesc/IDetectorElement.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ILVolume.h"
#include "DetDesc/IPVolume.h"
#include "DetDesc/Services.h"
#include "GeometryInfoException.h"

/** forward declarations */
class GaudiException;
class IDataProviderSvc;
class ISvcLocator;
class IUpdateManagerSvc;
class AlignmentCondition;

#define VERBO                                                                                                          \
  if ( isVerbose() ) verbose()

/** @class GeometryInfoPlus GeometryInfoPlus.h Lib/GeometryInfoPlus.h
 *
 *  Based on "most trivial" implementation of Vanya Belyaev and
 *  Sebastien Ponce.
 *
 *
 *  @author Vanya Belyaev
 *  @author Sebastien Ponce
 *  @author Juan PALACIOS
 *  @date   2005-04-28
 */

class GeometryInfoPlus : public IGeometryInfo {

  ///  friend class
  friend class DetectorElement;

public:
  /// type of vector of children's names
  typedef std::vector<std::string> ChildName;

  typedef std::vector<Gaudi::Transform3D>::const_iterator matrix_iterator;
  typedef std::vector<IGeometryInfo*>                     IGIChildren;
  typedef IGeometryInfo::IGIChildrens::iterator           iGInfo_iterator;

  /** constructor, creates "ghost"
   *  @param de pointer to detector element
   */
  GeometryInfoPlus( IDetectorElement* de );

  /** constructor, creates "orphan"
   *  @param de     pointer to detector element
   *  @param LogVol name of logical volume
   */
  GeometryInfoPlus( IDetectorElement* de, const std::string& LogVol );

  /** constructor, create regular element
   *  @param de     pointer to detector element
   *  @param LogVol name of logical volume
   *  @param Support name of support element
   *  @param ReplicaPath replica path/address
   *  @param alignmentPath alignment path/address
   */
  GeometryInfoPlus( IDetectorElement* de, const std::string& LogVol, const std::string& Support,
                    const ILVolume::ReplicaPath& ReplicaPath, const std::string& alignmentPath = "" );

  /** constructor, create regular element
   *  @param de              pointer to detector element
   *  @param LogVol          name of logical volume
   *  @param Support         name of support element
   *  @param ReplicaNamePath replica path/address
   *  @param alignmentPath   alignment path/address
   */
  GeometryInfoPlus( IDetectorElement* de, const std::string& LogVol, const std::string& Support,
                    const std::string& ReplicaNamePath, const std::string& alignmentPath = "" );

  virtual ~GeometryInfoPlus(); ///< Destructor

public:
  StatusCode initialize();

  StatusCode cache() override;

  StatusCode updateChildren() override;

  bool hasLVolume() const override;

  bool hasSupport() const override;

  const AlignmentCondition* alignmentCondition() const override;

  const Gaudi::Transform3D& toLocalMatrix() const override;

  const Gaudi::Transform3D& toGlobalMatrix() const override;

  const Gaudi::Transform3D& toLocalMatrixNominal() const override;

  const Gaudi::Transform3D& toGlobalMatrixNominal() const override;

  const Gaudi::Transform3D& ownToLocalMatrixNominal() const override;

  const Gaudi::Transform3D& ownToNominalMatrix() const override;

  Gaudi::Transform3D ownToOffNominalMatrix() const override;

  Gaudi::Transform3D ownMatrix() const override;

  StatusCode ownToOffNominalMatrix( const Gaudi::Transform3D& ) override;

  StatusCode ownToOffNominalParams( const std::vector<double>& trans, const std::vector<double>& rot,
                                    const std::vector<double>& pivot = std::vector<double>( 3 ) ) override;

  Gaudi::XYZPoint toLocal( const Gaudi::XYZPoint& globalPoint ) const override;

  Gaudi::XYZPoint toGlobal( const Gaudi::XYZPoint& localPoint ) const override;

  Gaudi::XYZVector toLocal( const Gaudi::XYZVector& globalDirection ) const override;

  Gaudi::XYZVector toGlobal( const Gaudi::XYZVector& localDirection ) const override;

  bool isInside( const Gaudi::XYZPoint& globalPoint ) const override;

  ///  return the name of the daughter element to which
  /// the given point belongs to
  std::string belongsToPath( const Gaudi::XYZPoint& globalPoint ) override;

  /** return the pointer to the daughter element to
   *  which the given point belongs to
   */
  IGeometryInfo* belongsTo( const Gaudi::XYZPoint& globalPoint ) override;

  /** return the name of the daughter element to which
   *  the given point belongs to (taking into account the level)
   */
  std::string belongsToPath( const Gaudi::XYZPoint& globalPoint, const int level ) override;

  /** return the pointer to the daughter element to
   *  which the given point belongs to  (taking into account the level)
   */
  IGeometryInfo* belongsTo( const Gaudi::XYZPoint& globalPoint, const int level ) override;

  /** return the full geometry info for a given point in the
   * global reference system
   */
  StatusCode fullGeoInfoForPoint( const Gaudi::XYZPoint& point, const int level, IGeometryInfo*& start,
                                  ILVolume::PVolumePath& volumePath ) override;

  /**  return the full geometry info for a given point in the
   *   global reference system
   */
  StatusCode fullGeoInfoForPoint( const Gaudi::XYZPoint& point, const int level, IGeometryInfo*& start,
                                  ILVolume::ReplicaPath& volumePath ) override;

  /** return the full geometry info for a given point in the
   *  global reference system
   */
  StatusCode fullGeoInfoForPoint( const Gaudi::XYZPoint& point, const int level, std::string& start,
                                  ILVolume::PVolumePath& volumePath ) override;

  /** return the full geometry info for a given point in the
   *  global reference system
   */
  StatusCode fullGeoInfoForPoint( const Gaudi::XYZPoint& point, const int level, std::string& start,
                                  ILVolume::ReplicaPath& volumePath ) override;

  /// return the name of the assosiated logical volume
  inline const std::string& lvolumeName() const override { return m_gi_lvolumeName; }

  /// return the pointer4 to the assosiated logical volume
  const ILVolume* lvolume() const override;

  /// return the location (address) of this geometry object
  StatusCode location( IGeometryInfo*& start, ILVolume::ReplicaPath& replicaPath ) const override;

  /// return the location (address) of this geometry object
  StatusCode location( std::string& start, ILVolume::ReplicaPath& replicaPath ) const override;

  ///  return the IGeometryInfo supporting this one.
  IGeometryInfo* supportIGeometryInfo() const override;

  /// return the name of the logical volume with a given address
  std::string lvolumePath( const std::string& start, const ILVolume::ReplicaPath& replicaPath ) override;

  /// return the pointer to  the logical volume with a given address
  const ILVolume* lvolume( const std::string& start, const ILVolume::ReplicaPath& replicaPath ) override;

  /// return the name of the logical volume with a given address
  std::string lvolumePath( IGeometryInfo* start, const ILVolume::ReplicaPath& replicaPath ) override;

  /// return the pointer to  the logical volume with a given address
  const ILVolume* lvolume( IGeometryInfo* start, const ILVolume::ReplicaPath& replicaPath ) override;

  /// retrive reference to replica path (mistrerious "rpath" or "npath")
  const ILVolume::ReplicaPath& supportPath() const override;

  /// pointer to the parent IGeometryInfo
  IGeometryInfo* parentIGeometryInfo() override;

  /// pointer to the parent IGeometryInfo (const version)
  const IGeometryInfo* parentIGeometryInfo() const override;

  /// (reference to) container of children IGeometryInfo
  inline IGeometryInfo::IGIChildrens& childIGeometryInfos() override {
    if ( UNLIKELY( !m_gi_childLoaded ) ) loadChildren();
    return m_gi_childrens;
  }
  /// (reference to) container of children IGeometryInfo
  inline const IGeometryInfo::IGIChildrens& childIGeometryInfos() const override {
    if ( UNLIKELY( !m_gi_childLoaded ) ) loadChildren();
    return m_gi_childrens;
  }
  /// iterators for manipulation of daughter elements
  /// begin iterator
  inline IGeometryInfo::IGIChildrens::const_iterator childBegin() const override {
    // if( !m_gi_childLoaded )
    // {
    //   try{ loadChildren(); }
    //   catch(...) { return m_gi_childrens.end(); }
    // }
    // return m_gi_childrens.begin();
    return childIGeometryInfos().begin();
  }

  /// begin iterator (const version)
  inline IGeometryInfo::IGIChildrens::iterator childBegin() override {
    // if( !m_gi_childLoaded )
    // {  try{ loadChildren(); } catch(...){ return m_gi_childrens.end(); }  }
    // return m_gi_childrens.begin();
    return childIGeometryInfos().begin();
  }

  /// end  iterator
  inline IGeometryInfo::IGIChildrens::iterator childEnd() override {
    // if( !m_gi_childLoaded )
    // {  try{ loadChildren(); } catch(...){ return m_gi_childrens.end(); }  }
    // return m_gi_childrens.end();
    return childIGeometryInfos().end();
  }

  /// end  iterator (const version)
  inline IGeometryInfo::IGIChildrens::const_iterator childEnd() const override {
    // if( !m_gi_childLoaded )
    //{  try{ loadChildren(); } catch(...){ return m_gi_childrens.end(); }  }
    // return m_gi_childrens.end();
    return childIGeometryInfos().end();
  }

  /// overloades printout to  std::ostream
  std::ostream& printOut( std::ostream& ) const override;
  /// overloaded printout to the MsgStream
  MsgStream& printOut( MsgStream& ) const override;
  /// reset to the initial state
  IGeometryInfo* reset() override;

  /** query the interface
   *  @param ID interface identifier
   *  @param ppI placeholder for returned interface
   *  @return status code
   */
  StatusCode queryInterface( const InterfaceID& ID, void** ppI ) override;

  /// add reference
  unsigned long addRef() override { return 0; }

  /// release
  unsigned long release() override { return 0; }

private:
  inline IMessageSvc*       msgSvc() const { return m_services->msgSvc(); }
  inline IDataProviderSvc*  dataSvc() const { return m_services->detSvc(); }
  inline ISvcLocator*       svcLocator() const { return m_services->svcLocator(); }
  inline IUpdateManagerSvc* updMgrSvc( bool create = false ) const { return m_services->updMgrSvc( create ); }

  inline bool isInsideDaughter( const Gaudi::XYZPoint& globalPoint ) const {
    if ( !childLoaded() || loadChildren().isFailure() ) return false;
    return std::any_of( childBegin(), childEnd(), IGeometryInfo_isInside( globalPoint ) );
  }

  inline bool childLoaded() const { return m_gi_childLoaded; }

  /// Register the parent IGeometryInfo. This should trigger a re-cahing
  /// if a parent changes its validity.
  StatusCode registerSupportGI();

  StatusCode calculateMatrices();

  StatusCode combineMatrices( matrix_iterator deltaFirst, matrix_iterator deltaEnd, matrix_iterator pvFirst,
                              matrix_iterator pvEnd );

  /// Calculate the combined ideal and delta matrix connecting
  /// this IGeometryInfo to the global frame.
  /// Set the corresponding inverse matrix.
  StatusCode calculateFullMatrices( matrix_iterator deltaFirst, matrix_iterator deltaEnd, matrix_iterator pvFirst );

  /// Calculate the total ideal matrix connecting
  /// this IGeometryInfo to the global frame.
  /// Set the corresponding inverse matrix.
  void calculateIdealMatrix( matrix_iterator pvFirst, matrix_iterator pvEnd );

  /// Force the local delta matrix to be newDelta and re-do all
  /// necessary matrix calculations
  StatusCode setLocalOffNominalDeltaMatrix( const Gaudi::Transform3D& newDelta );

  StatusCode getAlignmentCondition();

  StatusCode registerCondition();

  void clearMatrices();

  inline bool needsAlignmentCondition() const { return m_hasAlignmentPath; }

  inline void needsAlignmentCondition( const bool& needsAlignment ) { m_hasAlignmentPath = needsAlignment; }

  inline bool hasAlignmentCondition() const { return m_hasAlignment; }

  inline void hasAlignmentCondition( const bool& hasAlignment ) { m_hasAlignment = hasAlignment; }

  inline AlignmentCondition* myAlignmentCondition() const { return m_alignmentCondition; }

  IDetectorElement* parentIDetectorElement( IDetectorElement* iDetElem ) {
    return ( iDetElem ? iDetElem->parentIDetectorElement() : nullptr );
  }

  IGeometryInfo* geoByName( const std::string& name ) const;

  ILVolume* findLogical() const;
  /// could throw GeometryInfoException
  StatusCode loadChildren() const;

  inline IDetectorElement* detElem() const { return m_gi_iDetectorElement; }

  /// Assertion JUAN: beware: dummies do nothing!
  inline void Assert( bool assertion, const std::string& name ) const {
    if ( !assertion ) { throw GeometryInfoException( name, this ); }
  }

  /// Assertion
  inline void Assert( bool assertion, const std::string& name, const GaudiException& ge ) const {
    if ( !assertion ) { throw GeometryInfoException( name, ge, this ); }
  }

  Gaudi::Transform3D accumulateMatrices( const ILVolume::PVolumePath& volumePath ) const;

  bool idealMatrixLoaded() { return bool( m_idealMatrix ); }

  inline matrix_iterator idealBegin() const { return m_pvMatrices.begin(); }

  inline matrix_iterator idealEnd() const { return m_pvMatrices.end(); }

  inline matrix_iterator deltaBegin() const { return m_deltaMatrices.begin(); }

  inline matrix_iterator deltaEnd() const { return m_deltaMatrices.end(); }

private:
  inline MsgStream& log() const { return *m_log; }

  inline bool isVerbose() const { return log().level() <= MSG::VERBOSE; }

  inline MsgStream& verbose() const { return log() << MSG::VERBOSE; }

private:
  std::unique_ptr<MsgStream> m_log;

  /// flag for logical volume association
  bool m_gi_has_logical = true;
  /** name of logical volume ( full path (address)
   *  in the Transient Store )
   */
  std::string m_gi_lvolumeName;
  /// pointer to the logical volume (loaded on demand only!)
  mutable ILVolume* m_gi_lvolume = nullptr;

  /// Has alignment
  bool m_hasAlignment = false;

  /// Path to alignment
  std::string m_alignmentPath;

  /// Has alignment path
  bool m_hasAlignmentPath = false;

  AlignmentCondition* m_alignmentCondition = nullptr;

  /// Transformation from the  global reference system
  /// to the local reference system.
  /// Total matrix, including ideal alignment plus deltas.
  std::optional<Gaudi::Transform3D> m_matrix;

  /// Transformation from the  global reference system
  /// to the local reference system.
  /// Ideal geometry with no misalignments.
  mutable std::optional<Gaudi::Transform3D> m_idealMatrix;

  mutable std::optional<Gaudi::Transform3D> m_localIdealMatrix;
  mutable std::optional<Gaudi::Transform3D> m_localDeltaMatrix;

  /** transformation FROM local reference system  to the global
   *  reference system
   */

  /** transformation FROM local reference system  to the global
   *  reference system
   */

  mutable std::optional<Gaudi::Transform3D> m_matrixInv;
  mutable std::optional<Gaudi::Transform3D> m_idealMatrixInv;

  /// flag for support association
  bool m_gi_has_support = false;
  /**  name of DetectorElement (full path(address) in the
   *   Transient Store) , which supports the addres)
   */
  std::string m_gi_supportName;
  /// pointer to element, which supports the address (loaded on demand)
  mutable IGeometryInfo* m_gi_support = nullptr;
  /// the address itself (numeric replic apath)
  mutable ILVolume::ReplicaPath m_gi_supportPath;
  /// another form of address  (name replica path)
  std::string m_gi_supportNamePath;
  //
  ///  The corresponding IDtectorElement object
  mutable IDetectorElement* m_gi_iDetectorElement;

  /// flag for  parent object
  mutable bool m_gi_parentLoaded = false;
  /// pointer to parent object (resolved on demand only)
  mutable IGeometryInfo* m_gi_parent = nullptr;
  /// flag for children objects
  mutable bool m_gi_childLoaded = false;
  /**  container of pointers to children objects
   *  (resolved on demand only)
   */
  mutable IGeometryInfo::IGIChildrens m_gi_childrens;
  /** container of names of children objects
   * (resolved on demand only)
   */
  mutable GeometryInfoPlus::ChildName m_gi_childrensNames;

  /// reference to services
  DetDesc::ServicesPtr m_services;

  /// Keep for now the local and all the parents ideal and delta
  /// transformation matrices.
  /// The code is written such that these could become automatic,
  /// temporary objects in one algorithm. Juan.
  std::vector<Gaudi::Transform3D> m_pvMatrices;
  std::vector<Gaudi::Transform3D> m_deltaMatrices;
};

#endif // LIB_GEOMETRYINFOPLUS_H
