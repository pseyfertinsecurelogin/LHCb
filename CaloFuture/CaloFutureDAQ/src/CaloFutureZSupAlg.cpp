// local
#include "CaloFutureZSupAlg.h"

// ============================================================================
/** @file CaloFutureZSupAlg.cpp
 *
 *    CalorimeterFuture Zero suppression.
 *
 *    @author: Olivier Callot
 *    @date:   10 October 2002
 */
// ============================================================================

DECLARE_COMPONENT( CaloFutureZSupAlg )


namespace futuredetails {
    const char* toString(const zsupMethod_t& out) {
        switch(out) {
            case zsupMethod_t::none : return "NO";
            case zsupMethod_t::one_d: return "1D";
            case zsupMethod_t::two_d: return "2D";
        }
        throw "IMPOSSIBLE!";
    }
    StatusCode parse(zsupMethod_t& result, const std::string& input ) {
        if (input == toString(zsupMethod_t::none))  {
            result = zsupMethod_t::none;  return StatusCode::SUCCESS;
        }
        if (input == toString(zsupMethod_t::one_d)) {
            result = zsupMethod_t::one_d; return StatusCode::SUCCESS;
        }
        if (input == toString(zsupMethod_t::two_d)) {
            result = zsupMethod_t::two_d; return StatusCode::SUCCESS;
        }
        return StatusCode::FAILURE;
    }
}

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CaloFutureZSupAlg::CaloFutureZSupAlg( const std::string& name, ISvcLocator* pSvcLocator)
: GaudiAlgorithm( name , pSvcLocator )
{
  //=== Default values according to the name of the algorithm !
  if ( name.compare( 6 , 4, "Ecal" ) == 0 ) {
    m_detectorName     = DeCalorimeterLocation::Ecal;
    m_outputADCData    = LHCb::CaloAdcLocation::Ecal;
    m_outputDigitData  = LHCb::CaloDigitLocation::Ecal;
    m_zsupMethod       = futuredetails::zsupMethod_t::two_d;
    m_zsupThreshold    = 20;
    m_zsupNeighbor     = -5; // reject large negative noise
  } else if ( name.compare( 6 , 4, "Hcal" ) == 0 ) {
    m_detectorName     = DeCalorimeterLocation::Hcal;
    m_outputADCData    = LHCb::CaloAdcLocation::Hcal;
    m_outputDigitData  = LHCb::CaloDigitLocation::Hcal;
    m_zsupMethod       = futuredetails::zsupMethod_t::one_d;
    m_zsupThreshold    = 4;
  }
}


//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode CaloFutureZSupAlg::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) return sc;


  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    if( m_outputType.value().digitOnTES )
        debug() <<  "CaloFutureZSupAlg will produce CaloDigits on TES"
                              << rootInTES() + m_outputDigitData + m_extension
                              << endmsg;
    if( m_outputType.value().adcOnTES )
        debug() <<  "CaloFutureZSupAlg will produce CaloAdcs on TES"
                            << rootInTES() + m_outputADCData + m_extension
                            << endmsg;
     debug() << " get DeCalorimeter from " << m_detectorName.value() << endmsg;
  }
  // Retrieve the calorimeter we are working with.
  m_calo = getDet<DeCalorimeter>( m_detectorName.value() );
  m_numberOfCells = m_calo->numberOfCells();

  if ( m_zsupMethod == futuredetails::zsupMethod_t::none ) {
    if( -1000 != m_zsupThreshold ) {
      info() << " Threshold is reset from "<< m_zsupThreshold
             << " to " << -1000 << endmsg; }
    m_zsupThreshold = -1000 ; // no threshold at all !
  }

  info() << "CalorimeterFuture has " <<  m_numberOfCells
         << " cells. Zsup method "  << futuredetails::toString(m_zsupMethod.value())
         << " Threshold " << m_zsupThreshold.value() << endmsg;

  m_adcTool = tool<ICaloFutureEnergyFromRaw>("CaloFutureEnergyFromRaw" , name() + "Tool", this );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureZSupAlg::execute() {

  //*** some trivial printout
  if ( msgLevel(MSG::DEBUG) ) {
    if (m_outputType.value().adcOnTES) debug() << "Perform zero suppression - return CaloAdcs on TES at "
                            << rootInTES() + m_outputADCData + m_extension << endmsg;
    if ( m_outputType.value().digitOnTES) debug() << "Perform zero suppression - return CaloDigits on TES at "
                               << rootInTES() + m_outputDigitData  + m_extension << endmsg;
  }


  //*** get the input data
  const std::vector<LHCb::CaloAdc>& adcs = m_adcTool->adcs( );

  //***  prepare the output containers
  LHCb::CaloAdcs* newAdcs=nullptr;
  LHCb::CaloDigits* newDigits=nullptr;
  if(m_outputType.value().adcOnTES){
    newAdcs = new LHCb::CaloAdcs();
    put( newAdcs, m_outputADCData.value() + m_extension.value() );
  }
  if(m_outputType.value().digitOnTES) {
    newDigits = new LHCb::CaloDigits();
    put( newDigits, m_outputDigitData.value() + m_extension.value() );
  }

  if ( msgLevel(MSG::DEBUG) ) debug() << "Processing " << adcs.size()
                         << " Digits." << endmsg;

  enum {
    DefaultFlag   ,
    NeighborFlag  ,
    SeedFlag       };

  std::vector<int> caloFlags    ( m_numberOfCells, DefaultFlag ) ;

  int index;

  // == Apply the threshold. If 2DZsup, tag also the neighbours
  for( const auto& anAdc : adcs ) {

    LHCb::CaloCellID id = anAdc.cellID();

    index         = m_calo->cellIndex( id );
    int    digAdc = anAdc.adc();
    if( m_zsupThreshold <= digAdc ) {
      if( msgLevel(MSG::VERBOSE) ) {
        verbose() << id
                << format( " Energy adc %4d", digAdc );
        if (  m_zsupThreshold <= digAdc ) debug() << " seed";
        verbose() << endmsg;
      }

      caloFlags[index] = SeedFlag ;
      if( m_zsupMethod == futuredetails::zsupMethod_t::two_d ) {
        for ( const auto& neighbor :  m_calo->neighborCells( id ) ) {
          int& neighFlag = caloFlags[m_calo->cellIndex(neighbor)];
          if( neighFlag != SeedFlag ) neighFlag = NeighborFlag ;
        }
      }
    }
  }

  //** write tagged data as CaloAdc or CaloDigits according to m_digitsOutput

  double pedShift = m_calo->pedestalShift();
  for( const auto& anAdc : adcs ) {
    LHCb::CaloCellID id = anAdc.cellID();
    index         = m_calo->cellIndex( id );
    if( DefaultFlag == caloFlags[index] ) { continue; }
    if( NeighborFlag == caloFlags[index] && anAdc.adc() < m_zsupNeighbor)continue;

    if(m_outputType.value().adcOnTES){
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( id, anAdc.adc() );
        newAdcs->insert( adc.get() ) ;
        adc.release();
      } catch(GaudiException &exc) {
        counter("Duplicate ADC") += 1;
        std::ostringstream os("");
        os << "Duplicate ADC for channel " << id << std::endl;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_adcTool->deCaloFuture()->cardNumber( id );
        int tell1=  m_adcTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_adcTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }
    }

    if(m_outputType.value().digitOnTES){
      double e = ( double( anAdc.adc() ) - pedShift ) * m_calo->cellGain( id );
      try{
        auto digit = std::make_unique<LHCb::CaloDigit>(id,e);
        newDigits->insert( digit.get() ) ;
        digit.release();
      }
      catch(GaudiException &exc) {
        counter("Duplicate Digit") += 1;
        std::ostringstream os;
        os << "Duplicate Digit for channel " << id << std::endl;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_adcTool->deCaloFuture()->cardNumber( id );
        int tell1=  m_adcTool->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_adcTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
      }

    }

    if( msgLevel(MSG::VERBOSE) ) {
      if ( NeighborFlag == caloFlags[index] ) {
        verbose() << id << " added as Neighbor." << endmsg;
      } else {
        verbose() << id << " added as Seed.    " << endmsg;
      }
    }
  }

  if(msgLevel(MSG::DEBUG)) {
    if(m_outputType.value().adcOnTES)
      debug() << format("Have stored %5d CaloAdcs.", newAdcs->size()) << endmsg;
    if(m_outputType.value().digitOnTES)
      debug() << format("Have stored %5d CaloDigits.", newDigits->size()) << endmsg;
  }

  if(m_statusOnTES.value())m_adcTool->putStatusOnTES();

  return StatusCode::SUCCESS;
}
//=============================================================================
