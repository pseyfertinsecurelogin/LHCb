// include files
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/util/XMLString.hpp>
#include <functional>
#include <map>
#include <string>
#include <utility>

#include "DetDesc/Isotope.h"
#include "DetDesc/Material.h"
#include "DetDesc/TabulatedProperty.h"
#include "GaudiKernel/Converter.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/SmartRef.h"
#include "XmlIsotopeCnv.h"
#include "XmlTools/IXmlSvc.h"

class DataObject;
class IOpaqueAddress;
class ISvcLocator;


// -----------------------------------------------------------------------
// Instantiation of a static factory class used by clients to create
// instances of this service
// -----------------------------------------------------------------------
DECLARE_CONVERTER_FACTORY(XmlIsotopeCnv)
// -----------------------------------------------------------------------
// Material state string to state enumeration map
// -----------------------------------------------------------------------
typedef std::map< std::string, eState, std::less<std::string> > Str2StateMap;
static Str2StateMap ssMap;


// -----------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
XmlIsotopeCnv::XmlIsotopeCnv (ISvcLocator* svc) :
  XmlGenericCnv (svc, CLID_Isotope) {
  // Initializes State map
  if (0 == ssMap.size()) {
    ssMap.insert (Str2StateMap::value_type
                  (std::string ("undefined"), stateUndefined));
    ssMap.insert (Str2StateMap::value_type
                  (std::string("solid"), stateSolid));
    ssMap.insert (Str2StateMap::value_type
                  (std::string("liquid"), stateLiquid));
    ssMap.insert (Str2StateMap::value_type
                  (std::string("gas"), stateGas));
  }

  nameString = xercesc::XMLString::transcode("name");
  temperatureString = xercesc::XMLString::transcode("temperature");
  pressureString = xercesc::XMLString::transcode("pressure");
  stateString = xercesc::XMLString::transcode("state");
  AString = xercesc::XMLString::transcode("A");
  ZString = xercesc::XMLString::transcode("Z");
  IString = xercesc::XMLString::transcode("I");
  densityString = xercesc::XMLString::transcode("density");
  radlenString = xercesc::XMLString::transcode("radlen");
  lambdaString = xercesc::XMLString::transcode("lambda");
  tabpropsString = xercesc::XMLString::transcode("tabprops");
  addressString = xercesc::XMLString::transcode("address");
  
}


// -----------------------------------------------------------------------
// Desctructor
// ------------------------------------------------------------------------
XmlIsotopeCnv::~XmlIsotopeCnv () {
  xercesc::XMLString::release((XMLCh**)&nameString);
  xercesc::XMLString::release((XMLCh**)&temperatureString);
  xercesc::XMLString::release((XMLCh**)&pressureString);
  xercesc::XMLString::release((XMLCh**)&stateString);
  xercesc::XMLString::release((XMLCh**)&AString);
  xercesc::XMLString::release((XMLCh**)&ZString); 
  xercesc::XMLString::release((XMLCh**)&IString);
  xercesc::XMLString::release((XMLCh**)&densityString);
  xercesc::XMLString::release((XMLCh**)&radlenString);
  xercesc::XMLString::release((XMLCh**)&lambdaString);
  xercesc::XMLString::release((XMLCh**)&tabpropsString);
  xercesc::XMLString::release((XMLCh**)&addressString);   
 
}


// -----------------------------------------------------------------------
// Create an object corresponding to a DOM element
// -----------------------------------------------------------------------
StatusCode XmlIsotopeCnv::i_createObj (xercesc::DOMElement* element,
                                       DataObject*& refpObject) {
  // creates an object for the node found
  std::string elementName = dom2Std (element->getAttribute (nameString));
  Isotope* dataObj = new Isotope (elementName);
  refpObject = dataObj;
  
  // Now we have to process more material attributes if any
  std::string temperatureAttribute =
    dom2Std (element->getAttribute (temperatureString));
  if (!temperatureAttribute.empty()) {
    dataObj->setTemperature (xmlSvc()->eval(temperatureAttribute));
  }
  std::string pressureAttribute =
    dom2Std (element->getAttribute (pressureString));
  if (!pressureAttribute.empty()) {
    dataObj->setPressure (xmlSvc()->eval(pressureAttribute));
  }
  std::string stateAttribute = dom2Std (element->getAttribute (stateString));
  if (!stateAttribute.empty()) {
    dataObj->setState (ssMap[stateAttribute]);
  }
  std::string aAttribute = dom2Std (element->getAttribute ( AString ));
  if (!aAttribute.empty()) {
    dataObj->setA (xmlSvc()->eval(aAttribute));
  } 

  std::string iAttribute = dom2Std (element->getAttribute ( IString ));
  if (!iAttribute.empty()) 
      dataObj->setI (xmlSvc()->eval(iAttribute));
     
  std::string zAttribute = dom2Std (element->getAttribute ( ZString ));
  if (!zAttribute.empty()) {
    dataObj->setZ (xmlSvc()->eval(zAttribute, false));
  }
  std::string densityAttribute =
    dom2Std (element->getAttribute (densityString));
  if (!densityAttribute.empty()) {
    dataObj->setDensity (xmlSvc()->eval(densityAttribute));
  }
  std::string radlenAttribute = dom2Std (element->getAttribute (radlenString));
  if (!radlenAttribute.empty()) {
    dataObj->setRadiationLength (xmlSvc()->eval(radlenAttribute));
  }
  std::string lambdaAttribute = dom2Std (element->getAttribute (lambdaString));
  if (!lambdaAttribute.empty()) {
    dataObj->setAbsorptionLength (xmlSvc()->eval(lambdaAttribute));
  }

  

  // returns
  return StatusCode::SUCCESS;
} // end i_createObj


// -----------------------------------------------------------------------
// Fill an object with a new child element
// -----------------------------------------------------------------------
StatusCode XmlIsotopeCnv::i_fillObj (xercesc::DOMElement* childElement,
                                     DataObject* refpObject,
                                     IOpaqueAddress* /* address     */ ) 
{
  MsgStream log(msgSvc(), "XmlElementCnv" );
  
  // gets the object
  Isotope* dataObj = static_cast<Isotope*> (refpObject);
  // gets the element's name
  const XMLCh* tagName = childElement->getNodeName();
  // dispatches, based on the name
  if (0 == xercesc::XMLString::compareString(tabpropsString, tagName)) {
    // if we have a tabprops element, adds it to the current object
    const std::string address =
      dom2Std (childElement->getAttribute(addressString));
    long linkID = dataObj->linkMgr()->addLink(address, nullptr);
    SmartRef<TabulatedProperty> ref(dataObj, linkID);
    dataObj->tabulatedProperties().push_back(ref); 
  } else {
    // Something goes wrong, does it?
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "This tag makes no sense to isotope: "
        << tagNameString << endmsg;
    xercesc::XMLString::release(&tagNameString);
  }

  // returns
  return StatusCode::SUCCESS;
} // end i_fillObj

// ============================================================================
// End 
// ============================================================================
