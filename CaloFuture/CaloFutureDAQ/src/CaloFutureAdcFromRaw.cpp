/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
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
#include "CaloFutureAdcFromRaw.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureAdcFromRaw
//
//  generic algorithm to put CaloFuture(L0)Adc and L0PrsSpdBit on TES from rawEvent
//  Possibility to apply condDB calibration factor on ADC
//  ---> to be used to produce mis-calibrated data from calibrated ADC !!
//
// 2009-04-07 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CaloFutureAdcFromRaw )


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
CaloFutureAdcFromRaw::CaloFutureAdcFromRaw( const std::string& name,
                                ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
, m_detectorName( futuredetails::alg_name_to_detector( name ) )
{
 switch (m_detectorName) {
 case futuredetails::DetectorName_t::Ecal :
   m_location      = LHCb::CaloAdcLocation::Ecal    ;
   m_l0Location    = LHCb::L0CaloAdcLocation::Ecal  ;
   m_l0BitLocation = "";
   m_caloName    = DeCalorimeterLocation::Ecal;
   m_offset = +256;
   break;
 case futuredetails::DetectorName_t::Hcal:
   m_location    = LHCb::CaloAdcLocation::Hcal    ;
   m_l0Location  = LHCb::L0CaloAdcLocation::Hcal ;
   m_l0BitLocation = "";
   m_caloName = DeCalorimeterLocation::Hcal;
   m_offset = +256;
   break;
 case futuredetails::DetectorName_t::Prs:
   m_location       = LHCb::CaloAdcLocation::Prs ;
   m_l0Location     = "";
   m_l0BitLocation  = LHCb::L0PrsSpdHitLocation::Prs;
   m_caloName = DeCalorimeterLocation::Prs;
   m_offset = 0;
   break;
 case futuredetails::DetectorName_t::Spd:
   m_location       = "";
   m_l0Location     = "";
   m_l0BitLocation  = LHCb::L0PrsSpdHitLocation::Spd;
   m_caloName = DeCalorimeterLocation::Spd;
   m_offset = 0;
   break;
 default:
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
StatusCode CaloFutureAdcFromRaw::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Initialize" << endmsg;

  // get detector elements
  if( m_caloName.empty() ) return Error("Unknown calo detector name ",StatusCode::FAILURE);
  m_calo  = getDet<DeCalorimeter>( m_caloName );
  // get data provider tools
  m_data = tool<ICaloFutureDataProvider>("CaloFutureDataProvider", toString(m_detectorName) + "DataProvider",this);
  m_l0data = tool<ICaloFutureL0DataProvider>("CaloFutureL0DataProvider", toString(m_detectorName) + "L0DataProvider",this);

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureAdcFromRaw::execute() {

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
        int card =  m_data->deCaloFuture()->cardNumber( id );
        int tell1=  m_data->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_data->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
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
        int card =  m_data->deCaloFuture()->cardNumber( id );
        int tell1=  m_data->deCaloFuture()->cardToTell1( card);
        LHCb::RawBankReadoutStatus& status = m_data->status();
        status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
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
          os << "Duplicate L0Bit for channel " << id << '\n';
          Warning(os.str(),StatusCode::SUCCESS).ignore();
          int card =  m_data->deCaloFuture()->cardNumber( id );
          int tell1=  m_data->deCaloFuture()->cardToTell1( card);
          LHCb::RawBankReadoutStatus& status = m_data->status();
          status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
        }
      }
    }
  }
  return StatusCode::SUCCESS;
}

//=============================================================================
