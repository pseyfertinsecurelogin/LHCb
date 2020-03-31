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
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IDataSelector.h"
#include "GaudiKernel/IDataStoreLeaves.h"
#include "GaudiKernel/ObjectContainerBase.h"
#include <fstream>
#include <map>

//-----------------------------------------------------------------------------
// Implementation file for class : TESFingerPrint
//
// 2011-05-11 : Illya Shapoval
//-----------------------------------------------------------------------------

/** @class TESFingerPrint TESFingerPrint.h
 *
 *
 *  @author Illya Shapoval
 *  @date   2011-05-11
 */
class TESFingerPrint final : public GaudiAlgorithm {

public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode execute() override;  ///< Algorithm execution
  StatusCode finalize() override; ///< Algorithm finalization

private:
  /// Pointer to the (public) tool used to retrieve the objects in a file.
  PublicToolHandle<IDataStoreLeaves> m_leavesTool{this, "DataStoreLeavesTool", "DataSvcFileEntriesTool"};
  /// Counter map of nodes occurrences
  std::map<std::string, long> m_stat_map;
  /// Counter map of containers and their contents occurrences
  std::map<std::string, std::map<int, long>> m_cont_stat_map;

  /// Level of the heuristic analysis
  Gaudi::Property<std::string> m_heur_level{this, "HeuristicsLevel", "Low",
                                            "The level of TES heuristic analysis to be performed to obtain"
                                            " its finger print."};
  /// File name for the TES finger print output
  Gaudi::Property<std::string> m_output_file_name{this, "OutputFileName", "tes_finger_print.dat",
                                                  "The name of the output file to store the TES finger print."};
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( TESFingerPrint )

//=============================================================================
// Main execution
//=============================================================================
StatusCode TESFingerPrint::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // Get the objects in the same file as the root node
  /// Collection of objects being selected
  IDataSelector objects;
  try {
    const auto& leaves = m_leavesTool->leaves();
    objects.assign( leaves.begin(), leaves.end() );
  } catch ( GaudiException& e ) {
    error() << e.message() << endmsg;
    return StatusCode::FAILURE;
  }
  // Collect TES statistics
  for ( const auto* o : objects ) {
    const auto& entry = o->registry()->identifier();
    if ( auto [iter, inserted] = m_stat_map.try_emplace( entry, 1 ); !inserted ) ++iter->second;
    if ( m_heur_level == "Medium" ) {
      const auto* pCont = dynamic_cast<const ObjectContainerBase*>( o );
      if ( pCont ) {
        const auto cont_size = pCont->numberOfObjects();
        if ( auto [iter, inserted] = m_cont_stat_map[entry].try_emplace( cont_size, 1 ); !inserted ) ++iter->second;
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

  std::ofstream tes_finger_print{m_output_file_name.value()};

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
