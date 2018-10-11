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

#include "Event/L0PrsSpdHit.h"

// local
#include "CaloFutureTriggerBitsFromRawAlg.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureTriggerBitsFromRawAlg
//
// 2006-04-07 : Olivier Deschamps
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CaloFutureTriggerBitsFromRawAlg )


//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CaloFutureTriggerBitsFromRawAlg::CaloFutureTriggerBitsFromRawAlg( const std::string& name,
                                                      ISvcLocator* pSvcLocator)
: GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty("OutputData"  , m_outputData  );
  declareProperty( "Extension"  ,  m_extension = "" );
  declareProperty( "StatusOnTES"   , m_statusOnTES = true);


  m_toolType  = "CaloFutureTriggerBitsFromRaw";
  m_toolName = name + "Tool";
  if ( name.compare( 0 , 3, "Prs" ) == 0 ) {
    m_outputData = LHCb::L0PrsSpdHitLocation::Prs + m_extension;
    m_isPrs = true;
  } else if ( name.compare( 0 , 3, "Spd" ) == 0 ) {
    m_outputData = LHCb::L0PrsSpdHitLocation::Spd + m_extension;
  }

}
//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloFutureTriggerBitsFromRawAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << "==> Initialize " << name() << endmsg;

  m_l0BitTool = tool<ICaloFutureTriggerBitsFromRaw>( m_toolType, m_toolName , this);
  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureTriggerBitsFromRawAlg::execute() {

  if( UNLIKELY( msgLevel(MSG::DEBUG) ) ) debug() << "==> Execute"  <<endmsg;
  //*** create the output container
  LHCb::L0PrsSpdHits* newL0Bits = new LHCb::L0PrsSpdHits();
  put( newL0Bits , m_outputData );
  //*** get the input data from Raw and fill the output container
  LHCb::CaloFuture::FiredCells l0Cells = ( m_isPrs ? m_l0BitTool->prsCells()
                                             : m_l0BitTool->spdCells() );

  for( const auto& iCell : l0Cells ) {
    // protect against SEU
    try{
      auto l0Bit = std::make_unique<LHCb::L0PrsSpdHit>( iCell );
      newL0Bits->insert( l0Bit.get() ) ;
      l0Bit.release();
    }catch(GaudiException &exc) {
      counter("Duplicate l0Bit") += 1;
      std::ostringstream os;
      os << "Duplicate l0Bit for channel " << iCell << " " << std::endl;
      Warning(os.str(),StatusCode::SUCCESS).ignore();
      int card =  m_l0BitTool->deCaloFuture()->cardNumber( iCell );
      int tell1=  m_l0BitTool->deCaloFuture()->cardToTell1( card);
      LHCb::RawBankReadoutStatus& status = m_l0BitTool->status();
      status.addStatus( tell1 ,LHCb::RawBankReadoutStatus::Status::DuplicateEntry);
    }
  }

  if (m_statusOnTES) m_l0BitTool->putStatusOnTES();
  if (UNLIKELY( msgLevel(MSG::DEBUG) ) )
    debug() << " L0PrsSpdHits container size " << newL0Bits->size() << endmsg;
  return StatusCode::SUCCESS;
}

//=============================================================================
