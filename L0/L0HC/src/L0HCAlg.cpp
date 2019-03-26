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
#include "Event/HCDigit.h"
#include "Event/L0ProcessorData.h"

// Local
#include "L0HCAlg.h"

DECLARE_COMPONENT( L0HCAlg )

//=============================================================================
// Initialization
//=============================================================================
StatusCode L0HCAlg::initialize() {
  StatusCode sc = L0AlgBase::initialize();
  if ( sc.isFailure() ) return sc;

  // Set the thresholds.
  m_thresholds.assign( 5, std::vector<int>( 4, 0 ) );
  if ( m_triggerBitsFromADCs ) {
    if ( m_thresholdsB0.size() == 4 ) m_thresholds[0] = m_thresholdsB0;
    if ( m_thresholdsB1.size() == 4 ) m_thresholds[1] = m_thresholdsB1;
    if ( m_thresholdsB2.size() == 4 ) m_thresholds[2] = m_thresholdsB2;
    if ( m_thresholdsF1.size() == 4 ) m_thresholds[3] = m_thresholdsF1;
    if ( m_thresholdsF2.size() == 4 ) m_thresholds[4] = m_thresholdsF2;
  }

  m_channels.assign( 5, std::vector<unsigned int>( 4, 0 ) );
  // Check if the mapping is available in the conditions database.
  const std::string location = "Conditions/ReadoutConf/HC/Mapping";
  if ( existDet<Condition>( location ) ) {
    registerCondition( location, m_cond, &L0HCAlg::cacheMapping );
    // First update.
    sc = updMgrSvc()->update( this );
    if ( sc.isFailure() ) { return Error( "Cannot update mapping.", StatusCode::FAILURE ); }
  } else {
    warning() << "Cannot find " << location << " in database" << endmsg;
    mapChannels( m_channelsB0, 0, true );
    mapChannels( m_channelsB1, 1, true );
    mapChannels( m_channelsB2, 2, true );
    mapChannels( m_channelsF1, 1, false );
    mapChannels( m_channelsF2, 2, false );
  }

  // Check for meaningful setting of the flags
  if ( m_triggerBitsFromADCs && m_fakeHCL0Digits ) {
    return Error( "Requested to fake Herschel L0 trigger bits *and* derive them from raw data. Choose one." );
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
// Execute
//=============================================================================
StatusCode L0HCAlg::execute() {

  // Retrieve the Herschel (L0) digits.
  const auto      location = m_triggerBitsFromADCs ? m_digitLocation : m_l0digitLocation;
  LHCb::HCDigits* digits   = getIfExists<LHCb::HCDigits>( location );
  if ( !digits ) {
    Warning( "Cannot retrieve HC digits from " + location ).ignore();
    Warning( "Will assume all counters are above threshold." ).ignore();
    m_fakeHCL0Digits = true;
  }

  const unsigned int multB = m_fakeHCL0Digits ? 12 : multiplicity( *digits, true );
  const unsigned int multF = m_fakeHCL0Digits ? 8 : multiplicity( *digits, false );

  // Save the candidates in HCProcessor data location (for L0DU).
  LHCb::L0ProcessorDatas* L0HC = new LHCb::L0ProcessorDatas();
  put( L0HC, LHCb::L0ProcessorDataLocation::HC );

  // Using the same bit-shift as for Calo Pi0Global/Local data word; Et goes in
  // bits 1-8 of the 32
  const unsigned int codeB = 0x10000 + ( multB << L0DUBase::Calo::Et::Shift );
  L0HC->add( new LHCb::L0ProcessorData( L0DUBase::Fiber::CaloPi0Global, codeB ) );

  const unsigned int codeF = 0x10000 + ( multF << L0DUBase::Calo::Et::Shift );
  L0HC->add( new LHCb::L0ProcessorData( L0DUBase::Fiber::CaloPi0Local, codeF ) );

  if ( msgLevel( MSG::DEBUG ) ) {
    // Print the L0 candidates
    for ( LHCb::L0ProcessorData* cand : *L0HC ) {
      debug() << format( "Key %2d Word %8x", cand->key(), cand->word() ) << endmsg;
    }
  }

  return StatusCode::SUCCESS;
}

unsigned int L0HCAlg::multiplicity( const LHCb::HCDigits& digits, const bool bwd ) const {
  const unsigned int i0 = bwd ? 0 : 3;
  const unsigned int i1 = bwd ? 3 : 5;

  unsigned int sum = 0;
  // Loop over the stations.
  for ( unsigned int i = i0; i < i1; ++i ) {
    // Loop over the quadrants.
    for ( unsigned int j = 0; j < 4; ++j ) {
      // Find the cell ID for this quadrant.
      LHCb::HCCellID id( m_channels[i][j] );
      // Retrieve the digit.
      const LHCb::HCDigit* digit = digits.object( id );
      if ( !digit ) {
        const std::string ch =
            bwd ? "B" + std::to_string( i ) + std::to_string( j ) : "F" + std::to_string( i - 2 ) + std::to_string( j );
        Warning( "Cannot retrieve digit for " + ch ).ignore();
        continue;
      }
      if ( digit->adc() > m_thresholds[i][j] ) ++sum;
    }
  }
  return sum;
}

//=============================================================================
//  Save the Herschel channel mapping
//=============================================================================
StatusCode L0HCAlg::cacheMapping() {
  // Extract crate IDs and channel numbers from CondDB
  m_crateB     = m_cond->param<int>( "CrateB" );
  m_crateF     = m_cond->param<int>( "CrateF" );
  m_channelsB0 = m_cond->paramVect<int>( "ChannelsB0" );
  m_channelsB1 = m_cond->paramVect<int>( "ChannelsB1" );
  m_channelsB2 = m_cond->paramVect<int>( "ChannelsB2" );
  m_channelsF1 = m_cond->paramVect<int>( "ChannelsF1" );
  m_channelsF2 = m_cond->paramVect<int>( "ChannelsF2" );
  mapChannels( m_channelsB0, 0, true );
  mapChannels( m_channelsB1, 1, true );
  mapChannels( m_channelsB2, 2, true );
  mapChannels( m_channelsF1, 1, false );
  mapChannels( m_channelsF2, 2, false );
  return StatusCode::SUCCESS;
}

//=============================================================================
// Setup the channel map for a given station.
//=============================================================================
bool L0HCAlg::mapChannels( const std::vector<int>& channels, const unsigned int station, const bool bwd ) {
  // Indices 0, 1, 2 are B-side; 3, 4 are F-side.
  const unsigned int offset = bwd ? 0 : 2;

  // Check if the input is valid.
  if ( channels.size() != 4 ) {
    std::string s = bwd ? "B" : "F";
    s += std::to_string( station );
    Warning( "Invalid channel map for station " + s ).ignore();
    m_channels[station + offset].assign( 4, 0 );
    return false;
  }

  // Determine whether dealing with F-side or B-side crate
  const unsigned int crate = bwd ? m_crateB : m_crateF;
  // Loop over the four quadrants and assign channel numbers
  m_channels[station + offset].resize( 4 );
  for ( unsigned int i = 0; i < 4; ++i ) {
    if ( channels[i] < 0 ) {
      std::string s = bwd ? "B" : "F";
      s += std::to_string( station );
      warning() << "Invalid channel number " << channels[i] << " for quadrant " << s << i << endmsg;
      continue;
    }
    m_channels[station + offset][i] = ( crate << 6 ) | channels[i];
  }
  return true;
}
