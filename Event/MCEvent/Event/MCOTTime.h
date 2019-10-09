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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/OTChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  class MCOTDeposit;

  // Class ID definition
  static const CLID CLID_MCOTTime = 8002;

  // Namespace for locations in TDS
  namespace MCOTTimeLocation {
    inline const std::string Default = "/Event/MC/OT/Times";
  }

  /** @class MCOTTime MCOTTime.h
   *
   * MCOTTime are constucted from the MCOTDeposits
   *
   * @author Jeroen van Tilburg and Jacopo Nardulli
   *
   */

  class MCOTTime : public KeyedObject<LHCb::OTChannelID> {
  public:
    /// typedef for KeyedContainer of MCOTTime
    typedef KeyedContainer<MCOTTime, Containers::HashMap> Container;

    /// constructor with arguments
    MCOTTime( const LHCb::OTChannelID& aChannel, const SmartRefVector<LHCb::MCOTDeposit>& deposits );

    /// Default Constructor
    MCOTTime() {}

    /// Default Destructor
    virtual ~MCOTTime() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// get the OTChannelID from the key
    OTChannelID channel() const;

    /// get the TDC-time from the OTChannelID
    unsigned tdcTime() const;

    /// Retrieve (const)  References to all MCOTDeposits on this MCOTTime
    const SmartRefVector<LHCb::MCOTDeposit>& deposits() const;

    /// Add to  References to all MCOTDeposits on this MCOTTime
    void addToDeposits( const SmartRef<LHCb::MCOTDeposit>& value );

    /// Att to (pointer)  References to all MCOTDeposits on this MCOTTime
    void addToDeposits( const LHCb::MCOTDeposit* value );

    /// Remove from  References to all MCOTDeposits on this MCOTTime
    void removeFromDeposits( const SmartRef<LHCb::MCOTDeposit>& value );

    /// Clear  References to all MCOTDeposits on this MCOTTime
    void clearDeposits();

    friend std::ostream& operator<<( std::ostream& str, const MCOTTime& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    SmartRefVector<LHCb::MCOTDeposit> m_deposits; ///< References to all MCOTDeposits on this MCOTTime

  }; // class MCOTTime

  /// Definition of Keyed Container for MCOTTime
  typedef KeyedContainer<MCOTTime, Containers::HashMap> MCOTTimes;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations
#include "Event/MCOTDeposit.h"

inline LHCb::MCOTTime::MCOTTime( const LHCb::OTChannelID& aChannel, const SmartRefVector<LHCb::MCOTDeposit>& deposits )
    : m_deposits( deposits ) {
  this->setKey( aChannel );
}

inline const CLID& LHCb::MCOTTime::clID() const { return LHCb::MCOTTime::classID(); }

inline const CLID& LHCb::MCOTTime::classID() { return CLID_MCOTTime; }

inline std::ostream& LHCb::MCOTTime::fillStream( std::ostream& s ) const { return s; }

inline const SmartRefVector<LHCb::MCOTDeposit>& LHCb::MCOTTime::deposits() const { return m_deposits; }

inline void LHCb::MCOTTime::addToDeposits( const SmartRef<LHCb::MCOTDeposit>& value ) { m_deposits.push_back( value ); }

inline void LHCb::MCOTTime::addToDeposits( const LHCb::MCOTDeposit* value ) { m_deposits.push_back( value ); }

inline void LHCb::MCOTTime::removeFromDeposits( const SmartRef<LHCb::MCOTDeposit>& value ) {
  auto i = std::remove( m_deposits.begin(), m_deposits.end(), value );
  m_deposits.erase( i, m_deposits.end() );
}

inline void LHCb::MCOTTime::clearDeposits() { m_deposits.clear(); }

inline LHCb::OTChannelID LHCb::MCOTTime::channel() const { return key(); }

inline unsigned LHCb::MCOTTime::tdcTime() const { return channel().tdcTime(); }
