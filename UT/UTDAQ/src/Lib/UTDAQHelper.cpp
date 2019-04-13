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
#include "UTDAQ/UTDAQHelper.h"
#include "Event/RawBank.h"
#include "SiDAQ/SiHeaderWord.h"
#include <cmath>
#include <limits>

std::optional<unsigned int> LHCb::UTDAQ::nbUTClusters( LHCb::span<const RawBank*> banks, unsigned int maxNbClusters ) {
  size_t nbClusters = 0;
  for ( auto& bank : banks ) {
    nbClusters += SiHeaderWord( bank->data()[0] ).nClusters();
    // cut as soon as we have too many
    if ( nbClusters > maxNbClusters ) { return {}; }
  }
  return nbClusters;
}

void LHCb::UTDAQ::findSectors( unsigned int layer, float x, float y, float xTol, float yTol,
                               const LHCb::UTDAQ::LayerInfo&                             info,
                               boost::container::small_vector_base<std::pair<int, int>>& sectors ) {
  auto localX = x - info.dxDy * y;
  // deal with sector overlaps and geometry imprecision
  xTol += 1; // mm
  auto localXmin = localX - xTol;
  auto localXmax = localX + xTol;
  int  subcolmin = std::nearbyintf( localXmin * info.invHalfSectorXSize - 0.5 ) + 2 * info.nColsPerSide;
  int  subcolmax = std::nearbyintf( localXmax * info.invHalfSectorXSize - 0.5 ) + 2 * info.nColsPerSide;
  if ( subcolmax < 0 || subcolmin >= (int)( 4 * info.nColsPerSide ) ) {
    // out of acceptance, return empty result
    return;
  }
  // on the acceptance limit
  if ( subcolmax >= (int)( 4 * info.nColsPerSide ) ) subcolmax = (int)( 4 * info.nColsPerSide ) - 1;
  if ( subcolmin < 0 ) subcolmin = 0;
  // deal with sector shifts in tilted layers and overlaps in regular ones
  yTol += ( layer == 1 || layer == 2 ) ? 8 : 1; //  mm
  auto localYmin = y - yTol;
  auto localYmax = y + yTol;
  int  subrowmin = std::nearbyintf( localYmin * info.invHalfSectorYSize - 0.5 ) + 2 * info.nRowsPerSide;
  int  subrowmax = std::nearbyintf( localYmax * info.invHalfSectorYSize - 0.5 ) + 2 * info.nRowsPerSide;
  if ( subrowmax < 0 || subrowmin >= (int)( 4 * info.nRowsPerSide ) ) {
    // out of acceptance, return empty result
    return;
  }
  // on the acceptance limit
  if ( subrowmax >= (int)( 4 * info.nRowsPerSide ) ) subrowmax = (int)( 4 * info.nRowsPerSide ) - 1;
  if ( subrowmin < 0 ) subrowmin = 0;
  for ( int subcol = subcolmin; subcol <= subcolmax; subcol++ ) {
    int region = subcol < (int)( 2 * info.nColsPerSide - 4 ) ? 1 : subcol >= (int)( 2 * info.nColsPerSide + 4 ) ? 3 : 2;
    if ( region == 1 ) {
      for ( int subrow = subrowmin; subrow <= subrowmax; subrow++ ) {
        sectors.emplace_back( 1, ( subcol / 2 ) * info.nRowsPerSide * 2 + subrow / 2 + 1 );
      }
    } else if ( region == 2 ) {
      int subcolInReg = subcol - 2 * info.nColsPerSide + 4;
      for ( int subrow = subrowmin; subrow <= subrowmax; subrow++ ) {
        if ( subrow < (int)( 2 * info.nRowsPerSide - 4 ) || subrow >= (int)( 2 * info.nRowsPerSide + 4 ) ) {
          // no in central Region
          sectors.emplace_back( 2, mapSectorToSector[( subcolInReg / 2 ) * 14 + ( subrow / 2 )] );
        } else {
          // central region
          sectors.emplace_back(
              2, mapQuarterSectorToSectorCentralRegion[subcolInReg * 8 + subrow - 2 * info.nRowsPerSide + 4] );
        }
      }
    } else {
      for ( int subrow = subrowmin; subrow <= subrowmax; subrow++ ) {
        sectors.emplace_back( 3, ( subcol / 2 - info.nColsPerSide - 2 ) * info.nRowsPerSide * 2 + subrow / 2 + 1 );
      }
    }
  }
}

void LHCb::UTDAQ::computeGeometry( const DeUTDetector& utDet, std::array<LHCb::UTDAQ::LayerInfo, 4>& layers,
                                   std::array<SectorsInStationZ, 2>& sectorsZ ) {
  for ( int iStation = 0; iStation < 2; ++iStation ) {
    for ( int iLayer = 0; iLayer < 2; ++iLayer ) {
      // get layer
      unsigned int layerIndex = 2 * iStation + iLayer;
      DeUTLayer*   layer      = utDet.layers()[layerIndex];
      // get the z position of the layer and store it
      layers[layerIndex].z = layer->sectors()[0]->sensors()[0]->plane().ProjectOntoPlane( {0, 0, 0} ).z();
      // get the layer size and sector sizes. Go through all sectors, do not rely on ordering
      float        YFirstRow         = std::numeric_limits<float>::max();
      float        YLastRow          = std::numeric_limits<float>::lowest();
      float        smallestXLastCol  = std::numeric_limits<float>::max();
      float        smallestXFirstcol = std::numeric_limits<float>::max();
      float        biggestXFirstCol  = std::numeric_limits<float>::lowest();
      unsigned int biggestColumn     = 0;
      unsigned int smallestColumn    = 999;
      unsigned int topMostRow        = 0;
      unsigned int bottomMostRow     = 999;
      // First pass
      // deal with col/row numbers, we need a UTSector for that
      // Note that rows/cols cannot be relied on the middle columns/rows
      // as they are not anymore "rows/cols" but "number of sectors
      // in the column/row". So we use only first column and row
      for ( const auto& sector : layer->sectors() ) {
        DeUTSector* utSector = dynamic_cast<DeUTSector*>( sector );
        auto        column   = utSector->column();
        auto        row      = utSector->row();
        smallestColumn       = std::min( smallestColumn, column );
        if ( utSector->column() == smallestColumn ) {
          topMostRow    = std::max( topMostRow, row );
          bottomMostRow = std::min( bottomMostRow, row );
        }
        if ( utSector->row() == bottomMostRow ) { biggestColumn = std::max( biggestColumn, column ); }
      }
      // Second pass
      // find x and y values in the corners to deduce the geometry of the layer
      for ( const auto& sector : layer->sectors() ) {
        // deal with x,y coordinates. Remember the corner coordinates
        DeUTSector* utSector = dynamic_cast<DeUTSector*>( sector );
        auto        column   = utSector->column();
        auto        row      = utSector->row();
        auto        center   = sector->geometry()->toGlobal( Gaudi::XYZPoint{0, 0, 0} );
        if ( column == smallestColumn ) {
          if ( row == bottomMostRow ) {
            smallestXFirstcol = center.x();
            YFirstRow         = center.y();
          } else if ( row == topMostRow ) {
            biggestXFirstCol = center.x();
            YLastRow         = center.y();
          }
        }
        if ( column == biggestColumn && row == bottomMostRow ) { smallestXLastCol = center.x(); }
        sectorsZ[iStation][iLayer][sector->elementID().detRegion() - 1][sector->elementID().sector() - 1] = center.z();
      }
      // gather all information into the corresponding LayerInfo object
      auto ncols                            = biggestColumn - smallestColumn + 1;
      auto nrows                            = topMostRow - bottomMostRow + 1;
      layers[layerIndex].nColsPerSide       = ncols / 2;
      layers[layerIndex].nRowsPerSide       = nrows / 2;
      layers[layerIndex].invHalfSectorYSize = 2 * ( nrows - 1 ) / ( YLastRow - YFirstRow );
      layers[layerIndex].invHalfSectorXSize = 2 * ( ncols - 1 ) / ( smallestXLastCol - smallestXFirstcol );
      layers[layerIndex].dxDy               = ( biggestXFirstCol - smallestXFirstcol ) / ( YLastRow - YFirstRow );
    }
  }
}
