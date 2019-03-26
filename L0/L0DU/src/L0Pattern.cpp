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
// Include files
#include "Kernel/STLExtensions.h"
// L0Event
#include "Event/L0DUReport.h"
#include <algorithm>
// local
#include "L0Pattern.h"

//-----------------------------------------------------------------------------
// Implementation file for class : L0Pattern
//
// 2008-03-26 : Olivier Deschamps
//
// Produce L0 pattern to feed L0DU board test-benches
//
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( L0Pattern )

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode L0Pattern::initialize() {

  StatusCode sc = GaudiTupleAlg::initialize();
  if ( sc.isFailure() ) { return Error( "Could not initialize base class CaudiAlgorithm", sc ); }
  info() << "==> Initialize" << endmsg;

  // get the tools
  m_fromRaw  = tool<IL0DUFromRawTool>( m_fromRawTool, m_fromRawTool, this );
  m_emulator = tool<IL0DUEmulatorTool>( m_emulatorTool, m_emulatorTool );
  m_config   = tool<IL0DUConfigProvider>( "L0DUMultiConfigProvider", m_configTool );

  // Check
  if ( m_list.value().empty() ) return Error( "Empty list of TCK" );
  for ( auto it = m_list.value().begin(); it != m_list.value().end(); ++it ) {
    for ( auto jt = std::next( it ); jt != m_list.value().end(); ++jt ) {
      if ( ( *it ) == ( *jt ) ) {
        warning() << "The requested TCK " << *it << " appears twice in the TCKList " << endmsg;
        warning() << "Emulator may give wrong result due to  downscaling - please clean the list ..." << endmsg;
        return StatusCode::FAILURE;
      }
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode L0Pattern::execute() {
  using namespace LHCb;

  Tuple ntp = nTuple( 500, "L0DU pattern", CLID_ColumnWiseTuple );

  // get processor data
  if ( !m_fromRaw->decodeBank() ) {
    Error( "Unable to decode L0DU rawBank", StatusCode::SUCCESS ).ignore();
    return StatusCode::SUCCESS;
  }
  if ( m_fromRaw->version() == 0 ) {
    Error( "L0Pattern production only works with bank version > 0" ).ignore();
    return StatusCode::SUCCESS;
  }

  m_datas = m_fromRaw->L0ProcessorDatas();

  // built 12 bits BCID
  unsigned long sature = 0xDEB;
  if ( m_bcid > sature ) m_bcid = 0;
  // set bcid to the L0DU data words
  if ( m_setbcid ) {
    encode( m_bcid, L0DUBase::Electron::BCID );
    encode( m_bcid, L0DUBase::Photon::BCID );
    encode( m_bcid, L0DUBase::Hadron::BCID );
    encode( m_bcid, L0DUBase::Pi0Global::BCID );
    encode( m_bcid, L0DUBase::Pi0Local::BCID );
    encode( m_bcid, L0DUBase::Sum::BCID );
    encode( m_bcid, L0DUBase::Spd::BCID );
    encode( m_bcid, L0DUBase::Muon1::BCID1 );
    encode( m_bcid, L0DUBase::Muon1::BCID2 );
    encode( m_bcid, L0DUBase::Muon3::BCID1 );
    encode( m_bcid, L0DUBase::Muon3::BCID2 );
    encode( m_bcid, L0DUBase::Muon5::BCID1 );
    encode( m_bcid, L0DUBase::Muon5::BCID2 );
    encode( m_bcid, L0DUBase::Muon7::BCID1 );
    encode( m_bcid, L0DUBase::Muon7::BCID2 );
    encode( m_bcid, L0DUBase::PileUp::BCID1 );
    encode( m_bcid, L0DUBase::PileUp::BCID2 );
  }

  // RSDA vs TCK
  std::vector<int> rsda, tcks, report;
  // Run emulator over various configuration
  for ( const auto& l : m_list ) {
    if ( l.compare( 0, 2, "0x" ) != 0 ) {
      Error( "SKIP the requested TCK value " + l + " (MUST be in hexadecimal format '0x" + l + "')" ).ignore();
      continue;
    }
    int itck = std::stoi( l, nullptr, 16 );
    ;
    LHCb::L0DUConfig* config = m_config->config( itck );
    if ( !config ) return Error( "Unknown TCK", StatusCode::SUCCESS );
    m_emulator->process( config, m_datas );

    unsigned int rs = 0;
    switch ( m_fromRaw->version() ) {
    case 1: {
      rs = ( m_bcid & 0xFFF );
      rs |= ( config->emulatedDecision( LHCb::L0DUDecision::Physics ) ) << 12;
      break;
    }
    case 2: {
      rs = ( m_bcid & 0x3FF );
      rs |= ( config->emulatedDecision( LHCb::L0DUDecision::Physics ) ) << 12;
      rs |= ( config->emulatedDecision( LHCb::L0DUDecision::Beam2 ) ) << 11;
      rs |= ( config->emulatedDecision( LHCb::L0DUDecision::Beam1 ) ) << 10;
      break;
    }
    }

    rsda.push_back( rs );
    tcks.push_back( itck );
    // channel pattern
    LHCb::L0DUChannel::Map channels = config->channels();
    long                   pattern  = std::accumulate( channels.begin(), channels.end(), 0L,
                                    []( long p, const std::pair<std::string, LHCb::L0DUChannel*> c ) {
                                      const LHCb::L0DUChannel* channel = c.second;
                                      return p | ( channel->emulatedDecision() << channel->id() );
                                    } );
    report.push_back( pattern );

    // printout
    debug() << " === TCK=" << l << "====" << endmsg;
    for ( const auto& c : channels ) {
      // debug() << "   - channel : " <<c.first <<  " decision = " << ic.second->emulatedDecision() << endmsg;
      debug() << "   - channel : " << c.first << " decision = " << c.second->summary() << endmsg;
    }
  }

  ntp->farray( "rsda", rsda, "nConfig", m_list.size() ).ignore();
  ntp->farray( "tck", tcks, "nConfig", m_list.size() ).ignore();
  ntp->farray( "report", report, "nConfig", m_list.size() ).ignore();

  // DATA
  ntp->farray( {{"word", []( LHCb::L0ProcessorData* pd ) { return pd->word(); }},
                {"lsb", []( LHCb::L0ProcessorData* pd ) { return pd->lsb(); }},
                {"msb", []( LHCb::L0ProcessorData* pd ) { return pd->msb(); }},
                {"fiber", []( LHCb::L0ProcessorData* pd ) { return pd->key(); }}},
               m_datas->begin(), m_datas->end(), "nData", 24 )
      .ignore();
  ntp->column( "bcid", m_bcid ).ignore();

  m_bcid++;
  return ntp->write();
}

void L0Pattern::encode( unsigned int data, const std::array<unsigned int, L0DUBase::Index::Size>& base ) {
  LHCb::L0ProcessorData* fiber = m_datas->object( base[L0DUBase::Index::Fiber] );
  unsigned int           word  = fiber->word();
  word |= ( ( data << base[L0DUBase::Index::Shift] ) & base[L0DUBase::Index::Mask] );
  fiber->setWord( word );
  if ( L0DUBase::Fiber::Empty != base[L0DUBase::Index::Fiber2] ) {
    fiber            = m_datas->object( base[L0DUBase::Index::Fiber2] );
    word             = fiber->word();
    unsigned int val = data >> base[L0DUBase::Index::Offset];
    word |= ( ( val << base[L0DUBase::Index::Shift2] ) & base[L0DUBase::Index::Mask2] );
    fiber->setWord( word );
  }
}
