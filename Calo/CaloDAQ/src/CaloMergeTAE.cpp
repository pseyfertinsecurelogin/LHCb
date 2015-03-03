// $Id: CaloMergeTAE.cpp,v 1.3 2009-04-06 15:45:03 odescham Exp $
// Include files 

// from Gaudi
#include "GaudiKernel/AlgFactory.h" 

// local
#include "CaloMergeTAE.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloMergeTAE
//
// 2008-10-24 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( CaloMergeTAE );


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloMergeTAE::CaloMergeTAE( const std::string& name,
                            ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{

  declareProperty("MergeSlots"     , m_slots);
  declareProperty("Detector"       , m_detectorName);
  declareProperty("Threshold"      , m_threshold = 0.);
  declareProperty("OutputType"     , m_data = "Digits" );
  //
  m_slots.push_back("T0");
  m_slots.push_back("Prev1");
  m_slots.push_back("Next1");

  // set default detectorName
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  m_detectorName = name.substr( index, 4 ); 
  if ( name.substr(index,3) == "Prs" ) m_detectorName = "Prs";
  if ( name.substr(index,3) == "Spd" ) m_detectorName = "Spd";


  

}
//=============================================================================
// Destructor
//=============================================================================
CaloMergeTAE::~CaloMergeTAE() {} 

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
    m_locDigit     = LHCb::CaloDigitLocation::Ecal;
    m_locAdc     = LHCb::CaloAdcLocation::Ecal;
  } else if ( "Hcal" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
    m_locDigit     = LHCb::CaloDigitLocation::Hcal;
    m_locAdc     = LHCb::CaloAdcLocation::Hcal;
  } else if ( "Prs" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
    m_locDigit     = LHCb::CaloDigitLocation::Prs;
    m_locAdc     = LHCb::CaloAdcLocation::Prs;
  } else if ( "Spd" == m_detectorName ) {
    m_calo     = getDet<DeCalorimeter>( DeCalorimeterLocation::Spd ); 
    m_locDigit     = LHCb::CaloDigitLocation::Spd;
    m_locAdc     = LHCb::CaloAdcLocation::Spd;
  } else {
    error() << "Unknown detector name " << m_detectorName << endreq;
    return StatusCode::FAILURE;
  }
  //
  m_outDigit     = "TAE/" + m_locDigit;
  m_outAdc       = "TAE/" + m_locAdc;
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
  }  else {
    Error("Unknown data type " + m_data).ignore();
    return StatusCode::FAILURE;
  }  
  info() << "Will merge " << m_data << " from " << m_slots << " into " << loc << endreq;
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
//  Finalize
//=============================================================================
StatusCode CaloMergeTAE::finalize() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Finalize" << endmsg;

  return GaudiAlgorithm::finalize();  // must be called after all other actions
}

//=============================================================================
void CaloMergeTAE::mergeDigits(){

  std::map<std::string,LHCb::CaloDigits*> digitsMap;
  for(std::vector<std::string>::iterator islot = m_slots.begin(); islot != m_slots.end() ; islot++){
    std::string slot = *islot + "/";
    if(*islot == "T0") slot ="";
    if( exist<LHCb::CaloDigits>( slot + m_locDigit ))
      digitsMap[*islot] =  get<LHCb::CaloDigits>( slot + m_locDigit , false );
    else
      Warning("No CaloDigits found in " + slot + m_locDigit ).ignore();      

  }
  //
  LHCb::CaloDigits* newDigits = new LHCb::CaloDigits();
  put( newDigits, m_outDigit );
  //
  const CaloVector<CellParam>& cells = m_calo->cellParams();
  for( CaloVector<CellParam>::const_iterator icell = cells.begin() ; icell != cells.end() ; icell++){
    LHCb::CaloCellID id = (*icell).cellID();

    std::stringstream txt("");
    txt  << "Digit :" ;
    double sum = 0;
    for(  std::map<std::string,LHCb::CaloDigits*>::iterator imap = digitsMap.begin();imap!=digitsMap.end();imap++){
      LHCb::CaloDigits* digs = (*imap).second;
      if( NULL == digs) continue;
      LHCb::CaloDigit* dig = digs->object( id );
      if( NULL == dig ) continue;
      double e = dig->e();
      sum += e;
      txt << (*imap).first << " : " << e << " | " ;
    }
    
    if(  sum > m_threshold ){
      LHCb::CaloDigit* newDigit = new LHCb::CaloDigit(id, sum);
      newDigits->insert( newDigit);
      debug() << id << " : "  << txt.str() << " =>  TAE = " << sum <<endreq;
    }

  }

}


void CaloMergeTAE::mergeAdcs(){
  std::map<std::string,LHCb::CaloAdcs*> adcsMap;
  for(std::vector<std::string>::iterator islot = m_slots.begin(); islot != m_slots.end() ; islot++){
    std::string slot = *islot + "/";
    if(*islot == "T0") slot ="";
    if( exist<LHCb::CaloAdcs>( slot + m_locAdc ))
      adcsMap[*islot] =  get<LHCb::CaloAdcs>( slot + m_locAdc , false );
    else
      Warning("No CaloAdcs found in " + slot + m_locAdc ).ignore();      

  }
  //
  LHCb::CaloAdcs* newAdcs = new LHCb::CaloAdcs();
  put( newAdcs, m_outAdc );
  //
  const CaloVector<CellParam>& cells = m_calo->cellParams();
  for( CaloVector<CellParam>::const_iterator icell = cells.begin() ; icell != cells.end() ; icell++){
    LHCb::CaloCellID id = (*icell).cellID();

    std::stringstream txt("");
    txt  << "ADC :" ;
    int sum = 0;
    for(  std::map<std::string,LHCb::CaloAdcs*>::iterator imap = adcsMap.begin();imap!=adcsMap.end();imap++){
      LHCb::CaloAdcs* adcs = (*imap).second;
      if( NULL == adcs) continue;
      LHCb::CaloAdc* adc = adcs->object( id );
      if( NULL == adc ) continue;
      int val = adc->adc();
      sum += val;
      txt << (*imap).first << " : " << val << " | " ;
    }
    
    if(  (double) sum > m_threshold ){
      LHCb::CaloAdc* newAdc = new LHCb::CaloAdc(id, sum);
      newAdcs->insert( newAdc );
      debug() << id << " : "  << txt.str() << " =>  TAE = " << sum <<endreq;
    }

  }

}

