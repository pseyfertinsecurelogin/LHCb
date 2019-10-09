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
#include "Event/MuonBXFlag.h"
#include "Event/MuonOriginFlag.h"
#include "Event/PackMCMuonDigitHistory.h"
#include "Kernel/Packer.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class MCMuonHitHistory MCMuonHitHistory.h
   *
   * MC Muon Hit history info
   *
   * @author Alessia Satta
   *
   */

  class MCMuonHitHistory {
  public:
    /// Default Constructor
    MCMuonHitHistory() : m_hitHistory( 0 ) {}

    /// Default Destructor
    virtual ~MCMuonHitHistory() {}

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// has the hit fired the channel?
    bool hasFired();

    /// set firing  flag in the hitHistory word
    void setFiredFlag( unsigned int firedHit );

    /// is the hit out of geometrical acceptance?
    bool isHitOutGeoAccemtance();

    /// set out of geometrical acceptance  flag in the hitHistory word
    void setGeometricalNotAcceptedHitHistory( unsigned int geoOutHit );

    /// is the hit killed by deadtime?
    bool isHitInDeadtime();

    /// set the deadtime flag in the hitHistory word
    void setHitInDeadtime( unsigned int deadtimeHit );

    /// has the hit time jittered outside the time window?
    bool hasTimeJittered();

    /// set out of gate by time jitter  flag in the hitHistory word
    void setHitOutByTimeJitter( unsigned int jitteredHit );

    /// is the hit out after time adjustement ?
    bool isOutForTimeAdjustment();

    /// set out of gate after time adjustment flag in the hitHistory word
    void setHitOutByTimeAdjustment( unsigned int timeAdjustedHit );

    /// set  again in gate by time adjustemt after out by time jitter or in for a hit with TOF out--- flag in the
    /// hitHistory word
    void setHitInByTimeAdjustment( unsigned int adjustedJitteredHit );

    /// is the hit in after time adjustement while it was ouside for TOF or after timejitter?
    bool isInForTimeAdjustment();

    /// has the hit been killed by chamber inefficiency  ?
    bool isKilledByChamberInefficiency();

    /// set the killed by chamber inefficiency flag in the hitHistory word
    void setKilledByChamberInefficiencyHit( unsigned int killedChamberIneff );

    /// set the BX ID of hit in the hitHistory word
    void setBXOfHit( unsigned int BXID );

    /// in which BX the hit has originated ?
    unsigned int BX();

    /// is the hit originated in the current BX ?
    bool isHitOriginatedInCurrentEvent();

    /// is the hit originated in the current BX ?
    bool isHitOriginatedInPrevEvent();

    /// is the hit originated in the current BX ?
    bool isHitOriginatedInPrevPrevEvent();

    /// is the hit originated in the current BX ?
    bool isHitOriginatedInPrevPrevPrevEvent();

    /// is the hit originated in the current BX ?
    bool isHitOriginatedInPrevPrevPrevPrevEvent();

    /// is the hit in dialog deadtime ?
    bool isHitInDialogDeadtime();

    /// set the killed by dilaog deadtime flag in the hitHistory word
    void setKilledByDialogDeadtimeHit( unsigned int killedChamberIneff );

    /// set the nature of hit in the hitHistory word
    void setNatureOfHit( unsigned int NatureID );

    /// which is the nature of the hit?
    unsigned int natureOfHit();

    /// Is a GEANT hit?
    bool isGeantHit();

    /// Is a low energy background hit?
    bool isBackgroundHit();

    /// Is a chamber noise hit?
    bool isChamberNoiseHit();

    /// Is a flat spillover hit?
    bool isFlatSpilloverHit();

    /// Is a xtalk hit?
    bool isXTalkHit();

    /// Is a el noise hit?
    bool isElNoiseHit();

    /// Is a machine background ?
    bool isMachineBkgHit();

    /// Is hit firing the frontend ?
    bool isHitFiring();

    friend std::ostream& operator<<( std::ostream& str, const MCMuonHitHistory& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int m_hitHistory; ///< hit history bit pattern

  }; // class MCMuonHitHistory

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::MCMuonHitHistory::fillStream( std::ostream& s ) const {
  s << "{ "
    << "hitHistory :	" << m_hitHistory << std::endl
    << " }";
  return s;
}

inline bool LHCb::MCMuonHitHistory::hasFired() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskFiredHitHistory,
                         PackMCMuonDigitHistory::shiftFiredHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setFiredFlag( unsigned int firedHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftFiredHitHistory,
                  PackMCMuonDigitHistory::maskFiredHitHistory, firedHit );
}

inline bool LHCb::MCMuonHitHistory::isHitOutGeoAccemtance() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskGeoAcceptanceHitHistory,
                         PackMCMuonDigitHistory::shiftGeoAcceptanceHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setGeometricalNotAcceptedHitHistory( unsigned int geoOutHit ) {

  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftGeoAcceptanceHitHistory,
                  PackMCMuonDigitHistory::maskGeoAcceptanceHitHistory, geoOutHit );
}

inline bool LHCb::MCMuonHitHistory::isHitInDeadtime() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskDeadtimeHitHistory,
                         PackMCMuonDigitHistory::shiftDeadtimeHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setHitInDeadtime( unsigned int deadtimeHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftDeadtimeHitHistory,
                  PackMCMuonDigitHistory::maskDeadtimeHitHistory, deadtimeHit );
}

inline bool LHCb::MCMuonHitHistory::hasTimeJittered() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskJitteredHitHistory,
                         PackMCMuonDigitHistory::shiftJitteredHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setHitOutByTimeJitter( unsigned int jitteredHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftJitteredHitHistory,
                  PackMCMuonDigitHistory::maskJitteredHitHistory, jitteredHit );
}

inline bool LHCb::MCMuonHitHistory::isOutForTimeAdjustment() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskAdjustmentHitHistory,
                         PackMCMuonDigitHistory::shiftAdjustmentHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setHitOutByTimeAdjustment( unsigned int timeAdjustedHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftAdjustmentHitHistory,
                  PackMCMuonDigitHistory::maskAdjustmentHitHistory, timeAdjustedHit );
}

inline void LHCb::MCMuonHitHistory::setHitInByTimeAdjustment( unsigned int adjustedJitteredHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftAdjustmentJitterHitHistory,
                  PackMCMuonDigitHistory::maskAdjustmentJitterHitHistory, adjustedJitteredHit );
}

inline bool LHCb::MCMuonHitHistory::isInForTimeAdjustment() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskAdjustmentJitterHitHistory,
                         PackMCMuonDigitHistory::shiftAdjustmentJitterHitHistory ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isKilledByChamberInefficiency() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskChamberIneffHitHistory,
                         PackMCMuonDigitHistory::shiftChamberIneffHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setKilledByChamberInefficiencyHit( unsigned int killedChamberIneff ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftChamberIneffHitHistory,
                  PackMCMuonDigitHistory::maskChamberIneffHitHistory, killedChamberIneff );
}

inline void LHCb::MCMuonHitHistory::setBXOfHit( unsigned int BXID ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftBXbelongOfHitHistory,
                  PackMCMuonDigitHistory::maskBXbelongOfHitHistory, BXID );
}

inline unsigned int LHCb::MCMuonHitHistory::BX() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) );
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInCurrentEvent() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::CURRENT ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevEvent() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::PREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevEvent() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::TWOPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevPrevEvent() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::THREEPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevPrevPrevEvent() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::FOURPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitInDialogDeadtime() {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskDialogDeadtimeHitHistory,
                         PackMCMuonDigitHistory::shiftDialogDeadtimeHitHistory ) != 0;
}

inline void LHCb::MCMuonHitHistory::setKilledByDialogDeadtimeHit( unsigned int killedChamberIneff ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftDialogDeadtimeHitHistory,
                  PackMCMuonDigitHistory::maskDialogDeadtimeHitHistory, killedChamberIneff );
}

inline void LHCb::MCMuonHitHistory::setNatureOfHit( unsigned int NatureID ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftNatureOfHitHistory,
                  PackMCMuonDigitHistory::maskNatureOfHitHistory, NatureID );
}

inline unsigned int LHCb::MCMuonHitHistory::natureOfHit() {

  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) );
}

inline bool LHCb::MCMuonHitHistory::isGeantHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::GEANT ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isBackgroundHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::BACKGROUND ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isChamberNoiseHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::CHAMBERNOISE ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isFlatSpilloverHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::FLATSPILLOVER ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isXTalkHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::XTALK ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isElNoiseHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::ELECTRONICNOISE ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isMachineBkgHit() {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::LHCBACKGROUND ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitFiring() {
  return !( isHitOutGeoAccemtance() || isKilledByChamberInefficiency() || isHitInDeadtime() );
}
