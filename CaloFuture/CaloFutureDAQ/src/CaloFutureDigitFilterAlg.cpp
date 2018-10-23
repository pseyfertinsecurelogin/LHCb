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

// from Gaudi
#include "CaloDet/DeCalorimeter.h"
// local
#include "CaloFutureDigitFilterAlg.h"

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CaloFutureDigitFilterAlg )



//-----------------------------------------------------------------------------
// Implementation file for class : CaloFutureDigitFilterAlg
//
// 2010-12-21 : Olivier Deschamps
//-----------------------------------------------------------------------------
namespace {
  constexpr bool mask(int val)   {return (val&0x1) != 0;}
  constexpr bool offset(int val) {return (val&0x2) != 0;}
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode CaloFutureDigitFilterAlg::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;  // error printed already by GaudiAlgorithm

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Initialize" << endmsg;

  m_filter = tool<ICaloFutureDigitFilterTool>("CaloFutureDigitFilterTool","FilterTool");

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CaloFutureDigitFilterAlg::execute() {

  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  if( mask(m_ecal) || offset(m_ecal) )
    if( !m_filter->cleanDigits("Ecal",offset(m_ecal),mask(m_ecal)))
      Warning("Ecal digits filtering failed",StatusCode::SUCCESS).ignore();

  if( mask(m_hcal) || offset(m_hcal) )
    if( !m_filter->cleanDigits("Hcal",offset(m_hcal),mask(m_hcal)))
      Warning("Hcal digits filtering failed",StatusCode::SUCCESS).ignore();

  if( mask(m_prs) || offset(m_prs) )
    if( !m_filter->cleanDigits("Prs",offset(m_prs),mask(m_prs)))
      Warning("Prs digit filtering failed",StatusCode::SUCCESS).ignore();

  if( mask(m_spd) || offset(m_spd) )
    if( !m_filter->cleanDigits("Spd",offset(m_spd),mask(m_spd)))
      Warning("Spd digit filtering failed",StatusCode::SUCCESS).ignore();

  return StatusCode::SUCCESS;
}

//=============================================================================
