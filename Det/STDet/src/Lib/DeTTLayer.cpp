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

#include "STDet/DeTTSector.h"
#include "STDet/DeTTLayer.h"
#include "STDet/DeTTHalfModule.h"
#include "STDet/DeTTStation.h"

#include "DetDesc/IGeometryInfo.h"

#include "Kernel/TTNames.h"

//STL
#include <algorithm>
#include <numeric>

using namespace LHCb;

/** @file DeTTLayer.cpp
*
*  Implementation of class :  DeTTLayer
*
*    @author Matthew Needham
*/


DeTTLayer::DeTTLayer( const std::string& name ) :
  DeSTLayer( name ),
  m_parent(NULL)
{
  // constructor
  m_modules.clear();
}


const CLID& DeTTLayer::clID () const
{
  return DeTTLayer::classID() ;
}

StatusCode DeTTLayer::initialize() {

  // initialize method
  MsgStream msg(msgSvc(), name() );
  StatusCode sc = DeSTLayer::initialize();
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    m_parent = getParent<DeTTLayer>();
    STChannelID parentID = m_parent->elementID();
    STChannelID chan(STChannelID::detType::typeTT, parentID.station(), id(), 0, 0, 0);
    setElementID(chan);
    m_modules = getChildren<DeTTLayer>();
    flatten();
    m_nickname = TTNames().UniqueLayerToString(chan);
  }
  return sc;
}

DeTTHalfModule* DeTTLayer::findHalfModule(const STChannelID aChannel){

  // return pointer to the station from channel
  auto iter = std::find_if(m_modules.begin() , m_modules.end(),
                           [&](const DeTTHalfModule* m) { return m->contains(aChannel);});
  return iter != m_modules.end() ? *iter: nullptr;
}

DeTTHalfModule* DeTTLayer::findHalfModule(const Gaudi::XYZPoint& point) {

  // find the half module
  auto iter = std::find_if(m_modules.begin(), m_modules.end(),
                           [&](const DeTTHalfModule* m) { return m->isInside(point);});

  return iter != m_modules.end() ? *iter: nullptr;
}


void DeTTLayer::flatten() {

  DeTTLayer::Children::const_iterator iterModule = halfModules().begin();
  for (;iterModule != halfModules().end(); ++iterModule){
     DeTTHalfModule* tModule = *iterModule;
     DeTTHalfModule::Children::const_iterator iterSector = tModule->sectors().begin();
     for ( ; iterSector !=  tModule->sectors().end() ; ++iterSector ){
       DeSTSector* tSector = *iterSector;
       m_sectors.push_back(tSector);
     } //sectors
  } // half module

}


double DeTTLayer::fractionActive() const {

  return std::accumulate(m_modules.begin(), m_modules.end(), 0.0,
            [](double f, const DeTTHalfModule* m) { return f+m->fractionActive(); }
         )/double(m_modules.size());
}









