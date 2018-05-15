#include "FTDAQ/FTDAQHelper.h"
#include "Event/RawBank.h"
#include <numeric>      // std::accumulate

unsigned int LHCb::FTDAQ::nbFTClusters(const std::vector<RawBank *> &banks ) {
  // Simple loop over the banks to roughly estimate total number of clusters
  unsigned int nbClusters =
      std::accumulate(banks.begin(), banks.end(), 0u,
                      [](unsigned int sum, auto& bank)
                        { return sum + bank->size(); } );

  // Note that we check only the version of the first bank, supposing they
  // all have a consistent version
  int bankversion = (*begin(banks))->version() ;

  if( bankversion==2 || bankversion == 3) {
    // Bank size is roughly half the number of clusters and includes headers
    nbClusters *= 0.4;
  } else {
    // Bank size is given in bytes. There are 2 bytes per cluster.
    // Note that this overestimates slightly the number of clusters
    // due to bank padding. For v5, it further overestimates the
    // number of clusters due to the merging of clusters.
    nbClusters /= 2;
  }

  return nbClusters ;
}
