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
#include "Event/STLiteCluster.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/VectorMap.h"
#include "Kernel/ITNames.h"
#include "Kernel/STChannelID.h"
#include "Kernel/TTNames.h"
#include <algorithm>
#include <ostream>
#include <utility>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_STCluster = 9004;

  // Namespace for locations in TDS
  namespace STClusterLocation {
    inline const std::string TTClusters = "Raw/TT/Clusters";
    inline const std::string ITClusters = "Raw/IT/Clusters";
  } // namespace STClusterLocation

  /** @class STCluster STCluster.h
   *
   * A cluster as produced by the TELL1 with full ADC information.
   *
   * @author Matthew Needham
   *
   */

  class STCluster final : public KeyedObject<LHCb::STChannelID> {
  public:
    /// typedef for KeyedContainer of STCluster
    typedef KeyedContainer<STCluster, Containers::HashMap> Container;

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
    STCluster( const STLiteCluster& lCluster, const ADCVector& strips, double neighbourSum, unsigned sourceID,
               unsigned int tell1Chan, const LHCb::STCluster::Spill& spill )
        : m_liteCluster( lCluster )
        , m_stripValues( strips )
        , m_neighbourSum( neighbourSum )
        , m_sourceID( sourceID )
        , m_tell1Channel( tell1Chan )
        , m_spill( spill ) {}

    /// copy constructor
    STCluster( const LHCb::STCluster& cluster )
        : KeyedObject<LHCb::STChannelID>()
        , m_liteCluster( cluster.m_liteCluster )
        , m_stripValues( cluster.m_stripValues )
        , m_neighbourSum( cluster.m_neighbourSum )
        , m_sourceID( cluster.m_sourceID )
        , m_tell1Channel( cluster.m_tell1Channel )
        , m_spill( cluster.m_spill ) {}

    /// Default Constructor
    STCluster()
        : m_liteCluster(), m_stripValues(), m_neighbourSum( 0.0 ), m_sourceID( 0 ), m_tell1Channel( 0 ), m_spill() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type Spill
    static LHCb::STCluster::Spill SpillToType( const std::string& aName );

    /// conversion to string for enum type Spill
    static const std::string& SpillToString( int aEnum );

    /// clone method
    STCluster* clone() const;

    /// number of strips in cluster
    unsigned int size() const;

    /// adc value of strips in cluster
    unsigned int adcValue( unsigned int num ) const;

    /// strip number of strip in cluster
    int strip( unsigned int num ) const;

    /// The  Channel ID
    STChannelID channelID() const;

    /// total charge
    double totalCharge() const;

    /// position within a strip of cluster centre, quantized in xxths of a strip
    double interStripFraction() const;

    /// number of strips in cluster, 3 means 3 or more
    unsigned int pseudoSize() const;

    /// denotes a cluster with significant adc counts, less likely to be noise or spillover
    bool highThreshold() const;

    /// first ST Channel ID
    STChannelID firstChannel() const;

    /// first ST Channel ID
    STChannelID lastChannel() const;

    /// channel IDs
    std::vector<LHCb::STChannelID> channels() const;

    /// check if channel is inside cluster
    bool contains( const LHCb::STChannelID& chan ) const;

    /// true if clusters have strips in common
    bool overlaps( const STCluster* testCluster ) const;

    /// maximum adc value of strips in a cluster
    unsigned int maxADCValue() const;

    /// Print the cluster in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

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

    /// Retrieve const  TELL1 cluster without ADC values
    const LHCb::STLiteCluster& liteCluster() const;

    /// Retrieve const  strip numbers and their signals
    const ADCVector& stripValues() const;

    /// Update  strip numbers and their signals
    void setStripValues( const ADCVector& value );

    /// Retrieve const  sum neighbour channels
    double neighbourSum() const;

    /// Retrieve const  Identifier of the tell1 giving the cluster
    unsigned int sourceID() const;

    /// Retrieve const  tell1 channel number
    unsigned int tell1Channel() const;

    /// Retrieve const  spill the cluster corresponds to
    const LHCb::STCluster::Spill& spill() const;

    friend std::ostream& operator<<( std::ostream& str, const STCluster& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    LHCb::STLiteCluster    m_liteCluster;  ///< TELL1 cluster without ADC values
    ADCVector              m_stripValues;  ///< strip numbers and their signals
    double                 m_neighbourSum; ///< sum neighbour channels
    unsigned int           m_sourceID;     ///< Identifier of the tell1 giving the cluster
    unsigned int           m_tell1Channel; ///< tell1 channel number
    LHCb::STCluster::Spill m_spill;        ///< spill the cluster corresponds to

    static const GaudiUtils::VectorMap<std::string, Spill>& s_SpillTypMap();

  }; // class STCluster

  /// Definition of Keyed Container for STCluster
  typedef KeyedContainer<STCluster, Containers::HashMap> STClusters;

  inline std::ostream& operator<<( std::ostream& s, LHCb::STCluster::Spill e ) {
    switch ( e ) {
    case LHCb::STCluster::UnknownSpill:
      return s << "UnknownSpill";
    case LHCb::STCluster::Central:
      return s << "Central";
    case LHCb::STCluster::Prev1:
      return s << "Prev1";
    case LHCb::STCluster::Prev2:
      return s << "Prev2";
    case LHCb::STCluster::Prev3:
      return s << "Prev3";
    case LHCb::STCluster::Prev4:
      return s << "Prev4";
    case LHCb::STCluster::Prev5:
      return s << "Prev5";
    case LHCb::STCluster::Prev6:
      return s << "Prev6";
    case LHCb::STCluster::Prev7:
      return s << "Prev7";
    case LHCb::STCluster::Prev8:
      return s << "Prev8";
    case LHCb::STCluster::Prev9:
      return s << "Prev9";
    case LHCb::STCluster::Prev10:
      return s << "Prev10";
    case LHCb::STCluster::Next1:
      return s << "Next1";
    case LHCb::STCluster::Next2:
      return s << "Next2";
    case LHCb::STCluster::Next3:
      return s << "Next3";
    case LHCb::STCluster::Next4:
      return s << "Next4";
    case LHCb::STCluster::Next5:
      return s << "Next5";
    case LHCb::STCluster::Next6:
      return s << "Next6";
    case LHCb::STCluster::Next7:
      return s << "Next7";
    case LHCb::STCluster::Next8:
      return s << "Next8";
    case LHCb::STCluster::Next9:
      return s << "Next9";
    case LHCb::STCluster::Next10:
      return s << "Next10";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::STCluster::Spill";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::STCluster::clID() const { return LHCb::STCluster::classID(); }

inline const CLID& LHCb::STCluster::classID() { return CLID_STCluster; }

inline const GaudiUtils::VectorMap<std::string, LHCb::STCluster::Spill>& LHCb::STCluster::s_SpillTypMap() {
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

inline LHCb::STCluster::Spill LHCb::STCluster::SpillToType( const std::string& aName ) {
  auto iter = s_SpillTypMap().find( aName );
  return iter != s_SpillTypMap().end() ? iter->second : UnknownSpill;
}

inline const std::string& LHCb::STCluster::SpillToString( int aEnum ) {
  static const std::string s_UnknownSpill = "UnknownSpill";
  auto                     iter           = std::find_if( s_SpillTypMap().begin(), s_SpillTypMap().end(),
                            [&]( const std::pair<const std::string, Spill>& i ) { return i.second == aEnum; } );
  return iter != s_SpillTypMap().end() ? iter->first : s_UnknownSpill;
}

inline const LHCb::STLiteCluster& LHCb::STCluster::liteCluster() const { return m_liteCluster; }

inline const LHCb::STCluster::ADCVector& LHCb::STCluster::stripValues() const { return m_stripValues; }

inline void LHCb::STCluster::setStripValues( const ADCVector& value ) { m_stripValues = value; }

inline double LHCb::STCluster::neighbourSum() const { return m_neighbourSum; }

inline unsigned int LHCb::STCluster::sourceID() const { return m_sourceID; }

inline unsigned int LHCb::STCluster::tell1Channel() const { return m_tell1Channel; }

inline const LHCb::STCluster::Spill& LHCb::STCluster::spill() const { return m_spill; }

inline LHCb::STCluster* LHCb::STCluster::clone() const { return new LHCb::STCluster( *this ); }

inline unsigned int LHCb::STCluster::size() const { return stripValues().size(); }

inline unsigned int LHCb::STCluster::adcValue( unsigned int num ) const { return stripValues()[num].second; }

inline int LHCb::STCluster::strip( unsigned int num ) const { return stripValues()[num].first; }

inline LHCb::STChannelID LHCb::STCluster::channelID() const { return m_liteCluster.channelID(); }

inline double LHCb::STCluster::totalCharge() const {

  double                    sum  = 0;
  ADCVector::const_iterator iter = m_stripValues.begin();
  for ( ; iter != m_stripValues.end(); ++iter ) { sum += iter->second; }
  return sum;
}

inline double LHCb::STCluster::interStripFraction() const { return m_liteCluster.interStripFraction(); }

inline unsigned int LHCb::STCluster::pseudoSize() const { return m_liteCluster.pseudoSize(); }

inline bool LHCb::STCluster::highThreshold() const { return m_liteCluster.highThreshold(); }

inline LHCb::STChannelID LHCb::STCluster::firstChannel() const {

  return STChannelID( channelID().type(), channelID().station(), channelID().layer(), channelID().detRegion(),
                      channelID().sector(), channelID().strip() + stripValues().front().first );
}

inline LHCb::STChannelID LHCb::STCluster::lastChannel() const {

  return STChannelID( channelID().type(), channelID().station(), channelID().layer(), channelID().detRegion(),
                      channelID().sector(), channelID().strip() + stripValues().back().first );
}

inline std::vector<LHCb::STChannelID> LHCb::STCluster::channels() const {

  std::vector<LHCb::STChannelID> chanCont;
  STChannelID                    first = firstChannel();
  for ( unsigned int i = 0; i < size(); ++i ) {
    STChannelID aChan( first.type(), first.station(), first.layer(), first.detRegion(), first.sector(),
                       first.strip() + i );
    chanCont.push_back( aChan );
  }
  return chanCont;
}

inline bool LHCb::STCluster::contains( const LHCb::STChannelID& chan ) const {

  bool inside = false;
  if ( chan.uniqueSector() == channelID().uniqueSector() ) {
    const unsigned int firstStrip = channelID().strip() + stripValues().front().first;
    const unsigned int lastStrip  = channelID().strip() + stripValues().back().first;
    if ( chan.strip() >= firstStrip && chan.strip() <= lastStrip ) inside = true;
  }
  return inside;
}

inline bool LHCb::STCluster::overlaps( const STCluster* testCluster ) const {

  bool overlap = false;
  if ( channelID().uniqueSector() == testCluster->channelID().uniqueSector() ) {
    std::vector<LHCb::STChannelID> testchans = testCluster->channels();
    for ( std::vector<LHCb::STChannelID>::const_iterator iterC1 = testchans.begin();
          iterC1 != testchans.end() && overlap == false; ++iterC1 ) {
      if ( contains( *iterC1 ) == true ) overlap = true;
    } // loop chans
  }   // if
  return overlap;
}

inline unsigned int LHCb::STCluster::maxADCValue() const {

  unsigned int                               maxCharge = 0u;
  LHCb::STCluster::ADCVector::const_iterator iter      = stripValues().begin();
  while ( iter != stripValues().end() ) {
    if ( iter->second > maxCharge ) maxCharge = iter->second;
    ++iter;
  }
  return maxCharge;
}

inline std::ostream& LHCb::STCluster::fillStream( std::ostream& s ) const {

  s << "{ STCluster with key: " << (int)key() << " channel: " << sectorName() << " strip " << strip() << "Size "
    << size() << " charge :" << totalCharge() << " source ID " << sourceID() << " tell1 channel " << tell1Channel()
    << " Spill " << spill() << std::endl
    << " }";
  return s;
}

inline bool LHCb::STCluster::isTT() const { return channelID().isTT(); }

inline bool LHCb::STCluster::isIT() const { return channelID().isIT(); }

inline unsigned int LHCb::STCluster::station() const { return channelID().station(); }

inline unsigned int LHCb::STCluster::layer() const { return channelID().layer(); }

inline unsigned int LHCb::STCluster::detRegion() const { return channelID().detRegion(); }

inline unsigned int LHCb::STCluster::sector() const { return channelID().sector(); }

inline unsigned int LHCb::STCluster::strip() const { return channelID().strip(); }

inline std::string LHCb::STCluster::sectorName() const {

  return ( isTT() ? TTNames().UniqueSectorToString( channelID() ) : ITNames().UniqueSectorToString( channelID() ) );
}

inline std::string LHCb::STCluster::layerName() const {

  return ( isTT() ? TTNames().UniqueLayerToString( channelID() ) : ITNames().UniqueLayerToString( channelID() ) );
}

inline std::string LHCb::STCluster::detRegionName() const {

  return ( isTT() ? TTNames().UniqueRegionToString( channelID() ) : ITNames().UniqueBoxToString( channelID() ) );
}

inline std::string LHCb::STCluster::stationName() const {

  return ( isTT() ? TTNames().StationToString( channelID() ) : ITNames().StationToString( channelID() ) );
}
