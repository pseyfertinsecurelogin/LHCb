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
#include "GaudiKernel/VectorMap.h"
#include "Kernel/STChannelID.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class TTNames TTNames.h
   *
   *
   *
   * @author M Needham
   *
   */

  class TTNames {
  public:
    /// Station names
    enum Station { UnknownStation = 0, TTa = 1, TTb = 2 };
    /// Region names
    enum detRegion { UnknownRegion = 0, RegionC = 1, RegionB = 2, RegionA = 3 };

    /// Default Constructor
    TTNames() = default;

    /// Default Destructor
    ~TTNames() = default;

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// conversion of string to enum for type Station
    static LHCb::TTNames::Station StationToType( const std::string& aName );

    /// conversion to string for enum type Station
    static const std::string& StationToString( int aEnum );
    /// conversion of string to enum for type detRegion
    static LHCb::TTNames::detRegion detRegionToType( const std::string& aName );

    /// conversion to string for enum type detRegion
    static const std::string& detRegionToString( int aEnum );

    /// detector name
    std::string detector();

    /// station string from id
    std::string StationToString( const LHCb::STChannelID& chan );

    /// channel to string from id
    std::string channelToString( const LHCb::STChannelID& chan );

    /// unique box string from id
    std::string UniqueRegionToString( const LHCb::STChannelID& chan );

    /// unique layer string from id
    std::string UniqueLayerToString( const LHCb::STChannelID& chan );

    /// unique sector string from id
    std::string UniqueSectorToString( const LHCb::STChannelID& chan );

    /// sector string from id
    std::string SectorToString( const LHCb::STChannelID& chan );

    /// vector of string names for all stations
    std::vector<std::string> allStations();

    /// vector of string names for all regions
    std::vector<std::string> allDetRegions();

    /// vector of string names for all layers
    std::vector<std::string> allLayers();

    /// vector of string names for stations
    std::vector<std::string> stations();

    /// vector of string names for regions
    std::vector<std::string> detRegions();

    /// vector of string names for layers
    const std::vector<std::string>& layers();

    /// convert string to channel
    STChannelID stringToChannel( const std::string& name );

    friend std::ostream& operator<<( std::ostream& str, const TTNames& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// find type in vector
    unsigned int findStationType( const std::string& testName, const std::vector<std::string>& names );

    /// find type in vector
    unsigned int findRegionType( const std::string& testName, const std::vector<std::string>& names );

    /// convert string to int
    unsigned int toInt( const std::string& str );

    static const GaudiUtils::VectorMap<std::string, Station>&   s_StationTypMap();
    static const GaudiUtils::VectorMap<std::string, detRegion>& s_detRegionTypMap();

  }; // class TTNames

  inline std::ostream& operator<<( std::ostream& s, LHCb::TTNames::Station e ) {
    switch ( e ) {
    case LHCb::TTNames::UnknownStation:
      return s << "UnknownStation";
    case LHCb::TTNames::TTa:
      return s << "TTa";
    case LHCb::TTNames::TTb:
      return s << "TTb";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::TTNames::Station";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::TTNames::detRegion e ) {
    switch ( e ) {
    case LHCb::TTNames::UnknownRegion:
      return s << "UnknownRegion";
    case LHCb::TTNames::RegionC:
      return s << "RegionC";
    case LHCb::TTNames::RegionB:
      return s << "RegionB";
    case LHCb::TTNames::RegionA:
      return s << "RegionA";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::TTNames::detRegion";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::TTNames::fillStream( std::ostream& s ) const { return s; }

inline const GaudiUtils::VectorMap<std::string, LHCb::TTNames::Station>& LHCb::TTNames::s_StationTypMap() {
  static const GaudiUtils::VectorMap<std::string, Station> m = {
      {"UnknownStation", UnknownStation}, {"TTa", TTa}, {"TTb", TTb}};
  return m;
}

inline LHCb::TTNames::Station LHCb::TTNames::StationToType( const std::string& aName ) {
  auto iter = s_StationTypMap().find( aName );
  return iter != s_StationTypMap().end() ? iter->second : UnknownStation;
}

inline const std::string& LHCb::TTNames::StationToString( int aEnum ) {
  static const std::string s_UnknownStation = "UnknownStation";
  auto                     iter             = std::find_if( s_StationTypMap().begin(), s_StationTypMap().end(),
                            [&]( const std::pair<const std::string, Station>& i ) { return i.second == aEnum; } );
  return iter != s_StationTypMap().end() ? iter->first : s_UnknownStation;
}
inline const GaudiUtils::VectorMap<std::string, LHCb::TTNames::detRegion>& LHCb::TTNames::s_detRegionTypMap() {
  static const GaudiUtils::VectorMap<std::string, detRegion> m = {
      {"UnknownRegion", UnknownRegion}, {"RegionC", RegionC}, {"RegionB", RegionB}, {"RegionA", RegionA}};
  return m;
}

inline LHCb::TTNames::detRegion LHCb::TTNames::detRegionToType( const std::string& aName ) {
  auto iter = s_detRegionTypMap().find( aName );
  return iter != s_detRegionTypMap().end() ? iter->second : UnknownRegion;
}

inline const std::string& LHCb::TTNames::detRegionToString( int aEnum ) {
  static const std::string s_UnknownRegion = "UnknownRegion";
  auto                     iter            = std::find_if( s_detRegionTypMap().begin(), s_detRegionTypMap().end(),
                            [&]( const std::pair<const std::string, detRegion>& i ) { return i.second == aEnum; } );
  return iter != s_detRegionTypMap().end() ? iter->first : s_UnknownRegion;
}

inline std::string LHCb::TTNames::detector() { return "TT"; }

inline std::string LHCb::TTNames::StationToString( const LHCb::STChannelID& chan ) {
  return StationToString( chan.station() );
}

inline std::string LHCb::TTNames::UniqueRegionToString( const LHCb::STChannelID& chan ) {

  std::string temp = UniqueLayerToString( chan );
  temp += detRegionToString( chan.detRegion() );
  return temp;
}
