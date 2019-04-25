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
#include "Event/StandardPacker.h"

// local
#include "UnpackCaloHypo.h"

//-----------------------------------------------------------------------------
// Implementation file for class : UnpackCaloHypo
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

StatusCode UnpackCaloHypo::execute() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // If input does not exist, and we aren't making the output regardless, just return
  if ( !m_alwaysOutput.value() && !m_packedHypos.exist() ) return StatusCode::SUCCESS;

  const auto* dst = m_packedHypos.getOrCreate();

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Size of PackedCaloHypos = " << dst->hypos().size() << endmsg;

  auto* newCaloHypos = m_hypos.put( std::make_unique<LHCb::CaloHypos>() );

  // unpack
  const LHCb::CaloHypoPacker packer( this );
  packer.unpack( *dst, *newCaloHypos );

  return StatusCode::SUCCESS;
}

//=============================================================================

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( UnpackCaloHypo )

//=============================================================================
