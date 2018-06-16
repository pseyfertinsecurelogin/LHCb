#ifndef FTDAQHELPER_H
#define FTDAQHELPER_H 1

#include <vector>

namespace LHCb {

// forward declaration
class RawBank;

namespace FTDAQ {

/**
 * counts number of FT clusters in the given raw banks
 */
unsigned int nbFTClusters(const std::vector<RawBank *> &banks);

} // namespace FTDAQ

} // namespace LHCb

#endif // FTDAQHELPER_H
