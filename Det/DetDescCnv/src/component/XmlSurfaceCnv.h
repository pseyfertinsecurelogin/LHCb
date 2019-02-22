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
#ifndef DETDESCCNV_XMLSurfaceCNV_H
#define DETDESCCNV_XMLSurfaceCNV_H 1

// Include files
#include "DetDesc/Surface.h"
#include "DetDescCnv/XmlGenericCnv.h"

// Forward declarations
template <class TYPE>
class CnvFactory;

/** @class XmlSurfaceCnv
 *
 *  XML converter for Surfaces
 *
 *  @author  Sebastien Ponce
 *  @author  Vanya Belyaev
 */
class XmlSurfaceCnv : public XmlGenericCnv {

public:
  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID() { return Surface::classID(); }

  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlSurfaceCnv( ISvcLocator* svcs );

  /**
   * Default destructor
   */
  virtual ~XmlSurfaceCnv();

protected:
  /** Creates the transient representation of an object from a DOMElement.
   * Overrides the default method in XmlGenericCnv
   * @param element the DOMElement to be used to builds the object
   * @param refpObject the object to be built
   * @return status depending on the completion of the call
   */
  StatusCode i_createObj( xercesc::DOMElement* element, DataObject*& refpObject ) override;

  using XmlGenericCnv::i_fillObj;
  /** Fills the current object for its child element childElement.
   * Overrides the default method in XmlGenericCnv
   * @param childElement the child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillObj( xercesc::DOMElement* childElement, DataObject* refpObject, IOpaqueAddress* address ) override;

private:
  // Constant strings for element and parameter names
  const XMLCh* nameString;
  const XMLCh* modelString;
  const XMLCh* finishString;
  const XMLCh* typeString;
  const XMLCh* valueString;
  const XMLCh* volfirstString;
  const XMLCh* volsecondString;
  const XMLCh* tabpropsString;
  const XMLCh* addressString;
};

#endif // DETDESCCNV_XMLSurfaceCNV_H
