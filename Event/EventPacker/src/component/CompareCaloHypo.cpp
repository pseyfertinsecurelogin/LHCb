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

// local
#include "CompareCaloHypo.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareCaloHypo
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareCaloHypo )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CompareCaloHypo::CompareCaloHypo( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "InputName", m_inputName = LHCb::CaloHypoLocation::Electrons );
  declareProperty( "TestName", m_testName = LHCb::CaloHypoLocation::Electrons + "Test" );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CompareCaloHypo::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  LHCb::CaloHypos* old  = get<LHCb::CaloHypos>( m_inputName );
  LHCb::CaloHypos* test = get<LHCb::CaloHypos>( m_testName );

  // compare and return
  const LHCb::CaloHypoPacker packer( this );
  return packer.check( *old, *test );
}
//=============================================================================
