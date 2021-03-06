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
//  ====================================================================
//  MDFIO.h
//  --------------------------------------------------------------------
//
//  Author    : Markus Frank
//
//  ====================================================================
#ifndef DAQ_MDF_MDFIO_H
#define DAQ_MDF_MDFIO_H

#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/StreamBuffer.h"

// forward declarations
class IMessageSvc;
class IDataProviderSvc;
namespace Gaudi {
  class IFileCatalogSvc;
}

/*
 *    LHCb namespace
 */
namespace LHCb {

  class RawBank;
  class RawEvent;
  class MDFHeader;

  /**@class MDFIO MDFIO.h MDF/MDFIO.h
   *
   *
   *  @author  M.Frank
   *  @version 1.0
   */
  class MDFIO {

  public:
    // Data type: banks from TES=1,
    //            compressed records data from address = 2
    //            banks data from address (MDF bank first) = 3
    enum Writer_t { MDF_NONE = 1, MDF_RECORDS = 2, MDF_BANKS = 3 };
    /// MDF data descriptor definition
    typedef std::pair<char*, int> MDFDescriptor;

  protected:
    /// Streambuffer to hold compressed data
    StreamBuffer m_tmp;
    /// Pointer to message stream for error printing
    IMessageSvc* m_msgSvc;
    /// Pointer to data service for accesss to the TES
    IDataProviderSvc* m_evtSvc;
    /// Input data type
    int m_dataType;
    /// Parent
    std::string m_parent;
    /// File catalog service name
    std::string m_catalogName;
    /// Pointer to catalog service if it should be used
    Gaudi::IFileCatalogSvc* m_catalog;
    /// Flag to ignore the checksum
    bool m_ignoreChecksum;
    /// Flag to force TAE format of the MDF file
    bool m_forceTAE;
    /// Flag to not print messages if getDataSpace fails
    bool m_silent;
    /// Monitoring quantity: Number of write errors
    int m_writeErrors;
    /// Monitoring quantity: Number of write actions
    int m_writeActions;

    /// Monitoring quantity: Number of space errors
    int m_spaceErrors;
    /// Monitoring quantity: Number of space actions
    int m_spaceActions;

    /// Helper to retrieve data from Opaque address
    std::pair<const char*, int> getDataFromAddress();
    /// Helper to access header information
    MDFHeader* getHeader();
    /** Setup IO object: set service pointers
     * @param[in] msg Reference to message service for error logging
     * @param[in] svc Reference to data provider service to locate RawEvent
     */
    void setupMDFIO( IMessageSvc* msg, IDataProviderSvc* svc ) {
      m_msgSvc = msg;
      m_evtSvc = svc;
    }
    /// Set pointer to catalog service
    void setCatalog( Gaudi::IFileCatalogSvc* cat ) { m_catalog = cat; }
    /// Access to file catalog
    Gaudi::IFileCatalogSvc* catalog() const { return m_catalog; }

    /// Check usage of catalog
    bool useCatalog() const { return !m_catalogName.empty(); }

    /// Ignore checksum
    void setIgnoreChecksum( bool new_value ) { m_ignoreChecksum = new_value; }

    MDFDescriptor readLegacyBanks( const MDFHeader& h, void* const ioDesc, bool dbg );

    MDFDescriptor readBanks( const MDFHeader& h, void* const ioDesc, bool dbg );

    bool checkSumOk( int checksum, const char* src, int datSize, bool prt );

    RawBank* createDummyMDFHeader( RawEvent* raw, size_t len );

  public:
    /// Initializing constructor
    MDFIO( Writer_t typ, const std::string& nam )
        : m_msgSvc( 0 )
        , m_evtSvc( 0 )
        , m_dataType( typ )
        , m_parent( nam )
        , m_catalog( 0 )
        , m_ignoreChecksum( false )
        , m_forceTAE( false )
        , m_silent( false )
        , m_writeErrors( 0 )
        , m_writeActions( 0 )
        , m_spaceErrors( 0 )
        , m_spaceActions( 0 ) {}

    /// Default destructor
    virtual ~MDFIO() {}

    /// Transform file name in presence of catalogs
    virtual std::string getConnection( const std::string& org_conn );

    /** Allocate space
     * @param[in] ioDesc Output IO descriptor
     * @param[in] len    Total length of the data buffer
     *
     * @return  Pointer to allocated memory space
     */
    virtual MDFDescriptor getDataSpace( void* const ioDesc, size_t len ) = 0;

    /** Write raw char buffer to output stream
     * @param[in] ioDesc Output IO descriptor
     * @param[in] data   Data buffer to be streamed
     * @param[in] len    Total length of the data buffer
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode writeBuffer( void* const ioDesc, const void* data, size_t len );

    /** Write char buffer to output stream
     * @param[in] compTyp   Compression type
     * @param[in] chksumTyp Checksum type
     * @param[in] ioDesc    Output IO descriptor
     * @param[in] hdr       Bank with MDF header structure as payload
     * @param[in] data      Data buffer to be streamed
     * @param[in] len       Total length of the data buffer
     *
     * Note:
     *     on writing, data contains empty space in front
     *     of the size RawBank(MDF)::totalSize(),
     *     to this space either
     *       - the entire MDF Rawbank __or__ (m_dataType==MDF_BANKS)
     *       - the MDF header                (m_dataType==MDF_RECORDS)
     *     is copied depending on the value of m_dataType
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode writeDataSpace( int compTyp, int chksumTyp, void* const ioDesc, const RawBank* hdr,
                                       char* const data, size_t len );

    /** Commit raw banks to IO stream. -- Main entry point --
     * @param[in] compTyp   Compression type
     * @param[in] chksumTyp Checksum type
     * @param[in] ioDesc    Output IO descriptor
     * @param[in] location  Location of the raw event in the transient data store
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode commitRawBanks( int compTyp, int chksumTyp, void* const ioDesc, const std::string& location );

    /** Commit raw banks to IO stream. -- Main entry point --
     * @param[in] raw       Pointer to RawEvent
     * @param[in] hdr_bank  Pointer to MDF header bank
     * @param[in] compTyp   Compression type
     * @param[in] chksumTyp Checksum type
     * @param[in] ioDesc    Output IO descriptor
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode commitRawBanks( RawEvent* raw, const RawBank* hdr_bank, int compTyp, int chksumTyp,
                                       void* const ioDesc );

    /** Direct I/O with valid existing raw buffers
     * @param[in] type      raw buffer type (MDF_RECORDS, MDF_BANKS)
     * @param[in] compTyp   Compression type
     * @param[in] chksumTyp Checksum type
     * @param[in] ioDesc    Output IO descriptor
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode commitRawBuffer( int type, int compTyp, int chksumTyp, void* const ioDesc );

    /** Direct I/O with valid existing raw buffers
     * @param[in] data      Reference to data location
     * @param[in] len       length of data block
     * @param[in] type      raw buffer type (MDF_RECORDS, MDF_BANKS)
     * @param[in] compTyp   Compression type
     * @param[in] chksumTyp Checksum type
     * @param[in] ioDesc    Output IO descriptor
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode commitRawBuffer( const void* data, size_t len, int type, int compTyp, int chksumTyp,
                                        void* const ioDesc );

    /** Read raw banks from IO stream
     * @param[in] ioDesc Input IO descriptor
     * @param[in] dbg    Optional flag for debug printout (default false)
     *
     * @return  Number of valid bytes in stream buffer (-1 on failure).
     */
    virtual MDFDescriptor readBanks( void* const ioDesc, bool dbg = false );

    /** Read raw char buffer from input stream
     * @param[in] ioDesc Input IO descriptor
     * @param[in] data   Data buffer to reseive data
     * @param[in] len    Total length of data buffer to receive
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode readBuffer( void* const ioDesc, void* const data, size_t len );

    /** Pass raw banks to RawEvent object
     * @param[in] evt        Pointer to raw event receiving banks
     * @param[in] bnks       Vector of banks to be assigned to evt
     * @param[in] copy_banks Deep copy banks rather than pure assignment
     *
     * @return  Status code indicating success or failure.
     */
    virtual StatusCode adoptBanks( RawEvent* evt, const std::vector<RawBank*>& bnks, bool copy_banks = false );
  };
} // end namespace LHCb
#endif /* DAQ_MDF_MDFIO_H */
