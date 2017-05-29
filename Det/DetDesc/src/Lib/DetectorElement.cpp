#include <algorithm>
#include <iterator>
#include <map>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

#include "DetDesc/AlignmentInfo.h"
#include "DetDesc/CalibrationInfo.h"
#include "DetDesc/Condition.h"
#include "DetDesc/DetDesc.h"
///
#include "DetDesc/DetectorElement.h"
#include "DetDesc/DetectorElementException.h"
#include "DetDesc/FastControlInfo.h"
#include "DetDesc/GeoInfo.h"
#include "DetDesc/IAlignment.h"
#include "DetDesc/ICalibration.h"
#include "DetDesc/IDetectorElement.h"
#include "DetDesc/IFastControl.h"
#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/ILVolume.h"
#include "DetDesc/IReadOut.h"
#include "DetDesc/ISlowControl.h"
#include "DetDesc/ParamValidDataObject.h"
#include "DetDesc/ReadOutInfo.h"
#include "DetDesc/Services.h"
#include "DetDesc/SlowControlInfo.h"
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IInterface.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Point3DTypes.h"
#include "GaudiKernel/SmartDataPtr.h"
#include "GaudiKernel/SmartDataStorePtr.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/SmartRef.h"
#include "GaudiKernel/StatusCode.h"

class IUpdateManagerSvc;

/** @file DetectorElement.cpp
 *
 * Implementation of class DetectorElement
 *
 * @author Vanya Belyaev Ivan.Belyaev@itep.ru
 * @author Sebastien Ponce
 * @author Marco Clemencic <marco.clemencic@cern.ch>
 */
DetectorElement::DetectorElement( const std::string&   /* name */ )
: m_services{ DetDesc::services() }
{
}

DetectorElement::~DetectorElement() = default;

IDataProviderSvc* DetectorElement::dataSvc() const {
  return m_services->detSvc();
}

IUpdateManagerSvc* DetectorElement::updMgrSvc() const {
  return m_services->updMgrSvc();
}

IDetectorElement*  DetectorElement::parentIDetectorElement() const {
  SmartIF<IDataManagerSvc> mgr( dataSvc() );
  if( mgr ) {
    IRegistry* pRegParent = nullptr;
    StatusCode sc = mgr->objectParent(this, pRegParent);
    if ( sc.isSuccess() && pRegParent ) {
      return dynamic_cast<IDetectorElement*>(pRegParent->object());
    }
  }
  return nullptr;
}

unsigned long DetectorElement::addRef  () {
  return ParamValidDataObject::addRef();
}

unsigned long DetectorElement::release () {
  return ParamValidDataObject::release();
}

StatusCode
DetectorElement::queryInterface( const InterfaceID& ID , void** ppI )
{
  if (!ppI) return StatusCode::FAILURE;
  *ppI = nullptr ;
  if (IDetectorElement::interfaceID() == ID) {
    *ppI = static_cast<IDetectorElement*> (this);
  } else if (IInterface::interfaceID() == ID) {
    *ppI = static_cast<IInterface*> (this);
  } else {
    return StatusCode::FAILURE;
  }
  /// add the reference
  addRef();
  ///
  return StatusCode::SUCCESS;
}

std::ostream& DetectorElement::printOut( std::ostream& os ) const
{
  os << "DetectorElement::"  << name();
  return !geometry() ? os : (os << "GeometryInfo::" << geometry());
}

MsgStream& DetectorElement::printOut( MsgStream& os ) const
{
  os << "DetectorElement::"  << name();
  return !geometry() ? os : (os << "GeometryInfo::" << geometry());
}
/// reset to the initial state/////
IDetectorElement* DetectorElement::reset()
{
  /// reset the pointed-at geometry
  if( geometry() ) { geometry()->reset() ;}
  if( m_de_childrensLoaded )
    std::for_each( childBegin() , childEnd() ,
                   [](IDetectorElement* de) { de->reset(); } );
  m_de_childrensLoaded = false ;
  m_de_childrens.clear()       ;
  return this;
}

// ----------------------------------------------------------------------
bool DetectorElement::hasCondition(const std::string &name) const {
  return m_de_conditions.find(name) != m_de_conditions.end();
}

SmartRef<Condition> DetectorElement::condition(const std::string &name) const {
  auto cond = m_de_conditions.find(name);
  if ( cond == m_de_conditions.end() ) {
    std::ostringstream oss;
    oss << "Requested unknown condition '" << name << "' to '" << this->name() << "'";
    throw DetectorElementException(oss.str(), this,
        StatusCode(StatusCode::FAILURE,true));
  }
  return cond->second;
}

void DetectorElement::createCondition(std::string &name, std::string &path) {
  auto cond = m_de_conditions.find(name);
  Assert(cond == m_de_conditions.end(),
         "Could not add condition: " + name + " already present!" );
  long hint = linkMgr()->addLink(path,0);
  m_de_conditions.insert(ConditionMap::value_type(name,SmartRef<Condition>(this,hint)));
}

//-- N. Gilardi; 2005.07.08 ---------------------------------------------
/// Get the list of existing conditions.
std::vector<std::string> DetectorElement::conditionNames() const {
  std::vector<std::string> v; v.reserve(m_de_conditions.size());
  std::transform( m_de_conditions.begin(), m_de_conditions.end(),
                  std::back_inserter(v),
                  [](ConditionMap::const_reference i) { return i.first; } );
  return v;
}
// ----------------------------------------------------------------------

/////
const IGeometryInfo*
DetectorElement::createGeometryInfo()
{
  Assert( !geometry() ,
          "Could not create GHOST: Geometry already exist!" );
  m_de_iGeometry.reset(GeoInfo::createGeometryInfo( this ));
  return geometry();
}
/////
const IGeometryInfo*
DetectorElement::createGeometryInfo( const std::string& LogVol )
{
  Assert( !geometry() ,
          "Could not create ORPHAN: Geometry already exist!" );
  m_de_iGeometry.reset( GeoInfo::createGeometryInfo( this , LogVol ) );
  return geometry();
}
/////
const IGeometryInfo*
DetectorElement::createGeometryInfo( const std::string& LogVol   ,
                                     const std::string& Support  ,
                                     const std::string& NamePath )
{
  Assert( !geometry() ,
          "Could not create REGULAR(1): Geometry already exist!" );
  m_de_iGeometry.reset( GeoInfo::createGeometryInfo( this     ,
                                                LogVol   ,
                                                Support  ,
                                                NamePath ) );
  return geometry();
}
/////
const IGeometryInfo*
DetectorElement::createGeometryInfo( const std::string& LogVol   ,
                                     const std::string& Support  ,
                                     const std::string& NamePath,
                                     const std::string& alignmentPath)
{
  Assert( !geometry() ,
          "Could not create REGULAR(1): Geometry already exist!" );
  m_de_iGeometry.reset( GeoInfo::createGeometryInfo( this     ,
                                                LogVol   ,
                                                Support  ,
                                                NamePath ,
                                                alignmentPath) );
  return geometry();
}
//
const IGeometryInfo*
DetectorElement::createGeometryInfo( const std::string           & LogVol   ,
                                     const std::string           & Support  ,
                                     const ILVolume::ReplicaPath & rPath    )
{
  Assert( !geometry() ,
          "Could not create REGULAR(2): Geometry already exist!" );
  m_de_iGeometry.reset( GeoInfo::createGeometryInfo( this    ,
                                                LogVol  ,
                                                Support ,
                                                rPath   ) );
  return geometry();
}
//=============================================================================
const IGeometryInfo*
DetectorElement::createGeometryInfo( const std::string           & LogVol   ,
                                     const std::string           & Support  ,
                                     const ILVolume::ReplicaPath & rPath,
                                     const std::string& alignmentPath)
{
  Assert( !geometry() ,
          "Could not create REGULAR(2): Geometry already exist!" );
  m_de_iGeometry.reset( GeoInfo::createGeometryInfo( this    ,
                                                LogVol  ,
                                                Support ,
                                                rPath   ,
                                                alignmentPath) );
  return geometry();
}
//=============================================================================
const IAlignment*
DetectorElement::createAlignment (const std::string& condition) {
  Assert( !alignment() ,
          "Could not create AlignmentInfo: it already exists!" );
  if( !m_de_iAlignment )
    m_de_iAlignment = std::make_unique<AlignmentInfo>( this, condition );
  return alignment();
}

const ICalibration*
DetectorElement::createCalibration (const std::string& condition) {
  Assert( !calibration() ,
          "Could not create CalibrationInfo: it already exists!" );
  if( !m_de_iCalibration )
    m_de_iCalibration = std::make_unique<CalibrationInfo>( this, condition );
  return calibration();
}

const IReadOut*
DetectorElement::createReadOut (const std::string& condition) {
  Assert( !readOut() ,
          "Could not create ReadOutCalibrationInfo: it already exists!" );
  if( !m_de_iReadOut )
    m_de_iReadOut = std::make_unique<ReadOutInfo>( this, condition );
  return readOut();
}

const ISlowControl*
DetectorElement::createSlowControl (const std::string& condition) {
  Assert( !slowControl() ,
          "Could not create SlowControlInfo: it already exists!" );
  if( !m_de_iSlowControl )
    m_de_iSlowControl = std::make_unique<SlowControlInfo>( this, condition );
  return slowControl();
}

const IFastControl*
DetectorElement::createFastControl (const std::string& condition) {
  Assert( !fastControl() ,
          "Could not create FastControlInfo: it already exists!" );
  if( !m_de_iFastControl )
    m_de_iFastControl = std::make_unique<FastControlInfo>( this, condition );
  return fastControl();
}
//
StatusCode DetectorElement::initialize() {
  // this is a default implementation that does nothing.
  // it is up to the user to override this in a child of DetectorElement
  return StatusCode::SUCCESS;
}


//----------------------------------------------------------------------------

/// (reference to) container of pointers to child detector elements ///////////
IDetectorElement::IDEContainer&
DetectorElement::childIDetectorElements() const {
  /// already loaded?
  if( m_de_childrensLoaded ) { return m_de_childrens; }
  /// load them!
  SmartIF<IDataManagerSvc> mgr( dataSvc() );
  if( mgr ) {
    std::vector<IRegistry*> leaves;
    StatusCode sc = mgr->objectLeaves(this, leaves);
    if ( sc.isSuccess() ) {
      for ( const auto& l : leaves ) {
        Assert (l , "DirIterator points to NULL!" );
        const std::string& nam = l->identifier();
        SmartDataPtr<IDetectorElement> de( dataSvc() , nam );
        IDetectorElement* ide = de;
        Assert (ide , "Could not load child object="+nam );
        m_de_childrens.push_back( ide  );
      }
      m_de_childrensLoaded = true;
    }
  }
  return m_de_childrens;
}

const ParamValidDataObject *DetectorElement::params() const {
  return this;
}
/// sensitive volume identifier ///////////////////////////////////////////////
int DetectorElement::sensitiveVolumeID(const Gaudi::XYZPoint& globalPoint) const
{
  if (!isInside(globalPoint)) return -1;
  const IDetectorElement* child = childDEWithPoint(globalPoint);
  return child ? child->sensitiveVolumeID(globalPoint) : -1;
}

bool DetectorElement::isInside(const Gaudi::XYZPoint& globalPoint) const {
  const IGeometryInfo* gi = geometry();
  return gi && gi->isInside(globalPoint);
}

const IDetectorElement* DetectorElement::childDEWithPoint(const Gaudi::XYZPoint& globalPoint) const
{
  auto iDE = std::find_if( childBegin(), childEnd(),
                         [&](IDetectorElement::IDEContainer::const_reference i) {
    auto pDE = dynamic_cast<DetectorElement*>(i);
    return pDE && pDE->isInside(globalPoint);
  });
  return iDE!=childEnd() ? *iDE : nullptr;
}
// ============================================================================
const std::string& DetectorElement::name () const {
  static const std::string s_empty;
  IRegistry* pReg = registry();
  return pReg ? pReg->identifier() : s_empty;
}
// ============================================================================
// End
// ============================================================================
