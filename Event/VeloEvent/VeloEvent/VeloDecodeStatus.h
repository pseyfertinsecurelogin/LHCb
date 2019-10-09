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
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/SerializeSTL.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_VeloDecodeStatus = 4009;

  /** @class VeloDecodeStatus VeloDecodeStatus.h
   *
   * Record the status of the VELO Raw decoders for pattern recognition algs
   * later * * * Record the status of the VELO Raw decoders for pattern *
   * recognition algs later * *
   *
   * @author David Hutchcroft
   *
   */

  class VeloDecodeStatus : public DataObject {
  public:
    /// Default Constructor
    VeloDecodeStatus() : m_decoderStatus() {}

    /// Default Destructor
    virtual ~VeloDecodeStatus() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Set the status for a sensor
    void setDecoderState( unsigned int nSensor, bool decoderOK );

    /// Get the status for a sensor
    bool wasDecoded( unsigned int nSensor ) const;

    friend std::ostream& operator<<( std::ostream& str, const VeloDecodeStatus& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::map<unsigned int, bool> m_decoderStatus; ///< Output status for sensors as a map by sensor number (no entry =
                                                  ///< unknown)

  }; // class VeloDecodeStatus

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VeloDecodeStatus::clID() const { return LHCb::VeloDecodeStatus::classID(); }

inline const CLID& LHCb::VeloDecodeStatus::classID() { return CLID_VeloDecodeStatus; }

inline std::ostream& LHCb::VeloDecodeStatus::fillStream( std::ostream& s ) const {
  s << "{ "
    << "decoderStatus :	" << m_decoderStatus << std::endl
    << " }";
  return s;
}

inline void LHCb::VeloDecodeStatus::setDecoderState( unsigned int nSensor, bool decoderOK ) {

  m_decoderStatus[nSensor] = decoderOK;
}

inline bool LHCb::VeloDecodeStatus::wasDecoded( unsigned int nSensor ) const {

  if ( 0 == m_decoderStatus.count( nSensor ) ) return false;
  return m_decoderStatus.at( nSensor );
}
