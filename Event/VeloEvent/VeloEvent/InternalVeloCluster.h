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
#include "Kernel/VeloChannelID.h"
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations
  using GaudiUtils::operator<<;

  // Class ID definition
  static const CLID CLID_InternalVeloCluster = 4104;

  // Namespace for locations in TDS
  namespace InternalVeloClusterLocation {
    inline const std::string Default = "Raw/Velo/InternalClusters";
  }

  /** @class InternalVeloCluster InternalVeloCluster.h
   *
   * A cluster as on the DAQ output, after the data processor and without
   * applying any additional processing. It contains no simulation info.
   *
   * @author Chris Parkes, update Tomasz Szumlak
   *
   */

  class InternalVeloCluster : public KeyedObject<LHCb::VeloChannelID> {
  public:
    /// typedef for KeyedContainer of InternalVeloCluster
    typedef KeyedContainer<InternalVeloCluster, Containers::HashMap> Container;

    /// Constructor with key
    InternalVeloCluster( const LHCb::VeloChannelID& key )
        : KeyedObject<LHCb::VeloChannelID>( key ), m_sensorID( 0 ), m_stripSignals() {}

    /// Default Constructor
    InternalVeloCluster() : m_sensorID(), m_stripSignals(), m_hasHighThreshold() {}

    /// Default Destructor
    virtual ~InternalVeloCluster() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// number of strips in cluster
    long size() const;

    /// adc value of strips in cluster
    double adcValue( long num ) const;

    /// Sensor number
    long sensor() const;

    /// strip number of strip in cluster
    long strip( long num ) const;

    /// Set the sensor number (overrides default)
    void setSensor( long num );

    /// combined sensor and strip number ID (key) of strip in cluster
    VeloChannelID channelID( long num ) const;

    /// Retrieve const  Sensor number stored as VeloChannelID
    const LHCb::VeloChannelID& sensorID() const;

    /// Update  Sensor number stored as VeloChannelID
    void setSensorID( const LHCb::VeloChannelID& value );

    /// Retrieve const  strip numbers and their signals
    const std::vector<std::pair<long, double>>& stripSignals() const;

    /// Update  strip numbers and their signals
    void setStripSignals( const std::vector<std::pair<long, double>>& value );

    /// Retrieve const  Is cluster over spillover threshold
    bool hasHighThreshold() const;

    /// Update  Is cluster over spillover threshold
    void setHasHighThreshold( bool value );

    friend std::ostream& operator<<( std::ostream& str, const InternalVeloCluster& obj ) {
      return obj.fillStream( str );
    }

  protected:
  private:
    LHCb::VeloChannelID                  m_sensorID;         ///< Sensor number stored as VeloChannelID
    std::vector<std::pair<long, double>> m_stripSignals;     ///< strip numbers and their signals
    bool                                 m_hasHighThreshold; ///< Is cluster over spillover threshold

  }; // class InternalVeloCluster

  /// Definition of Keyed Container for InternalVeloCluster
  typedef KeyedContainer<InternalVeloCluster, Containers::HashMap> InternalVeloClusters;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::InternalVeloCluster::clID() const { return LHCb::InternalVeloCluster::classID(); }

inline const CLID& LHCb::InternalVeloCluster::classID() { return CLID_InternalVeloCluster; }

inline std::ostream& LHCb::InternalVeloCluster::fillStream( std::ostream& s ) const {
  char l_hasHighThreshold = ( m_hasHighThreshold ) ? 'T' : 'F';
  s << "{ "
    << "sensorID :	" << m_sensorID << std::endl
    << "stripSignals :	" << m_stripSignals << std::endl
    << "hasHighThreshold :	" << l_hasHighThreshold << std::endl
    << " }";
  return s;
}

inline const LHCb::VeloChannelID& LHCb::InternalVeloCluster::sensorID() const { return m_sensorID; }

inline void LHCb::InternalVeloCluster::setSensorID( const LHCb::VeloChannelID& value ) { m_sensorID = value; }

inline const std::vector<std::pair<long, double>>& LHCb::InternalVeloCluster::stripSignals() const {
  return m_stripSignals;
}

inline void LHCb::InternalVeloCluster::setStripSignals( const std::vector<std::pair<long, double>>& value ) {
  m_stripSignals = value;
}

inline bool LHCb::InternalVeloCluster::hasHighThreshold() const { return m_hasHighThreshold; }

inline void LHCb::InternalVeloCluster::setHasHighThreshold( bool value ) { m_hasHighThreshold = value; }

inline long LHCb::InternalVeloCluster::size() const { return stripSignals().size(); }

inline double LHCb::InternalVeloCluster::adcValue( long num ) const { return stripSignals()[num].second; }

inline long LHCb::InternalVeloCluster::sensor() const { return m_sensorID.sensor(); }

inline long LHCb::InternalVeloCluster::strip( long num ) const { return stripSignals()[num].first; }

inline void LHCb::InternalVeloCluster::setSensor( long num ) { m_sensorID.setSensor( num ); }

inline LHCb::VeloChannelID LHCb::InternalVeloCluster::channelID( long num ) const {

  return VeloChannelID( m_sensorID.sensor(), strip( num ), m_sensorID.type() );
}
