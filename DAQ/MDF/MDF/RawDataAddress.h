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
// $Id: RawDataAddress.h,v 1.9 2007-12-14 11:42:23 frankb Exp $
#ifndef MDF_RAWDATAADDRESS_H
#define MDF_RAWDATAADDRESS_H

// Framework include files
#include "GaudiKernel/GenericAddress.h"

/*
 *    LHCb namespace declaration
 */
namespace LHCb  {

  // Forward declaration
  class RawBank;

  /** @class RawDataAddress RawDataAddress.h GaudiKernel/RawDataAddress.h
    *
    * Generic Transient Address fro MBM or RawData data.
    *
    *  @author Markus Frank
    *  @version 1.0
    */
  class RawDataAddress : public GenericAddress  {
  public:
    static const size_t GIGA_BYTE = 1073741824;   // == (1024*1024*1024)

  protected:
    /// Pointer to vector of raw bank
    std::pair<char*,int> m_data;
    /// Flag to own data
    unsigned char        m_ownData;
  private:
    /// Assigment opertor is not allowed!
    RawDataAddress& operator=(const RawDataAddress& ) {
      return *this; 
    }
  public:
    /// Dummy constructor
  RawDataAddress() : GenericAddress(), m_data(0,0), m_ownData(0)  {
      m_par[1] = "0 ";
    }
    /// Copy Constructor
    RawDataAddress(const RawDataAddress& c) : GenericAddress(c), m_data(c.m_data), m_ownData(0)  {
    }
    /// Standard Constructor
    RawDataAddress( long svc,
                    const CLID& clid,
                    const std::string& p1="", 
                    const std::string& p2="",
                    unsigned long ip1=0,
                    unsigned long ip2=0)  
      : GenericAddress(svc,clid,p1,p2,ip1,ip2), m_data(0,0), m_ownData(0)
    {    }
    /// Standard Destructor
    virtual ~RawDataAddress()              {
      if ( m_ownData && m_data.first )  {
	m_ownData = 0;
	::free(m_data.first);
	m_data = std::pair<char*,int>(0,0);
      }
    }
    /// Pointer to raw buffer
    std::pair<char*,int> data()  const              { return m_data;           }
    /// Set data buffer
    void setData(const std::pair<char*,int>& val)   { m_data=val;              }
    /// Set data buffer with transfer ow ownership
    void adoptData(const std::pair<char*,int>& val) { m_data=val; m_ownData=1; }
    /// Access file offset
    unsigned long long int fileOffset() const {
      unsigned long long int giga_bytes  = par()[1][0]-'0';
      unsigned long long int local_bytes = ipar()[1];
      return GIGA_BYTE*giga_bytes + local_bytes;
    }
    /// Update file offset
    void setFileOffset(unsigned long long int offset) {
      char txt[2] = { char('0' + int(offset/GIGA_BYTE)), 0};
      m_ipar[1] = int(offset%GIGA_BYTE);
      m_par[1] = txt;
    }
  };
}
#endif // MDF_RAWDATAADDRESS_H
