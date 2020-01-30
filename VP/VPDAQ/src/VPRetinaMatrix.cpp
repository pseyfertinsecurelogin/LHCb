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
// LHCb
#include "Event/State.h"
// Local
#include "VPRetinaMatrix.h"

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
VPRetinaMatrix::VPRetinaMatrix( uint32_t SP_row, uint32_t SP_col, uint8_t SP_pixel )
    : Coordinate_Retina_row( SP_row - 1 ), Coordinate_Retina_col( SP_col - 2 ) {
  // record SP pixels.
  for ( uint32_t shift = 0; shift < 8; ++shift ) {
    const uint8_t pixel = SP_pixel & 1;
    if ( pixel ) {
      const uint32_t row     = 4 + shift % 4;
      const uint32_t col     = 4 + shift / 4;
      Pixel_Matrix[row][col] = pixel;
    }
    SP_pixel = SP_pixel >> 1;
    if ( 0 == SP_pixel ) break;
  }

  // record which SP is added
  SPixel_Matrix[SP_row - Coordinate_Retina_row][SP_col - Coordinate_Retina_col] = 1;
}

//=============================================================================
// Check if a SP coordinate are inside the Retina
//=============================================================================
bool VPRetinaMatrix::IsInRetina( uint32_t SP_row, uint32_t SP_col ) const {
  return Coordinate_Retina_row <= (int32_t)SP_row && (int32_t)SP_row < Coordinate_Retina_row + 3 &&
         Coordinate_Retina_col <= (int32_t)SP_col && (int32_t)SP_col < Coordinate_Retina_col + 5;
}

//=============================================================================
// Add a SP to the Retina
//=============================================================================
VPRetinaMatrix& VPRetinaMatrix::AddSP( uint32_t SP_row, uint32_t SP_col, uint8_t SP_pixel ) {
  // record which SP is added
  SPixel_Matrix[SP_row - Coordinate_Retina_row][SP_col - Coordinate_Retina_col] = 1;

  // record SP pixels.
  for ( uint32_t shift = 0; shift < 8; ++shift ) {
    const uint8_t pixel = SP_pixel & 1;
    if ( pixel ) {
      const int32_t row      = ( SP_row - Coordinate_Retina_row ) * 4 + shift % 4;
      const int32_t col      = ( SP_col - Coordinate_Retina_col ) * 2 + shift / 4;
      Pixel_Matrix[row][col] = pixel;
    }
    SP_pixel = SP_pixel >> 1;
    if ( 0 == SP_pixel ) break;
  }
  return *this;
}

//=============================================================================
// Search cluster
//=============================================================================

std::vector<uint32_t> VPRetinaMatrix::SearchCluster() const {
  std::vector<uint32_t> RetinaCluster;
  RetinaCluster.reserve( 63 );

  for ( unsigned int iX = 1; iX < 10 - 2; ++iX )
    for ( unsigned int iY = 1; iY < 12 - 2; ++iY ) {
      if ( ( ( Pixel_Matrix[iY][iX] == 1 ) |
             ( ( ( Pixel_Matrix[iY + 1][iX] == 1 ) ) && ( ( Pixel_Matrix[iY][iX + 1] == 1 ) ) ) ) &&
           Pixel_Matrix[iY][iX - 1] == 0 && Pixel_Matrix[iY + 1][iX - 1] == 0 && Pixel_Matrix[iY - 1][iX] == 0 &&
           Pixel_Matrix[iY - 1][iX + 1] == 0 && Pixel_Matrix[iY - 1][iX - 1] == 0 ) {

        uint32_t shift_col = 0;
        uint32_t shift_row = 0;
        uint32_t n         = 0;
        for ( unsigned int iiX = 0; iiX < 3; ++iiX )
          for ( unsigned int iiY = 0; iiY < 3; ++iiY )
            if ( Pixel_Matrix[iY + iiY][iX + iiX] == 1 ) {
              shift_col += iiX;
              shift_row += iiY;
              n++;
            }
        const uint64_t cX = ( ( Coordinate_Retina_col * 2 + iX ) << 3 ) + ( uint64_t )( ( shift_col << 3 ) / n );
        const uint64_t cY = ( ( Coordinate_Retina_row * 4 + iY ) << 3 ) + ( uint64_t )( ( shift_row << 3 ) / n );

        RetinaCluster.push_back( cX << 11 | cY );
      }
    }
  return RetinaCluster;
}
