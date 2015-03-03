
#include "STDet/DeITStation.h"
#include "STDet/DeITDetector.h"
#include "STDet/DeITBox.h"

/** @file DeITStation.cpp
*
*  Implementation of class :  DeITStation
*
*    @author Matthew Needham
*/

#include "Kernel/STChannelID.h"

// Boost
#include <boost/lambda/bind.hpp>
#include <boost/lambda/lambda.hpp>

using namespace boost::lambda;
using namespace LHCb;

DeITStation::DeITStation( const std::string& name ) :
  DeSTStation( name )
{ 
  // constructer
}


DeITStation::~DeITStation() {

}

const CLID& DeITStation::clID () const
{
  return DeITStation::classID() ;
}

StatusCode DeITStation::initialize() {

  // initialize
  MsgStream msg(msgSvc(), name() );
  StatusCode sc = DeSTStation::initialize();
  if (sc.isFailure() ){
    msg << MSG::ERROR << "Failed to initialize detector element" << endreq; 
  }
  else {
    // get the children
    m_parent = getParent<DeITStation>();
    STChannelID aChan(STChannelID::typeIT,this->id(),0,0,0,0);
    setElementID(aChan); 
    m_boxes = getChildren<DeITStation>(); 
  }
  return sc;
}

DeITBox* DeITStation::findBox(const STChannelID aChannel){

  // return pointer to the station from channel
  Children::iterator iter = std::find_if(m_boxes.begin() , m_boxes.end(), bind(&DeITBox::contains, _1, aChannel));
  return (iter != m_boxes.end() ? *iter: 0);
}

DeITBox* DeITStation::findBox(const Gaudi::XYZPoint& point) {

  // return pointer to the station from point in global system
  Children::iterator iter = std::find_if(m_boxes.begin(), m_boxes.end(), 
                                                        bind(&DeITBox::isInside, _1, point)); 
  return (iter != m_boxes.end() ? *iter: 0);
}



