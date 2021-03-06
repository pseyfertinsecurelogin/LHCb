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
#ifndef DETDESCCNV_XMLCNVSVC_XMLELEMENTCNV_H
#define DETDESCCNV_XMLCNVSVC_XMLELEMENTCNV_H

// Include files
#include "DetDesc/Element.h"
#include "DetDescCnv/XmlGenericCnv.h"

// Forward declarations
class Isotope;
class ISvcLocator;
template <class TYPE>
class CnvFactory;

/** @class XmlElementCnv
 *
 * XML converter for Elements
 *
 * @author Sebastien Ponce
 * @author Radovan Chytracek
 * @author Pere Mato
 */
class XmlElementCnv : public XmlGenericCnv {
public:
  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID() { return CLID_Element; }

  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlElementCnv( ISvcLocator* svcs );

  /**
   * Default destructor
   */
  virtual ~XmlElementCnv();

protected:
  /** Creates the transient representation of an object from a DOMElement.
   *  Overrides the default method in XmlGenericCnv
   *  @param element the DOMElement to be used to builds the object
   *  @param refpObject the object to be built
   *  @return status depending on the completion of the call
   */
  StatusCode i_createObj( xercesc::DOMElement* element, DataObject*& refpObject ) override;

  using XmlGenericCnv::i_fillObj;
  /** Fills the current object for its child element childElement.
   *  Overrides the default method in XmlGenericCnv
   *  @param childElement the child processed here
   *  @param refpObject the object to be filled
   *  @param address the address for this object
   *  @return status depending on the completion of the call
   */
  StatusCode i_fillObj( xercesc::DOMElement* childElement, DataObject* refpObject, IOpaqueAddress* address ) override;

  /** This processes the current object.
   *  Overrides the default method in XmlGenericCnv
   *  @param refpObject the object to be processed
   *  @param address the address for this object
   *  @return status depending on the completion of the call
   */
  StatusCode i_processObj( DataObject* refpObject, IOpaqueAddress* address ) override;

private:
  // Constant strings for element and parameter names
  const XMLCh* nameString;
  const XMLCh* temperatureString;
  const XMLCh* pressureString;
  const XMLCh* stateString;
  const XMLCh* densityString;
  const XMLCh* radlenString;
  const XMLCh* lambdaString;
  const XMLCh* symbolString;
  const XMLCh* AString;
  const XMLCh* ZeffString;
  const XMLCh* IString;
  const XMLCh* hrefString;
  const XMLCh* fractionmassString;
  const XMLCh* tabpropsString;
  const XMLCh* addressString;
  const XMLCh* isotoperefString;
  const XMLCh* atomString;
};

// ============================================================================
// End
// ============================================================================
#endif // DETDESCCNV_XMLCNVSVC_XMLELEMENTCNV_H
