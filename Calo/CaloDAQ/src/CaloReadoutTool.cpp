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
// local
#include "CaloReadoutTool.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloReadoutTool
//
// base class for Calo readout tools
// (CaloEnergyFromRaw, CaloTriggerAdcsFromRaw and CaloTriggerBitsFromRaw)
//
// 2007-02-01 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( CaloReadoutTool )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloReadoutTool::CaloReadoutTool( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ), m_detectorName{this, "DetectorName", details::alg_name_to_detector( name )} {
  // new for decoders, initialize search path, and then call the base method
  m_rawEventLocations = {LHCb::RawEventLocation::Calo, LHCb::RawEventLocation::Default};
  initRawEventSearch();
}

//=========================================================================
//  Get required CaloBanks (short or packed format) - Fill m_banks
//=========================================================================
StatusCode CaloReadoutTool::initialize() {

  StatusCode sc = base_class::initialize();
  if ( sc.isFailure() ) return sc; // error printed already by GaudiAlgorithm
  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Initialize " << name() << endmsg;
  IIncidentSvc* inc = incSvc();
  if ( inc ) inc->addListener( this, IncidentType::BeginEvent );
  return sc;
}

StatusCode CaloReadoutTool::finalize() {
  IIncidentSvc* inc = incSvc();
  if ( inc ) { inc->removeListener( this ); }
  return base_class::finalize();
}

bool CaloReadoutTool::getCaloBanksFromRaw() {

  m_readSources.clear();
  m_banks = {};

  // Retrieve the RawEvent:
  LHCb::RawEvent* rawEvt = findFirstRawEvent();

  if ( !rawEvt ) {
    if ( m_first && msgLevel( MSG::DEBUG ) )
      debug() << "WARNING : RawEvent not found at locations: " << m_rawEventLocations << " (message will be suppressed)"
              << endmsg;
    m_first = false;
    return false;
  }

  m_packed = false;
  if ( !m_packedIsDefault ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Banks of short type are requested as default" << endmsg;
    m_banks  = rawEvt->banks( m_shortType );
    m_status = LHCb::RawBankReadoutStatus( m_shortType );
  } else {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Banks of paked type are requested as default" << endmsg;
    m_banks  = rawEvt->banks( m_packedType );
    m_status = LHCb::RawBankReadoutStatus( m_packedType );
  }

  if ( m_banks.empty() ) {
    if ( !m_packedIsDefault ) {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " Requested banks of short type has not been found ... try packed type" << endmsg;
      m_banks  = rawEvt->banks( m_packedType );
      m_status = LHCb::RawBankReadoutStatus( m_packedType );
    } else {
      if ( msgLevel( MSG::DEBUG ) )
        debug() << " Requested banks of packed type has not been found ... try short type" << endmsg;
      m_banks  = rawEvt->banks( m_shortType );
      m_status = LHCb::RawBankReadoutStatus( m_shortType );
    }

    if ( m_banks.empty() ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << "WARNING : None of short and packed banks have been found " << endmsg;
      counter( "No " + toString( m_detectorName ) + " bank found" ) += 1;
      return false;
    } else {
      if ( !m_packedIsDefault ) {
        if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of packed type has been found" << endmsg;
        m_packed = true;
      } else {
        if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of short type has found" << endmsg;
      }
    }
  } else {
    if ( !m_packedIsDefault ) {
      if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of short type has been found" << endmsg;
    } else {
      if ( msgLevel( MSG::DEBUG ) ) debug() << " Requested banks of packed type has found" << endmsg;
      m_packed = true;
    }
  }

  // check whether the associated Error Bank is present or not
  for ( const auto& b : rawEvt->banks( m_errorType ) )
    m_status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::ErrorBank );

  // check banks integrity + Magic pattern
  std::vector<int> sources;
  sources.reserve( m_banks.size() );
  for ( const auto& b : m_banks ) {
    if ( !b ) continue;
    sources.push_back( b->sourceID() );
    if ( LHCb::RawBank::MagicPattern != b->magic() ) {
      Error( "Bad MagicPattern for sourceID " + Gaudi::Utils::toString( b->sourceID() ), StatusCode::SUCCESS ).ignore();
      m_status.addStatus( b->sourceID(), LHCb::RawBankReadoutStatus::Status::BadMagicPattern );
    }
  }

  if ( m_packed ) { // TELL1 format : 1 source per TELL1
    for ( const auto& t : m_calo->tell1Params() ) {
      bool ok = std::any_of( sources.begin(), sources.end(), [&]( int n ) { return t.number() == n; } );
      m_status.addStatus( t.number(),
                          ok ? LHCb::RawBankReadoutStatus::Status::OK : LHCb::RawBankReadoutStatus::Status::Missing );
    }
  } else { // Offline format : single source 0
    m_status.addStatus( 0, sources.empty() ? LHCb::RawBankReadoutStatus::Status::Missing
                                           : LHCb::RawBankReadoutStatus::Status::OK );
  }

  return true;
}

//========================
//  Check FE-Cards is PIN
//========================
bool CaloReadoutTool::checkCards( int nCards, const std::vector<int>& feCards ) const {
  bool check = true;
  if ( msgLevel( MSG::DEBUG ) )
    debug() << nCards - feCards.size() << "FE-Cards have been read among the " << nCards << " expected" << endmsg;
  for ( auto i : feCards ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << " Unread FE-Cards : " << m_calo->cardCode( i ) << "  - Is it a PinDiode readout FE-Card ? "
              << m_calo->isPinCard( i ) << endmsg;
    if ( m_calo->isPmtCard( i ) ) {
      Warning( " The standard (PMT) FE-Card " + Gaudi::Utils::toString( m_calo->cardCode( i ) ) +
               " expected in TELL1 bank has not been read !!" )
          .ignore();
      check = false;
    }
  }
  return check;
}

//===========================
//  Find Card number by code
//===========================
int CaloReadoutTool::findCardbyCode( const std::vector<int>& feCards, int code ) const {
  auto i = std::find_if( feCards.begin(), feCards.end(), [&]( int ife ) { return code == m_calo->cardCode( ife ); } );
  if ( i == feCards.end() ) {
    Error( "FE-Card [code : " + Gaudi::Utils::toString( code ) + "] does not match the condDB cabling scheme  ",
           StatusCode::SUCCESS )
        .ignore();
    return -1;
  }
  if ( msgLevel( MSG::DEBUG ) )
    debug() << " FE-Card [code : " << code << " | crate : " << m_calo->cardParam( *i ).crate()
            << " slot : " << m_calo->cardParam( *i ).slot() << "] has been found with (num : " << *i << ")  in condDB"
            << endmsg;
  return std::distance( feCards.begin(), i );
}

void CaloReadoutTool::putStatusOnTES() {
  // Readout Status
  typedef LHCb::RawBankReadoutStatus  Status;
  typedef LHCb::RawBankReadoutStatuss Statuss;
  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Creating container at " << LHCb::RawBankReadoutStatusLocation::Default << endmsg;
  Statuss* statuss = getOrCreate<Statuss, Statuss>( LHCb::RawBankReadoutStatusLocation::Default );
  Status*  nstatus = statuss->object( m_status.key() );
  if ( !nstatus ) {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << "Inserting new status for bankType " << Gaudi::Utils::toString( m_status.key() ) << endmsg;
    nstatus = new Status( m_status );
    statuss->insert( nstatus );
  } else {
    if ( msgLevel( MSG::DEBUG ) )
      debug() << "Status for bankType " << Gaudi::Utils::toString( m_status.key() ) << " already exists" << endmsg;
    if ( nstatus->status() != m_status.status() ) {
      Warning( "Status for bankType " + LHCb::RawBank::typeName( m_status.key() ) +
                   " already exists  with different status value -> merge both",
               StatusCode::SUCCESS )
          .ignore();
      for ( const auto& it : m_status.statusMap() ) { nstatus->addStatus( it.first, it.second ); }
    }
  }
}

void CaloReadoutTool::checkCtrl( int ctrl, int sourceID ) {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Control word :" << ctrl << endmsg;

  if ( 0 != ( 0x1 & ctrl ) || 0 != ( 0x20 & ctrl ) || 0 != ( 0x40 & ctrl ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Tell1 error bits have been detected in data" << endmsg;
    if ( 0 != ( 0x1 & ctrl ) ) m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Error );
    if ( 0 != ( 0x20 & ctrl ) ) m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Sync );
    if ( 0 != ( 0x40 & ctrl ) ) m_status.addStatus( sourceID, LHCb::RawBankReadoutStatus::Status::Tell1Link );
  }
}

bool CaloReadoutTool::checkSrc( int source ) {
  auto it   = std::find( m_readSources.begin(), m_readSources.end(), source );
  bool read = ( it != m_readSources.end() );
  if ( read ) {
    Warning( "Another bank with same sourceID " + Gaudi::Utils::toString( source ) + " has already been read" )
        .ignore();
    m_status.addStatus( source, LHCb::RawBankReadoutStatus::Status::NonUnique );
  } else {
    m_readSources.push_back( source );
  }
  return read;
}
