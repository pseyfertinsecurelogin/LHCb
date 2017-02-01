// Include files

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"

// local
#include "CaloDigitsFromRaw.h"

namespace {

  static const auto IncreasingByCellID =
      []( const LHCb::CaloDigit* dig1 , 
          const LHCb::CaloDigit* dig2 ) {
      return  !dig1 || ( dig2 && ( dig1->cellID().all() < dig2->cellID().all() ));
  };

}

//-----------------------------------------------------------------------------
// Implementation file for class : CaloDigitsFromRaw
//
// 2003-11-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_ALGORITHM_FACTORY( CaloDigitsFromRaw )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloDigitsFromRaw::CaloDigitsFromRaw( const std::string& name,
                                      ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  m_detectorNum = CaloCellCode::CaloNumFromName( name ) ;
  if ( m_detectorNum < 0 || m_detectorNum >= (int) CaloCellCode::CaloNums )
  { m_detectorNum = CaloCellCode::CaloNumFromName( "Ecal" ) ; }

  declareProperty( "Extension"               ,  m_extension = "" );
  declareProperty( "OutputType"              ,  m_outputType = "Digits"  ) ;
  declareProperty( "PinContainer"            ,  m_pinContainerName );
  declareProperty( "DigitsContainer"         ,  m_outputDigits );
  declareProperty( "AdcsContainer"           ,  m_outputADCs);
  declareProperty( "StatusOnTES"             ,  m_statusOnTES = true);

  if( 2 == m_detectorNum ) {
    m_outputDigits     = LHCb::CaloDigitLocation::Ecal  ;
    m_outputADCs       = LHCb::CaloAdcLocation::Ecal    ;
    m_pinContainerName = LHCb::CaloAdcLocation::EcalPin ;
  }
  else if( 3 == m_detectorNum ) {
    m_outputDigits     = LHCb::CaloDigitLocation::Hcal  ;
    m_outputADCs       = LHCb::CaloAdcLocation::Hcal    ;
    m_pinContainerName = LHCb::CaloAdcLocation::HcalPin ;
  }
  else if( 1 == m_detectorNum ) {
    m_outputDigits     = LHCb::CaloDigitLocation::Prs ;
    m_outputADCs       = LHCb::CaloAdcLocation::Prs ;
    m_pinContainerName = "None";
   }
  else if( 0 == m_detectorNum ) {
    m_outputDigits     = LHCb::CaloDigitLocation::Spd ;
    m_outputADCs       = LHCb::CaloAdcLocation::Spd ;
    m_pinContainerName = "None";
  }

}

//=========================================================================
//  Initialization
//=========================================================================
StatusCode CaloDigitsFromRaw::initialize ( ) {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;
  // get DeCalorimeter
  if( 2 == m_detectorNum ) {
    m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal );
  }
  else if( 3 == m_detectorNum ) {
    m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal );
  }
  else if( 1 == m_detectorNum ) {
    m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs );
   }
  else if( 0 == m_detectorNum ) {
    m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Spd );
  }
  else {
    std::ostringstream os;
    os << "Invalid detector Num =  " << m_detectorNum;
    return( Error( os.str() ) );
  }


  //
  if( 0 == m_detectorNum ){
    m_spdTool = tool<ICaloTriggerBitsFromRaw>( "CaloTriggerBitsFromRaw",  name() + "Tool",this );
  }else {
    m_energyTool = tool<ICaloEnergyFromRaw>( "CaloEnergyFromRaw",  name() + "Tool",this );
  }

  //
  std::string out( m_outputType );
  std::transform( m_outputType.begin() , m_outputType.end() , out.begin () , ::toupper ) ;
  m_outputType = out;
  if( m_outputType == "DIGITS" ||  m_outputType == "CALODIGITS" ||
      m_outputType == "DIGIT"  ||  m_outputType == "CALODIGIT"  ||
      m_outputType == "BOTH") m_digitOnTES = true;
  if(m_outputType == "ADCS" ||  m_outputType == "CALOADCS" ||
     m_outputType == "ADC"  ||  m_outputType == "CALOADC"  ||
     m_outputType == "BOTH")m_adcOnTES = true;
  if( !m_adcOnTES && !m_digitOnTES ){
    error()<< "CaloDigitsFromRaw configured to produce ** NO ** output (outputType = '" << m_outputType <<"')" << endmsg;
    return StatusCode::FAILURE;
  }
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    if( m_digitOnTES )
      debug() <<  "CaloDigitsFromRaw will produce CaloDigits on TES at "
              << rootInTES() + m_outputDigits + m_extension << endmsg;
    if( m_adcOnTES )
      debug() <<  "CaloDigitsFromRaw will produce CaloAdcs on TES at "
              << rootInTES() + m_outputADCs + m_extension << endmsg;
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloDigitsFromRaw::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  if       ( 0 == m_detectorNum ) {
    convertSpd ( 3.2 * Gaudi::Units::MeV );
  } else {
    convertCaloEnergies ( );
  }
  return StatusCode::SUCCESS;
}

//=========================================================================
//  Convert the SPD trigger bits to CaloDigits
//=========================================================================
void CaloDigitsFromRaw::convertSpd ( double energyScale ) {

  LHCb::Calo::FiredCells spdCells = m_spdTool->spdCells( );

  if(m_digitOnTES){
    LHCb::CaloDigits* digits = new LHCb::CaloDigits();
    put( digits, m_outputDigits + m_extension );
    for ( const auto& cells : spdCells ) {
      try{
        auto dig = std::make_unique<LHCb::CaloDigit>( cells, energyScale );
        digits->insert( dig.get() );
        dig.release();
      }catch(GaudiException &exc) {
        counter("Duplicate Spd 'digit'") += 1;
        std::ostringstream os; os << "Duplicate digit for channel " << cells; 
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_spdTool->deCalo()->cardNumber( cells );
        int tell1=  m_spdTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_spdTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }
    }
    std::stable_sort ( digits->begin(), digits->end(),
                       IncreasingByCellID );
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << m_outputDigits + m_extension << " CaloDigit container size " << digits->size() << endmsg;
  }

  if(m_adcOnTES){
    LHCb::CaloAdcs* adcs = new LHCb::CaloAdcs();
    put( adcs ,  m_outputADCs + m_extension );
    for ( const auto& cells : spdCells ) {
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( cells, 1 );
        adcs->insert( adc.get() );
        adc.release();
      }catch(GaudiException &exc){
        counter("Duplicate Spd 'ADC'") += 1;
        std::ostringstream os; os << "Duplicate ADC for channel " << cells;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_spdTool->deCalo()->cardNumber( cells );
        int tell1=  m_spdTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_spdTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }

    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() <<  m_outputADCs + m_extension << " CaloAdc container size " << adcs->size() << endmsg;
  }

  if(m_statusOnTES)m_spdTool->putStatusOnTES();


}

//=========================================================================
//  Converts the standard calorimeter adc-energy
//=========================================================================
void CaloDigitsFromRaw::convertCaloEnergies ( ) {

  if(m_digitOnTES){
    LHCb::CaloDigits* digits = new LHCb::CaloDigits();
    put( digits, m_outputDigits+ m_extension );
    for ( const auto& itD : m_energyTool->digits( ) ) {
      try {
        std::unique_ptr<LHCb::CaloDigit> dig{ itD.clone() };
        digits->insert( dig.get() );
        dig.release();
      } catch(GaudiException &exc) {
        counter("Duplicate CaloDigit") += 1;
        std::ostringstream os;
        os << "Duplicate digit for channel " << itD.cellID();
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_energyTool->deCalo()->cardNumber( itD.cellID() );
        int tell1=  m_energyTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_energyTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }

      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose() << "ID " << itD.cellID() << " energy " << itD.e() << endmsg;
    }
    std::stable_sort ( digits->begin(), digits->end(),
                       IncreasingByCellID );
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << m_outputDigits+ m_extension << " CaloDigit container size " << digits->size() << endmsg;
    if(m_statusOnTES)m_energyTool->putStatusOnTES();

  }

  if(m_adcOnTES){
    // Channel ADC
    LHCb::CaloAdcs* adcs = new LHCb::CaloAdcs();
    put( adcs ,  m_outputADCs+ m_extension );
    const std::vector<LHCb::CaloAdc>& allAdcs = m_energyTool->adcs( );
    if(m_statusOnTES)m_energyTool->putStatusOnTES();
    for ( const auto& itA : allAdcs ) {
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( itA.cellID(), itA.adc() ); // 'clone'
        adcs->insert(adc.get());
        adc.release();
      }catch(GaudiException &exc){
        counter("Duplicate CaloDigit") += 1;
        std::ostringstream os; os << "Duplicate digit for channel " << itA.cellID();
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_energyTool->deCalo()->cardNumber( itA.cellID() );
        int tell1=  m_energyTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_energyTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose() << "ID " << itA.cellID() << " ADC value " << itA.adc() << endmsg;
    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << " CaloAdc container '"  << m_outputADCs+ m_extension  << "' -> size = " << adcs->size() << endmsg;


    // PinDiode ADC (possibly in a different container)
    // MUST BE AFTER STANDARD ADCs decoding
    const std::vector<LHCb::CaloAdc>& allPinAdcs = m_energyTool->pinAdcs( );
    if( "None" != m_pinContainerName && 0 !=allPinAdcs.size() ){
      LHCb::CaloAdcs*  pinAdcs;
      if(m_pinContainerName == m_outputDigits || m_pinContainerName == "SAME"|| m_pinContainerName == "Same"){
        pinAdcs = adcs ;
      }else{
        pinAdcs = new LHCb::CaloAdcs();
        put(pinAdcs , m_pinContainerName + m_extension );
      }
      for ( const auto& itA : allPinAdcs ) {
        try{
          auto pinAdc = std::make_unique<LHCb::CaloAdc>( itA.cellID(), itA.adc() ); // 'clone'
          pinAdcs->insert(pinAdc.get());
          pinAdc.release();
        }catch(GaudiException &exc){
          counter("Duplicate CaloDigit") += 1;
          std::ostringstream os("");
          os << "Duplicate digit for channel " << itA.cellID();
          Warning(os.str(),StatusCode::SUCCESS).ignore();
          int card =  m_energyTool->deCalo()->cardNumber( itA.cellID() );
          int tell1=  m_energyTool->deCalo()->cardToTell1( card);
          LHCb::RawBankReadoutStatus& status = m_energyTool->status();
          status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
        }

        if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
          verbose() << "Pin-diode : ID " << itA.cellID() << " ADC value " << itA.adc() << endmsg;
      }
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << " Adding PIN-Diode CaloAdc to container '" << m_pinContainerName + m_extension
                << "' -> size = " << pinAdcs->size() << endmsg;
    }
    if(m_statusOnTES)m_energyTool->putStatusOnTES();
  }

}
//=============================================================================
