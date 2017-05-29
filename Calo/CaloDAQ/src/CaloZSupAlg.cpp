// local
#include "CaloZSupAlg.h"

#include <ctype.h>
#include <algorithm>
#include <functional>
#include <memory>
#include <ostream>
#include <vector>

#include "CaloDAQ/ICaloEnergyFromRaw.h"
#include "CaloDet/DeCalorimeter.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#include "Event/RawBankReadoutStatus.h"
#include "Gaudi/Details/PluginServiceDetails.h"
#include "GaudiAlg/GaudiCommonImp.h"
#include "GaudiKernel/Algorithm.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IMessageSvc.h"
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/Property.h"
#include "GaudiKernel/StatEntity.h"
#include "Kernel/CaloCellID.h"

class ISvcLocator;

// ============================================================================
/** @file CaloZSupAlg.cpp
 *
 *    Calorimeter Zero suppression.
 *
 *    @author: Olivier Callot
 *    @date:   10 October 2002
 */
// ============================================================================

DECLARE_ALGORITHM_FACTORY( CaloZSupAlg )

//=============================================================================
// Standard creator, initializes variables
//=============================================================================
CaloZSupAlg::CaloZSupAlg( const std::string& name, ISvcLocator* pSvcLocator)
: GaudiAlgorithm( name , pSvcLocator )
{
  //** Declare the algorithm's properties which can be set at run time and
  //** their default values
  declareProperty("DetectorName"    , m_detectorName       ) ;
  declareProperty("OutputADCData"   , m_outputADCData      ) ;
  declareProperty("OutputDigitData" , m_outputDigitData    ) ;
  declareProperty("ZsupMethod"      , m_zsupMethod         ) ;
  declareProperty("ZsupThreshold"   , m_zsupThreshold      ) ;
  declareProperty("ZsupNeighbor"    , m_zsupNeighbor=-256  ) ;
  declareProperty("OutputType"      , m_outputType = "Digits"  ) ;
  declareProperty( "Extension"      ,  m_extension = "" );
  declareProperty( "StatusOnTES"    , m_statusOnTES = true);

  //=== Default values according to the name of the algorithm !
  m_inputToolType = "CaloEnergyFromRaw";
  m_inputToolName = name + "Tool";
  if ( name.compare( 0 , 4, "Ecal" ) == 0 ) {
    m_detectorName     = DeCalorimeterLocation::Ecal;
    m_outputADCData    = LHCb::CaloAdcLocation::Ecal;
    m_outputDigitData  = LHCb::CaloDigitLocation::Ecal;
    m_zsupMethod       = "2D";
    m_zsup2D           = true;
    m_zsupThreshold    = 20;
    m_zsupNeighbor     = -5; // reject large negative noise
  } else if ( name.compare( 0 , 4, "Hcal" ) == 0 ) {
    m_detectorName     = DeCalorimeterLocation::Hcal;
    m_outputADCData    = LHCb::CaloAdcLocation::Hcal;
    m_outputDigitData  = LHCb::CaloDigitLocation::Hcal;
    m_zsupMethod       = "1D";
    m_zsupThreshold    = 4;
  }
}


//=============================================================================
// Initialisation. Check parameters
//=============================================================================
StatusCode CaloZSupAlg::initialize() {

  StatusCode sc = GaudiAlgorithm::initialize();
  if( sc.isFailure() ) return sc;


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
    error() << "CaloZSupAlg configured to produce ** NO ** output (outputType = '" << m_outputType <<"')" << endmsg;
    return StatusCode::FAILURE;
  }
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) {
    if( m_digitOnTES )debug() <<  "CaloZSupAlg will produce CaloDigits on TES"
                              << rootInTES() + m_outputDigitData + m_extension
                              << endmsg;
    if( m_adcOnTES )debug() <<  "CaloZSupAlg will produce CaloAdcs on TES"
                            << rootInTES() + m_outputADCData + m_extension
                            << endmsg;
     debug() << " get DeCalorimeter from " << m_detectorName << endmsg;
  }
  // Retrieve the calorimeter we are working with.
  m_calo = getDet<DeCalorimeter>( m_detectorName );
  m_numberOfCells = m_calo->numberOfCells();

  //*** A few check of the parameters
  if ( "NO" != m_zsupMethod &&
       "1D" != m_zsupMethod &&
       "2D" != m_zsupMethod) {
    error() << "Unknown Z-sup mode" << m_zsupMethod
            << " (must be NO,1D or 2D)" << endmsg;
    return StatusCode::FAILURE;
  }

  m_zsup2D = ( "2D" == m_zsupMethod );

  if ( "NO" == m_zsupMethod ) {
    if( -1000 != m_zsupThreshold ) {
      info() << " Threshold is reset from "<< m_zsupThreshold
             << " to " << -1000 << endmsg; }
    m_zsupThreshold = -1000 ; // no threshold at all !
  }

  info() << "Calorimeter has " <<  m_numberOfCells
         << " cells. Zsup method "  << m_zsupMethod
         << " Threshold " << m_zsupThreshold << endmsg;

  m_adcTool = tool<ICaloEnergyFromRaw>( m_inputToolType , m_inputToolName,this);


  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloZSupAlg::execute() {

  bool isDebug   = msgLevel() <= MSG::DEBUG;
  bool isVerbose = msgLevel() <= MSG::VERBOSE;

  //*** some trivial printout

  if ( isDebug ) {
    if (m_adcOnTES) debug() << "Perform zero suppression - return CaloAdcs on TES at "
                            << rootInTES() + m_outputADCData + m_extension << endmsg;
    if ( m_digitOnTES) debug() << "Perform zero suppression - return CaloDigits on TES at "
                               << rootInTES() + m_outputDigitData  + m_extension << endmsg;
  }


  //*** get the input data

  const std::vector<LHCb::CaloAdc>& adcs = m_adcTool->adcs( );

  //***  prepare the output containers
  LHCb::CaloAdcs* newAdcs=nullptr;
  LHCb::CaloDigits* newDigits=nullptr;
  if(m_adcOnTES){
    newAdcs = new LHCb::CaloAdcs();
    put( newAdcs, m_outputADCData + m_extension );
  }
  if(m_digitOnTES) {
    newDigits = new LHCb::CaloDigits();
    put( newDigits, m_outputDigitData + m_extension );
  }

  if ( isDebug ) debug() << "Processing " << adcs.size()
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
      if( isVerbose ) {
        verbose() << id
                << format( " Energy adc %4d", digAdc );
        if (  m_zsupThreshold <= digAdc ) debug() << " seed";
        verbose() << endmsg;
      }

      caloFlags[index] = SeedFlag ;
      if( m_zsup2D ) {
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

    if(m_adcOnTES){
      try{
        auto adc = std::make_unique<LHCb::CaloAdc>( id, anAdc.adc() );
        newAdcs->insert( adc.get() ) ;
        adc.release();
      } catch(GaudiException &exc) {
        counter("Duplicate ADC") += 1;
        std::ostringstream os("");
        os << "Duplicate ADC for channel " << id << std::endl;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_adcTool->deCalo()->cardNumber( id );
        int tell1=  m_adcTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_adcTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }
    }

    if(m_digitOnTES){
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
        int card =  m_adcTool->deCalo()->cardNumber( id );
        int tell1=  m_adcTool->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_adcTool->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }

    }

    if( isVerbose ) {
      if ( NeighborFlag == caloFlags[index] ) {
        verbose() << id << " added as Neighbor." << endmsg;
      } else {
        verbose() << id << " added as Seed.    " << endmsg;
      }
    }
  }

  if(isDebug) {
    if(m_adcOnTES)
      debug() << format("Have stored %5d CaloAdcs.", newAdcs->size()) << endmsg;
    if(m_digitOnTES)
      debug() << format("Have stored %5d CaloDigits.", newDigits->size()) << endmsg;
  }

  if(m_statusOnTES)m_adcTool->putStatusOnTES();

  return StatusCode::SUCCESS;
}

//=============================================================================
