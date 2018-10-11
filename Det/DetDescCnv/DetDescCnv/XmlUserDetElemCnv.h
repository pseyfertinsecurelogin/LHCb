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
#ifndef DETDESCCNV_XMLUSERDETELEMCNV_H
#define DETDESCCNV_XMLUSERDETELEMCNV_H 1

// Include files
#include "DetDescCnv/XmlBaseDetElemCnv.h"
#include "GaudiKernel/MsgStream.h"

// These two includes are not needed here but are needed inside children
// of this class. In order to avoid the user to forget to include them,
// we put it here
#include "XmlTools/IXmlSvc.h"

#if defined(__clang__) || defined(__cling__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Winconsistent-missing-override"
#elif defined(__GNUC__) && __GNUC__ >= 5
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wsuggest-override"
#endif

/** @class XmlUserDetElemCnv XmlUserDetElemCnv.h DetDescCnv/XmlUserDetElemCnv.h
 *
 * This is an empty, abstract, user specific, converter for DetectorElements.
 * Every actual specific convertor for DetectorElement should inherit from this
 * one. It basically provides the i_createObj, classID methods plus a
 * constructor and a destructor. The only thing to do in order to implement
 * an actual specific converter is thus to implement the second
 * i_fillSpecificObj method.
 *
 * @author Sebastien Ponce
 */
template <class DeType> class XmlUserDetElemCnv : public XmlBaseDetElemCnv {
 public:

  /**
   * Constructor for this converter
   * @param svc a ISvcLocator interface to find services
   */
  XmlUserDetElemCnv (ISvcLocator* svc);

  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID() { return DeType::classID(); }


 protected:

  /** Creates the transient representation of an object from a DOMElement.
   * Overrides the default method in XmlGenericCnv
   * @param element the DOMElement to be used to builds the object
   * @param refpObject the object to be built
   * @return status depending on the completion of the call
   */
  StatusCode i_createObj (xercesc::DOMElement* element,
                          DataObject*& refpObject) override;

  /** This fills the current object for specific child.
   * This is a very general method reimplemented from XmlBaseDetElemCnv.
   * It basically calls the other i_fillSpecificObj after casting the
   * current object to its real type.
   * @param childElement the specific child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                DetectorElement* refpObject,
                                IOpaqueAddress* address);

  /** This fills the current object for specific child.
   * This should never be called directly but always through the other
   * i_fillSpecificObj. This actually only exist to avoid the user to
   * implement a dynamic cast from DetectorElement into DeType.
   * @param childElement the specific child processed here
   * @param dataObj the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                DeType* dataObj,
                                IOpaqueAddress* address);

  /** This fills the current object for specific child.
   * \deprecated This is a deprecated function that was kept for backward
   * compatibility. You should use the new one which has a third argument
   * given the address of the created object
   * @param childElement the specific child processed here
   * @param dataObj the object to be filled
   * @return status depending on the completion of the call}
   */
  StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                DeType* dataObj);
};


// -----------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
template <class DeType>
XmlUserDetElemCnv<DeType>::XmlUserDetElemCnv (ISvcLocator* svc)
  : XmlBaseDetElemCnv (svc, DeType::classID()) {
} // end constructor


// -----------------------------------------------------------------------
// Create an object corresponding to a DOM element
// -----------------------------------------------------------------------
template <class DeType>
StatusCode
XmlUserDetElemCnv<DeType>::i_createObj(xercesc::DOMElement* /* element */,
                                       DataObject*& refpObject) {
  if( msgLevel(MSG::DEBUG) ) debug() << "entering i_createObj" << endmsg;

  // creates an object for the node found
  refpObject = new DeType();

  // returns
  return StatusCode::SUCCESS;
} // end i_createObj


// -----------------------------------------------------------------------
// Fill an object with a new specific child element
// ------------------------------------------------------------------------
template <class DeType>
StatusCode
XmlUserDetElemCnv<DeType>::i_fillSpecificObj (xercesc::DOMElement* childElement,
                                              DetectorElement* refpObject,
                                              IOpaqueAddress* address) {
  DeType* dataObj = dynamic_cast<DeType*> (refpObject);
  return i_fillSpecificObj (childElement, dataObj, address);
}

// -----------------------------------------------------------------------
// Fill an object with a new specific child element
// ------------------------------------------------------------------------
template <class DeType>
StatusCode
XmlUserDetElemCnv<DeType>::i_fillSpecificObj (xercesc::DOMElement* childElement,
                                              DeType* dataObj,
                                              IOpaqueAddress* /*address*/) {
  return i_fillSpecificObj (childElement, dataObj);
}

// -----------------------------------------------------------------------
// Fill an object with a new specific child element
// ------------------------------------------------------------------------
template <class DeType>
StatusCode
XmlUserDetElemCnv<DeType>::i_fillSpecificObj
(xercesc::DOMElement* /*childElement*/,
 DeType* /*dataObj*/) {
  return StatusCode::SUCCESS;
}

#if defined(__clang__) || defined(__cling__)
#pragma clang diagnostic pop
#elif defined(__GNUC__) && __GNUC__ >= 5
#pragma GCC diagnostic pop
#endif

#endif // DETDESCCNV_XMLUSERDETELEMCNV_H
