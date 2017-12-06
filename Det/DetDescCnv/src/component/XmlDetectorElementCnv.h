#ifndef DETDESCCNV_XMLCNVSVC_XMLDECNV_H
#define DETDESCCNV_XMLCNVSVC_XMLDECNV_H

/// Include files
#include "DetDescCnv/XmlBaseDetElemCnv.h"


/** @class XmlDetectorElementCnv
 *
 * XML converter for DetectorElements.
 *
 * @author Sebastien Ponce
 * @author Radovan Chytracek
 * @author Pere Mato
 */
class XmlDetectorElementCnv : public XmlBaseDetElemCnv {

public:
  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlDetectorElementCnv(ISvcLocator* svcs);

protected:
  /** This fills the current object for specific child.
   * Implements the abstract method in XmlBaseDetElemCnv
   * @param childElement the specific child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillSpecificObj (xercesc::DOMElement* childElement,
                                DetectorElement* refpObject,
                                IOpaqueAddress* address) override;

};

#endif // DETDESCCNV_XMLDECNV_H
