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
// Include files
#include <iostream>
#include <algorithm>
#include "MuonKernel/MuonStationLayout.h"
#include "Kernel/MuonTileID.h"

//------------------------------------------------------------------------------
//
/// Implementation of class :  MuonStationLayout
//
/// Author: A.Tsaregorodtsev
//
//------------------------------------------------------------------------------

MuonStationLayout::MuonStationLayout(const MuonLayout& lq1,
                                     const MuonLayout& lq2,
                                     const MuonLayout& lq3,
                                     const MuonLayout& lq4) 
: m_layouts{ lq1, lq2, lq3, lq4 }
{ }


std::vector<LHCb::MuonTileID> 
MuonStationLayout::tiles(const LHCb::MuonTileID& pad) const {
  return tilesInArea(pad,0,0);
}

std::vector<LHCb::MuonTileID> MuonStationLayout::tilesInArea(const LHCb::MuonTileID& pad, 
					int areaX, int areaY) const {
					
  unsigned int reg = pad.region();
  
  std::vector<LHCb::MuonTileID> result;
  
  for(unsigned int i = 0; i<4; i++) {
  
    int factorX = 1;
    int factorY = 1;
    if(i != reg) {
      factorX = m_layouts[i].xGrid()/m_layouts[reg].xGrid();
      factorY = m_layouts[i].yGrid()/m_layouts[reg].yGrid();
    }
  
    auto vtm=m_layouts[i].tilesInArea(pad,areaX*factorX,areaY*factorY);
    std::copy_if( vtm.begin(), vtm.end(), std::back_inserter(result),
                  [i](const LHCb::MuonTileID& x) { return x.region()==i; } );
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonStationLayout::tiles() const {
  std::vector<LHCb::MuonTileID> result;
  for (int i=0;i<16;++i) {
    const int ir = i/4; const int iq = i%4;
    auto tmp=m_layouts[ir].tiles(iq,ir);
    result.insert(result.end(),tmp.begin(),tmp.end());
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonStationLayout::tiles(int iq) const {
					
  std::vector<LHCb::MuonTileID> result;
  for (int ir = 0; ir<4; ir++) {
    auto tmp=m_layouts[ir].tiles(iq,ir);
    result.insert(result.end(),tmp.begin(),tmp.end());
  }
  return result;
}

std::vector<LHCb::MuonTileID> MuonStationLayout::tiles(int iq, int ir) const {
					
  return m_layouts[ir].tiles(iq,ir);
}

std::vector<LHCb::MuonTileID> MuonStationLayout::tilesInRegion(const LHCb::MuonTileID& pad, 
                                                int pregion) const{
    
  int reg = pad.region();
  return m_layouts[reg].tilesInRegion(pad,pregion);
  
}  

std::vector<LHCb::MuonTileID> 
MuonStationLayout::neighbours(const LHCb::MuonTileID& pad) const {

  std::vector<LHCb::MuonTileID> result;			      
  for ( unsigned int ireg = 0; ireg < 4; ++ireg ) {
    auto vreg = m_layouts[ireg].neighbours(pad);
    std::copy_if( vreg.begin(), vreg.end(), std::back_inserter(result),
                  [ireg](const LHCb::MuonTileID& x) { return x.region()==ireg; } );
  }
  return result;
}

std::vector<LHCb::MuonTileID> 
MuonStationLayout::neighbours(const LHCb::MuonTileID& pad,
                              int dirX, int dirY) const {
  
//  This function returns all the LHCb::MuonTileID's which are neighbours
//  of the given pad in the direction indicated by dirX and dirY and 
//  defined in terms of this layout. 

  unsigned int nreg = pad.region();
  auto vtm = neighbours(pad,dirX,dirY,1);
  // if no neigbours at all
  if(vtm.empty()) return vtm;
  // if the neigbours are all in the same region or larger region
  if(vtm[0].region() >= nreg) return vtm;
  // if there is only one neighbour
  if(vtm.size() == 1) return vtm;
  
  // We have got to the smaller region - make use of the corresponding
  // function in this region's MuonLayout
  nreg = vtm[0].region();
  return m_layouts[nreg].neighbours(pad,dirX,dirY);			      
}			      

std::vector<LHCb::MuonTileID> 
MuonStationLayout::neighbours(const LHCb::MuonTileID& pad,
                              int dirX, int dirY, int depth) const {
			      
  std::vector<LHCb::MuonTileID> result;			      
  for ( unsigned int ireg = 0; ireg < 4; ireg++ ) {
    auto vreg = m_layouts[ireg].neighbours(pad,dirX,dirY,depth);
    std::copy_if( vreg.begin(), vreg.end(), std::back_inserter(result),
                  [ireg](const LHCb::MuonTileID& x) { return x.region()==ireg; } );
  }  		      
  return result;
}

std::vector<LHCb::MuonTileID> 
MuonStationLayout::neighboursInArea(const LHCb::MuonTileID& pad,
                        	    int dirX, int dirY, int depthX, int depthY) const {
				    
  std::vector<LHCb::MuonTileID> result;			      
  for ( unsigned int ireg = 0; ireg < 4; ireg++ ) {
    auto vreg = m_layouts[ireg].neighboursInArea(pad,dirX,dirY,depthX,depthY);
    std::copy_if(vreg.begin(),vreg.end(),std::back_inserter(result),
                 [ireg](const LHCb::MuonTileID& id) { return id.region()==ireg; } );
  }  		      
  return result;
}				    

bool MuonStationLayout::isValidID(const LHCb::MuonTileID& pad) const {

  return m_layouts[pad.region()].isValidID(pad);
}

LHCb::MuonTileID MuonStationLayout::contains(const LHCb::MuonTileID& pad) const {
  // It is responsibility of the user to assure that the pad
  // layout is finer than the containing layout
  return m_layouts[pad.region()].contains(pad);
} 
