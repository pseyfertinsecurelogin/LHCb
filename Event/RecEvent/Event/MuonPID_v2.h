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
#include "Event/Track.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb::Event {

  namespace v2 {
    // Namespace for locations in TDS
    namespace MuonPIDLocation {
      inline std::string const Default = "Rec/Muon/MuonPID";
      inline std::string const Offline = "Rec/Muon/MuonPID";
      inline std::string const Hlt     = "Hlt/Muon/MuonPID";
    } // namespace MuonPIDLocation

    class MuonPID final {
    public:
      /*      MuonPID(const LHCb::MuonPID& lhs) : KeyedObject<int>(),
                                                m_MuonLLMu(lhs.m_MuonLLMu),
                                                m_MuonLLBg(lhs.m_MuonLLBg),
                                                m_NShared(lhs.m_NShared),
                                                m_Status(lhs.m_Status),
                                                m_chi2Corr(lhs.m_chi2Corr),
                                                m_muonMVA1(lhs.m_muonMVA1),
                                                m_muonMVA2(lhs.m_muonMVA2),
                                                m_muonMVA3(lhs.m_muonMVA3),
                                                m_muonMVA4(lhs.m_muonMVA4),
      //                                          m_IDTrack(lhs.m_IDTrack),
      //                                          m_muonTrack(lhs.m_muonTrack) {}

            MuonPID() : m_MuonLLMu(-20.0),
                        m_MuonLLBg(0.0),
                        m_NShared(0),
                        m_Status(0),
                        m_chi2Corr(0.0),
                        m_muonMVA1(-999.0),
                        m_muonMVA2(-999.0),
                        m_muonMVA3(-999.0),
                        m_muonMVA4(-999.0) {}
      */

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

      double MuonLLMu() const { return m_MuonLLMu; };

      void setMuonLLMu( double value ) { m_MuonLLMu = value; };

      double MuonLLBg() const { return m_MuonLLBg; };

      void setMuonLLBg( double value ) { m_MuonLLBg = value; };

      int nShared() const { return m_NShared; };

      void setNShared( int value ) { m_NShared = value; };

      unsigned int Status() const { return m_Status; };

      void setStatus( unsigned int value ) { m_Status = value; };

      bool IsMuon() const { return 0 != ( ( m_Status & IsMuonMask ) >> IsMuonBits ); };

      void setIsMuon( bool value ) {
        unsigned int val = (unsigned int)value;
        m_Status &= ~IsMuonMask;
        m_Status |= ( ( ( (unsigned int)val ) << IsMuonBits ) & IsMuonMask );
      };

      bool InAcceptance() const { return 0 != ( ( m_Status & InAcceptanceMask ) >> InAcceptanceBits ); };

      void setInAcceptance( bool value ) {
        unsigned int val = (unsigned int)value;
        m_Status &= ~InAcceptanceMask;
        m_Status |= ( ( ( (unsigned int)val ) << InAcceptanceBits ) & InAcceptanceMask );
      };

      bool PreSelMomentum() const { return 0 != ( ( m_Status & PreSelMomentumMask ) >> PreSelMomentumBits ); };

      void setPreSelMomentum( bool value ) {
        unsigned int val = (unsigned int)value;
        m_Status &= ~PreSelMomentumMask;
        m_Status |= ( ( ( (unsigned int)val ) << PreSelMomentumBits ) & PreSelMomentumMask );
      };

      bool IsMuonLoose() const { return 0 != ( ( m_Status & IsMuonLooseMask ) >> IsMuonLooseBits ); };

      void setIsMuonLoose( bool value ) {
        unsigned int val = (unsigned int)value;
        m_Status &= ~IsMuonLooseMask;
        m_Status |= ( ( ( (unsigned int)val ) << IsMuonLooseBits ) & IsMuonLooseMask );
      };

      bool IsMuonTight() const { return 0 != ( ( m_Status & IsMuonTightMask ) >> IsMuonTightBits ); };

      void setIsMuonTight( bool value ) {
        unsigned int val = (unsigned int)value;
        m_Status &= ~IsMuonTightMask;
        m_Status |= ( ( ( (unsigned int)val ) << IsMuonTightBits ) & IsMuonTightMask );
      };

      float chi2Corr() const { return m_chi2Corr; };

      void setChi2Corr( float value ) { m_chi2Corr = value; };

      float muonMVA1() const { return m_muonMVA1; };

      void setMuonMVA1( float value ) { m_muonMVA1 = value; };

      float muonMVA2() const { return m_muonMVA2; };

      void setMuonMVA2( float value ) { m_muonMVA2 = value; };

      float muonMVA3() const { return m_muonMVA3; };

      void setMuonMVA3( float value ) { m_muonMVA3 = value; };

      float muonMVA4() const { return m_muonMVA4; };

      void setMuonMVA4( float value ) { m_muonMVA4 = value; };

      //      const LHCb::Track* LHCb::MuonPID::idTrack() const { return m_IDTrack; };

      //      void setIDTrack(const SmartRef<LHCb::Track>& value) { m_IDTrack = value; };

      //      void setIDTrack(const LHCb::Track* value) { m_IDTrack = value; };

      //      const LHCb::Track* LHCb::MuonPID::muonTrack() const { return m_muonTrack; };

      //      void setMuonTrack(const SmartRef<LHCb::Track>& value) { m_muonTrack = value; };

      //      void setMuonTrack(const LHCb::Track* value) { m_muonTrack = value; };

    protected:
    private:
      enum StatusBits {
        IsMuonBits         = 0,
        InAcceptanceBits   = 1,
        PreSelMomentumBits = 2,
        IsMuonLooseBits    = 3,
        IsMuonTightBits    = 4
      };

      enum StatusMasks {
        IsMuonMask         = 0x1L,
        InAcceptanceMask   = 0x2L,
        PreSelMomentumMask = 0x4L,
        IsMuonLooseMask    = 0x8L,
        IsMuonTightMask    = 0x10L
      };

      double       m_MuonLLMu;
      double       m_MuonLLBg;
      int          m_NShared;
      unsigned int m_Status;
      float        m_chi2Corr;
      float        m_muonMVA1;
      float        m_muonMVA2;
      float        m_muonMVA3;
      float        m_muonMVA4;
      //      SmartRef<LHCb::Track> m_IDTrack;
      //      SmartRef<LHCb::Track> m_muonTrack;

    }; // class MuonPID

  } // namespace v2
} // namespace LHCb::Event

#endif
