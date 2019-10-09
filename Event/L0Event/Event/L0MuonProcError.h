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
#include "GaudiKernel/SerializeSTL.h"
#include "Kernel/MuonTileID.h"
#include <map>
#include <ostream>
#include <string>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_L0MuonProcError = 6010;

  // Namespace for locations in TDS
  namespace L0MuonProcErrorLocation {
    inline const std::string Default = "Trig/L0/MuonProcError";
  }

  /** @class L0MuonProcError L0MuonProcError.h
   *
   * L0Muon trigger processing board errors * representing the errors detected on
   * a processing board.
   *
   * @author J. Cogan
   *
   */

  class L0MuonProcError final : public KeyedObject<LHCb::MuonTileID> {
  public:
    /// typedef for KeyedContainer of L0MuonProcError
    typedef KeyedContainer<L0MuonProcError, Containers::HashMap> Container;

    /// Constructor
    L0MuonProcError( const LHCb::MuonTileID& mid, int ib )
        : KeyedObject<LHCb::MuonTileID>( mid )
        , m_board_index( ib )
        , m_decoding( false )
        , m_header( 0 )
        , m_bcsu_bcid_error( -1 )
        , m_pus_bcid_error()
        , m_bcsu_link_error( 0 )
        , m_pus_opt_link_error()
        , m_pus_par_link_error()
        , m_pus_ser_link_error()
        , m_bcsu_status( 0 )
        , m_pus_status() {}

    /// Default Constructor
    L0MuonProcError()
        : m_board_index( 0 )
        , m_decoding()
        , m_header( 0 )
        , m_bcsu_bcid_error( 0 )
        , m_pus_bcid_error()
        , m_bcsu_link_error( 0 )
        , m_pus_opt_link_error()
        , m_pus_par_link_error()
        , m_pus_ser_link_error()
        , m_bcsu_status( 0 )
        , m_pus_status() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Return true if no error was detected
    bool isEmpty();

    /// Add a pu with a pathological BCID
    void setPu_bcid_error( int ind_pu, int bcid );

    /// Add a pu with a pathological STATUS
    void setPu_status( int ind_pu, int status );

    /// Add a pu with a opt link error
    void setPu_opt_link_error( int ind_pu, int opt_link );

    /// Add a pu with a ser link error
    void setPu_ser_link_error( int ind_pu, int ser_link );

    /// Add a pu with a par link error
    void setPu_par_link_error( int ind_pu, int par_link );

    /// Return the pu pathological BCID
    int pu_bcid_error( int ind_pu );

    /// Return the pu pathological STATUS
    int pu_status( int ind_pu );

    /// Return the pu opt link error
    int pu_opt_link_error( int ind_pu );

    /// Return the pu ser link error
    int pu_ser_link_error( int ind_pu );

    /// Return the pu par link error
    int pu_par_link_error( int ind_pu );

    /// Retrieve const  Index of the processing board
    int board_index() const;

    /// Update  Index of the processing board
    void setBoard_index( int value );

    /// Retrieve const  data corruption flag
    bool decoding() const;

    /// Update  data corruption flag
    void setDecoding( bool value );

    /// Retrieve const  Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    int header() const;

    /// Update  Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    void setHeader( int value );

    /// Retrieve const  pathological BCSU BCID - 4 bits
    int bcsu_bcid_error() const;

    /// Update  pathological BCSU BCID - 4 bits
    void setBcsu_bcid_error( int value );

    /// Retrieve const  Error on BCSU input links (4 bits for H types + 4 bits for F types)
    int bcsu_link_error() const;

    /// Update  Error on BCSU input links (4 bits for H types + 4 bits for F types)
    void setBcsu_link_error( int value );

    /// Retrieve const  Pathological BCSU status - 4 bits
    int bcsu_status() const;

    /// Update  Pathological BCSU status - 4 bits
    void setBcsu_status( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0MuonProcError& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    int  m_board_index; ///< Index of the processing board
    bool m_decoding;    ///< data corruption flag
    int  m_header; ///< Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    int  m_bcsu_bcid_error;                  ///< pathological BCSU BCID - 4 bits
    std::map<int, int> m_pus_bcid_error;     ///< Map of PUs with pathological BCID - key= PU index, value = 4 bits BCID
    int                m_bcsu_link_error;    ///< Error on BCSU input links (4 bits for H types + 4 bits for F types)
    std::map<int, int> m_pus_opt_link_error; ///< Map of PUs with errors on opt. links - key= PU index, value = 8 bits
                                             ///< err. wd
    std::map<int, int> m_pus_par_link_error; ///< Map of PUs with errors on par. links - key= PU index, value = 7 bits
                                             ///< err. wd
    std::map<int, int> m_pus_ser_link_error; ///< Map of PUs with errors on ser. links - key= PU index, value = 6 bits
                                             ///< err. wd
    int                m_bcsu_status;        ///< Pathological BCSU status - 4 bits
    std::map<int, int> m_pus_status; ///< Map of PUs with pathological status - key= PU index, value = 4 bits status

  }; // class L0MuonProcError

  /// Definition of Keyed Container for L0MuonProcError
  typedef KeyedContainer<L0MuonProcError, Containers::HashMap> L0MuonProcErrors;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonProcError::clID() const { return LHCb::L0MuonProcError::classID(); }

inline const CLID& LHCb::L0MuonProcError::classID() { return CLID_L0MuonProcError; }

inline std::ostream& LHCb::L0MuonProcError::fillStream( std::ostream& s ) const {
  char l_decoding = ( m_decoding ) ? 'T' : 'F';
  s << "{ "
    << "board_index :	" << m_board_index << std::endl
    << "decoding :	" << l_decoding << std::endl
    << "header :	" << m_header << std::endl
    << "bcsu_bcid_error :	" << m_bcsu_bcid_error << std::endl
    << "pus_bcid_error :	" << m_pus_bcid_error << std::endl
    << "bcsu_link_error :	" << m_bcsu_link_error << std::endl
    << "pus_opt_link_error :	" << m_pus_opt_link_error << std::endl
    << "pus_par_link_error :	" << m_pus_par_link_error << std::endl
    << "pus_ser_link_error :	" << m_pus_ser_link_error << std::endl
    << "bcsu_status :	" << m_bcsu_status << std::endl
    << "pus_status :	" << m_pus_status << std::endl
    << " }";
  return s;
}

inline int LHCb::L0MuonProcError::board_index() const { return m_board_index; }

inline void LHCb::L0MuonProcError::setBoard_index( int value ) { m_board_index = value; }

inline bool LHCb::L0MuonProcError::decoding() const { return m_decoding; }

inline void LHCb::L0MuonProcError::setDecoding( bool value ) { m_decoding = value; }

inline int LHCb::L0MuonProcError::header() const { return m_header; }

inline void LHCb::L0MuonProcError::setHeader( int value ) { m_header = value; }

inline int LHCb::L0MuonProcError::bcsu_bcid_error() const { return m_bcsu_bcid_error; }

inline void LHCb::L0MuonProcError::setBcsu_bcid_error( int value ) { m_bcsu_bcid_error = value; }

inline int LHCb::L0MuonProcError::bcsu_link_error() const { return m_bcsu_link_error; }

inline void LHCb::L0MuonProcError::setBcsu_link_error( int value ) { m_bcsu_link_error = value; }

inline int LHCb::L0MuonProcError::bcsu_status() const { return m_bcsu_status; }

inline void LHCb::L0MuonProcError::setBcsu_status( int value ) { m_bcsu_status = value; }

inline bool LHCb::L0MuonProcError::isEmpty() {

  if ( m_decoding ) return false;
  if ( m_header > 0 ) return false;

  if ( m_bcsu_bcid_error >= 0 ) return false;
  if ( m_pus_bcid_error.size() > 0 ) return false;

  if ( m_bcsu_link_error > 0 ) return false;
  if ( m_pus_opt_link_error.size() > 0 ) return false;
  if ( m_pus_par_link_error.size() > 0 ) return false;
  if ( m_pus_ser_link_error.size() > 0 ) return false;

  if ( m_bcsu_status > 0 ) return false;
  if ( m_pus_status.size() > 0 ) return false;

  return true;
}

inline void LHCb::L0MuonProcError::setPu_bcid_error( int ind_pu, int bcid ) { m_pus_bcid_error[ind_pu] = bcid; }

inline void LHCb::L0MuonProcError::setPu_status( int ind_pu, int status ) { m_pus_status[ind_pu] = status; }

inline void LHCb::L0MuonProcError::setPu_opt_link_error( int ind_pu, int opt_link ) {

  m_pus_opt_link_error[ind_pu] = opt_link;
}

inline void LHCb::L0MuonProcError::setPu_ser_link_error( int ind_pu, int ser_link ) {

  m_pus_ser_link_error[ind_pu] = ser_link;
}

inline void LHCb::L0MuonProcError::setPu_par_link_error( int ind_pu, int par_link ) {

  m_pus_par_link_error[ind_pu] = par_link;
}

inline int LHCb::L0MuonProcError::pu_bcid_error( int ind_pu ) {

  if ( m_pus_bcid_error.find( ind_pu ) != m_pus_bcid_error.end() ) return m_pus_bcid_error[ind_pu];
  return -1;
}

inline int LHCb::L0MuonProcError::pu_status( int ind_pu ) {

  if ( m_pus_status.find( ind_pu ) != m_pus_status.end() ) return m_pus_status[ind_pu];
  return 0;
}

inline int LHCb::L0MuonProcError::pu_opt_link_error( int ind_pu ) {

  if ( m_pus_opt_link_error.find( ind_pu ) != m_pus_opt_link_error.end() ) return m_pus_opt_link_error[ind_pu];
  return 0;
}

inline int LHCb::L0MuonProcError::pu_ser_link_error( int ind_pu ) {

  if ( m_pus_ser_link_error.find( ind_pu ) != m_pus_ser_link_error.end() ) return m_pus_ser_link_error[ind_pu];
  return 0;
}

inline int LHCb::L0MuonProcError::pu_par_link_error( int ind_pu ) {

  if ( m_pus_par_link_error.find( ind_pu ) != m_pus_par_link_error.end() ) return m_pus_par_link_error[ind_pu];
  return 0;
}
