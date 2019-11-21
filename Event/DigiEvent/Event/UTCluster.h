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
#include "Event/UTLiteCluster.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/UTChannelID.h"
#include "Kernel/UTNames.h"
#include <algorithm>
#include <numeric>
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTCluster = 9004;

  // Namespace for locations in TDS
  namespace UTClusterLocation {
    inline const std::string UTClusters = "Raw/UT/Clusters";
  }

  /** @class UTCluster UTCluster.h
   *
   * A cluster as produced by the TELL1 with full ADC information.
   *
   * @author Andy Beiter (based on code by Matthew Needham)
   *
   */

  class UTCluster final : public KeyedObject<LHCb::UTChannelID> {
  public:
    /// typedef for KeyedContainer of UTCluster
    typedef KeyedContainer<UTCluster, Containers::HashMap> Container;

    /// ADC vector pair typedef
    typedef std::vector<std::pair<int, unsigned int>> ADCVector;

    /// spill names
    enum Spill {
      UnknownSpill = 99,
      Central      = 0,
      Prev1        = -1,
      Prev2        = -2,
      Prev3        = -3,
      Prev4        = -4,
      Prev5        = -5,
      Prev6        = -6,
      Prev7        = -7,
      Prev8        = -8,
      Prev9        = -9,
      Prev10       = -10,
      Next1        = 1,
      Next2        = 2,
      Next3        = 3,
      Next4        = 4,
      Next5        = 5,
      Next6        = 6,
      Next7        = 7,
      Next8        = 8,
      Next9        = 9,
      Next10       = 10
    };

    /// Constructor with source id
    UTCluster( const UTLiteCluster& lCluster, ADCVector strips, double neighbourSum, unsigned sourceID,
               unsigned int tell1Chan, const LHCb::UTCluster::Spill& spill )
        : m_liteCluster( lCluster )
        , m_stripValues( std::move( strips ) )
        , m_neighbourSum( neighbourSum )
        , m_sourceID( sourceID )
        , m_tell1Channel( tell1Chan )
        , m_spill( spill ) {}

    /// copy constructor
    UTCluster( const LHCb::UTCluster& cluster )
        : KeyedObject<LHCb::UTChannelID>()
        , m_liteCluster( cluster.m_liteCluster )
        , m_stripValues( cluster.m_stripValues )
        , m_neighbourSum( cluster.m_neighbourSum )
        , m_sourceID( cluster.m_sourceID )
        , m_tell1Channel( cluster.m_tell1Channel )
        , m_spill( cluster.m_spill ) {}

    /// Default Constructor
    UTCluster() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// conversion of string to enum for type Spill
    static LHCb::UTCluster::Spill SpillToType( const std::string& aName );

    /// conversion to string for enum type Spill
    static const std::string& SpillToString( int aEnum );

    /// clone method
    [[nodiscard]] UTCluster* clone() const;

    /// number of strips in cluster
    [[nodiscard]] unsigned int size() const;

    /// adc value of strips in cluster
    [[nodiscard]] unsigned int adcValue( unsigned int num ) const;

    /// strip number of strip in cluster
    [[nodiscard]] int strip( unsigned int num ) const;

    /// The  Channel ID
    [[nodiscard]] UTChannelID channelID() const;

    /// total charge
    [[nodiscard]] double totalCharge() const;

    /// position within a strip of cluster centre, quantized in xxths of a strip
    [[nodiscard]] double interStripFraction() const;

    /// number of strips in cluster, 3 means 3 or more
    [[nodiscard]] unsigned int pseudoSize() const;

    /// denotes a cluster with significant adc counts, less likely to be noise or spillover
    [[nodiscard]] bool highThreshold() const;

    /// first UT Channel ID
    [[nodiscard]] UTChannelID firstChannel() const;

    /// first UT Channel ID
    [[nodiscard]] UTChannelID lastChannel() const;

    /// channel IDs
    [[nodiscard]] std::vector<LHCb::UTChannelID> channels() const;

    /// check if channel is inside cluster
    [[nodiscard]] bool contains( const LHCb::UTChannelID& chan ) const;

    /// true if clusters have strips in common
    bool overlaps( const UTCluster* testCluster ) const;

    /// maximum adc value of strips in a cluster
    [[nodiscard]] unsigned int maxADCValue() const;

    /// Print the cluster in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// check if UT type
    [[nodiscard]] bool isUT() const;

    /// short cut for station
    [[nodiscard]] unsigned int station() const;

    /// shortcut for layer
    [[nodiscard]] unsigned int layer() const;

    /// short cut for detRegion
    [[nodiscard]] unsigned int detRegion() const;

    /// short cut for sector
    [[nodiscard]] unsigned int sector() const;

    /// short cut for strip
    [[nodiscard]] unsigned int strip() const;

    /// Print the unique sector name
    [[nodiscard]] std::string sectorName() const;

    /// Print the unique layer name
    [[nodiscard]] std::string layerName() const;

    /// Print the unique det region name
    [[nodiscard]] std::string detRegionName() const;

    /// Print the station name
    [[nodiscard]] std::string stationName() const;

    /// Retrieve const  TELL1 cluster without ADC values
    [[nodiscard]] const LHCb::UTLiteCluster& liteCluster() const;

    /// Retrieve const  strip numbers and their signals
    [[nodiscard]] const ADCVector& stripValues() const;

    /// Update  strip numbers and their signals
    void setStripValues( const ADCVector& value );

    /// Retrieve const  sum neighbour channels
    [[nodiscard]] double neighbourSum() const;

    /// Retrieve const  Identifier of the tell1 giving the cluster
    [[nodiscard]] unsigned int sourceID() const;

    /// Retrieve const  tell1 channel number
    [[nodiscard]] unsigned int tell1Channel() const;

    /// Retrieve const  spill the cluster corresponds to
    [[nodiscard]] const LHCb::UTCluster::Spill& spill() const;

    friend std::ostream& operator<<( std::ostream& str, const UTCluster& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::UTLiteCluster    m_liteCluster;       ///< TELL1 cluster without ADC values
    ADCVector              m_stripValues;       ///< strip numbers and their signals
    double                 m_neighbourSum{0.0}; ///< sum neighbour channels
    unsigned int           m_sourceID{0};       ///< Identifier of the tell1 giving the cluster
    unsigned int           m_tell1Channel{0};   ///< tell1 channel number
    LHCb::UTCluster::Spill m_spill{};           ///< spill the cluster corresponds to

    static const GaudiUtils::VectorMap<std::string, Spill>& s_SpillTypMap();

  }; // class UTCluster

  /// Definition of Keyed Container for UTCluster
  typedef KeyedContainer<UTCluster, Containers::HashMap> UTClusters;

  inline std::ostream& operator<<( std::ostream& s, LHCb::UTCluster::Spill e ) {
    switch ( e ) {
    case LHCb::UTCluster::UnknownSpill:
      return s << "UnknownSpill";
    case LHCb::UTCluster::Central:
      return s << "Central";
    case LHCb::UTCluster::Prev1:
      return s << "Prev1";
    case LHCb::UTCluster::Prev2:
      return s << "Prev2";
    case LHCb::UTCluster::Prev3:
      return s << "Prev3";
    case LHCb::UTCluster::Prev4:
      return s << "Prev4";
    case LHCb::UTCluster::Prev5:
      return s << "Prev5";
    case LHCb::UTCluster::Prev6:
      return s << "Prev6";
    case LHCb::UTCluster::Prev7:
      return s << "Prev7";
    case LHCb::UTCluster::Prev8:
      return s << "Prev8";
    case LHCb::UTCluster::Prev9:
      return s << "Prev9";
    case LHCb::UTCluster::Prev10:
      return s << "Prev10";
    case LHCb::UTCluster::Next1:
      return s << "Next1";
    case LHCb::UTCluster::Next2:
      return s << "Next2";
    case LHCb::UTCluster::Next3:
      return s << "Next3";
    case LHCb::UTCluster::Next4:
      return s << "Next4";
    case LHCb::UTCluster::Next5:
      return s << "Next5";
    case LHCb::UTCluster::Next6:
      return s << "Next6";
    case LHCb::UTCluster::Next7:
      return s << "Next7";
    case LHCb::UTCluster::Next8:
      return s << "Next8";
    case LHCb::UTCluster::Next9:
      return s << "Next9";
    case LHCb::UTCluster::Next10:
      return s << "Next10";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::UTCluster::Spill";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::UTCluster::clID() const { return LHCb::UTCluster::classID(); }

inline const CLID& LHCb::UTCluster::classID() { return CLID_UTCluster; }

inline const GaudiUtils::VectorMap<std::string, LHCb::UTCluster::Spill>& LHCb::UTCluster::s_SpillTypMap() {
  static const GaudiUtils::VectorMap<std::string, Spill> m = {{"UnknownSpill", UnknownSpill},
                                                              {"Central", Central},
                                                              {"Prev1", Prev1},
                                                              {"Prev2", Prev2},
                                                              {"Prev3", Prev3},
                                                              {"Prev4", Prev4},
                                                              {"Prev5", Prev5},
                                                              {"Prev6", Prev6},
                                                              {"Prev7", Prev7},
                                                              {"Prev8", Prev8},
                                                              {"Prev9", Prev9},
                                                              {"Prev10", Prev10},
                                                              {"Next1", Next1},
                                                              {"Next2", Next2},
                                                              {"Next3", Next3},
                                                              {"Next4", Next4},
                                                              {"Next5", Next5},
                                                              {"Next6", Next6},
                                                              {"Next7", Next7},
                                                              {"Next8", Next8},
                                                              {"Next9", Next9},
                                                              {"Next10", Next10}};
  return m;
}

inline LHCb::UTCluster::Spill LHCb::UTCluster::SpillToType( const std::string& aName ) {
  auto iter = s_SpillTypMap().find( aName );
  return iter != s_SpillTypMap().end() ? iter->second : UnknownSpill;
}

inline const std::string& LHCb::UTCluster::SpillToString( int aEnum ) {
  static const std::string s_UnknownSpill = "UnknownSpill";
  auto                     iter           = std::find_if( s_SpillTypMap().begin(), s_SpillTypMap().end(),
                            [&]( const std::pair<const std::string, Spill>& i ) { return i.second == aEnum; } );
  return iter != s_SpillTypMap().end() ? iter->first : s_UnknownSpill;
}

inline const LHCb::UTLiteCluster& LHCb::UTCluster::liteCluster() const { return m_liteCluster; }

inline const LHCb::UTCluster::ADCVector& LHCb::UTCluster::stripValues() const { return m_stripValues; }

inline void LHCb::UTCluster::setStripValues( const ADCVector& value ) { m_stripValues = value; }

inline double LHCb::UTCluster::neighbourSum() const { return m_neighbourSum; }

inline unsigned int LHCb::UTCluster::sourceID() const { return m_sourceID; }

inline unsigned int LHCb::UTCluster::tell1Channel() const { return m_tell1Channel; }

inline const LHCb::UTCluster::Spill& LHCb::UTCluster::spill() const { return m_spill; }

inline LHCb::UTCluster* LHCb::UTCluster::clone() const { return new LHCb::UTCluster( *this ); }

inline unsigned int LHCb::UTCluster::size() const { return stripValues().size(); }

inline unsigned int LHCb::UTCluster::adcValue( unsigned int num ) const { return stripValues()[num].second; }

inline int LHCb::UTCluster::strip( unsigned int num ) const { return stripValues()[num].first; }

inline LHCb::UTChannelID LHCb::UTCluster::channelID() const { return m_liteCluster.channelID(); }

inline double LHCb::UTCluster::totalCharge() const {

  return std::accumulate( m_stripValues.begin(), m_stripValues.end(), 0.,
                          []( double sum, const auto& sv ) { return sum + sv.second; } );
}

inline double LHCb::UTCluster::interStripFraction() const { return m_liteCluster.interStripFraction(); }

inline unsigned int LHCb::UTCluster::pseudoSize() const { return m_liteCluster.pseudoSize(); }

inline bool LHCb::UTCluster::highThreshold() const { return m_liteCluster.highThreshold(); }

inline LHCb::UTChannelID LHCb::UTCluster::firstChannel() const {

  return UTChannelID( channelID().type(), channelID().station(), channelID().layer(), channelID().detRegion(),
                      channelID().sector(), channelID().strip() + stripValues().front().first );
}

inline LHCb::UTChannelID LHCb::UTCluster::lastChannel() const {

  return UTChannelID( channelID().type(), channelID().station(), channelID().layer(), channelID().detRegion(),
                      channelID().sector(), channelID().strip() + stripValues().back().first );
}

inline std::vector<LHCb::UTChannelID> LHCb::UTCluster::channels() const {

  std::vector<LHCb::UTChannelID> chanCont;
  UTChannelID                    first = firstChannel();
  chanCont.reserve( size() );
  for ( unsigned int i = 0; i < size(); ++i ) {
    chanCont.emplace_back( first.type(), first.station(), first.layer(), first.detRegion(), first.sector(),
                           first.strip() + i );
  }
  return chanCont;
}

inline bool LHCb::UTCluster::contains( const LHCb::UTChannelID& chan ) const {

  bool inside = false;
  if ( chan.uniqueSector() == channelID().uniqueSector() ) {
    const unsigned int firstStrip = channelID().strip() + stripValues().front().first;
    const unsigned int lastStrip  = channelID().strip() + stripValues().back().first;
    if ( chan.strip() >= firstStrip && chan.strip() <= lastStrip ) inside = true;
  }
  return inside;
}

inline bool LHCb::UTCluster::overlaps( const UTCluster* testCluster ) const {

  bool overlap = false;
  if ( channelID().uniqueSector() == testCluster->channelID().uniqueSector() ) {
    const auto& testchans = testCluster->channels();
    overlap = std::any_of( testchans.begin(), testchans.end(), [&]( const auto& tc ) { return contains( tc ); } );
  } // if
  return overlap;
}

inline unsigned int LHCb::UTCluster::maxADCValue() const {

  const auto& svs = stripValues();
  return std::accumulate( svs.begin(), svs.end(), 0u,
                          []( unsigned int maxCharge, const auto& sv ) { return std::max( maxCharge, sv.second ); } );
}

inline std::ostream& LHCb::UTCluster::fillStream( std::ostream& s ) const {

  s << "{ UTCluster with key: " << (int)key() << " channel: " << sectorName() << " strip " << strip() << "Size "
    << size() << " charge :" << totalCharge() << " source ID " << sourceID() << " tell1 channel " << tell1Channel()
    << " Spill " << spill() << std::endl
    << " }";
  return s;
}

inline bool LHCb::UTCluster::isUT() const { return channelID().isUT(); }

inline unsigned int LHCb::UTCluster::station() const { return channelID().station(); }

inline unsigned int LHCb::UTCluster::layer() const { return channelID().layer(); }

inline unsigned int LHCb::UTCluster::detRegion() const { return channelID().detRegion(); }

inline unsigned int LHCb::UTCluster::sector() const { return channelID().sector(); }

inline unsigned int LHCb::UTCluster::strip() const { return channelID().strip(); }

inline std::string LHCb::UTCluster::sectorName() const { return ( UTNames().UniqueSectorToString( channelID() ) ); }

inline std::string LHCb::UTCluster::layerName() const { return ( UTNames().UniqueLayerToString( channelID() ) ); }

inline std::string LHCb::UTCluster::detRegionName() const { return ( UTNames().UniqueRegionToString( channelID() ) ); }

inline std::string LHCb::UTCluster::stationName() const { return ( UTNames().StationToString( channelID() ) ); }
