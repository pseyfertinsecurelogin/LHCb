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
#include "Kernel/ITNames.h"
#include "Kernel/STChannelID.h"
#include "Kernel/SiDataFunctor.h"
#include "Kernel/TTNames.h"
#include "LHCbMath/LHCbMath.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations
  // Namespace for locations in TDS
  namespace STLiteClusterLocation {
    inline const std::string TTClusters = "Raw/TT/LiteClusters";
    inline const std::string ITClusters = "Raw/IT/LiteClusters";
  } // namespace STLiteClusterLocation

  /** @class STLiteCluster STLiteCluster.h
   *
   * A cluster as produced by the TELL1 but without full ADC information
   *
   * @author M Needham
   *
   */

  class STLiteCluster final {
  public:
    /// channelID type
    typedef LHCb::STChannelID chan_type;
    /// fast container for light clusters
    typedef FastClusterContainer<LHCb::STLiteCluster, int> STLiteClusters;
    /// fast container for light clusters (old name: will be removed)
    typedef FastClusterContainer<LHCb::STLiteCluster, int> FastContainer;
    /// finding policy
    typedef SiDataFunctor::CompareByChannel<LHCb::STLiteCluster> findPolicy;

    /// Constructor
    STLiteCluster( const STChannelID& chan, double fracStrip, unsigned int size, bool secondThres,
                   bool moreBitsInChannelID = false );

    /// Constructor
    STLiteCluster( unsigned int fracStrip, unsigned int pseudoSize, bool secondThres, const STChannelID& chan,
                   bool moreBitsInChannelID = false );

    /// Default Constructor
    STLiteCluster() : m_liteCluster( 0 ) {}

    /// position within a strip of cluster centre
    double interStripFraction() const;

    /// number of strips in cluster, 3 means 3 or more
    unsigned int pseudoSize() const;

    /// The Channel
    STChannelID channelID() const;

    /// High Threshold
    bool highThreshold() const;

    /// Retrieve whether it is with 25-bits for channelID or not
    bool moreBitsInChannelID() const;

    /// check if TT type
    bool isTT() const;

    /// check if IT type
    bool isIT() const;

    /// short cut for station
    unsigned int station() const;

    /// shortcut for layer
    unsigned int layer() const;

    /// short cut for detRegion
    unsigned int detRegion() const;

    /// short cut for sector
    unsigned int sector() const;

    /// short cut for strip
    unsigned int strip() const;

    /// Print the unique sector name
    std::string sectorName() const;

    /// Print the unique layer name
    std::string layerName() const;

    /// Print the unique det region name
    std::string detRegionName() const;

    /// Print the station name
    std::string stationName() const;

    /// Print the lite cluster in a human readable way
    std::ostream& fillStream( std::ostream& s ) const;

    friend std::ostream& operator<<( std::ostream& str, const STLiteCluster& obj ) { return obj.fillStream( str ); }

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
    enum liteClusterBits { channelID0Bits = 0, interStripFraction0Bits = 24, size0Bits = 26, highThreshold0Bits = 27 };

    /// Bitmasks for bitfield liteCluster
    enum liteClusterMasks {
      channelID0Mask          = 0xffffffL,
      interStripFraction0Mask = 0x3000000L,
      size0Mask               = 0x4000000L,
      highThreshold0Mask      = 0x8000000L
    };

    unsigned int m_liteCluster; ///< lite Cluster

  }; // class STLiteCluster

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::STLiteCluster::STLiteCluster( const STChannelID& chan, double fracStrip, unsigned int size,
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

inline LHCb::STLiteCluster::STLiteCluster( unsigned int fracStrip, unsigned int pseudoSize, bool secondThres,
                                           const STChannelID& chan, bool moreBitsInChannelID ) {

  unsigned int tThres;
  secondThres == true ? tThres = 1 : tThres = 0;
  if ( moreBitsInChannelID )
    m_liteCluster = ( chan << channelID1Bits ) + ( fracStrip << interStripFraction1Bits ) +
                    ( pseudoSize << size1Bits ) + ( tThres << highThreshold1Bits ) + moreBitsInChannelIDMask;
  else
    m_liteCluster = ( chan << channelID0Bits ) + ( fracStrip << interStripFraction0Bits ) +
                    ( pseudoSize << size0Bits ) + ( tThres << highThreshold0Bits );
}

inline double LHCb::STLiteCluster::interStripFraction() const {

  return ( moreBitsInChannelID() ? ( fractionUnits() / double( 1 << ( size1Bits - interStripFraction1Bits ) ) )
                                 : ( fractionUnits() / double( 1 << ( size0Bits - interStripFraction0Bits ) ) ) );
}

inline unsigned int LHCb::STLiteCluster::pseudoSize() const {

  unsigned int cSize = ( moreBitsInChannelID() ? 1 + ( ( m_liteCluster & size1Mask ) >> size1Bits )
                                               : 1 + ( ( m_liteCluster & size0Mask ) >> size0Bits ) );
  if ( cSize == 1 ) {
    if ( fractionUnits() != 0 ) cSize = 2;
  } else {
    cSize = 3;
  }
  return cSize;
}

inline LHCb::STChannelID LHCb::STLiteCluster::channelID() const {

  return ( moreBitsInChannelID() ? STChannelID( ( m_liteCluster & channelID1Mask ) >> channelID1Bits )
                                 : STChannelID( ( m_liteCluster & channelID0Mask ) >> channelID0Bits ) );
}

inline bool LHCb::STLiteCluster::highThreshold() const {

  return ( moreBitsInChannelID() ? ( 0 != ( ( m_liteCluster & highThreshold1Mask ) >> highThreshold1Bits ) )
                                 : ( 0 != ( ( m_liteCluster & highThreshold0Mask ) >> highThreshold0Bits ) ) );
}

inline bool LHCb::STLiteCluster::moreBitsInChannelID() const {
  return 0 != ( ( m_liteCluster & moreBitsInChannelIDMask ) >> moreBitsInChannelIDBits );
}

inline bool LHCb::STLiteCluster::isTT() const { return channelID().isTT(); }

inline bool LHCb::STLiteCluster::isIT() const { return channelID().isIT(); }

inline unsigned int LHCb::STLiteCluster::station() const { return channelID().station(); }

inline unsigned int LHCb::STLiteCluster::layer() const { return channelID().layer(); }

inline unsigned int LHCb::STLiteCluster::detRegion() const { return channelID().detRegion(); }

inline unsigned int LHCb::STLiteCluster::sector() const { return channelID().sector(); }

inline unsigned int LHCb::STLiteCluster::strip() const { return channelID().strip(); }

inline std::string LHCb::STLiteCluster::sectorName() const {

  return ( isTT() ? TTNames().UniqueSectorToString( channelID() ) : ITNames().UniqueSectorToString( channelID() ) );
}

inline std::string LHCb::STLiteCluster::layerName() const {

  return ( isTT() ? TTNames().UniqueLayerToString( channelID() ) : ITNames().UniqueLayerToString( channelID() ) );
}

inline std::string LHCb::STLiteCluster::detRegionName() const {

  return ( isTT() ? TTNames().UniqueRegionToString( channelID() ) : ITNames().UniqueBoxToString( channelID() ) );
}

inline std::string LHCb::STLiteCluster::stationName() const {

  return ( isTT() ? TTNames().StationToString( channelID() ) : ITNames().StationToString( channelID() ) );
}

inline std::ostream& LHCb::STLiteCluster::fillStream( std::ostream& s ) const {

  s << "{ STLiteCluster: " << (int)channelID() << " channel: " << sectorName() << " strip " << strip()
    << " Pseudo size " << pseudoSize() << " high threshold :" << highThreshold() << " interStripFraction "
    << interStripFraction() << " moreBitsInChannelID " << moreBitsInChannelID() << std::endl
    << " }";
  return s;
}

inline int LHCb::STLiteCluster::fractionUnits() const {

  return ( moreBitsInChannelID() ? ( ( m_liteCluster & interStripFraction1Mask ) >> interStripFraction1Bits )
                                 : ( ( m_liteCluster & interStripFraction0Mask ) >> interStripFraction0Bits ) );
}
