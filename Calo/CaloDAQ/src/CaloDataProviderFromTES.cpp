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
#include "CaloDataProviderFromTES.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloDataProviderFromTES
//
// 2008-08-22 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Tool Factory
DECLARE_COMPONENT( CaloDataProviderFromTES )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloDataProviderFromTES::CaloDataProviderFromTES( const std::string& type,
                                  const std::string& name,
                                  const IInterface* parent )
  : base_class ( type, name , parent )
  , m_detectorName( details::alg_name_to_detector( name ) )
{
  //
  switch (m_detectorName) {
  case details::DetectorName_t::Ecal :
    m_digLoc = LHCb::CaloDigitLocation::Ecal;
    m_adcLoc = LHCb::CaloAdcLocation::Ecal;
    break;
  case details::DetectorName_t::Hcal :
    m_digLoc = LHCb::CaloDigitLocation::Hcal;
    m_adcLoc = LHCb::CaloAdcLocation::Hcal;
    break;
  case details::DetectorName_t::Prs :
    m_digLoc = LHCb::CaloDigitLocation::Prs;
    m_adcLoc = LHCb::CaloAdcLocation::Prs;
    break;
  case details::DetectorName_t::Spd :
    m_digLoc = LHCb::CaloDigitLocation::Spd;
    m_adcLoc = LHCb::CaloAdcLocation::Spd;
    break;
  case details::DetectorName_t::Unknown :
    ; // allow for genconf to work ;-(
  }

}

//=============================================================================
StatusCode CaloDataProviderFromTES::finalize ( ) {
  IIncidentSvc* inc = incSvc() ;
  if ( inc ) { inc->removeListener( this ); }
  return base_class::finalize();
}

StatusCode CaloDataProviderFromTES::initialize ( ) {
  StatusCode sc = base_class::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;

  // fill incident listener
  IIncidentSvc* inc = incSvc() ;
  if ( inc ) inc -> addListener  ( this , IncidentType::BeginEvent ) ;

  switch (m_detectorName) {
  case details::DetectorName_t::Ecal :
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    break;
  case details::DetectorName_t::Hcal :
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    break;
  case details::DetectorName_t::Prs :
  case details::DetectorName_t::Spd :  // Prs FE for SPD
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    break;
  default :
    throw std::invalid_argument("unknown detector: " + name() );
  }

  //
  if( m_loc.value().empty() ){
    switch (m_data.value()) {
      case details::source_t::from_adc   : m_loc = m_adcLoc; break;
      case details::source_t::from_digit : m_loc = m_digLoc; break;
    }
  }

  long nCells = m_calo->numberOfCells();
  long nPins  = m_calo->numberOfPins();
  m_adcs.reserve( nCells + nPins  );
  m_digits.reserve( nCells );
  clear();

  //

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << " Initialisation OK" << endmsg;
  return StatusCode::SUCCESS;
}

//-------------------------------------
bool CaloDataProviderFromTES::getBanks( ) {
  clear();
  m_getRaw = false;
  counter("Call for ADC containers") += 1;
  if( fromDigit() ){
    m_digCont = getIfExists<LHCb::CaloDigits>(evtSvc(),m_loc.value());
    if( m_digCont ){
      m_ok = true;
      if ( msgLevel( MSG::DEBUG) )debug() << "Found container " << m_loc.value() << " Content size " << m_digCont->size() << endmsg;
    }else m_ok = false;
  }else if( fromAdc() ){
    m_adcCont = getIfExists<LHCb::CaloAdcs>(evtSvc(),m_loc.value());
    if( m_adcCont ){
      m_ok = true;
      if ( msgLevel( MSG::DEBUG) )debug() << "Found container " << m_loc.value() << " Content size " << m_adcCont->size() << endmsg;
    }else m_ok = false;
  }
  if(!m_ok)counter("No container found") += 1;
  return m_ok;
}

//-------------------------------------
void CaloDataProviderFromTES::clear( ) {
  m_adcs.clear();
  m_digits.clear();
  m_readSources.clear();
  m_minADC = LHCb::CaloAdc(LHCb::CaloCellID() , 3840);
  m_minPinADC = LHCb::CaloAdc(LHCb::CaloCellID(), 3840);
  m_maxADC = LHCb::CaloAdc(LHCb::CaloCellID() , -256);
  m_maxPinADC = LHCb::CaloAdc(LHCb::CaloCellID(), -256);
}
void CaloDataProviderFromTES::cleanData(int feb ) {
  if(feb<0)return;
  CaloVector<LHCb::CaloAdc> temp;
  CaloVector<LHCb::CaloDigit> tempD;
  for(const auto& iadc : m_adcs) {
    if( m_calo->cellParam( iadc.cellID() ).cardNumber() == feb)continue;
    temp.addEntry( iadc, iadc.cellID() );
  }
  for(const auto& idig : m_digits) {
    if( m_calo->cellParam( idig.cellID() ).cardNumber() == feb)continue;
    tempD.addEntry( idig, idig.cellID() );
  }
  m_adcs.clear();
  m_digits.clear();
  for(const auto& iadc : temp ) m_adcs.addEntry(iadc, iadc.cellID() );
  for(const auto& idig : tempD) m_digits.addEntry(idig, idig.cellID() );
}
//---------
double CaloDataProviderFromTES::digit (LHCb::CaloCellID id, double def){
  double pedShift = m_calo->pedestalShift();
  if(m_getRaw)getBanks();
  if( fromDigit() ){
    if( !m_digCont )return def ;
    LHCb::CaloDigit* digit = m_digCont->object( id );
    if( !digit )return def;
    return digit->e();
  }
  if( fromAdc() ){
    if( !m_adcCont )return def ;
    LHCb::CaloAdc* adc = m_adcCont->object( id );
    if( !adc )return def;
    return ( (double) adc->adc() - pedShift ) * m_calo->cellGain( id );
  }
  return def;
}
//---------
int CaloDataProviderFromTES::adc (LHCb::CaloCellID id,int def){
  double pedShift = m_calo->pedestalShift();
  if(m_getRaw)getBanks();
  if( fromDigit() ){
    if( !m_digCont )return def ;
    LHCb::CaloDigit* digit = m_digCont->object( id );
    if( !digit )return def;
    return (int) (digit->e() / m_calo->cellGain( id ) + pedShift);
  }
  if(  fromAdc() ){
    if( !m_adcCont )return def ;
    LHCb::CaloAdc* adc = m_adcCont->object( id );
    if( !adc )return def;
    return adc->adc();
  }
  return def;
}
//---------


const CaloVector<LHCb::CaloAdc>& CaloDataProviderFromTES::adcs(int source,bool clean){
  return adcs(std::vector<int>(1,source),clean);
}
const CaloVector<LHCb::CaloDigit>& CaloDataProviderFromTES::digits(int source,bool clean){
  return digits(std::vector<int>(1,source),clean);
}


const CaloVector<LHCb::CaloAdc>& CaloDataProviderFromTES::adcs(std::vector<int> sources,bool clean){
  if(m_getRaw)getBanks();
  if(clean)clear();
  for(const auto& source : sources ) {
    if(checkSrc( source ))continue;
    if( fromDigit() ){
      if( !m_digCont )return m_adcs ;
      for(const auto& idig : *m_digCont) {
        LHCb::CaloCellID id = idig->cellID();
        if( source != -1 && source != m_calo->cardToTell1 ( m_calo->cardNumber( id ) )) continue;
        LHCb::CaloAdc cAdc(id ,adc(id) );
        m_adcs.addEntry( cAdc , id);
        if( id.area() != CaloCellCode::PinArea ){
          if( cAdc.adc() < m_minADC.adc()  )m_minADC = cAdc ;
          if( cAdc.adc() > m_maxADC.adc()  )m_maxADC = cAdc ;
        }else{
          if( cAdc.adc() < m_minPinADC.adc() )m_minPinADC = cAdc ;
          if( cAdc.adc() > m_maxPinADC.adc() )m_maxPinADC = cAdc ;
        }
      }
    }else if( fromAdc() ){
      if( !m_adcCont )return m_adcs ;
      for(const auto& iadc : *m_adcCont){
        LHCb::CaloCellID id = iadc->cellID();
        if( source != -1 && source != m_calo->cardToTell1 ( m_calo->cardNumber( id ) )) continue;
        LHCb::CaloAdc adc = *iadc;
        m_adcs.addEntry( adc , id);
        if( id.area() != CaloCellCode::PinArea ){
          if( adc.adc() < m_minADC.adc()  )m_minADC = adc ;
          if( adc.adc() > m_maxADC.adc()  )m_maxADC = adc ;
        }else{
          if( adc.adc() < m_minPinADC.adc() )m_minPinADC = adc ;
          if( adc.adc() > m_maxPinADC.adc() )m_maxPinADC = adc ;
        }
      }
    }
  }
  return m_adcs;
}
//---------
const CaloVector<LHCb::CaloDigit>& CaloDataProviderFromTES::digits(std::vector<int> sources,bool clean){
  if(m_getRaw)getBanks();
  if(clean)clear();
  for(const auto& source : sources) {
    if(checkSrc( source ))continue;
    if( fromDigit() ){
      if( !m_digCont )return m_digits ;
      for(const auto& idig : *m_digCont) {
        LHCb::CaloCellID id = idig->cellID();
        if( source != -1 && source != m_calo->cardToTell1 ( m_calo->cardNumber( id ) )) continue;
        m_digits.addEntry( *idig , id);
      }
    }else if( fromAdc() ){
      if( !m_adcCont )return m_digits ;
      for(const auto& iadc : *m_adcCont) {
        LHCb::CaloCellID id = iadc->cellID();
        if( source != -1 && source != m_calo->cardToTell1 ( m_calo->cardNumber( id ) )) continue;
        m_digits.addEntry( LHCb::CaloDigit( id , digit(id) ) , id);
      }
    }
  }
  return m_digits;
}

bool CaloDataProviderFromTES::checkSrc(int source){
  return std::any_of( m_readSources.begin(), m_readSources.end(),
                      [&](int i) { return source == i; } );
}
