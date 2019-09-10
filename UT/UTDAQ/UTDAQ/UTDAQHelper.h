/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef UTDAQHELPER_H
#define UTDAQHELPER_H 1

#include "Kernel/STLExtensions.h"
#include "UTDet/DeUTDetector.h"
#include "UTDet/DeUTSector.h"
#include "UTInfo.h"
#include <array>
#include <boost/container/small_vector.hpp>
#include <optional>

namespace LHCb {

  // forward declaration
  class RawBank;

  namespace UTDAQ {

    /**
     * counts number of UT clusters in the given raw banks
     * if count exceeds max, it gives up and returns no value
     */
    std::optional<unsigned int> nbUTClusters( LHCb::span<const RawBank*> banks, unsigned int maxNbClusters );

    struct LayerInfo final {
      float        z;
      unsigned int nColsPerSide;
      unsigned int nRowsPerSide;
      float        invHalfSectorYSize;
      float        invHalfSectorXSize;
      float        dxDy;
    };
    using SectorsInRegionZ  = std::array<float, UTInfo::Sectors>;
    using SectorsInLayerZ   = std::array<SectorsInRegionZ, UTInfo::Regions>;
    using SectorsInStationZ = std::array<SectorsInLayerZ, UTInfo::Layers>;

    // constexpr static const auto mapQuarterSectorToSectorCentralRegion =
    //  std::array{6,  6,  9,  9,  10, 10, 13, 13, 7,  7,  8,  8,  11, 11, 12, 12, 25, 25, 26, 28, 31, 33,
    //             34, 34, 24, 24, 27, 29, 30, 32, 35, 35, 46, 46, 49, 51, 52, 54, 57, 57, 47, 47, 48, 50,
    //             53, 55, 56, 56, 69, 69, 70, 70, 73, 73, 74, 74, 68, 68, 71, 71, 72, 72, 75, 75};

    // constexpr static const auto mapQuarterSectorToSectorCentralRegion =
    //    std::array{6,  6,  9,  9,  10, 10, 13, 13, 7,  7,  8,  8,  11, 11, 12, 12, 24, 24, 27, 28, 31, 32,
    //               35, 35, 25, 25, 26, 29, 30, 33, 34, 34, 47, 47, 48, 51, 52, 55, 56, 56, 46, 46, 49, 50,
    //               53, 54, 57, 57, 69, 69, 70, 70, 73, 73, 74, 74, 68, 68, 71, 71, 72, 72, 75, 75};

    // -- For the moment, this is assigned here and overwritten in "computeGeometry" in case a geometry
    // -- version with a "wrong" sector ordering is used
    static std::array<int, 64> mapQuarterSectorToSectorCentralRegion =
        std::array{6,  6,  9,  9,  10, 10, 13, 13, 7,  7,  8,  8,  11, 11, 12, 12, 24, 24, 27, 28, 31, 32,
                   35, 35, 25, 25, 26, 29, 30, 33, 34, 34, 47, 47, 48, 51, 52, 55, 56, 56, 46, 46, 49, 50,
                   53, 54, 57, 57, 69, 69, 70, 70, 73, 73, 74, 74, 68, 68, 71, 71, 72, 72, 75, 75};

    constexpr static const auto mapSectorToSector = std::array{
        1,  2,  3,  4,  5,  0, 0, 0, 0, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 0, 0, 0, 0, 36, 37, 38, 39, 40,
        41, 42, 43, 44, 45, 0, 0, 0, 0, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 0, 0, 0, 0, 76, 77, 78, 79, 80};

    /**
     * fills container of (region, sector) pairs with all sectors concerned by
     * a hit at given layer and coordinates and with given x tolerance
     */
    void findSectors( unsigned int layer, float x, float y, float xTol, float yTol, const LayerInfo& info,
                      boost::container::small_vector_base<std::pair<int, int>>& sectors );

    void computeGeometry( const DeUTDetector& utDet, std::array<LayerInfo, UTInfo::TotalLayers>& layers,
                          std::array<SectorsInStationZ, UTInfo::Stations>& sectorsZ );

  } // namespace UTDAQ

} // namespace LHCb

#endif // UTDAQHELPER_H
