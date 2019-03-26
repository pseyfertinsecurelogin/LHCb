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
#ifndef DETDESCCNV_XMLMIXTURECNV_H
#define DETDESCCNV_XMLMIXTURECNV_H

// Include files
#include "DetDesc/Mixture.h"
#include "DetDescCnv/XmlGenericCnv.h"
#include <string>

// Forward declarations
class ISvcLocator;
template <class TYPE>
class CnvFactory;

/** @class XmlLMixtureCnv
 *
 * XML converter for Mixtures
 *
 * @author Sebastien Ponce
 * @author Radovan Chytracek
 * @author Pere Mato
 */
class XmlMixtureCnv : public XmlGenericCnv {
public:
  /**
   * accessor to the type of elements that this converter converts
   * @return the classID for this type
   */
  static const CLID& classID() { return CLID_Mixture; }

  /**
   * Constructor for this converter
   * @param svcs a ISvcLocator interface to find services
   */
  XmlMixtureCnv( ISvcLocator* svcs );

  /**
   * Default destructor
   */
  virtual ~XmlMixtureCnv();

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

  /** This processes the current object.
   * Overrides the default method in XmlGenericCnv
   * @param refpObject the object to be processed
   * @param address the address for this object
   * @return status depending on the completion of the call
   */
  StatusCode i_processObj( DataObject* refpObject, IOpaqueAddress* address ) override;

  /**
   * This is used to describe the kind of ponderation used so far in the
   * mixture. MM_undefined means that the mixture is empty at this time,
   * MM_byFractionMass means that fraction of mass is used, MM_byNAtoms
   * means that number of atoms is used
   */
  enum MixMode { MM_undefined, MM_byFractionMass, MM_byNAtoms };

private:
  /// This tells what kind of ponderation has been used so far in the mixture
  MixMode m_mixMode;

  /**
   * compacts a path. This removes every ".." in the path.
   * @param path the path to be processed
   * @return the result
   */
  std::string compactPath( std::string path );

private:
  // Constant strings for element and parameter names
  const XMLCh* temperatureString;
  const XMLCh* pressureString;
  const XMLCh* stateString;
  const XMLCh* AeffString;
  const XMLCh* ZeffString;
  const XMLCh* IeffString;
  const XMLCh* densityString;
  const XMLCh* radlenString;
  const XMLCh* lambdaString;
  const XMLCh* tabpropsString;
  const XMLCh* addressString;
  const XMLCh* componentString;
  const XMLCh* nameString;
  const XMLCh* natomsString;
  const XMLCh* fractionmassString;
};

#endif // DETDESCCNV_XMLCNVSVC_XMLMIXTURECNV_H
