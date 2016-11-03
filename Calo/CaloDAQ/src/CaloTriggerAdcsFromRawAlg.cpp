// Include files 

#include "Event/L0CaloAdc.h" 

// local
#include "CaloTriggerAdcsFromRawAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloTriggerAdcsFromRawAlg
//
// 2006-04-07 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( CaloTriggerAdcsFromRawAlg )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloTriggerAdcsFromRawAlg::CaloTriggerAdcsFromRawAlg( const std::string& name,
                                                      ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator ), m_l0AdcTool(0)
{
  declareProperty("OutputData"  , m_outputData  );  
  declareProperty( "Extension"  ,  m_extension = "" );
  declareProperty( "PinContainer"  ,  m_pinContainer );
  declareProperty( "StatusOnTES"   , m_statusOnTES = true);
  

  m_toolType = "CaloTriggerAdcsFromRaw";  
  m_toolName  = name + "Tool";  
  if ( name.compare( 0 , 4,  "Ecal" ) == 0 ) {
    m_outputData   = LHCb::L0CaloAdcLocation::Ecal    + m_extension;    
    m_pinContainer = LHCb::L0CaloAdcLocation::EcalPin + m_extension; 
  } else if ( name.compare( 0 , 4, "Hcal")  == 0 ) {
    m_outputData   = LHCb::L0CaloAdcLocation::Hcal    + m_extension;
    m_pinContainer = LHCb::L0CaloAdcLocation::HcalPin + m_extension; 
  }

}

//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloTriggerAdcsFromRawAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" <<  endmsg;
  m_l0AdcTool = tool<ICaloTriggerAdcsFromRaw>( m_toolType , m_toolName, this);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloTriggerAdcsFromRawAlg::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute"  <<endmsg;

  //*** create the output container
  LHCb::L0CaloAdcs* newL0Adcs = new LHCb::L0CaloAdcs();
  put( newL0Adcs, m_outputData );

  //*** get the input data from Raw and fill the output container
  const std::vector<LHCb::L0CaloAdc>& l0Adcs = m_l0AdcTool->adcs( );


  for( const auto& il0Adc : l0Adcs) {
    try{
      auto adc = std::make_unique<LHCb::L0CaloAdc>( il0Adc.cellID(), il0Adc.adc() );    
      newL0Adcs->insert( adc.get() ) ;
      adc.release();
    } catch(GaudiException &exc) { 
      counter("Duplicate L0ADC") += 1;
      std::ostringstream os;
      os << "Duplicate l0ADC for channel " << il0Adc.cellID() << std::endl;
      Warning(os.str(),StatusCode::SUCCESS).ignore();
      int card =  m_l0AdcTool->deCalo()->cardNumber( il0Adc.cellID() );
      int tell1=  m_l0AdcTool->deCalo()->cardToTell1( card);
      LHCb::RawBankReadoutStatus& status = m_l0AdcTool->status();
      status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
    }     
  }
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) 
    debug() << " L0CaloAdcs container" << m_outputData 
            <<" size " << newL0Adcs->size() << endmsg;
  
  //*** get the PinDiode data and fill the output container
  // MUST BE AFTER STANDARD ADCs
  const std::vector<LHCb::L0CaloAdc>& l0PinAdcs = m_l0AdcTool->pinAdcs( );
  if( "None" != m_pinContainer && 0 != l0PinAdcs.size() ){
    LHCb::L0CaloAdcs* newL0PinAdcs = new LHCb::L0CaloAdcs();
    put( newL0PinAdcs, m_pinContainer );
    for( const auto& il0PinAdc : l0PinAdcs) {
    try{
      auto pinAdc = std::make_unique<LHCb::L0CaloAdc>( il0PinAdc.cellID(), il0PinAdc.adc() );
      newL0PinAdcs->insert( pinAdc.get() ) ;
      pinAdc.release();
    } catch(GaudiException &exc) { 
      counter("Duplicate PIN L0ADC") += 1;
      std::ostringstream os("");
      os << "Duplicate PIN l0ADC for channel " << il0PinAdc.cellID() << std::endl;
      Warning(os.str(),StatusCode::SUCCESS).ignore();
      int card =  m_l0AdcTool->deCalo()->cardNumber( il0PinAdc.cellID() );
      int tell1=  m_l0AdcTool->deCalo()->cardToTell1( card);
      LHCb::RawBankReadoutStatus& status = m_l0AdcTool->status();
      status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
    }

    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) 
      debug() << " PinDiode : L0CaloAdcs container " << m_pinContainer 
              << " size " << newL0Adcs->size() << endmsg;
  }

  if(m_statusOnTES)m_l0AdcTool->putStatusOnTES();  
  return StatusCode::SUCCESS;
}
//=============================================================================
