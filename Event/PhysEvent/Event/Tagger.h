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
#include "Event/Particle.h"
#include "GaudiKernel/SmartRefVector.h"
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class Tagger Tagger.h
   *
   * Individual tagger decision on B flavour
   *
   * @author Marco Musy, Julian Wishahi
   *
   */

  class Tagger {
  public:
    /// The type of tagger which has produced this decision
    enum TaggerType {
      none                 = 0,
      unknown              = 1,
      OS_Muon              = 2,
      OS_Electron          = 3,
      OS_Kaon              = 4,
      SS_Kaon              = 5,
      SS_Pion              = 6,
      jetCharge            = 7,
      OS_jetCharge         = 8,
      SS_jetCharge         = 9,
      VtxCharge            = 10,
      Topology             = 11,
      OS_nnetKaon          = 12,
      SS_nnetKaon          = 13,
      SS_Proton            = 14,
      OS_Charm             = 15,
      SS_PionBDT           = 16,
      OSMuon               = 10100,
      OSMuonLatest         = 10101,
      OSMuonLatestB2OC     = 10102,
      OSMuonLatestB2CC     = 10103,
      OSMuonDev            = 10199,
      OSElectron           = 10200,
      OSElectronLatest     = 10201,
      OSElectronLatestB2OC = 10202,
      OSElectronLatestB2CC = 10203,
      OSElectronDev        = 10299,
      OSKaon               = 10300,
      OSKaonLatest         = 10301,
      OSKaonLatestB2OC     = 10302,
      OSKaonLatestB2CC     = 10303,
      OSKaonDev            = 10399,
      OSVtxCh              = 10400,
      OSVtxChLatest        = 10401,
      OSVtxChDev           = 10499,
      OSCharm              = 10500,
      OSCharmLatest        = 10501,
      OSCharmDev           = 10599,
      OSProton             = 10600,
      OSProtonLatest       = 10601,
      OSProtonDev          = 10699,
      SSKaon               = 20100,
      SSKaonLatest         = 20101,
      SSKaonDev            = 20199,
      SSPion               = 20200,
      SSPionLatest         = 20201,
      SSPionDev            = 20299,
      SSProton             = 20300,
      SSProtonLatest       = 20301,
      SSProtonDev          = 20399,
      SSLambda             = 20400,
      SSLambdaLatest       = 20401,
      SSLambdaDev          = 20499
    };

    /// Copy Constructor
    Tagger( const Tagger& c )
        : m_type( c.type() )
        , m_decision( c.decision() )
        , m_omega( c.omega() )
        , m_mvaValue( c.mvaValue() )
        , m_charge( c.charge() )
        , m_taggerParts( c.taggerParts() ) {}

    /// Default Constructor
    Tagger() : m_type( 0 ), m_decision( 0 ), m_omega( 0.50 ), m_mvaValue( 0.0 ), m_charge( 0.0 ) {}

    /// Default Destructor
    virtual ~Tagger() {}

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// conversion of string to enum for type TaggerType
    static LHCb::Tagger::TaggerType TaggerTypeToType( const std::string& aName );

    /// conversion to string for enum type TaggerType
    static const std::string& TaggerTypeToString( int aEnum );

    /// Retrieve const  The type of tagger
    unsigned short int type() const;

    /// Update  The type of tagger
    void setType( unsigned short int value );

    /// Retrieve const  decision of tagger
    short int decision() const;

    /// Update  decision of tagger
    void setDecision( short int value );

    /// Retrieve const  wrong tag fraction of tagger from MC tuning
    float omega() const;

    /// Update  wrong tag fraction of tagger from MC tuning
    void setOmega( float value );

    /// Retrieve const  raw output of the mva
    float mvaValue() const;

    /// Update  raw output of the mva
    void setMvaValue( float value );

    /// Retrieve const  (weighted) charge of the tagging particle
    float charge() const;

    /// Update  (weighted) charge of the tagging particle
    void setCharge( float value );

    /// Retrieve (const)  Vector of Particle(s) used to build the Tagger
    const SmartRefVector<LHCb::Particle>& taggerParts() const;

    /// Update  Vector of Particle(s) used to build the Tagger
    void setTaggerParts( const SmartRefVector<LHCb::Particle>& value );

    /// Add to  Vector of Particle(s) used to build the Tagger
    void addToTaggerParts( const SmartRef<LHCb::Particle>& value );

    /// Att to (pointer)  Vector of Particle(s) used to build the Tagger
    void addToTaggerParts( const LHCb::Particle* value );

    /// Remove from  Vector of Particle(s) used to build the Tagger
    void removeFromTaggerParts( const SmartRef<LHCb::Particle>& value );

    /// Clear  Vector of Particle(s) used to build the Tagger
    void clearTaggerParts();

    friend std::ostream& operator<<( std::ostream& str, const Tagger& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned short int             m_type;        ///< The type of tagger
    short int                      m_decision;    ///< decision of tagger
    float                          m_omega;       ///< wrong tag fraction of tagger from MC tuning
    float                          m_mvaValue;    ///< raw output of the mva
    float                          m_charge;      ///< (weighted) charge of the tagging particle
    SmartRefVector<LHCb::Particle> m_taggerParts; ///< Vector of Particle(s) used to build the Tagger

    static const GaudiUtils::VectorMap<std::string, TaggerType>& s_TaggerTypeTypMap();

  }; // class Tagger

  inline std::ostream& operator<<( std::ostream& s, LHCb::Tagger::TaggerType e ) {
    switch ( e ) {
    case LHCb::Tagger::none:
      return s << "none";
    case LHCb::Tagger::unknown:
      return s << "unknown";
    case LHCb::Tagger::OS_Muon:
      return s << "OS_Muon";
    case LHCb::Tagger::OS_Electron:
      return s << "OS_Electron";
    case LHCb::Tagger::OS_Kaon:
      return s << "OS_Kaon";
    case LHCb::Tagger::SS_Kaon:
      return s << "SS_Kaon";
    case LHCb::Tagger::SS_Pion:
      return s << "SS_Pion";
    case LHCb::Tagger::jetCharge:
      return s << "jetCharge";
    case LHCb::Tagger::OS_jetCharge:
      return s << "OS_jetCharge";
    case LHCb::Tagger::SS_jetCharge:
      return s << "SS_jetCharge";
    case LHCb::Tagger::VtxCharge:
      return s << "VtxCharge";
    case LHCb::Tagger::Topology:
      return s << "Topology";
    case LHCb::Tagger::OS_nnetKaon:
      return s << "OS_nnetKaon";
    case LHCb::Tagger::SS_nnetKaon:
      return s << "SS_nnetKaon";
    case LHCb::Tagger::SS_Proton:
      return s << "SS_Proton";
    case LHCb::Tagger::OS_Charm:
      return s << "OS_Charm";
    case LHCb::Tagger::SS_PionBDT:
      return s << "SS_PionBDT";
    case LHCb::Tagger::OSMuon:
      return s << "OSMuon";
    case LHCb::Tagger::OSMuonLatest:
      return s << "OSMuonLatest";
    case LHCb::Tagger::OSMuonLatestB2OC:
      return s << "OSMuonLatestB2OC";
    case LHCb::Tagger::OSMuonLatestB2CC:
      return s << "OSMuonLatestB2CC";
    case LHCb::Tagger::OSMuonDev:
      return s << "OSMuonDev";
    case LHCb::Tagger::OSElectron:
      return s << "OSElectron";
    case LHCb::Tagger::OSElectronLatest:
      return s << "OSElectronLatest";
    case LHCb::Tagger::OSElectronLatestB2OC:
      return s << "OSElectronLatestB2OC";
    case LHCb::Tagger::OSElectronLatestB2CC:
      return s << "OSElectronLatestB2CC";
    case LHCb::Tagger::OSElectronDev:
      return s << "OSElectronDev";
    case LHCb::Tagger::OSKaon:
      return s << "OSKaon";
    case LHCb::Tagger::OSKaonLatest:
      return s << "OSKaonLatest";
    case LHCb::Tagger::OSKaonLatestB2OC:
      return s << "OSKaonLatestB2OC";
    case LHCb::Tagger::OSKaonLatestB2CC:
      return s << "OSKaonLatestB2CC";
    case LHCb::Tagger::OSKaonDev:
      return s << "OSKaonDev";
    case LHCb::Tagger::OSVtxCh:
      return s << "OSVtxCh";
    case LHCb::Tagger::OSVtxChLatest:
      return s << "OSVtxChLatest";
    case LHCb::Tagger::OSVtxChDev:
      return s << "OSVtxChDev";
    case LHCb::Tagger::OSCharm:
      return s << "OSCharm";
    case LHCb::Tagger::OSCharmLatest:
      return s << "OSCharmLatest";
    case LHCb::Tagger::OSCharmDev:
      return s << "OSCharmDev";
    case LHCb::Tagger::OSProton:
      return s << "OSProton";
    case LHCb::Tagger::OSProtonLatest:
      return s << "OSProtonLatest";
    case LHCb::Tagger::OSProtonDev:
      return s << "OSProtonDev";
    case LHCb::Tagger::SSKaon:
      return s << "SSKaon";
    case LHCb::Tagger::SSKaonLatest:
      return s << "SSKaonLatest";
    case LHCb::Tagger::SSKaonDev:
      return s << "SSKaonDev";
    case LHCb::Tagger::SSPion:
      return s << "SSPion";
    case LHCb::Tagger::SSPionLatest:
      return s << "SSPionLatest";
    case LHCb::Tagger::SSPionDev:
      return s << "SSPionDev";
    case LHCb::Tagger::SSProton:
      return s << "SSProton";
    case LHCb::Tagger::SSProtonLatest:
      return s << "SSProtonLatest";
    case LHCb::Tagger::SSProtonDev:
      return s << "SSProtonDev";
    case LHCb::Tagger::SSLambda:
      return s << "SSLambda";
    case LHCb::Tagger::SSLambdaLatest:
      return s << "SSLambdaLatest";
    case LHCb::Tagger::SSLambdaDev:
      return s << "SSLambdaDev";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::Tagger::TaggerType";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::Tagger::fillStream( std::ostream& s ) const {
  s << "{ "
    << "type :	" << m_type << std::endl
    << "decision :	" << m_decision << std::endl
    << "omega :	" << m_omega << std::endl
    << "mvaValue :	" << m_mvaValue << std::endl
    << "charge :	" << m_charge << std::endl
    << " }";
  return s;
}

inline const GaudiUtils::VectorMap<std::string, LHCb::Tagger::TaggerType>& LHCb::Tagger::s_TaggerTypeTypMap() {
  static const GaudiUtils::VectorMap<std::string, TaggerType> m = {{"none", none},
                                                                   {"unknown", unknown},
                                                                   {"OS_Muon", OS_Muon},
                                                                   {"OS_Electron", OS_Electron},
                                                                   {"OS_Kaon", OS_Kaon},
                                                                   {"SS_Kaon", SS_Kaon},
                                                                   {"SS_Pion", SS_Pion},
                                                                   {"jetCharge", jetCharge},
                                                                   {"OS_jetCharge", OS_jetCharge},
                                                                   {"SS_jetCharge", SS_jetCharge},
                                                                   {"VtxCharge", VtxCharge},
                                                                   {"Topology", Topology},
                                                                   {"OS_nnetKaon", OS_nnetKaon},
                                                                   {"SS_nnetKaon", SS_nnetKaon},
                                                                   {"SS_Proton", SS_Proton},
                                                                   {"OS_Charm", OS_Charm},
                                                                   {"SS_PionBDT", SS_PionBDT},
                                                                   {"OSMuon", OSMuon},
                                                                   {"OSMuonLatest", OSMuonLatest},
                                                                   {"OSMuonLatestB2OC", OSMuonLatestB2OC},
                                                                   {"OSMuonLatestB2CC", OSMuonLatestB2CC},
                                                                   {"OSMuonDev", OSMuonDev},
                                                                   {"OSElectron", OSElectron},
                                                                   {"OSElectronLatest", OSElectronLatest},
                                                                   {"OSElectronLatestB2OC", OSElectronLatestB2OC},
                                                                   {"OSElectronLatestB2CC", OSElectronLatestB2CC},
                                                                   {"OSElectronDev", OSElectronDev},
                                                                   {"OSKaon", OSKaon},
                                                                   {"OSKaonLatest", OSKaonLatest},
                                                                   {"OSKaonLatestB2OC", OSKaonLatestB2OC},
                                                                   {"OSKaonLatestB2CC", OSKaonLatestB2CC},
                                                                   {"OSKaonDev", OSKaonDev},
                                                                   {"OSVtxCh", OSVtxCh},
                                                                   {"OSVtxChLatest", OSVtxChLatest},
                                                                   {"OSVtxChDev", OSVtxChDev},
                                                                   {"OSCharm", OSCharm},
                                                                   {"OSCharmLatest", OSCharmLatest},
                                                                   {"OSCharmDev", OSCharmDev},
                                                                   {"OSProton", OSProton},
                                                                   {"OSProtonLatest", OSProtonLatest},
                                                                   {"OSProtonDev", OSProtonDev},
                                                                   {"SSKaon", SSKaon},
                                                                   {"SSKaonLatest", SSKaonLatest},
                                                                   {"SSKaonDev", SSKaonDev},
                                                                   {"SSPion", SSPion},
                                                                   {"SSPionLatest", SSPionLatest},
                                                                   {"SSPionDev", SSPionDev},
                                                                   {"SSProton", SSProton},
                                                                   {"SSProtonLatest", SSProtonLatest},
                                                                   {"SSProtonDev", SSProtonDev},
                                                                   {"SSLambda", SSLambda},
                                                                   {"SSLambdaLatest", SSLambdaLatest},
                                                                   {"SSLambdaDev", SSLambdaDev}};
  return m;
}

inline LHCb::Tagger::TaggerType LHCb::Tagger::TaggerTypeToType( const std::string& aName ) {
  auto iter = s_TaggerTypeTypMap().find( aName );
  return iter != s_TaggerTypeTypMap().end() ? iter->second : unknown;
}

inline const std::string& LHCb::Tagger::TaggerTypeToString( int aEnum ) {
  static const std::string s_unknown = "unknown";
  auto                     iter      = std::find_if( s_TaggerTypeTypMap().begin(), s_TaggerTypeTypMap().end(),
                            [&]( const std::pair<const std::string, TaggerType>& i ) { return i.second == aEnum; } );
  return iter != s_TaggerTypeTypMap().end() ? iter->first : s_unknown;
}

inline unsigned short int LHCb::Tagger::type() const { return m_type; }

inline void LHCb::Tagger::setType( unsigned short int value ) { m_type = value; }

inline short int LHCb::Tagger::decision() const { return m_decision; }

inline void LHCb::Tagger::setDecision( short int value ) { m_decision = value; }

inline float LHCb::Tagger::omega() const { return m_omega; }

inline void LHCb::Tagger::setOmega( float value ) { m_omega = value; }

inline float LHCb::Tagger::mvaValue() const { return m_mvaValue; }

inline void LHCb::Tagger::setMvaValue( float value ) { m_mvaValue = value; }

inline float LHCb::Tagger::charge() const { return m_charge; }

inline void LHCb::Tagger::setCharge( float value ) { m_charge = value; }

inline const SmartRefVector<LHCb::Particle>& LHCb::Tagger::taggerParts() const { return m_taggerParts; }

inline void LHCb::Tagger::setTaggerParts( const SmartRefVector<LHCb::Particle>& value ) { m_taggerParts = value; }

inline void LHCb::Tagger::addToTaggerParts( const SmartRef<LHCb::Particle>& value ) {
  m_taggerParts.push_back( value );
}

inline void LHCb::Tagger::addToTaggerParts( const LHCb::Particle* value ) { m_taggerParts.push_back( value ); }

inline void LHCb::Tagger::removeFromTaggerParts( const SmartRef<LHCb::Particle>& value ) {
  auto i = std::remove( m_taggerParts.begin(), m_taggerParts.end(), value );
  m_taggerParts.erase( i, m_taggerParts.end() );
}

inline void LHCb::Tagger::clearTaggerParts() { m_taggerParts.clear(); }
