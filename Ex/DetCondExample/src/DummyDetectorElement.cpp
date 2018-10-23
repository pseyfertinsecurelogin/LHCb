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
// $Id: DummyDetectorElement.cpp,v 1.7 2007-02-14 16:32:14 marcocle Exp $
// Include files 

#include <string>
#include "DetDesc/Condition.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

// local
#include "DummyDetectorElement.h"

//-----------------------------------------------------------------------------
// Implementation file for class : DummyDetectorElement
//
// 2005-04-11 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

#include "DetDescCnv/XmlUserDetElemCnv.h"

typedef  XmlUserDetElemCnv<DummyDetectorElement>  XmlDummyDetectorElementCnv;
DECLARE_CONVERTER( XmlDummyDetectorElementCnv )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
DummyDetectorElement::DummyDetectorElement(const std::string& name):
  DetectorElement(name),
  m_slowTemp(0.0),
  m_maxChannels(0)
{
}
//=============================================================================
// Destructor
//=============================================================================
DummyDetectorElement::~DummyDetectorElement() {}

//=============================================================================
// Initialization
//=============================================================================
StatusCode DummyDetectorElement::initialize(){
  MsgStream log(msgSvc(),name());
  
  log << MSG::DEBUG << " --- initialize DummyDetectorElement --- " << endmsg;
  StatusCode sc = DetectorElement::initialize();
  if ( !sc.isSuccess() ) return sc;
  
  try {
    log << MSG::DEBUG << "Registering conditions" << endmsg;
    updMgrSvc()->registerCondition(this,condition("Temperature").path(),&DummyDetectorElement::i_updateTemperatures);
    updMgrSvc()->registerCondition(this,condition("ReadOut").path(),&DummyDetectorElement::i_updateTemperatures);
    updMgrSvc()->registerCondition(this,condition("ReadOut").path(),&DummyDetectorElement::i_updateChannels);
    log << MSG::DEBUG << "Start first update" << endmsg;
    sc = updMgrSvc()->update(this);
    if ( !sc.isSuccess() ) {
  	  return sc;
    }
  } catch (DetectorElementException &e) {
    log << MSG::ERROR << e << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Update methods
//=============================================================================
StatusCode DummyDetectorElement::i_updateTemperatures(){
  MsgStream log(msgSvc(),name());
  log << MSG::DEBUG << "Entering i_updateTemperatures()" << endmsg;
  try {
    m_slowTemp   = condition("Temperature")->param<double>("Temperature");
    m_cratesTemp = condition("ReadOut")->param<std::vector<double> >("CrateTemps");
  } catch (...) {
    log << MSG::ERROR << "i_updateTemperatures: couldn't access condition" << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
// Update methods
//=============================================================================
StatusCode DummyDetectorElement::i_updateChannels(){
  MsgStream log(msgSvc(),name());
  log << MSG::DEBUG << "Entering i_updateChannels()" << endmsg;
  try {
    m_maxChannels = condition("ReadOut")->param<int>("NChannels");
    m_activeChannels = condition("ReadOut")->param<std::vector<int> >("Channels");
  } catch (...) {
    log << MSG::ERROR << "i_updateChannels: couldn't access condition" << endmsg;
    return StatusCode::FAILURE;
  }
  return StatusCode::SUCCESS;
}
//=============================================================================
