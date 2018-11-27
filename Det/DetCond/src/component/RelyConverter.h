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
#ifndef COMPONENT_RELYCONVERTER_H
#define COMPONENT_RELYCONVERTER_H 1

// Include files
#include "DetCond/CondDBGenericCnv.h"

// Forward and external declarations
class ISvcLocator;
template <class TYPE> class CnvFactory;

/** @class RelyConverter RelyConverter.h component/RelyConverter.cpp
 *
 *  ConditionsDBCnvSvc rely on the functionalities provided by the XmlCnvSvc.
 *  RelyConverter delegate the creation of the object to the XmlCnvSvc
 *  (via DetectorPersistencySvc).
 *
 *  @author Marco CLEMENCIC
 *  @date   2004-12-03
 */
class RelyConverter: public CondDBGenericCnv {
public:

  /// Operations that can be performed by delegation
  enum Operation {
    CreateObject,
    FillObjectRefs,
    UpdateObjectRefs
  };

  /**
   * Initializes the converter
   *  @return status depending on the completion of the call
   */
  StatusCode initialize() override;

  /**
   * Finalizes the converter
   *  @return status depending on the completion of the call
   */
  StatusCode finalize() override;

  /**
   * Creates the transient representation of an object.
   * @param  pAddress the address of the object representation
   * @param  pObject the object created
   * @return status depending on the completion of the call
   */
  StatusCode createObj (IOpaqueAddress *pAddress,
                        DataObject *&pObject) override;
  /**
   * Resolve the references of the created transient object.
   * @param  pAddress the address of the object representation
   * @param  pObject the object created
   * @return status depending on the completion of the call
   */
  StatusCode fillObjRefs (IOpaqueAddress *pAddress,
                          DataObject *pObject) override;
  /**
   * Resolve the references of the just updated transient object.
   * @param  pAddress the address of the object representation
   * @param  pObject the object created
   * @return status depending on the completion of the call
   */
  StatusCode updateObjRefs (IOpaqueAddress *pAddress,
                            DataObject *pObject) override;
  /**
   * Updates the transient object from the other representation (not implemented).
   * @param pAddress the address of the object representation
   * @param pObject the object updated
   *  @return status depending on the completion of the call
   */
  StatusCode updateObj (IOpaqueAddress *pAddress,
                        DataObject *pObject) override;

  /**
   * Converts the transient object to the requested representation (not implemented).
   * @param refpAddress the address of the object representation
   * @param pObject the object to convert
   *  @return status depending on the completion of the call
   */
  StatusCode createRep (DataObject* pObject,
                        IOpaqueAddress*& refpAddress) override;

  /**
   * Updates the converted representation of a transient object.
   * @param pAddress the address of the object representation
   * @param pObject the object whose representation has to be updated
   *  @return status depending on the completion of the call
   */
  StatusCode updateRep (IOpaqueAddress* pAddress,
                        DataObject* pObject) override;

  /**
   * Accessor to the type of elements that this converter converts.
   * @return the classID for this type
   */
  static const CLID& classID () { return CLID_Any; }

  /// Standard constructor
  RelyConverter(ISvcLocator* svc);

private:

  /**
   * Do the needed steps to perform a creation by delegation.
   */
  StatusCode i_delegatedCreation(IOpaqueAddress* pAddress,
                                 DataObject *&pObject,
                                 Operation op = CreateObject);

public:
  /**
   * Extract the storage_type from the folder or description.
   */
  static long getStorageType(const std::string &path, const std::string &desc);

  /** Generate a temporary IOpaqueAddress to be passed to a PersistencySvc for
   *  the actual conversion.
   *
   *  @param src         originating URL (required by the XML format)
   *  @param storageType storage type ID (@see getStorageType)
   *  @param name        name of the object inside the storage container
   *  @param data        data to embed in the address
   *  @param log         MsgStream instance to report errors
   *  @param creator     IAdressCreator to use
   *
   *  @return an IOpaqueAddress pointer in case of success, 0 in case of failure.
   */
  static IOpaqueAddress *createTmpAddress(const std::string &src,
                                          long storageType,
                                          const std::string &name,
                                          const CLID &clId,
                                          const std::string &data,
                                          MsgStream &log,
                                          SmartIF<IAddressCreator>& creator);

private:
  /// Handle to the IConversionSvc interface of the DetectorPersistencySvc
  IConversionSvc*      m_detPersSvc;

};
#endif // COMPONENT_XMLRELYCNV_H
