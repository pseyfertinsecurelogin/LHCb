// Include files

// local
#include "CaloMergeTAE.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloMergeTAE
//
// 2008-10-24 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( CaloMergeTAE )

namespace Calo { namespace DAQ { namespace TAE{

namespace details {
    const char* toString(const source_t& source) {
        switch (source) {
            case source_t::from_adc   : return "ADC";
            case source_t::from_digit : return "DIGIT";
            default: throw "IMPOSSIBLE VALUE"; return nullptr;
        }
    }

    StatusCode parse(source_t& result, const std::string& input ) {
        std::string out( input );
        std::transform( input.begin() ,input.end() , out.begin () , ::toupper ) ;

        if( out == "ADC" || out == "CALOADC" || out == "ADCS" || out == "CALOADCS") {
            result = source_t::from_adc;
            return StatusCode::SUCCESS;
        }
        if( out == "DIGIT" || out == "CALODIGIT" || out == "DIGITS" || out == "CALODIGITS")  {
            result = source_t::from_digit;
            return StatusCode::SUCCESS;
        }
        return StatusCode::FAILURE;
    }


}

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
Merger::Merger( const std::string& name,
                      ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  // set default detectorName
  auto index = name.find_last_of(".");
  index = ( index == std::string::npos ? 0 : index+1 );
  m_detectorName = ( name.compare( index, 3, "Prs" ) == 0 ? "Prs"
                   : name.compare( index, 3, "Spd" ) == 0 ? "Spd"
                   : name.substr( index, 4 ) );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode Merger::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get detectorElement
  if ( "Ecal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_locDigit     = LHCb::CaloDigitLocation::Ecal + m_inExt.value();
    m_locAdc     = LHCb::CaloAdcLocation::Ecal + m_inExt.value();
  } else if ( "Hcal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_locDigit     = LHCb::CaloDigitLocation::Hcal + m_inExt.value();
    m_locAdc     = LHCb::CaloAdcLocation::Hcal + m_inExt.value();
  } else if ( "Prs" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_locDigit     = LHCb::CaloDigitLocation::Prs + m_inExt.value();
    m_locAdc     = LHCb::CaloAdcLocation::Prs + m_inExt.value();
  } else if ( "Spd" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Spd );
    m_locDigit     = LHCb::CaloDigitLocation::Spd + m_inExt.value();
    m_locAdc     = LHCb::CaloAdcLocation::Spd + m_inExt.value();
  } else {
    error() << "Unknown detector name " << m_detectorName << endmsg;
    return StatusCode::FAILURE;
  }
  //
  std::tie( m_outDigit, m_outAdc ) = ( m_outputDataLoc.value().empty()
                                     ? std::make_pair( "TAE/"+m_locDigit, "TAE/"+m_locAdc)
                                     : std::make_pair( m_outputDataLoc.value(), m_outputDataLoc.value() ) );

  //
  if(m_slots.value().empty()){
    Error("No input slots defined").ignore();
    return StatusCode::FAILURE;
  }
  //
  //
  auto loc = [&]() -> decltype(auto) {
    switch ( m_data.value() ) {
      case details::source_t::from_adc   : return m_outAdc;
      case details::source_t::from_digit : return m_outDigit;
      default : throw "IMPOSSIBLE";
  } };
  info() << "Will merge " << toString(m_data.value()) << " from " << m_slots.value() << " into " << loc() << endmsg;
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode Merger::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  switch ( m_data.value() ) {
      case details::source_t::from_adc   : mergeAdcs();   break;
      case details::source_t::from_digit : mergeDigits(); break;
  };

  return StatusCode::SUCCESS;
}

//=============================================================================
void Merger::mergeDigits(){

  std::map<std::string,LHCb::CaloDigits*> digitsMap;
  for(const auto& islot : m_slots.value() ) {
    std::string slot = islot + "/";
    if(islot == "T0") slot ="";
    if( exist<LHCb::CaloDigits>( slot + m_locDigit ))
      digitsMap[islot] =  get<LHCb::CaloDigits>( slot + m_locDigit , false );
    else
      Warning("No CaloDigits found in " + slot + m_locDigit ).ignore();
  }
  //
  LHCb::CaloDigits* newDigits = new LHCb::CaloDigits();
  put( newDigits, m_outDigit );
  //
  for( const auto& icell : m_calo->cellParams() ) {
    LHCb::CaloCellID id = icell.cellID();

    std::string txt  = "Digit :" ;
    double sum = 0;
    for( const auto& map : digitsMap ) {
      const LHCb::CaloDigits* digs = map.second;
      if( !digs) continue;
      const LHCb::CaloDigit* dig = digs->object( id );
      if( !dig ) continue;
      double e = dig->e();
      if( e < m_slotcut.value() )continue;
      sum += e;
      if ( msgLevel( MSG::DEBUG) )txt += map.first + " : " + Gaudi::Utils::toString(e) + " | " ;
    }

    if(  sum > m_threshold.value() ){
      newDigits->insert( new LHCb::CaloDigit(id, sum) );
      if ( msgLevel( MSG::DEBUG) )debug() << id << " : "  << txt << " =>  TAE = " << sum <<endmsg;
    }
  }
}


void Merger::mergeAdcs(){
  std::map<std::string,LHCb::CaloAdcs*> adcsMap;
  for(const auto& islot : m_slots.value() ) {
    std::string slot = islot + "/";
    if(islot == "T0") slot ="";
    if( exist<LHCb::CaloAdcs>( slot + m_locAdc ))
      adcsMap[islot] =  get<LHCb::CaloAdcs>( slot + m_locAdc , false );
    else
      Warning("No CaloAdcs found in " + slot + m_locAdc ).ignore();
  }
  //
  LHCb::CaloAdcs* newAdcs = new LHCb::CaloAdcs();
  put( newAdcs, m_outAdc );
  //
  const CaloVector<CellParam>& cells = m_calo->cellParams();
  for( const auto& icell : cells ) {
    LHCb::CaloCellID id = icell.cellID();

    std::string txt =  "ADC :" ;
    int sum = 0;
    for( const auto& imap : adcsMap ) {
      const LHCb::CaloAdcs* adcs = imap.second;
      if( !adcs) continue;
      const LHCb::CaloAdc* adc = adcs->object( id );
      if( !adc ) continue;
      int val = adc->adc();
      if( val < m_slotcut.value()) continue;
      sum += val;
      if ( msgLevel( MSG::DEBUG) ) txt += imap.first + " : " + Gaudi::Utils::toString( val ) +  " | " ;
    }

    if(  (double) sum > m_threshold.value() ){
      LHCb::CaloAdc* newAdc = new LHCb::CaloAdc(id, sum);
      newAdcs->insert( newAdc );
      if ( msgLevel( MSG::DEBUG) )debug() << id << " : "  << txt << " =>  TAE = " << sum <<endmsg;
    }
  }
}
}}}
