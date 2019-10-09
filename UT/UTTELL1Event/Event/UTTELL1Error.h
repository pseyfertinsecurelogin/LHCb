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
#include "GaudiKernel/VectorMap.h"
#include <algorithm>
#include <ostream>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_UTTELL1Error = 9012;

  // Namespace for locations in TDS
  namespace UTTELL1ErrorLocation {
    inline const std::string UTError = "Raw/UT/Error";
  }

  /** @class UTTELL1Error UTTELL1Error.h
   *
   * Error info from one PP-FPGA for a TELL1 board
   *
   * @author Andy Beiter (based on code by Mathias Knecht)
   *
   */

  class UTTELL1Error : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of UTTELL1Error
    typedef KeyedContainer<UTTELL1Error, Containers::HashMap> Container;

    /// enumerate failure modes
    enum FailureMode {
      kNone            = 0,
      kCorruptedBank   = 1,
      kOptLinkDisabled = 2,
      kTlkLinkLoss     = 3,
      kOptLinkNoClock  = 4,
      kSyncRAMFull     = 5,
      kSyncEvtSize     = 6,
      kOptLinkNoEvent  = 7,
      kPseudoHeader    = 8,
      kWrongPCN        = 9
    };

    /// constructor fixed part of bank [5 words]
    UTTELL1Error( const unsigned int word0, const unsigned int word1, const unsigned int word2,
                  const unsigned int word3, const unsigned int word4 )
        : m_word0( word0 )
        , m_word1( word1 )
        , m_word2( word2 )
        , m_word3( word3 )
        , m_word4( word4 )
        , m_word5( 0 )
        , m_word6( 0 )
        , m_word7( 0 )
        , m_word8( 0 )
        , m_word9( 0 )
        , m_word10( 0 )
        , m_word11( 0 )
        , m_word12( 0 )
        , m_word13( 0 )
        , m_badLinks() {}

    /// Default Constructor
    UTTELL1Error()
        : m_word0( 0 )
        , m_word1( 0 )
        , m_word2( 0 )
        , m_word3( 0 )
        , m_word4( 0 )
        , m_word5( 0 )
        , m_word6( 0 )
        , m_word7( 0 )
        , m_word8( 0 )
        , m_word9( 0 )
        , m_word10( 0 )
        , m_word11( 0 )
        , m_word12( 0 )
        , m_word13( 0 )
        , m_badLinks() {}

    /// Default Destructor
    ~UTTELL1Error() {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// conversion of string to enum for type FailureMode
    static LHCb::UTTELL1Error::FailureMode FailureModeToType( const std::string& aName );

    /// conversion to string for enum type FailureMode
    static const std::string& FailureModeToString( int aEnum );

    /// Add information on a link
    bool addLinkInfo( const unsigned int key, const LHCb::UTTELL1Error::FailureMode& mode );

    /// Extract the information associated with the specified key
    UTTELL1Error::FailureMode linkInfo( const unsigned int beetle, const unsigned int port,
                                        const unsigned int pcn ) const;

    /// link is bad for some reason
    bool badLink( const unsigned int beetle, const unsigned int port, const unsigned int pcn ) const;

    /// find pcn for value for this beetle
    unsigned int findPCN( const unsigned int beetle ) const;

    /// bank has optional error words
    bool hasErrorInfo() const;

    /// bank has optional NSZ bank word
    bool hasNZS() const;

    /// bank has optional ped bank word
    bool hasPed() const;

    /// # words Tell1 ppx sent for this bank
    unsigned int nOptionalWords() const;

    /// # words Tell1 sent for this bank
    unsigned int sentWords() const;

    /// optional error words
    void setOptionalErrorWords( const unsigned int w1, const unsigned int w2, const unsigned int w3,
                                const unsigned int w4, const unsigned int w5 );

    /// majority vote on pcn
    unsigned int pcnVote() const;

    /// fraction of links that are OK
    double fractionOK( const unsigned int pcn ) const;

    /// Print out of error information
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Retrieve const  Word 0 of Error block
    unsigned int word0() const;

    /// Retrieve Bunch counter
    unsigned int bCnt() const;

    /// Retrieve ID of the sub-detector
    unsigned int DetectorID() const;

    /// Retrieve List of the Banks sent by the TELL1 (Internal use)
    unsigned int BankList() const;

    /// Retrieve For internal use
    unsigned int EventInformation() const;

    /// Retrieve const  Word 1 of Error block
    unsigned int word1() const;

    /// Retrieve L0 Event ID
    int L0EvtID() const;

    /// Retrieve const  Word 2 of Error block
    unsigned int word2() const;

    /// Retrieve ADC data section length
    unsigned int adC1() const;

    /// Retrieve Cluster data section length
    unsigned int Clust1() const;

    /// Retrieve const  Word 3 of Error block
    unsigned int word3() const;

    /// Retrieve Number of clusters
    unsigned int NumClusters() const;

    /// Retrieve PCN from Beetle 0
    unsigned int pcn() const;

    /// Retrieve Indicates what processing has been applied
    unsigned int ProcessInfo() const;

    /// Retrieve const  Word 4 of Error block
    unsigned int word4() const;

    /// Retrieve Empty value, should be 0x00
    unsigned int EmptyVal0() const;

    /// Retrieve Empty value, should be 0x8E
    unsigned int EmptyVal1() const;

    /// Retrieve Indicates if an error occured. Without error, value is 0x0, with an error value is 0x14
    unsigned int ErrorBankLength() const;

    /// Retrieve const  Word 5 of Error block=Word 3 of Event Info block if Error Bank length is 0x14
    unsigned int word5() const;

    /// Retrieve Indicates that no data has been received, one bit per link
    unsigned int OptLnkNoEvt() const;

    /// Retrieve Indicates if link is disabled by ECS, one bit per link
    unsigned int OptLnkDisable() const;

    /// Retrieve Set if more or less than 35 words are sent paer event (one bit per link)
    unsigned int SyncEvtSizeError() const;

    /// Retrieve Error on reception, bit set if the link is not plugged (one bit per link)
    unsigned int tlkLnkLoss() const;

    /// Retrieve Overflow on input sync RAM (one bit per link)
    unsigned int SyncRAMFull() const;

    /// Retrieve Address of the PP-FPGA (between 0 and 3)
    unsigned int ChipAddr() const;

    /// Retrieve const  Word 6 of Error block=Word 4 of Event Info block if Error Bank length is 0x14
    unsigned int word6() const;

    /// Retrieve Indicates if the ADC value is between low and high thresholds, one per Analog Link
    int HeaderPseudoError() const;

    /// Retrieve Indicates if no clock is received, one bit per link
    unsigned int OptLnkNoClock() const;

    /// Retrieve Indocates if the PCN among the links are not equal
    unsigned int pcnError() const;

    /// Retrieve Reserved bit for later use
    unsigned int R2() const;

    /// Retrieve const  Word 7 of Error block=Word 5 of Event Info block if Error Bank length is 0x14
    unsigned int word7() const;

    /// Retrieve PCN of the Beetle 0
    unsigned int pcnBeetle0() const;

    /// Retrieve PCN of the Beetle 1
    unsigned int pcnBeetle1() const;

    /// Retrieve PCN of the Beetle 2
    unsigned int pcnBeetle2() const;

    /// Retrieve PCN of the Beetle 3
    unsigned int pcnBeetle3() const;

    /// Retrieve const  Word 8 of Error block=Word 6 of Event Info block if Error Bank length is 0x14
    unsigned int word8() const;

    /// Retrieve PCN of the Beetle 4
    unsigned int pcnBeetle4() const;

    /// Retrieve PCN of the Beetle 5
    unsigned int pcnBeetle5() const;

    /// Retrieve Reserved bits for later use =0x8B8B
    unsigned int R3() const;

    /// Retrieve const  Word 9 of Error block=Word 7 of Event Info block if Error Bank length is 0x14
    unsigned int word9() const;

    /// Update  Word 9 of Error block=Word 7 of Event Info block if Error Bank length is 0x14
    void setWord9( unsigned int value );

    /// Retrieve I Header of the Beetle 0
    unsigned int iHeaderBeetle0() const;

    /// Retrieve I Header of the Beetle 1
    unsigned int iHeaderBeetle1() const;

    /// Retrieve I Header of the Beetle 2
    unsigned int iHeaderBeetle2() const;

    /// Retrieve I Header of the Beetle 3
    unsigned int iHeaderBeetle3() const;

    /// Retrieve I Header of the Beetle 4
    unsigned int iHeaderBeetle4() const;

    /// Retrieve I Header of the Beetle 5
    unsigned int iHeaderBeetle5() const;

    /// Retrieve Reserved bits for later use
    unsigned int R4() const;

    /// Retrieve const  Word 10 of Error block
    unsigned int word10() const;

    /// Update  Word 10 of Error block
    void setWord10( unsigned int value );

    /// Retrieve Empty value, should be 0x01
    unsigned int EmptyVal2() const;

    /// Retrieve Empty value, should be 0x8E
    unsigned int EmptyVal3() const;

    /// Retrieve Cluster data section length
    unsigned int Clust2() const;

    /// Retrieve const  Word 11 of Error block
    unsigned int word11() const;

    /// Update  Word 11 of Error block
    void setWord11( unsigned int value );

    /// Retrieve Empty value, should be 0x02
    unsigned int EmptyVal4() const;

    /// Retrieve Empty value, should be 0x8E
    unsigned int EmptyVal5() const;

    /// Retrieve ADC data section length
    unsigned int adC2() const;

    /// Retrieve const  Word 12 of Error block
    unsigned int word12() const;

    /// Update  Word 12 of Error block
    void setWord12( unsigned int value );

    /// Retrieve Empty value, should be 0x03
    unsigned int EmptyVal6() const;

    /// Retrieve Empty value, should be 0x8E
    unsigned int EmptyVal7() const;

    /// Retrieve NZS bank length
    unsigned int nzsBankLength() const;

    /// Retrieve const  Word 13 of Error block
    unsigned int word13() const;

    /// Update  Word 13 of Error block
    void setWord13( unsigned int value );

    /// Retrieve Empty value, should be 0x04
    unsigned int EmptyVal8() const;

    /// Retrieve Empty value, should be 0x8E
    unsigned int EmptyVal9() const;

    /// Retrieve Pedestal bank length
    unsigned int PedBankLength() const;

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
    enum word1Bits { L0EvtIDBits = 0 };

    /// Bitmasks for bitfield word1
    enum word1Masks { L0EvtIDMask = 0xffffffffL };

    /// Offsets of bitfield word2
    enum word2Bits { ADC1Bits = 0, Clust1Bits = 16 };

    /// Bitmasks for bitfield word2
    enum word2Masks { ADC1Mask = 0xffffL, Clust1Mask = 0xffff0000L };

    /// Offsets of bitfield word3
    enum word3Bits { NumClustersBits = 0, PCNBits = 16, ProcessInfoBits = 24 };

    /// Bitmasks for bitfield word3
    enum word3Masks { NumClustersMask = 0xffffL, PCNMask = 0xff0000L, ProcessInfoMask = 0xff000000L };

    /// Offsets of bitfield word4
    enum word4Bits { EmptyVal0Bits = 0, EmptyVal1Bits = 8, ErrorBankLengthBits = 16 };

    /// Bitmasks for bitfield word4
    enum word4Masks { EmptyVal0Mask = 0xffL, EmptyVal1Mask = 0xff00L, ErrorBankLengthMask = 0xffff0000L };

    /// Offsets of bitfield word5
    enum word5Bits {
      OptLnkNoEvtBits      = 0,
      OptLnkDisableBits    = 6,
      SyncEvtSizeErrorBits = 12,
      TLKLnkLossBits       = 18,
      SyncRAMFullBits      = 24,
      ChipAddrBits         = 30
    };

    /// Bitmasks for bitfield word5
    enum word5Masks {
      OptLnkNoEvtMask      = 0x3fL,
      OptLnkDisableMask    = 0xfc0L,
      SyncEvtSizeErrorMask = 0x3f000L,
      TLKLnkLossMask       = 0xfc0000L,
      SyncRAMFullMask      = 0x3f000000L,
      ChipAddrMask         = 0xc0000000L
    };

    /// Offsets of bitfield word6
    enum word6Bits { HeaderPseudoErrorBits = 0, OptLnkNoClockBits = 24, PCNErrorBits = 30, R2Bits = 31 };

    /// Bitmasks for bitfield word6
    enum word6Masks {
      HeaderPseudoErrorMask = 0xffffffL,
      OptLnkNoClockMask     = 0x3f000000L,
      PCNErrorMask          = 0x40000000L,
      R2Mask                = 0x80000000L
    };

    /// Offsets of bitfield word7
    enum word7Bits { PCNBeetle0Bits = 0, PCNBeetle1Bits = 8, PCNBeetle2Bits = 16, PCNBeetle3Bits = 24 };

    /// Bitmasks for bitfield word7
    enum word7Masks {
      PCNBeetle0Mask = 0xffL,
      PCNBeetle1Mask = 0xff00L,
      PCNBeetle2Mask = 0xff0000L,
      PCNBeetle3Mask = 0xff000000L
    };

    /// Offsets of bitfield word8
    enum word8Bits { PCNBeetle4Bits = 0, PCNBeetle5Bits = 8, R3Bits = 16 };

    /// Bitmasks for bitfield word8
    enum word8Masks { PCNBeetle4Mask = 0xffL, PCNBeetle5Mask = 0xff00L, R3Mask = 0xffff0000L };

    /// Offsets of bitfield word9
    enum word9Bits {
      IHeaderBeetle0Bits = 0,
      IHeaderBeetle1Bits = 4,
      IHeaderBeetle2Bits = 8,
      IHeaderBeetle3Bits = 12,
      IHeaderBeetle4Bits = 16,
      IHeaderBeetle5Bits = 20,
      R4Bits             = 24
    };

    /// Bitmasks for bitfield word9
    enum word9Masks {
      IHeaderBeetle0Mask = 0xfL,
      IHeaderBeetle1Mask = 0xf0L,
      IHeaderBeetle2Mask = 0xf00L,
      IHeaderBeetle3Mask = 0xf000L,
      IHeaderBeetle4Mask = 0xf0000L,
      IHeaderBeetle5Mask = 0xf00000L,
      R4Mask             = 0xff000000L
    };

    /// Offsets of bitfield word10
    enum word10Bits { EmptyVal2Bits = 0, EmptyVal3Bits = 8, Clust2Bits = 16 };

    /// Bitmasks for bitfield word10
    enum word10Masks { EmptyVal2Mask = 0xffL, EmptyVal3Mask = 0xff00L, Clust2Mask = 0xffff0000L };

    /// Offsets of bitfield word11
    enum word11Bits { EmptyVal4Bits = 0, EmptyVal5Bits = 8, ADC2Bits = 16 };

    /// Bitmasks for bitfield word11
    enum word11Masks { EmptyVal4Mask = 0xffL, EmptyVal5Mask = 0xff00L, ADC2Mask = 0xffff0000L };

    /// Offsets of bitfield word12
    enum word12Bits { EmptyVal6Bits = 0, EmptyVal7Bits = 8, NZSBankLengthBits = 16 };

    /// Bitmasks for bitfield word12
    enum word12Masks { EmptyVal6Mask = 0xffL, EmptyVal7Mask = 0xff00L, NZSBankLengthMask = 0xffff0000L };

    /// Offsets of bitfield word13
    enum word13Bits { EmptyVal8Bits = 0, EmptyVal9Bits = 8, PedBankLengthBits = 16 };

    /// Bitmasks for bitfield word13
    enum word13Masks { EmptyVal8Mask = 0xffL, EmptyVal9Mask = 0xff00L, PedBankLengthMask = 0xffff0000L };

    unsigned int m_word0;  ///< Word 0 of Error block
    unsigned int m_word1;  ///< Word 1 of Error block
    unsigned int m_word2;  ///< Word 2 of Error block
    unsigned int m_word3;  ///< Word 3 of Error block
    unsigned int m_word4;  ///< Word 4 of Error block
    unsigned int m_word5;  ///< Word 5 of Error block=Word 3 of Event Info block if Error Bank length is 0x14
    unsigned int m_word6;  ///< Word 6 of Error block=Word 4 of Event Info block if Error Bank length is 0x14
    unsigned int m_word7;  ///< Word 7 of Error block=Word 5 of Event Info block if Error Bank length is 0x14
    unsigned int m_word8;  ///< Word 8 of Error block=Word 6 of Event Info block if Error Bank length is 0x14
    unsigned int m_word9;  ///< Word 9 of Error block=Word 7 of Event Info block if Error Bank length is 0x14
    unsigned int m_word10; ///< Word 10 of Error block
    unsigned int m_word11; ///< Word 11 of Error block
    unsigned int m_word12; ///< Word 12 of Error block
    unsigned int m_word13; ///< Word 13 of Error block

    friend std::ostream& operator<<( std::ostream& str, const UTTELL1Error& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    /// Vector of arx [ each 32 channels] with failure
    typedef GaudiUtils::VectorMap<unsigned int, LHCb::UTTELL1Error::FailureMode> FailureInfo;

    /// few useful constants
    enum Tell1Const { nPort = 4, nBeetle = 6, totalNumberOfPorts = nPort * nBeetle, nMagic = 0x8E };

    /// link identifier 0 - 23
    unsigned int linkID( const unsigned int beetle, const unsigned int port ) const;

    /// flag bad links
    void flagBadLinks( const unsigned int link, const LHCb::UTTELL1Error::FailureMode& mode );

    /// fill error information map
    void fillErrorInfo();

    /// checks the magic patterns in error bank
    bool correctPatterns() const;

    FailureInfo m_badLinks; ///< list of links with failures

    static const GaudiUtils::VectorMap<std::string, FailureMode>& s_FailureModeTypMap();

  }; // class UTTELL1Error

  /// Definition of Keyed Container for UTTELL1Error
  typedef KeyedContainer<UTTELL1Error, Containers::HashMap> UTTELL1Errors;

  inline std::ostream& operator<<( std::ostream& s, LHCb::UTTELL1Error::FailureMode e ) {
    switch ( e ) {
    case LHCb::UTTELL1Error::kNone:
      return s << "kNone";
    case LHCb::UTTELL1Error::kCorruptedBank:
      return s << "kCorruptedBank";
    case LHCb::UTTELL1Error::kOptLinkDisabled:
      return s << "kOptLinkDisabled";
    case LHCb::UTTELL1Error::kTlkLinkLoss:
      return s << "kTlkLinkLoss";
    case LHCb::UTTELL1Error::kOptLinkNoClock:
      return s << "kOptLinkNoClock";
    case LHCb::UTTELL1Error::kSyncRAMFull:
      return s << "kSyncRAMFull";
    case LHCb::UTTELL1Error::kSyncEvtSize:
      return s << "kSyncEvtSize";
    case LHCb::UTTELL1Error::kOptLinkNoEvent:
      return s << "kOptLinkNoEvent";
    case LHCb::UTTELL1Error::kPseudoHeader:
      return s << "kPseudoHeader";
    case LHCb::UTTELL1Error::kWrongPCN:
      return s << "kWrongPCN";
    default:
      return s << "ERROR wrong value " << int( e ) << " for enum LHCb::UTTELL1Error::FailureMode";
    }
  }

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline const CLID& LHCb::UTTELL1Error::clID() const { return LHCb::UTTELL1Error::classID(); }

inline const CLID& LHCb::UTTELL1Error::classID() { return CLID_UTTELL1Error; }

inline const GaudiUtils::VectorMap<std::string, LHCb::UTTELL1Error::FailureMode>&
LHCb::UTTELL1Error::s_FailureModeTypMap() {
  static const GaudiUtils::VectorMap<std::string, FailureMode> m = {{"kNone", kNone},
                                                                    {"kCorruptedBank", kCorruptedBank},
                                                                    {"kOptLinkDisabled", kOptLinkDisabled},
                                                                    {"kTlkLinkLoss", kTlkLinkLoss},
                                                                    {"kOptLinkNoClock", kOptLinkNoClock},
                                                                    {"kSyncRAMFull", kSyncRAMFull},
                                                                    {"kSyncEvtSize", kSyncEvtSize},
                                                                    {"kOptLinkNoEvent", kOptLinkNoEvent},
                                                                    {"kPseudoHeader", kPseudoHeader},
                                                                    {"kWrongPCN", kWrongPCN}};
  return m;
}

inline LHCb::UTTELL1Error::FailureMode LHCb::UTTELL1Error::FailureModeToType( const std::string& aName ) {
  auto iter = s_FailureModeTypMap().find( aName );
  return iter != s_FailureModeTypMap().end() ? iter->second : kNone;
}

inline const std::string& LHCb::UTTELL1Error::FailureModeToString( int aEnum ) {
  static const std::string s_kNone = "kNone";
  auto                     iter    = std::find_if( s_FailureModeTypMap().begin(), s_FailureModeTypMap().end(),
                            [&]( const std::pair<const std::string, FailureMode>& i ) { return i.second == aEnum; } );
  return iter != s_FailureModeTypMap().end() ? iter->first : s_kNone;
}

inline unsigned int LHCb::UTTELL1Error::word0() const { return m_word0; }

inline unsigned int LHCb::UTTELL1Error::bCnt() const { return (unsigned int)( ( m_word0 & BCntMask ) >> BCntBits ); }

inline unsigned int LHCb::UTTELL1Error::DetectorID() const {
  return (unsigned int)( ( m_word0 & DetectorIDMask ) >> DetectorIDBits );
}

inline unsigned int LHCb::UTTELL1Error::BankList() const {
  return (unsigned int)( ( m_word0 & BankListMask ) >> BankListBits );
}

inline unsigned int LHCb::UTTELL1Error::EventInformation() const {
  return (unsigned int)( ( m_word0 & EventInformationMask ) >> EventInformationBits );
}

inline unsigned int LHCb::UTTELL1Error::word1() const { return m_word1; }

inline int LHCb::UTTELL1Error::L0EvtID() const { return (int)( ( m_word1 & L0EvtIDMask ) >> L0EvtIDBits ); }

inline unsigned int LHCb::UTTELL1Error::word2() const { return m_word2; }

inline unsigned int LHCb::UTTELL1Error::adC1() const { return (unsigned int)( ( m_word2 & ADC1Mask ) >> ADC1Bits ); }

inline unsigned int LHCb::UTTELL1Error::Clust1() const {
  return (unsigned int)( ( m_word2 & Clust1Mask ) >> Clust1Bits );
}

inline unsigned int LHCb::UTTELL1Error::word3() const { return m_word3; }

inline unsigned int LHCb::UTTELL1Error::NumClusters() const {
  return (unsigned int)( ( m_word3 & NumClustersMask ) >> NumClustersBits );
}

inline unsigned int LHCb::UTTELL1Error::pcn() const { return (unsigned int)( ( m_word3 & PCNMask ) >> PCNBits ); }

inline unsigned int LHCb::UTTELL1Error::ProcessInfo() const {
  return (unsigned int)( ( m_word3 & ProcessInfoMask ) >> ProcessInfoBits );
}

inline unsigned int LHCb::UTTELL1Error::word4() const { return m_word4; }

inline unsigned int LHCb::UTTELL1Error::EmptyVal0() const {
  return (unsigned int)( ( m_word4 & EmptyVal0Mask ) >> EmptyVal0Bits );
}

inline unsigned int LHCb::UTTELL1Error::EmptyVal1() const {
  return (unsigned int)( ( m_word4 & EmptyVal1Mask ) >> EmptyVal1Bits );
}

inline unsigned int LHCb::UTTELL1Error::ErrorBankLength() const {
  return (unsigned int)( ( m_word4 & ErrorBankLengthMask ) >> ErrorBankLengthBits );
}

inline unsigned int LHCb::UTTELL1Error::word5() const { return m_word5; }

inline unsigned int LHCb::UTTELL1Error::OptLnkNoEvt() const {
  return (unsigned int)( ( m_word5 & OptLnkNoEvtMask ) >> OptLnkNoEvtBits );
}

inline unsigned int LHCb::UTTELL1Error::OptLnkDisable() const {
  return (unsigned int)( ( m_word5 & OptLnkDisableMask ) >> OptLnkDisableBits );
}

inline unsigned int LHCb::UTTELL1Error::SyncEvtSizeError() const {
  return (unsigned int)( ( m_word5 & SyncEvtSizeErrorMask ) >> SyncEvtSizeErrorBits );
}

inline unsigned int LHCb::UTTELL1Error::tlkLnkLoss() const {
  return (unsigned int)( ( m_word5 & TLKLnkLossMask ) >> TLKLnkLossBits );
}

inline unsigned int LHCb::UTTELL1Error::SyncRAMFull() const {
  return (unsigned int)( ( m_word5 & SyncRAMFullMask ) >> SyncRAMFullBits );
}

inline unsigned int LHCb::UTTELL1Error::ChipAddr() const {
  return (unsigned int)( ( m_word5 & ChipAddrMask ) >> ChipAddrBits );
}

inline unsigned int LHCb::UTTELL1Error::word6() const { return m_word6; }

inline int LHCb::UTTELL1Error::HeaderPseudoError() const {
  return (int)( ( m_word6 & HeaderPseudoErrorMask ) >> HeaderPseudoErrorBits );
}

inline unsigned int LHCb::UTTELL1Error::OptLnkNoClock() const {
  return (unsigned int)( ( m_word6 & OptLnkNoClockMask ) >> OptLnkNoClockBits );
}

inline unsigned int LHCb::UTTELL1Error::pcnError() const {
  return (unsigned int)( ( m_word6 & PCNErrorMask ) >> PCNErrorBits );
}

inline unsigned int LHCb::UTTELL1Error::R2() const { return (unsigned int)( ( m_word6 & R2Mask ) >> R2Bits ); }

inline unsigned int LHCb::UTTELL1Error::word7() const { return m_word7; }

inline unsigned int LHCb::UTTELL1Error::pcnBeetle0() const {
  return (unsigned int)( ( m_word7 & PCNBeetle0Mask ) >> PCNBeetle0Bits );
}

inline unsigned int LHCb::UTTELL1Error::pcnBeetle1() const {
  return (unsigned int)( ( m_word7 & PCNBeetle1Mask ) >> PCNBeetle1Bits );
}

inline unsigned int LHCb::UTTELL1Error::pcnBeetle2() const {
  return (unsigned int)( ( m_word7 & PCNBeetle2Mask ) >> PCNBeetle2Bits );
}

inline unsigned int LHCb::UTTELL1Error::pcnBeetle3() const {
  return (unsigned int)( ( m_word7 & PCNBeetle3Mask ) >> PCNBeetle3Bits );
}

inline unsigned int LHCb::UTTELL1Error::word8() const { return m_word8; }

inline unsigned int LHCb::UTTELL1Error::pcnBeetle4() const {
  return (unsigned int)( ( m_word8 & PCNBeetle4Mask ) >> PCNBeetle4Bits );
}

inline unsigned int LHCb::UTTELL1Error::pcnBeetle5() const {
  return (unsigned int)( ( m_word8 & PCNBeetle5Mask ) >> PCNBeetle5Bits );
}

inline unsigned int LHCb::UTTELL1Error::R3() const { return (unsigned int)( ( m_word8 & R3Mask ) >> R3Bits ); }

inline unsigned int LHCb::UTTELL1Error::word9() const { return m_word9; }

inline void LHCb::UTTELL1Error::setWord9( unsigned int value ) { m_word9 = value; }

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle0() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle0Mask ) >> IHeaderBeetle0Bits );
}

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle1() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle1Mask ) >> IHeaderBeetle1Bits );
}

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle2() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle2Mask ) >> IHeaderBeetle2Bits );
}

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle3() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle3Mask ) >> IHeaderBeetle3Bits );
}

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle4() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle4Mask ) >> IHeaderBeetle4Bits );
}

inline unsigned int LHCb::UTTELL1Error::iHeaderBeetle5() const {
  return (unsigned int)( ( m_word9 & IHeaderBeetle5Mask ) >> IHeaderBeetle5Bits );
}

inline unsigned int LHCb::UTTELL1Error::R4() const { return (unsigned int)( ( m_word9 & R4Mask ) >> R4Bits ); }

inline unsigned int LHCb::UTTELL1Error::word10() const { return m_word10; }

inline void LHCb::UTTELL1Error::setWord10( unsigned int value ) { m_word10 = value; }

inline unsigned int LHCb::UTTELL1Error::EmptyVal2() const {
  return (unsigned int)( ( m_word10 & EmptyVal2Mask ) >> EmptyVal2Bits );
}

inline unsigned int LHCb::UTTELL1Error::EmptyVal3() const {
  return (unsigned int)( ( m_word10 & EmptyVal3Mask ) >> EmptyVal3Bits );
}

inline unsigned int LHCb::UTTELL1Error::Clust2() const {
  return (unsigned int)( ( m_word10 & Clust2Mask ) >> Clust2Bits );
}

inline unsigned int LHCb::UTTELL1Error::word11() const { return m_word11; }

inline void LHCb::UTTELL1Error::setWord11( unsigned int value ) { m_word11 = value; }

inline unsigned int LHCb::UTTELL1Error::EmptyVal4() const {
  return (unsigned int)( ( m_word11 & EmptyVal4Mask ) >> EmptyVal4Bits );
}

inline unsigned int LHCb::UTTELL1Error::EmptyVal5() const {
  return (unsigned int)( ( m_word11 & EmptyVal5Mask ) >> EmptyVal5Bits );
}

inline unsigned int LHCb::UTTELL1Error::adC2() const { return (unsigned int)( ( m_word11 & ADC2Mask ) >> ADC2Bits ); }

inline unsigned int LHCb::UTTELL1Error::word12() const { return m_word12; }

inline void LHCb::UTTELL1Error::setWord12( unsigned int value ) { m_word12 = value; }

inline unsigned int LHCb::UTTELL1Error::EmptyVal6() const {
  return (unsigned int)( ( m_word12 & EmptyVal6Mask ) >> EmptyVal6Bits );
}

inline unsigned int LHCb::UTTELL1Error::EmptyVal7() const {
  return (unsigned int)( ( m_word12 & EmptyVal7Mask ) >> EmptyVal7Bits );
}

inline unsigned int LHCb::UTTELL1Error::nzsBankLength() const {
  return (unsigned int)( ( m_word12 & NZSBankLengthMask ) >> NZSBankLengthBits );
}

inline unsigned int LHCb::UTTELL1Error::word13() const { return m_word13; }

inline void LHCb::UTTELL1Error::setWord13( unsigned int value ) { m_word13 = value; }

inline unsigned int LHCb::UTTELL1Error::EmptyVal8() const {
  return (unsigned int)( ( m_word13 & EmptyVal8Mask ) >> EmptyVal8Bits );
}

inline unsigned int LHCb::UTTELL1Error::EmptyVal9() const {
  return (unsigned int)( ( m_word13 & EmptyVal9Mask ) >> EmptyVal9Bits );
}

inline unsigned int LHCb::UTTELL1Error::PedBankLength() const {
  return (unsigned int)( ( m_word13 & PedBankLengthMask ) >> PedBankLengthBits );
}

inline bool LHCb::UTTELL1Error::hasErrorInfo() const { return ErrorBankLength() == 0x14; }

inline bool LHCb::UTTELL1Error::hasNZS() const { return ( BankList() & 0x8 ) != 0; }

inline bool LHCb::UTTELL1Error::hasPed() const { return ( BankList() & 0x10 ) != 0; }

inline unsigned int LHCb::UTTELL1Error::nOptionalWords() const {

  unsigned int wCount = 0;
  if ( hasErrorInfo() == true ) wCount += 5;
  if ( hasPed() == true ) ++wCount;
  if ( hasNZS() == true ) ++wCount;
  return wCount;
}

inline unsigned int LHCb::UTTELL1Error::sentWords() const {

  unsigned int wCount = 7 + nOptionalWords();
  return wCount;
}

inline void LHCb::UTTELL1Error::setOptionalErrorWords( const unsigned int w1, const unsigned int w2,
                                                       const unsigned int w3, const unsigned int w4,
                                                       const unsigned int w5 ) {

  m_word5 = w1;
  m_word6 = w2;
  m_word7 = w3;
  m_word8 = w4;
  m_word9 = w5;
  fillErrorInfo(); // fill the map
}

inline unsigned int LHCb::UTTELL1Error::linkID( const unsigned int beetle, const unsigned int port ) const {
  return ( 4 * beetle + port );
}

inline void LHCb::UTTELL1Error::flagBadLinks( const unsigned int link, const LHCb::UTTELL1Error::FailureMode& mode ) {

  const unsigned int start = linkID( link, 0u );
  for ( unsigned int i = start; i < start + 4; ++i ) { addLinkInfo( i, mode ); }
}
