// Include files
#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include "Kernel/MuonBase.h"
#include "Kernel/MuonLayout.h"
#include "Kernel/MuonTileID.h"

//------------------------------------------------------------------------------
//
/// Implementation of class :  MuonLayout
//
/// Author: A.Tsaregorodtsev
//
//------------------------------------------------------------------------------

MuonLayout::MuonLayout() {
    m_xgrid = 0;
    m_ygrid = 0;
}

MuonLayout::MuonLayout(unsigned int x, unsigned int y) {
    m_xgrid = x;
    m_ygrid = y;
}

MuonLayout::MuonLayout(std::pair<unsigned int, unsigned int> grid) {
    m_xgrid = grid.first;
    m_ygrid = grid.second;
}

int MuonLayout::region(unsigned int ix, unsigned int iy) const {
  int nrx = ( ix/m_xgrid  < 1 ?  -1
            : ix/m_xgrid == 1 ?   0
            : ix/m_xgrid <= 3 ?   1
            : ix/m_xgrid <= 7 ?   2
            :                     3 );
  int nry = ( iy/m_ygrid  < 1 ? -1
            : iy/m_ygrid == 1 ?  0
            : iy/m_ygrid <= 3 ?  1
            : iy/m_ygrid <= 7 ?  2
            :                    3 );
  return std::max(nrx,nry);
}

std::vector<LHCb::MuonTileID>
MuonLayout::tiles(const LHCb::MuonTileID& pad) const {
  return tilesInArea(pad,0,0);
}

std::vector<LHCb::MuonTileID>
MuonLayout::tilesInArea(const LHCb::MuonTileID& pad, int areaX, int areaY) const
{
//  This function returns all the MuonTileID's touched by the "pad"
//  defined with the "playout" MuonLayout +- areaX and +- areaY.
//  The areaX and areaY are defined in terms of this layout.

  MuonLayout playout = pad.layout();
  std::vector<LHCb::MuonTileID> vmt;

  if( ! isDefined() ) return vmt;

  int quarter = pad.quarter();
  int nreg = pad.region();
  int sta = pad.station();

   //std::cout << quarter << " " << nreg << " " << rfactor(nreg) << std::endl;
   //std::cout << m_xgrid << m_ygrid << endl;
   //cout << playout.xGrid() << playout.yGrid() << endl;

  // the finest grid of the two layouts
  int mxgrid = std::max(m_xgrid, playout.xGrid() );
  int mygrid = std::max(m_ygrid, playout.yGrid() );

//   std::cout << mxgrid << mygrid << std::endl;

  MuonLayout fineGrid(mxgrid,mygrid);

  int xratio = m_xgrid/playout.xGrid();
  int yratio = m_ygrid/playout.yGrid();

  if (xratio == 0) xratio = 1;
  if (yratio == 0) yratio = 1;

  // input pad area in terms of the finest grid
//   int maxX = (pad.nX()+areaX+1)*playout.rfactor(nreg)*xratio-1;
//   int maxY = (pad.nY()+areaY+1)*playout.rfactor(nreg)*yratio-1;
//   int minX = maxX - playout.rfactor(nreg)*xratio*(2*areaX+1) + 1;
//   int minY = maxY - playout.rfactor(nreg)*yratio*(2*areaY+1) + 1;
  // input pad area in terms of the finest grid and smallest region
  int maxX = (pad.nX()+1)*rfactor(nreg)*xratio+areaX*rfactor(nreg)-1;
  int maxY = (pad.nY()+1)*rfactor(nreg)*yratio+areaY*rfactor(nreg)-1;
  int minX = maxX - rfactor(nreg)*xratio - 2*areaX*rfactor(nreg) + 1;
  int minY = maxY - rfactor(nreg)*yratio - 2*areaY*rfactor(nreg) + 1;

  if (minX<0) minX=0;
  if (minY<0) minY=0;

  // Which tiles are hit ?
  xratio = mxgrid/m_xgrid;
  yratio = mygrid/m_ygrid;

  int ix = minX;
  int iy = minY;
  bool regleap = false;
  int nr = 0;
  int nrx = 0;

  while ( ix <= maxX ) {
    nrx = fineGrid.region(ix,iy);
    if( nrx == -1 ) nrx = 0;
    if( nrx > 3 ) break;
    while ( iy <= maxY ) {
      // which region
      nr = fineGrid.region(ix,iy);
      if( nr == -1 ) nr = 0;
      if( nr > 3 ) break;
      if(nr == nrx || !regleap) {
        unsigned int newx = ix/rfactor(nr)/xratio;
        unsigned int newy = iy/rfactor(nr)/yratio;
        if(newy < 2*yGrid() && newx < 2*xGrid()) {
          LHCb::MuonTileID newtile(sta,*this,nr,quarter,newx,newy);
          if(newtile.isValid()) vmt.push_back(newtile);
        }
      }
      iy += rfactor(nr)*yratio;
    }
    if(regleap) {
      regleap = false;
    } else if (nr != nrx) {
      regleap = true;
    }
    iy = minY;
    ix += rfactor(nrx)*xratio;
  }
  return vmt;
}

std::vector<LHCb::MuonTileID> MuonLayout::tiles() const {

  std::vector<LHCb::MuonTileID> vmt;
  if( ! isDefined() ) return vmt;
  for(int iq = 0; iq <4; iq++) {
    for(int ir = 0; ir <4; ir++) {
      auto vmtr = tiles(iq,ir);
      vmt.insert(vmt.end(),vmtr.begin(),vmtr.end());
    }
  }
  return vmt;

}

std::vector<LHCb::MuonTileID> MuonLayout::tiles(int iq) const {

  std::vector<LHCb::MuonTileID> vmt;
  if( !isDefined() ) return vmt;

  for(int ir = 0; ir <4; ir++) {
    auto vmtr = tiles(iq,ir);
    vmt.insert(vmt.end(),vmtr.begin(),vmtr.end());
  }
  return vmt;

}

std::vector<LHCb::MuonTileID> MuonLayout::tiles(int iq, int ir) const {

  std::vector<LHCb::MuonTileID> vmt;
  if( ! isDefined() ) return vmt;

  vmt.reserve(xGrid()*yGrid()*3);

  for(unsigned ix = 0; ix < 2*xGrid(); ix++) {
    for(unsigned iy = yGrid(); iy < 2*yGrid(); iy++) {
      vmt.emplace_back(0,*this,ir,iq,ix,iy);
    }
  }
  for(unsigned ix = xGrid(); ix < 2*xGrid(); ix++) {
    for(unsigned iy = 0; iy < yGrid(); iy++) {
      vmt.emplace_back(0,*this,ir,iq,ix,iy);
    }
  }
  return vmt;
}

std::vector<LHCb::MuonTileID>
MuonLayout::tilesInRegion(const LHCb::MuonTileID& pad, int pregion) const{

  if( ! isDefined() ) return {};

  int nr = pad.region();
  auto vmt = tiles(pad);
  if(nr == pregion) return vmt;

  int nq = pad.quarter();
  int sta = pad.station();

  // Bring the pads in vmt to the pregion definition

  if(nr<pregion) {
    // transform 'in situ' and return
    int factor = rfactor(pregion)/rfactor(nr);
    std::transform(vmt.begin(), vmt.end(), vmt.begin(),
                   [&](const LHCb::MuonTileID& id) {
                       int newX = id.nX()/factor;
                       int newY = id.nY()/factor;
                       return LHCb::MuonTileID{ sta,*this,pregion,nq,newX,newY };
                   } );
    return vmt;
  }

  // expand out...
  std::vector<LHCb::MuonTileID> nvmt;
  int factor = rfactor(nr)/rfactor(pregion);
  nvmt.reserve(factor*factor*vmt.size());
  for (const auto& ivmt : vmt ) {
    int minX = ivmt.nX()*factor;
    int minY = ivmt.nY()*factor;
    for(int ix=0; ix<factor; ix++) for(int iy=0; iy<factor; iy++) {
        nvmt.emplace_back(sta,*this,pregion,nq,minX+ix,minY+iy);
    }
  }
  return nvmt;
}

std::vector<LHCb::MuonTileID>
MuonLayout::neighbours(const LHCb::MuonTileID& pad) const {

  // This function returns all the neighbours within unity depth
  std::vector<LHCb::MuonTileID> result;
  std::vector<LHCb::MuonTileID> vmt;
  vmt = neighbours(pad,MuonBase::RIGHT,MuonBase::DOWN);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::RIGHT,MuonBase::CENTER);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::RIGHT,MuonBase::UP);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::LEFT,MuonBase::DOWN);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::LEFT,MuonBase::CENTER);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::LEFT,MuonBase::UP);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::CENTER,MuonBase::UP);
  result.insert(result.end(),vmt.begin(),vmt.end());
  vmt = neighbours(pad,MuonBase::CENTER,MuonBase::DOWN);
  result.insert(result.end(),vmt.begin(),vmt.end());
  return result;
}

std::vector<LHCb::MuonTileID>
MuonLayout::neighbours(const LHCb::MuonTileID& pad, int dirX, int dirY ) const {

//  This function returns all the MuonTileID's which are neighbours
//  of the given pad in the direction indicated by dirX and dirY and
//  defined in terms of this layout.

  unsigned int nreg = pad.region();
  std::vector<LHCb::MuonTileID> vtm = neighbours(pad,dirX,dirY,1);
  // if no neigbours at all
  if(vtm.empty()) return vtm;
  // if the neigbours are all in the same region or the neighboring
  // region is larger than the pad's one
  if(vtm[0].region() >= nreg) return vtm;
  // if there is only one neighbour
  if(vtm.size() == 1) return vtm;

  // We have got to the smaller region
  nreg = vtm[0].region();
  // Find the limits of the returned pad's area
  unsigned int minx=9999;
  unsigned int maxx=0;
  unsigned int miny=9999;
  unsigned int maxy=0;
  for ( const auto& ivtm : vtm ) {
    if( ivtm.nY() < miny ) miny = ivtm.nY();
    if( ivtm.nY() > maxy ) maxy = ivtm.nY();
    if( ivtm.nX() < minx ) minx = ivtm.nX();
    if( ivtm.nX() > maxx ) maxx = ivtm.nX();
  }

  // perform the check in the X direction first
  auto end = ( dirX == MuonBase::LEFT  ? std::remove_if( vtm.begin(), vtm.end(),
                                                         [&](const LHCb::MuonTileID& id) { return id.nX() != maxx; } )
             : dirX == MuonBase::RIGHT ? std::remove_if( vtm.begin(), vtm.end(),
                                                         [&](const LHCb::MuonTileID& id) { return id.nX() != minx; } )
             : vtm.end() );

  // check in the Y direction with the pads that are left
  end = (  dirY == MuonBase::DOWN ? std::remove_if( vtm.begin(), end,
                                                    [&](const LHCb::MuonTileID& id) { return id.nY() != maxy; } )
        :  dirY == MuonBase::UP   ? std::remove_if( vtm.begin(), end,
                                                    [&](const LHCb::MuonTileID& id) { return id.nY() != miny; } )
        : end );

  vtm.erase(end, vtm.end());
  return vtm;
}

std::vector<LHCb::MuonTileID>
MuonLayout::neighbours(const LHCb::MuonTileID& pad, int dirX, int dirY,
					                                          int depth) const {

//  This function returns all the MuonTileID's touched by the area
//  situated in the direction indicated by dirX and dirY and defined
//  in terms of this layout. The depth of the area is defined by
//  the depth parameter

  return neighboursInArea(pad,dirX,dirY,depth,depth);

}

//========================================================================
std::vector<LHCb::MuonTileID>
MuonLayout::neighboursInArea(const LHCb::MuonTileID& pad, int dirX, int dirY,
						                                  int depthX, int depthY) const {

//  This function returns all the MuonTileID's touched by the area
//  situated in the direction indicated by dirX and dirY and defined
//  in terms of this layout. The depth of the area is defined by
//  the depthX and depthY parameters. It is identical to the neighbours()
//  apart from the corner neighbours.


  MuonLayout playout = pad.layout();
  std::vector<LHCb::MuonTileID> vmt;

  if ( ! isDefined() ) return vmt;

  int quarter = pad.quarter();
  int nreg    = pad.region();
  int sta     = pad.station();

  // the finest grid of the two layouts
  int mxgrid = std::max(m_xgrid, playout.xGrid() );
  int mygrid = std::max(m_ygrid, playout.yGrid() );

  // cout << mxgrid << mygrid << endl;

  MuonLayout fineGrid(mxgrid,mygrid);

  int xratio = m_xgrid/playout.xGrid();
  int yratio = m_ygrid/playout.yGrid();

  xratio = (xratio == 0) ? 1 : xratio ;
  yratio = (yratio == 0) ? 1 : yratio ;

  // input pad area in terms of the finest grid and smallest region
  // depending on the direction of the search

  int maxX,maxY,minX,minY;

  if(dirX == MuonBase::RIGHT) {
    maxX = (pad.nX()+1)*rfactor(nreg)*xratio+depthX*rfactor(nreg)-1;
    minX = (pad.nX()+1)*rfactor(nreg)*xratio;
  } else if (dirX == MuonBase::LEFT) {
    maxX = pad.nX()*rfactor(nreg)*xratio-1;
    minX = pad.nX()*rfactor(nreg)*xratio - depthX*rfactor(nreg) ;
  } else {
    maxX = (pad.nX()+1)*rfactor(nreg)*xratio-1;
    minX = (pad.nX())*rfactor(nreg)*xratio;
  }

  if(dirY == MuonBase::UP) {
    maxY = (pad.nY()+1)*rfactor(nreg)*yratio+depthY*rfactor(nreg)-1;
    minY = (pad.nY()+1)*rfactor(nreg)*yratio;
  } else if (dirY == MuonBase::DOWN){
    maxY = pad.nY()*rfactor(nreg)*yratio-1;
    minY = pad.nY()*rfactor(nreg)*yratio - depthY*rfactor(nreg) ;
  } else {
    maxY = (pad.nY()+1)*rfactor(nreg)*yratio-1;
    minY = (pad.nY())*rfactor(nreg)*yratio;
  }

  minX = std::max(0,minX);
  minY = std::max(0,minY);

  //cout << minX << " " << minY << " " << maxX << " " << maxY << " " << endl;

  // Which tiles are hit ?

  xratio = mxgrid/m_xgrid;
  yratio = mygrid/m_ygrid;

  int ix = minX;
  int iy = minY;
  bool regleap = false;
  int nr = 0;
  int nrx = 0;

  while ( ix <= maxX ) {
    nrx = fineGrid.region(ix,iy);
    if( nrx == -1 ) nrx = 0;
    while ( iy <= maxY ) {
      // which region
      nr = fineGrid.region(ix,iy);
      if( nr == -1 ) nr = 0;
      if(nr == nrx || !regleap) {
        unsigned int newx = ix/rfactor(nr)/xratio;
        unsigned int newy = iy/rfactor(nr)/yratio;
        // New tile should not come out of its region
        if(newy < 2*yGrid() && newx < 2*xGrid()) {
          LHCb::MuonTileID newtile(sta,*this,nr,quarter,newx,newy);
          if (newtile.isValid()) {
            vmt.push_back(newtile);
          }
        }
      }
      iy += rfactor(nr)*yratio;
    }
    if(regleap) {
      regleap = false;
    } else if (nr != nrx) {
      regleap = true;
    }
    iy = minY;
    ix += rfactor(nrx)*xratio;
  }
  return vmt;
}

//==========================================================
bool MuonLayout::isValidID(const LHCb::MuonTileID& mt) const {

  if(mt.layout() != *this) return false;
  int nx = mt.nX();
  int ny = mt.nY();
  int xg = xGrid();
  int yg = yGrid();
  return (nx >=  0 && nx < 2*xg && ny >= yg && ny < 2*yg) ||
         (nx >= xg && nx < 2*xg && ny >= 0  && ny <   yg ) ;
}

LHCb::MuonTileID MuonLayout::contains(const LHCb::MuonTileID& pad) const {
  // It is responsibility of the user to assure that the pad
  // layout is finer than the containing layout
  std::vector<LHCb::MuonTileID> mtiles = tiles(pad);
  return mtiles.empty() ? LHCb::MuonTileID() :  mtiles[0];
}

bool MuonLayout::isDefined() const {
  return xGrid() > 0 && yGrid() > 0;
}
