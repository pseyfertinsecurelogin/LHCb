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
#include "Event/Track.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRef.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MuonPID = 11050;

  // Namespace for locations in TDS
  namespace MuonPIDLocation {
    inline const std::string Default = "Rec/Muon/MuonPID";
    inline const std::string Offline = "Rec/Muon/MuonPID";
    inline const std::string Hlt     = "Hlt/Muon/MuonPID";
  } // namespace MuonPIDLocation

  /** @class MuonPID MuonPID.h
   *
   * Stores the output of the Muon Identification in the muon system
   *
   * @author M. Gandelman, E. Polycarpo
   *
   */

  class MuonPID final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MuonPID
    typedef KeyedContainer<MuonPID, Containers::HashMap> Container;

    /// Copy constructor. Creates a new MuonPID object with the same pid information
    MuonPID( const LHCb::MuonPID& lhs )
        : KeyedObject<int>()
        , m_MuonLLMu( lhs.m_MuonLLMu )
        , m_MuonLLBg( lhs.m_MuonLLBg )
        , m_NShared( lhs.m_NShared )
        , m_Status( lhs.m_Status )
        , m_chi2Corr( lhs.m_chi2Corr )
        , m_muonMVA1( lhs.m_muonMVA1 )
        , m_muonMVA2( lhs.m_muonMVA2 )
        , m_muonMVA3( lhs.m_muonMVA3 )
        , m_muonMVA4( lhs.m_muonMVA4 )
        , m_IDTrack( lhs.m_IDTrack )
        , m_muonTrack( lhs.m_muonTrack ) {}

    /// Default Constructor
    MuonPID()
        : m_MuonLLMu( -20.0 )
        , m_MuonLLBg( 0.0 )
        , m_NShared( 0 )
        , m_Status( 0 )
        , m_chi2Corr( 0.0 )
        , m_muonMVA1( -999.0 )
        , m_muonMVA2( -999.0 )
        , m_muonMVA3( -999.0 )
        , m_muonMVA4( -999.0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Muon Likelihood for muons
    double MuonLLMu() const;

    /// Update  Muon Likelihood for muons
    void setMuonLLMu( double value );

    /// Retrieve const  Muon likelihood for non muons
    double MuonLLBg() const;

    /// Update  Muon likelihood for non muons
    void setMuonLLBg( double value );

    /// Retrieve const  Number of tracks which share hits
    int nShared() const;

    /// Update  Number of tracks which share hits
    void setNShared( int value );

    /// Retrieve const  Status of MuonPID
    unsigned int Status() const;

    /// Update  Status of MuonPID
    void setStatus( unsigned int value );

    /// Retrieve Boolean: Is track a muon ?
    bool IsMuon() const;

    /// Update Boolean: Is track a muon ?
    void setIsMuon( bool value );

    /// Retrieve Boolean: True if track extraoplation is in the Muon acceptance
    bool InAcceptance() const;

    /// Update Boolean: True if track extraoplation is in the Muon acceptance
    void setInAcceptance( bool value );

    /// Retrieve Boolean: True if track has minimal momentum
    bool PreSelMomentum() const;

    /// Update Boolean: True if track has minimal momentum
    void setPreSelMomentum( bool value );

    /// Retrieve Boolean: Is track a muon (looser defintion) ?
    bool IsMuonLoose() const;

    /// Update Boolean: Is track a muon (looser defintion) ?
    void setIsMuonLoose( bool value );

    /// Retrieve Boolean: Is track a muon (with x,y crossing requirement for muon hits) ?
    bool IsMuonTight() const;

    /// Update Boolean: Is track a muon (with x,y crossing requirement for muon hits) ?
    void setIsMuonTight( bool value );

    /// Retrieve const  chi2 using correlations of the hits
    float chi2Corr() const;

    /// Update  chi2 using correlations of the hits
    void setChi2Corr( float value );

    /// Retrieve const  multi variate algorithm for MuonID with tune1
    float muonMVA1() const;

    /// Update  multi variate algorithm for MuonID with tune1
    void setMuonMVA1( float value );

    /// Retrieve const  multi variate algorithm for MuonID with tune2
    float muonMVA2() const;

    /// Update  multi variate algorithm for MuonID with tune2
    void setMuonMVA2( float value );

    /// Retrieve const  multi variate algorithm for MuonID with tune3
    float muonMVA3() const;

    /// Update  multi variate algorithm for MuonID with tune3
    void setMuonMVA3( float value );

    /// Retrieve const  multi variate algorithm for MuonID with tune4
    float muonMVA4() const;

    /// Update  multi variate algorithm for MuonID with tune4
    void setMuonMVA4( float value );

    /// Retrieve (const)  The track that has been IDed by the Muon system
    const LHCb::Track* idTrack() const;

    /// Update  The track that has been IDed by the Muon system
    void setIDTrack( const SmartRef<LHCb::Track>& value );

    /// Update (pointer)  The track that has been IDed by the Muon system
    void setIDTrack( const LHCb::Track* value );

    /// Retrieve (const)  The track segment as reconstructed in the Muon system
    const LHCb::Track* muonTrack() const;

    /// Update  The track segment as reconstructed in the Muon system
    void setMuonTrack( const SmartRef<LHCb::Track>& value );

    /// Update (pointer)  The track segment as reconstructed in the Muon system
    void setMuonTrack( const LHCb::Track* value );

    friend std::ostream& operator<<( std::ostream& str, const MuonPID& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Offsets of bitfield Status
    enum StatusBits {
      IsMuonBits         = 0,
      InAcceptanceBits   = 1,
      PreSelMomentumBits = 2,
      IsMuonLooseBits    = 3,
      IsMuonTightBits    = 4
    };

    /// Bitmasks for bitfield Status
    enum StatusMasks {
      IsMuonMask         = 0x1L,
      InAcceptanceMask   = 0x2L,
      PreSelMomentumMask = 0x4L,
      IsMuonLooseMask    = 0x8L,
      IsMuonTightMask    = 0x10L
    };

    double                m_MuonLLMu;  ///< Muon Likelihood for muons
    double                m_MuonLLBg;  ///< Muon likelihood for non muons
    int                   m_NShared;   ///< Number of tracks which share hits
    unsigned int          m_Status;    ///< Status of MuonPID
    float                 m_chi2Corr;  ///< chi2 using correlations of the hits
    float                 m_muonMVA1;  ///< multi variate algorithm for MuonID with tune1
    float                 m_muonMVA2;  ///< multi variate algorithm for MuonID with tune2
    float                 m_muonMVA3;  ///< multi variate algorithm for MuonID with tune3
    float                 m_muonMVA4;  ///< multi variate algorithm for MuonID with tune4
    SmartRef<LHCb::Track> m_IDTrack;   ///< The track that has been IDed by the Muon system
    SmartRef<LHCb::Track> m_muonTrack; ///< The track segment as reconstructed in the Muon system

  }; // class MuonPID

  /// Definition of Keyed Container for MuonPID
  typedef KeyedContainer<MuonPID, Containers::HashMap> MuonPIDs;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::MuonPID::clID() const { return LHCb::MuonPID::classID(); }

inline const CLID& LHCb::MuonPID::classID() { return CLID_MuonPID; }

inline std::ostream& LHCb::MuonPID::fillStream( std::ostream& s ) const {
  s << "{ "
    << "MuonLLMu :	" << (float)m_MuonLLMu << std::endl
    << "MuonLLBg :	" << (float)m_MuonLLBg << std::endl
    << "NShared :	" << m_NShared << std::endl
    << "Status :	" << m_Status << std::endl
    << "chi2Corr :	" << m_chi2Corr << std::endl
    << "muonMVA1 :	" << m_muonMVA1 << std::endl
    << "muonMVA2 :	" << m_muonMVA2 << std::endl
    << "muonMVA3 :	" << m_muonMVA3 << std::endl
    << "muonMVA4 :	" << m_muonMVA4 << std::endl
    << " }";
  return s;
}

inline double LHCb::MuonPID::MuonLLMu() const { return m_MuonLLMu; }

inline void LHCb::MuonPID::setMuonLLMu( double value ) { m_MuonLLMu = value; }

inline double LHCb::MuonPID::MuonLLBg() const { return m_MuonLLBg; }

inline void LHCb::MuonPID::setMuonLLBg( double value ) { m_MuonLLBg = value; }

inline int LHCb::MuonPID::nShared() const { return m_NShared; }

inline void LHCb::MuonPID::setNShared( int value ) { m_NShared = value; }

inline unsigned int LHCb::MuonPID::Status() const { return m_Status; }

inline void LHCb::MuonPID::setStatus( unsigned int value ) { m_Status = value; }

inline bool LHCb::MuonPID::IsMuon() const { return 0 != ( ( m_Status & IsMuonMask ) >> IsMuonBits ); }

inline void LHCb::MuonPID::setIsMuon( bool value ) {
  unsigned int val = (unsigned int)value;
  m_Status &= ~IsMuonMask;
  m_Status |= ( ( ( (unsigned int)val ) << IsMuonBits ) & IsMuonMask );
}

inline bool LHCb::MuonPID::InAcceptance() const { return 0 != ( ( m_Status & InAcceptanceMask ) >> InAcceptanceBits ); }

inline void LHCb::MuonPID::setInAcceptance( bool value ) {
  unsigned int val = (unsigned int)value;
  m_Status &= ~InAcceptanceMask;
  m_Status |= ( ( ( (unsigned int)val ) << InAcceptanceBits ) & InAcceptanceMask );
}

inline bool LHCb::MuonPID::PreSelMomentum() const {
  return 0 != ( ( m_Status & PreSelMomentumMask ) >> PreSelMomentumBits );
}

inline void LHCb::MuonPID::setPreSelMomentum( bool value ) {
  unsigned int val = (unsigned int)value;
  m_Status &= ~PreSelMomentumMask;
  m_Status |= ( ( ( (unsigned int)val ) << PreSelMomentumBits ) & PreSelMomentumMask );
}

inline bool LHCb::MuonPID::IsMuonLoose() const { return 0 != ( ( m_Status & IsMuonLooseMask ) >> IsMuonLooseBits ); }

inline void LHCb::MuonPID::setIsMuonLoose( bool value ) {
  unsigned int val = (unsigned int)value;
  m_Status &= ~IsMuonLooseMask;
  m_Status |= ( ( ( (unsigned int)val ) << IsMuonLooseBits ) & IsMuonLooseMask );
}

inline bool LHCb::MuonPID::IsMuonTight() const { return 0 != ( ( m_Status & IsMuonTightMask ) >> IsMuonTightBits ); }

inline void LHCb::MuonPID::setIsMuonTight( bool value ) {
  unsigned int val = (unsigned int)value;
  m_Status &= ~IsMuonTightMask;
  m_Status |= ( ( ( (unsigned int)val ) << IsMuonTightBits ) & IsMuonTightMask );
}

inline float LHCb::MuonPID::chi2Corr() const { return m_chi2Corr; }

inline void LHCb::MuonPID::setChi2Corr( float value ) { m_chi2Corr = value; }

inline float LHCb::MuonPID::muonMVA1() const { return m_muonMVA1; }

inline void LHCb::MuonPID::setMuonMVA1( float value ) { m_muonMVA1 = value; }

inline float LHCb::MuonPID::muonMVA2() const { return m_muonMVA2; }

inline void LHCb::MuonPID::setMuonMVA2( float value ) { m_muonMVA2 = value; }

inline float LHCb::MuonPID::muonMVA3() const { return m_muonMVA3; }

inline void LHCb::MuonPID::setMuonMVA3( float value ) { m_muonMVA3 = value; }

inline float LHCb::MuonPID::muonMVA4() const { return m_muonMVA4; }

inline void LHCb::MuonPID::setMuonMVA4( float value ) { m_muonMVA4 = value; }

inline const LHCb::Track* LHCb::MuonPID::idTrack() const { return m_IDTrack; }

inline void LHCb::MuonPID::setIDTrack( const SmartRef<LHCb::Track>& value ) { m_IDTrack = value; }

inline void LHCb::MuonPID::setIDTrack( const LHCb::Track* value ) { m_IDTrack = value; }

inline const LHCb::Track* LHCb::MuonPID::muonTrack() const { return m_muonTrack; }

inline void LHCb::MuonPID::setMuonTrack( const SmartRef<LHCb::Track>& value ) { m_muonTrack = value; }

inline void LHCb::MuonPID::setMuonTrack( const LHCb::Track* value ) { m_muonTrack = value; }
