#ifndef UTDAQHELPER_H
#define UTDAQHELPER_H 1

#include <optional>
#include "Kernel/STLExtensions.h"

namespace LHCb {

// forward declaration
class RawBank;

namespace UTDAQ {

/**
 * counts number of UT clusters in the given raw banks
 * if count exceeds max, it gives up and returns no value
 */
  std::optional<unsigned int> nbUTClusters(LHCb::span<const RawBank *> banks,
                                           unsigned int maxNbClusters);

} // namespace UTDAQ

} // namespace LHCb

#endif // UTDAQHELPER_H
