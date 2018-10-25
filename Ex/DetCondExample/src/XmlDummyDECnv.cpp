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
// $Id: XmlDummyDECnv.cpp,v 1.3 2007-01-23 07:54:54 cattanem Exp $
// Include files 

#include <iostream>

// local
#include "XmlDummyDECnv.h"

//-----------------------------------------------------------------------------
// Implementation file for class : XmlDummyDECnv
//
// 2005-04-11 : Marco CLEMENCIC
//-----------------------------------------------------------------------------

DECLARE_CONVERTER( XmlDummyDECnv )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
XmlDummyDECnv::XmlDummyDECnv(ISvcLocator* svc):
    XmlUserDetElemCnv<DummyDetectorElement>(svc) {

  //std::cout << "XmlDummyDECnv: Hello world!" << std::endl;
}

//=============================================================================
// Destructor
//=============================================================================
XmlDummyDECnv::~XmlDummyDECnv() {}

//=============================================================================
// Specific Fill
//=============================================================================
StatusCode XmlDummyDECnv::i_fillSpecificObj(xercesc::DOMElement* childElement,
                                            DummyDetectorElement* dataObj,
                                            IOpaqueAddress* address){
  // Nothing special, just fill a generic DE
  return XmlUserDetElemCnv<DummyDetectorElement>::i_fillSpecificObj(childElement, dynamic_cast<DetectorElement*>(dataObj),
                                                                    address);
}
//=============================================================================



