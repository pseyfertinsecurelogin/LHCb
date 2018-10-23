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
#ifndef VPRETINAMATRIX_H
#define VPRETINAMATRIX_H 1

// LHCb
#include "Event/StateVector.h"
// Local

#include <array>
#include <vector>

namespace LHCb
{
  class State;
}

/** @class VPRetinaMatrix VPRetinaMatrix.h
 *  Class to store SuperPixel in a RETINA like architecture
 *  and find Cluster
 *
 *  @author Federico Lazzari
 *  @date   2015-04-18
 */

class VPRetinaMatrix final
{
public:

  /// Standard constructor
  VPRetinaMatrix(uint32_t SP_row, uint32_t SP_col, uint8_t SP_pixel);

  /// Check if a SP coordinate are inside the Retina
  bool IsInRetina(uint32_t SP_row, uint32_t SP_col) const;
  
  /// Add a SP to the Retina
  void AddSP(uint32_t SP_row, uint32_t SP_col, uint8_t SP_pixel);
  
  /// Search cluster
  std::vector<uint32_t> SearchCluster() const;
  
private:

  // Coordinate of the lower left SP
  int32_t Coordinate_Retina_row;
  int32_t Coordinate_Retina_col;
  
  // Matrix of pixel
  std::array<std::array<unsigned char,5>,3> SPixel_Matrix{};
  std::array<std::array<unsigned char,10>,12> Pixel_Matrix{};
};

#endif // VPRetinaMatrix_H
