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
#include "Kernel/VPChannelID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_VPCluster = 4504;

  // Namespace for locations in TDS
  namespace VPClusterLocation {
    inline const std::string Default = "Raw/VP/Clusters";
  }

  /** @class VPCluster VPCluster.h
   *
   * This class represents a cluster of pixel hits
   *
   * @author Marcin Kucharczyk
   *
   */

  class VPCluster final : public KeyedObject<LHCb::VPChannelID> {
  public:
    /// typedef for KeyedContainer of VPCluster
    typedef KeyedContainer<VPCluster, Containers::HashMap> Container;

    /// Constructor
    VPCluster( const std::pair<double, double>& fraction, const double x, const double y, const double z,
               const std::vector<LHCb::VPChannelID>& pixels )
        : m_fraction( fraction ), m_x( x ), m_y( y ), m_z( z ), m_pixels( pixels ) {}

    /// Copy constructor
    VPCluster( const LHCb::VPCluster& cluster )
        : KeyedObject<LHCb::VPChannelID>()
        , m_fraction( cluster.m_fraction )
        , m_x( cluster.m_x )
        , m_y( cluster.m_y )
        , m_z( cluster.m_z )
        , m_pixels( cluster.m_pixels ) {}

    /// Default Constructor
    VPCluster() : m_fraction(), m_x( 0.0 ), m_y( 0.0 ), m_z( 0.0 ), m_pixels() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Clone method
    VPCluster* clone() const;

    /// Return the cluster channelID = key
    VPChannelID channelID() const;

    /// Print the cluster key = channelID
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  inter-pixel fraction
    const std::pair<double, double>& fraction() const;

    /// Update  inter-pixel fraction
    void setFraction( const std::pair<double, double>& value );

    /// Retrieve const  global x coordinate
    double x() const;

    /// Update  global x coordinate
    void setX( double value );

    /// Retrieve const  global y coordinate
    double y() const;

    /// Update  global y coordinate
    void setY( double value );

    /// Retrieve const  global z coordinate
    double z() const;

    /// Update  global z coordinate
    void setZ( double value );

    /// Retrieve const  vector of channel IDs
    const std::vector<LHCb::VPChannelID>& pixels() const;

    /// Update  vector of channel IDs
    void setPixels( const std::vector<LHCb::VPChannelID>& value );

    friend std::ostream& operator<<( std::ostream& str, const VPCluster& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    std::pair<double, double>      m_fraction; ///< inter-pixel fraction
    double                         m_x;        ///< global x coordinate
    double                         m_y;        ///< global y coordinate
    double                         m_z;        ///< global z coordinate
    std::vector<LHCb::VPChannelID> m_pixels;   ///< vector of channel IDs

  }; // class VPCluster

  /// Definition of Keyed Container for VPCluster
  typedef KeyedContainer<VPCluster, Containers::HashMap> VPClusters;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::VPCluster::clID() const { return LHCb::VPCluster::classID(); }

inline const CLID& LHCb::VPCluster::classID() { return CLID_VPCluster; }

inline const std::pair<double, double>& LHCb::VPCluster::fraction() const { return m_fraction; }

inline void LHCb::VPCluster::setFraction( const std::pair<double, double>& value ) { m_fraction = value; }

inline double LHCb::VPCluster::x() const { return m_x; }

inline void LHCb::VPCluster::setX( double value ) { m_x = value; }

inline double LHCb::VPCluster::y() const { return m_y; }

inline void LHCb::VPCluster::setY( double value ) { m_y = value; }

inline double LHCb::VPCluster::z() const { return m_z; }

inline void LHCb::VPCluster::setZ( double value ) { m_z = value; }

inline const std::vector<LHCb::VPChannelID>& LHCb::VPCluster::pixels() const { return m_pixels; }

inline void LHCb::VPCluster::setPixels( const std::vector<LHCb::VPChannelID>& value ) { m_pixels = value; }

inline LHCb::VPCluster* LHCb::VPCluster::clone() const { return new LHCb::VPCluster( *this ); }

inline LHCb::VPChannelID LHCb::VPCluster::channelID() const { return key(); }

inline std::ostream& LHCb::VPCluster::fillStream( std::ostream& s ) const {

  s << "{VPCluster's key: " << (int)key() << "}";
  return s;
}
