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
#include "Event/UTTELL1EventInfo.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTTELL1Data = 9010;

  // Namespace for locations in TDS
  namespace UTTELL1DataLocation {
    inline const std::string UTFull       = "Raw/UT/Full";
    inline const std::string UTPedestal   = "Raw/UT/Pedestal";
    inline const std::string UTProcFull   = "Raw/UT/ProcFull";
    inline const std::string UTPedSubADCs = "Raw/UT/PedSubADCs";
    inline const std::string UTSubPeds    = "Raw/UT/SubPeds";
    inline const std::string UTLCMSADCs   = "Raw/UT/LCMSADCs";
    inline const std::string UTEmulated   = "Raw/UT/Emulated";
  } // namespace UTTELL1DataLocation

  /** @class UTTELL1Data UTTELL1Data.h
   *
   * ADCs from a TELL1 board
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class UTTELL1Data : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of UTTELL1Data
    typedef KeyedContainer<UTTELL1Data, Containers::HashMap> Container;

    /// 2-d data vector of signed int typedef
    typedef std::vector<std::vector<signed int>> Data;
    /// Vector of UTTELL1EventInfo typedef
    typedef SmartRefVector<LHCb::UTTELL1EventInfo> Info;

    /// constructor with arguments
    UTTELL1Data( const Data& data, const Data& headers, const std::vector<unsigned int>& sentPP, const Info& info )
        : m_data( data ), m_header( headers ), m_sentPPs( sentPP ), m_eventInfo( info ) {}

    /// constructor with arguments
    UTTELL1Data( const Data& data ) : m_data( data ) {}

    /// constructor with key
    UTTELL1Data( const int numberOfTELL1 ) : KeyedObject<int>( numberOfTELL1 ), m_data(), m_header() {}

    /// Default Constructor
    UTTELL1Data() : m_data(), m_header(), m_sentPPs() {}

    /// Default Destructor
    virtual ~UTTELL1Data() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// data from pps that are read out
    std::vector<const LHCb::UTTELL1EventInfo*> validData() const;

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
    const SmartRefVector<LHCb::UTTELL1EventInfo>& eventInfo() const;

    /// Update  Vector of 4 Event Info block (1 per PP)
    void setEventInfo( const SmartRefVector<LHCb::UTTELL1EventInfo>& value );

    /// Add to  Vector of 4 Event Info block (1 per PP)
    void addToEventInfo( const SmartRef<LHCb::UTTELL1EventInfo>& value );

    /// Att to (pointer)  Vector of 4 Event Info block (1 per PP)
    void addToEventInfo( const LHCb::UTTELL1EventInfo* value );

    /// Remove from  Vector of 4 Event Info block (1 per PP)
    void removeFromEventInfo( const SmartRef<LHCb::UTTELL1EventInfo>& value );

    /// Clear  Vector of 4 Event Info block (1 per PP)
    void clearEventInfo();

  protected:
  private:
    Data                                   m_data;      ///< vector of raw data
    Data                                   m_header;    ///< vector of headers
    std::vector<unsigned int>              m_sentPPs;   ///< which pp sent data
    SmartRefVector<LHCb::UTTELL1EventInfo> m_eventInfo; ///< Vector of 4 Event Info block (1 per PP)

  }; // class UTTELL1Data

  /// Definition of Keyed Container for UTTELL1Data
  typedef KeyedContainer<UTTELL1Data, Containers::HashMap> UTTELL1Datas;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::UTTELL1Data::clID() const { return LHCb::UTTELL1Data::classID(); }

inline const CLID& LHCb::UTTELL1Data::classID() { return CLID_UTTELL1Data; }

inline const LHCb::UTTELL1Data::Data& LHCb::UTTELL1Data::data() const { return m_data; }

inline LHCb::UTTELL1Data::Data& LHCb::UTTELL1Data::data() { return m_data; }

inline void LHCb::UTTELL1Data::setData( const Data& value ) { m_data = value; }

inline const LHCb::UTTELL1Data::Data& LHCb::UTTELL1Data::header() const { return m_header; }

inline void LHCb::UTTELL1Data::setHeader( const Data& value ) { m_header = value; }

inline const std::vector<unsigned int>& LHCb::UTTELL1Data::sentPPs() const { return m_sentPPs; }

inline void LHCb::UTTELL1Data::setSentPPs( const std::vector<unsigned int>& value ) { m_sentPPs = value; }

inline const SmartRefVector<LHCb::UTTELL1EventInfo>& LHCb::UTTELL1Data::eventInfo() const { return m_eventInfo; }

inline void LHCb::UTTELL1Data::setEventInfo( const SmartRefVector<LHCb::UTTELL1EventInfo>& value ) {
  m_eventInfo = value;
}

inline void LHCb::UTTELL1Data::addToEventInfo( const SmartRef<LHCb::UTTELL1EventInfo>& value ) {
  m_eventInfo.push_back( value );
}

inline void LHCb::UTTELL1Data::addToEventInfo( const LHCb::UTTELL1EventInfo* value ) { m_eventInfo.push_back( value ); }

inline void LHCb::UTTELL1Data::removeFromEventInfo( const SmartRef<LHCb::UTTELL1EventInfo>& value ) {
  auto i = std::remove( m_eventInfo.begin(), m_eventInfo.end(), value );
  m_eventInfo.erase( i, m_eventInfo.end() );
}

inline void LHCb::UTTELL1Data::clearEventInfo() { m_eventInfo.clear(); }

inline unsigned int LHCb::UTTELL1Data::dataSize() const { return m_data.size(); }

inline unsigned int LHCb::UTTELL1Data::TELL1ID() const { return key(); }

inline const std::vector<signed int>& LHCb::UTTELL1Data::beetleData( unsigned char beetleID ) const {
  return m_data[beetleID];
}
