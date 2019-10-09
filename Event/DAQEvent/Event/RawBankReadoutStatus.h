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
#include "Event/RawBank.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <algorithm>
#include <map>
#include <numeric>
#include <ostream>
#include <sstream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Namespace for locations in TDS
  namespace RawBankReadoutStatusLocation {
    inline const std::string Default = "Transient/DAQ/Status";
    inline const std::string Ecal    = "Transient/DAQ/StatusEcal";
    inline const std::string Hcal    = "Transient/DAQ/StatusHcal";
  } // namespace RawBankReadoutStatusLocation

  /** @class RawBankReadoutStatus RawBankReadoutStatus.h
   *
   * Class for the status of RawBank decoding
   *
   * @author Olivier Deschamps
   *
   */

  class RawBankReadoutStatus final : public KeyedObject<LHCb::RawBank::BankType> {
  public:
    /// typedef for KeyedContainer of RawBankReadoutStatus
    typedef KeyedContainer<RawBankReadoutStatus, Containers::HashMap> Container;

    /// Status value
    enum Status {
      OK              = 0,
      Corrupted       = 1,
      Incomplete      = 2,
      Missing         = 4,
      Empty           = 8,
      NonUnique       = 16,
      Tell1Sync       = 32,
      Tell1Link       = 64,
      Tell1Error      = 128,
      ErrorBank       = 256,
      MissingStatus   = 512,
      BadMagicPattern = 1024,
      DuplicateEntry  = 2048,
      Unknown         = 4096
    };

    /// non-default constructor
    RawBankReadoutStatus( const LHCb::RawBank::BankType& type ) : Base( type ), m_status() {}

    /// Copy Constructor
    RawBankReadoutStatus( const LHCb::RawBankReadoutStatus& rh ) : Base( rh.key() ), m_status( rh.m_status ) {}

    /// Default Constructor
    RawBankReadoutStatus() : m_status() {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Assignment operator
    RawBankReadoutStatus& operator=( const LHCb::RawBankReadoutStatus& rhs );

    /// get Readout status for a given source
    long status( int source ) const;

    /// set Readout status for a given source
    void addStatus( int source, long stat );

    /// get Readout status for the whole bank
    long status() const;

    /// return the status map attribute
    const std::map<int, long>& statusMap() const;

    friend std::ostream& operator<<( std::ostream& str, const RawBankReadoutStatus& obj ) {
      return obj.fillStream( str );
    }

  protected:
    /// RawBankReadoutStatus key (BankType)
    typedef KeyedObject<LHCb::RawBank::BankType> Base;

  private:
    std::map<int, long> m_status; ///<  mapping of source status

  }; // class RawBankReadoutStatus

  /// Definition of Keyed Container for RawBankReadoutStatus
  typedef KeyedContainer<RawBankReadoutStatus, Containers::HashMap> RawBankReadoutStatuss;

  inline std::ostream& operator<<( std::ostream& s, LHCb::RawBankReadoutStatus::Status e ) {
    switch ( e ) {
    case LHCb::RawBankReadoutStatus::OK:
      return s << "OK";
    case LHCb::RawBankReadoutStatus::Corrupted:
      return s << "Corrupted";
    case LHCb::RawBankReadoutStatus::Incomplete:
      return s << "Incomplete";
    case LHCb::RawBankReadoutStatus::Missing:
      return s << "Missing";
    case LHCb::RawBankReadoutStatus::Empty:
      return s << "Empty";
    case LHCb::RawBankReadoutStatus::NonUnique:
      return s << "NonUnique";
    case LHCb::RawBankReadoutStatus::Tell1Sync:
      return s << "Tell1Sync";
    case LHCb::RawBankReadoutStatus::Tell1Link:
      return s << "Tell1Link";
    case LHCb::RawBankReadoutStatus::Tell1Error:
      return s << "Tell1Error";
    case LHCb::RawBankReadoutStatus::ErrorBank:
      return s << "ErrorBank";
    case LHCb::RawBankReadoutStatus::MissingStatus:
      return s << "MissingStatus";
    case LHCb::RawBankReadoutStatus::BadMagicPattern:
      return s << "BadMagicPattern";
    case LHCb::RawBankReadoutStatus::DuplicateEntry:
      return s << "DuplicateEntry";
    case LHCb::RawBankReadoutStatus::Unknown:
      return s << "Unknown";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RawBankReadoutStatus::Status";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::RawBankReadoutStatus::fillStream( std::ostream& s ) const {
  s << "{ "
    << "status :	" << m_status << std::endl
    << " }";
  return s;
}

inline LHCb::RawBankReadoutStatus& LHCb::RawBankReadoutStatus::operator=( const LHCb::RawBankReadoutStatus& rhs ) {

  if ( this != &rhs ) {
    m_status = rhs.m_status;
    if ( key() != rhs.key() ) setKey( rhs.key() );
  }
  return *this;
}

inline long LHCb::RawBankReadoutStatus::status( int source ) const {

  auto it = m_status.find( source );
  return it != m_status.end() ? it->second : long( Status::Unknown );
}

inline void LHCb::RawBankReadoutStatus::addStatus( int source, long stat ) {

  auto it = m_status.find( source );
  if ( it != m_status.end() )
    it->second |= stat;
  else
    m_status.emplace( source, stat );
}

inline long LHCb::RawBankReadoutStatus::status() const {

  return m_status.empty() ? long( Status::Unknown )
                          : std::accumulate( m_status.begin(), m_status.end(), 0L,
                                             []( long s, const std::pair<int, long>& p ) { return s | p.second; } );
}

inline const std::map<int, long>& LHCb::RawBankReadoutStatus::statusMap() const { return m_status; }
