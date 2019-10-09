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
#include "Kernel/FastClusterContainer.h"
#include "Kernel/SiDataFunctor.h"
#include "Kernel/VeloChannelID.h"
#include "LHCbMath/LHCbMath.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  // Namespace for locations in TDS
  namespace VeloLiteClusterLocation {
    inline const std::string Default = "Raw/Velo/LiteClusters";
  }

  /** @class VeloLiteCluster VeloLiteCluster.h
   *
   * A cluster as produced by the TELL1 but without full ADC information
   *
   * @author C Parkes
   *
   */

  class VeloLiteCluster final {
  public:
    /// channelID type
    typedef LHCb::VeloChannelID chan_type;
    /// fast container for light clusters
    typedef FastClusterContainer<LHCb::VeloLiteCluster, int> VeloLiteClusters;
    /// fast container for light clusters (old name: will be removed)
    typedef FastClusterContainer<LHCb::VeloLiteCluster, int> FastContainer;
    /// finding policy
    typedef SiDataFunctor::CompareByChannel<LHCb::VeloLiteCluster> findPolicy;

    /// Constructor
    VeloLiteCluster( const VeloChannelID& chan, double fracStrip, unsigned int size, bool secondThres,
                     bool moreBitsInChannelID = false );

    /// Constructor
    VeloLiteCluster( unsigned int fracStrip, unsigned int pseudoSize, bool secondThres, const VeloChannelID& chan,
                     bool moreBitsInChannelID = false );

    /// Default Constructor
    VeloLiteCluster() : m_liteCluster( 0 ) {}

    /// position within a strip of cluster centre
    double interStripFraction() const;

    /// number of strips in cluster, 3 means 3 or more
    unsigned int pseudoSize() const;

    /// The Channel
    VeloChannelID channelID() const;

    /// High Threshold
    bool highThreshold() const;

    /// Retrieve whether it is with 25-bits for channelID or not
    bool moreBitsInChannelID() const;

    /// check if R type
    bool isRType() const;

    /// check if Phi type
    bool isPhiType() const;

    /// check if PileUp
    bool isPileUp() const;

  protected:
  private:
    /// Offsets of bitfield liteCluster with 25 bits for channelID
    enum liteCluster1Bits {
      channelID1Bits          = 0,
      interStripFraction1Bits = 25,
      size1Bits               = 27,
      highThreshold1Bits      = 28,
      moreBitsInChannelIDBits = 31
    };
    /// Bitmasks for bitfield liteCluster with 25 bits for channelID
    enum liteCluster1Masks {
      channelID1Mask          = 0x1ffffffL,
      interStripFraction1Mask = 0x6000000L,
      size1Mask               = 0x8000000L,
      highThreshold1Mask      = 0x10000000L,
      moreBitsInChannelIDMask = 0x80000000L
    };

    /// number between 0 and 7 /8ths of a strip
    int fractionUnits() const;

    /// Offsets of bitfield liteCluster
    enum liteClusterBits { channelID0Bits = 0, interStripFraction0Bits = 24, size0Bits = 27, highThreshold0Bits = 28 };

    /// Bitmasks for bitfield liteCluster
    enum liteClusterMasks {
      channelID0Mask          = 0xffffffL,
      interStripFraction0Mask = 0x7000000L,
      size0Mask               = 0x8000000L,
      highThreshold0Mask      = 0x10000000L
    };

    unsigned int m_liteCluster; ///< lite Cluster

  }; // class VeloLiteCluster

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::VeloLiteCluster::VeloLiteCluster( const VeloChannelID& chan, double fracStrip, unsigned int size,
                                               bool secondThres, bool moreBitsInChannelID ) {

  unsigned int tFrac;
  if ( moreBitsInChannelID )
    tFrac = LHCb::Math::round( fracStrip * ( 1 << ( size1Bits - interStripFraction1Bits ) ) );
  else
    tFrac = LHCb::Math::round( fracStrip * ( 1 << ( size0Bits - interStripFraction0Bits ) ) );

  unsigned int pseudoSize;
  size < 3 ? pseudoSize = 0 : pseudoSize = 1;
  unsigned int tThres;
  secondThres == true ? tThres = 1 : tThres = 0;

  if ( moreBitsInChannelID )
    m_liteCluster = ( chan << channelID1Bits ) + ( tFrac << interStripFraction1Bits ) + ( pseudoSize << size1Bits ) +
                    ( tThres << highThreshold1Bits ) + moreBitsInChannelIDMask;
  else
    m_liteCluster = ( chan << channelID0Bits ) + ( tFrac << interStripFraction0Bits ) + ( pseudoSize << size0Bits ) +
                    ( tThres << highThreshold0Bits );
}

inline LHCb::VeloLiteCluster::VeloLiteCluster( unsigned int fracStrip, unsigned int pseudoSize, bool secondThres,
                                               const VeloChannelID& chan, bool moreBitsInChannelID ) {

  unsigned int tThres;
  secondThres == true ? tThres = 1 : tThres = 0;
  if ( moreBitsInChannelID )
    m_liteCluster = ( chan << channelID1Bits ) + ( fracStrip << interStripFraction1Bits ) +
                    ( pseudoSize << size1Bits ) + ( tThres << highThreshold1Bits ) + moreBitsInChannelIDMask;
  else
    m_liteCluster = ( chan << channelID0Bits ) + ( fracStrip << interStripFraction0Bits ) +
                    ( pseudoSize << size0Bits ) + ( tThres << highThreshold0Bits );
}

inline double LHCb::VeloLiteCluster::interStripFraction() const {

  return ( moreBitsInChannelID() ? ( fractionUnits() / double( 1 << ( size1Bits - interStripFraction1Bits ) ) )
                                 : ( fractionUnits() / double( 1 << ( size0Bits - interStripFraction0Bits ) ) ) );
}

inline unsigned int LHCb::VeloLiteCluster::pseudoSize() const {

  unsigned int cSize = ( moreBitsInChannelID() ? 1 + ( ( m_liteCluster & size1Mask ) >> size1Bits )
                                               : 1 + ( ( m_liteCluster & size0Mask ) >> size0Bits ) );
  if ( cSize == 1 ) {
    if ( fractionUnits() != 0 ) cSize = 2;
  } else {
    cSize = 3;
  }
  return cSize;
}

inline LHCb::VeloChannelID LHCb::VeloLiteCluster::channelID() const {

  return ( moreBitsInChannelID() ? VeloChannelID( ( m_liteCluster & channelID1Mask ) >> channelID1Bits )
                                 : VeloChannelID( ( m_liteCluster & channelID0Mask ) >> channelID0Bits ) );
}

inline bool LHCb::VeloLiteCluster::highThreshold() const {

  return ( moreBitsInChannelID() ? ( 0 != ( ( m_liteCluster & highThreshold1Mask ) >> highThreshold1Bits ) )
                                 : ( 0 != ( ( m_liteCluster & highThreshold0Mask ) >> highThreshold0Bits ) ) );
}

inline bool LHCb::VeloLiteCluster::moreBitsInChannelID() const {
  return 0 != ( ( m_liteCluster & moreBitsInChannelIDMask ) >> moreBitsInChannelIDBits );
}

inline bool LHCb::VeloLiteCluster::isRType() const { return channelID().isRType(); }

inline bool LHCb::VeloLiteCluster::isPhiType() const { return channelID().isPhiType(); }

inline bool LHCb::VeloLiteCluster::isPileUp() const { return channelID().isPileUp(); }

inline int LHCb::VeloLiteCluster::fractionUnits() const {

  return ( moreBitsInChannelID() ? ( ( m_liteCluster & interStripFraction1Mask ) >> interStripFraction1Bits )
                                 : ( ( m_liteCluster & interStripFraction0Mask ) >> interStripFraction0Bits ) );
}
