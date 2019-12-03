/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// STD
#include <limits>
#include <memory>
#include <set>
#include <sstream>
#include <string>

// Gaudi Array properties ( must be first ...)
#include "GaudiKernel/ParsersFactory.h"
#include "GaudiKernel/StdArrayAsProperty.h"

// Rich (Future) Kernel
#include "RichFutureKernel/RichAlgBase.h"

// DetDesc
#include "DetDesc/ConditionAccessorHolder.h"

// Gaudi Functional
#include "GaudiAlg/Transformer.h"

// Rich Utils
#include "RichFutureUtils/RichDecodedData.h"
#include "RichUtils/RichException.h"
#include "RichUtils/RichHashMap.h"
#include "RichUtils/RichMap.h"

// RICH DAQ Kernel
#include "RichDAQKernel/RichDAQVersions.h"

// Event model
#include "Event/ODIN.h"
#include "Event/RawEvent.h"

// RichDet
#include "RichDet/DeRichSystem.h"

// Boost
#include "boost/format.hpp"
#include "boost/lexical_cast.hpp"

// Old 32 bit SmartID
#include "Kernel/RichSmartID32.h"

using namespace Rich::Future::DAQ;
using namespace Rich::DAQ;

namespace Rich::Future {

  namespace {
    /// Default 'fake' HPD RichSmartID
    static const LHCb::RichSmartID s_fakeHPDID( Rich::Rich1, Rich::top, 0, 0 );
  } // namespace

  // Use the functional framework
  using namespace Gaudi::Functional;

  /** @class RawBankDecoder RichRawBankDecoder.h
   *
   *  RICH Raw bank decoder.
   *
   *  @author Chris Jones
   *  @date   2016-09-21
   */
  class RawBankDecoder final : public Transformer<Rich::Future::DAQ::L1Map( const LHCb::RawEvent&, //
                                                                            const LHCb::ODIN&,     //
                                                                            const DeRichSystem& ),
                                                  LHCb::DetDesc::usesBaseAndConditions<AlgBase<>, DeRichSystem>> {

  public:
    // framework

    /// Standard constructor
    RawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator )
        : Transformer( name, pSvcLocator,
                       // data inputs
                       {KeyValue{"RawEventLocation", concat_alternatives( {LHCb::RawEventLocation::Rich,
                                                                           LHCb::RawEventLocation::Default} )},
                        KeyValue{"OdinLocation", LHCb::ODINLocation::Default},
                        // conditions input
                        KeyValue{"DeRichSystem", DeRichLocations::RichSystem}},
                       // output data
                       {KeyValue{"DecodedDataLocation", L1MapLocation::Default}} ) {
      // setProperty( "OutputLevel", MSG::VERBOSE );
    }

    /// Initialize
    StatusCode initialize() override;

    /// Algorithm execution via transform
    Rich::Future::DAQ::L1Map operator()( const LHCb::RawEvent& rawEvent, //
                                         const LHCb::ODIN&     odin,     //
                                         const DeRichSystem&   deRichSys ) const override;

  private:
    // methods

    /// Returns the RawBank version enum for the given bank
    inline Rich::DAQ::BankVersion bankVersion( const LHCb::RawBank& bank ) const {
      return static_cast<Rich::DAQ::BankVersion>( bank.version() );
    }

    /// Simple class to cache the PDDataBank objects
    class PDBanks {
    public: // the cached PD decoders
      /// Non-zero suppressed decoder (LHCb mode)
      std::unique_ptr<Rich::DAQ::PDDataBank> lhcb_nonZS;
      ///     zero suppressed decoder (LHCb mode)
      std::unique_ptr<Rich::DAQ::PDDataBank> lhcb_ZS;
      /// Non-zero suppressed decoder (ALICE mode)
      std::unique_ptr<Rich::DAQ::PDDataBank> alice_nonZS;
      ///     zero suppressed decoder (ALICE mode)
      std::unique_ptr<Rich::DAQ::PDDataBank> alice_ZS;

    public:
      /// The bank type the current decoders are for
      Rich::DAQ::BankVersion version{Rich::DAQ::UndefinedBankVersion};
    };

  private:
    /** Creates a bank data from the given raw block of data
     *
     *  @param dataStart Pointer to the start of the raw data
     *  @param dataSize  The length of the data block (excluding header HPD word)
     *  @param version   The RICH DAQ data bank version
     */
    const Rich::DAQ::PDDataBank* createDataBank( const Rich::DAQ::LongType*   dataStart, //
                                                 const Rich::DAQ::BankVersion version,   //
                                                 PDBanks&                     banks ) const;

  private:
    /// Decode a RawBank into RichSmartID identifiers
    void decodeToSmartIDs( const LHCb::RawBank&      bank,        //
                           const LHCb::ODIN&         odin,        //
                           const DeRichSystem&       deRichSys,   //
                           Rich::Future::DAQ::L1Map& decodedData, //
                           PDBanks&                  banks ) const;

    /// Decode a RawBank into RichSmartID identifiers
    /// Version compatible with first 2007 "final" L1 firmware
    void decodeToSmartIDs_2007( const LHCb::RawBank&      bank,        //
                                const LHCb::ODIN&         odin,        //
                                const DeRichSystem&       deRichSys,   //
                                Rich::Future::DAQ::L1Map& decodedData, //
                                PDBanks&                  banks ) const;

    /// Decode a RawBank into RichSmartID identifiers
    /// MaPMT0 version
    void decodeToSmartIDs_MaPMT0( const LHCb::RawBank&      bank,      //
                                  const DeRichSystem&       deRichSys, //
                                  Rich::Future::DAQ::L1Map& decodedData ) const;

    /// Check if a given L1 ID should be decoded
    inline bool okToDecode( const Rich::DAQ::Level1HardwareID L1ID, //
                            const DeRichSystem&               deRichSys ) const {
      // First check if we are decoding each RICH.
      // If so, no need to lookup RICH type from ID
      bool ok = m_richIsActive[Rich::Rich1] && m_richIsActive[Rich::Rich2];
      if ( UNLIKELY( !ok ) ) {
        // Now lookup the RICH type
        const auto rich = deRichSys.richDetector( L1ID );
        if ( UNLIKELY( rich == Rich::InvalidDetector ) ) {
          ++m_l1InvalidRich;
          ok = false;
        } else {
          ok = m_richIsActive[rich];
        }
      }
      return ok;
    }

  private:
    /// Suppress hot pixels
    void suppressHotPixels( const LHCb::RichSmartID&   hpdID, //
                            LHCb::RichSmartID::Vector& newids ) const;

  private:
    /** Print the given RawBank as a simple hex dump
     *  @param bank The RawBank to dump out
     *  @param os   The Message Stream to print to
     */
    void dumpRawBank( const LHCb::RawBank& bank, MsgStream& os ) const;

    /// Print the given data word as Hex and as bits, to the given precision
    template <class TYPE>
    inline void rawDump( MsgStream&                 os,   //
                         const TYPE                 word, //
                         const Rich::DAQ::ShortType nBits = 32 ) const {
      std::ostringstream hexW;
      hexW << std::hex << word;
      std::string tmpW = hexW.str();
      if ( tmpW.size() < 8 ) { tmpW = std::string( 8 - tmpW.size(), '0' ) + tmpW; }
      os << tmpW << "  |";
      for ( int iCol = nBits - 1; iCol >= 0; --iCol ) { os << "  " << isBitOn( word, iCol ); }
    }

    /// Test if a given bit in a word is set on
    template <class TYPE>
    inline bool isBitOn( const TYPE                 data, //
                         const Rich::DAQ::ShortType pos ) const noexcept {
      return ( 0 != ( data & ( 1 << pos ) ) );
    }

  private:
    // types

    /// Type for hot pixel list
    using HotPixelListType = std::vector<LHCb::RichSmartID::KeyType>;

    /// Type for Storage of pixels to mask for each HPD
    using HPDHotPixels = Rich::HashMap<LHCb::RichSmartID, std::set<LHCb::RichSmartID>>;

  private:
    // properties

    /// Flag to turn on/off decoding of each RICH detector (default is both on)
    Gaudi::Property<Rich::DetectorArray<bool>> m_richIsActive{this, "Detectors", {true, true}};

    /// Flag to turn on/off the use of the ODIN data bank during decoding for integrity checks
    Gaudi::Property<bool> m_decodeUseOdin{this, "DecodeUsingODIN", false};

    /// Turn on/off Event ID integrity checks between the RICH ingress headers and the ODIN
    Gaudi::Property<bool> m_checkODINEventsIDs{this, "CheckODINEventIDs", false};

    /// Turn on/off internal RICH Event ID integrity checks between the ingress and HPD header words
    Gaudi::Property<bool> m_checkRICHEventsIDs{this, "CheckRICHEventIDs", true};

    /// Perform check of HPD L1 hardward ID to that expected by the DB
    Gaudi::Property<bool> m_hpdL1check{this, "CheckHPDL1IDs", false};

    /** Flag to turn on/off the purging off data from HPDs that fail the data
     *  integrity checks (default is on) */
    Gaudi::Property<bool> m_purgeHPDsFailIntegrity{this, "PurgeHPDsFailIntegrityTest", true};

    /// Flag to activate the raw printout of each Rawbank
    Gaudi::Property<bool> m_dumpBanks{this, "DumpRawBanks", false};

    /// Turn on/off BX ID integrity checks
    Gaudi::Property<bool> m_checkBxIDs{this, "CheckBXIDs", true};

    /// Turn on/off data integrity checks
    Gaudi::Property<bool> m_checkDataIntegrity{this, "CheckDataIntegrity", true};

    /// Max HPD Occupancy Cut
    Gaudi::Property<Rich::DAQ::ShortType> m_maxHPDOc{this, "MaxHPDOccupancy",
                                                     std::numeric_limits<Rich::DAQ::ShortType>::max()};

    /** Turn on/off detailed error messages.
     *  VERY verbose in case of frequent errors... */
    Gaudi::Property<bool> m_verboseErrors{this, "VerboseErrors", false};

    /** Software suppression of hot channels. List of RichSmartIDs (as unsigned ints)
     *  to suppress in the data. */
    Gaudi::Property<HotPixelListType> m_hotChannels{this, "HotPixelsToMask"};

    /** @brief Turns on the use of a fake HPD RichSmartID for each HPD data data block.
     *
     *  Useful for deep debugging cases when the HPD L0 ID is missing in the database.
     *
     *  @attention If set to true, decoded data is not complete (RICH,HPD panel and HPD info is
     *  missing). Consequently, this option should only be used to test the data decoding and not
     *  if the RichSmartIDs are needed for analysis downstream.
     */
    Gaudi::Property<bool> m_useFakeHPDID{this, "UseFakeHPDID", false};

  private:
    // implementation details

    /// Boolean to indicate if there are any pixels that need suppressing
    bool m_pixelsToSuppress = false;

    /// Storage of pixels to mask for each HPD
    HPDHotPixels m_hotPixels;

  private:
    // messaging

    /// L1 bank with invalid RICh detector
    mutable WarningCounter m_l1InvalidRich{this, "L1 bank with invalid RICH detector mapping"};

    /// error reading flatlist
    mutable WarningCounter m_flatListReadWarn{this, "Invalid RichSmartID read from FlatList data format"};

    /// Bank decoding error
    mutable ErrorCounter m_rawReadErr{this, "Error decoding RawBank"};

    /// Null RawBank pointer
    mutable ErrorCounter m_nullRawBankErr{this, "Retrieved null pointer to RawBank"};

    /// Magic pattern error
    mutable ErrorCounter m_magicErr{this, "Magic Pattern mis-match"};

    /// ODIN error
    mutable ErrorCounter m_odinErr{this, "ODIN Mismatch"};

    /// UnknownPDID
    mutable ErrorCounter m_unknownPD{this, "Unknown PD ID"};

    /// Event ID mis-match
    mutable ErrorCounter m_eventIDmismatch{this, "EventID Mismatch"};

    /// PMT small/large flag mis-match
    mutable ErrorCounter m_pmtSLFlagMismatch{this, "Small/Large PD flag mis-match"};
  };

} // namespace Rich::Future

using namespace Rich::Future;

//=============================================================================

StatusCode RawBankDecoder::initialize() {
  // Initialise base class
  const auto sc = Transformer::initialize();
  if ( !sc ) return sc;

  {
    // workaround to the issue with circular dependency in HPD initialization
    DataObject* tmp = nullptr;
    detSvc()->retrieveObject( inputLocation<2>(), tmp ).ignore();
  }

  // report inactive RICHes
  if ( !m_richIsActive[Rich::Rich1] ) { info() << "Decoding for RICH1 disabled" << endmsg; }
  if ( !m_richIsActive[Rich::Rich2] ) { info() << "Decoding for RICH2 disabled" << endmsg; }

  // if suppression is less than max possible number of (ALICE) hits, print a message.
  if ( m_maxHPDOc < HPD::BitsPerDataWord * HPD::MaxDataSizeALICE ) {
    info() << "Will suppress PDs with more than " << m_maxHPDOc << " digits" << endmsg;
  }

  // messages if optional features are enabled
  if ( m_decodeUseOdin ) { info() << "ODIN integrity checks are enabled" << endmsg; }
  if ( m_hpdL1check ) { info() << "HPD L1 ID checks are enabled" << endmsg; }

  // warnings if checks are disabled
  if ( !m_checkDataIntegrity ) { info() << "HPD Data integrity checks are disabled" << endmsg; }
  if ( m_checkODINEventsIDs ) { info() << "Ingress/ODIN Event ID checks are enabled" << endmsg; }
  if ( !m_checkRICHEventsIDs ) { info() << "Internal RICH Event ID checks are disabled" << endmsg; }
  if ( !m_checkBxIDs ) { info() << "Header BX ID checks are disabled" << endmsg; }

  // Do we have pixels to suppress ?
  if ( !m_hotChannels.empty() ) {
    info() << "Hot HPD pixel list to suppress :-" << endmsg;
    m_pixelsToSuppress = true;
    for ( const auto& C : m_hotChannels ) {
      const LHCb::RichSmartID channel( C );
      m_hotPixels[channel.pdID()].insert( channel );
      info() << " " << channel << " (" << channel.key() << ")" << endmsg;
    }
  }

  return sc;
}

//=============================================================================

L1Map RawBankDecoder::operator()( const LHCb::RawEvent& rawEvent, //
                                  const LHCb::ODIN&     odin,     //
                                  const DeRichSystem&   deRichSys ) const {

  // Make the data map to return
  L1Map decodedData;

  // Get the banks for the Rich
  const auto& richBanks = rawEvent.banks( LHCb::RawBank::Rich );

  // reserve top level size
  decodedData.reserve( richBanks.size() );

  // Bank decoder cache
  PDBanks banks;

  // Loop over data banks
  for ( const auto* bank : richBanks ) {
    // test bank is OK
    if ( bank ) {
      // Decode this bank
      try {
        decodeToSmartIDs( *bank, odin, deRichSys, decodedData, banks );
      } catch ( const GaudiException& expt ) {
        // count errors
        ++m_rawReadErr;
        _ri_verbo << expt.message() << endmsg;
        // dump the full bank
        if ( m_verboseErrors ) dumpRawBank( *bank, error() );
      }
    } else {
      ++m_nullRawBankErr;
    }
  }

  // do not print if faking HPDID, since smartIDs.size() then has no meaning
  if ( !m_useFakeHPDID ) { _ri_debug << "Decoded in total " << richBanks.size() << " RICH Level1 bank(s)" << endmsg; }

  // return the fill map
  return decodedData;
}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs( const LHCb::RawBank& bank,        //
                                       const LHCb::ODIN&    odin,        //
                                       const DeRichSystem&  deRichSys,   //
                                       L1Map&               decodedData, //
                                       PDBanks&             banks ) const {

  // Check magic code for general data corruption
  if ( UNLIKELY( LHCb::RawBank::MagicPattern != bank.magic() ) ) {

    ++m_magicErr;

  } else {
    // All OK so decode

    // Check this is a RICH bank
    if ( bank.type() != LHCb::RawBank::Rich ) {
      std::ostringstream message;
      message << "BankType is not RICH : type = " << bank.type();
      throw Rich::Exception( message.str() );
    }

    // Get L1 ID
    const Level1HardwareID L1ID( bank.sourceID() );

    // Get bank version
    const auto version = bankVersion( bank );

    // Are we decoding this bank ?
    if ( okToDecode( L1ID, deRichSys ) ) {

      // if configured, dump raw event before decoding
      if ( msgLevel( MSG::VERBOSE ) ) {
        dumpRawBank( bank, verbose() );
      } else if ( m_dumpBanks ) {
        dumpRawBank( bank, info() );
      }

      // Now, delegate the work to a version of the decoding appropriate to the version
      if ( version == LHCb5 || version == FlatList ) {
        decodeToSmartIDs_2007( bank, odin, deRichSys, decodedData, banks );
      } else if ( MaPMT0 == version ) {
        decodeToSmartIDs_MaPMT0( bank, deRichSys, decodedData );
      } else // Some problem ...
      {
        std::ostringstream mess;
        mess << "Unknown RICH L1 version number " << version;
        throw Rich::Exception( mess.str() );
      }

    } // L1 is decoded

  } // magic OK
}

//=============================================================================

const Rich::DAQ::PDDataBank* RawBankDecoder::createDataBank( const LongType*   dataStart, //
                                                             const BankVersion version,   //
                                                             PDBanks&          banks ) const {

  Rich::DAQ::PDDataBank* dataBank = nullptr;

  // If bank version is different to cache, force a reset
  if ( UNLIKELY( version != banks.version ) ) {
    banks.lhcb_nonZS.reset( nullptr );
    banks.lhcb_ZS.reset( nullptr );
    banks.alice_nonZS.reset( nullptr );
    banks.alice_ZS.reset( nullptr );
    banks.version = version;
  }

  if ( LHCb5 == version ) {

    // Quick check of header for HPD data type
    const bool isZS    = RichDAQ_LHCb5::Header::zeroSuppressed( dataStart );
    const bool isAlice = RichDAQ_LHCb5::Header::aliceMode( dataStart );

    // Construct the correct data block
    if ( isZS ) {
      if ( UNLIKELY( isAlice ) ) {
        if ( !banks.alice_ZS.get() ) { banks.alice_ZS = std::make_unique<RichDAQ_LHCb5::ZeroSuppAlice>(); }
        dataBank = banks.alice_ZS.get();
      } else {
        if ( !banks.lhcb_ZS.get() ) { banks.lhcb_ZS = std::make_unique<RichDAQ_LHCb5::ZeroSuppLHCb>(); }
        dataBank = banks.lhcb_ZS.get();
      }
    } else {
      if ( UNLIKELY( isAlice ) ) {
        if ( !banks.alice_nonZS.get() ) { banks.alice_nonZS = std::make_unique<RichDAQ_LHCb5::NonZeroSuppAlice>(); }
        dataBank = banks.alice_nonZS.get();
      } else {
        if ( !banks.lhcb_nonZS.get() ) { banks.lhcb_nonZS = std::make_unique<RichDAQ_LHCb5::NonZeroSuppLHCb>(); }
        dataBank = banks.lhcb_nonZS.get();
      }
    }

    // reset for the new data block
    dataBank->reset( dataStart );
  } else if ( FlatList == version ) {
    if ( !banks.lhcb_ZS.get() ) { banks.lhcb_ZS = std::make_unique<RichDAQ_FlatList::Data>(); }
    dataBank = banks.lhcb_ZS.get();
    dataBank->reset( dataStart );
  } else {
    std::ostringstream message;
    message << "Unknown RICH Raw Buffer version " << version << " -> No data bank";
    Exception( message.str() );
  }

  // Printout this bank
  if ( UNLIKELY( dataBank && msgLevel( MSG::VERBOSE ) ) ) {
    verbose() << endmsg << "Created HPD Data Bank for Decoding :-" << endmsg;
    verbose() << *dataBank << endmsg;
  }

  return dataBank;
}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs_2007( const LHCb::RawBank& bank,        //
                                            const LHCb::ODIN&    odin,        //
                                            const DeRichSystem&  deRichSys,   //
                                            L1Map&               decodedData, //
                                            PDBanks&             banks ) const {

  using namespace Rich::DAQ::HPD;

  // Get L1 ID
  const Level1HardwareID L1ID( bank.sourceID() );
  _ri_debug << "Decoding L1 bank " << L1ID << endmsg;

  // various counts
  DetectorArray<unsigned int> nHPDbanks{{0, 0}}, decodedHits{{0, 0}};

  // Data bank size in 32 bit words
  const auto bankSize = bank.size() / 4;

  // Get bank version
  const auto version = bankVersion( bank );

  // Flag to indicate if a given L1 bank has been printed out in case of an error
  bool l1BankErrorDump = true;

  // List of active L1 ingress inputs (define here so can be reused for each ingress)
  L1IngressInputs inputs;

  // If we have some words to process, start the decoding
  if ( bankSize > 0 ) {

    // Get Ingress map to decode into for this L1 board
    decodedData.emplace_back( std::piecewise_construct, std::forward_as_tuple( L1ID ), std::forward_as_tuple() );
    auto& ingressMap = decodedData.back().second;

    // reserve size
    ingressMap.reserve( NumIngressPerL1 );

    // Loop over bank, find headers and produce a data bank for each
    // Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {

      // First, the ingress header word (and increment to the next line)
      const L1IngressHeader ingressWord( bank.data()[lineC++] );
      _ri_debug << " Ingress " << ingressWord << endmsg;

      // Get data for this ingress
      ingressMap.emplace_back( std::piecewise_construct, std::forward_as_tuple( ingressWord.ingressID() ),
                               std::forward_as_tuple() );
      auto& ingressInfo = ingressMap.back().second;

      // Set ingress header in decoded data map
      ingressInfo.setIngressHeader( ingressWord );

      // Compare Ingress header to the ODIN
      _ri_verbo << "ODIN : EventNumber=" << EventID( odin.eventNumber() ) << " BunchID=" << BXID( odin.bunchId() )
                << endmsg;
      const bool odinOK =
          ( !m_decodeUseOdin ? true
                             : ( !m_checkODINEventsIDs || ingressWord.eventID() == EventID( odin.eventNumber() ) ) &&
                                   ( !m_checkBxIDs || ingressWord.bxID() == BXID( odin.bunchId() ) ) );
      if ( !odinOK ) { ++m_odinErr; }

      // get list of active ingress inputs
      ingressWord.activeHPDInputs( inputs );
      _ri_debug << "  Found " << inputs.size() << " PDs with data blocks : " << inputs << endmsg;

      // Check the Ingress supression flag
      if ( !ingressWord.hpdsSuppressed() ) {
        // Ingress is OK, so read HPD data

        // reserve size
        ingressInfo.pdData().reserve( inputs.size() );

        // Loop over active HPDs
        for ( const auto& HPD : inputs ) {

          // Create data bank and decode into RichSmartIDs
          const auto hpdBank( createDataBank( &bank.data()[lineC], // pointer to start of data
                                              version, banks ) );

          // is this HPD suppressed ?
          const bool hpdIsSuppressed = hpdBank->suppressed();

          // Is the PD in extended mode
          const bool isExtend = hpdBank->isExtended();

          // Try to add a new HPDInfo to container
          if ( !isExtend ) {
            ingressInfo.pdData().emplace_back(
                std::piecewise_construct, std::forward_as_tuple( ingressWord.ingressID(), HPD ),
                std::forward_as_tuple( LHCb::RichSmartID(), hpdBank->primaryHeaderWord() ) );
          } else {
            ingressInfo.pdData().emplace_back(
                std::piecewise_construct, std::forward_as_tuple( ingressWord.ingressID(), HPD ),
                std::forward_as_tuple( LHCb::RichSmartID(), hpdBank->primaryHeaderWord(),
                                       hpdBank->extendedHeaderWords(), hpdBank->footerWords() ) );
          }
          auto& hpdInfo = ingressInfo.pdData().back().second;

          // Only try and decode this HPD if ODIN test was OK
          if ( odinOK && !hpdIsSuppressed ) {

            // get HPD RichSmartID
            // do in a try block incase HPD ID is unknown
            LHCb::RichSmartID hpdID;
            try {
              hpdID = ( m_useFakeHPDID ? s_fakeHPDID : deRichSys.richSmartID( hpdBank->level0ID() ) );
            } catch ( const GaudiException& expt ) {
              ++m_unknownPD;
              _ri_debug << "'" << expt.message() << "' | L1HardID=" << L1ID << " Ingress=" << ingressWord.ingressID()
                        << " Input=" << HPD << endmsg;
            }
            // If the HPD smartID was successfully found, continue with decoding
            if ( hpdID.isValid() ) {

              _ri_debug << "   Decoding HPD " << hpdID << endmsg;

              // save HPD ID
              hpdInfo.setPdID( hpdID );

              // local hit count
              unsigned int hpdHitCount( 0 );

              // smartIDs
              auto& newids = hpdInfo.smartIDs();

              // Compare Event IDs for errors
              bool OK =
                  ( hpdIsSuppressed ? true : !m_checkRICHEventsIDs || ingressWord.eventID() == hpdBank->eventID() );
              if ( UNLIKELY( !OK ) ) {
                ++m_eventIDmismatch;
                _ri_debug << "EventID Mismatch : HPD L0ID=" << hpdBank->level0ID() << " " << hpdID << endmsg;
              } else {

                // Check this HPD is connected to the expected L1 board
                OK = true;                      // default to OK
                if ( UNLIKELY( m_hpdL1check ) ) // Do check ?
                {
                  const auto db_L1ID = deRichSys.level1HardwareID( hpdBank->level0ID() );
                  OK                 = ( L1ID == db_L1ID );
                }
                if ( OK ) // only carry on if OK
                {

                  // decode to smartIDs
                  hpdHitCount = hpdBank->fillRichSmartIDs( newids, hpdID );

                  // Do data integrity checks
                  OK = ( !m_checkDataIntegrity || hpdBank->checkDataIntegrity( newids, this ) );
                  if ( UNLIKELY( !OK ) ) {
                    _ri_debug << "HPD L0ID=" << hpdBank->level0ID() << " " << hpdID
                              << " data block failed integrity check" << endmsg;
                    if ( m_purgeHPDsFailIntegrity ) { newids.clear(); }
                  }

                  // Is all 'OK' but header is in extended mode ?
                  if ( UNLIKELY( isExtend && OK ) ) {
                    _ri_debug << "HPD L0ID=" << hpdBank->level0ID() << " " << hpdID
                              << " in extended mode for UNKNOWN reasons" << endmsg;
                  }

                  if ( msgLevel( MSG::VERBOSE ) && hpdHitCount > 0 ) {
                    // printout decoded RichSmartIDs
                    verbose() << " Decoded RichSmartIDs :-" << endmsg;
                    for ( const auto& ID : newids ) { verbose() << "   " << ID << endmsg; }
                  }
                }

              } // event IDs OK

              // is data OK
              if ( OK ) {

                // apply suppression of high occupancy HPDs
                if ( hpdHitCount < m_maxHPDOc ) {
                  const auto rich = hpdID.rich();
                  // counts
                  ++nHPDbanks[rich];
                  decodedHits[rich] += hpdHitCount;
                  // suppress hot pixels
                  suppressHotPixels( hpdID, newids );
                } else {
                  newids.clear();
                }
              } else if ( m_verboseErrors ) {
                // decoding error ....
                error() << "Error in decoding -> Data is rejected for HPD " << hpdID << endmsg;

                if ( l1BankErrorDump ) {
                  error() << " -> Dump of raw L1 data :-" << endmsg;
                  dumpRawBank( bank, error() );
                  l1BankErrorDump = false;
                }

                error() << " -> Ingress header : " << ingressWord << endmsg;
                error() << "  -> ";
                rawDump( error(), ingressWord.data() );
                error() << endmsg;
                error() << " -> HPD data block : " << endmsg;
                error() << "  -> HPD Event ID = " << hpdBank->eventID() << " ";
                rawDump( error(), hpdBank->eventID().data(), hpdBank->eventID().activeBits() );
                error() << endmsg;
                error() << *hpdBank << endmsg;
              }

            } // HPD ID OK

          } // ODIN OK and not suppressed

          // Increment line number to next data block
          lineC += hpdBank->nTotalWords();

        } // active HPDs

      } // ingress not suppressed

      // clear the inputs for next time
      inputs.clear();

    } // bank while loop

  } // data bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
  decodedData.addToActivePDs( nHPDbanks );

  // debug printout
  _ri_debug << "Decoded " << boost::format( "%2i" ) % ( nHPDbanks[Rich::Rich1] + nHPDbanks[Rich::Rich2] );
  _ri_debug << " PDs from Level1 Bank ID = " << boost::format( "%2i" ) % L1ID.data();
  _ri_debug << " : Size " << boost::format( "%4i" ) % ( bank.size() / 4 ) << " words : Version " << version << endmsg;
}

//=============================================================================

void RawBankDecoder::decodeToSmartIDs_MaPMT0( const LHCb::RawBank& bank,      //
                                              const DeRichSystem&  deRichSys, //
                                              L1Map&               decodedData ) const {

  using namespace Rich::DAQ::HPD; // to be changed...

  // Get L1 ID
  const Level1HardwareID L1ID( bank.sourceID() );
  _ri_debug << "FlatList Decoding L1 bank " << L1ID << endmsg;

  // Data bank size in 32 bit words
  const auto bankSize = bank.size() / 4;

  // various counts
  DetectorArray<unsigned int> decodedHits{{0, 0}};

  // If we have some words to process, start the decoding
  if ( bankSize > 0 ) {

    // Get Ingress map to decode into for this L1 board
    decodedData.emplace_back( std::piecewise_construct, std::forward_as_tuple( L1ID ), std::forward_as_tuple() );
    auto& ingressMap = decodedData.back().second;

    // reserve size
    ingressMap.reserve( NumIngressPerL1 );

    // cache the last PD info
    PDInfo*           last_pdInfo = nullptr;
    LHCb::RichSmartID last_pdID;

    // Loop over bank, Fill data into RichSmartIDs
    int lineC( 0 );
    while ( lineC < bankSize ) {
      // Read the smartID direct from the banks
      LHCb::RichSmartID id( LHCb::RichSmartID32( bank.data()[lineC++] ) );
      const bool        isLarge = deRichSys.isLargePD( id );
      if ( id.isLargePMT() != isLarge ) {
        ++m_pmtSLFlagMismatch;
        // hack to fix up large PMT flag
        id.setLargePMT( isLarge );
      }
      // OK ?
      if ( UNLIKELY( !id.isValid() ) ) {
        ++m_flatListReadWarn;
      } else {
        _ri_debug << " -> " << id << endmsg;

        // Get the L1 input from the DB
        const auto l1Input = deRichSys.level1InputNum( id );

        // The ingress info to fill
        IngressInfo* ingressInfo = nullptr;

        // Do we have an entry for this Ingress ID ?
        const auto inIt = std::find_if( ingressMap.begin(), ingressMap.end(),
                                        [&l1Input]( const auto& i ) { return l1Input.ingressID() == i.first; } );
        if ( UNLIKELY( inIt == ingressMap.end() ) ) {
          ingressMap.emplace_back( std::piecewise_construct, std::forward_as_tuple( l1Input.ingressID() ),
                                   std::forward_as_tuple() );
          ingressInfo = &( ingressMap.back().second );
          L1IngressHeader iHeader;
          iHeader.setIngressID( l1Input.ingressID() );
          ingressInfo->setIngressHeader( iHeader );
          // reserve size (guess as we don't know here...)
          ingressInfo->pdData().reserve( 32 );
        } else {
          // use existing entry
          ingressInfo = &( inIt->second );
        }

        // the PD data map
        auto& pdMap = ingressInfo->pdData();

        // The PD info object to fill
        PDInfo* pdInfo = nullptr;

        // The PD ID
        const auto pdID = id.pdID();

        // The RICH
        const auto rich = id.rich();

        // Has PD changed ?
        if ( pdID != last_pdID || !last_pdInfo ) {

          // Does this PD have an entry ?
          const auto pdIt =
              std::find_if( pdMap.begin(), pdMap.end(), [&l1Input]( const auto& i ) { return l1Input == i.first; } );
          if ( UNLIKELY( pdIt == pdMap.end() ) ) {
            // make a new entry
            pdMap.emplace_back( std::piecewise_construct, std::forward_as_tuple( l1Input ), std::forward_as_tuple() );
            pdInfo = &( pdMap.back().second );
            // Set the PD ID
            pdInfo->setPdID( pdID );
            // set the header
            // PDInfo::Header header;
            // CRJ - Comment out until decide what to do about maPMT Level0 IDs ...
            //           const Level0ID l0id = m_richSys->level0ID(id);
            //           if ( ! header.setL0ID( l0id ) )
            //           {
            //             std::ostringstream mess;
            //             mess << "Failed to set L0ID " << l0id;
            //             Warning( mess.str() ).ignore();
            //           }
            // pdInfo->setHeader( header );
            // reserve size (guess) in hit vector
            pdInfo->smartIDs().reserve( 16 );
            // Add to active PD count for current rich
            decodedData.addToActivePDs( rich );
          } else {
            // use found entry
            pdInfo = &( pdIt->second );
          }

          // update the PD cache
          last_pdID   = pdID;
          last_pdInfo = pdInfo;
        } else {
          // use last PD cache
          pdInfo = last_pdInfo;
        }

        // add the hit to the list
        pdInfo->smartIDs().emplace_back( id );

        // count the hits
        ++decodedHits[rich];
      }
    }

  } // bank not empty

  // Add to the total number of decoded hits
  decodedData.addToTotalHits( decodedHits );
}

//=============================================================================

void RawBankDecoder::suppressHotPixels( const LHCb::RichSmartID&   hpdID, //
                                        LHCb::RichSmartID::Vector& newids ) const {

  // clean out hot pixels enabled at all ?
  if ( m_pixelsToSuppress ) {
    // Does this HPD have some pixels to suppress
    const auto iHPDSup = m_hotPixels.find( hpdID );
    if ( iHPDSup != m_hotPixels.end() ) {
      // temporary copy of original hit channels
      const auto oldids = newids;

      // clear hits before suppression
      newids.clear();

      // loop over original hits and save only those not supressed
      for ( const auto& ID : oldids ) {
        if ( iHPDSup->second.find( ID ) == iHPDSup->second.end() ) {
          // not suppressed, so keep
          newids.push_back( ID );
        }
      } // loop over pixels

    } // this HPD has pixels to suppress

  } // hot pixel suppression enabled
}

//=============================================================================

void RawBankDecoder::dumpRawBank( const LHCb::RawBank& bank, MsgStream& os ) const {

  // Get bank version and ID
  const Level1HardwareID L1ID( bank.sourceID() );
  const auto             version = bankVersion( bank );

  // Data bank size in words
  const auto bankSize = bank.size() / 4;

  std::ostringstream magicAsHex;
  magicAsHex << std::hex << bank.magic();
  os << "RawBank version=" << version << " L1ID=" << L1ID << " datasize=" //
     << bankSize << " magic=" << magicAsHex.str() << endmsg;

  // Printout raw data

  // Is this an empty bank ?
  if ( bankSize > 0 ) {
    const std::string& LINES = "-------------------------------------------------------------------"
                               "---------------------------------------------";

    // Bit numbers
    os << LINES << endmsg;
    os << "      bit |";
    for ( int iCol = 31; iCol >= 0; --iCol ) { os << boost::format( "%3i" ) % iCol; }
    os << endmsg;
    os << LINES << endmsg;
    typedef unsigned int Dtype;
    for ( const Dtype* dataW = bank.begin<Dtype>(); dataW != bank.end<Dtype>(); ++dataW ) {
      rawDump( os, *dataW );
      os << endmsg;
    }
    os << LINES << endmsg;
  } else {
    os << "  -> Bank is empty" << endmsg;
  }
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawBankDecoder )

//=============================================================================
