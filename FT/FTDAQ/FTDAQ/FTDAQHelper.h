#ifndef FTDAQHELPER_H
#define FTDAQHELPER_H 1

#include <vector>
#include <boost/optional.hpp>

namespace LHCb {

// forward declaration
class RawBank;

namespace FTDAQ {

/**
 * counts number of FT clusters in the given raw banks
 * if count exceeds max, it gives up and returns no value
 */
boost::optional<unsigned int> nbFTClusters(const std::vector<RawBank *> &banks,
                                           unsigned int maxNbClusters,
                                           unsigned int clusterMaxWidth);

} // namespace FTDAQ

} // namespace LHCb

#endif // FTDAQHELPER_H
