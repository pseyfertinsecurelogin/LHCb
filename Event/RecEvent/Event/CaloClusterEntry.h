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
#include "Event/CaloDigit.h"
#include "Event/CaloDigitStatus.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class CaloClusterEntry CaloClusterEntry.h
   *
   * @brief Cluster Entry: Digit and it's Status in the Cluster * * * The class
   * represents the entry to the cluster: * the reference to CaloDigit objects, *
   * status of the digit in the cluster and fraction of * energy to be used
   * within the cluster * *
   *
   * @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *
   */

  class CaloClusterEntry final {
  public:
    /// Smart Reference to Calorimeter Digit object
    typedef SmartRef<LHCb::CaloDigit> Digit;
    /// Status of the digit (see the Mask enum in CaloDigitStatus.h)
    typedef LHCb::CaloDigitStatus::Status Status;
    /// Fraction of energy
    typedef double Fraction;

    /// Non-default constructor
    CaloClusterEntry( const Digit& d, const Status& s, const Fraction& f )
        : m_digit( d ), m_status( s ), m_fraction( f ) {}

    /// Non-default constructor
    CaloClusterEntry( const Digit& d, const Status& s ) : m_digit( d ), m_status( s ), m_fraction( 1 ) {}

    /// Default Constructor
    CaloClusterEntry() : m_digit(), m_status( LHCb::CaloDigitStatus::Undefined ), m_fraction( 1 ) {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// update the status @attention (add bits!)
    void addStatus( const Status& St );

    /// update the status @attention (remove bits!)
    void removeStatus( const Status& St );

    /// reset the status
    void resetStatus();

    /// Retrieve const  (Smart) Reference to the Calorimeter Digit object
    const Digit& digit() const;

    /// Retrieve  (Smart) Reference to the Calorimeter Digit object
    Digit& digit();

    /// Update  (Smart) Reference to the Calorimeter Digit object
    void setDigit( const Digit& value );

    /// Retrieve const  The status of Calorimeter Digit object in the Calorimeter Cluster
    const Status& status() const;

    /// Update  The status of Calorimeter Digit object in the Calorimeter Cluster
    void setStatus( const Status& value );

    /// Retrieve const  The fraction of energy of Calorimeter Digit object
    const Fraction& fraction() const;

    /// Update  The fraction of energy of Calorimeter Digit object
    void setFraction( const Fraction& value );

    friend std::ostream& operator<<( std::ostream& str, const CaloClusterEntry& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    Digit    m_digit;    ///< (Smart) Reference to the Calorimeter Digit object
    Status   m_status;   ///< The status of Calorimeter Digit object in the Calorimeter Cluster
    Fraction m_fraction; ///< The fraction of energy of Calorimeter Digit object

  }; // class CaloClusterEntry

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::CaloClusterEntry::fillStream( std::ostream& s ) const {
  s << "{ "
    << "digit :	" << m_digit << std::endl
    << "status :	" << m_status << std::endl
    << "fraction :	" << m_fraction << std::endl
    << " }";
  return s;
}

inline const LHCb::CaloClusterEntry::Digit& LHCb::CaloClusterEntry::digit() const { return m_digit; }

inline LHCb::CaloClusterEntry::Digit& LHCb::CaloClusterEntry::digit() { return m_digit; }

inline void LHCb::CaloClusterEntry::setDigit( const Digit& value ) { m_digit = value; }

inline const LHCb::CaloClusterEntry::Status& LHCb::CaloClusterEntry::status() const { return m_status; }

inline void LHCb::CaloClusterEntry::setStatus( const Status& value ) { m_status = value; }

inline const LHCb::CaloClusterEntry::Fraction& LHCb::CaloClusterEntry::fraction() const { return m_fraction; }

inline void LHCb::CaloClusterEntry::setFraction( const Fraction& value ) { m_fraction = value; }

inline void LHCb::CaloClusterEntry::addStatus( const Status& St ) { setStatus( St | status() ); }

inline void LHCb::CaloClusterEntry::removeStatus( const Status& St ) { setStatus( ( ~St ) & status() ); }

inline void LHCb::CaloClusterEntry::resetStatus() { setStatus( CaloDigitStatus::Undefined ); }
