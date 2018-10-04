// Include files

// from Gaudi
#include "GaudiKernel/SystemOfUnits.h"

// local
#include "CaloFutureDigitsFromRaw.h"

DECLARE_COMPONENT( CaloFutureDigitsFromRaw )

namespace {

  static const auto IncreasingByCellID =
      []( const LHCb::CaloDigit* dig1 ,
          const LHCb::CaloDigit* dig2 ) {
      return  !dig1 || ( dig2 && ( dig1->cellID().all() < dig2->cellID().all() ));
  };

}

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureDigitsFromRaw
//
// 2003-11-18 : Olivier Callot
//-----------------------------------------------------------------------------


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureDigitsFromRaw::CaloFutureDigitsFromRaw( const std::string& name,
                                      ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  m_detectorNum = CaloCellCode::CaloNumFromName( name ) ;
  if ( m_detectorNum < 0 || m_detectorNum >= (int) CaloCellCode::CaloNums )
  { m_detectorNum = CaloCellCode::CaloNumFromName( "Ecal" ) ; }

  switch (m_detectorNum) {
      case 2 : m_outputDigits     = LHCb::CaloDigitLocation::Ecal  ;
               m_outputADCs       = LHCb::CaloAdcLocation::Ecal    ;
               m_pinContainerName = LHCb::CaloAdcLocation::EcalPin ;
               break;
      case 3 : m_outputDigits     = LHCb::CaloDigitLocation::Hcal  ;
               m_outputADCs       = LHCb::CaloAdcLocation::Hcal    ;
               m_pinContainerName = LHCb::CaloAdcLocation::HcalPin ;
               break;
      case 1 : m_outputDigits     = LHCb::CaloDigitLocation::Prs ;
               m_outputADCs       = LHCb::CaloAdcLocation::Prs ;
               m_pinContainerName = "None";
               break;
      case 0 : m_outputDigits     = LHCb::CaloDigitLocation::Spd ;
               m_outputADCs       = LHCb::CaloAdcLocation::Spd ;
               m_pinContainerName = "None";
               break;
  }

}

//=========================================================================
//  Initialization
//=========================================================================
StatusCode CaloFutureDigitsFromRaw::initialize ( ) {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first

  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;

  // get DeCalorimeter
  switch( m_detectorNum ) {
      case 2 : m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Ecal ); break;
      case 3 : m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Hcal ); break;
      case 1 : m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Prs ); break;
      case 0 : m_calo = getDet<DeCalorimeter>( DeCalorimeterLocation::Spd ); break;
      default :
            std::ostringstream os;
            os << "Invalid detector Num =  " << m_detectorNum;
            return( Error( os.str() ) );
  }
  //
  if( 0 == m_detectorNum ){
    m_spdTool = tool<ICaloFutureTriggerBitsFromRaw>( "CaloFutureTriggerBitsFromRaw",  name() + "Tool",this );
  }else {
    m_energyTool = tool<ICaloFutureEnergyFromRaw>( "CaloFutureEnergyFromRaw",  name() + "Tool",this );
  }

  //
  if( msgLevel(MSG::DEBUG) ) {
    if( m_outputType.value().digitOnTES )
      debug() <<  "CaloFutureDigitsFromRaw will produce CaloDigits on TES at "
              << rootInTES() + m_outputDigits + m_extension << endmsg;
    if( m_outputType.value().adcOnTES )
      debug() <<  "CaloFutureDigitsFromRaw will produce CaloAdcs on TES at "
              << rootInTES() + m_outputADCs + m_extension << endmsg;
  }

  return StatusCode::SUCCESS;
}
//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureDigitsFromRaw::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  switch (m_detectorNum) {
      case 0  : convertSpd ( 3.2 * Gaudi::Units::MeV ); break;
      default : convertCaloFutureEnergies( ); break;
  }
  return StatusCode::SUCCESS;
}

//=========================================================================
//  Convert the SPD trigger bits to CaloDigits
//=========================================================================
void CaloFutureDigitsFromRaw::convertSpd ( double energyScale ) {

  LHCb::CaloFuture::FiredCells spdCells = m_spdTool->spdCells( );

  if(m_outputType.value().digitOnTES){
    LHCb::CaloDigits* digits = new LHCb::CaloDigits();
    put( digits, m_outputDigits.value() + m_extension.value() );
    for ( const auto& cells : spdCells ) {
      try{
        auto dig = std::make_unique<LHCb::CaloDigit>( cells, energyScale );
        digits->insert( dig.get() );
        dig.release();
      }catch(GaudiException &exc) {
        counter("Duplicate Spd 'digit'") += 1;
        std::ostringstream os; os << "Duplicate digit for channel " << cells;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_spdTool->deCaloFuture()->cardNumber( cells );
        int tell1=  m_spdTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_spdTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }
    }
    std::stable_sort ( digits->begin(), digits->end(),
                       IncreasingByCellID );
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << m_outputDigits.value() + m_extension.value() << " CaloDigit container size " << digits->size() << endmsg;
  }

  if(m_outputType.value().adcOnTES){
    LHCb::CaloAdcs* adcs = new LHCb::CaloAdcs();
    put( adcs ,  m_outputADCs.value() + m_extension.value() );
    for ( const auto& cells : spdCells ) {
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( cells, 1 );
        adcs->insert( adc.get() );
        adc.release();
      }catch(GaudiException &exc){
        counter("Duplicate Spd 'ADC'") += 1;
        std::ostringstream os; os << "Duplicate ADC for channel " << cells;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_spdTool->deCaloFuture()->cardNumber( cells );
        int tell1=  m_spdTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_spdTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }

    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() <<  m_outputADCs.value() + m_extension.value() << " CaloAdc container size " << adcs->size() << endmsg;
  }

  if(m_statusOnTES.value())m_spdTool->putStatusOnTES();


}

//=========================================================================
//  Converts the standard calorimeter adc-energy
//=========================================================================
void CaloFutureDigitsFromRaw::convertCaloFutureEnergies ( ) {

  if(m_outputType.value().digitOnTES){
    LHCb::CaloDigits* digits = new LHCb::CaloDigits();
    put( digits, m_outputDigits.value() + m_extension.value() );
    for ( const auto& itD : m_energyTool->digits( ) ) {
      try {
        auto dig = std::make_unique<LHCb::CaloDigit>(itD);
        digits->insert( dig.get() );
        dig.release();
      } catch(GaudiException &exc) {
        counter("Duplicate CaloDigit") += 1;
        std::ostringstream os;
        os << "Duplicate digit for channel " << itD.cellID();
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_energyTool->deCaloFuture()->cardNumber( itD.cellID() );
        int tell1=  m_energyTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_energyTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }

      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose() << "ID " << itD.cellID() << " energy " << itD.e() << endmsg;
    }
    std::stable_sort ( digits->begin(), digits->end(),
                       IncreasingByCellID );
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << m_outputDigits.value()+ m_extension.value() << " CaloDigit container size " << digits->size() << endmsg;
    if(m_statusOnTES.value())m_energyTool->putStatusOnTES();

  }

  if(m_outputType.value().adcOnTES){
    // Channel ADC
    LHCb::CaloAdcs* adcs = new LHCb::CaloAdcs();
    put( adcs ,  m_outputADCs.value()+ m_extension.value() );
    const std::vector<LHCb::CaloAdc>& allAdcs = m_energyTool->adcs( );
    if(m_statusOnTES.value())m_energyTool->putStatusOnTES();
    for ( const auto& itA : allAdcs ) {
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( itA.cellID(), itA.adc() ); // 'clone'
        adcs->insert(adc.get());
        adc.release();
      }catch(GaudiException &exc){
        counter("Duplicate CaloDigit") += 1;
        std::ostringstream os; os << "Duplicate digit for channel " << itA.cellID();
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_energyTool->deCaloFuture()->cardNumber( itA.cellID() );
        int tell1=  m_energyTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_energyTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }
      if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
        verbose() << "ID " << itA.cellID() << " ADC value " << itA.adc() << endmsg;
    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << " CaloAdc container '"  << m_outputADCs.value()+ m_extension.value()  << "' -> size = " << adcs->size() << endmsg;


    // PinDiode ADC (possibly in a different container)
    // MUST BE AFTER STANDARD ADCs decoding
    const std::vector<LHCb::CaloAdc>& allPinAdcs = m_energyTool->pinAdcs( );
    if( "None" != m_pinContainerName && !allPinAdcs.empty() ){
      LHCb::CaloAdcs*  pinAdcs;
      if(m_pinContainerName.value() == m_outputDigits.value() || m_pinContainerName == "SAME"|| m_pinContainerName == "Same"){
        pinAdcs = adcs ;
      }else{
        pinAdcs = new LHCb::CaloAdcs();
        put(pinAdcs , m_pinContainerName.value() + m_extension.value() );
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
          int card =  m_energyTool->deCaloFuture()->cardNumber( itA.cellID() );
          int tell1=  m_energyTool->deCaloFuture()->cardToTell1( card);
          LHCb::RawBankReadoutStatus& status = m_energyTool->status();
          status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
        }

        if( UNLIKELY( msgLevel(MSG::VERBOSE) ) )
          verbose() << "Pin-diode : ID " << itA.cellID() << " ADC value " << itA.adc() << endmsg;
      }
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << " Adding PIN-Diode CaloAdc to container '" << m_pinContainerName.value() + m_extension.value()
                << "' -> size = " << pinAdcs->size() << endmsg;
    }
    if(m_statusOnTES.value())m_energyTool->putStatusOnTES();
  }

}

//=============================================================================
