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
#include "Event/RawBank.h"
#include "Event/RawBankReadoutStatus.h"
#include "GaudiAlg/GaudiAlgorithm.h"

//-----------------------------------------------------------------------------
// Implementation file for class : RawBankReadoutStatusFilter
//
// 2007-12-05 : Olivier Deschamps
//-----------------------------------------------------------------------------

/** @class RawBankReadoutStatusFilter RawBankReadoutStatusFilter.h component/RawBankReadoutStatusFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-05
 */
class RawBankReadoutStatusFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawBankReadoutStatusFilter( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode execute() override; ///< Algorithm execution

private:
  mutable Gaudi::Accumulators::BinomialCounter<> m_acceptedEventCount{this, "Accepted events"};

  Gaudi::Property<int>  m_type{this, "BankType", LHCb::RawBank::LastType};
  Gaudi::Property<long> m_mask{this, "RejectionMask", 0x0}; // filterPassed = true anyway
  Gaudi::Property<bool> m_invert{this, "InvertedFilter", false};
};
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( RawBankReadoutStatusFilter )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
RawBankReadoutStatusFilter::RawBankReadoutStatusFilter( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {}

//=============================================================================
// Main execution
//=============================================================================
StatusCode RawBankReadoutStatusFilter::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  int value = LHCb::RawBankReadoutStatus::Status::OK;

  if ( m_type == LHCb::RawBank::LastType ) {
    return Warning( "No BankType requested in RawBankReadoutStatusFilter -> filterPassed = true", StatusCode::SUCCESS,
                    0 );
  }

  LHCb::RawBankReadoutStatus*  status = nullptr;
  LHCb::RawBankReadoutStatuss* statuss =
      getIfExists<LHCb::RawBankReadoutStatuss>( LHCb::RawBankReadoutStatusLocation::Default );
  if ( statuss ) {
    status = statuss->object( LHCb::RawBank::BankType( m_type.value() ) );
  } else {
    Warning( "No Readout status container found at " + LHCb::RawBankReadoutStatusLocation::Default + " for the bank " +
                 Gaudi::Utils::toString( m_type.value() ),
             StatusCode::SUCCESS )
        .ignore();
    value = LHCb::RawBankReadoutStatus::Status::MissingStatus;
  }
  if ( status ) {
    value = status->status();
  } else {
    Warning( "No Readout status found for bankType " + Gaudi::Utils::toString( m_type.value() ), StatusCode::SUCCESS )
        .ignore();
    value = LHCb::RawBankReadoutStatus::Status::MissingStatus;
  }

  int  decision = value & m_mask;
  bool accept   = ( decision != 0 ? m_invert.value() : !m_invert.value() );
  setFilterPassed( accept );
  m_acceptedEventCount += accept;

  if ( msgLevel( MSG::DEBUG ) )
    debug() << "Status value : " << value << " Mask : " << m_mask << " => " << filterPassed() << endmsg;
  return StatusCode::SUCCESS;
}
//=============================================================================
