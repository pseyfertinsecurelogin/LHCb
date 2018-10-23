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
#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTModule.h"
#include "UTDet/DeUTStation.h"

#include "DetDesc/IGeometryInfo.h"
#include "DetDesc/SolidBox.h"

#include "Kernel/UTNames.h"

#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

#include <algorithm>
#include <numeric>

using namespace LHCb;

/** @file DeUTLayer.cpp
 *
 *  Implementation of class :  DeUTLayer
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTLayer::DeUTLayer( const std::string& name ) :
  DeUTBaseElement( name ),
  m_id(0u),
  m_angle(0.0),
  m_sinAngle(0.0),
  m_cosAngle(0.0),
  m_parent(NULL)
{
  m_modules.clear();
}


const CLID& DeUTLayer::clID () const
{
  return DeUTLayer::classID() ;
}


StatusCode DeUTLayer::initialize()
{
  // initialize method
  MsgStream msg(msgSvc(), name() );
  StatusCode sc = DeUTBaseElement::initialize();
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    m_id = param<int>("layerID");
    m_angle = param<double>("stereoangle");
    m_cosAngle = cos(m_angle);
    m_sinAngle = sin(m_angle);

    // cache trajectories
    sc = registerCondition(this,this->geometry(),&DeUTLayer::cachePlane);
    if (sc.isFailure() ){
      msg << MSG::ERROR << "Failed to register conditions" << endmsg;
      return StatusCode::FAILURE;
    }
    m_parent = getParent<DeUTLayer>();
    UTChannelID parentID = m_parent->elementID();
    UTChannelID chan(UTChannelID::detType::typeUT, parentID.station(), id(), 0, 0, 0);
    setElementID(chan);
    m_modules = getChildren<DeUTLayer>();
    flatten();
    m_nickname = UTNames().UniqueLayerToString(chan);
  }
  return sc;
}

std::ostream& DeUTLayer::printOut( std::ostream& os ) const{

  // stream to cout
  os << " Layer: "  << m_id << std::endl;
  os << "stereo angle " << m_angle << std::endl
     << " Nickname: " << m_nickname
     << std::endl;

  return os;
}

MsgStream& DeUTLayer::printOut( MsgStream& os ) const{

  // stream to Msg service
  os << " Station : "  << m_id << endmsg;
  os << "stereo angle " << m_angle
     << " Nickname: " << m_nickname
     << endmsg;

  return os;
}

StatusCode DeUTLayer::cachePlane(){

 Gaudi::XYZPoint p1 = globalPoint(0,0,0);
 Gaudi::XYZPoint p2 = globalPoint(3*Gaudi::Units::m,0,0);
 Gaudi::XYZPoint p3 = globalPoint(0,3*Gaudi::Units::cm,0);

 m_plane = Gaudi::Plane3D(p1,p2,p3);

 return StatusCode::SUCCESS;
}

DeUTModule* DeUTLayer::findModule(const UTChannelID aChannel)
{
  auto iter = std::find_if(m_modules.begin() , m_modules.end(),
                           [&](const DeUTModule* m) { return m->contains(aChannel);});
  return iter != m_modules.end() ? *iter: nullptr;
}


DeUTModule* DeUTLayer::findModule(const Gaudi::XYZPoint& point)
{
  auto iter = std::find_if(m_modules.begin(), m_modules.end(),
                           [&](const DeUTModule* m) { return m->isInside(point);});
  return iter != m_modules.end() ? *iter: nullptr;
}


void DeUTLayer::flatten()
{
  DeUTLayer::Children::const_iterator iterModule = modules().begin();
  for (;iterModule != modules().end(); ++iterModule){
    DeUTModule* tModule = *iterModule;
    DeUTModule::Children::const_iterator iterSector = tModule->sectors().begin();
    for ( ; iterSector !=  tModule->sectors().end() ; ++iterSector ){
      DeUTSector* tSector = *iterSector;
      m_sectors.push_back(tSector);
    }
  }
}


double DeUTLayer::fractionActive() const
{
  return std::accumulate(m_modules.begin(), m_modules.end(), 0.0,
                [](double f, const DeUTModule* m) { return f + m->fractionActive(); }
         )/double(m_modules.size());
}
