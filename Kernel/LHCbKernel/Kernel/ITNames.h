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

  /** @class ITNames ITNames.h
   *
   *
   *
   * @author M Needham
   *
   */

  class ITNames {
  public:
    /// Station names
    enum Station { UnknownStation = 0, IT1 = 1, IT2 = 2, IT3 = 3 };
    /// Box names
    enum Box { UnknownBox = 0, CSide = 1, ASide = 2, Bottom = 3, Top = 4 };
    /// Layer names
    enum Layer { UnknownLayer = 0, X1 = 1, U = 2, V = 3, X2 = 4 };

    /// Default Constructor
    ITNames() = default;

    /// Default Destructor
    ~ITNames() = default;

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const;

    /// conversion of string to enum for type Station
    static LHCb::ITNames::Station StationToType( const std::string& aName );

    /// conversion to string for enum type Station
    static const std::string& StationToString( int aEnum );
    /// conversion of string to enum for type Box
    static LHCb::ITNames::Box BoxToType( const std::string& aName );

    /// conversion to string for enum type Box
    static const std::string& BoxToString( int aEnum );
    /// conversion of string to enum for type Layer
    static LHCb::ITNames::Layer LayerToType( const std::string& aName );

    /// conversion to string for enum type Layer
    static const std::string& LayerToString( int aEnum );

    /// detector name
    std::string detector();

    /// station string from id
    std::string StationToString( const LHCb::STChannelID& chan );

    /// box string from id
    std::string BoxToString( const LHCb::STChannelID& chan );

    /// layer string from id
    std::string LayerToString( const LHCb::STChannelID& chan );

    /// unique box string from id
    std::string UniqueBoxToString( const LHCb::STChannelID& chan );

    /// unique layer string from id
    std::string UniqueLayerToString( const LHCb::STChannelID& chan );

    /// unique sector string from id
    std::string UniqueSectorToString( const LHCb::STChannelID& chan );

    /// sector string from id
    std::string SectorToString( const LHCb::STChannelID& chan );

    /// channel to string from id
    std::string channelToString( const LHCb::STChannelID& chan );

    /// vector of string names for all stations
    std::vector<std::string> allStations();

    /// vector of string names for all boxes
    std::vector<std::string> allBoxes();

    /// vector of string names for all layers
    std::vector<std::string> allLayers();

    /// vector of string names for stations
    std::vector<std::string> stations();

    /// vector of string names for boxes
    std::vector<std::string> boxes();

    /// vector of string names for layers
    std::vector<std::string> layers();

    /// convert string to channel
    STChannelID stringToChannel( const std::string& name );

    friend std::ostream& operator<<( std::ostream& str, const ITNames& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// find type in vector
    unsigned int findStationType( const std::string& testName, const std::vector<std::string>& names );

    /// find type in vector
    unsigned int findLayerType( const std::string& testName, const std::vector<std::string>& names );

    /// find type in vector
    unsigned int findBoxType( const std::string& testName, const std::vector<std::string>& names );

    /// convert string to int
    unsigned int toInt( const std::string& str );

    static const GaudiUtils::VectorMap<std::string, Station>& s_StationTypMap();
    static const GaudiUtils::VectorMap<std::string, Box>&     s_BoxTypMap();
    static const GaudiUtils::VectorMap<std::string, Layer>&   s_LayerTypMap();

  }; // class ITNames

  inline std::ostream& operator<<( std::ostream& s, LHCb::ITNames::Station e ) {
    switch ( e ) {
    case LHCb::ITNames::UnknownStation:
      return s << "UnknownStation";
    case LHCb::ITNames::IT1:
      return s << "IT1";
    case LHCb::ITNames::IT2:
      return s << "IT2";
    case LHCb::ITNames::IT3:
      return s << "IT3";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ITNames::Station";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ITNames::Box e ) {
    switch ( e ) {
    case LHCb::ITNames::UnknownBox:
      return s << "UnknownBox";
    case LHCb::ITNames::CSide:
      return s << "CSide";
    case LHCb::ITNames::ASide:
      return s << "ASide";
    case LHCb::ITNames::Bottom:
      return s << "Bottom";
    case LHCb::ITNames::Top:
      return s << "Top";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ITNames::Box";
    }
  }

  inline std::ostream& operator<<( std::ostream& s, LHCb::ITNames::Layer e ) {
    switch ( e ) {
    case LHCb::ITNames::UnknownLayer:
      return s << "UnknownLayer";
    case LHCb::ITNames::X1:
      return s << "X1";
    case LHCb::ITNames::U:
      return s << "U";
    case LHCb::ITNames::V:
      return s << "V";
    case LHCb::ITNames::X2:
      return s << "X2";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::ITNames::Layer";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::ITNames::fillStream( std::ostream& s ) const { return s; }

inline const GaudiUtils::VectorMap<std::string, LHCb::ITNames::Station>& LHCb::ITNames::s_StationTypMap() {
  static const GaudiUtils::VectorMap<std::string, Station> m = {
      {"UnknownStation", UnknownStation}, {"IT1", IT1}, {"IT2", IT2}, {"IT3", IT3}};
  return m;
}

inline LHCb::ITNames::Station LHCb::ITNames::StationToType( const std::string& aName ) {
  auto iter = s_StationTypMap().find( aName );
  return iter != s_StationTypMap().end() ? iter->second : UnknownStation;
}

inline const std::string& LHCb::ITNames::StationToString( int aEnum ) {
  static const std::string s_UnknownStation = "UnknownStation";
  auto                     iter             = std::find_if( s_StationTypMap().begin(), s_StationTypMap().end(),
                            [&]( const std::pair<const std::string, Station>& i ) { return i.second == aEnum; } );
  return iter != s_StationTypMap().end() ? iter->first : s_UnknownStation;
}
inline const GaudiUtils::VectorMap<std::string, LHCb::ITNames::Box>& LHCb::ITNames::s_BoxTypMap() {
  static const GaudiUtils::VectorMap<std::string, Box> m = {
      {"UnknownBox", UnknownBox}, {"CSide", CSide}, {"ASide", ASide}, {"Bottom", Bottom}, {"Top", Top}};
  return m;
}

inline LHCb::ITNames::Box LHCb::ITNames::BoxToType( const std::string& aName ) {
  auto iter = s_BoxTypMap().find( aName );
  return iter != s_BoxTypMap().end() ? iter->second : UnknownBox;
}

inline const std::string& LHCb::ITNames::BoxToString( int aEnum ) {
  static const std::string s_UnknownBox = "UnknownBox";
  auto                     iter         = std::find_if( s_BoxTypMap().begin(), s_BoxTypMap().end(),
                            [&]( const std::pair<const std::string, Box>& i ) { return i.second == aEnum; } );
  return iter != s_BoxTypMap().end() ? iter->first : s_UnknownBox;
}
inline const GaudiUtils::VectorMap<std::string, LHCb::ITNames::Layer>& LHCb::ITNames::s_LayerTypMap() {
  static const GaudiUtils::VectorMap<std::string, Layer> m = {
      {"UnknownLayer", UnknownLayer}, {"X1", X1}, {"U", U}, {"V", V}, {"X2", X2}};
  return m;
}

inline LHCb::ITNames::Layer LHCb::ITNames::LayerToType( const std::string& aName ) {
  auto iter = s_LayerTypMap().find( aName );
  return iter != s_LayerTypMap().end() ? iter->second : UnknownLayer;
}

inline const std::string& LHCb::ITNames::LayerToString( int aEnum ) {
  static const std::string s_UnknownLayer = "UnknownLayer";
  auto                     iter           = std::find_if( s_LayerTypMap().begin(), s_LayerTypMap().end(),
                            [&]( const std::pair<const std::string, Layer>& i ) { return i.second == aEnum; } );
  return iter != s_LayerTypMap().end() ? iter->first : s_UnknownLayer;
}

inline std::string LHCb::ITNames::detector() { return "IT"; }

inline std::string LHCb::ITNames::StationToString( const LHCb::STChannelID& chan ) {
  return StationToString( chan.station() );
}

inline std::string LHCb::ITNames::BoxToString( const LHCb::STChannelID& chan ) {
  return BoxToString( chan.detRegion() );
}

inline std::string LHCb::ITNames::LayerToString( const LHCb::STChannelID& chan ) {
  return LayerToString( chan.layer() );
}

inline std::string LHCb::ITNames::UniqueBoxToString( const LHCb::STChannelID& chan ) {
  std::string temp = StationToString( chan.station() );
  temp += BoxToString( chan.detRegion() );
  return temp;
}

inline std::string LHCb::ITNames::UniqueLayerToString( const LHCb::STChannelID& chan ) {

  std::string temp = StationToString( chan.station() );
  temp += BoxToString( chan.detRegion() );
  temp += LayerToString( chan.layer() );
  return temp;
}
