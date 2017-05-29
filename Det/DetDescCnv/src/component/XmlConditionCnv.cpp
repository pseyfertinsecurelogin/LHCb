//  $ID: $

#include "GaudiKernel/Converter.h"
#include "XmlConditionCnv.h"

class Condition;
class IOpaqueAddress;
class ISvcLocator;
namespace xercesc_3_1 {
class DOMElement;
}  // namespace xercesc_3_1


// -----------------------------------------------------------------------
// Instantiation of a static factory class used by clients to create
// instances of this service
// -----------------------------------------------------------------------
DECLARE_CONVERTER_FACTORY(XmlConditionCnv)

// -----------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
XmlConditionCnv::XmlConditionCnv (ISvcLocator* svc) :
  XmlBaseConditionCnv (svc) {
}


// -----------------------------------------------------------------------
// Fill an object with a new specific child
// -----------------------------------------------------------------------
StatusCode
XmlConditionCnv::i_fillSpecificObj (xercesc::DOMElement* /*childElement*/,
                                    Condition* /*refpObject*/,
                                    IOpaqueAddress* /*address*/) {
  // default implementation : ignore child
  return StatusCode::SUCCESS;
} // end i_fillObj


