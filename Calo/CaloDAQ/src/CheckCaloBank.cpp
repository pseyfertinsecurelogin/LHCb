/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// local
#include "CheckCaloBank.h"

// ============================================================================
/** @file CheckCaloBank.cpp
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */
// ============================================================================

// MANDATORY!!!
DECLARE_COMPONENT( CheckCaloBank )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CheckCaloBank::CheckCaloBank( const std::string& name, ISvcLocator* pSvcLocator)
  : GaudiAlgorithm       ( name , pSvcLocator            )
  , m_trig(LHCb::RawBank::LastType)
  , m_bank(LHCb::RawBank::LastType)
  , m_typ("Invalid")
{
  if( "Prs" == name.substr( 0 , 3 ) ){
    m_trig = LHCb::RawBank::PrsTrig;
    m_bank = LHCb::RawBank::PrsE;
    m_typ  = "Prs";
  }else if( "Ecal" == name.substr( 0 , 4 ) ){
    m_trig = LHCb::RawBank::EcalTrig;
    m_bank = LHCb::RawBank::EcalE;
    m_typ  = "Ecal";
  }else if( "Hcal" == name.substr( 0 , 4 ) ){
    m_trig = LHCb::RawBank::HcalTrig;
    m_bank = LHCb::RawBank::HcalE;
    m_typ  = "Hcal";
  }

  declareProperty( "RawEventLocations", m_rawEventLocations,
                   "List of possible locations of the RawEvent object in the"
                   " transient store. By default it is LHCb::RawEventLocation::Calo,"
                   " LHCb::RawEventLocation::Default.");

}

//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode CheckCaloBank::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) return sc;

  if( "Prs" != m_typ && "Ecal" != m_typ && "Hcal" != m_typ ){
    error() << "Undefined Calo type : " << m_typ << endmsg;
    return StatusCode::FAILURE;
  }else{
    info() << " Check the 0-suppressed banks exist for " << m_typ << endmsg;
    info() << " if not (MC) re-create them from packed bank and delete packed " << endmsg;
  }

  // Initialise the RawEvent locations
  bool usingDefaultLocation = (m_rawEventLocations.empty() ||  LHCb::RawEventLocation::Default==m_rawEventLocations[0] || LHCb::RawEventLocation::Calo==m_rawEventLocations[0]);
  if (std::find(m_rawEventLocations.begin(), m_rawEventLocations.end(), LHCb::RawEventLocation::Default)
      == m_rawEventLocations.end()) {
    // append the defaults to the search path
    m_rawEventLocations.push_back(LHCb::RawEventLocation::Calo);
    m_rawEventLocations.push_back(LHCb::RawEventLocation::Default);
  }

  if (!usingDefaultLocation) {
    info() << "Using '" << m_rawEventLocations << "' as search path for the RawEvent object" << endmsg;
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CheckCaloBank::execute() {

  setFilterPassed(false);

  // Retrieve the RawEvent:
  LHCb::RawEvent* rawEvt = nullptr ;
  for (auto p = m_rawEventLocations.begin(); p != m_rawEventLocations.end() && ! rawEvt; ++p) {
    rawEvt = getIfExists<LHCb::RawEvent>(*p);
  }

  if( rawEvt == NULL ) {
    return StatusCode::SUCCESS;
  }

  bool trig = ( 0 == (rawEvt->banks( m_trig )).size() ) ?  false : true ;
  bool bank = ( 0 == (rawEvt->banks( m_bank )).size() ) ?  false : true ;

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    debug() << "TYPE of Calo Bank to be checked : " << m_typ << " " << trig << " " << bank <<endmsg;
    if(trig ||  bank ){
      debug() << " Calo Banks with 'compressed' format exist - Do nothing" << endmsg;
    }else{
      debug() << " Calo Banks with 'compressed' format DOESN'T exist - Will be created from PACKED banks" << endmsg;
    }
  }

  if(trig && bank)setFilterPassed(false);// no need to rebuilt 'compressed' bank
  else if(!trig && !bank)setFilterPassed(true); // re-Built compressed banks and (delete packed bank)
  else {
    warning() << " Compressed bank incomplete !! Do nothing" << endmsg;
    setFilterPassed(false);
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
