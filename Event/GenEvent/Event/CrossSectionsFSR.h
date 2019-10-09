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
  static const CLID CLID_CrossSectionsFSR = 13602;

  /** @class CrossSectionsFSR CrossSectionsFSR.h
   *
   * Enum class for Generator fractions and cross-sections
   *
   * @author Davide Fazzini
   *
   */

  class CrossSectionsFSR final : public DataObject {
  public:
    /// lookup table for fraction and cross-section keys
    enum CrossSectionKey {
      MBCrossSection      = 0,  // Total cross-section of Minimum Bias events
      MeanNoZeroPUInt     = 2,  // Mean number of non-empty generated Pile-Up interactions
      MeanPUInt           = 3,  // Mean number of generated Pile-Up interactions
      MeanPUIntAcc        = 5,  // Mean number of accepted Pile-Up interactions
      FullEvtCut          = 7,  // Full event cut efficiency
      bCrossGen           = 10, // Generated b cross-section
      ThreebCrossGen      = 11, // Double generated b cross-section
      PromptBCrossGen     = 12, // Generated prompt B cross-section
      cCrossGen           = 13, // Generated c cross-section
      ThreecCrossGen      = 14, // Double generated c cross-section
      PromptCCrossGen     = 15, // Generated prompt C cross-section
      bAndcCrossGen       = 16, // Generated b and c cross-section
      bCrossAcc           = 17, // Accepted b cross-section
      ThreebCrossAcc      = 18, // Double accepted b cross-section
      PromptBCrossAcc     = 19, // Accepted prompt B cross-section
      cCrossAcc           = 20, // Accepted c cross-section
      ThreecCrossAcc      = 21, // Double accepted c cross-section
      PromptCCrossAcc     = 22, // Accepted prompt C cross-section
      bAndcCrossAcc       = 23, // Accepted b and c cross-section
      GenLevelCut         = 27, // Generator level cut efficiency
      FractionB0Gen       = 30, // Fraction of generated B0
      FractionantiB0Gen   = 31, // Fraction of generated B0bar
      FractionBplusGen    = 32, // Fraction of generated B+
      FractionBminusGen   = 33, // Fraction of generated B-
      FractionBs0Gen      = 34, // Fraction of generated Bs0
      FractionantiBs0Gen  = 35, // Fraction of generated anti-Bs0
      FractionbBarGen     = 36, // Fraction of generated b-Baryon
      FractionantibBarGen = 37, // Fraction of generated anti-b-Baryon
      FractionBcplusGen   = 38, // Fraction of generated Bc+
      FractionBcminusGen  = 39, // Fraction of generated Bc-
      FractionB0Acc       = 41, // Fraction of accepted B0
      FractionantiB0Acc   = 42, // Fraction of accepted B0bar
      FractionBplusAcc    = 43, // Fraction of accepted B+
      FractionBminusAcc   = 44, // Fraction of accepted B-
      FractionBs0Acc      = 45, // Fraction of accepted Bs0
      FractionantiBs0Acc  = 46, // Fraction of accepted anti-Bs0
      FractionbBaryonAcc  = 47, // Fraction of accepted b-Baryon
      FractionantibBarAcc = 48, // Fraction of accepted anti-b-Baryon
      FractionBcplusAcc   = 49, // Fraction of accepted Bc+
      FractionBcminusAcc  = 50, // Fraction of accepted Bc-
      FractionD0Gen       = 55, // Fraction of generated D0
      FractionantiD0Gen   = 56, // Fraction of generated D0bar
      FractionDplusGen    = 57, // Fraction of generated D+
      FractionDminusGen   = 58, // Fraction of generated D-
      FractionDsplusGen   = 59, // Fraction of generated Ds+
      FractionDsminusGen  = 60, // Fraction of generated Ds-
      FractioncBarGen     = 61, // Fraction of generated c-Baryon
      FractionanticBarGen = 62, // Fraction of generated anti-c-Baryon
      FractionD0Acc       = 64, // Fraction of accepted D0
      FractionantiD0Acc   = 65, // Fraction of accepted D0bar
      FractionDplusAcc    = 66, // Fraction of accepted D+
      FractionDminusAcc   = 67, // Fraction of accepted D-
      FractionDsplusAcc   = 68, // Fraction of accepted Ds+
      FractionDsminusAcc  = 69, // Fraction of accepted Ds-
      FractioncBaryonAcc  = 70, // Fraction of accepted c-Baryon
      FractionanticBarAcc = 71, // Fraction of accepted anti-c-Baryon
      FractionBGen        = 75, // Fraction of generated B
      FractionBstarGen    = 76, // Fraction of generated B*
      FractionB2starGen   = 77, // Fraction of generated B**
      FractionBAcc        = 78, // Fraction of accepted B
      FractionBstarAcc    = 79, // Fraction of accepted B*
      FractionB2starAcc   = 80, // Fraction of accepted B**
      FractionDGen        = 85, // Fraction of generated D
      FractionDstarGen    = 86, // Fraction of generated D*
      FractionD2starGen   = 87, // Fraction of generated D**
      FractionDAcc        = 88, // Fraction of accepted D
      FractionDstarAcc    = 89, // Fraction of accepted D*
      FractionD2starAcc   = 90, // Fraction of accepted D**
      FractionSignal      = 91, // Fraction of generated signal events
      FractionantiSignal  = 92, // Fraction of generated anti-signal events
      PartCutEff          = 94, // Particle cut efficiency
      antiPartCutEff      = 96, // Anti-particle cut efficiency
      Unknown             = 98  // Unknown value
    };

    /// Default Constructor
    CrossSectionsFSR() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type CrossSectionKey
    static LHCb::CrossSectionsFSR::CrossSectionKey CrossSectionKeyToType( const std::string& aName );

    /// conversion to string for enum type CrossSectionKey
    static const std::string& CrossSectionKeyToString( int aEnum );

    /// return the list with the complete names of cross-sections
    static const std::map<std::string, std::string>& getFullNames();

    /// printout
    std::ostream& fillStream( std::ostream& s ) const override;

    friend std::ostream& operator<<( std::ostream& str, const CrossSectionsFSR& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    static const GaudiUtils::VectorMap<std::string, CrossSectionKey>& s_CrossSectionKeyTypMap();

  }; // class CrossSectionsFSR

  inline std::ostream& operator<<( std::ostream& s, LHCb::CrossSectionsFSR::CrossSectionKey e ) {
    switch ( e ) {
    case LHCb::CrossSectionsFSR::MBCrossSection:
      return s << "MBCrossSection";
    case LHCb::CrossSectionsFSR::MeanNoZeroPUInt:
      return s << "MeanNoZeroPUInt";
    case LHCb::CrossSectionsFSR::MeanPUInt:
      return s << "MeanPUInt";
    case LHCb::CrossSectionsFSR::MeanPUIntAcc:
      return s << "MeanPUIntAcc";
    case LHCb::CrossSectionsFSR::FullEvtCut:
      return s << "FullEvtCut";
    case LHCb::CrossSectionsFSR::bCrossGen:
      return s << "bCrossGen";
    case LHCb::CrossSectionsFSR::ThreebCrossGen:
      return s << "ThreebCrossGen";
    case LHCb::CrossSectionsFSR::PromptBCrossGen:
      return s << "PromptBCrossGen";
    case LHCb::CrossSectionsFSR::cCrossGen:
      return s << "cCrossGen";
    case LHCb::CrossSectionsFSR::ThreecCrossGen:
      return s << "ThreecCrossGen";
    case LHCb::CrossSectionsFSR::PromptCCrossGen:
      return s << "PromptCCrossGen";
    case LHCb::CrossSectionsFSR::bAndcCrossGen:
      return s << "bAndcCrossGen";
    case LHCb::CrossSectionsFSR::bCrossAcc:
      return s << "bCrossAcc";
    case LHCb::CrossSectionsFSR::ThreebCrossAcc:
      return s << "ThreebCrossAcc";
    case LHCb::CrossSectionsFSR::PromptBCrossAcc:
      return s << "PromptBCrossAcc";
    case LHCb::CrossSectionsFSR::cCrossAcc:
      return s << "cCrossAcc";
    case LHCb::CrossSectionsFSR::ThreecCrossAcc:
      return s << "ThreecCrossAcc";
    case LHCb::CrossSectionsFSR::PromptCCrossAcc:
      return s << "PromptCCrossAcc";
    case LHCb::CrossSectionsFSR::bAndcCrossAcc:
      return s << "bAndcCrossAcc";
    case LHCb::CrossSectionsFSR::GenLevelCut:
      return s << "GenLevelCut";
    case LHCb::CrossSectionsFSR::FractionB0Gen:
      return s << "FractionB0Gen";
    case LHCb::CrossSectionsFSR::FractionantiB0Gen:
      return s << "FractionantiB0Gen";
    case LHCb::CrossSectionsFSR::FractionBplusGen:
      return s << "FractionBplusGen";
    case LHCb::CrossSectionsFSR::FractionBminusGen:
      return s << "FractionBminusGen";
    case LHCb::CrossSectionsFSR::FractionBs0Gen:
      return s << "FractionBs0Gen";
    case LHCb::CrossSectionsFSR::FractionantiBs0Gen:
      return s << "FractionantiBs0Gen";
    case LHCb::CrossSectionsFSR::FractionbBarGen:
      return s << "FractionbBarGen";
    case LHCb::CrossSectionsFSR::FractionantibBarGen:
      return s << "FractionantibBarGen";
    case LHCb::CrossSectionsFSR::FractionBcplusGen:
      return s << "FractionBcplusGen";
    case LHCb::CrossSectionsFSR::FractionBcminusGen:
      return s << "FractionBcminusGen";
    case LHCb::CrossSectionsFSR::FractionB0Acc:
      return s << "FractionB0Acc";
    case LHCb::CrossSectionsFSR::FractionantiB0Acc:
      return s << "FractionantiB0Acc";
    case LHCb::CrossSectionsFSR::FractionBplusAcc:
      return s << "FractionBplusAcc";
    case LHCb::CrossSectionsFSR::FractionBminusAcc:
      return s << "FractionBminusAcc";
    case LHCb::CrossSectionsFSR::FractionBs0Acc:
      return s << "FractionBs0Acc";
    case LHCb::CrossSectionsFSR::FractionantiBs0Acc:
      return s << "FractionantiBs0Acc";
    case LHCb::CrossSectionsFSR::FractionbBaryonAcc:
      return s << "FractionbBaryonAcc";
    case LHCb::CrossSectionsFSR::FractionantibBarAcc:
      return s << "FractionantibBarAcc";
    case LHCb::CrossSectionsFSR::FractionBcplusAcc:
      return s << "FractionBcplusAcc";
    case LHCb::CrossSectionsFSR::FractionBcminusAcc:
      return s << "FractionBcminusAcc";
    case LHCb::CrossSectionsFSR::FractionD0Gen:
      return s << "FractionD0Gen";
    case LHCb::CrossSectionsFSR::FractionantiD0Gen:
      return s << "FractionantiD0Gen";
    case LHCb::CrossSectionsFSR::FractionDplusGen:
      return s << "FractionDplusGen";
    case LHCb::CrossSectionsFSR::FractionDminusGen:
      return s << "FractionDminusGen";
    case LHCb::CrossSectionsFSR::FractionDsplusGen:
      return s << "FractionDsplusGen";
    case LHCb::CrossSectionsFSR::FractionDsminusGen:
      return s << "FractionDsminusGen";
    case LHCb::CrossSectionsFSR::FractioncBarGen:
      return s << "FractioncBarGen";
    case LHCb::CrossSectionsFSR::FractionanticBarGen:
      return s << "FractionanticBarGen";
    case LHCb::CrossSectionsFSR::FractionD0Acc:
      return s << "FractionD0Acc";
    case LHCb::CrossSectionsFSR::FractionantiD0Acc:
      return s << "FractionantiD0Acc";
    case LHCb::CrossSectionsFSR::FractionDplusAcc:
      return s << "FractionDplusAcc";
    case LHCb::CrossSectionsFSR::FractionDminusAcc:
      return s << "FractionDminusAcc";
    case LHCb::CrossSectionsFSR::FractionDsplusAcc:
      return s << "FractionDsplusAcc";
    case LHCb::CrossSectionsFSR::FractionDsminusAcc:
      return s << "FractionDsminusAcc";
    case LHCb::CrossSectionsFSR::FractioncBaryonAcc:
      return s << "FractioncBaryonAcc";
    case LHCb::CrossSectionsFSR::FractionanticBarAcc:
      return s << "FractionanticBarAcc";
    case LHCb::CrossSectionsFSR::FractionBGen:
      return s << "FractionBGen";
    case LHCb::CrossSectionsFSR::FractionBstarGen:
      return s << "FractionBstarGen";
    case LHCb::CrossSectionsFSR::FractionB2starGen:
      return s << "FractionB2starGen";
    case LHCb::CrossSectionsFSR::FractionBAcc:
      return s << "FractionBAcc";
    case LHCb::CrossSectionsFSR::FractionBstarAcc:
      return s << "FractionBstarAcc";
    case LHCb::CrossSectionsFSR::FractionB2starAcc:
      return s << "FractionB2starAcc";
    case LHCb::CrossSectionsFSR::FractionDGen:
      return s << "FractionDGen";
    case LHCb::CrossSectionsFSR::FractionDstarGen:
      return s << "FractionDstarGen";
    case LHCb::CrossSectionsFSR::FractionD2starGen:
      return s << "FractionD2starGen";
    case LHCb::CrossSectionsFSR::FractionDAcc:
      return s << "FractionDAcc";
    case LHCb::CrossSectionsFSR::FractionDstarAcc:
      return s << "FractionDstarAcc";
    case LHCb::CrossSectionsFSR::FractionD2starAcc:
      return s << "FractionD2starAcc";
    case LHCb::CrossSectionsFSR::FractionSignal:
      return s << "FractionSignal";
    case LHCb::CrossSectionsFSR::FractionantiSignal:
      return s << "FractionantiSignal";
    case LHCb::CrossSectionsFSR::PartCutEff:
      return s << "PartCutEff";
    case LHCb::CrossSectionsFSR::antiPartCutEff:
      return s << "antiPartCutEff";
    case LHCb::CrossSectionsFSR::Unknown:
      return s << "Unknown";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::CrossSectionsFSR::CrossSectionKey";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::CrossSectionsFSR::clID() const { return LHCb::CrossSectionsFSR::classID(); }

inline const CLID& LHCb::CrossSectionsFSR::classID() { return CLID_CrossSectionsFSR; }

inline const GaudiUtils::VectorMap<std::string, LHCb::CrossSectionsFSR::CrossSectionKey>&
LHCb::CrossSectionsFSR::s_CrossSectionKeyTypMap() {
  static const GaudiUtils::VectorMap<std::string, CrossSectionKey> m = {{"MBCrossSection", MBCrossSection},
                                                                        {"MeanNoZeroPUInt", MeanNoZeroPUInt},
                                                                        {"MeanPUInt", MeanPUInt},
                                                                        {"MeanPUIntAcc", MeanPUIntAcc},
                                                                        {"FullEvtCut", FullEvtCut},
                                                                        {"bCrossGen", bCrossGen},
                                                                        {"ThreebCrossGen", ThreebCrossGen},
                                                                        {"PromptBCrossGen", PromptBCrossGen},
                                                                        {"cCrossGen", cCrossGen},
                                                                        {"ThreecCrossGen", ThreecCrossGen},
                                                                        {"PromptCCrossGen", PromptCCrossGen},
                                                                        {"bAndcCrossGen", bAndcCrossGen},
                                                                        {"bCrossAcc", bCrossAcc},
                                                                        {"ThreebCrossAcc", ThreebCrossAcc},
                                                                        {"PromptBCrossAcc", PromptBCrossAcc},
                                                                        {"cCrossAcc", cCrossAcc},
                                                                        {"ThreecCrossAcc", ThreecCrossAcc},
                                                                        {"PromptCCrossAcc", PromptCCrossAcc},
                                                                        {"bAndcCrossAcc", bAndcCrossAcc},
                                                                        {"GenLevelCut", GenLevelCut},
                                                                        {"FractionB0Gen", FractionB0Gen},
                                                                        {"FractionantiB0Gen", FractionantiB0Gen},
                                                                        {"FractionBplusGen", FractionBplusGen},
                                                                        {"FractionBminusGen", FractionBminusGen},
                                                                        {"FractionBs0Gen", FractionBs0Gen},
                                                                        {"FractionantiBs0Gen", FractionantiBs0Gen},
                                                                        {"FractionbBarGen", FractionbBarGen},
                                                                        {"FractionantibBarGen", FractionantibBarGen},
                                                                        {"FractionBcplusGen", FractionBcplusGen},
                                                                        {"FractionBcminusGen", FractionBcminusGen},
                                                                        {"FractionB0Acc", FractionB0Acc},
                                                                        {"FractionantiB0Acc", FractionantiB0Acc},
                                                                        {"FractionBplusAcc", FractionBplusAcc},
                                                                        {"FractionBminusAcc", FractionBminusAcc},
                                                                        {"FractionBs0Acc", FractionBs0Acc},
                                                                        {"FractionantiBs0Acc", FractionantiBs0Acc},
                                                                        {"FractionbBaryonAcc", FractionbBaryonAcc},
                                                                        {"FractionantibBarAcc", FractionantibBarAcc},
                                                                        {"FractionBcplusAcc", FractionBcplusAcc},
                                                                        {"FractionBcminusAcc", FractionBcminusAcc},
                                                                        {"FractionD0Gen", FractionD0Gen},
                                                                        {"FractionantiD0Gen", FractionantiD0Gen},
                                                                        {"FractionDplusGen", FractionDplusGen},
                                                                        {"FractionDminusGen", FractionDminusGen},
                                                                        {"FractionDsplusGen", FractionDsplusGen},
                                                                        {"FractionDsminusGen", FractionDsminusGen},
                                                                        {"FractioncBarGen", FractioncBarGen},
                                                                        {"FractionanticBarGen", FractionanticBarGen},
                                                                        {"FractionD0Acc", FractionD0Acc},
                                                                        {"FractionantiD0Acc", FractionantiD0Acc},
                                                                        {"FractionDplusAcc", FractionDplusAcc},
                                                                        {"FractionDminusAcc", FractionDminusAcc},
                                                                        {"FractionDsplusAcc", FractionDsplusAcc},
                                                                        {"FractionDsminusAcc", FractionDsminusAcc},
                                                                        {"FractioncBaryonAcc", FractioncBaryonAcc},
                                                                        {"FractionanticBarAcc", FractionanticBarAcc},
                                                                        {"FractionBGen", FractionBGen},
                                                                        {"FractionBstarGen", FractionBstarGen},
                                                                        {"FractionB2starGen", FractionB2starGen},
                                                                        {"FractionBAcc", FractionBAcc},
                                                                        {"FractionBstarAcc", FractionBstarAcc},
                                                                        {"FractionB2starAcc", FractionB2starAcc},
                                                                        {"FractionDGen", FractionDGen},
                                                                        {"FractionDstarGen", FractionDstarGen},
                                                                        {"FractionD2starGen", FractionD2starGen},
                                                                        {"FractionDAcc", FractionDAcc},
                                                                        {"FractionDstarAcc", FractionDstarAcc},
                                                                        {"FractionD2starAcc", FractionD2starAcc},
                                                                        {"FractionSignal", FractionSignal},
                                                                        {"FractionantiSignal", FractionantiSignal},
                                                                        {"PartCutEff", PartCutEff},
                                                                        {"antiPartCutEff", antiPartCutEff},
                                                                        {"Unknown", Unknown}};
  return m;
}

inline LHCb::CrossSectionsFSR::CrossSectionKey
LHCb::CrossSectionsFSR::CrossSectionKeyToType( const std::string& aName ) {
  auto iter = s_CrossSectionKeyTypMap().find( aName );
  return iter != s_CrossSectionKeyTypMap().end() ? iter->second : Unknown;
}

inline const std::string& LHCb::CrossSectionsFSR::CrossSectionKeyToString( int aEnum ) {
  static const std::string s_Unknown = "Unknown";
  auto                     iter =
      std::find_if( s_CrossSectionKeyTypMap().begin(), s_CrossSectionKeyTypMap().end(),
                    [&]( const std::pair<const std::string, CrossSectionKey>& i ) { return i.second == aEnum; } );
  return iter != s_CrossSectionKeyTypMap().end() ? iter->first : s_Unknown;
}

inline const std::map<std::string, std::string>& LHCb::CrossSectionsFSR::getFullNames() {

  static const std::map<std::string, std::string> map = {

      {"MBCrossSection", "Total cross-section of Minimum Bias events"},

      {"MeanNoZeroPUInt", "Mean number of non-empty generated Pile-Up interactions"},
      {"MeanPUInt", "Mean number of generated Pile-Up interactions"},
      {"MeanPUIntAcc", "Mean number of accepted Pile-Up interactions"},

      {"FullEvtCut", "Full event cut efficiency"},

      {"bCrossGen", "Generated b cross-section"},
      {"ThreebCrossGen", "Double generated b cross-section"},
      {"PromptBCrossGen", "Generated prompt B cross-section"},
      {"cCrossGen", "Generated c cross-section"},
      {"ThreecCrossGen", "Double generated c cross-section"},
      {"PromptCCrossGen", "Generated prompt C cross-section"},
      {"bAndcCrossGen", "Generated b and c cross-section"},
      {"bCrossAcc", "Accepted b cross-section"},
      {"ThreebCrossAcc", "Double accepted b cross-section"},
      {"PromptBCrossAcc", "Accepted prompt B cross-section"},
      {"cCrossAcc", "Accepted c cross-section"},
      {"ThreecCrossAcc", "Double accepted c cross-section"},
      {"PromptCCrossAcc", "Accepted prompt C cross-section"},
      {"bAndcCrossAcc", "Accepted b and c cross-section"},

      {"GenLevelCut", "Generator level cut efficiency"},

      {"FractionB0Gen", "Fraction of generated B0"},
      {"FractionantiB0Gen", "Fraction of generated B0bar"},
      {"FractionBplusGen", "Fraction of generated B+"},
      {"FractionBminusGen", "Fraction of generated B-"},
      {"FractionBs0Gen", "Fraction of generated Bs0"},
      {"FractionantiBs0Gen", "Fraction of generated anti-Bs0"},
      {"FractionbBarGen", "Fraction of generated b-Baryon"},
      {"FractionantibBarGen", "Fraction of generated anti-b-Baryon"},
      {"FractionBcplusGen", "Fraction of generated Bc+"},
      {"FractionBcminusGen", "Fraction of generated Bc-"},
      {"FractionB0Acc", "Fraction of accepted B0"},
      {"FractionantiB0Acc", "Fraction of accepted B0bar"},
      {"FractionBplusAcc", "Fraction of accepted B+"},
      {"FractionBminusAcc", "Fraction of accepted B-"},
      {"FractionBs0Acc", "Fraction of accepted Bs0"},
      {"FractionantiBs0Acc", "Fraction of accepted anti-Bs0"},
      {"FractionbBaryonAcc", "Fraction of accepted b-Baryon"},
      {"FractionantibBarAcc", "Fraction of accepted anti-b-Baryon"},
      {"FractionBcplusAcc", "Fraction of accepted Bc+"},
      {"FractionBcminusAcc", "Fraction of accepted Bc-"},

      {"FractionD0Gen", "Fraction of generated D0"},
      {"FractionantiD0Gen", "Fraction of generated D0bar"},
      {"FractionDplusGen", "Fraction of generated D+"},
      {"FractionDminusGen", "Fraction of generated D-"},
      {"FractionDsplusGen", "Fraction of generated Ds+"},
      {"FractionDsminusGen", "Fraction of generated Ds-"},
      {"FractioncBarGen", "Fraction of generated c-Baryon"},
      {"FractionanticBarGen", "Fraction of generated anti-c-Baryon"},
      {"FractionD0Acc", "Fraction of accepted D0"},
      {"FractionantiD0Acc", "Fraction of accepted D0bar"},
      {"FractionDplusAcc", "Fraction of accepted D+"},
      {"FractionDminusAcc", "Fraction of accepted D-"},
      {"FractionDsplusAcc", "Fraction of accepted Ds+"},
      {"FractionDsminusAcc", "Fraction of accepted Ds-"},
      {"FractioncBaryonAcc", "Fraction of accepted c-Baryon"},
      {"FractionanticBarAcc", "Fraction of accepted anti-c-Baryon"},

      {"FractionBGen", "Fraction of generated B"},
      {"FractionBstarGen", "Fraction of generated B*"},
      {"FractionB2starGen", "Fraction of generated B**"},
      {"FractionBAcc", "Fraction of accepted B"},
      {"FractionBstarAcc", "Fraction of accepted B*"},
      {"FractionB2starAcc", "Fraction of accepted B**"},

      {"FractionDGen", "Fraction of generated D"},
      {"FractionDstarGen", "Fraction of generated D*"},
      {"FractionD2starGen", "Fraction of generated D**"},
      {"FractionDAcc", "Fraction of accepted D"},
      {"FractionDstarAcc", "Fraction of accepted D*"},
      {"FractionD2starAcc", "Fraction of accepted D**"},

      {"FractionSignal", "Fraction of generated signal events"},
      {"FractionantiSignal", "Fraction of generated anti-signal events"},
      {"PartCutEff", "Particle cut efficiency"},
      {"antiPartCutEff", "Anti-particle cut efficiency"}};

  return map;
}

inline std::ostream& LHCb::CrossSectionsFSR::fillStream( std::ostream& s ) const { return s << "{ }"; }
