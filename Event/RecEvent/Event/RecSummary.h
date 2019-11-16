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
#include "GaudiKernel/DataObject.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_RecSummary = 106;

  // Namespace for locations in TDS
  namespace RecSummaryLocation {
    inline const std::string Default = "Rec/Summary";
    inline const std::string Track   = "Rec/TrackSummary";
  } // namespace RecSummaryLocation

  /** @class RecSummary RecSummary.h
   *
   * Holds basic summary information from the reconstruction
   *
   * @author C. Jones
   *
   */

  class RecSummary final : public DataObject {
  public:
    /// Vector of additional information
    typedef GaudiUtils::VectorMap<int, int> SummaryData;

    /// List of summary data
    enum DataTypes {
      nPVs              = 0,   // Number of reconstructed Primary Vertices
      nLongTracks       = 10,  // Number of reconstructed Long Tracks
      nDownstreamTracks = 11,  // Number of reconstructed Downstream Tracks
      nUpstreamTracks   = 12,  // Number of reconstructed Upstream Tracks
      nVeloTracks       = 13,  // Number of reconstructed VELO Tracks
      nTTracks          = 14,  // Number of reconstructed T-station Tracks
      nBackTracks       = 15,  // Number of reconstructed VELO backwards Tracks
      nTracks           = 16,  // Number of reconstructed Tracks
      nGhosts           = 17,  // Number of identified ghost Tracks
      nRich1Hits        = 20,  // Number of hits in RICH1
      nRich2Hits        = 21,  // Number of hits in RICH2
      nVeloClusters     = 30,  // Number of VELO clusters
      nITClusters       = 40,  // Number of IT clusters
      nTTClusters       = 50,  // Number of TT clusters
      nUTClusters       = 51,  // Number of UT clusters
      nOTClusters       = 60,  // Number of OT clusters
      nFTClusters       = 41,  // Number of FT clusters
      nSPDhits          = 70,  // Number of SPD hits
      nMuonCoordsS0     = 80,  // Number of Coords in Muon Station 0
      nMuonCoordsS1     = 91,  // Number of Coords in Muon Station 1
      nMuonCoordsS2     = 92,  // Number of Coords in Muon Station 2
      nMuonCoordsS3     = 93,  // Number of Coords in Muon Station 3
      nMuonCoordsS4     = 94,  // Number of Coords in Muon Station 4
      nMuonTracks       = 95,  // Number of Muon Tracks
      TypeUnknown       = 1000 // Unknown RecSummary entry
    };

    /// Default Constructor
    RecSummary() = default;

    // Retrieve pointer to class definition structure
    [[nodiscard]] const CLID& clID() const override;
    static const CLID&        classID();

    /// conversion of string to enum for type DataTypes
    static LHCb::RecSummary::DataTypes DataTypesToType( const std::string& aName );

    /// conversion to string for enum type DataTypes
    static const std::string& DataTypesToString( int aEnum );

    /// Does the information exist for the given key
    [[nodiscard]] bool hasInfo( int key ) const;

    /// Add or replace new information, associated with the specified key
    bool addInfo( int key, int info );

    /// Extract the information associated with the specified key. If there is no such information the default value
    /// will be returned.
    [[nodiscard]] int info( int key, int def ) const;

    /// Erase the information associated with the specified key
    RecSummary::SummaryData::size_type eraseInfo( int key );

    /// Print this ProtoParticle in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Storage of the summary data
    [[nodiscard]] const SummaryData& summaryData() const;

    friend std::ostream& operator<<( std::ostream& str, const RecSummary& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    SummaryData m_summaryData; ///< Storage of the summary data

    static const GaudiUtils::VectorMap<std::string, DataTypes>& s_DataTypesTypMap();

  }; // class RecSummary

  inline std::ostream& operator<<( std::ostream& s, LHCb::RecSummary::DataTypes e ) {
    switch ( e ) {
    case LHCb::RecSummary::nPVs:
      return s << "nPVs";
    case LHCb::RecSummary::nLongTracks:
      return s << "nLongTracks";
    case LHCb::RecSummary::nDownstreamTracks:
      return s << "nDownstreamTracks";
    case LHCb::RecSummary::nUpstreamTracks:
      return s << "nUpstreamTracks";
    case LHCb::RecSummary::nVeloTracks:
      return s << "nVeloTracks";
    case LHCb::RecSummary::nTTracks:
      return s << "nTTracks";
    case LHCb::RecSummary::nBackTracks:
      return s << "nBackTracks";
    case LHCb::RecSummary::nTracks:
      return s << "nTracks";
    case LHCb::RecSummary::nGhosts:
      return s << "nGhosts";
    case LHCb::RecSummary::nRich1Hits:
      return s << "nRich1Hits";
    case LHCb::RecSummary::nRich2Hits:
      return s << "nRich2Hits";
    case LHCb::RecSummary::nVeloClusters:
      return s << "nVeloClusters";
    case LHCb::RecSummary::nITClusters:
      return s << "nITClusters";
    case LHCb::RecSummary::nTTClusters:
      return s << "nTTClusters";
    case LHCb::RecSummary::nUTClusters:
      return s << "nUTClusters";
    case LHCb::RecSummary::nOTClusters:
      return s << "nOTClusters";
    case LHCb::RecSummary::nFTClusters:
      return s << "nFTClusters";
    case LHCb::RecSummary::nSPDhits:
      return s << "nSPDhits";
    case LHCb::RecSummary::nMuonCoordsS0:
      return s << "nMuonCoordsS0";
    case LHCb::RecSummary::nMuonCoordsS1:
      return s << "nMuonCoordsS1";
    case LHCb::RecSummary::nMuonCoordsS2:
      return s << "nMuonCoordsS2";
    case LHCb::RecSummary::nMuonCoordsS3:
      return s << "nMuonCoordsS3";
    case LHCb::RecSummary::nMuonCoordsS4:
      return s << "nMuonCoordsS4";
    case LHCb::RecSummary::nMuonTracks:
      return s << "nMuonTracks";
    case LHCb::RecSummary::TypeUnknown:
      return s << "TypeUnknown";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::RecSummary::DataTypes";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::RecSummary::clID() const { return LHCb::RecSummary::classID(); }

inline const CLID& LHCb::RecSummary::classID() { return CLID_RecSummary; }

inline const GaudiUtils::VectorMap<std::string, LHCb::RecSummary::DataTypes>& LHCb::RecSummary::s_DataTypesTypMap() {
  static const GaudiUtils::VectorMap<std::string, DataTypes> m = {{"nPVs", nPVs},
                                                                  {"nLongTracks", nLongTracks},
                                                                  {"nDownstreamTracks", nDownstreamTracks},
                                                                  {"nUpstreamTracks", nUpstreamTracks},
                                                                  {"nVeloTracks", nVeloTracks},
                                                                  {"nTTracks", nTTracks},
                                                                  {"nBackTracks", nBackTracks},
                                                                  {"nTracks", nTracks},
                                                                  {"nGhosts", nGhosts},
                                                                  {"nRich1Hits", nRich1Hits},
                                                                  {"nRich2Hits", nRich2Hits},
                                                                  {"nVeloClusters", nVeloClusters},
                                                                  {"nITClusters", nITClusters},
                                                                  {"nTTClusters", nTTClusters},
                                                                  {"nUTClusters", nUTClusters},
                                                                  {"nOTClusters", nOTClusters},
                                                                  {"nFTClusters", nFTClusters},
                                                                  {"nSPDhits", nSPDhits},
                                                                  {"nMuonCoordsS0", nMuonCoordsS0},
                                                                  {"nMuonCoordsS1", nMuonCoordsS1},
                                                                  {"nMuonCoordsS2", nMuonCoordsS2},
                                                                  {"nMuonCoordsS3", nMuonCoordsS3},
                                                                  {"nMuonCoordsS4", nMuonCoordsS4},
                                                                  {"nMuonTracks", nMuonTracks},
                                                                  {"TypeUnknown", TypeUnknown}};
  return m;
}

inline LHCb::RecSummary::DataTypes LHCb::RecSummary::DataTypesToType( const std::string& aName ) {
  auto iter = s_DataTypesTypMap().find( aName );
  return iter != s_DataTypesTypMap().end() ? iter->second : TypeUnknown;
}

inline const std::string& LHCb::RecSummary::DataTypesToString( int aEnum ) {
  static const std::string s_TypeUnknown = "TypeUnknown";
  auto                     iter          = std::find_if( s_DataTypesTypMap().begin(), s_DataTypesTypMap().end(),
                            [&]( const std::pair<const std::string, DataTypes>& i ) { return i.second == aEnum; } );
  return iter != s_DataTypesTypMap().end() ? iter->first : s_TypeUnknown;
}

inline const LHCb::RecSummary::SummaryData& LHCb::RecSummary::summaryData() const { return m_summaryData; }

inline bool LHCb::RecSummary::hasInfo( const int key ) const {

  return m_summaryData.end() != m_summaryData.find( key );
}

inline bool LHCb::RecSummary::addInfo( const int key, const int info ) {

  return m_summaryData.insert( key, info ).second;
}

inline int LHCb::RecSummary::info( const int key, const int def ) const {

  auto i = m_summaryData.find( key );
  return m_summaryData.end() == i ? def : i->second;
}

inline LHCb::RecSummary::SummaryData::size_type LHCb::RecSummary::eraseInfo( const int key ) {

  return m_summaryData.erase( key );
}
