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
#ifndef _DeUTBaseElement_H_
#define _DeUTBaseElement_H_

#include <algorithm>
#include <string>
#include <vector>

#include "DetDesc/DetectorElement.h"
#include "GaudiKernel/GaudiException.h"
#include "Kernel/UTChannelID.h"
#include "UTDet/UTDetTraits.h"

#include "GaudiKernel/IUpdateManagerSvc.h"

class DeUTBaseElement : public DetectorElement {

public:
  /** Constructor */
  DeUTBaseElement( const std::string& name = "" );

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /** transform global to local point
   * @param  point global point
   * @return       local point
   */
  Gaudi::XYZPoint toLocal( const Gaudi::XYZPoint& point ) const;

  /** transform global to local point
   * @param  point  local point
   * @return        global point
   */
  Gaudi::XYZPoint toGlobal( const Gaudi::XYZPoint& point ) const;

  /** centre in global frame
   * @return global centre
   */
  Gaudi::XYZPoint globalCentre() const;

  /** convert x,y,z in local frame to global frame
   * @param x local x
   * @param y local y
   * @param z local z
   * @return point
   */
  Gaudi::XYZPoint globalPoint( const double x, const double y, const double z ) const;

  /** detector element id  - for experts only !*/
  LHCb::UTChannelID elementID() const;

  /** check whether contains
   *  @param  aChannel channel
   *  @return bool
   */
  virtual bool contains( const LHCb::UTChannelID aChannel ) const = 0;

  /** set detector element id  - for experts only !*/
  void setElementID( const LHCb::UTChannelID& chanID );

  /** get the parent of the detector element
  @return parenttype */
  template <typename TYPE, typename ParentType = typename UTDetTraits<TYPE>::parent>
  ParentType* getParent() const {
    auto* parent = dynamic_cast<ParentType*>( this->parentIDetectorElement() );
    if ( !parent ) { throw GaudiException( "Orphaned detector element", "DeUTBaseElement", StatusCode::FAILURE ); }
    return parent;
  }

  /** retrieve the children
   * @return children
   */
  template <typename TYPE>
  std::vector<typename UTDetTraits<TYPE>::child*> getChildren();

  /**
   * call back for update service
   * @param caller
   * @param object
   * @param mf
   * @param forceUpdate force update
   * @return StatusCode Success or Failure
   */
  template <typename CallerClass, typename ObjectClass>
  StatusCode registerCondition( CallerClass* caller, ObjectClass* object,
                                typename ObjectMemberFunction<CallerClass>::MemberFunctionType mf,
                                bool                                                           forceUpdate = true );

  /**
   * call back for update service
   * @param caller
   * @param conditionName
   * @param mf
   * @param forceUpdate force update
   * @return StatusCode Success or Failure
   */
  template <typename CallerClass>
  StatusCode registerCondition( CallerClass* caller, const std::string& conditionName,
                                typename ObjectMemberFunction<CallerClass>::MemberFunctionType mf,
                                bool                                                           forceUpdate = true );

private:
  StatusCode cachePoint();

  bool duplicate( const std::string& testString, const std::vector<std::string>& names ) const;

  LHCb::UTChannelID m_elementID;
  Gaudi::XYZPoint   m_globalCentre;
};

#include "DetDesc/IGeometryInfo.h"

inline Gaudi::XYZPoint DeUTBaseElement::toLocal( const Gaudi::XYZPoint& point ) const {
  return this->geometry()->toLocal( point );
}

inline Gaudi::XYZPoint DeUTBaseElement::toGlobal( const Gaudi::XYZPoint& point ) const {
  return this->geometry()->toGlobal( point );
}

inline Gaudi::XYZPoint DeUTBaseElement::globalCentre() const { return m_globalCentre; }

inline LHCb::UTChannelID DeUTBaseElement::elementID() const { return m_elementID; }

inline void DeUTBaseElement::setElementID( const LHCb::UTChannelID& chanID ) { m_elementID = chanID; }

template <typename TYPE>
inline std::vector<typename UTDetTraits<TYPE>::child*> DeUTBaseElement::getChildren() {

  using cType               = typename UTDetTraits<TYPE>::child;
  const unsigned int  nElem = childIDetectorElements().size();
  std::vector<cType*> childVector;
  childVector.reserve( nElem );
  std::vector<std::string> names;
  names.reserve( nElem );

  for ( auto iChild = childBegin(); this->childEnd() != iChild; ++iChild ) {
    auto* aChild = dynamic_cast<cType*>( *iChild );
    if ( aChild != 0 ) {
      if ( duplicate( aChild->name(), names ) == false ) {
        names.push_back( aChild->name() );
        childVector.push_back( aChild );
      } else {
        MsgStream msg( msgSvc(), name() );
        msg << MSG::WARNING << "tried to make duplicate detector element !" << aChild->name() << endmsg;
      } // if
    }   // if
  }     // iStation

  if ( childVector.empty() ) {
    throw GaudiException( "Sterile detector element", "DeUTBaseElement", StatusCode::FAILURE );
  }

  return childVector;
}

inline bool DeUTBaseElement::duplicate( const std::string& testString, const std::vector<std::string>& names ) const {
  auto iter = std::find( names.begin(), names.end(), testString );
  return ( iter != names.end() );
}

template <typename CallerClass, typename ObjectClass>
inline StatusCode DeUTBaseElement::registerCondition( CallerClass* caller, ObjectClass* object,
                                                      typename ObjectMemberFunction<CallerClass>::MemberFunctionType mf,
                                                      bool forceUpdate ) {

  // initialize method
  MsgStream  msg( msgSvc(), name() );
  StatusCode sc = StatusCode::SUCCESS;

  try {
    //   if (forceUpdate) updMgrSvc()->invalidate(this);
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Registering conditions" << endmsg;
    updMgrSvc()->registerCondition( caller, object, mf );
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Start first update" << endmsg;
    if ( forceUpdate ) {
      sc = updMgrSvc()->update( caller );
      if ( sc.isFailure() ) { msg << MSG::WARNING << "failed to update detector element " << endmsg; }
    }
  } catch ( DetectorElementException& e ) {
    msg << MSG::ERROR << " Failed to update condition:" << caller->name() << endmsg;
    msg << MSG::ERROR << e << endmsg;
    return StatusCode::FAILURE;
  }
  return sc;
}

template <typename CallerClass>
inline StatusCode DeUTBaseElement::registerCondition( CallerClass* caller, const std::string& conditionName,
                                                      typename ObjectMemberFunction<CallerClass>::MemberFunctionType mf,
                                                      bool forceUpdate ) {

  // initialize method
  MsgStream  msg( msgSvc(), name() );
  StatusCode sc = StatusCode::SUCCESS;

  try {
    // if (forceUpdate) updMgrSvc()->invalidate(this);
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Registering " << conditionName << " condition" << endmsg;
    updMgrSvc()->registerCondition( caller, condition( conditionName ).path(), mf );
    if ( msg.level() <= MSG::DEBUG ) msg << MSG::DEBUG << "Start first update" << endmsg;
    if ( forceUpdate ) {
      sc = updMgrSvc()->update( caller );
      if ( sc.isFailure() ) {
        msg << MSG::WARNING << "failed to update detector element " << condition( conditionName ).path() << endmsg;
      }
    }
  } catch ( DetectorElementException& e ) {
    msg << MSG::ERROR << " Failed to update condition:" << conditionName << " " << caller->name() << endmsg;
    msg << MSG::ERROR << "Message is " << e << endmsg;
    return StatusCode::FAILURE;
  }
  return sc;
}

#endif
