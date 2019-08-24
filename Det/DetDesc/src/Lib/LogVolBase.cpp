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
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/System.h"
// =============================================================================
// DetDesc
// ============================================================================
#include "DetDesc/LogVolBase.h"
#include "DetDesc/PVolume.h"
#include "DetDesc/VolumeIntersectionIntervals.h"
// ============================================================================
/** @file LogVolBase.cpp
 *
 *  Implementation file for class : LogVolBase
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @author Sebastien Ponce
 *  @author Marco Clemencic
 */
// ============================================================================
/*  constructor
 *  @exception LVolumeException wrong paramaters value
 *  @param name name of logical volume
 *  @param sensitivity  name of sensitive detector object (for simulation)
 *  @param magnetic  nam eof magnetic field object (for simulation)
 */
// ============================================================================
LogVolBase::LogVolBase( const std::string& /*name*/, const std::string& sensitivity, const std::string& magnetic )
    : m_sdName( sensitivity ), m_mfName( magnetic ) {
  // get services
}
// ============================================================================
// destructor
// ============================================================================
LogVolBase::~LogVolBase() { clearVolumes(); }
// ============================================================================
/*
 *  accessor to data service used for retriving of the material
 *  @return pointer to data service
 */
IDataProviderSvc* LogVolBase::dataSvc() const { return m_services->detSvc(); }
// ============================================================================
/*
 *  accessor to message service
 *  @return pointer to message service
 */
IMessageSvc* LogVolBase::msgSvc() const { return m_services->msgSvc(); }
// ============================================================================
/*  add the reference
 *  @return reference counter
 */
// ============================================================================
unsigned long LogVolBase::addRef() { return DataObject::addRef(); }
// ============================================================================
/*  release the interface
 *  @return reference counter
 */
// ============================================================================
unsigned long LogVolBase::release() { return DataObject::release(); }
// ============================================================================
/*  query the interface
 *  @param ID unique interface identifier
 *  @param ppI placeholder for returned interface
 *  @return status code
 */
// ============================================================================
StatusCode LogVolBase::queryInterface( const InterfaceID& ID, void** ppI ) {
  if ( !ppI ) { return StatusCode::FAILURE; }
  *ppI = nullptr;
  if ( ID == ILVolume::interfaceID() ) {
    *ppI = static_cast<ILVolume*>( this );
  } else if ( ID == IInterface::interfaceID() ) {
    *ppI = static_cast<IInterface*>( this );
  } else {
    return StatusCode::FAILURE;
  } ///< RETURN !
  ///
  addRef();
  ///
  return StatusCode::SUCCESS;
}
// ============================================================================
/*  traverse the sequence of paths  \n
 *  transform the sequence of replicas to sequence of  physical volumes
 *  @see ILVolume
 *  @param replicaPath replica-Path
 *  @param volumePath  vector of physical volumes
 *  @return status code
 */
// ============================================================================
StatusCode LogVolBase::traverse( ILVolume::ReplicaPath::const_iterator pathBegin,
                                 ILVolume::ReplicaPath::const_iterator pathEnd,
                                 ILVolume::PVolumePath&                pVolumePath ) const {
  /*   optimized implementation
   *   using functional IPVolume_fromReplica();
   */
  pVolumePath.clear(); ///< clear/reset the output container
  ///< create transformer
  std::transform( pathBegin, pathEnd, std::back_inserter( pVolumePath ), IPVolume_fromReplica( this ) );
  /// errors?
  if ( pVolumePath.end() != std::find( pVolumePath.begin(), pVolumePath.end(), nullptr ) ) {
    pVolumePath.clear();
    return StatusCode::FAILURE;
  }
  ///
  return StatusCode::SUCCESS;
}
// ============================================================================
/*  printout to STD/STL stream
 *  @param os STD/STL stream
 *  @return reference to the stream
 */
// ============================================================================
std::ostream& LogVolBase::printOut( std::ostream& os ) const {
  os << " " << System::typeinfoName( typeid( *this ) ) << " (" << count() << ") "
     << " name = '" << name() << "' ";
  if ( !sdName().empty() ) { os << " sensDet='" << sdName() << "'"; }
  if ( !mfName().empty() ) { os << " magField='" << mfName() << "'"; }
  if ( !surfaces().empty() ) { os << " #surfaces=" << surfaces().size(); }
  ///
  os << " #physvols" << m_pvolumes.size();
  int i = 0;
  for ( const auto& pvol : m_pvolumes ) { os << "#" << i++ << " " << pvol << '\n'; }
  ///
  return os << std::endl;
}
// ============================================================================
/*  printout to Gaudi MsgStream stream
 *  @param os Gausi MsgStreamstream
 *  @return reference to the stream
 */
// ============================================================================
MsgStream& LogVolBase::printOut( MsgStream& os ) const {
  os << " " << System::typeinfoName( typeid( *this ) ) << " (" << count() << ") "
     << " name = '" << name() << "' ";
  if ( !sdName().empty() ) { os << " sensDet='" << sdName() << "'"; }
  if ( !mfName().empty() ) { os << " magField='" << mfName() << "'"; }
  if ( !surfaces().empty() ) { os << " #surfaces=" << surfaces().size(); }
  ///
  os << " #physvols" << m_pvolumes.size();
  int i = 0;
  for ( const auto& pvol : m_pvolumes ) { os << "#" << i++ << " " << pvol << endmsg; }
  ///
  return os << endmsg;
}
// ===========================================================================
/*  create EMPTY daughter physical volume
 *  @return pointer to created physical volume
 */
// ===========================================================================
IPVolume* LogVolBase::createPVolume() { return createPVolume( "", "" ); }
// ===========================================================================

// ===========================================================================
/*  create daughter physical volume
 *  @param PVname name of daughter volume
 *  @param LVnameForPV name of logical volume for the physical volume
 *  @param Position position of logical volume
 *  @param Rotation rotation of logical volume
 *  @return pointer to created physical volume
 */
// ===========================================================================
IPVolume* LogVolBase::createPVolume( const std::string& PVname, const std::string& LVnameForPV,
                                     const Gaudi::XYZPoint& Position, const Gaudi::Rotation3D& Rotation ) {
  std::unique_ptr<PVolume> pv;
  try {
    pv = std::make_unique<PVolume>( PVname, LVnameForPV,
                                    //  copy_number ,
                                    Position, Rotation );
  } catch ( const GaudiException& Exception ) {
    Assert( false, "createPVolume() , exception caught! ", Exception );
  } catch ( ... ) { Assert( false, "createPVolume() , unknown exception!" ); }

  if ( !pv ) {
    throw LogVolumeException( "LVolume::createPVolume, could not create volume " + PVname + "(lv=" + LVnameForPV + ")",
                              this, StatusCode::FAILURE );
  }

  updateCover( *pv );
  m_pvolumes.push_back( pv.release() );
  return m_pvolumes.back();
}
// ============================================================================
/*  create daughter physical volume
 *  @param PVname      name of daughter volume
 *  @param LVnameForPV name of logical volume for the physical volume
 *  @param Transform   tranformation
 *  @return pointer to created physical volume
 */
// ============================================================================
IPVolume* LogVolBase::createPVolume( const std::string& PVname, const std::string& LVnameForPV,
                                     const Gaudi::Transform3D& Transform ) {
  std::unique_ptr<PVolume> pv;
  try {
    pv = std::make_unique<PVolume>( PVname, LVnameForPV,
                                    //                         copy_number ,
                                    Transform );
  } catch ( const GaudiException& Exception ) {
    Assert( false, "createPVolume() , exception caught! ", Exception );
  } catch ( ... ) { Assert( false, "createPVolume() , unknown exception!" ); }

  if ( !pv ) {
    throw LogVolumeException( "LVolume::createPVolume, could not create volume " + PVname + "(lv=" + LVnameForPV + ")",
                              this, StatusCode::FAILURE );
  }
  updateCover( *pv );
  m_pvolumes.push_back( pv.release() );
  return m_pvolumes.back();
}
// ============================================================================
/*  Auxillary method  to calculate intersections with daughters
 *  @exception LVolumeException wrong parameters or geometry error
 *  @param Point initial point at the line
 *  @param Vector direction vector of the line
 *  @param intersections output container
 *  @param tickMin minimum value of possible Tick
 *  @param tickMax maximum value of possible Tick
 *  @param threshold threshold value
 *  @return number of intersections
 */
// ============================================================================
unsigned int LogVolBase::intersectDaughters( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                                             ILVolume::Intersections& childIntersections, const ISolid::Tick& tickMin,
                                             const ISolid::Tick& tickMax, const double Threshold ) const {
  if ( m_pvolumes.empty() ) { return 0; } /// RETURN!!!
  ///
  ILVolume::Intersections child;
  for ( IPVolume* pv : m_pvolumes ) {
    /// construct the intersections container for each daughter volumes
    if ( pv ) {
      pv->intersectLine( Point, Vector, child, tickMin, tickMax, Threshold );
    } else {
      Assert( false, "LVolume::intersect line, IPVolume==NULL for " + name() );
    }

    /// merge individidual containers and clear child container
    std::copy( child.begin(), child.end(), std::back_inserter( childIntersections ) );
    child.clear();
  }
  ///
  std::stable_sort( childIntersections.begin(), childIntersections.end(),
                    VolumeIntersectionIntervals::CompareIntersections() );

  /// V.B.: try to correct
  VolumeIntersectionIntervals::correct( this, childIntersections, Vector.R() );

  return childIntersections.size();
  ///
}
// ============================================================================
/*  Auxillary method  to calculate intersection with daughters
 *  @exception LVolumeException wrong parameters or geometry error
 *  @param Point initial point at the line
 *  @param Vector direction vector of the line
 *  @param intersections output container
 *  @param threshold threshold value
 *  @return number of intersections
 */
// ============================================================================
unsigned int LogVolBase::intersectDaughters( const Gaudi::XYZPoint& Point, const Gaudi::XYZVector& Vector,
                                             ILVolume::Intersections& childIntersections,
                                             const double             Threshold ) const {
  ///
  if ( m_pvolumes.empty() ) { return 0; } /// RETURN!!!
  ///
  ILVolume::Intersections child;
  for ( const auto& pv : m_pvolumes ) {
    /// construct the intersections container for each daughter volumes
    if ( pv ) {
      pv->intersectLine( Point, Vector, child, Threshold );
    } else {
      Assert( false, "LogVolBase::intersectDaug, IPVolume==NULL for " + name() );
    }
    /// merge individidual containers and clear child container
    std::copy( child.begin(), child.end(), std::back_inserter( childIntersections ) );
    child.clear();
  }
  ///
  std::stable_sort( childIntersections.begin(), childIntersections.end(),
                    VolumeIntersectionIntervals::CompareIntersections() );

  /// V.B.: try to correct
  VolumeIntersectionIntervals::correct( this, childIntersections, Vector.R() );

  return childIntersections.size();
  ///
}
// ============================================================================
// The End
// ============================================================================
