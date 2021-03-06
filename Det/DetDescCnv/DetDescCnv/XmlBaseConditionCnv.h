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

#ifndef DETDESCCNV_XMLCONDITIONCNV_H
#define DETDESCCNV_XMLCONDITIONCNV_H

/// Include files
#include "DetDesc/Condition.h"
#include "DetDescCnv/XmlGenericCnv.h"

#include "GaudiKernel/Converter.h"

#include <xercesc/dom/DOMNodeList.hpp>

/// Forward and external declarations
class ISvcLocator;
template <class TYPE>
class CnvFactory;

/** @class XmlBaseConditionCnv
 *
 * XML converter for Conditions
 *
 * @author Sebastien Ponce
 */
class XmlBaseConditionCnv : public XmlGenericCnv {

public:
  /**
   * Initializes the converter - Overrides the default method in XmlGenericCnv
   * @return status depending on the completion of the call
   */
  StatusCode initialize() override;

  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID();

protected:
  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlBaseConditionCnv( ISvcLocator* svcs );

  /**
   * Constructor for this converter
   * @param svc   a ISvcLocator interface to find services
   * @param clsID the type of object the converter is able to convert
   */
  XmlBaseConditionCnv( ISvcLocator* svc, const CLID& clsID );

  /**
   * Default destructor
   */
  virtual ~XmlBaseConditionCnv();

  /**
   * Resolves the references of the created transient object.
   */
  StatusCode fillObjRefs( IOpaqueAddress* pAddress, DataObject* pObject ) override;

  /**
   * Resolves the references of the just updated transient object.
   */
  StatusCode updateObjRefs( IOpaqueAddress* pAddress, DataObject* pObject ) override;

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
   * @param refpObject   the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_fillObj( xercesc::DOMElement* childElement, DataObject* refpObject, IOpaqueAddress* address ) override;

  /** This fills the current object for specific child.
   * Specific children are children of children \<specific\>
   * of the \<condition\> tag. This is typically used for specializing
   * consditions and define new properties.
   * This will be called for each specific child of the current object
   * @param childElement the specific child processed here
   * @param refpObject the object to be filled
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  virtual StatusCode i_fillSpecificObj( xercesc::DOMElement* childElement, Condition* refpObject,
                                        IOpaqueAddress* address ) = 0;

private:
  /// Whether to use the generic converter in case a specific one does not exist
  bool m_doGenericCnv;

  // Constant strings for element names
  const XMLCh* specificString;
  const XMLCh* paramString;
  const XMLCh* paramVectorString;
  const XMLCh* mapString;
  const XMLCh* itemString;

  // Constant Strings for parameter names
  const XMLCh* typeString;
  const XMLCh* nameString;
  const XMLCh* commentString;
  const XMLCh* keytypeString;
  const XMLCh* valuetypeString;
  const XMLCh* keyString;
  const XMLCh* valueString;

  // Functions used for the map parameter
  template <class T>
  inline T i_convert( const XMLCh* value ) {
    return (T)xmlSvc()->eval( i_convert<std::string>( value ), false );
  }

  template <class T>
  inline T i_convert( const std::string& value ) {
    return (T)xmlSvc()->eval( value, false );
  }

  template <class T>
  inline T i_convertVect( XMLCh* value ) {
    std::vector<T>     v;
    std::istringstream cstr( i_convert<std::string>( value ) );
    std::string        itemval;
    while ( cstr >> itemval ) { v.push_back( i_convert<T>( itemval ) ); }
    return v;
  }

  template <class K, class V>
  std::map<K, V> i_makeMap( xercesc::DOMNodeList* entries ) {
    std::map<K, V> map;
    XMLSize_t      i;
    for ( i = 0; i < entries->getLength(); i++ ) {
      xercesc::DOMNode* entryNode = entries->item( i );
      if ( entryNode->getNodeType() == xercesc::DOMNode::ELEMENT_NODE ) {
        xercesc::DOMElement* entry                            = static_cast<xercesc::DOMElement*>( entryNode );
        map[i_convert<K>( entry->getAttribute( keyString ) )] = i_convert<V>( entry->getAttribute( valueString ) );
      }
    }
    return map;
  }
};

template <>
inline std::string XmlBaseConditionCnv::i_convert<std::string>( const XMLCh* value ) {
  return dom2Std( value );
}

template <>
inline std::string XmlBaseConditionCnv::i_convert<std::string>( const std::string& value ) {
  return value;
}

#endif // DETDESCCNV_XMLCONDITIONCNV_H
