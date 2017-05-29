// Include files

// local
#include "CaloAdcFromRaw.h"

#include <functional>
#include <memory>
#include <ostream>

#include "CaloDAQ/ICaloDataProvider.h"
#include "CaloDAQ/ICaloL0DataProvider.h"
#include "CaloDet/CellParam.h"
#include "CaloDet/DeCalorimeter.h"
#include "CaloKernel/CaloVector.h"
#include "Event/CaloAdc.h"
#include "Event/L0CaloAdc.h"
#include "Event/L0PrsSpdHit.h"
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

//-----------------------------------------------------------------------------
// Implementation file for class : CaloAdcFromRaw
//
//  generic algorithm to put Calo(L0)Adc and L0PrsSpdBit on TES from rawEvent
//  Possibility to apply condDB calibration factor on ADC
//  ---> to be used to produce mis-calibrated data from calibrated ADC !!
//
// 2009-04-07 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_ALGORITHM_FACTORY( CaloAdcFromRaw )

namespace {

// C++17:
template <typename T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi) noexcept
{
        return ( v < lo ) ? lo : ( hi < v ) ? hi : v;
}

}
//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloAdcFromRaw::CaloAdcFromRaw( const std::string& name,
                                ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("AdcLocation"        , m_location );
  declareProperty("L0AdcLocation"      , m_l0Location );
  declareProperty("L0BitLocation"      , m_l0BitLocation );
  declareProperty("DeCalibration"      , m_calib ); // Expert usage

  // set default detectorName
  int index = name.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
  m_detectorName = ( name.compare(index, 3, "Prs") == 0 ? "Prs"
                   : name.compare(index, 3, "Spd") == 0 ? "Spd"
                   : name.substr( index, 4 ) );

 if( "Ecal" == m_detectorName ) {
   m_location      = LHCb::CaloAdcLocation::Ecal    ;
   m_l0Location    = LHCb::L0CaloAdcLocation::Ecal  ;
   m_l0BitLocation = "";
   m_caloName    = DeCalorimeterLocation::Ecal;
   m_offset = +256;
 }else if( "Hcal" == m_detectorName ) {
   m_location    = LHCb::CaloAdcLocation::Hcal    ;
   m_l0Location  = LHCb::L0CaloAdcLocation::Hcal ;
   m_l0BitLocation = "";
   m_caloName = DeCalorimeterLocation::Hcal;
   m_offset = +256;
 }else if( "Prs" == m_detectorName ) {
   m_location       = LHCb::CaloAdcLocation::Prs ;
   m_l0Location     = "";
   m_l0BitLocation  = LHCb::L0PrsSpdHitLocation::Prs;
   m_caloName = DeCalorimeterLocation::Prs;
   m_offset = 0;
 }else if( "Spd" == m_detectorName ) {
   m_location       = "";
   m_l0Location     = "";
   m_l0BitLocation  = LHCb::L0PrsSpdHitLocation::Spd;
   m_caloName = DeCalorimeterLocation::Spd;
   m_offset = 0;
 }else{
   m_location = "";
   m_l0Location = "";
   m_l0BitLocation = "";
   m_caloName = "";
   m_offset = 0;
 }
}
//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloAdcFromRaw::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  // get detector elements
  if( m_caloName.empty() ) return Error("Unknown calo detector name " + m_detectorName,StatusCode::FAILURE);
  m_calo  = getDet<DeCalorimeter>( m_caloName );
  // get data provider tools
  m_data = tool<ICaloDataProvider>("CaloDataProvider", m_detectorName + "DataProvider",this);
  m_l0data = tool<ICaloL0DataProvider>("CaloL0DataProvider",m_detectorName + "L0DataProvider",this);

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloAdcFromRaw::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute" << endmsg;

  // ADCs (ecal/hcal/prs)
  if( !m_location.empty() && m_data->getBanks()){
    LHCb::CaloAdcs* outs = new LHCb::CaloAdcs();
    put( outs , m_location);
    const CaloVector<LHCb::CaloAdc>& adcs = m_data->adcs();

    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << " #ADCS " << adcs.size() << endmsg;
    for(LHCb::CaloAdc adc : adcs ) {
      LHCb::CaloCellID id = adc.cellID();
      int value = adc.adc();
      double calib = 1.;
      if( m_calib){
        calib  = m_calo->cellParam( id ).calibration();
        value = (calib > 0) ? int( (double) adc.adc() / calib) : 0 ;
        if( m_calo->isDead( id )) value = 0;
      }
      value = clamp( value, -m_offset, static_cast<int>(m_calo->adcMax()) - m_offset );
      try{
        auto out = std::make_unique<LHCb::CaloAdc>( id, value);
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug() << "Inserting : " << id << " adc = " << value << "  =  " << adc.adc() << " / " << calib
                  << "  (dead channel ? " << m_calo->isDead( id ) << ")" << endmsg;
        outs->insert(out.get());
        out.release();
      } catch(GaudiException &exc) {
        counter("Duplicate CaloADC") += 1;
        std::ostringstream os("");
        os << "Duplicate CaloADC for channel " << id << std::endl;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_data->deCalo()->cardNumber( id );
        int tell1=  m_data->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_data->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }
    }
  }
  // L0ADCs (ecal/hcal)
  if( !m_l0Location.empty() && m_l0data->getBanks() ){
    LHCb::L0CaloAdcs* outs = new LHCb::L0CaloAdcs();
    put( outs , m_l0Location);
    for(LHCb::L0CaloAdc adc : m_l0data->l0Adcs() ) {
      LHCb::CaloCellID id = adc.cellID();
      int value = adc.adc();
      double calib = 1.;
      if( m_calib){
        calib  = m_calo->cellParam( id ).calibration();
        value = ( calib > 0) ? int( (double) adc.adc() / calib) : 0;
        if( m_calo->isDead( id ))value = 0;
      }
      value = clamp( value, 0, 255 ); // 8 bin L0ADC
      if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
        debug() << "Inserting : " << id << " l0adc = " << value << "  =  " << adc.adc() << " / " << calib
                << "  (dead channel ? " << m_calo->isDead( id ) << ")" << endmsg;

      try {
        auto out = std::make_unique<LHCb::L0CaloAdc>( id, value);
        outs->insert(out.get());
        out.release();
      } catch(GaudiException &exc) {
        counter("Duplicate L0ADC") += 1;
        std::ostringstream os("");
        os << "Duplicate L0ADC for channel " << id << std::endl;
        Warning(os.str(),StatusCode::SUCCESS).ignore();
        int card =  m_data->deCalo()->cardNumber( id );
        int tell1=  m_data->deCalo()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_data->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
      }
    }
  }
  // Trigger bits (prs/spd)
  if( !m_l0BitLocation.empty() && m_l0data->getBanks() ){
    LHCb::L0PrsSpdHits* outs = new LHCb::L0PrsSpdHits();
    put( outs , m_l0BitLocation);
    for(LHCb::L0CaloAdc adc : m_l0data->l0Adcs()) {
      LHCb::CaloCellID id = adc.cellID();
      int value = adc.adc();
      if( m_calib & m_calo->isDead( id ))value = 0;
      int satur = 1; // 1 bit trigger
      if(value > satur)value = satur;
      if(value < 0)value = 0;
      if( 1 == value){
        if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
          debug() << "Inserting : " << id << " bit = " << value
                  << "  (dead channel ? " << m_calo->isDead( id ) << ")" << endmsg;
        try{
          auto out = std::make_unique<LHCb::L0PrsSpdHit>( id );
          outs->insert(out.get());
          out.release();
        } catch(GaudiException &exc) {
          counter("Duplicate L0Bit") += 1;
          std::ostringstream os("");
          os << "Duplicate L0Bit for channel " << id << std::endl;
          Warning(os.str(),StatusCode::SUCCESS).ignore();
          int card =  m_data->deCalo()->cardNumber( id );
          int tell1=  m_data->deCalo()->cardToTell1( card);
          LHCb::RawBankReadoutStatus& status = m_data->status();
          status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::DuplicateEntry);
        }
      }
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
