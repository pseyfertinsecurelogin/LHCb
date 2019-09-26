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

    // -- For the moment, this is assigned here and overwritten in "computeGeometry" in case a geometry
    // -- version with a "wrong" sector ordering is used
    extern std::array<int, 64> mapQuarterSectorToSectorCentralRegion;

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
