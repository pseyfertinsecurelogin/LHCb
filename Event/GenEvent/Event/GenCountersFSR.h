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
#include <map>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_GenCountersFSR = 13601;

  /** @class GenCountersFSR GenCountersFSR.h
   *
   * Enum class for Generator counters
   *
   * @author Davide Fazzini
   *
   */

  class GenCountersFSR : public DataObject {
  public:
    /// lookup table for counter keys
    enum CounterKey {
      AllEvt = 0, // Total number of events generated, including events with no interactions (empty events). One event
                  // can be made of several interactions
      ZeroInt      = 1,  // Events (amongst the AllEvt) with no interactions
      EvtGenerated = 2,  // Total number of generated events in general of Minimum Bias type (one event contains several
                         // pile-up interactions)
      IntGenerated  = 3, // Total number of generated interactions in the generated events
      EvtAccepted   = 4, // Number of accepted events amongst the generated events (event containing one signal B, ...)
      IntAccepted   = 5, // Number of interactions contained in the accepted events
      BeforeFullEvt = 6, // Number of events before the full event generator level cut
      AfterFullEvt  = 7, // Number of events after the full event generator level cut
      OnebGen       = 10, // Number of generated interactions containing at least 1 b quark
      ThreebGen     = 11, // Number of generated interactions containing at least 3 b quarks
      PromptBGen =
          12, // Number of generated interactions containing at least 1 prompt B (one B hadron without a b quark)
      OnecGen    = 13, // Number of generated interactions containing at least 1 c quark
      ThreecGen  = 14, // Number of generated interactions containing at least 3 c quarks
      PromptCGen = 15, // Number of generated interactions containing at least 1 prompt charmed hadron (one D hadron
                       // without a c quark)
      bAndcGen  = 16,  // Number of generated interactions containing at least 1 b quark and 1 c quark
      OnebAcc   = 17,  // Number of accepted  interactions containing at least 1 b quark
      ThreebAcc = 18,  // Number of accepted  interactions containing at least 3 b quarks
      PromptBAcc =
          19, // Number of accepted  interactions containing at least 1 prompt B (one B hadron without a b quark)
      OnecAcc    = 20,     // Number of accepted  interactions containing at least 1 c quark
      ThreecAcc  = 21,     // Number of accepted  interactions containing at least 3 c quarks
      PromptCAcc = 22,     // Number of accepted  interactions containing at least 1 prompt charmed hadron (one D hadron
                           // without a c quark)
      bAndcAcc       = 23, // Number of accepted  interactions containing at least 1 b quark and 1 c quark
      BeforeLevelCut = 26, // Number of inclusive events generated
      AfterLevelCut  = 27, // Number of inclusive events passing the generator level cut and where at least one selected
                           // particle has pz > 0.
      EvtInverted = 28,    // Number of z-inverted events
      B0Gen =
          30, // Number of generated B0 in events with at least one selected particle (contained in the inclusive list)
      antiB0Gen  = 31, // Number of generated anti-B0 in events with at least one selected particle
      BplusGen   = 32, // Number of generated B+ in events with at least one selected particle
      BminusGen  = 33, // Number of generated B- in events with at least one selected particle
      Bs0Gen     = 34, // Number of generated Bs0 in events with at least one selected particle
      antiBs0Gen = 35, // Number of generated anti-Bs0 in events with at least one selected particle
      bBaryonGen =
          36, // Number of generated b-Baryon (with negative PDG code) in events with at least one selected particle
      antibBaryonGen = 37, // Number of generated anti-b-Baryon (with positive PDG code) in events with at least one
                           // selected particle
      BcplusGen      = 38, // Number of generated Bc+ in events with at least one selected particle
      BcminusGen     = 39, // Number of generated Bc- in events with at least one selected particle
      bbGen          = 40, // Number of generated b anti-b states in events with at least one selected particle
      B0Acc          = 41, // Number of B0 in accepted events (passing the generator level cuts)
      antiB0Acc      = 42, // Number of anti-B0 in accepted events
      BplusAcc       = 43, // Number of B+ in accepted events
      BminusAcc      = 44, // Number of B- in accepted events
      Bs0Acc         = 45, // Number of Bs0 in accepted events
      antiBs0Acc     = 46, // Number of anti-Bs0 in accepted events
      bBaryonAcc     = 47, // Number of b-Baryon (with negative PDG code) in accepted events
      antibBaryonAcc = 48, // Number of anti-b-Baryon (with positive PDG code) in accepted events
      BcplusAcc      = 49, // Number of Bc+ in accepted events
      BcminusAcc     = 50, // Number of Bc- in accepted events
      bbAcc          = 51, // Number of b anti-b states in accepted events
      D0Gen          = 55, // Number of generated D0
      antiD0Gen      = 56, // Number of generated anti-D0
      DplusGen       = 57, // Number of generated D+
      DminusGen      = 58, // Number of generated D-
      DsplusGen      = 59, // Number of generated Ds+
      DsminusGen     = 60, // Number of generated Ds-
      cBaryonGen     = 61, // Number of generated c-Baryon (with positive PDG code)
      anticBaryonGen = 62, // Number of generated anti-c-Baryon (with negative PDG code)
      ccGen          = 63, // Number of generated c anti-c states
      D0Acc          = 64, // Number of D0 in accepted events
      antiD0Acc      = 65, // Number of anti-D0 in accepted events
      DplusAcc       = 66, // Number of D+ in accepted events
      DminusAcc      = 67, // Number of D- in accepted events
      DsplusAcc      = 68, // Number of Ds+ in accepted events
      DsminusAcc     = 69, // Number of Ds- in accepted events
      cBaryonAcc     = 70, // Number of c-Baryon (with positive PDG code) in accepted events
      anticBaryonAcc = 71, // Number of anti-c-Baryon (with negative PDG code) in accepted events
      ccAcc          = 72, // Number of c anti-c states in accepted events
      BGen           = 75, // Number of generated B (spin 0, angular momentum 0)
      BstarGen       = 76, // Number of generated B* (spin 0, angular momentum >= 1)
      B2starGen      = 77, // Number of generated B** (spin >= 1)
      BAcc           = 78, // Number of B (spin 0, angular momentum 0) in accepted events
      BstarAcc       = 79, // Number of B* (spin 0, angular momentum >= 1) in accepted events
      B2starAcc      = 80, // Number of B** (spin >= 1) in accepted events
      DGen           = 85, // Number of generated D (spin 0, angular momentum 0)
      DstarGen       = 86, // Number of generated D* (spin 0, angular momentum >= 1)
      D2starGen      = 87, // Number of generated D** (spin >= 1)
      DAcc           = 88, // Number of D (spin 0, angular momentum 0) in accepted events
      DstarAcc       = 89, // Number of D* (spin 0, angular momentum >= 1) in accepted events
      D2starAcc      = 90, // Number of D** (spin >= 1) in accepted events
      EvtSignal      = 91, // Number of generated signal events
      EvtantiSignal  = 92, // Number of generated anti-signal events
      BeforePCut     = 93, // Number of signal events containing a signal particle generated
      AfterPCut      = 94, // Number of signal particle passing the generator level cut with pz > 0
      BeforeantiPCut = 95, // Number of signal events containing a signal anti-particle generated
      AfterantiPCut  = 96, // Number of signal anti-particle passing the generator level cut with pz > 0
      Unknown        = 98, // Unknown value
      AllsubProcess  = 100 // Total cross-section of Minimum Bias events
    };

    /// Default Constructor
    GenCountersFSR() {}

    /// Default Destructor
    virtual ~GenCountersFSR() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type CounterKey
    static LHCb::GenCountersFSR::CounterKey CounterKeyToType( const std::string& aName );

    /// conversion to string for enum type CounterKey
    static const std::string& CounterKeyToString( int aEnum );

    /// return a map with the complete names of generator counters
    std::map<std::string, std::string> getFullNames();

    /// printout
    std::ostream& fillStream( std::ostream& s ) const override;

    friend std::ostream& operator<<( std::ostream& str, const GenCountersFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    static const GaudiUtils::VectorMap<std::string, CounterKey>& s_CounterKeyTypMap();

  }; // class GenCountersFSR

  inline std::ostream& operator<<( std::ostream& s, LHCb::GenCountersFSR::CounterKey e ) {
    switch ( e ) {
    case LHCb::GenCountersFSR::AllEvt:
      return s << "AllEvt";
    case LHCb::GenCountersFSR::ZeroInt:
      return s << "ZeroInt";
    case LHCb::GenCountersFSR::EvtGenerated:
      return s << "EvtGenerated";
    case LHCb::GenCountersFSR::IntGenerated:
      return s << "IntGenerated";
    case LHCb::GenCountersFSR::EvtAccepted:
      return s << "EvtAccepted";
    case LHCb::GenCountersFSR::IntAccepted:
      return s << "IntAccepted";
    case LHCb::GenCountersFSR::BeforeFullEvt:
      return s << "BeforeFullEvt";
    case LHCb::GenCountersFSR::AfterFullEvt:
      return s << "AfterFullEvt";
    case LHCb::GenCountersFSR::OnebGen:
      return s << "OnebGen";
    case LHCb::GenCountersFSR::ThreebGen:
      return s << "ThreebGen";
    case LHCb::GenCountersFSR::PromptBGen:
      return s << "PromptBGen";
    case LHCb::GenCountersFSR::OnecGen:
      return s << "OnecGen";
    case LHCb::GenCountersFSR::ThreecGen:
      return s << "ThreecGen";
    case LHCb::GenCountersFSR::PromptCGen:
      return s << "PromptCGen";
    case LHCb::GenCountersFSR::bAndcGen:
      return s << "bAndcGen";
    case LHCb::GenCountersFSR::OnebAcc:
      return s << "OnebAcc";
    case LHCb::GenCountersFSR::ThreebAcc:
      return s << "ThreebAcc";
    case LHCb::GenCountersFSR::PromptBAcc:
      return s << "PromptBAcc";
    case LHCb::GenCountersFSR::OnecAcc:
      return s << "OnecAcc";
    case LHCb::GenCountersFSR::ThreecAcc:
      return s << "ThreecAcc";
    case LHCb::GenCountersFSR::PromptCAcc:
      return s << "PromptCAcc";
    case LHCb::GenCountersFSR::bAndcAcc:
      return s << "bAndcAcc";
    case LHCb::GenCountersFSR::BeforeLevelCut:
      return s << "BeforeLevelCut";
    case LHCb::GenCountersFSR::AfterLevelCut:
      return s << "AfterLevelCut";
    case LHCb::GenCountersFSR::EvtInverted:
      return s << "EvtInverted";
    case LHCb::GenCountersFSR::B0Gen:
      return s << "B0Gen";
    case LHCb::GenCountersFSR::antiB0Gen:
      return s << "antiB0Gen";
    case LHCb::GenCountersFSR::BplusGen:
      return s << "BplusGen";
    case LHCb::GenCountersFSR::BminusGen:
      return s << "BminusGen";
    case LHCb::GenCountersFSR::Bs0Gen:
      return s << "Bs0Gen";
    case LHCb::GenCountersFSR::antiBs0Gen:
      return s << "antiBs0Gen";
    case LHCb::GenCountersFSR::bBaryonGen:
      return s << "bBaryonGen";
    case LHCb::GenCountersFSR::antibBaryonGen:
      return s << "antibBaryonGen";
    case LHCb::GenCountersFSR::BcplusGen:
      return s << "BcplusGen";
    case LHCb::GenCountersFSR::BcminusGen:
      return s << "BcminusGen";
    case LHCb::GenCountersFSR::bbGen:
      return s << "bbGen";
    case LHCb::GenCountersFSR::B0Acc:
      return s << "B0Acc";
    case LHCb::GenCountersFSR::antiB0Acc:
      return s << "antiB0Acc";
    case LHCb::GenCountersFSR::BplusAcc:
      return s << "BplusAcc";
    case LHCb::GenCountersFSR::BminusAcc:
      return s << "BminusAcc";
    case LHCb::GenCountersFSR::Bs0Acc:
      return s << "Bs0Acc";
    case LHCb::GenCountersFSR::antiBs0Acc:
      return s << "antiBs0Acc";
    case LHCb::GenCountersFSR::bBaryonAcc:
      return s << "bBaryonAcc";
    case LHCb::GenCountersFSR::antibBaryonAcc:
      return s << "antibBaryonAcc";
    case LHCb::GenCountersFSR::BcplusAcc:
      return s << "BcplusAcc";
    case LHCb::GenCountersFSR::BcminusAcc:
      return s << "BcminusAcc";
    case LHCb::GenCountersFSR::bbAcc:
      return s << "bbAcc";
    case LHCb::GenCountersFSR::D0Gen:
      return s << "D0Gen";
    case LHCb::GenCountersFSR::antiD0Gen:
      return s << "antiD0Gen";
    case LHCb::GenCountersFSR::DplusGen:
      return s << "DplusGen";
    case LHCb::GenCountersFSR::DminusGen:
      return s << "DminusGen";
    case LHCb::GenCountersFSR::DsplusGen:
      return s << "DsplusGen";
    case LHCb::GenCountersFSR::DsminusGen:
      return s << "DsminusGen";
    case LHCb::GenCountersFSR::cBaryonGen:
      return s << "cBaryonGen";
    case LHCb::GenCountersFSR::anticBaryonGen:
      return s << "anticBaryonGen";
    case LHCb::GenCountersFSR::ccGen:
      return s << "ccGen";
    case LHCb::GenCountersFSR::D0Acc:
      return s << "D0Acc";
    case LHCb::GenCountersFSR::antiD0Acc:
      return s << "antiD0Acc";
    case LHCb::GenCountersFSR::DplusAcc:
      return s << "DplusAcc";
    case LHCb::GenCountersFSR::DminusAcc:
      return s << "DminusAcc";
    case LHCb::GenCountersFSR::DsplusAcc:
      return s << "DsplusAcc";
    case LHCb::GenCountersFSR::DsminusAcc:
      return s << "DsminusAcc";
    case LHCb::GenCountersFSR::cBaryonAcc:
      return s << "cBaryonAcc";
    case LHCb::GenCountersFSR::anticBaryonAcc:
      return s << "anticBaryonAcc";
    case LHCb::GenCountersFSR::ccAcc:
      return s << "ccAcc";
    case LHCb::GenCountersFSR::BGen:
      return s << "BGen";
    case LHCb::GenCountersFSR::BstarGen:
      return s << "BstarGen";
    case LHCb::GenCountersFSR::B2starGen:
      return s << "B2starGen";
    case LHCb::GenCountersFSR::BAcc:
      return s << "BAcc";
    case LHCb::GenCountersFSR::BstarAcc:
      return s << "BstarAcc";
    case LHCb::GenCountersFSR::B2starAcc:
      return s << "B2starAcc";
    case LHCb::GenCountersFSR::DGen:
      return s << "DGen";
    case LHCb::GenCountersFSR::DstarGen:
      return s << "DstarGen";
    case LHCb::GenCountersFSR::D2starGen:
      return s << "D2starGen";
    case LHCb::GenCountersFSR::DAcc:
      return s << "DAcc";
    case LHCb::GenCountersFSR::DstarAcc:
      return s << "DstarAcc";
    case LHCb::GenCountersFSR::D2starAcc:
      return s << "D2starAcc";
    case LHCb::GenCountersFSR::EvtSignal:
      return s << "EvtSignal";
    case LHCb::GenCountersFSR::EvtantiSignal:
      return s << "EvtantiSignal";
    case LHCb::GenCountersFSR::BeforePCut:
      return s << "BeforePCut";
    case LHCb::GenCountersFSR::AfterPCut:
      return s << "AfterPCut";
    case LHCb::GenCountersFSR::BeforeantiPCut:
      return s << "BeforeantiPCut";
    case LHCb::GenCountersFSR::AfterantiPCut:
      return s << "AfterantiPCut";
    case LHCb::GenCountersFSR::Unknown:
      return s << "Unknown";
    case LHCb::GenCountersFSR::AllsubProcess:
      return s << "AllsubProcess";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::GenCountersFSR::CounterKey";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::GenCountersFSR::clID() const { return LHCb::GenCountersFSR::classID(); }

inline const CLID& LHCb::GenCountersFSR::classID() { return CLID_GenCountersFSR; }

inline const GaudiUtils::VectorMap<std::string, LHCb::GenCountersFSR::CounterKey>&
LHCb::GenCountersFSR::s_CounterKeyTypMap() {
  static const GaudiUtils::VectorMap<std::string, CounterKey> m = {{"AllEvt", AllEvt},
                                                                   {"ZeroInt", ZeroInt},
                                                                   {"EvtGenerated", EvtGenerated},
                                                                   {"IntGenerated", IntGenerated},
                                                                   {"EvtAccepted", EvtAccepted},
                                                                   {"IntAccepted", IntAccepted},
                                                                   {"BeforeFullEvt", BeforeFullEvt},
                                                                   {"AfterFullEvt", AfterFullEvt},
                                                                   {"OnebGen", OnebGen},
                                                                   {"ThreebGen", ThreebGen},
                                                                   {"PromptBGen", PromptBGen},
                                                                   {"OnecGen", OnecGen},
                                                                   {"ThreecGen", ThreecGen},
                                                                   {"PromptCGen", PromptCGen},
                                                                   {"bAndcGen", bAndcGen},
                                                                   {"OnebAcc", OnebAcc},
                                                                   {"ThreebAcc", ThreebAcc},
                                                                   {"PromptBAcc", PromptBAcc},
                                                                   {"OnecAcc", OnecAcc},
                                                                   {"ThreecAcc", ThreecAcc},
                                                                   {"PromptCAcc", PromptCAcc},
                                                                   {"bAndcAcc", bAndcAcc},
                                                                   {"BeforeLevelCut", BeforeLevelCut},
                                                                   {"AfterLevelCut", AfterLevelCut},
                                                                   {"EvtInverted", EvtInverted},
                                                                   {"B0Gen", B0Gen},
                                                                   {"antiB0Gen", antiB0Gen},
                                                                   {"BplusGen", BplusGen},
                                                                   {"BminusGen", BminusGen},
                                                                   {"Bs0Gen", Bs0Gen},
                                                                   {"antiBs0Gen", antiBs0Gen},
                                                                   {"bBaryonGen", bBaryonGen},
                                                                   {"antibBaryonGen", antibBaryonGen},
                                                                   {"BcplusGen", BcplusGen},
                                                                   {"BcminusGen", BcminusGen},
                                                                   {"bbGen", bbGen},
                                                                   {"B0Acc", B0Acc},
                                                                   {"antiB0Acc", antiB0Acc},
                                                                   {"BplusAcc", BplusAcc},
                                                                   {"BminusAcc", BminusAcc},
                                                                   {"Bs0Acc", Bs0Acc},
                                                                   {"antiBs0Acc", antiBs0Acc},
                                                                   {"bBaryonAcc", bBaryonAcc},
                                                                   {"antibBaryonAcc", antibBaryonAcc},
                                                                   {"BcplusAcc", BcplusAcc},
                                                                   {"BcminusAcc", BcminusAcc},
                                                                   {"bbAcc", bbAcc},
                                                                   {"D0Gen", D0Gen},
                                                                   {"antiD0Gen", antiD0Gen},
                                                                   {"DplusGen", DplusGen},
                                                                   {"DminusGen", DminusGen},
                                                                   {"DsplusGen", DsplusGen},
                                                                   {"DsminusGen", DsminusGen},
                                                                   {"cBaryonGen", cBaryonGen},
                                                                   {"anticBaryonGen", anticBaryonGen},
                                                                   {"ccGen", ccGen},
                                                                   {"D0Acc", D0Acc},
                                                                   {"antiD0Acc", antiD0Acc},
                                                                   {"DplusAcc", DplusAcc},
                                                                   {"DminusAcc", DminusAcc},
                                                                   {"DsplusAcc", DsplusAcc},
                                                                   {"DsminusAcc", DsminusAcc},
                                                                   {"cBaryonAcc", cBaryonAcc},
                                                                   {"anticBaryonAcc", anticBaryonAcc},
                                                                   {"ccAcc", ccAcc},
                                                                   {"BGen", BGen},
                                                                   {"BstarGen", BstarGen},
                                                                   {"B2starGen", B2starGen},
                                                                   {"BAcc", BAcc},
                                                                   {"BstarAcc", BstarAcc},
                                                                   {"B2starAcc", B2starAcc},
                                                                   {"DGen", DGen},
                                                                   {"DstarGen", DstarGen},
                                                                   {"D2starGen", D2starGen},
                                                                   {"DAcc", DAcc},
                                                                   {"DstarAcc", DstarAcc},
                                                                   {"D2starAcc", D2starAcc},
                                                                   {"EvtSignal", EvtSignal},
                                                                   {"EvtantiSignal", EvtantiSignal},
                                                                   {"BeforePCut", BeforePCut},
                                                                   {"AfterPCut", AfterPCut},
                                                                   {"BeforeantiPCut", BeforeantiPCut},
                                                                   {"AfterantiPCut", AfterantiPCut},
                                                                   {"Unknown", Unknown},
                                                                   {"AllsubProcess", AllsubProcess}};
  return m;
}

inline LHCb::GenCountersFSR::CounterKey LHCb::GenCountersFSR::CounterKeyToType( const std::string& aName ) {
  auto iter = s_CounterKeyTypMap().find( aName );
  return iter != s_CounterKeyTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::GenCountersFSR::CounterKeyToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter      = std::find_if( s_CounterKeyTypMap().begin(), s_CounterKeyTypMap().end(),
                            [&]( const std::pair<const std::string, CounterKey>& i ) { return i.second == aEnum; } );
  return iter != s_CounterKeyTypMap().end() ? iter->first : s_Unknown;
}

inline std::map<std::string, std::string> LHCb::GenCountersFSR::getFullNames() {

  std::map<std::string, std::string> map;

  map.insert( std::pair<std::string, std::string>( "AllEvt", "all events (including empty events)" ) );
  map.insert( std::pair<std::string, std::string>( "ZeroInt", "events with 0 interactions" ) );

  map.insert( std::pair<std::string, std::string>( "EvtGenerated", "generated events" ) );
  map.insert( std::pair<std::string, std::string>( "IntGenerated", "generated interactions" ) );
  map.insert( std::pair<std::string, std::string>( "EvtAccepted", "accepted events" ) );
  map.insert( std::pair<std::string, std::string>( "IntAccepted", "interactions in accepted events" ) );

  map.insert( std::pair<std::string, std::string>( "BeforeFullEvt", "before full event generator level cut" ) );
  map.insert( std::pair<std::string, std::string>( "AfterFullEvt", "full event generator level cut" ) );

  map.insert( std::pair<std::string, std::string>( "OnebGen", "generated interactions with >= 1b" ) );
  map.insert( std::pair<std::string, std::string>( "ThreebGen", "generated interactions with >= 3b" ) );
  map.insert( std::pair<std::string, std::string>( "PromptBGen", "generated interactions with 1 prompt B" ) );
  map.insert( std::pair<std::string, std::string>( "OnecGen", "generated interactions with >= 1c" ) );
  map.insert( std::pair<std::string, std::string>( "ThreecGen", "generated interactions with >= 3c" ) );
  map.insert( std::pair<std::string, std::string>( "PromptCGen", "generated interactions with 1 prompt C" ) );
  map.insert( std::pair<std::string, std::string>( "bAndcGen", "generated interactions with b and c" ) );
  map.insert( std::pair<std::string, std::string>( "OnebAcc", "accepted interactions with >= 1b" ) );
  map.insert( std::pair<std::string, std::string>( "ThreebAcc", "accepted interactions with >= 3b" ) );
  map.insert( std::pair<std::string, std::string>( "PromptBAcc", "accepted interactions with 1 prompt B" ) );
  map.insert( std::pair<std::string, std::string>( "OnecAcc", "accepted  interactions with >= 1c" ) );
  map.insert( std::pair<std::string, std::string>( "ThreecAcc", "accepted interactions with >= 3c" ) );
  map.insert( std::pair<std::string, std::string>( "PromptCAcc", "accepted interactions with 1 prompt C" ) );
  map.insert( std::pair<std::string, std::string>( "bAndcAcc", "accepted interactions with b and c" ) );

  map.insert( std::pair<std::string, std::string>( "BeforeLevelCut", "generated inclusive events" ) );
  map.insert( std::pair<std::string, std::string>( "AfterLevelCut", "generator level cut" ) );
  map.insert( std::pair<std::string, std::string>( "EvtInverted", "z-inverted events" ) );

  map.insert( std::pair<std::string, std::string>( "B0Gen", "generated B0" ) );
  map.insert( std::pair<std::string, std::string>( "antiB0Gen", "generated anti-B0" ) );
  map.insert( std::pair<std::string, std::string>( "BplusGen", "generated B+" ) );
  map.insert( std::pair<std::string, std::string>( "BminusGen", "generated B-" ) );
  map.insert( std::pair<std::string, std::string>( "Bs0Gen", "generated Bs0" ) );
  map.insert( std::pair<std::string, std::string>( "antiBs0Gen", "generated anti-Bs0" ) );
  map.insert( std::pair<std::string, std::string>( "bBaryonGen", "generated b-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "antibBaryonGen", "generated anti-b-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "BcplusGen", "generated Bc+" ) );
  map.insert( std::pair<std::string, std::string>( "BcminusGen", "generated Bc-" ) );
  map.insert( std::pair<std::string, std::string>( "bbGen", "generated (bb)" ) );
  map.insert( std::pair<std::string, std::string>( "B0Acc", "accepted B0" ) );
  map.insert( std::pair<std::string, std::string>( "antiB0Acc", "accepted anti-B0" ) );
  map.insert( std::pair<std::string, std::string>( "BplusAcc", "accepted B+" ) );
  map.insert( std::pair<std::string, std::string>( "BminusAcc", "accepted B-" ) );
  map.insert( std::pair<std::string, std::string>( "Bs0Acc", "accepted Bs0" ) );
  map.insert( std::pair<std::string, std::string>( "antiBs0Acc", "accepted anti-Bs0s" ) );
  map.insert( std::pair<std::string, std::string>( "bBaryonAcc", "accepted b-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "antibBaryonAcc", "accepted anti-b-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "BcplusAcc", "accepted Bc+s" ) );
  map.insert( std::pair<std::string, std::string>( "BcminusAcc", "accepted Bc-" ) );
  map.insert( std::pair<std::string, std::string>( "bbAcc", "accepted (bb)" ) );

  map.insert( std::pair<std::string, std::string>( "D0Gen", "generated D0" ) );
  map.insert( std::pair<std::string, std::string>( "antiD0Gen", "generated anti-D0" ) );
  map.insert( std::pair<std::string, std::string>( "DplusGen", "generated D+" ) );
  map.insert( std::pair<std::string, std::string>( "DminusGen", "generated D-" ) );
  map.insert( std::pair<std::string, std::string>( "DsplusGen", "generated Ds+" ) );
  map.insert( std::pair<std::string, std::string>( "Dsminus0Gen", "generated Ds-" ) );
  map.insert( std::pair<std::string, std::string>( "cBaryonGen", "generated c-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "anticBaryonGen", "generated anti-c-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "ccGen", "generated (cc)" ) );
  map.insert( std::pair<std::string, std::string>( "D0Acc", "accepted D0" ) );
  map.insert( std::pair<std::string, std::string>( "antiD0Acc", "accepted anti-D0" ) );
  map.insert( std::pair<std::string, std::string>( "DplusAcc", "accepted D+" ) );
  map.insert( std::pair<std::string, std::string>( "DminusAcc", "acceptedD-" ) );
  map.insert( std::pair<std::string, std::string>( "DsplusAcc", "accepted Ds+" ) );
  map.insert( std::pair<std::string, std::string>( "Dsminus0Acc", "accepted Ds-" ) );
  map.insert( std::pair<std::string, std::string>( "cBaryonAcc", "accepted c-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "anticBaryonAcc", "accepted anti-c-Baryon" ) );
  map.insert( std::pair<std::string, std::string>( "ccAcc", "accepted (cc)" ) );

  map.insert( std::pair<std::string, std::string>( "BGen", "generated B(L=0, J=0)" ) );
  map.insert( std::pair<std::string, std::string>( "BstarGen", "generated B* (L=0, J=1)" ) );
  map.insert( std::pair<std::string, std::string>( "B2starGen", "generated B** (L=1, J=0,1,2)" ) );
  map.insert( std::pair<std::string, std::string>( "BAcc", "accepted B(L=0, J=0)" ) );
  map.insert( std::pair<std::string, std::string>( "BstarAcc", "accepted B* (L=0, J=1)" ) );
  map.insert( std::pair<std::string, std::string>( "B2starAcc", "accepted B** (L=1, J=0,1,2)" ) );

  map.insert( std::pair<std::string, std::string>( "DGen", "generated D(L=0, J=0)" ) );
  map.insert( std::pair<std::string, std::string>( "DstarGen", "generated D* (L=0, J=1)" ) );
  map.insert( std::pair<std::string, std::string>( "D2starGen", "generated D** (L=1, J=0,1,2)" ) );
  map.insert( std::pair<std::string, std::string>( "DAcc", "accepted D(L=0, J=0)" ) );
  map.insert( std::pair<std::string, std::string>( "DstarAcc", "accepted D* (L=0, J=1)" ) );
  map.insert( std::pair<std::string, std::string>( "D2starAcc", "accepted D** (L=1, J=0,1,2)" ) );

  map.insert( std::pair<std::string, std::string>( "EvtSignal", "generated signal events" ) );
  map.insert( std::pair<std::string, std::string>( "EvtantiSignal", "generated anti-signal events" ) );

  map.insert( std::pair<std::string, std::string>( "BeforePCut", "generated signal events" ) );
  map.insert( std::pair<std::string, std::string>( "AfterPCut", "generator particle level cut" ) );
  map.insert( std::pair<std::string, std::string>( "BeforeantiPCut", "generated anti-signal events" ) );
  map.insert( std::pair<std::string, std::string>( "AfterantiPCut", "generator anti-particle level cut" ) );

  map.insert( std::pair<std::string, std::string>( "AllsubProcess", "Total cross-section" ) );

  return map;
}

inline std::ostream& LHCb::GenCountersFSR::fillStream( std::ostream& s ) const {

  std::ostream& ss = const_cast<std::ostream&>( s );
  ss << "{ }";

  return ss;
}
