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
// ROOT
#include "TAxis.h"
#include "TH2D.h"

// Gaudi
#include "GaudiAlg/FunctionalUtilities.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiUtils/Aida2ROOT.h"
#include "GaudiUtils/HistoLabels.h"

// LHCb
// Event/DAQEvent
#include "Event/ODIN.h"
#include "Event/RawEvent.h"
// Event/DigiEvent
#include "Event/HCDigit.h"
// Kernel/LHCbKernel
#include "Kernel/HCCellID.h"

// Local
#include "HCRawBankDecoder.h"

using namespace Gaudi::Utils::Histos;

DECLARE_COMPONENT( HCRawBankDecoder )

//=============================================================================
// Constructor
//=============================================================================
HCRawBankDecoder::HCRawBankDecoder( const std::string& name, ISvcLocator* pSvcLocator )
    : MultiTransformer( name, pSvcLocator,
                        {KeyValue{"RawEventLocations", Gaudi::Functional::concat_alternatives(
                                                           {LHCb::RawEventLocation::HC, LHCb::RawEventLocation::Default,
                                                            LHCb::RawEventLocation::Other} )},
                         KeyValue{"OdinLocation", LHCb::ODINLocation::Default}},
                        {KeyValue{"DigitLocation", LHCb::HCDigitLocation::Default},
                         KeyValue{"L0DigitLocation", LHCb::HCDigitLocation::L0}} ) {
  declareProperty( "SkipErrorBanks", m_skipErrorBanks = false );
  declareProperty( "SkipTriggerWords", m_skipTrigger = false );
  declareProperty( "SkipAdcWords", m_skipAdc = false );
  declareProperty( "Monitoring", m_monitoring = false );
}

//=============================================================================
// Initialisation
//=============================================================================
StatusCode HCRawBankDecoder::initialize() {

  StatusCode sc = MultiTransformer::initialize();
  if ( sc.isFailure() ) return sc;

  // Book histograms.
  for ( unsigned int i = 0; i < 2; ++i ) {
    const std::string name  = "ErrorBanks/DiffBX/Crate" + std::to_string( i );
    const std::string title = "Crate " + std::to_string( i );
    m_hBxDiff.push_back( book1D( name, title, -20.5, 20.5, 41 ) );
    setAxisLabels( m_hBxDiff[i], "BX_{FE} - BX_{ODIN}", "Entries" );
  }
  m_hLinkErrors  = book2D( "ErrorBanks/Links", "Errors", -0.5, 1.5, 2, -0.5, 15.5, 16 );
  m_hTell1Errors = book2D( "RawBanks/ErrorBitsTELL1", "Errors", -0.5, 2.5, 3, -0.5, 1.5, 2 );
  TH2D* h        = Gaudi::Utils::Aida2ROOT::aida2root( m_hLinkErrors );
  if ( h ) {
    TAxis* axis = h->GetYaxis();
    if ( axis ) {
      for ( unsigned int i = 0; i < 4; ++i ) {
        for ( unsigned int j = 0; j < 4; ++j ) {
          const unsigned int bin   = 16 - ( 4 * i + j );
          const std::string  label = "PP" + std::to_string( i ) + " L" + std::to_string( j );
          axis->SetBinLabel( bin, label.c_str() );
        }
      }
    }
    axis = h->GetXaxis();
    if ( axis ) {
      axis->SetBinLabel( 1, "Link Error" );
      axis->SetBinLabel( 2, "Synchronisation Error" );
    }
  }
  h = Gaudi::Utils::Aida2ROOT::aida2root( m_hTell1Errors );
  if ( h ) {
    TAxis* axis = h->GetYaxis();
    if ( axis ) {
      axis->SetBinLabel( 1, "Crate 0" );
      axis->SetBinLabel( 2, "Crate 1" );
    }
    axis = h->GetXaxis();
    if ( axis ) {
      axis->SetBinLabel( 1, "General" );
      axis->SetBinLabel( 2, "Synchronisation" );
      axis->SetBinLabel( 3, "Link" );
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Execution
//=============================================================================
std::tuple<LHCb::HCDigits, LHCb::HCDigits> HCRawBankDecoder::operator()( const LHCb::RawEvent& rawEvent,
                                                                         const LHCb::ODIN&     odin ) const {
  // Create containers
  std::tuple<LHCb::HCDigits, LHCb::HCDigits> output;

  auto& digits    = std::get<0>( output );
  auto& l0_digits = std::get<1>( output );

  // Retrieve the HC raw banks.
  const auto& banks = rawEvent.banks( LHCb::RawBank::HC );
  if ( banks.empty() ) { Warning( "Cannot find HC raw banks", StatusCode::SUCCESS ).ignore(); }

  for ( const auto& bank : banks ) {
    // Make sure the bank is not corrupted.
    if ( LHCb::RawBank::MagicPattern != bank->magic() ) {
      error() << "Bad magic pattern" << endmsg;
      continue;
    }
    const unsigned int source  = bank->sourceID();
    const unsigned int version = bank->version();
    if ( msgLevel( MSG::DEBUG ) ) { debug() << "Source: " << source << ", version: " << version << endmsg; }
    // Decode the raw bank.
    if ( version != 2 && version != 3 ) {
      throw GaudiException( "Unknown raw bank version (" + std::to_string( version ) + ")", name(),
                            StatusCode::FAILURE );
    }
    decode( *bank, digits, l0_digits );
  }

  if ( !m_skipErrorBanks ) {
    // Get event information from ODIN.
    const auto bxid = odin.bunchId();

    auto errorBanks = rawEvent.banks( LHCb::RawBank::HCError );
    if ( errorBanks.empty() && msgLevel( MSG::DEBUG ) ) { debug() << "No error banks in raw event." << endmsg; }
    for ( const auto& bank : errorBanks ) {
      // Make sure the bank is not corrupted.
      if ( LHCb::RawBank::MagicPattern != bank->magic() ) {
        error() << "Bad magic pattern in error bank" << endmsg;
        continue;
      }
      decodeErrorBank( *bank, bxid );
    }
  }
  return output;
}

//=============================================================================
// Decoding of raw banks (compressed format, PS/SPD)
//=============================================================================
void HCRawBankDecoder::decode( const LHCb::RawBank& bank, LHCb::HCDigits& digits, LHCb::HCDigits& l0_digits ) const {
  const uint32_t* data   = bank.data();
  unsigned int    nWords = bank.size() / sizeof( uint32_t );
  while ( nWords > 0 ) {
    // Read the bank header.
    uint32_t word = *data++;
    --nWords;
    // Length of the trigger part.
    unsigned int lenTrig = word & 0x7F;
    // Length of the ADC part.
    unsigned int       lenAdc = ( word >> 7 ) & 0x7F;
    const unsigned int card   = ( word >> 14 ) & 0xF;
    const unsigned int crate  = ( word >> 18 ) & 0x1F;
    // Control word.
    const unsigned int ctrl = ( word >> 23 ) & 0x1FF;
    if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) {
      verbose() << format( "Crate: %02u, card: %02u, ", crate, card ) << format( "trigger part: %03u bytes, ", lenTrig )
                << format( "ADC part: %03u bytes", 2 * lenAdc ) << endmsg;
      const unsigned int ttype = ( ctrl >> 1 ) & 0xF;
      const unsigned int chk   = ( ctrl >> 7 ) & 0x2;
      verbose() << format( "Trigger type: 0x%1x, ", ttype ) << "control bits: " << chk << endmsg;
    }
    const unsigned int generalError = ctrl & 0x1;
    const unsigned int syncError    = ctrl & 0x20;
    const unsigned int linkError    = ctrl & 0x40;
    if ( generalError != 0 || syncError != 0 || linkError != 0 ) {
      if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) {
        debug() << "Tell1 error bits have been detected in data" << endmsg;
        if ( 0 != generalError ) debug() << "General error" << endmsg;
        if ( 0 != syncError ) debug() << "Synchronisation error" << endmsg;
        if ( 0 != linkError ) debug() << "Link error" << endmsg;
      }
      if ( m_monitoring ) {
        if ( 0 != generalError ) m_hTell1Errors->fill( 0, crate );
        if ( 0 != syncError ) m_hTell1Errors->fill( 1, crate );
        if ( 0 != linkError ) m_hTell1Errors->fill( 2, crate );
      }
    }
    unsigned int offset = 32;
    // Read the trigger data.
    if ( m_skipTrigger ) {
      const unsigned int nSkip = ( lenTrig + 3 ) / 4;
      data += nSkip;
      nWords -= nSkip;
    } else {
      while ( lenTrig > 0 ) {
        if ( 32 == offset ) {
          // Read the next word.
          word = *data++;
          --nWords;
          offset = 0;
        }
        const unsigned int channel = ( word >> offset ) & 0x3F;
        const int          prs     = ( word >> ( offset + 6 ) ) & 1;
        const int          spd     = ( word >> ( offset + 7 ) ) & 1;
        const int          adc     = prs;
        if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) {
          verbose() << format( "Channel: %06u ", channel ) << format( "PRS: %01i ", prs ) << format( "SPD: %01i ", spd )
                    << endmsg;
        }
        LHCb::HCCellID cell( channel | ( crate << 6 ) );
        // Make sure there isn't already an object ID.
        if ( l0_digits.object( cell ) ) {
          warning() << "Duplicated cell ID " << cell << endmsg;
        } else {
          l0_digits.insert( new LHCb::HCDigit( adc ), cell );
        }
        offset += 8;
        --lenTrig;
      }
    }
    if ( m_skipAdc ) {
      const unsigned int nSkip = ( lenAdc + 1 ) / 2;
      data += nSkip;
      nWords -= nSkip;
      continue;
    }
    // Read the ADC data
    offset = 32;
    while ( lenAdc > 0 ) {
      if ( 32 == offset ) {
        // Read the next word.
        word = *data++;
        --nWords;
        offset = 0;
      }
      const int          adc     = ( word >> offset ) & 0x3FF;
      const unsigned int channel = ( word >> ( offset + 10 ) ) & 0x3F;
      --lenAdc;
      offset += 16;
      LHCb::HCCellID cell( channel | crate << 6 );
      if ( digits.object( cell ) ) {
        warning() << "Duplicated cell ID " << cell << endmsg;
      } else {
        digits.insert( new LHCb::HCDigit( adc ), cell );
      }
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) {
        verbose() << format( "Channel: %06u ", channel ) << format( "ADC: %06i", adc ) << endmsg;
      }
    }
  }
}

//=============================================================================
// Decoding of error raw banks
//=============================================================================
void HCRawBankDecoder::decodeErrorBank( const LHCb::RawBank& bank, const int bxid ) const {

  const uint32_t*    data = bank.data();
  const unsigned int nPP  = 4;
  for ( unsigned int i = 0; i < nPP; ++i ) {
    const auto evtrl     = *data;
    const auto bcnt      = evtrl & 0xFFF;
    const auto detid     = ( evtrl >> 12 ) & 0xF;
    const auto bankList  = ( evtrl >> 16 ) & 0xFF;
    const auto eventInfo = ( evtrl >> 24 ) & 0xFF;
    if ( msgLevel( MSG::VERBOSE ) ) {
      verbose() << std::string( 60, '=' ) << endmsg;
      verbose() << "PP FPGA " << i << endmsg;
      verbose() << std::string( 60, '=' ) << endmsg;
      verbose() << "Bunch counter: " << bcnt << endmsg;
      verbose() << format( "Detector ID: 0x%x", detid ) << endmsg;
      verbose() << format( "Bank list: 0x%x", bankList ) << endmsg;
      if ( eventInfo != 0 ) { verbose() << format( "Event info: 0x%x", eventInfo ) << endmsg; }
    }
    ++data;
    const auto l0cnt = *data;
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << "L0 event counter: " << l0cnt << endmsg; }
    data += 3;
    const auto errorBankLength = ( *data >> 16 ) & 0xFFFF;
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << "Error bank length: " << errorBankLength << endmsg; }
    if ( errorBankLength == 0 ) {
      data += 2;
      continue;
    }
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << "Link  Sync Err  Link Err  Crate  Card   L0   BX" << endmsg; }
    for ( unsigned int j = 0; j < 4; ++j ) {
      ++data;
      const auto word    = *data;
      const auto febx    = word & 0x3FF;
      const auto fel0    = ( word >> 10 ) & 0x3FF;
      const auto card    = ( word >> 20 ) & 0xF;
      const auto crate   = ( word >> 24 ) & 0x1F;
      const auto errLink = ( word >> 29 ) & 0x1;
      const auto errSync = ( word >> 30 ) & 0x1;
      if ( msgLevel( MSG::VERBOSE ) ) {
        verbose() << j << "         " << errSync << "         " << errLink << "      "
                  << format( "%2u    %2u   %4u  %4u", crate, card, fel0, febx ) << endmsg;
      }
      if ( m_monitoring ) {
        const unsigned int bin = 15 - ( 4 * i + j );
        if ( errLink > 0 ) m_hLinkErrors->fill( 0, bin );
        if ( errSync > 0 ) m_hLinkErrors->fill( 1, bin );
        m_hBxDiff[crate]->fill( int( febx ) - bxid );
      }
    }
    data += 2;
  }
}
