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
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_STTELL1EventInfo = 9011;

  // Namespace for locations in TDS
  namespace STTELL1EventInfoLocation {
    inline const std::string ITEventInfo = "Raw/IT/EventInfo";
    inline const std::string TTEventInfo = "Raw/TT/EventInfo";
    inline const std::string UTEventInfo = "Raw/UT/EventInfo";
  } // namespace STTELL1EventInfoLocation

  /** @class STTELL1EventInfo STTELL1EventInfo.h
   *
   * Event info from a TELL1 board
   *
   * @author Mathias Knecht
   *
   */

  class STTELL1EventInfo : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of STTELL1EventInfo
    typedef KeyedContainer<STTELL1EventInfo, Containers::HashMap> Container;

    /// Default Constructor
    STTELL1EventInfo()
        : m_word0( 0 )
        , m_word1( 0 )
        , m_word2( 0 )
        , m_word3( 0 )
        , m_word4( 0 )
        , m_word5( 0 )
        , m_word6( 0 )
        , m_word7( 0 )
        , m_sourceID( 100 ) {}

    /// Default Destructor
    virtual ~STTELL1EventInfo() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// find pcn for value for this beetle
    unsigned int findPCN( const unsigned int beetle ) const;

    /// find iheader for each beetle
    unsigned int findIHeader( const unsigned int beetle ) const;

    /// majority vote on pcn
    unsigned int pcnVote() const;

    /// checks that pcn values are the same
    bool pcnConsistency() const;

    /// test for event loss on link
    bool noEvent( const unsigned int ) const;

    /// test for clock loss on link
    bool noClock( const unsigned int ) const;

    /// test if link has link loss
    bool TLKLinkLoss( const unsigned int ) const;

    /// test if link has synch RAM Full error
    bool syncRAMFull( const unsigned int ) const;

    /// test if link has synch event size error
    bool syncEvtSizeError( const unsigned int ) const;

    /// test if link enabled
    bool linkEnabled( const unsigned int ) const;

    /// true if pp has enabled links
    bool enabled() const;

    /// Print in a human readable way
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Word 0 of Event Info block
    unsigned int word0() const;

    /// Update  Word 0 of Event Info block
    void setWord0( unsigned int value );

    /// Retrieve Bunch counter
    short bCnt() const;

    /// Update Bunch counter
    void setBCnt( short value );

    /// Retrieve ID of the sub-detector
    unsigned char DetectorID() const;

    /// Update ID of the sub-detector
    void setDetectorID( unsigned char value );

    /// Retrieve List of the Banks sent by the TELL1 (Internal use)
    unsigned char BankList() const;

    /// Update List of the Banks sent by the TELL1 (Internal use)
    void setBankList( unsigned char value );

    /// Retrieve For internal use
    unsigned char EventInformation() const;

    /// Update For internal use
    void setEventInformation( unsigned char value );

    /// Retrieve const  Word 1 of Event Info block
    unsigned int word1() const;

    /// Update  Word 1 of Event Info block
    void setWord1( unsigned int value );

    /// Retrieve L0 Event ID
    int L0EvID() const;

    /// Update L0 Event ID
    void setL0EvID( int value );

    /// Retrieve const  Word 2 of Event Info block
    unsigned int word2() const;

    /// Update  Word 2 of Event Info block
    void setWord2( unsigned int value );

    /// Retrieve PCN from Beetle 0
    unsigned char pcn() const;

    /// Update PCN from Beetle 0
    void setPCN( unsigned char value );

    /// Retrieve Indicates what processing has been applied
    unsigned char ProcessInfo() const;

    /// Update Indicates what processing has been applied
    void setProcessInfo( unsigned char value );

    /// Retrieve Reserved bits for later use, =0x8A8A
    short R1() const;

    /// Update Reserved bits for later use, =0x8A8A
    void setR1( short value );

    /// Retrieve const  Word 3 of Event Info block
    unsigned int word3() const;

    /// Update  Word 3 of Event Info block
    void setWord3( unsigned int value );

    /// Retrieve Indicates that no data has been received, one bit per link
    unsigned char OptLnkNoEvt() const;

    /// Update Indicates that no data has been received, one bit per link
    void setOptLnkNoEvt( unsigned char value );

    /// Retrieve Indicates if link is disabled by ECS, one bit per link
    unsigned char OptLnkDisable() const;

    /// Update Indicates if link is disabled by ECS, one bit per link
    void setOptLnkDisable( unsigned char value );

    /// Retrieve Set if more or less than 35 words are sent paer event (one bit per link)
    unsigned char SyncEvtSizeError() const;

    /// Update Set if more or less than 35 words are sent paer event (one bit per link)
    void setSyncEvtSizeError( unsigned char value );

    /// Retrieve Error on reception, bit set if the link is not plugged (one bit per link)
    unsigned char tlkLnkLoss() const;

    /// Update Error on reception, bit set if the link is not plugged (one bit per link)
    void setTLKLnkLoss( unsigned char value );

    /// Retrieve Overflow on input sync RAM (one bit per link)
    unsigned char SyncRAMFull() const;

    /// Update Overflow on input sync RAM (one bit per link)
    void setSyncRAMFull( unsigned char value );

    /// Retrieve Address of the PP-FPGA (between 0 and 3)
    unsigned char ChipAddr() const;

    /// Update Address of the PP-FPGA (between 0 and 3)
    void setChipAddr( unsigned char value );

    /// Retrieve const  Word 4 of Event Info block
    unsigned int word4() const;

    /// Update  Word 4 of Event Info block
    void setWord4( unsigned int value );

    /// Retrieve Indicates if the ADC value is between low and high thresholds, one per Analog Link
    int HeaderPseudoError() const;

    /// Update Indicates if the ADC value is between low and high thresholds, one per Analog Link
    void setHeaderPseudoError( int value );

    /// Retrieve Indicates if no clock is received, one bit per link
    unsigned char OptLnkNoClock() const;

    /// Update Indicates if no clock is received, one bit per link
    void setOptLnkNoClock( unsigned char value );

    /// Retrieve Indocates if the PCN among the links are not equal
    unsigned char pcnError() const;

    /// Update Indocates if the PCN among the links are not equal
    void setPCNError( unsigned char value );

    /// Retrieve Reserved bit for later use
    unsigned char R2() const;

    /// Update Reserved bit for later use
    void setR2( unsigned char value );

    /// Retrieve const  Word 5 of Event Info block
    unsigned int word5() const;

    /// Update  Word 5 of Event Info block
    void setWord5( unsigned int value );

    /// Retrieve PCN of the Beetle 0
    unsigned char pcnBeetle0() const;

    /// Update PCN of the Beetle 0
    void setPCNBeetle0( unsigned char value );

    /// Retrieve PCN of the Beetle 1
    unsigned char pcnBeetle1() const;

    /// Update PCN of the Beetle 1
    void setPCNBeetle1( unsigned char value );

    /// Retrieve PCN of the Beetle 2
    unsigned char pcnBeetle2() const;

    /// Update PCN of the Beetle 2
    void setPCNBeetle2( unsigned char value );

    /// Retrieve PCN of the Beetle 3
    unsigned char pcnBeetle3() const;

    /// Update PCN of the Beetle 3
    void setPCNBeetle3( unsigned char value );

    /// Retrieve const  Word 6 of Event Info block
    unsigned int word6() const;

    /// Update  Word 6 of Event Info block
    void setWord6( unsigned int value );

    /// Retrieve PCN of the Beetle 4
    unsigned char pcnBeetle4() const;

    /// Update PCN of the Beetle 4
    void setPCNBeetle4( unsigned char value );

    /// Retrieve PCN of the Beetle 5
    unsigned char pcnBeetle5() const;

    /// Update PCN of the Beetle 5
    void setPCNBeetle5( unsigned char value );

    /// Retrieve Reserved bits for later use =0x8B8B
    short R3() const;

    /// Update Reserved bits for later use =0x8B8B
    void setR3( short value );

    /// Retrieve const  Word 7 of Event Info block
    unsigned int word7() const;

    /// Update  Word 7 of Event Info block
    void setWord7( unsigned int value );

    /// Retrieve I Header of the Beetle 0
    unsigned char iHeaderBeetle0() const;

    /// Update I Header of the Beetle 0
    void setIHeaderBeetle0( unsigned char value );

    /// Retrieve I Header of the Beetle 1
    unsigned char iHeaderBeetle1() const;

    /// Update I Header of the Beetle 1
    void setIHeaderBeetle1( unsigned char value );

    /// Retrieve I Header of the Beetle 2
    unsigned char iHeaderBeetle2() const;

    /// Update I Header of the Beetle 2
    void setIHeaderBeetle2( unsigned char value );

    /// Retrieve I Header of the Beetle 3
    unsigned char iHeaderBeetle3() const;

    /// Update I Header of the Beetle 3
    void setIHeaderBeetle3( unsigned char value );

    /// Retrieve I Header of the Beetle 4
    unsigned char iHeaderBeetle4() const;

    /// Update I Header of the Beetle 4
    void setIHeaderBeetle4( unsigned char value );

    /// Retrieve I Header of the Beetle 5
    unsigned char iHeaderBeetle5() const;

    /// Update I Header of the Beetle 5
    void setIHeaderBeetle5( unsigned char value );

    /// Retrieve Reserved bits for later use
    unsigned char R4() const;

    /// Update Reserved bits for later use
    void setR4( unsigned char value );

    /// Retrieve const  source id of board
    unsigned int sourceID() const;

    /// Update  source id of board
    void setSourceID( unsigned int value );

    /// Offsets of bitfield word0
    enum word0Bits { BCntBits = 0, DetectorIDBits = 12, BankListBits = 16, EventInformationBits = 24 };

    /// Bitmasks for bitfield word0
    enum word0Masks {
      BCntMask             = 0xfffL,
      DetectorIDMask       = 0xf000L,
      BankListMask         = 0xff0000L,
      EventInformationMask = 0xff000000L
    };

    /// Offsets of bitfield word1
    enum word1Bits { L0EvIDBits = 0 };

    /// Bitmasks for bitfield word1
    enum word1Masks { L0EvIDMask = 0xffffffffL };

    /// Offsets of bitfield word2
    enum word2Bits { PCNBits = 0, ProcessInfoBits = 8, R1Bits = 16 };

    /// Bitmasks for bitfield word2
    enum word2Masks { PCNMask = 0xffL, ProcessInfoMask = 0xff00L, R1Mask = 0xffff0000L };

    /// Offsets of bitfield word3
    enum word3Bits {
      OptLnkNoEvtBits      = 0,
      OptLnkDisableBits    = 6,
      SyncEvtSizeErrorBits = 12,
      TLKLnkLossBits       = 18,
      SyncRAMFullBits      = 24,
      ChipAddrBits         = 30
    };

    /// Bitmasks for bitfield word3
    enum word3Masks {
      OptLnkNoEvtMask      = 0x3fL,
      OptLnkDisableMask    = 0xfc0L,
      SyncEvtSizeErrorMask = 0x3f000L,
      TLKLnkLossMask       = 0xfc0000L,
      SyncRAMFullMask      = 0x3f000000L,
      ChipAddrMask         = 0xc0000000L
    };

    /// Offsets of bitfield word4
    enum word4Bits { HeaderPseudoErrorBits = 0, OptLnkNoClockBits = 24, PCNErrorBits = 30, R2Bits = 31 };

    /// Bitmasks for bitfield word4
    enum word4Masks {
      HeaderPseudoErrorMask = 0xffffffL,
      OptLnkNoClockMask     = 0x3f000000L,
      PCNErrorMask          = 0x40000000L,
      R2Mask                = 0x80000000L
    };

    /// Offsets of bitfield word5
    enum word5Bits { PCNBeetle0Bits = 0, PCNBeetle1Bits = 8, PCNBeetle2Bits = 16, PCNBeetle3Bits = 24 };

    /// Bitmasks for bitfield word5
    enum word5Masks {
      PCNBeetle0Mask = 0xffL,
      PCNBeetle1Mask = 0xff00L,
      PCNBeetle2Mask = 0xff0000L,
      PCNBeetle3Mask = 0xff000000L
    };

    /// Offsets of bitfield word6
    enum word6Bits { PCNBeetle4Bits = 0, PCNBeetle5Bits = 8, R3Bits = 16 };

    /// Bitmasks for bitfield word6
    enum word6Masks { PCNBeetle4Mask = 0xffL, PCNBeetle5Mask = 0xff00L, R3Mask = 0xffff0000L };

    /// Offsets of bitfield word7
    enum word7Bits {
      IHeaderBeetle0Bits = 0,
      IHeaderBeetle1Bits = 4,
      IHeaderBeetle2Bits = 8,
      IHeaderBeetle3Bits = 12,
      IHeaderBeetle4Bits = 16,
      IHeaderBeetle5Bits = 20,
      R4Bits             = 24
    };

    /// Bitmasks for bitfield word7
    enum word7Masks {
      IHeaderBeetle0Mask = 0xfL,
      IHeaderBeetle1Mask = 0xf0L,
      IHeaderBeetle2Mask = 0xf00L,
      IHeaderBeetle3Mask = 0xf000L,
      IHeaderBeetle4Mask = 0xf0000L,
      IHeaderBeetle5Mask = 0xf00000L,
      R4Mask             = 0xff000000L
    };

    unsigned int m_word0;    ///< Word 0 of Event Info block
    unsigned int m_word1;    ///< Word 1 of Event Info block
    unsigned int m_word2;    ///< Word 2 of Event Info block
    unsigned int m_word3;    ///< Word 3 of Event Info block
    unsigned int m_word4;    ///< Word 4 of Event Info block
    unsigned int m_word5;    ///< Word 5 of Event Info block
    unsigned int m_word6;    ///< Word 6 of Event Info block
    unsigned int m_word7;    ///< Word 7 of Event Info block
    unsigned int m_sourceID; ///< source id of board

    friend std::ostream& operator<<( std::ostream& str, const STTELL1EventInfo& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// few useful constants
    enum Tell1Const { nPort = 4, nBeetle = 6, nMagic = 0x8E };

  }; // class STTELL1EventInfo

  /// Definition of Keyed Container for STTELL1EventInfo
  typedef KeyedContainer<STTELL1EventInfo, Containers::HashMap> STTELL1EventInfos;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::STTELL1EventInfo::clID() const { return LHCb::STTELL1EventInfo::classID(); }

inline const CLID& LHCb::STTELL1EventInfo::classID() { return CLID_STTELL1EventInfo; }

inline unsigned int LHCb::STTELL1EventInfo::word0() const { return m_word0; }

inline void LHCb::STTELL1EventInfo::setWord0( unsigned int value ) { m_word0 = value; }

inline short LHCb::STTELL1EventInfo::bCnt() const { return (short)( ( m_word0 & BCntMask ) >> BCntBits ); }

inline void LHCb::STTELL1EventInfo::setBCnt( short value ) {
  unsigned int val = (unsigned int)value;
  m_word0 &= ~BCntMask;
  m_word0 |= ( ( ( (unsigned int)val ) << BCntBits ) & BCntMask );
}

inline unsigned char LHCb::STTELL1EventInfo::DetectorID() const {
  return (unsigned char)( ( m_word0 & DetectorIDMask ) >> DetectorIDBits );
}

inline void LHCb::STTELL1EventInfo::setDetectorID( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word0 &= ~DetectorIDMask;
  m_word0 |= ( ( ( (unsigned int)val ) << DetectorIDBits ) & DetectorIDMask );
}

inline unsigned char LHCb::STTELL1EventInfo::BankList() const {
  return (unsigned char)( ( m_word0 & BankListMask ) >> BankListBits );
}

inline void LHCb::STTELL1EventInfo::setBankList( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word0 &= ~BankListMask;
  m_word0 |= ( ( ( (unsigned int)val ) << BankListBits ) & BankListMask );
}

inline unsigned char LHCb::STTELL1EventInfo::EventInformation() const {
  return (unsigned char)( ( m_word0 & EventInformationMask ) >> EventInformationBits );
}

inline void LHCb::STTELL1EventInfo::setEventInformation( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word0 &= ~EventInformationMask;
  m_word0 |= ( ( ( (unsigned int)val ) << EventInformationBits ) & EventInformationMask );
}

inline unsigned int LHCb::STTELL1EventInfo::word1() const { return m_word1; }

inline void LHCb::STTELL1EventInfo::setWord1( unsigned int value ) { m_word1 = value; }

inline int LHCb::STTELL1EventInfo::L0EvID() const { return (int)( ( m_word1 & L0EvIDMask ) >> L0EvIDBits ); }

inline void LHCb::STTELL1EventInfo::setL0EvID( int value ) {
  unsigned int val = (unsigned int)value;
  m_word1 &= ~L0EvIDMask;
  m_word1 |= ( ( ( (unsigned int)val ) << L0EvIDBits ) & L0EvIDMask );
}

inline unsigned int LHCb::STTELL1EventInfo::word2() const { return m_word2; }

inline void LHCb::STTELL1EventInfo::setWord2( unsigned int value ) { m_word2 = value; }

inline unsigned char LHCb::STTELL1EventInfo::pcn() const { return (unsigned char)( ( m_word2 & PCNMask ) >> PCNBits ); }

inline void LHCb::STTELL1EventInfo::setPCN( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word2 &= ~PCNMask;
  m_word2 |= ( ( ( (unsigned int)val ) << PCNBits ) & PCNMask );
}

inline unsigned char LHCb::STTELL1EventInfo::ProcessInfo() const {
  return (unsigned char)( ( m_word2 & ProcessInfoMask ) >> ProcessInfoBits );
}

inline void LHCb::STTELL1EventInfo::setProcessInfo( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word2 &= ~ProcessInfoMask;
  m_word2 |= ( ( ( (unsigned int)val ) << ProcessInfoBits ) & ProcessInfoMask );
}

inline short LHCb::STTELL1EventInfo::R1() const { return (short)( ( m_word2 & R1Mask ) >> R1Bits ); }

inline void LHCb::STTELL1EventInfo::setR1( short value ) {
  unsigned int val = (unsigned int)value;
  m_word2 &= ~R1Mask;
  m_word2 |= ( ( ( (unsigned int)val ) << R1Bits ) & R1Mask );
}

inline unsigned int LHCb::STTELL1EventInfo::word3() const { return m_word3; }

inline void LHCb::STTELL1EventInfo::setWord3( unsigned int value ) { m_word3 = value; }

inline unsigned char LHCb::STTELL1EventInfo::OptLnkNoEvt() const {
  return (unsigned char)( ( m_word3 & OptLnkNoEvtMask ) >> OptLnkNoEvtBits );
}

inline void LHCb::STTELL1EventInfo::setOptLnkNoEvt( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~OptLnkNoEvtMask;
  m_word3 |= ( ( ( (unsigned int)val ) << OptLnkNoEvtBits ) & OptLnkNoEvtMask );
}

inline unsigned char LHCb::STTELL1EventInfo::OptLnkDisable() const {
  return (unsigned char)( ( m_word3 & OptLnkDisableMask ) >> OptLnkDisableBits );
}

inline void LHCb::STTELL1EventInfo::setOptLnkDisable( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~OptLnkDisableMask;
  m_word3 |= ( ( ( (unsigned int)val ) << OptLnkDisableBits ) & OptLnkDisableMask );
}

inline unsigned char LHCb::STTELL1EventInfo::SyncEvtSizeError() const {
  return (unsigned char)( ( m_word3 & SyncEvtSizeErrorMask ) >> SyncEvtSizeErrorBits );
}

inline void LHCb::STTELL1EventInfo::setSyncEvtSizeError( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~SyncEvtSizeErrorMask;
  m_word3 |= ( ( ( (unsigned int)val ) << SyncEvtSizeErrorBits ) & SyncEvtSizeErrorMask );
}

inline unsigned char LHCb::STTELL1EventInfo::tlkLnkLoss() const {
  return (unsigned char)( ( m_word3 & TLKLnkLossMask ) >> TLKLnkLossBits );
}

inline void LHCb::STTELL1EventInfo::setTLKLnkLoss( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~TLKLnkLossMask;
  m_word3 |= ( ( ( (unsigned int)val ) << TLKLnkLossBits ) & TLKLnkLossMask );
}

inline unsigned char LHCb::STTELL1EventInfo::SyncRAMFull() const {
  return (unsigned char)( ( m_word3 & SyncRAMFullMask ) >> SyncRAMFullBits );
}

inline void LHCb::STTELL1EventInfo::setSyncRAMFull( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~SyncRAMFullMask;
  m_word3 |= ( ( ( (unsigned int)val ) << SyncRAMFullBits ) & SyncRAMFullMask );
}

inline unsigned char LHCb::STTELL1EventInfo::ChipAddr() const {
  return (unsigned char)( ( m_word3 & ChipAddrMask ) >> ChipAddrBits );
}

inline void LHCb::STTELL1EventInfo::setChipAddr( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word3 &= ~ChipAddrMask;
  m_word3 |= ( ( ( (unsigned int)val ) << ChipAddrBits ) & ChipAddrMask );
}

inline unsigned int LHCb::STTELL1EventInfo::word4() const { return m_word4; }

inline void LHCb::STTELL1EventInfo::setWord4( unsigned int value ) { m_word4 = value; }

inline int LHCb::STTELL1EventInfo::HeaderPseudoError() const {
  return (int)( ( m_word4 & HeaderPseudoErrorMask ) >> HeaderPseudoErrorBits );
}

inline void LHCb::STTELL1EventInfo::setHeaderPseudoError( int value ) {
  unsigned int val = (unsigned int)value;
  m_word4 &= ~HeaderPseudoErrorMask;
  m_word4 |= ( ( ( (unsigned int)val ) << HeaderPseudoErrorBits ) & HeaderPseudoErrorMask );
}

inline unsigned char LHCb::STTELL1EventInfo::OptLnkNoClock() const {
  return (unsigned char)( ( m_word4 & OptLnkNoClockMask ) >> OptLnkNoClockBits );
}

inline void LHCb::STTELL1EventInfo::setOptLnkNoClock( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word4 &= ~OptLnkNoClockMask;
  m_word4 |= ( ( ( (unsigned int)val ) << OptLnkNoClockBits ) & OptLnkNoClockMask );
}

inline unsigned char LHCb::STTELL1EventInfo::pcnError() const {
  return (unsigned char)( ( m_word4 & PCNErrorMask ) >> PCNErrorBits );
}

inline void LHCb::STTELL1EventInfo::setPCNError( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word4 &= ~PCNErrorMask;
  m_word4 |= ( ( ( (unsigned int)val ) << PCNErrorBits ) & PCNErrorMask );
}

inline unsigned char LHCb::STTELL1EventInfo::R2() const { return (unsigned char)( ( m_word4 & R2Mask ) >> R2Bits ); }

inline void LHCb::STTELL1EventInfo::setR2( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word4 &= ~R2Mask;
  m_word4 |= ( ( ( (unsigned int)val ) << R2Bits ) & R2Mask );
}

inline unsigned int LHCb::STTELL1EventInfo::word5() const { return m_word5; }

inline void LHCb::STTELL1EventInfo::setWord5( unsigned int value ) { m_word5 = value; }

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle0() const {
  return (unsigned char)( ( m_word5 & PCNBeetle0Mask ) >> PCNBeetle0Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle0( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word5 &= ~PCNBeetle0Mask;
  m_word5 |= ( ( ( (unsigned int)val ) << PCNBeetle0Bits ) & PCNBeetle0Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle1() const {
  return (unsigned char)( ( m_word5 & PCNBeetle1Mask ) >> PCNBeetle1Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle1( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word5 &= ~PCNBeetle1Mask;
  m_word5 |= ( ( ( (unsigned int)val ) << PCNBeetle1Bits ) & PCNBeetle1Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle2() const {
  return (unsigned char)( ( m_word5 & PCNBeetle2Mask ) >> PCNBeetle2Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle2( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word5 &= ~PCNBeetle2Mask;
  m_word5 |= ( ( ( (unsigned int)val ) << PCNBeetle2Bits ) & PCNBeetle2Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle3() const {
  return (unsigned char)( ( m_word5 & PCNBeetle3Mask ) >> PCNBeetle3Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle3( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word5 &= ~PCNBeetle3Mask;
  m_word5 |= ( ( ( (unsigned int)val ) << PCNBeetle3Bits ) & PCNBeetle3Mask );
}

inline unsigned int LHCb::STTELL1EventInfo::word6() const { return m_word6; }

inline void LHCb::STTELL1EventInfo::setWord6( unsigned int value ) { m_word6 = value; }

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle4() const {
  return (unsigned char)( ( m_word6 & PCNBeetle4Mask ) >> PCNBeetle4Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle4( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word6 &= ~PCNBeetle4Mask;
  m_word6 |= ( ( ( (unsigned int)val ) << PCNBeetle4Bits ) & PCNBeetle4Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::pcnBeetle5() const {
  return (unsigned char)( ( m_word6 & PCNBeetle5Mask ) >> PCNBeetle5Bits );
}

inline void LHCb::STTELL1EventInfo::setPCNBeetle5( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word6 &= ~PCNBeetle5Mask;
  m_word6 |= ( ( ( (unsigned int)val ) << PCNBeetle5Bits ) & PCNBeetle5Mask );
}

inline short LHCb::STTELL1EventInfo::R3() const { return (short)( ( m_word6 & R3Mask ) >> R3Bits ); }

inline void LHCb::STTELL1EventInfo::setR3( short value ) {
  unsigned int val = (unsigned int)value;
  m_word6 &= ~R3Mask;
  m_word6 |= ( ( ( (unsigned int)val ) << R3Bits ) & R3Mask );
}

inline unsigned int LHCb::STTELL1EventInfo::word7() const { return m_word7; }

inline void LHCb::STTELL1EventInfo::setWord7( unsigned int value ) { m_word7 = value; }

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle0() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle0Mask ) >> IHeaderBeetle0Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle0( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle0Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle0Bits ) & IHeaderBeetle0Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle1() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle1Mask ) >> IHeaderBeetle1Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle1( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle1Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle1Bits ) & IHeaderBeetle1Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle2() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle2Mask ) >> IHeaderBeetle2Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle2( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle2Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle2Bits ) & IHeaderBeetle2Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle3() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle3Mask ) >> IHeaderBeetle3Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle3( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle3Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle3Bits ) & IHeaderBeetle3Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle4() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle4Mask ) >> IHeaderBeetle4Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle4( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle4Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle4Bits ) & IHeaderBeetle4Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::iHeaderBeetle5() const {
  return (unsigned char)( ( m_word7 & IHeaderBeetle5Mask ) >> IHeaderBeetle5Bits );
}

inline void LHCb::STTELL1EventInfo::setIHeaderBeetle5( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~IHeaderBeetle5Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << IHeaderBeetle5Bits ) & IHeaderBeetle5Mask );
}

inline unsigned char LHCb::STTELL1EventInfo::R4() const { return (unsigned char)( ( m_word7 & R4Mask ) >> R4Bits ); }

inline void LHCb::STTELL1EventInfo::setR4( unsigned char value ) {
  unsigned int val = (unsigned int)value;
  m_word7 &= ~R4Mask;
  m_word7 |= ( ( ( (unsigned int)val ) << R4Bits ) & R4Mask );
}

inline unsigned int LHCb::STTELL1EventInfo::sourceID() const { return m_sourceID; }

inline void LHCb::STTELL1EventInfo::setSourceID( unsigned int value ) { m_sourceID = value; }
