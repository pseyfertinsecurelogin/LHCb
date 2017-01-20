
#include "STDet/DeTTStation.h"
#include "STDet/DeTTDetector.h"
#include "STDet/DeTTLayer.h"
#include "Kernel/TTNames.h"

//STL
#include <algorithm>
#include <numeric>

#include "Kernel/STChannelID.h"

using namespace LHCb;

/** @file DeTTStation.cpp
*
*  Implementation of class :  DeTTStation
*
*    @author Matthew Needham
*/


DeTTStation::DeTTStation( const std::string& name ) :
  DeSTStation( name ),
  m_parent(NULL)
{
  // constructor
}


const CLID& DeTTStation::clID () const
{
  return DeTTStation::classID() ;
}

StatusCode DeTTStation::initialize() {

  // initialize
  StatusCode sc = DeSTStation::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {

   // and the parent
   m_parent = getParent<parent_type>();

   STChannelID aChan(STChannelID::typeTT, this->id(),0,0,0,0);
   setElementID(aChan);
   m_nickname = TTNames().StationToString(aChan);

   // get the children
   m_layers = getChildren<DeTTStation>();

  }
  return sc;
}

DeTTLayer* DeTTStation::findLayer(const STChannelID aChannel){

  // return pointer to the station from channel
  auto iter = std::find_if(m_layers.begin() , m_layers.end(),
                           [&](const DeTTLayer* l) { return l->contains(aChannel);});
  return iter != m_layers.end() ? *iter: nullptr;
}

DeTTLayer* DeTTStation::findLayer(const Gaudi::XYZPoint& point) {

  // return pointer to the station from point in global system
  auto iter = std::find_if(m_layers.begin(), m_layers.end(),
                           [&](const DeTTLayer* l) { return l->isInside(point);});
  return iter != m_layers.end() ? *iter: nullptr;
}


double DeTTStation::fractionActive() const {

  return std::accumulate(m_layers.begin(), m_layers.end(), 0.0,
                         [](double f, const DeTTLayer* l) {
                            return f + l->fractionActive();
                         })/double(m_layers.size());
}


