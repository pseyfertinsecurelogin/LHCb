/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef RecEvent_MuonPID_v2_H
#define RecEvent_MuonPID_v2_H 1

// Include files
#include "SOAContainer/SOAField.h"
#include "SOAContainer/SOASkin.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb::Event::v2 {
  // Namespace for locations in TDS
  namespace MuonPIDLocation {
    inline std::string const Default = "Rec/Muon/MuonPID";
    inline std::string const Offline = "Rec/Muon/MuonPID";
    inline std::string const Hlt     = "Hlt/Muon/MuonPID";
  } // namespace MuonPIDLocation

  class MuonPID final {
  public:
    std::ostream& fillStream( std::ostream& s ) const {
      s << "{ "
        << "MuonLLMu :  " << (float)m_MuonLLMu << std::endl
        << "MuonLLBg :  " << (float)m_MuonLLBg << std::endl
        << "NShared :   " << m_NShared << std::endl
        << "Status :    " << m_Status << std::endl
        << "chi2Corr :  " << m_chi2Corr << std::endl
        << "muonMVA1 :  " << m_muonMVA1 << std::endl
        << "muonMVA2 :  " << m_muonMVA2 << std::endl
        << "muonMVA3 :  " << m_muonMVA3 << std::endl
        << "muonMVA4 :  " << m_muonMVA4 << std::endl
        << " }";
      return s;
    };

    [[nodiscard]] double MuonLLMu() const { return m_MuonLLMu; };

    void setMuonLLMu( double value ) { m_MuonLLMu = value; };

    [[nodiscard]] double MuonLLBg() const { return m_MuonLLBg; };

    void setMuonLLBg( double value ) { m_MuonLLBg = value; };

    [[nodiscard]] int nShared() const { return m_NShared; };

    void setNShared( int value ) { m_NShared = value; };

    [[nodiscard]] unsigned int Status() const { return m_Status; };

    void setStatus( unsigned int value ) { m_Status = value; };

    [[nodiscard]] bool IsMuon() const { return test<StatusMask::isMuon>(); }

    void setIsMuon( bool value ) { set<StatusMask::isMuon>( value ); }

    [[nodiscard]] bool InAcceptance() const { return test<StatusMask::inAcceptance>(); }

    void setInAcceptance( bool value ) { set<StatusMask::inAcceptance>( value ); }

    [[nodiscard]] bool PreSelMomentum() const { return test<StatusMask::preSelMomentum>(); }

    void setPreSelMomentum( bool value ) { set<StatusMask::preSelMomentum>( value ); }

    [[nodiscard]] bool IsMuonLoose() const { return test<StatusMask::isMuonLoose>(); }

    void setIsMuonLoose( bool value ) { set<StatusMask::isMuonLoose>( value ); }

    [[nodiscard]] bool IsMuonTight() const { return test<StatusMask::isMuonTight>(); }

    void setIsMuonTight( bool value ) { set<StatusMask::isMuonTight>( value ); }

    [[nodiscard]] float chi2Corr() const { return m_chi2Corr; };

    void setChi2Corr( float value ) { m_chi2Corr = value; };

    [[nodiscard]] float muonMVA1() const { return m_muonMVA1; };

    void setMuonMVA1( float value ) { m_muonMVA1 = value; };

    [[nodiscard]] float muonMVA2() const { return m_muonMVA2; };

    void setMuonMVA2( float value ) { m_muonMVA2 = value; };

    [[nodiscard]] float muonMVA3() const { return m_muonMVA3; };

    void setMuonMVA3( float value ) { m_muonMVA3 = value; };

    [[nodiscard]] float muonMVA4() const { return m_muonMVA4; };

    void setMuonMVA4( float value ) { m_muonMVA4 = value; };

  private:
    template <unsigned int mask>
    void set( bool value ) {
      m_Status = ( m_Status & ~mask ) | ( -value & mask );
    }
    template <unsigned int mask>
    bool test() const {
      return ( m_Status & mask ) != 0;
    }

    enum StatusMask : unsigned int {
      isMuon         = 0x1U,
      inAcceptance   = 0x2U,
      preSelMomentum = 0x4U,
      isMuonLoose    = 0x8U,
      isMuonTight    = 0x10U
    };

    double       m_MuonLLMu = -20.0;
    double       m_MuonLLBg = 0.0;
    int          m_NShared  = 0;
    unsigned int m_Status   = 0;
    float        m_chi2Corr = 0.F;
    float        m_muonMVA1 = -999.F;
    float        m_muonMVA2 = -999.F;
    float        m_muonMVA3 = -999.F;
    float        m_muonMVA4 = -999.F;

  }; // class MuonPID

  // clang-format off
    SOAFIELD( MuonPIDField, MuonPID, SOAFIELD_ACCESSORS( muonPID )
              auto IsMuon() const { return muonPID().IsMuon(); }
              );
  // clang-format on
  SOASKIN_TRIVIAL( MuonID, MuonPIDField );
} // namespace LHCb::Event::v2

#endif
