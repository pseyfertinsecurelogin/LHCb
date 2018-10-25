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
// $Id: RawDataConnection.h,v 1.2 2008-04-10 09:58:42 cattanem Exp $
#ifndef MDF_RAWDATACONNECTION_H
#define MDF_RAWDATACONNECTION_H

// Framework include files
#include "GaudiUtils/IIODataManager.h" // for IDataConnection class definition
#include "MDF/StreamDescriptor.h"

/*
 *  LHCb namespace declaration
 */
namespace LHCb  {

  /** @class RawDataConnection RawDataConnection.h MDF/RawDataConnection.h
    *
    *  @author  M.Frank
    *  @version 1.0
    *  @date    20/10/2007
    */
  class RawDataConnection : virtual public Gaudi::IDataConnection  {
  protected:
    StreamDescriptor::Access m_bind;
    StreamDescriptor::Access m_access;
  public:
    /// Standard constructor
    RawDataConnection(const IInterface* own, const std::string& nam);
    /// Standard destructor
    virtual ~RawDataConnection();
    /// Check if connected to data source
    bool isConnected() const        override {     return m_access.ioDesc != 0; }
    /// Open data stream in read mode
    StatusCode connectRead() override;
    /// Open data stream in write mode
    StatusCode connectWrite(IoType typ) override;
    /// Release data stream
    StatusCode disconnect() override;
    /// Read raw byte buffer from input stream
    StatusCode read(void* const data, size_t len) override;
    /// Write raw byte buffer to output stream
    StatusCode write(const void* data, int len) override;
    /// Seek on the file described by ioDesc. Arguments as in ::seek()
    long long int seek(long long int where, int origin) override;
  };
}         // End namespace Gaudi
#endif    // MDF_RAWDATACONNECTION_H
