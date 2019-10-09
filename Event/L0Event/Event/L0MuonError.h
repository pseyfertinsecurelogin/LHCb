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
#include "Kernel/MuonTileID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_L0MuonError = 6011;

  // Namespace for locations in TDS
  namespace L0MuonErrorLocation {
    inline const std::string Default  = "Trig/L0/MuonErrorCtrl";
    inline const std::string CtrlCUSU = "Trig/L0/MuonErrorCtrl";
    inline const std::string CtrlBCSU = "Trig/L0/MuonErrorBCSUCtrl";
    inline const std::string ProcBCSU = "Trig/L0/MuonErrorBCSU";
    inline const std::string ProcPU   = "Trig/L0/MuonErrorPU";
  } // namespace L0MuonErrorLocation

  /** @class L0MuonError L0MuonError.h
   *
   * L0Muon trigger errors * representing the errors detected in the L0Muon
   * system.
   *
   * @author J. Cogan
   *
   */

  class L0MuonError final : public KeyedObject<LHCb::MuonTileID> {
  public:
    /// typedef for KeyedContainer of L0MuonError
    typedef KeyedContainer<L0MuonError, Containers::HashMap> Container;

    /// Constructor
    L0MuonError( const LHCb::MuonTileID& mid )
        : KeyedObject<LHCb::MuonTileID>( mid ), m_decoding( 0 ), m_hardware( 0 ), m_status( 0 ), m_bcid( 0 ) {}

    /// Default Constructor
    L0MuonError() : m_decoding( 0 ), m_hardware( 0 ), m_status( 0 ), m_bcid( 0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// update input argument with quarter, board and pu index
    void index( int& quarter, int& board, int& pu ) const;

    /// Avoids hiding base class method, ICC warning 1125
    long index() const override;

    /// True if an error occurred in the bank containing data
    bool decoding_data() const;

    /// True if an error occurred in the bank containing candidates
    bool decoding_cand() const;

    /// True if an overflow was reported (more than 2 candidates in a PU)
    bool overflow() const;

    /// True if an internal error was reported (wrong bcid/input links)
    bool internal() const;

    /// Retrieve const  Decoding error
    int decoding() const;

    /// Update  Decoding error
    void setDecoding( int value );

    /// Retrieve const  Hardware error
    int hardware() const;

    /// Update  Hardware error
    void setHardware( int value );

    /// Retrieve const  Status if in error
    int status() const;

    /// Update  Status if in error
    void setStatus( int value );

    /// Retrieve const  BCID if in error
    int bcid() const;

    /// Update  BCID if in error
    void setBcid( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0MuonError& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int m_decoding; ///< Decoding error
    int m_hardware; ///< Hardware error
    int m_status;   ///< Status if in error
    int m_bcid;     ///< BCID if in error

  }; // class L0MuonError

  /// Definition of Keyed Container for L0MuonError
  typedef KeyedContainer<L0MuonError, Containers::HashMap> L0MuonErrors;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonError::clID() const { return LHCb::L0MuonError::classID(); }

inline const CLID& LHCb::L0MuonError::classID() { return CLID_L0MuonError; }

inline std::ostream& LHCb::L0MuonError::fillStream( std::ostream& s ) const {
  s << "{ "
    << "decoding :	" << m_decoding << std::endl
    << "hardware :	" << m_hardware << std::endl
    << "status :	" << m_status << std::endl
    << "bcid :	" << m_bcid << std::endl
    << " }";
  return s;
}

inline int LHCb::L0MuonError::decoding() const { return m_decoding; }

inline void LHCb::L0MuonError::setDecoding( int value ) { m_decoding = value; }

inline int LHCb::L0MuonError::hardware() const { return m_hardware; }

inline void LHCb::L0MuonError::setHardware( int value ) { m_hardware = value; }

inline int LHCb::L0MuonError::status() const { return m_status; }

inline void LHCb::L0MuonError::setStatus( int value ) { m_status = value; }

inline int LHCb::L0MuonError::bcid() const { return m_bcid; }

inline void LHCb::L0MuonError::setBcid( int value ) { m_bcid = value; }

inline long LHCb::L0MuonError::index() const { return ContainedObject::index(); }

inline bool LHCb::L0MuonError::decoding_data() const { return ( ( m_decoding & 0x2 ) > 0 ); }

inline bool LHCb::L0MuonError::decoding_cand() const { return ( ( m_decoding & 0x1 ) > 0 ); }

inline bool LHCb::L0MuonError::overflow() const { return ( m_status & 0x3 ) > 0; }

inline bool LHCb::L0MuonError::internal() const {
  return ( ( ( m_status & 0xC ) > 0 ) || ( m_hardware > 0 ) || ( m_bcid > 0 ) );
}
