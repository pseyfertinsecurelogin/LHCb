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
//  $ID: $

// include files
#include "DetDesc/TabulatedProperty.h"
#include "GaudiKernel/ISvcLocator.h"

#include "XmlDetectorElementCnv.h"

// -----------------------------------------------------------------------
// Instantiation of a static factory class used by clients to create
// instances of this service
// -----------------------------------------------------------------------
DECLARE_CONVERTER( XmlDetectorElementCnv )

// -----------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
XmlDetectorElementCnv::XmlDetectorElementCnv( ISvcLocator* svc ) : XmlBaseDetElemCnv( svc ) {}

// -----------------------------------------------------------------------
// Fill an object with a new specific child
// -----------------------------------------------------------------------
StatusCode XmlDetectorElementCnv::i_fillSpecificObj( xercesc::DOMElement* /*childElement*/,
                                                     DetectorElement* /*refpObject*/, IOpaqueAddress* /*address*/ ) {
  // default implementation : ignore child
  return StatusCode::SUCCESS;
} // end i_fillObj
