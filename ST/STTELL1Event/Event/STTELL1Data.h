/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/STTELL1EventInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_STTELL1Data = 9010;

  // Namespace for locations in TDS
  namespace STTELL1DataLocation {
    inline const std::string ITFull       = "Raw/IT/Full";
    inline const std::string TTFull       = "Raw/TT/Full";
    inline const std::string ITPedestal   = "Raw/IT/Pedestal";
    inline const std::string TTPedestal   = "Raw/TT/Pedestal";
    inline const std::string ITProcFull   = "Raw/IT/ProcFull";
    inline const std::string TTProcFull   = "Raw/TT/ProcFull";
    inline const std::string ITPedSubADCs = "Raw/IT/PedSubADCs";
    inline const std::string TTPedSubADCs = "Raw/TT/PedSubADCs";
    inline const std::string ITSubPeds    = "Raw/IT/SubPeds";
    inline const std::string TTSubPeds    = "Raw/TT/SubPeds";
    inline const std::string ITLCMSADCs   = "Raw/IT/LCMSADCs";
    inline const std::string TTLCMSADCs   = "Raw/TT/LCMSADCs";
    inline const std::string ITEmulated   = "Raw/IT/Emulated";
    inline const std::string TTEmulated   = "Raw/TT/Emulated";
  } // namespace STTELL1DataLocation

  /** @class STTELL1Data STTELL1Data.h
   *
   * ADCs from a TELL1 board
   *
   * @author Matthew Needham
   *
   */

  class STTELL1Data : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of STTELL1Data
    typedef KeyedContainer<STTELL1Data, Containers::HashMap> Container;

    /// 2-d data vector of signed int typedef
    typedef std::vector<std::vector<signed int>> Data;
    /// Vector of STTELL1EventInfo typedef
    typedef SmartRefVector<LHCb::STTELL1EventInfo> Info;

    /// constructor with arguments
    STTELL1Data( const Data& data, const Data& headers, const std::vector<unsigned int>& sentPP, const Info& info )
        : m_data( data ), m_header( headers ), m_sentPPs( sentPP ), m_eventInfo( info ) {}

    /// constructor with arguments
    STTELL1Data( const Data& data ) : m_data( data ) {}

    /// constructor with key
    STTELL1Data( const int numberOfTELL1 ) : KeyedObject<int>( numberOfTELL1 ), m_data(), m_header() {}

    /// Default Constructor
    STTELL1Data() : m_data(), m_header(), m_sentPPs() {}

    /// Default Destructor
    virtual ~STTELL1Data() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// data from pps that are read out
    std::vector<const LHCb::STTELL1EventInfo*> validData() const;

    /// Print in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// size of TELL1 data
    unsigned int dataSize() const;

    /// Returns the TELL1D id
    unsigned int TELL1ID() const;

    /// Returns the data of a single beetle
    const std::vector<signed int>& beetleData( unsigned char beetleID ) const;

    /// Retrieve const  vector of raw data
    const Data& data() const;

    /// Retrieve  vector of raw data
    Data& data();

    /// Update  vector of raw data
    void setData( const Data& value );

    /// Retrieve const  vector of headers
    const Data& header() const;

    /// Update  vector of headers
    void setHeader( const Data& value );

    /// Retrieve const  which pp sent data
    const std::vector<unsigned int>& sentPPs() const;

    /// Update  which pp sent data
    void setSentPPs( const std::vector<unsigned int>& value );

    /// Retrieve (const)  Vector of 4 Event Info block (1 per PP)
    const SmartRefVector<LHCb::STTELL1EventInfo>& eventInfo() const;

    /// Update  Vector of 4 Event Info block (1 per PP)
    void setEventInfo( const SmartRefVector<LHCb::STTELL1EventInfo>& value );

    /// Add to  Vector of 4 Event Info block (1 per PP)
    void addToEventInfo( const SmartRef<LHCb::STTELL1EventInfo>& value );

    /// Att to (pointer)  Vector of 4 Event Info block (1 per PP)
    void addToEventInfo( const LHCb::STTELL1EventInfo* value );

    /// Remove from  Vector of 4 Event Info block (1 per PP)
    void removeFromEventInfo( const SmartRef<LHCb::STTELL1EventInfo>& value );

    /// Clear  Vector of 4 Event Info block (1 per PP)
    void clearEventInfo();

  protected:
  private:
    Data                                   m_data;      ///< vector of raw data
    Data                                   m_header;    ///< vector of headers
    std::vector<unsigned int>              m_sentPPs;   ///< which pp sent data
    SmartRefVector<LHCb::STTELL1EventInfo> m_eventInfo; ///< Vector of 4 Event Info block (1 per PP)

  }; // class STTELL1Data

  /// Definition of Keyed Container for STTELL1Data
  typedef KeyedContainer<STTELL1Data, Containers::HashMap> STTELL1Datas;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::STTELL1Data::clID() const { return LHCb::STTELL1Data::classID(); }

inline const CLID& LHCb::STTELL1Data::classID() { return CLID_STTELL1Data; }

inline const LHCb::STTELL1Data::Data& LHCb::STTELL1Data::data() const { return m_data; }

inline LHCb::STTELL1Data::Data& LHCb::STTELL1Data::data() { return m_data; }

inline void LHCb::STTELL1Data::setData( const Data& value ) { m_data = value; }

inline const LHCb::STTELL1Data::Data& LHCb::STTELL1Data::header() const { return m_header; }

inline void LHCb::STTELL1Data::setHeader( const Data& value ) { m_header = value; }

inline const std::vector<unsigned int>& LHCb::STTELL1Data::sentPPs() const { return m_sentPPs; }

inline void LHCb::STTELL1Data::setSentPPs( const std::vector<unsigned int>& value ) { m_sentPPs = value; }

inline const SmartRefVector<LHCb::STTELL1EventInfo>& LHCb::STTELL1Data::eventInfo() const { return m_eventInfo; }

inline void LHCb::STTELL1Data::setEventInfo( const SmartRefVector<LHCb::STTELL1EventInfo>& value ) {
  m_eventInfo = value;
}

inline void LHCb::STTELL1Data::addToEventInfo( const SmartRef<LHCb::STTELL1EventInfo>& value ) {
  m_eventInfo.push_back( value );
}

inline void LHCb::STTELL1Data::addToEventInfo( const LHCb::STTELL1EventInfo* value ) { m_eventInfo.push_back( value ); }

inline void LHCb::STTELL1Data::removeFromEventInfo( const SmartRef<LHCb::STTELL1EventInfo>& value ) {
  auto i = std::remove( m_eventInfo.begin(), m_eventInfo.end(), value );
  m_eventInfo.erase( i, m_eventInfo.end() );
}

inline void LHCb::STTELL1Data::clearEventInfo() { m_eventInfo.clear(); }

inline unsigned int LHCb::STTELL1Data::dataSize() const { return m_data.size(); }

inline unsigned int LHCb::STTELL1Data::TELL1ID() const { return key(); }

inline const std::vector<signed int>& LHCb::STTELL1Data::beetleData( unsigned char beetleID ) const {
  return m_data[beetleID];
}
