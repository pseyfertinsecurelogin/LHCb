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
#include "Kernel/OTChannelID.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_OTTime = 8001;

  // Namespace for locations in TDS
  namespace OTTimeLocation {
    inline const std::string Default = "Raw/OT/Times";
  }

  /** @class OTTime OTTime.h
   *
   * For each MCOTTime an OTTime is made
   *
   * @author Jacopo Nardulli
   *
   */

  class OTTime final : public KeyedObject<LHCb::OTChannelID> {
  public:
    /// typedef for KeyedContainer of OTTime
    typedef KeyedContainer<OTTime, Containers::HashMap> Container;

    /// constructor with arguments
    OTTime( const LHCb::OTChannelID& aChannel, double calTime );

    /// Default Constructor
    OTTime() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// get the OTChannelID from the key
    [[nodiscard]] OTChannelID channel() const;

    /// get the TDC-time from the key
    [[nodiscard]] unsigned tdcTime() const;

    /// Retrieve const  t0 corrected; calibratedTime=propagationTime+driftTime
    [[nodiscard]] double calibratedTime() const;

    /// Update  t0 corrected; calibratedTime=propagationTime+driftTime
    void setCalibratedTime( double value );

    friend std::ostream& operator<<( std::ostream& str, const OTTime& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    double m_calibratedTime{0.0}; ///< t0 corrected; calibratedTime=propagationTime+driftTime

  }; // class OTTime

  /// Definition of Keyed Container for OTTime
  typedef KeyedContainer<OTTime, Containers::HashMap> OTTimes;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::OTTime::OTTime( const LHCb::OTChannelID& aChannel, double calTime ) : m_calibratedTime( calTime ) {
  this->setKey( aChannel );
}

inline const CLID& LHCb::OTTime::clID() const { return LHCb::OTTime::classID(); }

inline const CLID& LHCb::OTTime::classID() { return CLID_OTTime; }

inline std::ostream& LHCb::OTTime::fillStream( std::ostream& s ) const {
  s << "{ "
    << "calibratedTime :	" << (float)m_calibratedTime << std::endl
    << " }";
  return s;
}

inline double LHCb::OTTime::calibratedTime() const { return m_calibratedTime; }

inline void LHCb::OTTime::setCalibratedTime( double value ) { m_calibratedTime = value; }

inline LHCb::OTChannelID LHCb::OTTime::channel() const { return key(); }

inline unsigned LHCb::OTTime::tdcTime() const { return channel().tdcTime(); }
