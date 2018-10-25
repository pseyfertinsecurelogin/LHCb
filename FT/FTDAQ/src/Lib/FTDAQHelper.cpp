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
#include "FTDAQ/FTDAQHelper.h"
#include "Event/RawBank.h"
#include <numeric>      // std::accumulate

unsigned int LHCb::FTDAQ::nbFTClusters(LHCb::span<const RawBank *> banks ) {
  // Simple loop over the banks to roughly estimate total number of clusters
  unsigned int nbClusters =
      std::accumulate(banks.begin(), banks.end(), 0u,
                      [](unsigned int sum, auto& bank)
                        { return sum + bank->size(); } );

  // Note that we check only the version of the first bank, supposing they
  // all have a consistent version
  int bankversion = (*banks.begin())->version() ;

  if( bankversion==2 || bankversion == 3) {
    // Bank size is roughly half the number of clusters and includes headers
    nbClusters *= 0.4;
  } else {
    // Bank size is given in bytes. There are 2 bytes per cluster.
    // 4 bytes are removed for the header.
    // Note that this overestimates slightly the number of clusters
    // due to bank padding in 32b. For v5, it further overestimates the
    // number of clusters due to the merging of clusters.
    nbClusters = nbClusters/2 - 2u;
  }

  return nbClusters ;
}
