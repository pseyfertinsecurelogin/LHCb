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
#ifndef KERNEL_MUONLAYOUT_H
#define KERNEL_MUONLAYOUT_H 1

// Include files
#include <iostream>
#include <vector>

#include "GaudiKernel/MsgStream.h"

/** @class MuonLayout MuonLayout.h Kernel/MuonLayout.h

   Defines a Muon station single plane logical layout. Layouts in
   all the regions of the station are the same with the scaling factor 2
   when passing from a region to a larger one. The class also implements
   various layout/tiles manipulation functions.

   @author A.Tsaregorodtsev
   @date 6 April 2001
*/

#include "Kernel/IMuonLayout.h"

// Forward declarations

namespace LHCb {
  class MuonTileID;
}

class MuonLayout final : public IMuonLayout {

public:
  /// Default constructor
  MuonLayout();
  /** Constructor taking X and Y granularities
    @param   xgrid  granularity in X
    @param   ygrid  granularity in Y
  */
  MuonLayout( unsigned int xgrid, unsigned int ygrid );
  /** Constructor taking X and Y granularities as std::pair
    @param   xygrid  granularity in X
  */
  MuonLayout( std::pair<unsigned int, unsigned int> xygrid );

  /// Accessor to MuonLayout grid corresponding to the given MuonTileID
  /// reimplemented from IMuonLayout
  [[nodiscard]] std::pair<unsigned int, unsigned int> grid( const LHCb::MuonTileID& ) const override {
    return {xGrid(), yGrid()};
  }

  /// Accessor to X granularity
  [[nodiscard]] unsigned int xGrid() const { return m_xgrid; }
  /// Accessor to Y granularity
  [[nodiscard]] unsigned int yGrid() const { return m_ygrid; }

  /** find a vector of MuonTileID's defined in terms of this MuonLayout
      which are touched by an area around a given MuonTileID defined
      in its own MuonLayout

      @param pad   :  tile defining the area covered
  */

  [[nodiscard]] std::vector<LHCb::MuonTileID> tiles( const LHCb::MuonTileID& pad ) const override;

  /** find a vector of MuonTileID's defined in terms of this MuonLayout
      which are touched by an area around a given MuonTileID defined
      in its own MuonLayout

      @param pad   : central for the search
      @param areaX : limits of the search area in X
      @param areaY : limits of the search area in Y
  */

  [[nodiscard]] std::vector<LHCb::MuonTileID> tilesInArea( const LHCb::MuonTileID& pad, int areaX,
                                                           int areaY ) const override;

  /// returns a vector of its MuonTileID's.
  /// Implementation of the IMuonLayout	interface
  [[nodiscard]] std::vector<LHCb::MuonTileID> tiles() const override;

  /** returns a vector of its MuonTileID's in a given quarter

      @param quarter : the quarter number to look into
  */
  [[nodiscard]] std::vector<LHCb::MuonTileID> tiles( int quarter ) const override;

  /** returns a vector of its MuonTileID's in a given quarter and region

      @param quarter : the quarter number to look into
      @param region : the region number to look into
  */
  [[nodiscard]] std::vector<LHCb::MuonTileID> tiles( int quarter, int region ) const override;

  /// find a tile containing the argument tile
  [[nodiscard]] LHCb::MuonTileID contains( const LHCb::MuonTileID& pad ) const override;

  /// Get tiles touched by pad defined in terms of pregion region number
  [[nodiscard]] std::vector<LHCb::MuonTileID> tilesInRegion( const LHCb::MuonTileID& pad, int pregion ) const override;

  /// find all the MuonTileID's which are neighbours of the argument tile
  [[nodiscard]] std::vector<LHCb::MuonTileID> neighbours( const LHCb::MuonTileID& pad ) const override;

  /** find all the MuonTileID's which are neighbours of the argument tile
      in the specified direction.
      @param   pad   find the neighbours of this tile
      @param   dirX  horizontal direction in which to look for neighbours
      @param   dirY  vertical direction in which to look for neighbours
  */
  [[nodiscard]] std::vector<LHCb::MuonTileID> neighbours( const LHCb::MuonTileID& pad, int dirX,
                                                          int dirY ) const override;

  /** find all the MuonTileID's which are neighbours of the argument tile
      in the specified direction.
      @param   pad   find the neighbours of this tile
      @param   dirX  horizontal direction in which to look for neighbours
      @param   dirY  vertical direction in which to look for neighbours
      @param   depth depth of the band in which to look for neighbours
  */
  [[nodiscard]] std::vector<LHCb::MuonTileID> neighbours( const LHCb::MuonTileID& pad, int dirX, int dirY,
                                                          int depth ) const override;

  /** find all the MuonTileID's which are neighbours of the argument tile
      in the specified direction. This version takes into account that
      area to look in can take two parameters to specify the depth of the
      search in cases of neighbours at the corners, i.e. LEFT-UP.
      @param   pad   find the neighbours of this tile
      @param   dirX  horizontal direction in which to look for neighbours
      @param   dirY  vertical direction in which to look for neighbours
      @param   depthX depth of the band in which to look for neighbours
      @param   depthY depth of the band in which to look for neighbours
  */
  [[nodiscard]] std::vector<LHCb::MuonTileID> neighboursInArea( const LHCb::MuonTileID& pad, int dirX, int dirY,
                                                                int depthX, int depthY ) const override;

  /// check if the given MuonTileID is valid for this layout
  [[nodiscard]] bool isValidID( const LHCb::MuonTileID& mt ) const;

  /// check if the layout itself is defined
  [[nodiscard]] bool isDefined() const;

  /// printout to std::ostream
  inline std::ostream& printOut( std::ostream& ) const;

  /// printout to MsgStream
  inline MsgStream& printOut( MsgStream& ) const;

private:
  /// find magnification factor of pads in the given region
  [[nodiscard]] int rfactor( unsigned int nr ) const { return 1 << nr; }

  /** find region for the given pad indices. The pad indices
      are given in terms of the most inner region. If the pad indices
      are not fitting into the layout, the result returned is -1
  */
  [[nodiscard]] int region( unsigned int bx, unsigned int by ) const;

private:
  unsigned int m_xgrid;
  unsigned int m_ygrid;
};

// print to std::ostream
inline std::ostream& MuonLayout::printOut( std::ostream& os ) const {
  os << "(";
  return os << xGrid() << "," << yGrid() << ")";
}

// print to MsgStream
inline MsgStream& MuonLayout::printOut( MsgStream& os ) const {
  os << "(";
  return os << xGrid() << "," << yGrid() << ")";
}

// output to MsgStream
inline MsgStream& operator<<( MsgStream& os, const MuonLayout& id ) { return id.printOut( os ); }

// output to std::ostream
inline std::ostream& operator<<( std::ostream& os, const MuonLayout& id ) { return id.printOut( os ); }

inline bool operator==( const MuonLayout& ml1, const MuonLayout& ml2 ) {
  return ml1.xGrid() == ml2.xGrid() && ml1.yGrid() == ml2.yGrid();
}
inline bool operator!=( const MuonLayout& ml1, const MuonLayout& ml2 ) { return !( ml1 == ml2 ); }
#endif // KERNEL_MUONLAYOUT_H
