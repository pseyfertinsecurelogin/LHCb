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
#ifndef DETDESCCNV_XMLCNVSVC_XMLCONDITIONCNV_H
#define DETDESCCNV_XMLCNVSVC_XMLCONDITIONCNV_H

/// Include files
#include "DetDescCnv/XmlBaseConditionCnv.h"


/** @class XmlConditionCnv
 *
 * XML converter for Conditions.
 *
 * @author Sebastien Ponce
 */
class XmlConditionCnv : public XmlBaseConditionCnv {
public:
  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlConditionCnv(ISvcLocator* svcs);

protected:
  /** This fills the current object for specific child.
   * Implements the abstract method in XmlConditionCnv
   * @param childElement the specific child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                Condition* refpObject,
                                IOpaqueAddress* address) override;

};

#endif // DETDESCCNV_XMLCONDITIONCNV_H
