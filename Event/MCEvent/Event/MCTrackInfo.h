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
#ifndef MCEVENT_MCTRACKINFO_H
#define MCEVENT_MCTRACKINFO_H 1

#include "Event/MCProperty.h"

// Forward declarations
class IDataProviderSvc;
class IMessageSvc;

namespace LHCb {
  class MCParticle;
}

/** @class MCTrackInfo MCTrackInfo.h Event/MCTrackInfo.h
 *
 *
 *  @author Olivier Callot revised by M. Needham
 *
 *  @date   2004-01-08
 */
class MCTrackInfo final {

public:
  enum flagMasks {
    maskVeloR   = 0x00000001,
    maskVeloPhi = 0x00000002,
    maskTT1     = 0x00000004,
    maskTT2     = 0x00000008,
    maskT1X     = 0x00000010,
    maskT1S     = 0x00000020,
    maskT2X     = 0x00000040,
    maskT2S     = 0x00000080,
    maskT3X     = 0x00000100,
    maskT3S     = 0x00000200,

    maskHasVelo     = maskVeloR | maskVeloPhi, // R + Phi
    maskHasTT       = maskTT1 | maskTT2,       // TT1 and TT2
    maskHasT1       = maskT1X | maskT1S,
    maskHasT2       = maskT2X | maskT2S,
    maskHasT3       = maskT3X | maskT3S,
    maskHasT        = maskHasT1 | maskHasT2 | maskHasT3, // all T stations
    maskHasVeloAndT = maskHasVelo | maskHasT,

    maskAccVeloR   = 0x00000400,
    maskAccVeloPhi = 0x00000800,
    maskAccTT1     = 0x00001000,
    maskAccTT2     = 0x00002000,
    maskAccT1X     = 0x00004000,
    maskAccT1S     = 0x00008000,
    maskAccT2X     = 0x00010000,
    maskAccT2S     = 0x00020000,
    maskAccT3X     = 0x00040000,
    maskAccT3S     = 0x00080000,

    maskAccVelo     = maskAccVeloR | maskAccVeloPhi, // R and Phi hits
    maskAccTT       = maskAccTT1 | maskAccTT2,       // TT1 and TT2
    maskAccT1       = maskAccT1X | maskAccT1S,
    maskAccT2       = maskAccT2X | maskAccT2S,
    maskAccT3       = maskAccT3X | maskAccT3S,
    maskAccT        = maskAccT1 | maskAccT2 | maskAccT3, // T stations
    maskAccVeloAndT = maskAccVelo | maskAccT,

    multVeloR   = 20,
    multVeloPhi = 25,

    maskMultVeloR   = 0x01F00000, // Velo R CLuster multiplicity
    maskMultVeloPhi = 0x3E000000  // VeloPhi cluster multipliity
  };

  /// Standard constructor
  MCTrackInfo( const LHCb::MCProperty& flags ) : m_flags( &flags ) {}

  /// Deprecated constructor
  [[deprecated( "please fetch LHCb::MCProperty from LHCb::MCPropertyLocation::TrackInfo, and pass it to the "
                "MCTrackInfo constructor instead" )]] MCTrackInfo( IDataProviderSvc* eventSvc, IMessageSvc* msgSvc );

  //== Main accessors: Velo, TT, T and Velo+T for cluster and acceptance

  bool hasVelo( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasVelo ); }

  bool hasTT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasTT ); }

  bool hasT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasT ); }

  bool hasVeloAndT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasVeloAndT ); }

  bool hasT1( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasT1 ); }

  bool hasT2( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasT2 ); }

  bool hasT3( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskHasT3 ); }

  bool accVelo( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccVelo ); }

  bool accTT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccTT ); }

  bool accT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT ); }

  bool accT1( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT1 ); }

  bool accT2( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT2 ); }

  bool accT3( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT3 ); }

  bool accVeloAndT( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccVeloAndT ); }

  unsigned int fullInfo( const LHCb::MCParticle* part ) const { return m_flags->property( part ); }

  //============= Access to individual bits, 'has' = enough digits/clusters

  bool hasVeloR( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskVeloR ); }

  bool hasVeloPhi( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskVeloPhi ); }

  bool hasTT1( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskTT1 ); }

  bool hasTT2( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskTT2 ); }

  bool hasT1X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT1X ); }

  bool hasT1S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT1S ); }

  bool hasT2X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT2X ); }

  bool hasT2S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT2S ); }

  bool hasT3X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT3X ); }

  bool hasT3S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskT3S ); }

  //=== Number of Velo clusters

  unsigned int nbVeloR( const LHCb::MCParticle* part ) const;

  unsigned int nbVeloPhi( const LHCb::MCParticle* part ) const;

  //============= Access to individual bits for acceptance = MCHits

  bool accVeloR( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccVeloR ); }

  bool accVeloPhi( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccVeloPhi ); }

  bool accTT1( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccTT1 ); }

  bool accTT2( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccTT2 ); }

  bool accT1X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT1X ); }

  bool accT1S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT1S ); }

  bool accT2X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT2X ); }

  bool accT2S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT2S ); }

  bool accT3X( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT3X ); }

  bool accT3S( const LHCb::MCParticle* part ) const { return testMask( part, MCTrackInfo::maskAccT3S ); }

  bool testMask( const LHCb::MCParticle* part, unsigned int mask ) const;

private:
  const LHCb::MCProperty* m_flags = nullptr;
};

// Include files
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/SmartDataPtr.h"

inline MCTrackInfo make_MCTrackInfo( IDataProviderSvc* eventSvc, IMessageSvc* msgSvc ) {
  SmartDataPtr<LHCb::MCProperty> flags( eventSvc, LHCb::MCPropertyLocation::TrackInfo );
  if ( !flags ) {
    MsgStream msg( msgSvc, "MCTrackInfo" );
    msg << MSG::ERROR << "*** Flag container " << LHCb::MCPropertyLocation::TrackInfo << " not found." << endmsg;
  }
  return {flags};
}

inline MCTrackInfo::MCTrackInfo( IDataProviderSvc* eventSvc, IMessageSvc* msgSvc ) {
  SmartDataPtr<LHCb::MCProperty> flags( eventSvc, LHCb::MCPropertyLocation::TrackInfo );
  if ( !flags ) {
    MsgStream msg( msgSvc, "MCTrackInfo" );
    msg << MSG::ERROR << "*** Flag container " << LHCb::MCPropertyLocation::TrackInfo << " not found." << endmsg;
  }
  m_flags = flags;
}

inline bool MCTrackInfo::testMask( const LHCb::MCParticle* part, const unsigned int mask ) const {
  return !m_flags ? false : mask == ( mask & m_flags->property( part ) );
}

inline unsigned int MCTrackInfo::nbVeloR( const LHCb::MCParticle* part ) const {
  const unsigned int word = m_flags->property( part ) & MCTrackInfo::maskMultVeloR;
  return word >> MCTrackInfo::multVeloR;
}

inline unsigned int MCTrackInfo::nbVeloPhi( const LHCb::MCParticle* part ) const {
  const unsigned int word = m_flags->property( part ) & MCTrackInfo::maskMultVeloPhi;
  return word >> MCTrackInfo::multVeloPhi;
}

#endif // MCEVENT_MCTRACKINFO_H
