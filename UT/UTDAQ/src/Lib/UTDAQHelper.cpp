#include "UTDAQ/UTDAQHelper.h"
#include "Event/RawBank.h"
#include "SiDAQ/SiHeaderWord.h"


std::optional<unsigned int>
LHCb::UTDAQ::nbUTClusters(LHCb::span<const RawBank *> banks,
                          unsigned int maxNbClusters) {
  size_t nbClusters = 0;
  for (auto &bank : banks) {
    nbClusters += SiHeaderWord(bank->data()[0]).nClusters();
    // cut as soon as we have too many
    if (nbClusters > maxNbClusters) {
      return {};
    }
  }
  return nbClusters;
}