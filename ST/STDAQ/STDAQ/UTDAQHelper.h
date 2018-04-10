#ifndef UTDAQHELPER_H
#define UTDAQHELPER_H 1

#include <vector>
#include <boost/optional.hpp>

namespace LHCb {

// forward declaration
class RawBank;

namespace UTDAQ {

/**
 * counts number of UT clusters in the given raw banks
 * if count exceeds max, it gives up and returns no value
 */
boost::optional<unsigned int> nbUTClusters(const std::vector<RawBank *> &banks,
                                           unsigned int maxNbClusters);

} // namespace UTDAQ

} // namespace LHCb

#endif // UTDAQHELPER_H
