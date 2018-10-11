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
#include "Event/StandardPacker.h"
#include "Event/PackedCaloHypo.h"
#include "Event/CaloHypo.h"

// local
#include "UnpackCaloHypo.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackCaloHypo
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
UnpackCaloHypo::UnpackCaloHypo( const std::string& name,
                                ISvcLocator* pSvcLocator)
  : GaudiAlgorithm ( name , pSvcLocator )
{
  declareProperty( "InputName" , m_inputName  = LHCb::PackedCaloHypoLocation::Electrons );
  declareProperty( "OutputName", m_outputName = LHCb::CaloHypoLocation::Electrons );
  declareProperty( "AlwaysCreateOutput", m_alwaysOutput = false );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode UnpackCaloHypo::execute() 
{
  if ( msgLevel(MSG::DEBUG) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput && !exist<LHCb::PackedCaloHypos>(m_inputName) )
    return StatusCode::SUCCESS;

  const auto* dst =
    getOrCreate<LHCb::PackedCaloHypos,LHCb::PackedCaloHypos>( m_inputName );

  if ( msgLevel(MSG::DEBUG) )
    debug() << "Size of PackedCaloHypos = " << dst->hypos().size() << endmsg;

  auto* newCaloHypos = new LHCb::CaloHypos();
  put( newCaloHypos, m_outputName );

   // unpack
  const LHCb::CaloHypoPacker packer(this);
  packer.unpack( *dst, *newCaloHypos );

  return StatusCode::SUCCESS;
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UnpackCaloHypo )

//=============================================================================
