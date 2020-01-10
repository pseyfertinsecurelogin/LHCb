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
    MCMuonHitHistory() = default;

    /// Default Destructor
    virtual ~MCMuonHitHistory() = default;

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// has the hit fired the channel?
    [[nodiscard]] bool hasFired() const;

    /// set firing  flag in the hitHistory word
    MCMuonHitHistory& setFiredFlag( unsigned int firedHit );

    /// is the hit out of geometrical acceptance?
    [[nodiscard]] bool isHitOutGeoAccemtance() const;

    /// set out of geometrical acceptance  flag in the hitHistory word
    MCMuonHitHistory& setGeometricalNotAcceptedHitHistory( unsigned int geoOutHit );

    /// is the hit killed by deadtime?
    [[nodiscard]] bool isHitInDeadtime() const;

    /// set the deadtime flag in the hitHistory word
    MCMuonHitHistory& setHitInDeadtime( unsigned int deadtimeHit );

    /// has the hit time jittered outside the time window?
    [[nodiscard]] bool hasTimeJittered() const;

    /// set out of gate by time jitter  flag in the hitHistory word
    MCMuonHitHistory& setHitOutByTimeJitter( unsigned int jitteredHit );

    /// is the hit out after time adjustement ?
    [[nodiscard]] bool isOutForTimeAdjustment() const;

    /// set out of gate after time adjustment flag in the hitHistory word
    MCMuonHitHistory& setHitOutByTimeAdjustment( unsigned int timeAdjustedHit );

    /// set  again in gate by time adjustemt after out by time jitter or in for a hit with TOF out--- flag in the
    /// hitHistory word
    MCMuonHitHistory& setHitInByTimeAdjustment( unsigned int adjustedJitteredHit );

    /// is the hit in after time adjustement while it was ouside for TOF or after timejitter?
    [[nodiscard]] bool isInForTimeAdjustment() const;

    /// has the hit been killed by chamber inefficiency  ?
    [[nodiscard]] bool isKilledByChamberInefficiency() const;

    /// set the killed by chamber inefficiency flag in the hitHistory word
    MCMuonHitHistory& setKilledByChamberInefficiencyHit( unsigned int killedChamberIneff );

    /// set the BX ID of hit in the hitHistory word
    MCMuonHitHistory& setBXOfHit( unsigned int BXID );

    /// in which BX the hit has originated ?
    [[nodiscard]] unsigned int BX() const;

    /// is the hit originated in the current BX ?
    [[nodiscard]] bool isHitOriginatedInCurrentEvent() const;

    /// is the hit originated in the current BX ?
    [[nodiscard]] bool isHitOriginatedInPrevEvent() const;

    /// is the hit originated in the current BX ?
    [[nodiscard]] bool isHitOriginatedInPrevPrevEvent() const;

    /// is the hit originated in the current BX ?
    [[nodiscard]] bool isHitOriginatedInPrevPrevPrevEvent() const;

    /// is the hit originated in the current BX ?
    [[nodiscard]] bool isHitOriginatedInPrevPrevPrevPrevEvent() const;

    /// is the hit in dialog deadtime ?
    [[nodiscard]] bool isHitInDialogDeadtime() const;

    /// set the killed by dilaog deadtime flag in the hitHistory word
    MCMuonHitHistory& setKilledByDialogDeadtimeHit( unsigned int killedChamberIneff );

    /// set the nature of hit in the hitHistory word
    MCMuonHitHistory& setNatureOfHit( unsigned int NatureID );

    /// which is the nature of the hit?
    [[nodiscard]] unsigned int natureOfHit() const;

    /// Is a GEANT hit?
    [[nodiscard]] bool isGeantHit() const;

    /// Is a low energy background hit?
    [[nodiscard]] bool isBackgroundHit() const;

    /// Is a chamber noise hit?
    [[nodiscard]] bool isChamberNoiseHit() const;

    /// Is a flat spillover hit?
    [[nodiscard]] bool isFlatSpilloverHit() const;

    /// Is a xtalk hit?
    [[nodiscard]] bool isXTalkHit() const;

    /// Is a el noise hit?
    [[nodiscard]] bool isElNoiseHit() const;

    /// Is a machine background ?
    [[nodiscard]] bool isMachineBkgHit() const;

    /// Is hit firing the frontend ?
    [[nodiscard]] bool isHitFiring() const;

    friend std::ostream& operator<<( std::ostream& str, const MCMuonHitHistory& obj ) { return obj.fillStream( str ); }

  private:
    unsigned int m_hitHistory{0}; ///< hit history bit pattern

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

inline bool LHCb::MCMuonHitHistory::hasFired() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskFiredHitHistory,
                         PackMCMuonDigitHistory::shiftFiredHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setFiredFlag( unsigned int firedHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftFiredHitHistory,
                  PackMCMuonDigitHistory::maskFiredHitHistory, firedHit );
  return *this;
}

inline bool LHCb::MCMuonHitHistory::isHitOutGeoAccemtance() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskGeoAcceptanceHitHistory,
                         PackMCMuonDigitHistory::shiftGeoAcceptanceHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setGeometricalNotAcceptedHitHistory( unsigned int geoOutHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftGeoAcceptanceHitHistory,
                  PackMCMuonDigitHistory::maskGeoAcceptanceHitHistory, geoOutHit );
  return *this;
}

inline bool LHCb::MCMuonHitHistory::isHitInDeadtime() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskDeadtimeHitHistory,
                         PackMCMuonDigitHistory::shiftDeadtimeHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setHitInDeadtime( unsigned int deadtimeHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftDeadtimeHitHistory,
                  PackMCMuonDigitHistory::maskDeadtimeHitHistory, deadtimeHit );
  return *this;
}

inline bool LHCb::MCMuonHitHistory::hasTimeJittered() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskJitteredHitHistory,
                         PackMCMuonDigitHistory::shiftJitteredHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setHitOutByTimeJitter( unsigned int jitteredHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftJitteredHitHistory,
                  PackMCMuonDigitHistory::maskJitteredHitHistory, jitteredHit );
  return *this;
}

inline bool LHCb::MCMuonHitHistory::isOutForTimeAdjustment() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskAdjustmentHitHistory,
                         PackMCMuonDigitHistory::shiftAdjustmentHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setHitOutByTimeAdjustment( unsigned int timeAdjustedHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftAdjustmentHitHistory,
                  PackMCMuonDigitHistory::maskAdjustmentHitHistory, timeAdjustedHit );
  return *this;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setHitInByTimeAdjustment( unsigned int adjustedJitteredHit ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftAdjustmentJitterHitHistory,
                  PackMCMuonDigitHistory::maskAdjustmentJitterHitHistory, adjustedJitteredHit );
  return *this;
}

inline bool LHCb::MCMuonHitHistory::isInForTimeAdjustment() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskAdjustmentJitterHitHistory,
                         PackMCMuonDigitHistory::shiftAdjustmentJitterHitHistory ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isKilledByChamberInefficiency() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskChamberIneffHitHistory,
                         PackMCMuonDigitHistory::shiftChamberIneffHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory&
LHCb::MCMuonHitHistory::setKilledByChamberInefficiencyHit( unsigned int killedChamberIneff ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftChamberIneffHitHistory,
                  PackMCMuonDigitHistory::maskChamberIneffHitHistory, killedChamberIneff );
  return *this;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setBXOfHit( unsigned int BXID ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftBXbelongOfHitHistory,
                  PackMCMuonDigitHistory::maskBXbelongOfHitHistory, BXID );
  return *this;
}

inline unsigned int LHCb::MCMuonHitHistory::BX() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) );
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInCurrentEvent() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::CURRENT ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevEvent() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::PREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevEvent() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::TWOPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevPrevEvent() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::THREEPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitOriginatedInPrevPrevPrevPrevEvent() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskBXbelongOfHitHistory,
                           PackMCMuonDigitHistory::shiftBXbelongOfHitHistory ) == MuonBXFlag::FOURPREV ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitInDialogDeadtime() const {
  return Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskDialogDeadtimeHitHistory,
                         PackMCMuonDigitHistory::shiftDialogDeadtimeHitHistory ) != 0;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setKilledByDialogDeadtimeHit( unsigned int killedChamberIneff ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftDialogDeadtimeHitHistory,
                  PackMCMuonDigitHistory::maskDialogDeadtimeHitHistory, killedChamberIneff );
  return *this;
}

inline LHCb::MCMuonHitHistory& LHCb::MCMuonHitHistory::setNatureOfHit( unsigned int NatureID ) {
  Packer::setBit( m_hitHistory, PackMCMuonDigitHistory::shiftNatureOfHitHistory,
                  PackMCMuonDigitHistory::maskNatureOfHitHistory, NatureID );
  return *this;
}

inline unsigned int LHCb::MCMuonHitHistory::natureOfHit() const {

  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) );
}

inline bool LHCb::MCMuonHitHistory::isGeantHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::GEANT ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isBackgroundHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::BACKGROUND ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isChamberNoiseHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::CHAMBERNOISE ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isFlatSpilloverHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::FLATSPILLOVER ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isXTalkHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::XTALK ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isElNoiseHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::ELECTRONICNOISE ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isMachineBkgHit() const {
  return ( Packer::getBit( m_hitHistory, PackMCMuonDigitHistory::maskNatureOfHitHistory,
                           PackMCMuonDigitHistory::shiftNatureOfHitHistory ) == MuonOriginFlag::LHCBACKGROUND ) != 0;
}

inline bool LHCb::MCMuonHitHistory::isHitFiring() const {
  return !( isHitOutGeoAccemtance() || isKilledByChamberInefficiency() || isHitInDeadtime() );
}
