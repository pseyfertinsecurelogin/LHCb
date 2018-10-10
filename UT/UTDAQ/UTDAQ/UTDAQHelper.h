#ifndef UTDAQHELPER_H
#define UTDAQHELPER_H 1

#include <optional>
#include "Kernel/STLExtensions.h"
#include <array>
#include <boost/container/small_vector.hpp>
#include "STDet/DeUTDetector.h"
#include "STDet/DeUTSector.h"

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

  struct LayerInfo final {
    float z;
    unsigned int nColsPerSide;
    unsigned int nRowsPerSide;
    float invHalfSectorYSize;
    float invHalfSectorXSize;
    float dxDy;
  };
  using SectorsInRegionZ = std::array<float,98>;
  using SectorsInLayerZ = std::array<SectorsInRegionZ,3>;
  using SectorsInStationZ = std::array<SectorsInLayerZ,2>;

  constexpr static const auto mapQuarterSectorToSectorCentralRegion = std::array {
    6,6,9,9,10,10,13,13,
    7,7,8,8,11,11,12,12,
    25,25,26,28,31,33,34,34,
    24,24,27,29,30,32,35,35,
    46,46,49,51,52,54,57,57,
    47,47,48,50,53,55,56,56,
    69,69,70,70,73,73,74,74,
    68,68,71,71,72,72,75,75
  };

  constexpr static const auto  mapSectorToSector = std::array {
    1,2,3,4,5,0,0,0,0,14,15,16,17,18,
    19,20,21,22,23,0,0,0,0,36,37,38,39,40,
    41,42,43,44,45,0,0,0,0,58,59,60,61,62,
    63,64,65,66,67,0,0,0,0,76,77,78,79,80
  };

  /**
   * fills container of (region, sector) pairs with all sectors concerned by
   * a hit at given layer and coordinates and with given x tolerance
   */
  void findSectors(unsigned int layer, float x, float y, float xTol, float yTol,
                   const LayerInfo& info,
                   boost::container::small_vector_base<std::pair<int, int>>& sectors);

  void computeGeometry(const DeSTDetector &utDet,
                       std::array<LayerInfo,4> &layers,
                       std::array<SectorsInStationZ,2> &sectorsZ);

} // namespace UTDAQ

} // namespace LHCb

#endif // UTDAQHELPER_H
