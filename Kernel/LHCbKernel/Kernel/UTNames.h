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
#include "Kernel/UTChannelID.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class UTNames UTNames.h
   *
   *
   *
   * @author Jianchun Wang
   *
   */

  class UTNames {
  public:
    /// Station names
    enum Station { UnknownStation = 0, UTa = 1, UTb = 2 };
    /// Region names
    enum detRegion { UnknownRegion = 0, RegionC = 1, RegionB = 2, RegionA = 3 };

    /// Default Constructor
    UTNames() {}

    /// Default Destructor
    ~UTNames() {}

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// conversion of string to enum for type Station
    static LHCb::UTNames::Station StationToType( const std::string& aName );

    /// conversion to string for enum type Station
    static const std::string& StationToString( int aEnum );
    /// conversion of string to enum for type detRegion
    static LHCb::UTNames::detRegion detRegionToType( const std::string& aName );

    /// conversion to string for enum type detRegion
    static const std::string& detRegionToString( int aEnum );

    /// detector name
    std::string detector();

    /// station string from id
    std::string StationToString( const LHCb::UTChannelID& chan );

    /// channel to string from id
    std::string channelToString( const LHCb::UTChannelID& chan );

    /// unique box string from id
    std::string UniqueRegionToString( const LHCb::UTChannelID& chan );

    /// unique layer string from id
    std::string UniqueLayerToString( const LHCb::UTChannelID& chan );

    /// unique sector string from id
    std::string UniqueSectorToString( const LHCb::UTChannelID& chan );

    /// sector string from id
    std::string SectorToString( const LHCb::UTChannelID& chan );

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
    UTChannelID stringToChannel( const std::string& name );

    friend std::ostream& operator<<( std::ostream& str, const UTNames& obj ) { return obj.fillStream( str ); }

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

  }; // class UTNames

  inline std::ostream& operator<<( std::ostream& s, LHCb::UTNames::Station e ) {
    switch ( e ) {
    case LHCb::UTNames::UnknownStation:
      return s << "UnknownStation";
    case LHCb::UTNames::UTa:
      return s << "UTa";
    case LHCb::UTNames::UTb:
      return s << "UTb";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::UTNames::Station";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::UTNames::detRegion e ) {
    switch ( e ) {
    case LHCb::UTNames::UnknownRegion:
      return s << "UnknownRegion";
    case LHCb::UTNames::RegionC:
      return s << "RegionC";
    case LHCb::UTNames::RegionB:
      return s << "RegionB";
    case LHCb::UTNames::RegionA:
      return s << "RegionA";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::UTNames::detRegion";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::UTNames::fillStream( std::ostream& s ) const { return s; }

inline const GaudiUtils::VectorMap<std::string, LHCb::UTNames::Station>& LHCb::UTNames::s_StationTypMap() {
  static const GaudiUtils::VectorMap<std::string, Station> m = {
      {"UnknownStation", UnknownStation}, {"UTa", UTa}, {"UTb", UTb}};
  return m;
}

inline LHCb::UTNames::Station LHCb::UTNames::StationToType( const std::string& aName ) {
  auto iter = s_StationTypMap().find( aName );
  return iter != s_StationTypMap().end() ? iter->second : UnknownStation;
}

inline const std::string& LHCb::UTNames::StationToString( int aEnum ) {
  static const std::string s_UnknownStation = "UnknownStation";
  auto                     iter             = std::find_if( s_StationTypMap().begin(), s_StationTypMap().end(),
                            [&]( const std::pair<const std::string, Station>& i ) { return i.second == aEnum; } );
  return iter != s_StationTypMap().end() ? iter->first : s_UnknownStation;
}
inline const GaudiUtils::VectorMap<std::string, LHCb::UTNames::detRegion>& LHCb::UTNames::s_detRegionTypMap() {
  static const GaudiUtils::VectorMap<std::string, detRegion> m = {
      {"UnknownRegion", UnknownRegion}, {"RegionC", RegionC}, {"RegionB", RegionB}, {"RegionA", RegionA}};
  return m;
}

inline LHCb::UTNames::detRegion LHCb::UTNames::detRegionToType( const std::string& aName ) {
  auto iter = s_detRegionTypMap().find( aName );
  return iter != s_detRegionTypMap().end() ? iter->second : UnknownRegion;
}

inline const std::string& LHCb::UTNames::detRegionToString( int aEnum ) {
  static const std::string s_UnknownRegion = "UnknownRegion";
  auto                     iter            = std::find_if( s_detRegionTypMap().begin(), s_detRegionTypMap().end(),
                            [&]( const std::pair<const std::string, detRegion>& i ) { return i.second == aEnum; } );
  return iter != s_detRegionTypMap().end() ? iter->first : s_UnknownRegion;
}

inline std::string LHCb::UTNames::detector() { return "UT"; }

inline std::string LHCb::UTNames::StationToString( const LHCb::UTChannelID& chan ) {
  return StationToString( chan.station() );
}

inline std::string LHCb::UTNames::UniqueRegionToString( const LHCb::UTChannelID& chan ) {

  std::string temp = UniqueLayerToString( chan );
  temp += detRegionToString( chan.detRegion() );
  return temp;
}
