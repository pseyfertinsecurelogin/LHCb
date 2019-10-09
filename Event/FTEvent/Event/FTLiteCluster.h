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
#include "Kernel/FTChannelID.h"
#include "Kernel/MultiIndexedContainer.h"
#include "Kernel/SiDataFunctor.h"

// Forward declarations

namespace LHCb {

  // Forward declarations
  // Namespace for locations in TDS
  namespace FTLiteClusterLocation {
    inline const std::string Default = "Raw/FT/LiteClusters";
  }

  /** @class FTLiteCluster FTLiteCluster.h
   *
   * Lite cluster, all information on 32 bits for fast container
   *
   * @author Olivier Callot
   *
   */

  class FTLiteCluster final {
  public:
    /// channelID type
    typedef LHCb::FTChannelID chan_type;
    /// finding policy
    typedef SiDataFunctor::CompareByChannel<LHCb::FTLiteCluster> findPolicy;
    /// fast container of Lite Cluster
    typedef LHCb::Container::MultiIndexedContainer<LHCb::FTLiteCluster, 48> FTLiteClusters;

    /// Constructor
    FTLiteCluster( const LHCb::FTChannelID& chan, int fraction, int pseudoSize );

    /// Default Constructor
    FTLiteCluster() : m_liteCluster( 0 ) {}

    /// Returns the FT channel ID
    FTChannelID channelID() const;

    /// returns the fraction of channel
    float fraction() const;

    /// returns the fraction of channel as the integer value (0 or 1)
    int fractionBit() const;

    /// Retrieve number of cells in the cluster
    unsigned int pseudoSize() const;

  protected:
  private:
    /// Offsets of bitfield liteCluster
    enum liteClusterBits { channelIDBits = 0, fractionBits = 20, pseudoSizeBits = 21 };

    /// Bitmasks for bitfield liteCluster
    enum liteClusterMasks { channelIDMask = 0xfffffL, fractionMask = 0x100000L, pseudoSizeMask = 0x1e00000L };

    unsigned int m_liteCluster; ///< Lite Cluster

  }; // class FTLiteCluster

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::FTLiteCluster::FTLiteCluster( const LHCb::FTChannelID& chan, int fraction, int pseudoSize ) {

  m_liteCluster = ( chan << channelIDBits ) + ( fraction << fractionBits ) + ( pseudoSize << pseudoSizeBits );
}

inline unsigned int LHCb::FTLiteCluster::pseudoSize() const {
  return (unsigned int)( ( m_liteCluster & pseudoSizeMask ) >> pseudoSizeBits );
}

inline LHCb::FTChannelID LHCb::FTLiteCluster::channelID() const {
  return LHCb::FTChannelID( ( m_liteCluster & channelIDMask ) >> channelIDBits );
}

inline float LHCb::FTLiteCluster::fraction() const {
  return 0.5 * ( ( m_liteCluster & fractionMask ) >> fractionBits );
}

inline int LHCb::FTLiteCluster::fractionBit() const { return ( m_liteCluster & fractionMask ) >> fractionBits; }
