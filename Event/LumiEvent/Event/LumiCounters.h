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
  static const CLID CLID_LumiCounters = 13500;

  /** @class LumiCounters LumiCounters.h
   *
   * Enum class for Lumi counters
   *
   * @author Jaap Panman
   *
   */

  class LumiCounters final : public DataObject {
  public:
    /// lookup table for counter keys
    enum counterKey {
      PUMult          = 0,  // number of PU hits
      RZVelo          = 1,  // number of Velo tracks
      RZVeloBW        = 2,  // number of backward RZVelo tracks
      Velo            = 3,  // number of Velo tracks pointing to IR
      Muon            = 4,  // number of muon tracks
      BKMuon          = 5,  // number of muon tracks NOT pointing to PV
      SPDMult         = 6,  // number of SPD hits
      CaloEt          = 7,  // Calo Et
      TTIP            = 8,  // number of TT tracks from the IP
      TTMIB           = 9,  // number of TT tracks parallel with beam
      PV2D            = 10, // number of 2D vertices
      PV3D            = 11, // number of 3D vertices
      PU              = 13, // number of PU vertices
      Vertex          = 14, // number of vertices accepted in IR
      Method          = 20, // method: 0 or missing is random method
      Random          = 21, // random method: 0 or missing is L0 method
      PoissonPUMult   = 50, // number of times PU hits over threshold
      PoissonRZVelo   = 51, // number of times Velo tracks over threshold
      PoissonRZVeloBW = 52, // number of times backward RZVelo tracks over threshold
      PoissonVelo     = 53, // number of times Velo tracks pointing to IR over threshold
      PoissonMuon     = 54, // number of times muon tracks over threshold
      PoissonBKMuon   = 55, // number of times muon tracks NOT pointing to PV over threshold
      PoissonSPDMult  = 56, // number of times SPD hits over threshold
      PoissonCaloEt   = 57, // number of times Calo Et over threshold
      PoissonTTIP     = 58, // number of timesTT tracks from the IP over threshold
      PoissonTTMIB    = 59, // number of timesTT tracks parallel with beam over threshold
      PoissonPV2D     = 60, // number of times2D vertices over threshold
      PoissonPV3D     = 61, // number of times 3D vertices over threshold
      PoissonPU       = 63, // number of times PU vertices over threshold
      PoissonVertex   = 64, // number of times vertices in IR range over threshold
      PoissonMethod   = 70, // method: 0 or missing is random method
      PoissonRandom   = 71, // random method: 0 or missing is L0 method
      Unknown         = 98, // unknown value
      LastGlobal      = 99  // User variables should use flags greater than this value
    };

    /// Default Constructor
    LumiCounters() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type counterKey
    static LHCb::LumiCounters::counterKey counterKeyToType( const std::string& aName );

    /// conversion to string for enum type counterKey
    static const std::string& counterKeyToString( int aEnum );

    /// intelligent printout
    std::ostream& fillStream( std::ostream& s ) const override;

    friend std::ostream& operator<<( std::ostream& str, const LumiCounters& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    static const GaudiUtils::VectorMap<std::string, counterKey>& s_counterKeyTypMap();

  }; // class LumiCounters

  inline std::ostream& operator<<( std::ostream& s, LHCb::LumiCounters::counterKey e ) {
    switch ( e ) {
    case LHCb::LumiCounters::PUMult:
      return s << "PUMult";
    case LHCb::LumiCounters::RZVelo:
      return s << "RZVelo";
    case LHCb::LumiCounters::RZVeloBW:
      return s << "RZVeloBW";
    case LHCb::LumiCounters::Velo:
      return s << "Velo";
    case LHCb::LumiCounters::Muon:
      return s << "Muon";
    case LHCb::LumiCounters::BKMuon:
      return s << "BKMuon";
    case LHCb::LumiCounters::SPDMult:
      return s << "SPDMult";
    case LHCb::LumiCounters::CaloEt:
      return s << "CaloEt";
    case LHCb::LumiCounters::TTIP:
      return s << "TTIP";
    case LHCb::LumiCounters::TTMIB:
      return s << "TTMIB";
    case LHCb::LumiCounters::PV2D:
      return s << "PV2D";
    case LHCb::LumiCounters::PV3D:
      return s << "PV3D";
    case LHCb::LumiCounters::PU:
      return s << "PU";
    case LHCb::LumiCounters::Vertex:
      return s << "Vertex";
    case LHCb::LumiCounters::Method:
      return s << "Method";
    case LHCb::LumiCounters::Random:
      return s << "Random";
    case LHCb::LumiCounters::PoissonPUMult:
      return s << "PoissonPUMult";
    case LHCb::LumiCounters::PoissonRZVelo:
      return s << "PoissonRZVelo";
    case LHCb::LumiCounters::PoissonRZVeloBW:
      return s << "PoissonRZVeloBW";
    case LHCb::LumiCounters::PoissonVelo:
      return s << "PoissonVelo";
    case LHCb::LumiCounters::PoissonMuon:
      return s << "PoissonMuon";
    case LHCb::LumiCounters::PoissonBKMuon:
      return s << "PoissonBKMuon";
    case LHCb::LumiCounters::PoissonSPDMult:
      return s << "PoissonSPDMult";
    case LHCb::LumiCounters::PoissonCaloEt:
      return s << "PoissonCaloEt";
    case LHCb::LumiCounters::PoissonTTIP:
      return s << "PoissonTTIP";
    case LHCb::LumiCounters::PoissonTTMIB:
      return s << "PoissonTTMIB";
    case LHCb::LumiCounters::PoissonPV2D:
      return s << "PoissonPV2D";
    case LHCb::LumiCounters::PoissonPV3D:
      return s << "PoissonPV3D";
    case LHCb::LumiCounters::PoissonPU:
      return s << "PoissonPU";
    case LHCb::LumiCounters::PoissonVertex:
      return s << "PoissonVertex";
    case LHCb::LumiCounters::PoissonMethod:
      return s << "PoissonMethod";
    case LHCb::LumiCounters::PoissonRandom:
      return s << "PoissonRandom";
    case LHCb::LumiCounters::Unknown:
      return s << "Unknown";
    case LHCb::LumiCounters::LastGlobal:
      return s << "LastGlobal";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::LumiCounters::counterKey";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::LumiCounters::clID() const { return LHCb::LumiCounters::classID(); }

inline const CLID& LHCb::LumiCounters::classID() { return CLID_LumiCounters; }

inline const GaudiUtils::VectorMap<std::string, LHCb::LumiCounters::counterKey>&
LHCb::LumiCounters::s_counterKeyTypMap() {
  static const GaudiUtils::VectorMap<std::string, counterKey> m = {{"PUMult", PUMult},
                                                                   {"RZVelo", RZVelo},
                                                                   {"RZVeloBW", RZVeloBW},
                                                                   {"Velo", Velo},
                                                                   {"Muon", Muon},
                                                                   {"BKMuon", BKMuon},
                                                                   {"SPDMult", SPDMult},
                                                                   {"CaloEt", CaloEt},
                                                                   {"TTIP", TTIP},
                                                                   {"TTMIB", TTMIB},
                                                                   {"PV2D", PV2D},
                                                                   {"PV3D", PV3D},
                                                                   {"PU", PU},
                                                                   {"Vertex", Vertex},
                                                                   {"Method", Method},
                                                                   {"Random", Random},
                                                                   {"PoissonPUMult", PoissonPUMult},
                                                                   {"PoissonRZVelo", PoissonRZVelo},
                                                                   {"PoissonRZVeloBW", PoissonRZVeloBW},
                                                                   {"PoissonVelo", PoissonVelo},
                                                                   {"PoissonMuon", PoissonMuon},
                                                                   {"PoissonBKMuon", PoissonBKMuon},
                                                                   {"PoissonSPDMult", PoissonSPDMult},
                                                                   {"PoissonCaloEt", PoissonCaloEt},
                                                                   {"PoissonTTIP", PoissonTTIP},
                                                                   {"PoissonTTMIB", PoissonTTMIB},
                                                                   {"PoissonPV2D", PoissonPV2D},
                                                                   {"PoissonPV3D", PoissonPV3D},
                                                                   {"PoissonPU", PoissonPU},
                                                                   {"PoissonVertex", PoissonVertex},
                                                                   {"PoissonMethod", PoissonMethod},
                                                                   {"PoissonRandom", PoissonRandom},
                                                                   {"Unknown", Unknown},
                                                                   {"LastGlobal", LastGlobal}};
  return m;
}

inline LHCb::LumiCounters::counterKey LHCb::LumiCounters::counterKeyToType( const std::string& aName ) {
  auto iter = s_counterKeyTypMap().find( aName );
  return iter != s_counterKeyTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::LumiCounters::counterKeyToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter      = std::find_if( s_counterKeyTypMap().begin(), s_counterKeyTypMap().end(),
                            [&]( const std::pair<const std::string, counterKey>& i ) { return i.second == aEnum; } );
  return iter != s_counterKeyTypMap().end() ? iter->first : s_Unknown;
}

inline std::ostream& LHCb::LumiCounters::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ }";

  return ss;
}
