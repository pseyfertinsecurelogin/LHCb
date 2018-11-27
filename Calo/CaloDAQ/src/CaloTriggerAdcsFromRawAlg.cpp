/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
DECLARE_COMPONENT( CaloTriggerAdcsFromRawAlg )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloTriggerAdcsFromRawAlg::CaloTriggerAdcsFromRawAlg( const std::string& name,
                                                      ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
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
  m_l0AdcTool = tool<ICaloTriggerAdcsFromRaw>( "CaloTriggerAdcsFromRaw", name() + "Tool", this);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloTriggerAdcsFromRawAlg::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute"  <<endmsg;

  //*** create the output container
  LHCb::L0CaloAdcs* newL0Adcs = new LHCb::L0CaloAdcs();
  put( newL0Adcs, m_outputData.value() );

  //*** get the input data from Raw and fill the output container
  for( const auto& il0Adc : m_l0AdcTool->adcs( )) {
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
      status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
    }
  }
  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << " L0CaloAdcs container" << m_outputData.value()
            <<" size " << newL0Adcs->size() << endmsg;

  //*** get the PinDiode data and fill the output container
  // MUST BE AFTER STANDARD ADCs
  const std::vector<LHCb::L0CaloAdc>& l0PinAdcs = m_l0AdcTool->pinAdcs( );
  if( "None" != m_pinContainer && !l0PinAdcs.empty() ){
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
      status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
    }

    }
    if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
      debug() << " PinDiode : L0CaloAdcs container " << m_pinContainer
              << " size " << newL0Adcs->size() << endmsg;
  }

  if(m_statusOnTES.value()) m_l0AdcTool->putStatusOnTES();
  return StatusCode::SUCCESS;
}

//=============================================================================
