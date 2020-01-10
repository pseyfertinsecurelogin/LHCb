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
#include "Event/PackMCMuonDigitInfo.h"
#include "Kernel/Packer.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  /** @class MCMuonDigitInfo MCMuonDigitInfo.h
   *
   * This class permits to trace back the history of a logical channel: the
   * origin of the hit that has fired it or the reason why the digit was not
   * fired (inefficiency, time resolution, deadtime etc.). In the latter case the
   * correspondig MuonDigit does not exist.
   *
   * @author Alessia Satta
   *
   */

  class MCMuonDigitInfo final {
  public:
    /// Default Constructor
    MCMuonDigitInfo() = default;

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// Is the earliest a GEANT hit?
    [[nodiscard]] bool isGeantHit() const;

    /// set the nature of hit flag in the DIGITINFO word
    MCMuonDigitInfo& setNatureHit( unsigned int natureOfHit );

    /// Is the earliest a low energy background hit?
    [[nodiscard]] bool isBackgroundHit() const;

    /// Is the earliest a chamber noise hit?
    [[nodiscard]] bool isChamberNoiseHit() const;

    /// Is the earliest a flat spillover hit?
    [[nodiscard]] bool isFlatSpilloverHit() const;

    /// Is the earliest a xtalk hit?
    [[nodiscard]] bool isXTalkHit() const;

    /// Is the earliest a el noise hit?
    [[nodiscard]] bool isElNoiseHit() const;

    /// Is the earliest a machine background hit?
    [[nodiscard]] bool isMachineBkgHit() const;

    /// which is the nature of the earliest  hit?
    [[nodiscard]] unsigned int natureOfHit() const;

    /// retrieve the second part of the ifno word
    [[nodiscard]] unsigned int secondPart() const;

    /// set the second part of the DIGITINFO word
    MCMuonDigitInfo& setSecondPart( unsigned int secondPart );

    /// Does earliest hit belong to the current event?
    [[nodiscard]] bool doesFiringHitBelongToCurrentEvent() const;

    /// set the BX ID flag in the DIGITINFO word
    MCMuonDigitInfo& setBXIDFlagHit( unsigned int BXIDFlag );

    /// which is the BX of the earliest hit?
    [[nodiscard]] unsigned int BX() const;

    /// Does earliest hit belong to the previous event?
    [[nodiscard]] bool doesFiringHitBelongToPrevEvent() const;

    /// Does earliest hit belong to the previous previous event?
    [[nodiscard]] bool doesFiringHitBelongToPrevPrevEvent() const;

    /// Does earliest hit belong to the previous previous previous event?
    [[nodiscard]] bool doesFiringHitBelongToPrevPrevPrevEvent() const;

    /// Does earliest hit belong to the previous previous previous previous event?
    [[nodiscard]] bool doesFiringHitBelongToPrevPrevPrevPrevEvent() const;

    /// is the digit fired ?
    [[nodiscard]] bool isAlive() const;

    /// set the fired flag in the DIGITINFO word
    MCMuonDigitInfo& setAliveDigit( unsigned int digitFired );

    /// is the digit in the deadtime?
    [[nodiscard]] bool isInDeadTime() const;

    /// set the killed by deadtime flag in the DIGITINFO word
    MCMuonDigitInfo& setDeadtimeDigit( unsigned int deadtimeKilled );

    /// is the digit in the dialog deadtime?
    [[nodiscard]] bool isInDialogDeadTime() const;

    /// set the killed by deadtime flag in the DIGITINFO word
    MCMuonDigitInfo& setDialogDeadtimeDigit( unsigned int deadtimeKilled );

    /// is the digit in chamber inefficiency?
    [[nodiscard]] bool isDeadForChamberInefficiency() const;

    /// set the killed by chamber inefficiency flag in the DIGITINFO word
    MCMuonDigitInfo& setChamberInefficiencyDigit( unsigned int chamberIneff );

    /// is the digit killed by time jitter?
    [[nodiscard]] bool isDeadByTimeJitter() const;

    /// set the killed by time jitter flag in the DIGITINFO word
    MCMuonDigitInfo& setTimeJitteredDigit( unsigned int timeJitterKilled );

    /// is the digit killed by time adjustment?
    [[nodiscard]] bool isDeadByTimeAdjustment() const;

    /// set the killed by time adjustment flag in the DIGITINFO word
    MCMuonDigitInfo& setTimeAdjDigit( unsigned int timeAdjustmentKilled );

    /// is the digit alive due to time adjustment?
    [[nodiscard]] bool isAliveByTimeAdjustment() const;

    /// set the alive due to  time adjustment flag in the DIGITINFO word
    MCMuonDigitInfo& setAliveTimeAdjDigit( unsigned int timeAdjustmentKilled );

    /// is the digit killed by geometry acceptance?
    [[nodiscard]] bool isDeadByGeometry() const;

    /// set the killed by geometry inefficiency flag in the DIGITINFO word
    MCMuonDigitInfo& setGeometryInefficiency( unsigned int geometryIneffKilled );

    /// Retrieve const  Packed information of the origin of the hit   generating the digit, the earliest, and the fate
    /// of the digit
    [[nodiscard]] unsigned int DigitInfo() const;

    /// Update  Packed information of the origin of the hit   generating the digit, the earliest, and the fate of the
    /// digit
    MCMuonDigitInfo& setDigitInfo( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const MCMuonDigitInfo& obj ) { return obj.fillStream( str ); }

  private:
    unsigned int m_DigitInfo{0}; ///< Packed information of the origin of the hit   generating the digit, the earliest,
                                 ///< and the fate of the digit

  }; // class MCMuonDigitInfo

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline std::ostream& LHCb::MCMuonDigitInfo::fillStream( std::ostream& s ) const {
  s << "{ "
    << "DigitInfo :	" << m_DigitInfo << std::endl
    << " }";
  return s;
}

inline unsigned int LHCb::MCMuonDigitInfo::DigitInfo() const { return m_DigitInfo; }

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setDigitInfo( unsigned int value ) {
  m_DigitInfo = value;
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isGeantHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::GEANT ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setNatureHit( unsigned int natureOfHit ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftOriginDigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                  natureOfHit );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isBackgroundHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::BACKGROUND ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isChamberNoiseHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::CHAMBERNOISE ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isFlatSpilloverHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::FLATSPILLOVER ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isXTalkHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::XTALK ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isElNoiseHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::ELECTRONICNOISE ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isMachineBkgHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::LHCBACKGROUND ) != 0;
}

inline unsigned int LHCb::MCMuonDigitInfo::natureOfHit() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) );
}

inline unsigned int LHCb::MCMuonDigitInfo::secondPart() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskSecondPart, PackMCMuonDigitInfo::shiftSecondPart ) );
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setSecondPart( unsigned int secondPart ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftSecondPart, PackMCMuonDigitInfo::maskSecondPart, secondPart );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToCurrentEvent() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::CURRENT ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setBXIDFlagHit( unsigned int BXIDFlag ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftBXIDDigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                  BXIDFlag );
  return *this;
}

inline unsigned int LHCb::MCMuonDigitInfo::BX() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo, PackMCMuonDigitInfo::shiftBXIDDigitInfo );
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevEvent() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::PREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevEvent() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::TWOPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevPrevEvent() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::THREEPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevPrevPrevEvent() const {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::FOURPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isAlive() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskAliveDigitInfo,
                         PackMCMuonDigitInfo::shiftAliveDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setAliveDigit( unsigned int digitFired ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftAliveDigitInfo, PackMCMuonDigitInfo::maskAliveDigitInfo,
                  digitFired );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isInDeadTime() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskDeadtimeDigitInfo,
                         PackMCMuonDigitInfo::shiftDeadtimeDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setDeadtimeDigit( unsigned int deadtimeKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftDeadtimeDigitInfo, PackMCMuonDigitInfo::maskDeadtimeDigitInfo,
                  deadtimeKilled );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isInDialogDeadTime() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskDialogDeadtimeDigitInfo,
                         PackMCMuonDigitInfo::shiftDialogDeadtimeDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setDialogDeadtimeDigit( unsigned int deadtimeKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftDialogDeadtimeDigitInfo,
                  PackMCMuonDigitInfo::maskDialogDeadtimeDigitInfo, deadtimeKilled );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isDeadForChamberInefficiency() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskChamberInefficiencyDigitInfo,
                         PackMCMuonDigitInfo::shiftChamberInefficiencyDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setChamberInefficiencyDigit( unsigned int chamberIneff ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftChamberInefficiencyDigitInfo,
                  PackMCMuonDigitInfo::maskChamberInefficiencyDigitInfo, chamberIneff );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isDeadByTimeJitter() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskTimeJitterDigitInfo,
                         PackMCMuonDigitInfo::shiftTimeJitterDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setTimeJitteredDigit( unsigned int timeJitterKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftTimeJitterDigitInfo,
                  PackMCMuonDigitInfo::maskTimeJitterDigitInfo, timeJitterKilled );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isDeadByTimeAdjustment() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskTimeAdjustmentDigitInfo,
                         PackMCMuonDigitInfo::shiftTimeAdjustmentDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setTimeAdjDigit( unsigned int timeAdjustmentKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftTimeAdjustmentDigitInfo,
                  PackMCMuonDigitInfo::maskTimeAdjustmentDigitInfo, timeAdjustmentKilled );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isAliveByTimeAdjustment() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskAliveTimeAdjustmentDigitInfo,
                         PackMCMuonDigitInfo::shiftAliveTimeAdjustmentDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setAliveTimeAdjDigit( unsigned int timeAdjustmentKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftAliveTimeAdjustmentDigitInfo,
                  PackMCMuonDigitInfo::maskAliveTimeAdjustmentDigitInfo, timeAdjustmentKilled );
  return *this;
}

inline bool LHCb::MCMuonDigitInfo::isDeadByGeometry() const {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskGeometryAcceptanceDigitInfo,
                         PackMCMuonDigitInfo::shiftGeometryAcceptanceDigitInfo ) != 0;
}

inline LHCb::MCMuonDigitInfo& LHCb::MCMuonDigitInfo::setGeometryInefficiency( unsigned int geometryIneffKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftGeometryAcceptanceDigitInfo,
                  PackMCMuonDigitInfo::maskGeometryAcceptanceDigitInfo, geometryIneffKilled );
  return *this;
}
