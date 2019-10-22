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

#include "Event/UTSummary.h"

// fillstream method
std::ostream& LHCb::UTSummary::fillStream( std::ostream& s ) const {

  std::string syncStatus = "OK";
  if ( !m_pcnSynch ) { syncStatus = "Not" + syncStatus; }
  s << "Summary Block " << std::endl;
  s << "# Clusters " << m_nClusters << " PCN " << m_pcn << " Sync status " << syncStatus;
  s << std::endl;
  s << "# bytes " << m_rawBufferSize << std::endl;
  s << "# full banks " << m_nFullBanks << std::endl;
  s << "# pedestal banks " << m_nPedestalBanks << std::endl;
  s << "# error banks " << m_nErrorBanks << std::endl;
  if ( !m_missingBanks.empty() ) {
    // print the missing in banks
    s << "Missing In Action";
    for ( const auto& M : m_missingBanks ) { s << "Missing: Bank " << M << std::endl; }
  }

  if ( !m_corruptedBanks.empty() ) {
    // print the banks in error
    s << "Corrupted banks: ";
    for ( const auto& E : m_corruptedBanks ) { s << "Corrupted: Bank " << E << std::endl; }
  }

  if ( !m_recoveredBanks.empty() ) {
    // print the banks that were recovered
    s << "Recovered: ";
    for ( const auto& R : m_recoveredBanks ) { s << " Bank " << R.first << " fraction " << R.second; }
  }

  return s;
}
