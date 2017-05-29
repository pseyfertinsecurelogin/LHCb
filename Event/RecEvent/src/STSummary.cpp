
#include "Event/STSummary.h"

#include <utility>

// fillstream method
std::ostream& LHCb::STSummary::fillStream(std::ostream& s) const
{

  std::string syncStatus = "OK";
  if ( !m_pcnSynch ) syncStatus = "Not" + syncStatus; 
  s << "Summary Block " << std::endl;
  s << "# Clusters " << m_nClusters << " PCN " << m_pcn << " Sync status " << syncStatus;
  s << std::endl;
  s << "# bytes " << m_rawBufferSize << std::endl;
  s << "# full banks " << m_nFullBanks << std::endl;
  s << "# pedestal banks " << m_nPedestalBanks << std::endl;
  s << "# error banks " << m_nErrorBanks << std::endl;
  if ( !m_missingBanks.empty() ) 
  {
    // print the missing in banks
    s << "Missing In Action";
    for ( const auto & M : m_missingBanks )
    {
      s << "Missing: Bank " << M << std::endl;      
    } 
  }

  if ( !m_corruptedBanks.empty() )
  {
    // print the banks in error
    s << "Corrupted banks: ";
    for ( const auto & E : m_corruptedBanks )
    {
      s << "Corrupted: Bank " << E << std::endl;      
    } 
  }
 
 
  if ( !m_recoveredBanks.empty() ) 
  {
    // print the banks that were recovered
    s << "Recovered: ";
    for ( const auto & R : m_recoveredBanks )
    {
      s << " Bank " << R.first << " fraction " << R.second;
    } 
  }

  return s;
}
