#include "FTDAQ/FTDAQHelper.h"
#include "../FTRawBankParams.h"
#include "Event/RawBank.h"
#include <boost/optional.hpp>

boost::optional<unsigned int>
LHCb::FTDAQ::nbFTClusters(const std::vector<RawBank *> &banks,
                          unsigned int maxNbClusters,
                          unsigned int clusterMaxWidth) {
  unsigned int nbClusters = 0;
  // first rough count of FT clusters banks
  // we ignore for now the cluster group encoding specific to version 3
  for (auto &bank : banks) {
    auto first = bank->begin<short int>();
    auto last = bank->end<short int>();
    while (first != last) {
      int sipmHeader = *first++;
      int nClus = sipmHeader & FTRawBank::nbClusMaximum;
      nbClusters += nClus;
      // maybe clusters are already too numerous ?
      if (nbClusters > maxNbClusters) {
        return {};
      }
      first += nClus;
    }
  }

  // Now deal with FT bank version 3.
  // For these, the previous count is missing clusters, as only the edges
  // of cluster groups were counted.
  // Note that we check only the version of the first bank, supposing they
  // all have a consistent version
  if ((*begin(banks))->version() == 3) {
    for (auto &bank : banks) {
      auto first = bank->begin<short int>();
      auto last = bank->end<short int>();
      while (first != last) {
        int sipmHeader = *first++;
        int nClus = sipmHeader & FTRawBank::nbClusMaximum;
        for (auto it = first; it < first + nClus; ++it) {
          short int c = *it;
          bool cSize = (c >> FTRawBank::sizeShift) & FTRawBank::sizeMaximum;
          // not the last cluster
          if (!cSize && it < first + nClus - 1) {
            short int c2 = *(it + 1);
            bool cSize2 = (c2 >> FTRawBank::sizeShift) & FTRawBank::sizeMaximum;
            // fragmented cluster, last edge found
            if (cSize2) {
              unsigned channel =
                  (c >> FTRawBank::cellShift) & FTRawBank::cellMaximum;
              unsigned channel2 =
                  (c2 >> FTRawBank::cellShift) & FTRawBank::cellMaximum;
              unsigned int diff = (channel2 - channel);
              // fragmented clusters, size > 2*max size. Only edges were saved,
              // add middles
              if (diff > clusterMaxWidth) {
                nbClusters += (diff - 1) / clusterMaxWidth + 1;
                // cut as soon as we have too many
                if (nbClusters > maxNbClusters) {
                  return {};
                }
              }
            } // end if adjacent clusters
            ++it;
          }
        }
        first += nClus;
      }
    }
  }
  return nbClusters;
}
