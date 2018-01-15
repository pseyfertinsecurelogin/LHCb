
#include "STDet/DeITStation.h"
#include "STDet/DeITDetector.h"
#include "STDet/DeITBox.h"

#include "Kernel/ITNames.h"

/** @file DeITStation.cpp
*
*  Implementation of class :  DeITStation
*
*    @author Matthew Needham
*/

#include "Kernel/STChannelID.h"

//STL
#include <algorithm>
#include <numeric>


using namespace LHCb;

DeITStation::DeITStation( const std::string& name ) :
  DeSTStation( name ),
  m_parent(NULL)
{
  // constructor
}


const CLID& DeITStation::clID () const
{
  return DeITStation::classID() ;
}

StatusCode DeITStation::initialize() {

  // initialize
  StatusCode sc = DeSTStation::initialize();
  if (sc.isFailure() ){
    MsgStream msg(msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  }
  else {
    // get the children
    m_parent = getParent<DeITStation>();
    STChannelID aChan(STChannelID::detType::typeIT,this->id(),0,0,0,0);
    setElementID(aChan);
    m_nickname = ITNames().StationToString(aChan);
    m_boxes = getChildren<DeITStation>();
  }
  return sc;
}

DeITBox* DeITStation::findBox(const STChannelID aChannel){

  // return pointer to the station from channel
  auto iter = std::find_if(m_boxes.begin() , m_boxes.end(),
                           [&](const DeITBox* b) { return b->contains(aChannel);});
  return iter != m_boxes.end() ? *iter: nullptr;
}

DeITBox* DeITStation::findBox(const Gaudi::XYZPoint& point) {

  // return pointer to the station from point in global system
  auto iter = std::find_if(m_boxes.begin(), m_boxes.end(),
                           [&](const DeITBox* b) { return b->isInside(point);});
  return iter != m_boxes.end() ? *iter: nullptr;
}

double DeITStation::fractionActive() const {

  return std::accumulate(m_boxes.begin(), m_boxes.end(), 0.0,
                         [](double f, const DeITBox* b) { return f + b->fractionActive(); }
                        )/double(m_boxes.size());
}




