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
#include <map>
#include <ostream>
#include <string>
#include <utility>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_L0MuonCtrlError = 6008;

  // Namespace for locations in TDS
  namespace L0MuonCtrlErrorLocation {
    inline const std::string Default = "Trig/L0/MuonCtrlError";
  }

  /** @class L0MuonCtrlError L0MuonCtrlError.h
   *
   * L0Muon trigger controller board errors * representing the errors detected on
   * a controller board.
   *
   * @author J. Cogan
   *
   */

  class L0MuonCtrlError final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of L0MuonCtrlError
    typedef KeyedContainer<L0MuonCtrlError, Containers::HashMap> Container;

    /// Constructor
    L0MuonCtrlError( int iq )
        : KeyedObject<int>( iq )
        , m_decoding( false )
        , m_header( 0 )
        , m_cu_bcid_error( -1 )
        , m_su_bcid_error( -1 )
        , m_bcsus_bcid_error()
        , m_cu_link_error()
        , m_su_link_error()
        , m_status( 0 )
        , m_bcsus_status() {}

    /// Default Constructor
    L0MuonCtrlError()
        : m_decoding()
        , m_header( 0 )
        , m_cu_bcid_error( 0 )
        , m_su_bcid_error( 0 )
        , m_bcsus_bcid_error()
        , m_cu_link_error()
        , m_su_link_error()
        , m_status( 0 )
        , m_bcsus_status() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Return true if no error was detected
    bool isEmpty();

    /// Add a BCSU with a pathological BCID
    void setBcsu_bcid_error( int ind_bcsu, int bcid1, int bcid2 );

    /// Add a bcsu with a pathological STATUS
    void setBcsu_status( int ind_bcsu, int status );

    /// Add a PB with a link error on CU
    void setCu_link_error( int ind_pb, int err );

    /// Add a PB with a link error on SU
    void setSu_link_error( int ind_pb, int err );

    /// Return the BCSU pathological BCID
    int bcsu_bcid_error( int ind_bcsu, int ich );

    /// Return the bcsu pathological STATUS
    int bcsu_status( int ind_bcsu );

    /// Return the PB error on CU
    int cu_link_error( int ind_pb );

    /// Return the PB error on SU
    int su_link_error( int ind_pb );

    /// Retrieve const  data corruption flag
    bool decoding() const;

    /// Update  data corruption flag
    void setDecoding( bool value );

    /// Retrieve const  Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    int header() const;

    /// Update  Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    void setHeader( int value );

    /// Retrieve const  pathological CU BCID - 4 bits
    int cu_bcid_error() const;

    /// Update  pathological CU BCID - 4 bits
    void setCu_bcid_error( int value );

    /// Retrieve const  pathological SU BCID - 4 bits
    int su_bcid_error() const;

    /// Update  pathological SU BCID - 4 bits
    void setSu_bcid_error( int value );

    /// Retrieve const  Pathological status - 4 bits
    int status() const;

    /// Update  Pathological status - 4 bits
    void setStatus( int value );

    friend std::ostream& operator<<( std::ostream& str, const L0MuonCtrlError& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    bool m_decoding; ///< data corruption flag
    int  m_header; ///< Error on frame header (6 bits : L0EventNumber + L0_B_ID + BoardIndex x 2 transmission channels)
    int  m_cu_bcid_error;                                  ///< pathological CU BCID - 4 bits
    int  m_su_bcid_error;                                  ///< pathological SU BCID - 4 bits
    std::map<int, std::pair<int, int>> m_bcsus_bcid_error; ///< Map of BCSUs with pathological BCID - key= PU index,
                                                           ///< value = 4 bits BCID x 2 transmission channels
    std::map<int, int> m_cu_link_error; ///< Map of PB with errors on CU links - key= BCSU index, value = 2 bits err. wd
    std::map<int, int> m_su_link_error; ///< Map of PB with errors on SU links - key= BCSU index, value = 3 bits err. wd
    int                m_status;        ///< Pathological status - 4 bits
    std::map<int, int> m_bcsus_status;  ///< Map of BCSUs with pathological status - key= BCSU index, value = 4 bits
                                        ///< status

  }; // class L0MuonCtrlError

  /// Definition of Keyed Container for L0MuonCtrlError
  typedef KeyedContainer<L0MuonCtrlError, Containers::HashMap> L0MuonCtrlErrors;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::L0MuonCtrlError::clID() const { return LHCb::L0MuonCtrlError::classID(); }

inline const CLID& LHCb::L0MuonCtrlError::classID() { return CLID_L0MuonCtrlError; }

inline std::ostream& LHCb::L0MuonCtrlError::fillStream( std::ostream& s ) const {
  char l_decoding = ( m_decoding ) ? 'T' : 'F';
  s << "{ "
    << "decoding :	" << l_decoding << std::endl
    << "header :	" << m_header << std::endl
    << "cu_bcid_error :	" << m_cu_bcid_error << std::endl
    << "su_bcid_error :	" << m_su_bcid_error << std::endl
    << "bcsus_bcid_error :	" << m_bcsus_bcid_error << std::endl
    << "cu_link_error :	" << m_cu_link_error << std::endl
    << "su_link_error :	" << m_su_link_error << std::endl
    << "status :	" << m_status << std::endl
    << "bcsus_status :	" << m_bcsus_status << std::endl
    << " }";
  return s;
}

inline bool LHCb::L0MuonCtrlError::decoding() const { return m_decoding; }

inline void LHCb::L0MuonCtrlError::setDecoding( bool value ) { m_decoding = value; }

inline int LHCb::L0MuonCtrlError::header() const { return m_header; }

inline void LHCb::L0MuonCtrlError::setHeader( int value ) { m_header = value; }

inline int LHCb::L0MuonCtrlError::cu_bcid_error() const { return m_cu_bcid_error; }

inline void LHCb::L0MuonCtrlError::setCu_bcid_error( int value ) { m_cu_bcid_error = value; }

inline int LHCb::L0MuonCtrlError::su_bcid_error() const { return m_su_bcid_error; }

inline void LHCb::L0MuonCtrlError::setSu_bcid_error( int value ) { m_su_bcid_error = value; }

inline int LHCb::L0MuonCtrlError::status() const { return m_status; }

inline void LHCb::L0MuonCtrlError::setStatus( int value ) { m_status = value; }

inline bool LHCb::L0MuonCtrlError::isEmpty() {

  if ( m_decoding ) return false;
  if ( m_header > 0 ) return false;

  if ( m_cu_bcid_error >= 0 ) return false;
  if ( m_su_bcid_error >= 0 ) return false;
  if ( m_bcsus_bcid_error.size() > 0 ) return false;

  if ( m_cu_link_error.size() > 0 ) return false;
  if ( m_su_link_error.size() > 0 ) return false;

  if ( m_status > 0 ) return false;
  if ( m_bcsus_status.size() > 0 ) return false;

  return true;
}

inline void LHCb::L0MuonCtrlError::setBcsu_bcid_error( int ind_bcsu, int bcid1, int bcid2 ) {

  m_bcsus_bcid_error[ind_bcsu] = std::pair<int, int>( bcid1, bcid2 );
}

inline void LHCb::L0MuonCtrlError::setBcsu_status( int ind_bcsu, int status ) { m_bcsus_status[ind_bcsu] = status; }

inline void LHCb::L0MuonCtrlError::setCu_link_error( int ind_pb, int err ) { m_cu_link_error[ind_pb] = err; }

inline void LHCb::L0MuonCtrlError::setSu_link_error( int ind_pb, int err ) { m_su_link_error[ind_pb] = err; }

inline int LHCb::L0MuonCtrlError::bcsu_bcid_error( int ind_bcsu, int ich ) {

  if ( m_bcsus_bcid_error.find( ind_bcsu ) != m_bcsus_bcid_error.end() ) {
    switch ( ich ) {
    case 0:
      return m_bcsus_bcid_error[ind_bcsu].first;
    case 1:
      return m_bcsus_bcid_error[ind_bcsu].second;
    default:
      return -1;
    }
  }
  return -1;
}

inline int LHCb::L0MuonCtrlError::bcsu_status( int ind_bcsu ) {

  if ( m_bcsus_status.find( ind_bcsu ) != m_bcsus_status.end() ) return m_bcsus_status[ind_bcsu];
  return 0;
}

inline int LHCb::L0MuonCtrlError::cu_link_error( int ind_pb ) {

  if ( m_cu_link_error.find( ind_pb ) != m_cu_link_error.end() ) return m_cu_link_error[ind_pb];
  return 0;
}

inline int LHCb::L0MuonCtrlError::su_link_error( int ind_pb ) {

  if ( m_su_link_error.find( ind_pb ) != m_su_link_error.end() ) return m_su_link_error[ind_pb];
  return 0;
}
