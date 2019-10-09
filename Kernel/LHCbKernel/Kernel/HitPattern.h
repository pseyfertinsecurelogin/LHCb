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
#include "Kernel/LHCbID.h"
#include <bitset>
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class HitPattern HitPattern.h
   *
   * pattern of hits used on tracks
   *
   * @author Stephanie Hansmann-Menzemer and Wouter Hulsbergen (2009)
   *
   */

  class HitPattern {
  public:
    /// Velo hit pattern
    typedef std::bitset<26> VeloPattern;
    /// TT hit pattern
    typedef std::bitset<4> TTPattern;
    /// UT hit pattern
    typedef std::bitset<4> UTPattern;
    /// T-station hit pattern
    typedef std::bitset<12> TPattern;
    /// Muon station hit pattern
    typedef std::bitset<5> MuonPattern;

    /// number of detector layers/regions
    enum Number { NumVelo = 26, NumTT = 4, NumT = 12, NumMuon = 5 };

    /// Constructor from LHCbIDs
    HitPattern( const std::vector<LHCbID>& ids );

    /// Default Constructor
    HitPattern()
        : m_veloRA()
        , m_veloRC()
        , m_veloPhiA()
        , m_veloPhiC()
        , m_ot1stMonoLayer()
        , m_ot2ndMonoLayer()
        , m_itAC()
        , m_itTopBottom()
        , m_tt()
        , m_ut()
        , m_muon() {}

    /// Default Destructor
    ~HitPattern() {}

    /// Print this HitPattern
    std::ostream& fillStream( std::ostream& s ) const;

    /// it layer pattern
    TPattern it() const;

    /// ot layer pattern
    TPattern ot() const;

    /// velo layer pattern
    VeloPattern velo() const;

    /// number of velo R hits
    size_t numVeloR() const;

    /// number of velo phi hits
    size_t numVeloPhi() const;

    /// number of OT hits, both monolayers
    size_t numOTHits() const;

    /// number of IT hits
    size_t numITHits() const;

    /// number of TT hits
    size_t numTTHits() const;

    /// number of UT hits
    size_t numUTHits() const;

    /// number of velo stations with one R or phi hit
    size_t numVeloStations() const;

    /// number of velo stations with one R or phi hit on both A and C side
    size_t numVeloStationsOverlap() const;

    /// number of it stations with hit in more than one box
    size_t numITStationsOverlap() const;

    /// number of T stations with hits in both IT and OT
    size_t numITOTStationsOverlap() const;

    /// number of holes in velo (layer) pattern
    size_t numVeloHoles() const;

    /// number of T layers
    size_t numTLayers() const;

    /// number of holes in T (layer) pattern
    size_t numTHoles() const;

    /// reset bit maps
    void reset();

    /// comparison equality
    bool operator==( const HitPattern& hitPat ) const;

    /// Retrieve const  Velo R hit pattern on detector A side
    const VeloPattern& veloRA() const;

    /// Update  Velo R hit pattern on detector A side
    void setVeloRA( const VeloPattern& value );

    /// Retrieve const  Velo R hit pattern on detector C side
    const VeloPattern& veloRC() const;

    /// Update  Velo R hit pattern on detector C side
    void setVeloRC( const VeloPattern& value );

    /// Retrieve const  Velo Phi hit pattern on detector A side
    const VeloPattern& veloPhiA() const;

    /// Update  Velo Phi hit pattern on detector A side
    void setVeloPhiA( const VeloPattern& value );

    /// Retrieve const  Velo Phi hit pattern on detector C side
    const VeloPattern& veloPhiC() const;

    /// Update  Velo Phi hit pattern on detector C side
    void setVeloPhiC( const VeloPattern& value );

    /// Retrieve const  ot hit pattern in 1st monolayer
    const TPattern& ot1stMonoLayer() const;

    /// Update  ot hit pattern in 1st monolayer
    void setOt1stMonoLayer( const TPattern& value );

    /// Retrieve const  ot hit pattern in 2nd monolayer
    const TPattern& ot2ndMonoLayer() const;

    /// Update  ot hit pattern in 2nd monolayer
    void setOt2ndMonoLayer( const TPattern& value );

    /// Retrieve const  it bit pattern in A and C side boxes
    const TPattern& itAC() const;

    /// Update  it bit pattern in A and C side boxes
    void setItAC( const TPattern& value );

    /// Retrieve const  it bit pattern in top and bottom boxes
    const TPattern& itTopBottom() const;

    /// Update  it bit pattern in top and bottom boxes
    void setItTopBottom( const TPattern& value );

    /// Retrieve const  tt bit pattern
    const TTPattern& tt() const;

    /// Update  tt bit pattern
    void setTt( const TTPattern& value );

    /// Retrieve const  ut bit pattern
    const UTPattern& ut() const;

    /// Update  ut bit pattern
    void setUt( const UTPattern& value );

    /// Retrieve const  muon bit pattern
    const MuonPattern& muon() const;

    /// Update  muon bit pattern
    void setMuon( const MuonPattern& value );

    friend std::ostream& operator<<( std::ostream& str, const HitPattern& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    VeloPattern m_veloRA;         ///< Velo R hit pattern on detector A side
    VeloPattern m_veloRC;         ///< Velo R hit pattern on detector C side
    VeloPattern m_veloPhiA;       ///< Velo Phi hit pattern on detector A side
    VeloPattern m_veloPhiC;       ///< Velo Phi hit pattern on detector C side
    TPattern    m_ot1stMonoLayer; ///< ot hit pattern in 1st monolayer
    TPattern    m_ot2ndMonoLayer; ///< ot hit pattern in 2nd monolayer
    TPattern    m_itAC;           ///< it bit pattern in A and C side boxes
    TPattern    m_itTopBottom;    ///< it bit pattern in top and bottom boxes
    TTPattern   m_tt;             ///< tt bit pattern
    UTPattern   m_ut;             ///< ut bit pattern
    MuonPattern m_muon;           ///< muon bit pattern

  }; // class HitPattern

  inline std::ostream& operator<<( std::ostream& s, LHCb::HitPattern::Number e ) {
    switch ( e ) {
    case LHCb::HitPattern::NumVelo:
      return s << "NumVelo";
    case LHCb::HitPattern::NumTT:
      return s << "NumTT";
    case LHCb::HitPattern::NumT:
      return s << "NumT";
    case LHCb::HitPattern::NumMuon:
      return s << "NumMuon";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::HitPattern::Number";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const LHCb::HitPattern::VeloPattern& LHCb::HitPattern::veloRA() const { return m_veloRA; }

inline void LHCb::HitPattern::setVeloRA( const VeloPattern& value ) { m_veloRA = value; }

inline const LHCb::HitPattern::VeloPattern& LHCb::HitPattern::veloRC() const { return m_veloRC; }

inline void LHCb::HitPattern::setVeloRC( const VeloPattern& value ) { m_veloRC = value; }

inline const LHCb::HitPattern::VeloPattern& LHCb::HitPattern::veloPhiA() const { return m_veloPhiA; }

inline void LHCb::HitPattern::setVeloPhiA( const VeloPattern& value ) { m_veloPhiA = value; }

inline const LHCb::HitPattern::VeloPattern& LHCb::HitPattern::veloPhiC() const { return m_veloPhiC; }

inline void LHCb::HitPattern::setVeloPhiC( const VeloPattern& value ) { m_veloPhiC = value; }

inline const LHCb::HitPattern::TPattern& LHCb::HitPattern::ot1stMonoLayer() const { return m_ot1stMonoLayer; }

inline void LHCb::HitPattern::setOt1stMonoLayer( const TPattern& value ) { m_ot1stMonoLayer = value; }

inline const LHCb::HitPattern::TPattern& LHCb::HitPattern::ot2ndMonoLayer() const { return m_ot2ndMonoLayer; }

inline void LHCb::HitPattern::setOt2ndMonoLayer( const TPattern& value ) { m_ot2ndMonoLayer = value; }

inline const LHCb::HitPattern::TPattern& LHCb::HitPattern::itAC() const { return m_itAC; }

inline void LHCb::HitPattern::setItAC( const TPattern& value ) { m_itAC = value; }

inline const LHCb::HitPattern::TPattern& LHCb::HitPattern::itTopBottom() const { return m_itTopBottom; }

inline void LHCb::HitPattern::setItTopBottom( const TPattern& value ) { m_itTopBottom = value; }

inline const LHCb::HitPattern::TTPattern& LHCb::HitPattern::tt() const { return m_tt; }

inline void LHCb::HitPattern::setTt( const TTPattern& value ) { m_tt = value; }

inline const LHCb::HitPattern::UTPattern& LHCb::HitPattern::ut() const { return m_ut; }

inline void LHCb::HitPattern::setUt( const UTPattern& value ) { m_ut = value; }

inline const LHCb::HitPattern::MuonPattern& LHCb::HitPattern::muon() const { return m_muon; }

inline void LHCb::HitPattern::setMuon( const MuonPattern& value ) { m_muon = value; }

inline LHCb::HitPattern::TPattern LHCb::HitPattern::it() const { return ( m_itAC | m_itTopBottom ); }

inline LHCb::HitPattern::TPattern LHCb::HitPattern::ot() const { return ( m_ot1stMonoLayer | m_ot2ndMonoLayer ); }

inline LHCb::HitPattern::VeloPattern LHCb::HitPattern::velo() const {

  return ( m_veloRA | m_veloRC | m_veloPhiA | m_veloPhiC );
}

inline size_t LHCb::HitPattern::numVeloR() const { return ( m_veloRA.count() + m_veloRC.count() ); }

inline size_t LHCb::HitPattern::numVeloPhi() const { return ( m_veloPhiA.count() + m_veloPhiC.count() ); }

inline size_t LHCb::HitPattern::numOTHits() const { return ( m_ot1stMonoLayer.count() + m_ot2ndMonoLayer.count() ); }

inline size_t LHCb::HitPattern::numITHits() const { return ( m_itAC.count() + m_itTopBottom.count() ); }

inline size_t LHCb::HitPattern::numTTHits() const { return ( m_tt.count() ); }

inline size_t LHCb::HitPattern::numUTHits() const { return ( m_ut.count() ); }

inline size_t LHCb::HitPattern::numVeloStations() const { return ( velo().count() ); }

inline size_t LHCb::HitPattern::numTLayers() const { return ( ( ot() | it() ).count() ); }

inline void LHCb::HitPattern::reset() {

  m_veloRA.reset();
  m_veloRC.reset();
  m_veloPhiA.reset();
  m_veloPhiC.reset();

  m_tt.reset();
  m_ut.reset();

  m_itAC.reset();
  m_itTopBottom.reset();

  m_ot1stMonoLayer.reset();
  m_ot2ndMonoLayer.reset();

  m_muon.reset();
}
