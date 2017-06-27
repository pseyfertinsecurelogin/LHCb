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


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloMergeTAE::CaloMergeTAE( const std::string& name,
                            ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator ), m_calo(0)
{
  declareProperty("MergeSlots"     , m_slots = { "T0", "Prev1", "Next1" } );
  declareProperty("Detector"       , m_detectorName);
  declareProperty("SumThreshold"   , m_threshold = -256);
  declareProperty("SlotThreshold"  , m_slotcut   = -256);
  declareProperty("OutputType"     , m_data = "Digits" );
  declareProperty("OutputDataLocation", m_outputDataLoc=""      ) ;
  declareProperty("inputExtension"     , m_inExt = "" );

  // set default detectorName
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  m_detectorName = ( name.substr(index,3) == "Prs" ? "Prs"
                   : name.substr(index,3) == "Spd" ? "Spd"
                   : name.substr( index, 4 ) );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloMergeTAE::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  // get detectorElement
  if ( "Ecal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
    m_locDigit     = LHCb::CaloDigitLocation::Ecal + m_inExt;
    m_locAdc     = LHCb::CaloAdcLocation::Ecal + m_inExt;
  } else if ( "Hcal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_locDigit     = LHCb::CaloDigitLocation::Hcal + m_inExt;
    m_locAdc     = LHCb::CaloAdcLocation::Hcal + m_inExt;
  } else if ( "Prs" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_locDigit     = LHCb::CaloDigitLocation::Prs + m_inExt;
    m_locAdc     = LHCb::CaloAdcLocation::Prs + m_inExt;
  } else if ( "Spd" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Spd );
    m_locDigit     = LHCb::CaloDigitLocation::Spd + m_inExt;
    m_locAdc     = LHCb::CaloAdcLocation::Spd + m_inExt;
  } else {
    error() << "Unknown detector name " << m_detectorName << endmsg;
    return StatusCode::FAILURE;
  }
  //
  if( "" != m_outputDataLoc ) {
    m_outDigit = m_outputDataLoc;
    m_outAdc   = m_outputDataLoc;
  }else{
    m_outDigit     = "TAE/" + m_locDigit;
    m_outAdc       = "TAE/" + m_locAdc;
  }


  //
  if(m_slots.size() == 0){
    Error("No input slots defined").ignore();
    return StatusCode::FAILURE;
  }
  //
  //
  std::string loc ;
  if( fromDigit() ){
    loc = m_outDigit;
  }else if(fromAdc() ){
    loc= m_outAdc ;
  } else {
    Error("Unknown data type " + m_data).ignore();
    return StatusCode::FAILURE;
  }
  info() << "Will merge " << m_data << " from " << m_slots << " into " << loc << endmsg;
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloMergeTAE::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;


  if( fromDigit()    )mergeDigits();
  else if( fromAdc() ) mergeAdcs();
  else{
    Warning("Unable to merged data type " + m_data).ignore();
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
void CaloMergeTAE::mergeDigits(){

  std::map<std::string,LHCb::CaloDigits*> digitsMap;
  for(const auto& islot : m_slots ) {
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
      if( e < m_slotcut )continue;
      sum += e;
      if ( msgLevel( MSG::DEBUG) )txt += map.first + " : " + Gaudi::Utils::toString(e) + " | " ;
    }

    if(  sum > m_threshold ){
      LHCb::CaloDigit* newDigit = new LHCb::CaloDigit(id, sum);
      newDigits->insert( newDigit);
      if ( msgLevel( MSG::DEBUG) )debug() << id << " : "  << txt << " =>  TAE = " << sum <<endmsg;
    }
  }
}


void CaloMergeTAE::mergeAdcs(){
  std::map<std::string,LHCb::CaloAdcs*> adcsMap;
  for(const auto& islot : m_slots ) {
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
      if( val < m_slotcut)continue;
      sum += val;
      if ( msgLevel( MSG::DEBUG) )txt += imap.first + " : " + Gaudi::Utils::toString( val ) +  " | " ;
    }

    if(  (double) sum > m_threshold ){
      LHCb::CaloAdc* newAdc = new LHCb::CaloAdc(id, sum);
      newAdcs->insert( newAdc );
      if ( msgLevel( MSG::DEBUG) )debug() << id << " : "  << txt << " =>  TAE = " << sum <<endmsg;
    }
  }
}

