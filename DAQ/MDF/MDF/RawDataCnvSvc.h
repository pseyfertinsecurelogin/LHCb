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
//  ====================================================================
//  RawDataCnvSvc.h
//  --------------------------------------------------------------------
//
//  Author    : Markus Frank
//
//  ====================================================================
#ifndef MDF_RAWDATACNVSVC_H
#define MDF_RAWDATACNVSVC_H

// Framework include files
#include "GaudiKernel/ConversionSvc.h"
#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiUtils/IIODataManager.h"
#include "GaudiKernel/IRegistry.h"
#include "MDF/MDFIO.h"

// C++ include files
#include <map>

/*
 *    LHCb namespace declaration
 */
namespace LHCb  {

  // Forward declarations
  class RawDataAddress;
  class RawEvent;
  class RawBank;

  /** @class RawDataCnvSvc RawDataCnvSvc.h  MDF/RawDataCnvSvc.h
    *
    * Conversion service for the online data.
    *
    * @author  M.Frank
    * @version 1.0
    * @date    01/01/2005
    */
  class RawDataCnvSvc : public ConversionSvc, public MDFIO  {
  protected:
    typedef const std::string&            CSTR;
    typedef std::vector<RawBank*>         Banks;
    typedef std::map<std::string, void*>  FileMap;

    FileMap::iterator      m_current;
    /// Mapping of accesed files
    FileMap                m_fileMap;

    bool                   m_wrFlag;
    /// Reference to data manager interface
    SmartIF<IDataManagerSvc> m_dataMgr;
    /// Reference to file manager service
    SmartIF<Gaudi::IIODataManager> m_ioMgr;
    /// Property: Compression algorithm identifier
    int                    m_compress;
    /// Property: Flag to create checksum
    int                    m_genChecksum;
    /// Property: Properties for time alignment events
    int                    m_evtsBefore, m_evtsAfter;
    /// Property: Flag to copy banks to the raw event (or only reference the banks)
    int                    m_copyBanks;
    /// Property: Property to indicate input data type (RAW, RDST)
    std::string            m_sourceType;
    /// Property: Location of RAW banks in the TES
    std::string            m_bankLocation;
    /// Property: Location of DST banks in TES
    std::string            m_dstLocation;
    /// Property: Name of the IO manager service
    std::string            m_ioMgrName;

    /// Helper to print errors and return bad status
    StatusCode error(CSTR msg)  const;

    /// Open MDF file
    virtual void* openIO(CSTR fname, CSTR  mode) const;

    /// Close MDF file
    virtual StatusCode closeIO(void* ioDesc) const;

    /// Close all files disconnected from the IO manager
    virtual void closeDisconnected();

    /// Read raw banks
    virtual StatusCode readRawBanks(RawDataAddress* pAddr);

    /// Helper to install opaque address leaf
    StatusCode regAddr(IRegistry* pReg,RawDataAddress* pA,CSTR path,const CLID& clid);

    /// Access the raw data from MDF file
    MDFDescriptor accessRawData(RawDataAddress* pAddRaw);

    /// Decode a TAE event record from MDF banks
    StatusCode registerRawAddresses(IRegistry* pReg,
                                    RawDataAddress* pAddRaw,
                                    const std::vector<std::string>& names);

    /// MDFIO interface: Allocate data space for input or output
    MDFDescriptor getDataSpace(void* const ioDesc, size_t len) override;

    /// MDFIO interface: Write data block to output stream
    StatusCode writeBuffer(void* const ioDesc, const void* data, size_t len) override;

    /// MDFIO interface: Read raw byte buffer from input stream
    StatusCode readBuffer(void* const ioDesc, void* const data, size_t len) override;

  public:

    /** Initializing constructor
      *  @param[in]   nam   Name of the service
      *  @param[in]   loc   Pointer to the service locator object
      *  @return Initialized reference to service object
      */
    RawDataCnvSvc(CSTR nam, ISvcLocator* loc);

    /// Service initialization
    StatusCode initialize() override;

    /// Service finalization
    StatusCode finalize() override;

    /// Concrete class type
    const CLID& objType() const override;

    /** Object creation callback
      *  @param[in]   pAddr     Reference to opaque object address
      *  @param[out]  refpObj   Reference to store pointer to created object
      *
      *  @return Status code indicating success or failure
      */
    StatusCode createObj(IOpaqueAddress* pAddr, DataObject*& refpObj) override;

    /** Callback for reference processing (misused to attach leaves)
      *  @param[in]   pAddr     Reference to opaque object address
      *  @param[out]  pObj      Reference to store pointer to created object
      *
      *  @return Status code indicating success or failure
      */
    StatusCode fillObjRefs(IOpaqueAddress* pAddr, DataObject* pObj) override;

    /// Connect the output file to the service with open mode.
    StatusCode connectOutput(CSTR name, CSTR mode) override;

    /// Connect the output file to the service.
    StatusCode connectOutput(const std::string& output) override
    { return this->ConversionSvc::connectOutput(output);        }

    /// Connect the input file to the service with READ mode
    virtual StatusCode connectInput(CSTR fname, void*& iodesc);

    /// Commit pending output.
    StatusCode commitOutput(CSTR , bool doCommit) override;

    /// Convert the transient object to the requested representation.
    StatusCode createRep(DataObject* pObject, IOpaqueAddress*& refpAddress) override;

    /// Resolve the references of the converted object.
    StatusCode fillRepRefs(IOpaqueAddress* pAddress,DataObject* pObject) override;

    /// Create a Generic address using explicit arguments to identify a single object.
    StatusCode createAddress(long typ, const CLID& clid, const std::string* par,
                             const unsigned long* ip, IOpaqueAddress*& refpAddress) override;

    /// Convert an address in string form to object form
    StatusCode createAddress( long svc_type,
				      const CLID& clid,
				      const std::string& refAddress,
				      IOpaqueAddress*& refpAddress) override
    { return this->ConversionSvc::createAddress(svc_type,clid,refAddress,refpAddress); }

  };
}      // End namespace LHCb
#endif // MDF_RAWDATACNVSVC_H
