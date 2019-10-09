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

  class MCMuonDigitInfo {
  public:
    /// Default Constructor
    MCMuonDigitInfo() : m_DigitInfo( 0 ) {}

    /// Default Destructor
    virtual ~MCMuonDigitInfo() {}

    /// Fill the ASCII output stream
    virtual std::ostream& fillStream( std::ostream& s ) const;

    /// Is the earliest a GEANT hit?
    bool isGeantHit();

    /// set the nature of hit flag in the DIGITINFO word
    void setNatureHit( unsigned int natureOfHit );

    /// Is the earliest a low energy background hit?
    bool isBackgroundHit();

    /// Is the earliest a chamber noise hit?
    bool isChamberNoiseHit();

    /// Is the earliest a flat spillover hit?
    bool isFlatSpilloverHit();

    /// Is the earliest a xtalk hit?
    bool isXTalkHit();

    /// Is the earliest a el noise hit?
    bool isElNoiseHit();

    /// Is the earliest a machine background hit?
    bool isMachineBkgHit();

    /// which is the nature of the earliest  hit?
    unsigned int natureOfHit();

    /// retrieve the second part of the ifno word
    unsigned int secondPart();

    /// set the second part of the DIGITINFO word
    void setSecondPart( unsigned int secondPart );

    /// Does earliest hit belong to the current event?
    bool doesFiringHitBelongToCurrentEvent();

    /// set the BX ID flag in the DIGITINFO word
    void setBXIDFlagHit( unsigned int BXIDFlag );

    /// which is the BX of the earliest hit?
    unsigned int BX();

    /// Does earliest hit belong to the previous event?
    bool doesFiringHitBelongToPrevEvent();

    /// Does earliest hit belong to the previous previous event?
    bool doesFiringHitBelongToPrevPrevEvent();

    /// Does earliest hit belong to the previous previous previous event?
    bool doesFiringHitBelongToPrevPrevPrevEvent();

    /// Does earliest hit belong to the previous previous previous previous event?
    bool doesFiringHitBelongToPrevPrevPrevPrevEvent();

    /// is the digit fired ?
    bool isAlive();

    /// set the fired flag in the DIGITINFO word
    void setAliveDigit( unsigned int digitFired );

    /// is the digit in the deadtime?
    bool isInDeadTime();

    /// set the killed by deadtime flag in the DIGITINFO word
    void setDeadtimeDigit( unsigned int deadtimeKilled );

    /// is the digit in the dialog deadtime?
    bool isInDialogDeadTime();

    /// set the killed by deadtime flag in the DIGITINFO word
    void setDialogDeadtimeDigit( unsigned int deadtimeKilled );

    /// is the digit in chamber inefficiency?
    bool isDeadForChamberInefficiency();

    /// set the killed by chamber inefficiency flag in the DIGITINFO word
    void setChamberInefficiencyDigit( unsigned int chamberIneff );

    /// is the digit killed by time jitter?
    bool isDeadByTimeJitter();

    /// set the killed by time jitter flag in the DIGITINFO word
    void setTimeJitteredDigit( unsigned int timeJitterKilled );

    /// is the digit killed by time adjustment?
    bool isDeadByTimeAdjustment();

    /// set the killed by time adjustment flag in the DIGITINFO word
    void setTimeAdjDigit( unsigned int timeAdjustmentKilled );

    /// is the digit alive due to time adjustment?
    bool isAliveByTimeAdjustment();

    /// set the alive due to  time adjustment flag in the DIGITINFO word
    void setAliveTimeAdjDigit( unsigned int timeAdjustmentKilled );

    /// is the digit killed by geometry acceptance?
    bool isDeadByGeometry();

    /// set the killed by geometry inefficiency flag in the DIGITINFO word
    void setGeometryInefficiency( unsigned int geometryIneffKilled );

    /// Retrieve const  Packed information of the origin of the hit   generating the digit, the earliest, and the fate
    /// of the digit
    unsigned int DigitInfo() const;

    /// Update  Packed information of the origin of the hit   generating the digit, the earliest, and the fate of the
    /// digit
    void setDigitInfo( unsigned int value );

    friend std::ostream& operator<<( std::ostream& str, const MCMuonDigitInfo& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    unsigned int m_DigitInfo; ///< Packed information of the origin of the hit   generating the digit, the earliest, and
                              ///< the fate of the digit

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

inline void LHCb::MCMuonDigitInfo::setDigitInfo( unsigned int value ) { m_DigitInfo = value; }

inline bool LHCb::MCMuonDigitInfo::isGeantHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::GEANT ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setNatureHit( unsigned int natureOfHit ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftOriginDigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                  natureOfHit );
}

inline bool LHCb::MCMuonDigitInfo::isBackgroundHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::BACKGROUND ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isChamberNoiseHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::CHAMBERNOISE ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isFlatSpilloverHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::FLATSPILLOVER ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isXTalkHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::XTALK ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isElNoiseHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::ELECTRONICNOISE ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isMachineBkgHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) == MuonOriginFlag::LHCBACKGROUND ) != 0;
}

inline unsigned int LHCb::MCMuonDigitInfo::natureOfHit() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskOriginDigitInfo,
                           PackMCMuonDigitInfo::shiftOriginDigitInfo ) );
}

inline unsigned int LHCb::MCMuonDigitInfo::secondPart() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskSecondPart, PackMCMuonDigitInfo::shiftSecondPart ) );
}

inline void LHCb::MCMuonDigitInfo::setSecondPart( unsigned int secondPart ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftSecondPart, PackMCMuonDigitInfo::maskSecondPart, secondPart );
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToCurrentEvent() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::CURRENT ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setBXIDFlagHit( unsigned int BXIDFlag ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftBXIDDigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                  BXIDFlag );
}

inline unsigned int LHCb::MCMuonDigitInfo::BX() {
  return (
      Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo, PackMCMuonDigitInfo::shiftBXIDDigitInfo ) );
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevEvent() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::PREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevEvent() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::TWOPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevPrevEvent() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::THREEPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::doesFiringHitBelongToPrevPrevPrevPrevEvent() {
  return ( Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskBXIDDigitInfo,
                           PackMCMuonDigitInfo::shiftBXIDDigitInfo ) == MuonBXFlag::FOURPREV ) != 0;
}

inline bool LHCb::MCMuonDigitInfo::isAlive() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskAliveDigitInfo,
                         PackMCMuonDigitInfo::shiftAliveDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setAliveDigit( unsigned int digitFired ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftAliveDigitInfo, PackMCMuonDigitInfo::maskAliveDigitInfo,
                  digitFired );
}

inline bool LHCb::MCMuonDigitInfo::isInDeadTime() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskDeadtimeDigitInfo,
                         PackMCMuonDigitInfo::shiftDeadtimeDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setDeadtimeDigit( unsigned int deadtimeKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftDeadtimeDigitInfo, PackMCMuonDigitInfo::maskDeadtimeDigitInfo,
                  deadtimeKilled );
}

inline bool LHCb::MCMuonDigitInfo::isInDialogDeadTime() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskDialogDeadtimeDigitInfo,
                         PackMCMuonDigitInfo::shiftDialogDeadtimeDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setDialogDeadtimeDigit( unsigned int deadtimeKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftDialogDeadtimeDigitInfo,
                  PackMCMuonDigitInfo::maskDialogDeadtimeDigitInfo, deadtimeKilled );
}

inline bool LHCb::MCMuonDigitInfo::isDeadForChamberInefficiency() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskChamberInefficiencyDigitInfo,
                         PackMCMuonDigitInfo::shiftChamberInefficiencyDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setChamberInefficiencyDigit( unsigned int chamberIneff ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftChamberInefficiencyDigitInfo,
                  PackMCMuonDigitInfo::maskChamberInefficiencyDigitInfo, chamberIneff );
}

inline bool LHCb::MCMuonDigitInfo::isDeadByTimeJitter() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskTimeJitterDigitInfo,
                         PackMCMuonDigitInfo::shiftTimeJitterDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setTimeJitteredDigit( unsigned int timeJitterKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftTimeJitterDigitInfo,
                  PackMCMuonDigitInfo::maskTimeJitterDigitInfo, timeJitterKilled );
}

inline bool LHCb::MCMuonDigitInfo::isDeadByTimeAdjustment() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskTimeAdjustmentDigitInfo,
                         PackMCMuonDigitInfo::shiftTimeAdjustmentDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setTimeAdjDigit( unsigned int timeAdjustmentKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftTimeAdjustmentDigitInfo,
                  PackMCMuonDigitInfo::maskTimeAdjustmentDigitInfo, timeAdjustmentKilled );
}

inline bool LHCb::MCMuonDigitInfo::isAliveByTimeAdjustment() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskAliveTimeAdjustmentDigitInfo,
                         PackMCMuonDigitInfo::shiftAliveTimeAdjustmentDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setAliveTimeAdjDigit( unsigned int timeAdjustmentKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftAliveTimeAdjustmentDigitInfo,
                  PackMCMuonDigitInfo::maskAliveTimeAdjustmentDigitInfo, timeAdjustmentKilled );
}

inline bool LHCb::MCMuonDigitInfo::isDeadByGeometry() {
  return Packer::getBit( m_DigitInfo, PackMCMuonDigitInfo::maskGeometryAcceptanceDigitInfo,
                         PackMCMuonDigitInfo::shiftGeometryAcceptanceDigitInfo ) != 0;
}

inline void LHCb::MCMuonDigitInfo::setGeometryInefficiency( unsigned int geometryIneffKilled ) {
  Packer::setBit( m_DigitInfo, PackMCMuonDigitInfo::shiftGeometryAcceptanceDigitInfo,
                  PackMCMuonDigitInfo::maskGeometryAcceptanceDigitInfo, geometryIneffKilled );
}
