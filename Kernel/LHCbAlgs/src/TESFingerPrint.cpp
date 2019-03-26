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
// from Gaudi
#include "GaudiKernel/IDataStoreLeaves.h"
#include "GaudiKernel/ObjectContainerBase.h"
// local
#include "TESFingerPrint.h"
#include <fstream>

//-----------------------------------------------------------------------------
// Implementation file for class : TESFingerPrint
//
// 2011-05-11 : Illya Shapoval
//-----------------------------------------------------------------------------

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TESFingerPrint )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
TESFingerPrint::TESFingerPrint( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "HeuristicsLevel", m_heur_level = "Low",
                   "The level of TES heuristic analysis to be performed to obtain"
                   " its finger print." );
  declareProperty( "OutputFileName", m_output_file_name = "tes_finger_print.dat",
                   "The name of the output file to store the TES finger print." );
}

//=============================================================================
// Initialization
//=============================================================================
StatusCode TESFingerPrint::initialize() {
  StatusCode sc = GaudiAlgorithm::initialize(); // must be executed first
  if ( sc.isFailure() ) return sc;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initialize" << endmsg;

  sc = toolSvc()->retrieveTool( "DataSvcFileEntriesTool", m_leavesTool );
  if ( sc.isFailure() ) return sc;

  return sc;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode TESFingerPrint::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // Get the objects in the same file as the root node
  try {
    const auto& leaves = m_leavesTool->leaves();
    m_objects.assign( leaves.begin(), leaves.end() );
  } catch ( GaudiException& e ) {
    error() << e.message() << endmsg;
    return StatusCode::FAILURE;
  }
  // Collect TES statistics
  std::string entry;
  for ( const auto* o : m_objects ) {
    entry = o->registry()->identifier();

    auto& m = m_stat_map[entry];
    if ( m ) {
      ++m;
    } else {
      m = 1;
    }

    if ( m_heur_level == "Medium" ) {
      const auto* pCont = dynamic_cast<const ObjectContainerBase*>( o );
      if ( pCont ) {
        const auto cont_size = pCont->numberOfObjects();
        auto&      mm        = m_cont_stat_map[entry][cont_size];
        if ( mm ) {
          ++mm;
        } else {
          mm = 1;
        }
      }
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
//  Finalize
//=============================================================================
StatusCode TESFingerPrint::finalize() {
  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Finalize" << endmsg;

  toolSvc()->releaseTool( m_leavesTool ).ignore();
  m_leavesTool = nullptr;

  std::ofstream tes_finger_print;
  tes_finger_print.open( m_output_file_name.c_str() );

  using namespace GaudiUtils;
  // Write low level TES statistics
  tes_finger_print << m_stat_map << "\n";
  if ( msgLevel( MSG::VERBOSE ) ) { verbose() << "==> TES DataObject statistics: " << m_stat_map << endmsg; }

  // Write medium level TES statistics
  if ( m_heur_level == "Medium" ) {
    tes_finger_print << m_cont_stat_map << "\n";
    if ( msgLevel( MSG::VERBOSE ) ) { verbose() << "==> TES Container sizes: " << m_cont_stat_map << endmsg; }
  }

  tes_finger_print.close();

  return GaudiAlgorithm::finalize(); // must be called after all other actions
}

//=============================================================================
