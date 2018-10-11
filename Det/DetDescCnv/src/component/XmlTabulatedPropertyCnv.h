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
#ifndef     DETDESCCNV_XMLTABULATEDPROPERTYCNV_H
#define     DETDESCCNV_XMLTABULATEDPROPERTYCNV_H 1

// Include files
#include "DetDescCnv/XmlGenericCnv.h"
#include "DetDesc/TabulatedProperty.h"

// Forward declarations
template <class TYPE> class CnvFactory;


/** @class XmlTabulatedPropertyCnv
 *
 * Xml converter for TabulatedProperties
 *
 * @author Sebastien Ponce
 * @author  Vanya Belyaev
 */
class XmlTabulatedPropertyCnv : public XmlGenericCnv {

 public:

  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID() { return TabulatedProperty::classID(); }

  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlTabulatedPropertyCnv (ISvcLocator* svcs);

  /**
   * Default destructor
   */
  virtual ~XmlTabulatedPropertyCnv();

 protected:

  /** Creates the transient representation of an object from a DOMElement.
   * Overrides the default method in XmlGenericCnv
   * @param element the DOMElement to be used to builds the object
   * @param refpObject the object to be built
   * @return status depending on the completion of the call
   */
  StatusCode i_createObj (xercesc::DOMElement* element,
                          DataObject*& refpObject) override;

  /** This fills the current object for its child childElement.
   * Overrides the default method in XmlGenericCnv
   * @param childElement the DOMElement that addr represents and from which we
   * will get the informations to creates new addresses
   * @param refpObject the object to be built
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillObj (xercesc::DOMElement* childElement,
                        DataObject* refpObject,
                        IOpaqueAddress* address) override;

  /** This fills the current object for its child text node childText.
   * Overrides the default method in XmlGenericCnv
   * @param childText the child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillObj (xercesc::DOMText* childText,
                        DataObject* refpObject,
                        IOpaqueAddress* address) override;

  /** This processes the current object.
   * Overrides the default method in XmlGenericCnv
   * @param refpObject the object to be processed
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_processObj (DataObject* refpObject,
                           IOpaqueAddress* address) override;

 private:

  /// user defined unit for the x axe
  double m_xunit;

  /// user defined unit for the y axe
  double m_yunit;

private:

  // Constant strings for element and parameter names
  const XMLCh* nameString;
  const XMLCh* typeString;
  const XMLCh* xaxisString;
  const XMLCh* yaxisString;
  const XMLCh* xunitString;
  const XMLCh* yunitString;
  const XMLCh* entryString;
  const XMLCh* xString;
  const XMLCh* yString;

};


#endif   // DETDESCCNV_XMLTABULATEDPROPERTYCNV_H
