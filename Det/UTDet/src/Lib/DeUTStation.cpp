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
#include "UTDet/DeUTStation.h"
#include "UTDet/DeUTDetector.h"
#include "UTDet/DeUTLayer.h"
#include "Kernel/UTNames.h"
#include "Kernel/UTChannelID.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"

#include <algorithm>
#include <numeric>

using namespace LHCb;

/** @file DeUTStation.cpp
 *
 *  Implementation of class :  DeUTStation
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTStation::DeUTStation( std::string name ) :
  DeUTBaseElement( std::move(name) ),
  m_id(0u)
{
}

const CLID& DeUTStation::clID () const
{
  return DeUTStation::classID();
}

StatusCode DeUTStation::initialize()
{
  // initialize
  StatusCode sc = DeUTBaseElement::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    // and the parent
    m_id = (unsigned int)param<int>("stationID");
    m_parent = getParent<parent_type>();

    UTChannelID aChan(UTChannelID::detType::typeUT, this->id(),0,0,0,0);
    setElementID(aChan);
    m_nickname = UTNames().StationToString(aChan);

    // get the children
    m_layers = getChildren<DeUTStation>();
  }

  return sc;
}

std::ostream& DeUTStation::printOut( std::ostream& os ) const{

  // stream to cout
  os << " Station: "  << m_id << std::endl;
  os   << " Nickname: " << m_nickname
     << std::endl;

  return os;
}

MsgStream& DeUTStation::printOut( MsgStream& os ) const{

  // stream to Msg service
  os << " Station : "  << m_id << endmsg;
  os   << " Nickname: " << m_nickname << endmsg;

  return os;
}


DeUTLayer* DeUTStation::findLayer(const UTChannelID aChannel)
{
  auto iter = std::find_if(m_layers.begin() , m_layers.end(),
                           [&](const DeUTLayer *l) { return l->contains(aChannel); } );
  return (iter != m_layers.end() ? *iter: nullptr);
}


DeUTLayer* DeUTStation::findLayer(const Gaudi::XYZPoint& point)
{
  auto iter = std::find_if(m_layers.begin(), m_layers.end(),
                           [&](const DeUTLayer *l) { return l->isInside(point); } );
  return (iter != m_layers.end() ? *iter: nullptr);
}


double DeUTStation::fractionActive() const
{
  return std::accumulate(m_layers.begin(), m_layers.end(), 0.0,  [&](double f,const DeUTLayer* l)
                         { return f + l->fractionActive(); } )/double(m_layers.size());
}
