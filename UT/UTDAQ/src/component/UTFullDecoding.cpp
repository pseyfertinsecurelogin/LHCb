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
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/UTTELL1Data.h"
#include "GaudiAlg/Transformer.h"
#include "Kernel/IUTReadoutTool.h"
#include "Kernel/UTAlgBase.h"
#include "Kernel/UTDAQDefinitions.h"
#include "Kernel/UTRawBankMap.h"
#include "Kernel/UTTell1Board.h"
#include "Kernel/UTTell1ID.h"
#include "boost/lexical_cast.hpp"
#include <bitset>

/** @class UTFullDecoding UTFullDecoding.h
 *
 *  Algorithm to decode the NZS UT data in the RawEvent buffer into UUTELL1Data
 *  objects. Job options:
 *  - \b PrintErrorInfo: Flag to print out errors from event info.
 *  - \b InputLocation: Location of RawEvent
 *  - \b OutputLocation: Location of NZS output data, e.g. UTFull
 *  - \b EventInfoLocation: Location of event info data
 *  \sa{https://edms.cern.ch/document/692431/3}.
 *
 *  @author Andy Beiter (based on code by Mathias Knecht, Jeroen van Tilburg)
 *  @date   2018-09-04
 */
class UTFullDecoding
    : public Gaudi::Functional::MultiTransformer<std::tuple<LHCb::UTTELL1Datas, LHCb::UTTELL1EventInfos>(
                                                     const LHCb::RawEvent& ),
                                                 Gaudi::Functional::Traits::BaseClass_t<UT::AlgBase>> {

public:
  /// Standard constructor
  UTFullDecoding( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override; ///< Algorithm initialization
  std::tuple<LHCb::UTTELL1Datas, LHCb::UTTELL1EventInfos>
  operator()( const LHCb::RawEvent& ) const override; ///< Algorithm execution

private:
  LHCb::RawBank::BankType m_bankType;

  // job options
  Gaudi::Property<bool> m_printErrorInfo{this, "PrintErrorInfo", true}; ///< Flag to print out errors from event info
};

using namespace LHCb;
using namespace UTDAQ;

//-----------------------------------------------------------------------------
// Implementation file for class : UTFullDecoding
//
// 2007-09-11: Mathias Knecht, Jeroen van Tilburg
//-----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UTFullDecoding )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UTFullDecoding::UTFullDecoding( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer{name,
                       pSvcLocator,
                       {"InputLocation", RawEventLocation::Default},
                       {KeyValue{"OutputLocation", UTTELL1DataLocation::UTFull},
                        KeyValue{"EventInfoLocation", UTTELL1EventInfoLocation::UTEventInfo}}} {

  setForcedInit();
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode UTFullDecoding::initialize() {
  return UT::AlgBase::initialize().andThen( [&] {
    // initialize bank type
    m_bankType = UTRawBankMap::stringToType( detType() + "Full" );
  } );
}

//=============================================================================
// Main execution
//=============================================================================
std::tuple<UTTELL1Datas, UTTELL1EventInfos> UTFullDecoding::operator()( const LHCb::RawEvent& raw ) const {
  // Get the raw data
  std::tuple<UTTELL1Datas, UTTELL1EventInfos> output;
  // make container of TELL1 boards
  auto& [outputData, eventInfos] = output;

  // Initialize some counters
  unsigned int L0EvtID = 0;

  // Pick up UTFull bank
  const auto& itf = raw.banks( RawBank::BankType( m_bankType ) );
  if ( msgLevel( MSG::DEBUG ) ) {
    debug() << "Starting to decode " << itf.size() << detType() << "Full bank(s)" << endmsg;
  }

  int nBank = 0;
  for ( const RawBank* p : itf ) {

    // Create an empty tell1 data object
    UTTELL1Data::Data tell1Data;
    tell1Data.resize( noptlinks );
    for ( auto& i : tell1Data ) i.resize( nports * nstrips, 0 );

    // Create an empty tell1 header object
    UTTELL1Data::Data tell1Header;
    tell1Header.resize( noptlinks );

    for ( auto j = tell1Header.begin(); j != tell1Header.end(); ++j ) { j->resize( nports * nheaders, 0 ); }

    // Create an empty eventInfo object
    UTTELL1Data::Info eventInfo;
    for ( unsigned int i = 0; i < npp; ++i ) {
      UTTELL1EventInfo* evtInfo = new UTTELL1EventInfo();
      eventInfo.push_back( evtInfo );
      eventInfos.insert( evtInfo );
    }
    std::vector<unsigned int> sentPP;

    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << "Decoding bank number [" << nBank++ << "] of type " << detType() << "Full (TELL1 ID: " << p->sourceID()
              << ", Size: " << p->size() << " bytes)" << endmsg;
    }

    // Check if the board is valid
    UTTell1ID           tell1ID = UTTell1ID( (unsigned int)p->sourceID(), detType() == "UT" );
    const UTTell1Board* aBoard  = readoutTool()->findByBoardID( tell1ID );
    if ( !aBoard ) { // not a valid b
      std::string invalidSource =
          "Invalid source ID --> skip bank " + boost::lexical_cast<std::string>( p->sourceID() );
      Warning( invalidSource, StatusCode::SUCCESS, 2 ).ignore();
      ++counter( "Skipped banks" );
      continue;
    }

    if ( (unsigned int)p->size() % nwordsFull != 0 ) {
      error() << "Wrong bank size for this type!! You should have multiple of " << nwordsFull << " bytes" << endmsg;
    }

    // Counters
    unsigned int cntWD = 0; // Word counter, resets for each PP. Range 0 to 223.
    unsigned int cntPP = 0; // PP-FPGA counter, goes from 0 to 3.

    // Now loop over all WORDS in a bank

    for ( const unsigned int* w = p->begin<unsigned int>(); w != p->end<unsigned int>(); ++w ) {

      if ( cntWD % 224 == 0 ) { // Each 224 words we have a new PP-FPGA
        cntWD = 0;
        cntPP = ( *( w + 219 ) & UTTELL1EventInfo::ChipAddrMask ) >> UTTELL1EventInfo::ChipAddrBits;
        sentPP.push_back( cntPP );

        if ( msgLevel( MSG::DEBUG ) ) {
          debug() << "#######  Parsing now data from PP " << cntPP << " ##################" << endmsg;
        }
      }

      // Set the Tell1 sourceID for each PP
      UTTELL1EventInfo* evtInfo = eventInfo[cntPP];
      evtInfo->setSourceID( p->sourceID() );

      // Unpack the 32-bit word into 8-bit chunks
      unsigned int bits1 = 0;
      unsigned int bits2 = 8;
      unsigned int bits3 = 16;
      unsigned int bits4 = 24;
      unsigned int p1    = ( *w & mask1 ) >> bits1;
      unsigned int p2    = ( *w & mask2 ) >> bits2;
      unsigned int p3    = ( *w & mask3 ) >> bits3;
      unsigned int p4    = ( *w & mask4 ) >> bits4;

      if ( cntWD < 216 ) { // Words below 216 contain data and header
        int iPort   = cntWD / ( nbeetles * 36 );
        int iWord   = ( cntWD % ( nbeetles * 36 ) ) / nbeetles;
        int iBeetle = 2 * ( cntWD % nbeetles ) + nBeetlesPerPPx * cntPP;

        if ( iWord >= 1 && iWord < 4 ) { // Header part
          int iHeader                                           = iWord - 1;
          tell1Header[iBeetle][iHeader + 3 * iPort]             = p1;
          tell1Header[iBeetle][iHeader + 3 * ( iPort + 2 )]     = p2;
          tell1Header[iBeetle + 1][iHeader + 3 * iPort]         = p3;
          tell1Header[iBeetle + 1][iHeader + 3 * ( iPort + 2 )] = p4;
        } else if ( iWord >= 4 && iWord < 36 ) { // Data part
          int iChan                                          = iWord - 4;
          tell1Data[iBeetle][iChan + 32 * iPort]             = p1;
          tell1Data[iBeetle][iChan + 32 * ( iPort + 2 )]     = p2;
          tell1Data[iBeetle + 1][iChan + 32 * iPort]         = p3;
          tell1Data[iBeetle + 1][iChan + 32 * ( iPort + 2 )] = p4;
        }
      } else { // Words 216-223 contains Event Info
        switch ( cntWD ) {
        case 216: {
          evtInfo->setWord0( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Event Information (bits): " << std::bitset<8>( evtInfo->EventInformation() )
                    << " | Bank List (bits): " << std::bitset<8>( evtInfo->BankList() )
                    << " | Detector ID (dec): " << (unsigned int)evtInfo->DetectorID()
                    << " | Bunch Counter (dec): " << evtInfo->bCnt() << endmsg;
          }
          break;
        }

        case 217: {
          evtInfo->setWord1( *w );
          if ( msgLevel( MSG::DEBUG ) ) { debug() << "(Event Info) L0-EventID (dec): " << (int)*w << endmsg; }
          if ( L0EvtID == 0 ) {
            // For each bank, L0EvtID is initialized. So the first time in the
            // bank, L0EvtID is checked.
            L0EvtID = (unsigned int)evtInfo->L0EvID();
          } else {
            // The rest of the time (for all PPs, all TELL1), there's a check
            // that L0EvtID is the same for all.
            if ( (unsigned int)evtInfo->L0EvID() != L0EvtID ) {
              error() << "L0-Event ID not the same for all!" << endmsg;
            }
          }
          break;
        }
        case 218: {
          evtInfo->setWord2( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Reserved Bits (hex): " << std::hex << evtInfo->R1()
                    << " | Process Info (bits): " << std::bitset<8>( evtInfo->ProcessInfo() )
                    << " | PCN (from Beetle 0) (dec): " << std::dec << (unsigned int)evtInfo->pcn() << endmsg;
          }
          break;
        }
        case 219: {
          evtInfo->setWord3( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Chip Addr (dec): " << (unsigned int)evtInfo->ChipAddr()
                    << " | Sync RAM Full (bits): " << std::bitset<6>( evtInfo->SyncRAMFull() )
                    << " | TLK Link Loss (bits): " << std::bitset<6>( evtInfo->tlkLnkLoss() ) << endmsg;
            debug() << "(Event Info) | Sync Evt Size Err. (bits): " << std::bitset<6>( evtInfo->SyncEvtSizeError() )
                    << " | Opt. Link Disable (bits): " << std::bitset<6>( evtInfo->OptLnkDisable() )
                    << " | Opt. Link NoEvent (bits): " << std::bitset<6>( evtInfo->OptLnkNoEvt() ) << endmsg;
          }
          if ( m_printErrorInfo ) {
            if ( evtInfo->SyncRAMFull() != 0 )
              error() << "Sync RAM Full in TELL1 ID " << p->sourceID() << ", PP-FPGA " << cntPP
                      << ". Value (One bit per link): " << std::bitset<6>( evtInfo->SyncRAMFull() ) << endmsg;
            if ( evtInfo->tlkLnkLoss() != 0 )
              error() << "TLK Link loss in TELL1 ID " << p->sourceID() << ", PP-FPGA " << cntPP
                      << ". Value (One bit per link): " << std::bitset<6>( evtInfo->tlkLnkLoss() ) << endmsg;
            if ( evtInfo->SyncEvtSizeError() != 0 )
              error() << "Sync Event size error in TELL1 ID " << p->sourceID() << ", PP-FPGA " << cntPP
                      << ". Value (One bit per link): " << std::bitset<6>( evtInfo->SyncEvtSizeError() ) << endmsg;
            if ( evtInfo->OptLnkNoEvt() != 0 )
              error() << "Optical Link No Event in TELL1 ID " << p->sourceID() << ", PP-FPGA " << cntPP
                      << ". Value (One bit per link): " << std::bitset<6>( evtInfo->OptLnkNoEvt() ) << endmsg;
          }
          break;
        }
        case 220: {
          evtInfo->setWord4( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Reserved bit (bits): " << std::bitset<1>( evtInfo->R2() )
                    << " | PCN Error (bits):" << std::bitset<1>( evtInfo->pcnError() )
                    << " | Optical Link no clock (bits): " << std::bitset<6>( evtInfo->OptLnkNoClock() ) << endmsg;
            debug() << "(Event Info) | Header Pseudo Err. (bits): " << std::bitset<24>( evtInfo->HeaderPseudoError() )
                    << endmsg;
          }

          if ( m_printErrorInfo ) {
            if ( evtInfo->HeaderPseudoError() != 0 )
              error() << "Header Pseudo Error in TELL1 ID " << p->sourceID() << ", PP-FPGA " << cntPP
                      << ". Value (One bit per port=24 bits): " << std::bitset<24>( evtInfo->HeaderPseudoError() )
                      << endmsg;
          }
          break;
        }
        case 221: {
          evtInfo->setWord5( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Beetle3 PCN (dec): " << (unsigned int)evtInfo->pcnBeetle3()
                    << " | Beetle2 PCN (dec): " << (unsigned int)evtInfo->pcnBeetle2()
                    << " | Beetle1 PCN (dec): " << (unsigned int)evtInfo->pcnBeetle1()
                    << " | Beetle0 PCN (dec): " << (unsigned int)evtInfo->pcnBeetle0() << endmsg;
          }
          break;
        }
        case 222: {
          eventInfo[cntPP]->setWord6( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Reserved bits (hex): " << std::hex << evtInfo->R3()
                    << " | Beetle5 PCN (dec): " << std::dec << (unsigned int)evtInfo->pcnBeetle5()
                    << " | Beetle4 PCN (dec): " << (unsigned int)evtInfo->pcnBeetle4() << endmsg;
          }
          break;
        }
        case 223: {
          eventInfo[cntPP]->setWord7( *w );
          if ( msgLevel( MSG::DEBUG ) ) {
            debug() << "(Event Info) Reserved bits (hex): " << std::hex << (unsigned int)evtInfo->R4()
                    << " | I Headers: Beet.5 (dec): " << std::dec << (unsigned int)evtInfo->iHeaderBeetle5()
                    << " | Beet.4: " << (unsigned int)evtInfo->iHeaderBeetle4()
                    << " | Beet.3: " << (unsigned int)evtInfo->iHeaderBeetle3()
                    << " | Beet.2: " << (unsigned int)evtInfo->iHeaderBeetle2()
                    << " | Beet.1: " << (unsigned int)evtInfo->iHeaderBeetle1()
                    << " | Beet.0: " << (unsigned int)evtInfo->iHeaderBeetle0() << std::dec << endmsg;
          }
          break;
        }
        default:
          error() << "Not the right number of words: word number " << cntWD << ", you should have 224 words per PP"
                  << endmsg;
        }
      }

      cntWD++;
    } // Loop over all words

    // make an empty tell1 data object
    UTTELL1Data* myData = new UTTELL1Data( tell1Data, tell1Header, sentPP, eventInfo );

    // put into the container, second argument is TELL1 id
    outputData.insert( myData, int( p->sourceID() ) );

  } // end of loop over banks of a certain type

  return output;
}
