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
// Include files
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/LinkManager.h"
#include "GaudiKernel/MsgStream.h"

#include "DetDesc/LogVolBase.h"
#include "DetDesc/LVolume.h"
#include "DetDesc/LAssembly.h"
#include "DetDesc/Surface.h"

#include "DetDescCnv/XmlCnvException.h"

#include <numeric>
#include <sstream>
#include <cstdlib>
#include <float.h>
#include <map>
#include <optional>

#include <xercesc/dom/DOMNodeList.hpp>
#include "XmlLVolumeCnv.h"
#include  <boost/algorithm/string/replace.hpp>

namespace {
  void replaceTagInString ( std::string& s, const std::string& numeral ) {
    boost::algorithm::replace_first(s,"-KEY-",numeral);
  }

  template <typename Type>
  bool is_any_of(const XMLCh* tag, std::initializer_list<Type> c) {
    return std::any_of( begin(c), end(c),
                        [&](const auto& i)
                        { return 0 == xercesc::XMLString::compareString(i,tag); } );
  }
}
// -----------------------------------------------------------------------
// Instantiation of a static factory class used by clients to create
// instances of this service
// -----------------------------------------------------------------------
DECLARE_CONVERTER( XmlLVolumeCnv )

// -----------------------------------------------------------------------
// Constructor
// ------------------------------------------------------------------------
XmlLVolumeCnv::XmlLVolumeCnv (ISvcLocator* svc) :
  XmlGenericCnv (svc, CLID_LVolume) {
  unionString = xercesc::XMLString::transcode("union");
  subtractionString = xercesc::XMLString::transcode("subtraction");
  intersectionString = xercesc::XMLString::transcode("intersection");
  boxString = xercesc::XMLString::transcode("box");
  trdString = xercesc::XMLString::transcode("trd");
  trapString = xercesc::XMLString::transcode("trap");
  consString = xercesc::XMLString::transcode("cons");
  polyconeString = xercesc::XMLString::transcode("polycone");
  tubsString = xercesc::XMLString::transcode("tubs");
  sphereString = xercesc::XMLString::transcode("sphere");
  transformationString = xercesc::XMLString::transcode("transformation");
  posXYZString = xercesc::XMLString::transcode("posXYZ");
  posRPhiZString = xercesc::XMLString::transcode("posRPhiZ");
  posRThPhiString = xercesc::XMLString::transcode("posRThPhi");
  rotXYZString = xercesc::XMLString::transcode("rotXYZ");
  rotAxisString = xercesc::XMLString::transcode("rotAxis");
  paramphysvolString = xercesc::XMLString::transcode("paramphysvol");
  paramphysvol2DString = xercesc::XMLString::transcode("paramphysvol2D");
  paramphysvol3DString = xercesc::XMLString::transcode("paramphysvol3D");
  materialString = xercesc::XMLString::transcode("material");
  magfieldString = xercesc::XMLString::transcode("magfield");
  sensdetString = xercesc::XMLString::transcode("sensdet");
  nameString = xercesc::XMLString::transcode("name");
  physvolString = xercesc::XMLString::transcode("physvol");
  surfString = xercesc::XMLString::transcode("surf");
  logvolString = xercesc::XMLString::transcode("logvol");
  numberString = xercesc::XMLString::transcode("number");
  addressString = xercesc::XMLString::transcode("address");
  outerRadiusMZString = xercesc::XMLString::transcode("outerRadiusMZ");
  outerRadiusPZString = xercesc::XMLString::transcode("outerRadiusPZ");
  innerRadiusMZString = xercesc::XMLString::transcode("innerRadiusMZ");
  innerRadiusPZString = xercesc::XMLString::transcode("innerRadiusPZ");
  outerRadiusString = xercesc::XMLString::transcode("outerRadius");
  innerRadiusString = xercesc::XMLString::transcode("innerRadius");
  sizeXString = xercesc::XMLString::transcode("sizeX");
  sizeX1String = xercesc::XMLString::transcode("sizeX1");
  sizeX2String = xercesc::XMLString::transcode("sizeX2");
  sizeX3String = xercesc::XMLString::transcode("sizeX3");
  sizeX4String = xercesc::XMLString::transcode("sizeX4");
  sizeYString = xercesc::XMLString::transcode("sizeY");
  sizeY1String = xercesc::XMLString::transcode("sizeY1");
  sizeY2String = xercesc::XMLString::transcode("sizeY2");
  sizeZString = xercesc::XMLString::transcode("sizeZ");
  thetaString = xercesc::XMLString::transcode("theta");
  phiString = xercesc::XMLString::transcode("phi");
  alp1String = xercesc::XMLString::transcode("alp1");
  alp2String = xercesc::XMLString::transcode("alp2");
  xString = xercesc::XMLString::transcode("x");
  yString = xercesc::XMLString::transcode("y");
  zString = xercesc::XMLString::transcode("z");
  rString = xercesc::XMLString::transcode("r");
  axThetaString = xercesc::XMLString::transcode("axTheta");
  axPhiString = xercesc::XMLString::transcode("axPhi");
  angleString = xercesc::XMLString::transcode("angle");
  zplaneString = xercesc::XMLString::transcode("zplane");
  startThetaAngleString = xercesc::XMLString::transcode("startThetaAngle");
  deltaThetaAngleString = xercesc::XMLString::transcode("deltaThetaAngle");
  startPhiAngleString = xercesc::XMLString::transcode("startPhiAngle");
  deltaPhiAngleString = xercesc::XMLString::transcode("deltaPhiAngle");
  rotXString = xercesc::XMLString::transcode("rotX");
  rotYString = xercesc::XMLString::transcode("rotY");
  rotZString = xercesc::XMLString::transcode("rotZ");
  serialNumber = xercesc::XMLString::transcode("SerialNumber");
}


// -----------------------------------------------------------------------
// Destructor
// ------------------------------------------------------------------------
XmlLVolumeCnv::~XmlLVolumeCnv () {
  xercesc::XMLString::release((XMLCh**)&unionString);
  xercesc::XMLString::release((XMLCh**)&subtractionString);
  xercesc::XMLString::release((XMLCh**)&intersectionString);
  xercesc::XMLString::release((XMLCh**)&boxString);
  xercesc::XMLString::release((XMLCh**)&trdString);
  xercesc::XMLString::release((XMLCh**)&trapString);
  xercesc::XMLString::release((XMLCh**)&consString);
  xercesc::XMLString::release((XMLCh**)&polyconeString);
  xercesc::XMLString::release((XMLCh**)&tubsString);
  xercesc::XMLString::release((XMLCh**)&sphereString);
  xercesc::XMLString::release((XMLCh**)&transformationString);
  xercesc::XMLString::release((XMLCh**)&posXYZString);
  xercesc::XMLString::release((XMLCh**)&posRPhiZString);
  xercesc::XMLString::release((XMLCh**)&posRThPhiString);
  xercesc::XMLString::release((XMLCh**)&rotXYZString);
  xercesc::XMLString::release((XMLCh**)&rotAxisString);
  xercesc::XMLString::release((XMLCh**)&paramphysvolString);
  xercesc::XMLString::release((XMLCh**)&paramphysvol2DString);
  xercesc::XMLString::release((XMLCh**)&paramphysvol3DString);
  xercesc::XMLString::release((XMLCh**)&materialString);
  xercesc::XMLString::release((XMLCh**)&magfieldString);
  xercesc::XMLString::release((XMLCh**)&sensdetString);
  xercesc::XMLString::release((XMLCh**)&nameString);
  xercesc::XMLString::release((XMLCh**)&physvolString);
  xercesc::XMLString::release((XMLCh**)&surfString);
  xercesc::XMLString::release((XMLCh**)&logvolString);
  xercesc::XMLString::release((XMLCh**)&numberString);
  xercesc::XMLString::release((XMLCh**)&addressString);
  xercesc::XMLString::release((XMLCh**)&outerRadiusMZString);
  xercesc::XMLString::release((XMLCh**)&outerRadiusPZString);
  xercesc::XMLString::release((XMLCh**)&innerRadiusMZString);
  xercesc::XMLString::release((XMLCh**)&innerRadiusPZString);
  xercesc::XMLString::release((XMLCh**)&outerRadiusString);
  xercesc::XMLString::release((XMLCh**)&innerRadiusString);
  xercesc::XMLString::release((XMLCh**)&sizeXString);
  xercesc::XMLString::release((XMLCh**)&sizeX1String);
  xercesc::XMLString::release((XMLCh**)&sizeX2String);
  xercesc::XMLString::release((XMLCh**)&sizeX3String);
  xercesc::XMLString::release((XMLCh**)&sizeX4String);
  xercesc::XMLString::release((XMLCh**)&sizeYString);
  xercesc::XMLString::release((XMLCh**)&sizeY1String);
  xercesc::XMLString::release((XMLCh**)&sizeY2String);
  xercesc::XMLString::release((XMLCh**)&sizeZString);
  xercesc::XMLString::release((XMLCh**)&thetaString);
  xercesc::XMLString::release((XMLCh**)&phiString);
  xercesc::XMLString::release((XMLCh**)&alp1String);
  xercesc::XMLString::release((XMLCh**)&alp2String);
  xercesc::XMLString::release((XMLCh**)&xString);
  xercesc::XMLString::release((XMLCh**)&yString);
  xercesc::XMLString::release((XMLCh**)&zString);
  xercesc::XMLString::release((XMLCh**)&rString);
  xercesc::XMLString::release((XMLCh**)&axThetaString);
  xercesc::XMLString::release((XMLCh**)&axPhiString);
  xercesc::XMLString::release((XMLCh**)&angleString);
  xercesc::XMLString::release((XMLCh**)&zplaneString);
  xercesc::XMLString::release((XMLCh**)&startThetaAngleString);
  xercesc::XMLString::release((XMLCh**)&deltaThetaAngleString);
  xercesc::XMLString::release((XMLCh**)&startPhiAngleString);
  xercesc::XMLString::release((XMLCh**)&deltaPhiAngleString);
  xercesc::XMLString::release((XMLCh**)&rotXString);
  xercesc::XMLString::release((XMLCh**)&rotYString);
  xercesc::XMLString::release((XMLCh**)&rotZString);
  xercesc::XMLString::release((XMLCh**)&serialNumber);
}


// -----------------------------------------------------------------------
// Tests whether the tag denotes a boolean solid
// -----------------------------------------------------------------------
bool XmlLVolumeCnv::isBooleanSolid (const XMLCh* tag) const {
  return is_any_of( tag, { std::cref(unionString),
                           std::cref(subtractionString),
                           std::cref(intersectionString) } );
} // end isBooleanSolid


// -----------------------------------------------------------------------
// Tests whether the tag denotes a simple solid
// -----------------------------------------------------------------------
bool XmlLVolumeCnv::isSimpleSolid (const XMLCh* tag) const {
  return is_any_of( tag, { std::cref(boxString), std::cref(trdString),
                           std::cref(trapString), std::cref(consString),
                           std::cref(polyconeString), std::cref(tubsString),
                           std::cref(sphereString) } );
} // end isSimpleSolid


// -----------------------------------------------------------------------
// Tests whether the tag denotes a solid
// -----------------------------------------------------------------------
bool XmlLVolumeCnv::isSolid (const XMLCh* tag) const {
  return isSimpleSolid(tag) || isBooleanSolid(tag);
} // end isSolid


// -----------------------------------------------------------------------
// Tests whether the tag denotes a transformation
// -----------------------------------------------------------------------
bool XmlLVolumeCnv::isTransformation (const XMLCh* tag) const {
  return is_any_of( tag, { std::cref(transformationString), std::cref(posXYZString),
                           std::cref(posRPhiZString), std::cref(posRThPhiString),
                           std::cref(rotXYZString), std::cref(rotAxisString) } );
} // end isTransformation


// -----------------------------------------------------------------------
// Tests whether the tag denotes a parametrized physical volume
// -----------------------------------------------------------------------
bool XmlLVolumeCnv::isParamphysvol (const XMLCh* tag) const {
  return is_any_of(tag, { std::cref(paramphysvolString),
                          std::cref(paramphysvol2DString),
                          std::cref(paramphysvol3DString) } );
} // end isTransformation


// -----------------------------------------------------------------------
// returns a string locating the element
// -----------------------------------------------------------------------
std::string XmlLVolumeCnv::locateElement (const xercesc::DOMElement* element) const {
  // tries to find a parent element with a name
  const xercesc::DOMNode* parentNode = element;
  xercesc::DOMElement* parentElement = nullptr;
  std::string parentName;
  bool hasName = false;
  while (!hasName) {
    parentNode = parentNode->getParentNode();
    if (!parentNode) break;
    if (parentNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
      parentElement = (xercesc::DOMElement*) parentNode;
      parentName = dom2Std (parentElement->getAttribute(nameString));
      if (!parentName.empty()) hasName = true;
    }
  }
  // if one was found, builds the result from it
  std::string result;
  if (hasName) {
    parentNode = element;
    xercesc::DOMNode* grandParent = element->getParentNode();
    while (parentNode != parentElement) {
      xercesc::DOMNodeList* list = grandParent->getChildNodes();
      unsigned int i = 0;
      for (; i < list->getLength(); i++) {
        if (parentNode == list->item(i)) break;
      }

      result = "child number ";
      result += std::to_string(i+1);
      result += " (";
      result += dom2Std (parentNode->getNodeName());
      result += ") of ";
      parentNode = grandParent;
      grandParent = parentNode->getParentNode();
    }
    result += dom2Std (parentElement->getNodeName());
    result += " ";
    result += parentName;
  } else {
    // else just give the name of the parent
    parentNode = element->getParentNode();
    if (parentNode != 0) {
      result += "tag ";
      result += dom2Std (parentNode->getNodeName());
    } else {
      result += "top node";
    }
  }
  return result;
} // end locateElement


// -----------------------------------------------------------------------
// Create the name of a physical volume
// -----------------------------------------------------------------------
std::string XmlLVolumeCnv::createPvName (const PVolumeItem& pv) const {
  // builds the actual name of the volume
  return pv.indexed ? pv.physvolName + ":" + std::to_string(pv.tag)
                    : pv.physvolName;
}

// -----------------------------------------------------------------------
// Create an object corresponding to a DOM element
// -----------------------------------------------------------------------
StatusCode XmlLVolumeCnv::internalCreateObj (xercesc::DOMElement* element,
                                             DataObject*&       refpObject ,
                                             IOpaqueAddress* /* address */ )
{
  // gets the attributes
  std::string materialName = dom2Std (element->getAttribute (materialString));
  std::string magFieldName = dom2Std (element->getAttribute (magfieldString));
  std::string sensDetName = dom2Std (element->getAttribute (sensdetString));
  std::string volName = dom2Std (element->getAttribute (nameString));

  std::string numeral = dom2Std (element->getAttribute ( serialNumber ) );
  if ( 0 != numeral.size() ) {
    replaceTagInString( volName, numeral );
    replaceTagInString( materialName, numeral );
  }

  // processes the children. The dtd says we should find
  // ((%solid;, %transformation;?)?, (physvol | paramphysvol | surf)*)
  xercesc::DOMNodeList* childNodes = element->getChildNodes();
  // get the first one, it should be a solid, a physical volume, a
  // parametrized physical volume, a surface or nothing
  unsigned int i = 0;
  while (i < childNodes->getLength() &&
         childNodes->item(i)->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) {
    ++i;
  }

  // if no child, return
  if (i >= childNodes->getLength()) return StatusCode::SUCCESS;

  // if there is a child, gets it as an element and gets its name
  xercesc::DOMNode* childNode = childNodes->item(i);
  xercesc::DOMElement* childElement = (xercesc::DOMElement *) childNode;
  const XMLCh* tagName = childElement->getNodeName();


  // try to see if it is a solid and deal with it and a possible
  // transformation if yes
  std::unique_ptr<ISolid> solid;
  if (isSolid (tagName)) {
    // deal with the solid itself
    solid = dealWithSolid (childElement, numeral);
    // finds the next child
    ++i;
    while (i < childNodes->getLength() &&
           childNodes->item(i)->getNodeType() !=
           xercesc::DOMNode::ELEMENT_NODE) {
      ++i;
    }
    // if there is more children, try to see if it is a transformation
    if (i < childNodes->getLength()) {
      childNode = childNodes->item(i);
      childElement = (xercesc::DOMElement*) childNode;
      const XMLCh* childTagName = childElement->getNodeName();
      if (isTransformation(childTagName)) {
        // deal with the transformation itself
        auto transformation = dealWithTransformation(element, &i);
        // modifies the solid in consequence
        // TO BE IMPLEMENTED --- TODO
        MsgStream log(msgSvc(), "XmlLVolumeCnv" );
        char* tagNameString = xercesc::XMLString::transcode(tagName);
        log << MSG::WARNING
            << "In logvol " << volName << ", a transformation ("
            << childTagName << ") is applied to the first solid ("
            << tagNameString
            << "). This functionnality is not implemented yet. The "
            << " transformation will be ignored." << endmsg;
        xercesc::XMLString::release(&tagNameString);
        // finds the next child
        while (i < childNodes->getLength() &&
               childNodes->item(i)->getNodeType() !=
               xercesc::DOMNode::ELEMENT_NODE) {
          ++i;
        }
      }
    }
  }

  std::unique_ptr<LogVolBase> dataObj;
  if (solid) {
    // computes the actual material name
    if (materialName.empty() || materialName[0] != '/') {
      materialName.insert(0,"/dd/Materials/");
    }
    // if there is a solid, creates a logical volume and stores the solid inside
    dataObj = std::make_unique<LVolume>(volName,
                                        solid.release(), // TODO: make LVolume accept unique_ptr...
                                        materialName,
                                        sensDetName,
                                        magFieldName);
  } else {
    // if materialName was not null, display a warning that it will be ignored
    if (!materialName.empty()) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING << "The logical volume " << volName
          << " has no associated solid. Thus, the material that you "
          << "defined for it (" << materialName << ") will be ignored."
          << endmsg;
    }
    // create an Assembly volume
    dataObj = std::make_unique<LAssembly>(volName,
                                          sensDetName,
                                          magFieldName);
  }

  // Whatever we found up to now, the next child to process is at index i.
  // The dtd says that we will find (physvol | paramphysvol | surf)* now
  // So let's deal with it.
  while (i < childNodes->getLength()) {
    childNode = childNodes->item(i);
    childElement = (xercesc::DOMElement*) childNode;
    tagName = childElement->getNodeName();
    // dispatches according to the tagname
    if (0 == xercesc::XMLString::compareString(physvolString, tagName)) {
      // deals with a physical volume, adds it to the logical volume
      // and frees the memory
      auto volume = dealWithPhysvol(childElement, numeral);
      dataObj->createPVolume (createPvName(volume),
                              volume.logvolName,
                              volume.transformation ? volume.transformation->Inverse()
                                                    : Gaudi::Transform3D() );
    } else if (isParamphysvol (tagName)) {
      // deals with a parametrized physical volume, adds all physical
      // volumes created to the logical volume and frees the memory
      auto volumes = dealWithParamphysvol(childElement, numeral);
      for (const auto& vol : *volumes) {
        dataObj->createPVolume (createPvName (vol),
                                vol.logvolName,
                                vol.transformation ? vol.transformation->Inverse()
                                                   : Gaudi::Transform3D() ) ;
      }
    } else if (0 == xercesc::XMLString::compareString(surfString, tagName)) {
      // deals with a surface and adds it to the logical volume
      std::string address = dealWithSurf(childElement);
      long linkID = dataObj->linkMgr()->addLink(address, nullptr);
      dataObj->surfaces().emplace_back(dataObj.get(), linkID);
    } else {
      // Something goes wrong, does it?
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      char* tagNameString = xercesc::XMLString::transcode(tagName);
      log << MSG::WARNING
          << "This tag makes no sense to LVolume " << volName
          << " : " << tagNameString
          << ". It will be ignored" << endmsg;
      xercesc::XMLString::release(&tagNameString);
      return StatusCode::FAILURE;
    }

    //XmlLVolumeCnv:: gets the next child
    ++i;
    while (i < childNodes->getLength() &&
           childNodes->item(i)->getNodeType() !=
           xercesc::DOMNode::ELEMENT_NODE) {
      ++i;
    }
  }
  refpObject = dataObj.release();
  return StatusCode::SUCCESS;
} // end internalCreateObj


// -----------------------------------------------------------------------
// Deal with Physical volume
// -----------------------------------------------------------------------
XmlLVolumeCnv::PVolumeItem
XmlLVolumeCnv::dealWithPhysvol(const xercesc::DOMElement* element,
                               const std::string& numeral) const {
  // gets attributes
  std::string nameAttribute = dom2Std (element->getAttribute (nameString));
  std::string logvolAttribute = dom2Std (element->getAttribute (logvolString));

  // gets the children
  xercesc::DOMNodeList* childNodes = element->getChildNodes();

  // deal with a possible transformation inside element
  std::unique_ptr<Gaudi::Transform3D> transformation;
  // while there are children, and no transformation found, try
  // to find one
  unsigned int i = 0;
  while ((!transformation ||
          *transformation == Gaudi::Transform3D())
         && i < childNodes->getLength()) {
    while (i < childNodes->getLength() &&
           childNodes->item(i)->getNodeType() !=
           xercesc::DOMNode::ELEMENT_NODE) {
      ++i;
    }
    // is there is one, deal with the transformation
    if (i < childNodes->getLength()) {
      transformation = dealWithTransformation(element, &i);
    }
  }
  // if there are still children after the transformation,
  // raise an error message
  while (i < childNodes->getLength() &&
         childNodes->item(i)->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) {
    ++i;
  }
  if (i < childNodes->getLength()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING
        << "There are too many children in physical volume "
        << nameAttribute << ". The exceeding ones will be ignored."
        << endmsg;
  }

  // deals with the name of the physical volume
  // it could end with ':' and digits. In this case, the name itself is the
  // part before ':' and the digits give an index for this volume
  auto columnPos = nameAttribute.find_last_of (':');
  int index = 0;
  bool indexed = false;
  if (columnPos != std::string::npos) {
    std::string digits = nameAttribute.substr (columnPos + 1);
    nameAttribute = nameAttribute.substr (0, columnPos);
    indexed = true;
    if (!digits.empty()) {
      if ( 0 != numeral.size() ) {
        replaceTagInString(digits, numeral);
      }
      index = (int) xmlSvc()->eval(digits, false);
    }
  }

  // builds physvol and returns
  PVolumeItem result;
  result.physvolName = nameAttribute;
  result.tag = index;
  result.indexed = indexed;
  result.logvolName = logvolAttribute;
  result.transformation = std::move(transformation);
  if ( !numeral.empty() ) {
    replaceTagInString( result.logvolName, numeral );
    replaceTagInString( result.physvolName, numeral );
  }
  return result;
} // end dealWithPhysVol


// -----------------------------------------------------------------------
// Deal with parametrized physical volume
// -----------------------------------------------------------------------
std::unique_ptr<XmlLVolumeCnv::PVolumes>
XmlLVolumeCnv::dealWithParamphysvol(const xercesc::DOMElement* element,
                                    const std::string& numeral) const {
  // gets the element's name
  const XMLCh* tagName = element->getNodeName();
  // dispatches, based on the name
  if (0 == xercesc::XMLString::compareString(paramphysvolString, tagName)) {
    return dealWithParamphysvol (element, 1, numeral);
  } else if (0 == xercesc::XMLString::compareString
             (paramphysvol2DString, tagName)) {
    return dealWithParamphysvol (element, 2, numeral);
  } else if (0 == xercesc::XMLString::compareString
             (paramphysvol3DString, tagName)) {
    return dealWithParamphysvol (element, 3, numeral);
  } else {
    // unknown tag
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "In " << locateElement (element)
        << " : the tag " << tagNameString
        << " does no denote a parametrized physical volume. "
        << "It will be ignored" << endmsg;
    xercesc::XMLString::release(&tagNameString);
  }
  return nullptr;
} // end dealWithParamphysvol


// -----------------------------------------------------------------------
// Deal with parametrized physical volume
// -----------------------------------------------------------------------
std::unique_ptr<XmlLVolumeCnv::PVolumes>
XmlLVolumeCnv::dealWithParamphysvol(const xercesc::DOMElement* element,
                                    unsigned int nD, const std::string& numeral) const {
  // nD should be positive
  if (0 == nD) return nullptr;

  // gets attributes
  std::vector<std::string> numberAttributes(nD);
  if (1 == nD) {
    numberAttributes[0] = dom2Std (element->getAttribute (numberString));
  } else {
    for (unsigned int i = 0; i < nD; i++) {
      // builds the actual name of the attribute
      std::ostringstream ost;
      ost << "number" << i + 1;
      const unsigned int len = ost.str().size();
      auto resstr = std::make_unique<char[]>(len+1); //TODO: replace with string buffer outside loop
      strncpy(resstr.get(),ost.str().c_str(),len);
      resstr[len] = 0;
      const XMLCh* attrName = xercesc::XMLString::transcode (resstr.get());
      numberAttributes[i] = dom2Std (element->getAttribute (attrName));
      xercesc::XMLString::release((XMLCh**)&attrName);
    }
  }

  // computes the values
  std::vector<int> numbers(nD);
  {
    unsigned int i = 0;
    for (i = 0; i < nD; i++) {
      numbers[i] = (int) xmlSvc()->eval(numberAttributes[i], false);
    }
  }

  // some checks : numbers should be positive
  {
    unsigned int i = 0;
    for (i = 0; i < nD; i++) {
      if (numbers[i] < 0) {
        throw XmlCnvException
          (" paramphysvolume : number of copies must be positive ! ",
           CORRUPTED_DATA);
        return 0;
      }
    }
  }

  // builds the result
  std::unique_ptr<PVolumes> result;

  // deals with the children of the tag. The dtd says it should be
  // (physvol | paramphysvol), %transformation;*nD
  xercesc::DOMNodeList* childNodes = element->getChildNodes();
  // finds the first child that is an element
  unsigned int i = 0;
  while (i < childNodes->getLength() &&
         childNodes->item(i)->getNodeType() != xercesc::DOMNode::ELEMENT_NODE) {
    ++i;
  }

  // is there is one, it should be a physvol or a paramphysvol
  if (i < childNodes->getLength()) {
    xercesc::DOMNode* firstChildNode = childNodes->item(i);
    xercesc::DOMElement* firstChildElement =
      (xercesc::DOMElement*) firstChildNode;
    // get the name of the first child
    const XMLCh* tagName = firstChildElement->getNodeName();
    if (0 == xercesc::XMLString::compareString(physvolString, tagName)) {
      // gets the physical volume and add it to the result as the first
      // volume of the vector
      result = std::make_unique<PVolumes>();
      result->push_back( dealWithPhysvol (firstChildElement, numeral) );
    } else if (isParamphysvol (tagName)) {
      // gets the vector of physical volumes and use it as first result
      // that will be upgraded later for the current parametrisation
      result = dealWithParamphysvol (firstChildElement, numeral);
    } else {
      // unknown tag -> display an error, return 0
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      char* tagNameString = xercesc::XMLString::transcode(tagName);
      log << MSG::WARNING << "In " << locateElement (element)
          << " : the tag " << tagNameString
          << " is not a valid tag as first child of <paramphysvol";
      xercesc::XMLString::release(&tagNameString);
      if (0 != nD) {
        log << nD << "D";
      }
      log << ">. It should be either physvol or paramphysvol(nD). "
          << "As a consequence, "
          << "this parametrised physical volume will be ignored." << endmsg;
      return nullptr;
    }

    // now looks subsequent child, they will define the transformations
    std::vector<std::unique_ptr<Gaudi::Transform3D>> transformations; transformations.reserve(nD);

    // while there are still children, and not enough transformations found, try
    // to find more transformations
    unsigned int j = i + 1;
    while (transformations.size() < nD
           && j < childNodes->getLength()) {
      while (j < childNodes->getLength() &&
             childNodes->item(j)->getNodeType() !=
             xercesc::DOMNode::ELEMENT_NODE) {
        ++j;
      }
      // is there is one child, deal with the transformation
      if (j < childNodes->getLength()) {
        transformations.push_back( dealWithTransformation(element, &j) );
      }
    }

    // if there are still children after the transformation,
    // raise an error message
    while (j < childNodes->getLength() &&
           childNodes->item(j)->getNodeType()
           != xercesc::DOMNode::ELEMENT_NODE) {
      ++j;
    }
    if (j < childNodes->getLength()) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING << "In " << locateElement (element)
          << " : there are too many children in this parametrized physical "
          << "volume. The exceeding ones will be ignored."
          << endmsg;
    }

    // last but not least build the final result from the current one
    // by expansion of the vector using transformation and number
    result = expandParamPhysVol(std::move(result), numbers, transformations);

  } else {
    // no element child -> display an error, return 0
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << " : this <paramphysvol> cannot be empty, it should contain "
        << "either physvol or paramphysvol(nD). As a consequence, this "
        << "parametrised physical volume will be ignored." << endmsg;
    return nullptr;
  }

  // returns
  return result;
} // end dealWithParamphysvol


// -----------------------------------------------------------------------
// Expand parametrised physical volume
// -----------------------------------------------------------------------
std::unique_ptr<XmlLVolumeCnv::PVolumes>
XmlLVolumeCnv::expandParamPhysVol(std::unique_ptr<PVolumes> volumes,
                                  const std::vector<int>& numbers,
                                  const std::vector<std::unique_ptr<Gaudi::Transform3D>>& transformations) const {
  // gets the number of dimensions, check the arguments are ok
  unsigned int nD = numbers.size();
  if (transformations.size() != nD) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::FATAL
        << "In expandParamPhysVol, the dimensions of the arguments are "
        << "not compatible : I got " << nD << " numbers and "
        << transformations.size() << " transformations. "
        << "The parametrized physical volume will be ignored." << endmsg;
    return nullptr;
  }

  // get the number of volumes in volumes
  unsigned int volNumber = volumes->size();
  // computes the number of replication of a single volume
  unsigned int nV = std::accumulate( numbers.begin(), numbers.end(),
                                     1u, std::multiplies<unsigned int>());

  // get the translations and rotations to apply
  std::vector<std::pair<Gaudi::XYZVector,Gaudi::Rotation3D>> stepTransRot(nD);
  for (unsigned i = 0; i < nD; i++) {
    auto& p = stepTransRot[i];
    transformations[i]->GetDecomposition(p.second,p.first);
  }

  // creates the result
  auto result = std::make_unique<PVolumes>();
  // creates the set of translations and rotations that the volumes will use
  std::vector<Gaudi::XYZVector> translations(nV);
  std::vector<Gaudi::Rotation3D> rotations(nV);
  {
    unsigned int numberOfItems = 1;
    for (unsigned int dimension = 0; dimension < nD; dimension++) {
      const auto& transRot = stepTransRot[dimension];
      for (unsigned int item = 0; item < numberOfItems; item++) {
        for (int i = 1; i < numbers[dimension]; i++) {
          translations[numberOfItems*i+item] =
            transRot.first + translations[numberOfItems*(i-1)+item];
          rotations[numberOfItems*i+item] =
            transRot.second * rotations[numberOfItems*(i-1)+item];
        }
      }
      numberOfItems *= numbers[dimension];
    }
  }

  Gaudi::XYZVector tmpTrans;
  Gaudi::Rotation3D tmpRot;

  // for each volume
  for (auto& vol : *volumes) {
    // add the volume to the result
    result->push_back(std::move(vol));
    auto& mvol = result->back();

    // replicates the volume
    for (unsigned int step = 1; step < nV; ++step) {
      // first give it a tag
      unsigned int tag = volNumber * step + mvol.tag;
      // add a new physical volume to the result
      result->emplace_back();
      auto& nvol = result->back();
      nvol.physvolName = mvol.physvolName;
      nvol.tag = tag;
      nvol.indexed = true;
      nvol.logvolName = mvol.logvolName;
      if ( !mvol.transformation) {
        auto transformation = std::make_unique<Gaudi::Transform3D>(translations[step]);
        *transformation = *transformation * Gaudi::Transform3D(rotations[step] );
        nvol.transformation = std::move(transformation);
      } else {
        mvol.transformation->GetDecomposition(tmpRot, tmpTrans);
        auto transformation = std::make_unique<Gaudi::Transform3D>(translations[step] + tmpTrans );
        *transformation = *transformation * Gaudi::Transform3D( rotations[step]*tmpRot );
        nvol.transformation = std::move(transformation);
      }
    }
  }
  return result;
} // end expandParamphysvol


// -----------------------------------------------------------------------
// Deal with Surface
// -----------------------------------------------------------------------
std::string XmlLVolumeCnv::dealWithSurf(const xercesc::DOMElement* element) const {
  return dom2Std (element->getAttribute (addressString));
} // end dealWithSurf


// -----------------------------------------------------------------------
// Deal with solid
// -----------------------------------------------------------------------
std::unique_ptr<ISolid> XmlLVolumeCnv::dealWithSolid(const xercesc::DOMElement* element,
                                                     const std::string& numeral) const {
  // gets the element's name
  const XMLCh* tagName = element->getNodeName();
  // dispatches, based on the name
  if (isSimpleSolid (tagName)) {
    return dealWithSimpleSolid (element, numeral);
  } else if (isBooleanSolid(tagName)) {
    return dealWithBoolean (element, numeral);
  } else {
    // unknown tag
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "In " << locateElement (element)
        << " : this should be a Solid but is a : "
        << tagName << ". It will be ignored" << endmsg;
    xercesc::XMLString::release(&tagNameString);
  }
  return nullptr;
} // end dealWithSolid


// -----------------------------------------------------------------------
// Deal with boolean
// -----------------------------------------------------------------------
std::unique_ptr<SolidBoolean> XmlLVolumeCnv::dealWithBoolean(const xercesc::DOMElement* element,
                                                             const std::string& numeral) const {
  // gets the element's name
  std::string nameAttribute = dom2Std (element->getAttribute (nameString));
  const XMLCh* tagName = element->getNodeName();

  // Checks it's a boolean solid
  if (!isBooleanSolid (tagName)) {
    // unknow tag
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "In " << locateElement (element)
        << " : this sould be a boolean solid but is a : "
        << tagNameString << ". It will be ignored" << endmsg;
    xercesc::XMLString::release(&tagNameString);
    return nullptr;
  }

  // builds the list of the children
  auto solids = dealWithBooleanChildren (element, numeral);

  // checks that there are at least two solids
  if (solids->size() < 2) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << " : this boolean solid contains less than 2 solids. "
        << "It will be ignored" << endmsg;
    if (solids->size() > 0) {
      solids->pop_back();
    }
    return nullptr;
  }

  // creates the object
  std::unique_ptr<SolidBoolean> result;
  PlacedSolid placedSolid = std::move(solids->front());
  solids->pop_front();
  if (0 == xercesc::XMLString::compareString(unionString, tagName)) {
    auto unionResult = std::make_unique<SolidUnion>(nameAttribute, std::move(placedSolid.solid));
    // TO BE IMPLEMENTED -- TODO
    if (placedSolid.transformation) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING
          << "In union " << nameAttribute << ", a transformation"
          << " is applied to the first solid."
          << " This functionnality is not implemented yet. The "
          << " transformation will be ignored." << endmsg;
    }
    // add every child to the boolean solid
    while (!solids->empty()) {
      placedSolid = std::move(solids->front());
      solids->pop_front();
      StatusCode sc = unionResult->unite (std::move(placedSolid.solid),
                                          placedSolid.transformation.get());
      if( !sc.isSuccess() ) {
        MsgStream log(msgSvc(), "XmlLVolumeCnv" );
        log << MSG::WARNING
            << "unionResult->unite failed. Don't know what to do..." << endmsg;
      }
    }
    result = std::move(unionResult);
  } else if (0 == xercesc::XMLString::compareString
             (subtractionString, tagName)) {
    auto subtractionResult =
      std::make_unique<SolidSubtraction>(nameAttribute, std::move(placedSolid.solid));
    // TO BE IMPLEMENTED -- TODO
    if ( placedSolid.transformation) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING
          << "In subtraction " << nameAttribute << ", a transformation"
          << " is applied to the first solid."
          << " This functionnality is not implemented yet. The "
          << " transformation will be ignored." << endmsg;
    }
    // add every child to the boolean solid
    while (!solids->empty()) {
      placedSolid = std::move(solids->front());
      solids->pop_front();
      StatusCode sc = subtractionResult->subtract (std::move(placedSolid.solid),
                                                   placedSolid.transformation.get());
      if( !sc.isSuccess() ) {
        MsgStream log(msgSvc(), "XmlLVolumeCnv" );
        log << MSG::WARNING
            << "unionResult->subtract failed. Don't know what to do.." << endmsg;
      }
    }
    result = std::move(subtractionResult);
  } else if (0 == xercesc::XMLString::compareString
             (intersectionString, tagName)) {
    auto intersectionResult =
      std::make_unique<SolidIntersection>(nameAttribute, std::move(placedSolid.solid));
    // TO BE IMPLEMENTED -- TODO
    if ( placedSolid.transformation) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING
          << "In intersection " << nameAttribute << ", a transformation"
          << " is applied to the first solid."
          << " This functionnality is not implemented yet. The "
          << " transformation will be ignored." << endmsg;
    }
    // add every child to the boolean solid
    while (!solids->empty()) {
      placedSolid = std::move(solids->front());
      solids->pop_front();
      StatusCode sc = intersectionResult->intersect (std::move(placedSolid.solid),
                                                     placedSolid.transformation.get());
      if( !sc.isSuccess() ) {
        MsgStream log(msgSvc(), "XmlLVolumeCnv" );
        log << MSG::WARNING
            << "unionResult->intersect failed. Don't know what to do!" << endmsg;
      }
    }
    result = std::move(intersectionResult);
  }

  return result;
} // end dealWithBoolean


// -----------------------------------------------------------------------
// Deal with boolean children
// -----------------------------------------------------------------------
std::unique_ptr<XmlLVolumeCnv::PlacedSolidList>
XmlLVolumeCnv::dealWithBooleanChildren(const xercesc::DOMElement* element,
                                       const std::string& numeral) const {
  // builds an empty list
  auto result = std::make_unique<PlacedSolidList>();

  // the dtd says ((%solid; %transformation;?), (%solid; %transformation;?)+)
  // but we do here (%solid; %transformation;?)*
  xercesc::DOMNodeList* childNodes = element->getChildNodes();
  unsigned int i = 0;
  while (i < childNodes->getLength()) {
    if (childNodes->item(i)->getNodeType() == xercesc::DOMNode::ELEMENT_NODE) {
      // gets the current child
      xercesc::DOMNode* childNode = childNodes->item(i);
      xercesc::DOMElement* childElement = (xercesc::DOMElement*) childNode;
      // gets its name
      const XMLCh* tagName = childElement->getNodeName();
      // checks it's a solid
      if (isSolid (tagName)) {
        // get the C++ object from it
        auto solid = dealWithSolid(childElement, numeral);
        // see if there is a transformation afterwards
        std::unique_ptr<Gaudi::Transform3D> transformation;
        ++i;
        while (i < childNodes->getLength() &&
               childNodes->item(i)->getNodeType() !=
               xercesc::DOMNode::ELEMENT_NODE) {
          ++i;
        }
        if (i < childNodes->getLength()) {
          childNode = childNodes->item(i);
          childElement = (xercesc::DOMElement*) childNode;
          tagName = childElement->getNodeName();
          if (isTransformation (tagName)) {
            transformation =  dealWithTransformation (element, &i);
            transformation->Invert();
          }
        }
        result->push_back( PlacedSolid{std::move(solid), std::move(transformation)} );
      } else {
        // we should have a solid here
        MsgStream log(msgSvc(), "XmlLVolumeCnv" );
        char* tagNameString = xercesc::XMLString::transcode(tagName);
        log << MSG::WARNING << "In " << locateElement (childElement)
            << " : this " << tagName << " makes no sense to a boolean solid. "
            << "A solid is needed here. It will be ignored" << endmsg;
        xercesc::XMLString::release(&tagNameString);
        ++i;
      }
    } else {
      ++i;
    }
  }
  // returns
  return result;
} // end dealWithBooleanChildren


// -----------------------------------------------------------------------
// Deal with simple solid
// -----------------------------------------------------------------------
std::unique_ptr<ISolid> XmlLVolumeCnv::dealWithSimpleSolid(const xercesc::DOMElement* element,
                                                           const std::string& numeral) const {
  // gets the element's name
  const XMLCh* tagName = element->getNodeName();

  // dispatches, based on the name
  if (0 == xercesc::XMLString::compareString(boxString, tagName)) {
    return dealWithBox (element, numeral);
  } else if (0 == xercesc::XMLString::compareString(trdString, tagName)) {
    return dealWithTrd (element);
  } else if (0 == xercesc::XMLString::compareString(trapString, tagName)) {
    return dealWithTrap (element);
  } else if (0 == xercesc::XMLString::compareString(consString, tagName)) {
    return dealWithCons (element);
  } else if (0 == xercesc::XMLString::compareString(polyconeString, tagName)) {
    return dealWithPolycone (element);
  } else if (0 == xercesc::XMLString::compareString(tubsString, tagName)) {
    return dealWithTubs (element);
  } else if (0 == xercesc::XMLString::compareString(sphereString, tagName)) {
    return dealWithSphere (element);
  } else {
    // unknow tag
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "In " << locateElement (element)
        << " : this should be a SimpleSolid but is a : "
        << tagNameString << ". It will be ignored" << endmsg;
    xercesc::XMLString::release(&tagNameString);
  }
  return nullptr;
} // end dealWithSimpleSolid


// -----------------------------------------------------------------------
// Deal with box
// -----------------------------------------------------------------------
std::unique_ptr<SolidBox> XmlLVolumeCnv::dealWithBox(const xercesc::DOMElement* element,
                                                     const std::string& numeral) const {
  // gets attributes
  std::string sizeXAttribute = dom2Std (element->getAttribute (sizeXString));
  std::string sizeYAttribute = dom2Std (element->getAttribute (sizeYString));
  std::string sizeZAttribute = dom2Std (element->getAttribute (sizeZString));
  std::string solidName = dom2Std (element->getAttribute (nameString));
  if ( 0 != numeral.size() ) {
    replaceTagInString( solidName, numeral );
  }

  // computes the values
  double sizeX = 0.0;
  double sizeY = 0.0;
  double sizeZ = 0.0;
  if (!sizeXAttribute.empty()) {
    sizeX = xmlSvc()->eval(sizeXAttribute);
  }
  if (!sizeYAttribute.empty()) {
    sizeY = xmlSvc()->eval(sizeYAttribute);
  }
  if (!sizeZAttribute.empty()) {
    sizeZ = xmlSvc()->eval(sizeZAttribute);
  }

  // builds solid
  auto result = std::make_unique< SolidBox >
    (solidName, sizeX / 2.0, sizeY / 2.0, sizeZ / 2.0);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << " : a box should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithBox


// -----------------------------------------------------------------------
// Deal with trd
// -----------------------------------------------------------------------
std::unique_ptr<SolidTrd> XmlLVolumeCnv::dealWithTrd(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string sizeX1Attribute =
    dom2Std (element->getAttribute (sizeX1String));
  std::string sizeX2Attribute =
    dom2Std (element->getAttribute (sizeX2String));
  std::string sizeY1Attribute =
    dom2Std (element->getAttribute (sizeY1String));
  std::string sizeY2Attribute =
    dom2Std (element->getAttribute (sizeY2String));
  std::string sizeZAttribute =
    dom2Std (element->getAttribute (sizeZString));
  std::string trdName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double sizeX1 = 0.0;
  double sizeX2 = 0.0;
  double sizeY1 = 0.0;
  double sizeY2 = 0.0;
  double sizeZ = 0.0;
  if (!sizeX1Attribute.empty()) {
    sizeX1 = xmlSvc()->eval(sizeX1Attribute);
  }
  if (!sizeX2Attribute.empty()) {
    sizeX2 = xmlSvc()->eval(sizeX2Attribute);
  }
  if (!sizeY1Attribute.empty()) {
    sizeY1 = xmlSvc()->eval(sizeY1Attribute);
  }
  if (!sizeY2Attribute.empty()) {
    sizeY2 = xmlSvc()->eval(sizeY2Attribute);
  }
  if (!sizeZAttribute.empty()) {
    sizeZ = xmlSvc()->eval(sizeZAttribute);
  }

  // builds solid
  auto result = std::make_unique<SolidTrd>
    (trdName, sizeZ / 2.0, sizeX1 / 2.0, sizeY1 / 2.0, sizeX2 / 2.0,
     sizeY2 / 2.0);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << " : trd should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithTrd


// -----------------------------------------------------------------------
// Deal with trap
// -----------------------------------------------------------------------
std::unique_ptr<SolidTrap> XmlLVolumeCnv::dealWithTrap(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string sizeZAttribute =
    dom2Std (element->getAttribute (sizeZString));
  std::string thetaAttribute =
    dom2Std (element->getAttribute (thetaString));
  std::string phiAttribute =
    dom2Std (element->getAttribute (phiString));
  std::string sizeY1Attribute =
    dom2Std (element->getAttribute (sizeY1String));
  std::string sizeX1Attribute =
    dom2Std (element->getAttribute (sizeX1String));
  std::string sizeX2Attribute =
    dom2Std (element->getAttribute (sizeX2String));
  std::string alp1Attribute =
    dom2Std (element->getAttribute (alp1String));
  std::string sizeY2Attribute =
    dom2Std (element->getAttribute (sizeY2String));
  std::string sizeX3Attribute =
    dom2Std (element->getAttribute (sizeX3String));
  std::string sizeX4Attribute =
    dom2Std (element->getAttribute (sizeX4String));
  std::string alp2Attribute =
    dom2Std (element->getAttribute (alp2String));
  std::string trapName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double sizeZ = 0.0;
  double theta = 0.0;
  double phi = 0.0;
  double sizeY1 = 0.0;
  double sizeX1 = 0.0;
  double sizeX2 = 0.0;
  double alp1 = 0.0;
  double sizeY2 = 0.0;
  double sizeX3 = 0.0;
  double sizeX4 = 0.0;
  double alp2 = 0.0;
  if (!sizeZAttribute.empty()) {
    sizeZ = xmlSvc()->eval(sizeZAttribute);
  }
  if (!thetaAttribute.empty()) {
    theta = xmlSvc()->eval(thetaAttribute, false);
  }
  if (!phiAttribute.empty()) {
    phi = xmlSvc()->eval(phiAttribute, false);
  }
  if (!sizeY1Attribute.empty()) {
    sizeY1 = xmlSvc()->eval(sizeY1Attribute);
  }
  if (!sizeX1Attribute.empty()) {
    sizeX1 = xmlSvc()->eval(sizeX1Attribute);
  }
  if (!sizeX2Attribute.empty()) {
    sizeX2 = xmlSvc()->eval(sizeX2Attribute);
  }
  if (!alp1Attribute.empty()) {
    alp1 = xmlSvc()->eval(alp1Attribute, false);
  }
  if (!sizeY2Attribute.empty()) {
    sizeY2 = xmlSvc()->eval(sizeY2Attribute);
  }
  if (!sizeX3Attribute.empty()) {
    sizeX3 = xmlSvc()->eval(sizeX3Attribute);
  }
  if (!sizeX4Attribute.empty()) {
    sizeX4 = xmlSvc()->eval(sizeX4Attribute);
  }
  if (!alp2Attribute.empty()) {
    alp2 = xmlSvc()->eval(alp2Attribute, false);
  }

  // builds solid
  auto result = std::make_unique<SolidTrap>
    (trapName, sizeZ / 2.0, theta, phi, sizeY1 / 2.0, sizeX1 / 2.0,
     sizeX2 / 2.0, alp1, sizeY2 / 2.0, sizeX3 / 2.0, sizeX4 / 2.0,
     alp2);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << "A trap should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithTrap


// -----------------------------------------------------------------------
// Deal with cons
// -----------------------------------------------------------------------
std::unique_ptr<SolidCons> XmlLVolumeCnv::dealWithCons(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string sizeZAttribute =
    dom2Std (element->getAttribute (sizeZString));
  std::string outerRadiusPZAttribute =
    dom2Std (element->getAttribute (outerRadiusPZString));
  std::string outerRadiusMZAttribute =
    dom2Std (element->getAttribute (outerRadiusMZString));
  std::string innerRadiusPZAttribute =
    dom2Std (element->getAttribute (innerRadiusPZString));
  std::string innerRadiusMZAttribute =
    dom2Std (element->getAttribute (innerRadiusMZString));
  std::string startPhiAngleAttribute =
    dom2Std (element->getAttribute (startPhiAngleString));
  std::string deltaPhiAngleAttribute =
    dom2Std (element->getAttribute (deltaPhiAngleString));
  std::string consName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double sizeZ = 0.0;
  double outerRadiusPZ = 0.0;
  double outerRadiusMZ = 0.0;
  double innerRadiusPZ = 0.0;
  double innerRadiusMZ = 0.0;
  double startPhiAngle = 0.0;
  double deltaPhiAngle = 360.0 * Gaudi::Units::degree;
  if (!sizeZAttribute.empty()) {
    sizeZ = xmlSvc()->eval(sizeZAttribute);
  }
  if (!outerRadiusPZAttribute.empty()) {
    outerRadiusPZ = xmlSvc()->eval(outerRadiusPZAttribute);
  }
  if (!outerRadiusMZAttribute.empty()) {
    outerRadiusMZ = xmlSvc()->eval(outerRadiusMZAttribute);
  }
  if (!innerRadiusPZAttribute.empty()) {
    innerRadiusPZ = xmlSvc()->eval(innerRadiusPZAttribute);
  }
  if (!innerRadiusMZAttribute.empty()) {
    innerRadiusMZ = xmlSvc()->eval(innerRadiusMZAttribute);
  }
  if (!startPhiAngleAttribute.empty()) {
    startPhiAngle = xmlSvc()->eval(startPhiAngleAttribute, false);
  }
  if (!deltaPhiAngleAttribute.empty()) {
    deltaPhiAngle = xmlSvc()->eval(deltaPhiAngleAttribute, false);
  }

  // builds solid
  auto result = std::make_unique< SolidCons >
    (consName, sizeZ / 2.0, outerRadiusMZ, outerRadiusPZ, innerRadiusMZ,
     innerRadiusPZ, startPhiAngle, deltaPhiAngle);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << "A cons should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithCons


// -----------------------------------------------------------------------
// Deal with polycone
// -----------------------------------------------------------------------
std::unique_ptr<SolidPolycone> XmlLVolumeCnv::dealWithPolycone(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string startPhiAngleAttribute =
    dom2Std (element->getAttribute (startPhiAngleString));
  std::string deltaPhiAngleAttribute =
    dom2Std (element->getAttribute (deltaPhiAngleString));
  std::string polyconeName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double startPhiAngle = 0.0;
  double deltaPhiAngle = 360.0 * Gaudi::Units::degree;
  if (!startPhiAngleAttribute.empty()) {
    startPhiAngle = xmlSvc()->eval(startPhiAngleAttribute, false);
  }
  if (!deltaPhiAngleAttribute.empty()) {
    deltaPhiAngle = xmlSvc()->eval(deltaPhiAngleAttribute, false);
  }

  // this are the zplanes contained by this node
  SolidPolycone::Triplets zplanes;

  // deals with the children
  xercesc::DOMNodeList* children = element->getElementsByTagName (zplaneString);
  for (unsigned int i = 0; i < children->getLength(); i++) {
    xercesc::DOMNode* childNode = children->item(i);
    xercesc::DOMElement* child = (xercesc::DOMElement*) childNode;

    // gets attributes
    std::string zAttribute =
      dom2Std (child->getAttribute (zString));
    std::string outerRadiusAttribute =
      dom2Std (child->getAttribute (outerRadiusString));
    std::string innerRadiusAttribute =
      dom2Std (child->getAttribute (innerRadiusString));

    // computes the values
    double z = 0.0;
    double outerRadius = 0.0;
    double innerRadius = 0.0;
    if (!zAttribute.empty()) {
      z = xmlSvc()->eval(zAttribute);
    }
    if (!outerRadiusAttribute.empty()) {
      outerRadius = xmlSvc()->eval(outerRadiusAttribute);
    }
    if (!innerRadiusAttribute.empty()) {
      innerRadius = xmlSvc()->eval(innerRadiusAttribute);
    }

    // builds zplane and adds it to the list
    zplanes.push_back (SolidPolycone::Triplet
                       (z, SolidPolycone::Pair (outerRadius, innerRadius)));

    // checks there are no children
    if (child->hasChildNodes()) {
      MsgStream log(msgSvc(), "XmlLVolumeCnv" );
      log << MSG::WARNING << "In " << locateElement (child)
          << "A zplane should not have any child. They will be ignored"
          << endmsg;
    }
  }

  // builds solid
  try {

    return std::make_unique< SolidPolycone>
      (polyconeName, zplanes, startPhiAngle, deltaPhiAngle);

  } catch (const GaudiException &e) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::ERROR << "Was not able to create SolidPolycone "
        << "due to following GaudiException : ";
    e.printOut (log);
    log << endmsg;
    return nullptr;
  }

} // end dealWithPolycone


// -----------------------------------------------------------------------
// Deal with tubs
// -----------------------------------------------------------------------
std::unique_ptr<SolidTubs> XmlLVolumeCnv::dealWithTubs(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string sizeZAttribute =
    dom2Std (element->getAttribute (sizeZString));
  std::string outerRadiusAttribute =
    dom2Std (element->getAttribute (outerRadiusString));
  std::string innerRadiusAttribute =
    dom2Std (element->getAttribute (innerRadiusString));
  std::string startPhiAngleAttribute =
    dom2Std (element->getAttribute (startPhiAngleString));
  std::string deltaPhiAngleAttribute =
    dom2Std (element->getAttribute (deltaPhiAngleString));
  std::string tubsName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double sizeZ = 0.0;
  double outerRadius = 0.0;
  double innerRadius = 0.0;
  double startPhiAngle = 0.0;
  double deltaPhiAngle = 360.0 * Gaudi::Units::degree;
  if (!sizeZAttribute.empty()) {
    sizeZ = xmlSvc()->eval(sizeZAttribute);
  }
  if (!outerRadiusAttribute.empty()) {
    outerRadius = xmlSvc()->eval(outerRadiusAttribute);
  }
  if (!innerRadiusAttribute.empty()) {
    innerRadius = xmlSvc()->eval(innerRadiusAttribute);
  }
  if (!startPhiAngleAttribute.empty()) {
    startPhiAngle = xmlSvc()->eval(startPhiAngleAttribute, false);
  }
  if (!deltaPhiAngleAttribute.empty()) {
    deltaPhiAngle = xmlSvc()->eval(deltaPhiAngleAttribute, false);
  }

  // builds solid
  auto result = std::make_unique< SolidTubs >
    (tubsName, sizeZ / 2.0, outerRadius, innerRadius,
     startPhiAngle, deltaPhiAngle);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << "A tubs should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithTubs


// -----------------------------------------------------------------------
// Deal with sphere
// -----------------------------------------------------------------------
std::unique_ptr<SolidSphere> XmlLVolumeCnv::dealWithSphere(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string outerRadiusAttribute =
    dom2Std (element->getAttribute (outerRadiusString));
  std::string innerRadiusAttribute =
    dom2Std (element->getAttribute (innerRadiusString));
  std::string startPhiAngleAttribute =
    dom2Std (element->getAttribute (startPhiAngleString));
  std::string deltaPhiAngleAttribute =
    dom2Std (element->getAttribute (deltaPhiAngleString));
  std::string startThetaAngleAttribute =
    dom2Std (element->getAttribute (startThetaAngleString));
  std::string deltaThetaAngleAttribute =
    dom2Std (element->getAttribute (deltaThetaAngleString));
  std::string sphereName = dom2Std (element->getAttribute (nameString));

  // computes the values
  double outerRadius = 0.0;
  double innerRadius = 0.0;
  double startPhiAngle = 0.0;
  double deltaPhiAngle = 360.0 * Gaudi::Units::degree;
  double startThetaAngle = 0.0;
  double deltaThetaAngle = 180.0 * Gaudi::Units::degree;
  if (!outerRadiusAttribute.empty()) {
    outerRadius = xmlSvc()->eval(outerRadiusAttribute);
  }
  if (!innerRadiusAttribute.empty()) {
    innerRadius = xmlSvc()->eval(innerRadiusAttribute);
  }
  if (!startPhiAngleAttribute.empty()) {
    startPhiAngle = xmlSvc()->eval(startPhiAngleAttribute, false);
  }
  if (!deltaPhiAngleAttribute.empty()) {
    deltaPhiAngle = xmlSvc()->eval(deltaPhiAngleAttribute, false);
  }
  if (!startThetaAngleAttribute.empty()) {
    startThetaAngle = xmlSvc()->eval(startThetaAngleAttribute, false);
  }
  if (!deltaThetaAngleAttribute.empty()) {
    deltaThetaAngle = xmlSvc()->eval(deltaThetaAngleAttribute, false);
  }

  // take care that if startThetaAngle+deltaThetaAngle = 180 degree,
  // it could be 180 + epsilon in case of a round error.
  // In this case, we correct deltaThetaAngle
  if ((startThetaAngle + deltaThetaAngle > M_PI) &&
      (startThetaAngle + deltaThetaAngle <= M_PI + 2*DBL_EPSILON)) {
    deltaThetaAngle = M_PI - startThetaAngle;
  }

  // builds the solid
  auto result = std::make_unique< SolidSphere >
    (sphereName, outerRadius, innerRadius, startPhiAngle,
     deltaPhiAngle, startThetaAngle, deltaThetaAngle);

  // checks there are no children
  if (element->hasChildNodes()) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << "A sphere should not have any child. They will be ignored" << endmsg;
  }

  // returns
  return result;
} // end dealWithSphere


// -----------------------------------------------------------------------
// Deal with transformation
// -----------------------------------------------------------------------
std::unique_ptr<Gaudi::Transform3D>
XmlLVolumeCnv::dealWithTransformation(const xercesc::DOMElement* element,
                                      unsigned int* index) const {
  // gets children of element
  xercesc::DOMNodeList* childNodes = element->getChildNodes();

  // gets the first node of the transformation
  xercesc::DOMNode* childNode = childNodes->item (*index);
  xercesc::DOMElement* childElement = (xercesc::DOMElement*) childNode;

  // put index on the next node
  ++*index;
  while (*index < childNodes->getLength() &&
         childNodes->item(*index)->getNodeType() !=
         xercesc::DOMNode::ELEMENT_NODE) {
    ++*index;
  }

  // the result
  std::optional<Gaudi::Transform3D> result;

  // gets the tag name for childElement
  const XMLCh* tagName = childElement->getNodeName();
  // gets the transformation corresponding to the first child
  if (0 == xercesc::XMLString::compareString(posXYZString, tagName)) {
    result = dealWithPosXYZ(childElement);
  } else if (0 == xercesc::XMLString::compareString(posRPhiZString, tagName)) {
    // catches an exception in case the r attribute is negative
    try {
      result = dealWithPosRPhiZ(childElement);
    } catch (const XmlCnvException &e) {
      result.emplace();
    }
  } else if (0 == xercesc::XMLString::compareString(posRThPhiString, tagName)) {
    // catches an exception in case the r attribute is negative
    try {
      result = dealWithPosRThPhi(childElement);
    } catch (const XmlCnvException &e) {
      result.emplace();
    }
  } else if (0 == xercesc::XMLString::compareString
             (transformationString, tagName)) {
    return std::make_unique<Gaudi::Transform3D>(dealWithTransformation (childElement));
  } else {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    char* tagNameString = xercesc::XMLString::transcode(tagName);
    log << MSG::WARNING << "In " << locateElement (childElement)
        << " : this should be a rotation or a translation but is a : "
        << tagNameString << ". It will be ignored." << endmsg;
    xercesc::XMLString::release(&tagNameString);
    return std::make_unique<Gaudi::Transform3D>();
  }

  if (!result) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv");
    log << MSG::ERROR << "did not get expected translation???" << endmsg;
  }

  // We are here because we found a translation. Try now to see if
  // it is followed by a rotation
  if (*index < childNodes->getLength()) {
    childNode = childNodes->item (*index);
    childElement = (xercesc::DOMElement*) childNode;
    bool foundRotation = false;

    // gets the tag name for the new childElement
    tagName = childElement->getNodeName();
    if (0 == xercesc::XMLString::compareString(rotXYZString, tagName)) {
      *result = *result * dealWithRotXYZ(childElement);
      foundRotation = true;
    } else if (0 == xercesc::XMLString::compareString(rotAxisString, tagName)) {
      // catches an exception in case the theta or phi value is out of range
      try {
        *result = *result * dealWithRotAxis(childElement);
      } catch (const XmlCnvException &e) {
      }
      foundRotation = true;
    }

    // if needed, put index on the next node
    if (foundRotation) {
      ++*index;
      while (*index < childNodes->getLength() &&
             childNodes->item(*index)->getNodeType() !=
             xercesc::DOMNode::ELEMENT_NODE) {
        ++*index;
      }
    }
  }

  return std::make_unique<Gaudi::Transform3D>( *result );
} // end dealWithTransformation


// -----------------------------------------------------------------------
// Deal with transformation
// -----------------------------------------------------------------------
Gaudi::Transform3D
XmlLVolumeCnv::dealWithTransformation(const xercesc::DOMElement* element) const {
  // gets the children
  xercesc::DOMNodeList* childNodes = element->getChildNodes();

  Gaudi::Transform3D result;
  // scans the children and builds the transformations
  // computes the result
  unsigned int i = 0;
  unsigned int n = 0;
  while (i < childNodes->getLength()) {
    auto transformation = dealWithTransformation (element, &i);
    if (transformation)  {
      result = *transformation * result;
      ++n;
    }
  }

  // checks there are at least 2 transformations
  if (n < 2) {
    MsgStream log(msgSvc(), "XmlLVolumeCnv" );
    log << MSG::WARNING << "In " << locateElement (element)
        << "A transformation should have at least two children." << endmsg;
  }

  // returns
  return result;
} // end dealWithTransformation


// -----------------------------------------------------------------------
// Deal with posXYZ
// -----------------------------------------------------------------------
Gaudi::Transform3D XmlLVolumeCnv::dealWithPosXYZ(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string xAttribute = dom2Std (element->getAttribute (xString));
  std::string yAttribute = dom2Std (element->getAttribute (yString));
  std::string zAttribute = dom2Std (element->getAttribute (zString));

  // computes the values
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  if (!xAttribute.empty()) {
    x = xmlSvc()->eval(xAttribute);
  }
  if (!yAttribute.empty()) {
    y = xmlSvc()->eval(yAttribute);
  }
  if (!zAttribute.empty()) {
    z = xmlSvc()->eval(zAttribute);
  }

  // builds the translation.
  return  Gaudi::Transform3D(Gaudi::XYZVector(x, y, z) );
} // end dealWithPosXYZ


// -----------------------------------------------------------------------
// Deal with posRPhiZ
// -----------------------------------------------------------------------
Gaudi::Transform3D XmlLVolumeCnv::dealWithPosRPhiZ(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string rAttribute = dom2Std (element->getAttribute (rString));
  std::string phiAttribute = dom2Std (element->getAttribute (phiString));
  std::string zAttribute = dom2Std (element->getAttribute (zString));

  // computes the values
  double rho = 0.0;
  double z = 0.0;
  double phi = 0.0;

  if (!rAttribute.empty()) {
    rho = xmlSvc()->eval(rAttribute);
  }
  if (!phiAttribute.empty()) {
    phi = xmlSvc()->eval(phiAttribute, false);
  }
  if (!zAttribute.empty()) {
    z = xmlSvc()->eval(zAttribute);
  }
  // checks the validity of the value of r
  if (0.0 > rho) {
    throw XmlCnvException
      (" RPhiZTranslation : r must be non-negative value !", CORRUPTED_DATA);
  }

  // builds the translation.

  return Gaudi::Transform3D(Gaudi::RhoZPhiVector( rho, z, phi ) );
} // end dealWithPosRPhiZ


// -----------------------------------------------------------------------
// Deal with posRThPhi
// -----------------------------------------------------------------------
Gaudi::Transform3D XmlLVolumeCnv::dealWithPosRThPhi(const xercesc::DOMElement* element) const {
  // gets attributes
  std::string rAttribute = dom2Std (element->getAttribute (rString));
  std::string thetaAttribute = dom2Std (element->getAttribute (thetaString));
  std::string phiAttribute = dom2Std (element->getAttribute (phiString));

  // computes the values
  double r = 0.0;
  double theta = 0.0;
  double phi = 0.0;
  if (!rAttribute.empty()) {
    r = xmlSvc()->eval(rAttribute);
  }
  if (!thetaAttribute.empty()) {
    theta = xmlSvc()->eval(thetaAttribute, false);
  }
  if (!phiAttribute.empty()) {
    phi = xmlSvc()->eval(phiAttribute, false);
  }
  // checks the validity of the value of r
  if (0.0 > r) {
    throw XmlCnvException
      (" RThPhiTranslation : r must be non-negative value !", CORRUPTED_DATA);
  }

  // builds the translation.

  return Gaudi::Transform3D(Gaudi::Polar3DVector(r, theta, phi) );
} // end dealWithPosRThPhi


// -----------------------------------------------------------------------
// Deal with rotXYZ
// -----------------------------------------------------------------------
Gaudi::Transform3D
XmlLVolumeCnv::dealWithRotXYZ(const xercesc::DOMElement* element) const {
  // get attributes
  std::string rotXAttribute = dom2Std (element->getAttribute (rotXString));
  std::string rotYAttribute = dom2Std (element->getAttribute (rotYString));
  std::string rotZAttribute = dom2Std (element->getAttribute (rotZString));

  // computes the values
  double rx = 0.0;
  double ry = 0.0;
  double rz = 0.0;
  if (!rotXAttribute.empty()) {
    rx = xmlSvc()->eval(rotXAttribute, false);
  }
  if (!rotYAttribute.empty()) {
    ry = xmlSvc()->eval(rotYAttribute, false);
  }
  if (!rotZAttribute.empty()) {
    rz = xmlSvc()->eval(rotZAttribute, false);
  }

  // computes the rotation
  Gaudi::Rotation3D result = Gaudi::RotationX(rx) *
    Gaudi::RotationY(ry) *
    Gaudi::RotationZ(rz);

  // Recover precision in cases when rotation is by halfpi or pi
  double xx, xy, xz, yx, yy, yz, zx, zy, zz;
  result.GetComponents( xx, xy, xz, yx, yy, yz, zx, zy, zz );
  if( fabs(xx) < 1.e-15 ) xx = 0.;
  if( fabs(xy) < 1.e-15 ) xy = 0.;
  if( fabs(xz) < 1.e-15 ) xz = 0.;
  if( fabs(yx) < 1.e-15 ) yx = 0.;
  if( fabs(yy) < 1.e-15 ) yy = 0.;
  if( fabs(yz) < 1.e-15 ) yz = 0.;
  if( fabs(zx) < 1.e-15 ) zx = 0.;
  if( fabs(zy) < 1.e-15 ) zy = 0.;
  if( fabs(zz) < 1.e-15 ) zz = 0.;
  result.SetComponents( xx, xy, xz, yx, yy, yz, zx, zy, zz );

  return Gaudi::Transform3D(result);
} // end dealWithRotXYZ


// -----------------------------------------------------------------------
// Deal with rotAxis
// -----------------------------------------------------------------------
Gaudi::Transform3D
XmlLVolumeCnv::dealWithRotAxis(const xercesc::DOMElement* element) const {
  // get attributes
  std::string axThetaAttribute =
    dom2Std (element->getAttribute (axThetaString));
  std::string axPhiAttribute =
    dom2Std (element->getAttribute (axPhiString));
  std::string angleAttribute =
    dom2Std (element->getAttribute (angleString));

  // computes the values
  double axTheta = 0.0;
  double axPhi = 0.0;
  double angle = 0.0;
  if (!axThetaAttribute.empty()) {
    axTheta = xmlSvc()->eval (axThetaAttribute, false);
  }
  if (!axPhiAttribute.empty()) {
    axPhi = xmlSvc()->eval (axPhiAttribute, false);
  }
  if (!angleAttribute.empty()) {
    angle = xmlSvc()->eval (angleAttribute, false);
  }
  // checks the validity of the theta angle
  if (axTheta < 0 || axTheta > 180 * Gaudi::Units::degree) {
    throw XmlCnvException
      (" AxisRotation : axTheta must be inside 0*degree"
       " and 180*degree ! ", CORRUPTED_DATA);
  }
  // checks the validity of the phi angle
  if (axPhi < 0 || axPhi > 360 * Gaudi::Units::degree) {
    throw XmlCnvException
      (" AxisRotation : axPhi  must be inside 0*degree"
       " and 360*degree ! ", CORRUPTED_DATA);
  }
  // Construction of vector with input of theta and phi
  Gaudi::XYZVector axis( sin(axTheta)*cos(axPhi),
                         sin(axTheta)*sin(axPhi),
                         cos(axTheta)             );

  return Gaudi::Transform3D(Gaudi::AxisAngle(axis, angle),
                            Gaudi::XYZVector());


} // end dealWithRotAxis
