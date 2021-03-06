/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef RecEvent_VPLightCluster_H
#define RecEvent_VPLightCluster_H 1

// Include files
#include "Kernel/VPChannelID.h"

#include "GaudiKernel/ClassID.h"

#include <boost/container/flat_map.hpp>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Namespace for locations in TDS
  namespace VPClusterLocation {
    inline const std::string Light = "Raw/VP/LightClusters";
  }

  class VPLightCluster final {
  public:
    /// Constructor
    VPLightCluster( const unsigned char xfraction, const unsigned char yfraction, const float x, const float y,
                    const float z, const unsigned vpID )
        : m_fx( xfraction ), m_fy( yfraction ), m_x( x ), m_y( y ), m_z( z ), m_vpID( vpID ) {}

    /// Return the cluster channelID = key
    [[nodiscard]] VPChannelID channelID() const noexcept { return m_vpID; }

    /// Print the cluster key = channelID
    std::ostream& fillStream( std::ostream& s ) const {
      s << "{VPCluster's key: " << m_vpID << "}";
      return s;
    }

    /// Retrieve const  inter-pixel fraction
    [[nodiscard]] unsigned char xfraction() const noexcept { return m_fx; }
    [[nodiscard]] unsigned char yfraction() const noexcept { return m_fy; }

    /// Retrieve const  global x coordinate
    [[nodiscard]] float x() const noexcept { return m_x; }

    /// Retrieve const  global y coordinate
    [[nodiscard]] float y() const noexcept { return m_y; }

    /// Retrieve const  global z coordinate
    [[nodiscard]] float z() const noexcept { return m_z; }

    /// classID method, needed for MC linking. Returns the CLID of VPCluster for backward compatibility
    static inline CLID classID() { return 4504; } // CLID_VPCluster

  private:
    unsigned char m_fx;   ///< inter-pixel fraction in x coordinate
    unsigned char m_fy;   ///< inter-pixel fraction in y coordinate
    float         m_x;    ///< global x coordinate
    float         m_y;    ///< global y coordinate
    float         m_z;    ///< global z coordinate
    VPChannelID   m_vpID; ///< channelID of cluster
    /* std::vector<VPChannelID> m_pixels; */

  }; // class VPLightCluster

  /// lightweight container for VPLightCluster
  using VPLightClusters = std::vector<VPLightCluster>;

  inline std::ostream& operator<<( std::ostream& str, const VPLightCluster& obj ) { return obj.fillStream( str ); }

} // namespace LHCb

#endif /// DigiEvent_VPCluster_H
