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
// $Id: XmlDummyDECnv.h,v 1.1 2005-04-22 15:17:54 marcocle Exp $
#ifndef XMLDUMMYDECNV_H
#define XMLDUMMYDECNV_H 1

// Include files
#include "DetDescCnv/XmlUserDetElemCnv.h"
#include "DummyDetectorElement.h"

/** @class XmlDummyDECnv XmlDummyDECnv.h
 *
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-04-11
 */
class XmlDummyDECnv: public XmlUserDetElemCnv<DummyDetectorElement>{
public:
  /// Standard constructor
  XmlDummyDECnv(ISvcLocator* svc);

  virtual ~XmlDummyDECnv(); ///< Destructor

  using XmlUserDetElemCnv<DummyDetectorElement>::i_fillSpecificObj;
  virtual StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                        DummyDetectorElement* dataObj,
                                        IOpaqueAddress* address);

protected:

private:

};
#endif // XMLDUMMYDECNV_H
