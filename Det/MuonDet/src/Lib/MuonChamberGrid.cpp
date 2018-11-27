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
// Include files

// local
#include "GaudiKernel/IToolSvc.h"
#include "Kernel/STLExtensions.h"

#include "MuonDet/MuonChamberGrid.h"
#include "MuonDet/MuonFrontEndID.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <cassert>
#include "boost/lexical_cast.hpp"
#include "boost/utility/string_ref.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : MuonChamberGrid
//
// 2004-01-07 : Alessia(o) Sat(r)ta(i)
//-----------------------------------------------------------------------------

//=============================================================================
StatusCode MuonChamberGrid::initialize(){

  boost::string_ref n = this->name();
  assert( n.starts_with( "/G" ) );
  n.remove_prefix(2);
  m_number_of_grid = boost::lexical_cast<int>( n.data(), n.size() );

  m_x_pad_rdout1     = param< std::vector<double> >("xrd1");
  m_y_pad_rdout1     = param< std::vector<double> >("yrd1");
  m_x_pad_rdout2     = param< std::vector<double> >("xrd2");
  m_y_pad_rdout2     = param< std::vector<double> >("yrd2");
  m_readoutType      = param< std::vector<int> >("grrd");
  m_mapType          = param< std::vector<int> >("mapT");

  return StatusCode::SUCCESS;
}

std::vector< std::pair< MuonFrontEndID,std::array<float,4> > >
MuonChamberGrid::listOfPhysChannels(double x_enter,double y_enter,
                                    double x_exit,double y_exit) {

  using GaudiUtils::operator<<; // for streaming std::vector

  std::vector< std::pair< MuonFrontEndID,std::array<float,4> > > keepTemporary;
  double ParallelCutOff=0.0001;

  bool debug = false;
  bool parallelFlag = false;

  double slopeY(0),intercept(0);
  if(fabs(x_exit-x_enter)>ParallelCutOff){
    slopeY =(y_exit - y_enter)/(x_exit - x_enter);
    intercept = y_exit - slopeY * x_exit ;
  } else {
    parallelFlag = true;
  }

  //The choice on the readout numbers is taken
  //looking for dummy grids
  int TstRead =  m_x_pad_rdout2.size();
  int RdN = 1;
  if(TstRead > 1) RdN = 2;

  if(debug) std::cout<< "Returning List of Phys Channels for grid G"
		     << m_number_of_grid<<" . A Details. Vectors rd1x: "
		     << m_x_pad_rdout1<<" rd1y: "
		     << m_y_pad_rdout1<<" rd2x: "
		     << m_x_pad_rdout2<<" rd2y: "
		     << m_y_pad_rdout2<<std::endl;

  //Questa parte sui readout va vista bene
  for(int iRd=0;iRd<RdN;iRd++){

    int PhNx,PhNy;

    //Readout choice
    const std::vector<double>& x_rdout = ( iRd ? m_x_pad_rdout2 : m_x_pad_rdout1 );
    const std::vector<double>& y_rdout = ( iRd ? m_y_pad_rdout2 : m_y_pad_rdout1 );

    //Physical channels settings related to the grid/readout
    PhNx = x_rdout.size();
    PhNy = y_rdout.size();

    unsigned int nxChaEntry, nxChaExit;
    int tmpNxChaEntry(0), tmpNxChaExit(20), inxLo, inyLo;

    if(debug) std::cout<< "Looping on readout n. "<<iRd<< " whith vct dim nX: "
                       << PhNx<<" nY: "
                       << PhNy<<". Entry/Exit of hit. x_e: "
                       << x_enter<<" y_e: "
                       << y_enter<<" x_x:  "
                       << x_exit<<" y_x: "
                       << y_exit<<" "<<std::endl;

    double tmpXLenght(0);
    for(inxLo = 0; inxLo < PhNx; inxLo++) {
      if(x_enter - tmpXLenght > ParallelCutOff) {
        tmpXLenght += x_rdout[inxLo];
        tmpNxChaEntry =  inxLo;
        //	tmpNxChaEntry =  inxLo+1;
      } else {
        break;
      }
    }
    tmpXLenght = 0;
    for(inxLo = 0; inxLo < PhNx; inxLo++) {
      if(x_exit - tmpXLenght > ParallelCutOff) {
        tmpXLenght += x_rdout[inxLo];
        tmpNxChaExit =  inxLo;
        //	tmpNxChaExit =  inxLo+1;
      } else {
        break;
      }
    }
    double xstart,xstop;

    if(debug) std::cout<< "************************************* "<<std::endl;

    if(debug) std::cout<< "Returning List of Phys Channels. TmpCha_entry: "
                       << tmpNxChaEntry<<" TmpCha_exit: "
                       << tmpNxChaExit<<std::endl;

    if(tmpNxChaEntry<=tmpNxChaExit){
      // normal order
      if( tmpNxChaEntry<0)tmpNxChaEntry=0;
      if( tmpNxChaExit>=(int)PhNx) tmpNxChaExit=(int)PhNx-1 ;

      xstart =	x_enter;      xstop  =	x_exit;
      nxChaEntry = (unsigned int) tmpNxChaEntry;
      nxChaExit  = (unsigned int) tmpNxChaExit;

    } else {
      // inverse order
      if( tmpNxChaExit<0)tmpNxChaExit=0;
      if( tmpNxChaEntry>=(int)PhNx) tmpNxChaEntry=(int)PhNx-1 ;
      xstop  =	x_enter;      xstart =	x_exit;
      nxChaEntry = (unsigned int) tmpNxChaExit;
      nxChaExit  = (unsigned int) tmpNxChaEntry;
    }

    unsigned int nyBegin ;    unsigned int nyEnd ;
    double xBegin(0.),xEnd(0.);       double yBegin(0.), yEnd(0.);

    if(debug) std::cout<< "Returning List of Phys Channels. Cha_entry: "
                       << nxChaEntry<<" Cha_exit: "
                       << nxChaExit<<std::endl;

    for (unsigned int Xloop=nxChaEntry;Xloop<=nxChaExit;Xloop++){
      xBegin = (Xloop==nxChaEntry ? xstart : retLenght(Xloop,x_rdout) );
      xEnd   = (Xloop==nxChaExit  ? xstop  : retLenght(Xloop+1,x_rdout) );

      if(debug) std::cout<< "Linear Parameters bef p flag. Xbeg "
                         <<xBegin<<"; xEnd: "
                         <<xEnd<<" Ybeg "
                         <<yBegin<<"; yEnd: "
                         <<yEnd<<" int: "
                         <<intercept<<" slope: "
                         <<slopeY<<std::endl;


      if(parallelFlag){
        yBegin=y_enter;	yEnd= y_exit;
      } else {
        yBegin=intercept+slopeY*xBegin ;
        yEnd=intercept+slopeY*xEnd ;
      }

      if(debug) std::cout<< "Computing Linear Parameters. Xbeg "
                         <<xBegin<<"; xEnd: "
                         <<xEnd<<" Ybeg "
                         <<yBegin<<"; yEnd: "
                         <<yEnd<<" int: "
                         <<intercept<<" slope: "
                         <<slopeY<<std::endl;

      double xinit,yinit,xend,yend;
      int tmpYBegin(0),tmpYEnd(0);
      double tmpYLenght(0);

      for(inyLo = 0; inyLo < PhNy; inyLo++) {
        if(yBegin - tmpYLenght > ParallelCutOff) {
          tmpYLenght += y_rdout[inyLo];
          tmpYBegin =  inyLo;
        } else {
          break;
        }
      }
      tmpYLenght = 0;
      for(inyLo = 0; inyLo < PhNy; inyLo++) {
        if(yEnd - tmpYLenght > ParallelCutOff) {
          tmpYLenght += y_rdout[inyLo];
          tmpYEnd =  inyLo;
        } else {
          break;
        }
      }

      if(debug) std::cout<< "Debugging tmpY_beg: "
                         <<tmpYBegin<<"; tmpY_end: "
                         <<tmpYEnd<<std::endl;

      if(tmpYBegin<=tmpYEnd){
        if( tmpYBegin<0)tmpYBegin=0;
        if( tmpYEnd>=(int)PhNy)tmpYEnd=(int)PhNy-1 ;

        nyBegin=tmpYBegin; nyEnd=tmpYEnd;

        xinit=xBegin;	yinit=yBegin;
        xend=xEnd;	yend=yEnd;

      } else {

        if( tmpYEnd<0)tmpYEnd=0;
        if( tmpYBegin>=(int)PhNy)tmpYBegin=(int)PhNy-1 ;

        nyBegin=tmpYEnd; nyEnd=tmpYBegin;

        xinit=xEnd;	yinit=yEnd;
        xend=xBegin;	yend=yBegin;

      }

      if(debug) std::cout<< "Debugging Y_beg: "
                         <<nyBegin<<"; Y_end: "
                         <<nyEnd<<std::endl;

      for (unsigned int Yloop=nyBegin;Yloop<=nyEnd;Yloop++){
        // Compute distance from the boundaries
        // of the physical channel
        double myX(0),myY(0);
        if(nyBegin==nyEnd){
          myX = (xinit+xend)/2;
          myY = (yinit+yend)/2;
        } else if(Yloop==nyBegin&&Yloop!=nyEnd){
          if(parallelFlag){
            myX=xinit;
          }else{
            myX = (xinit + (retLenght(Yloop+1,y_rdout)-intercept)/slopeY)/2;
          }
          myY = (yinit + retLenght(Yloop+1,y_rdout))/2;
        } else if(Yloop!=nyBegin&&Yloop==nyEnd){
          if(parallelFlag){
            myX=xend;
          }else{
            myX = (((retLenght(Yloop,y_rdout)-intercept)/slopeY)+xend)/2;
          }
          myY = (retLenght(Yloop,y_rdout)+yend)/2;
        } else {
          if(parallelFlag){
            myX=(xend+xinit)/2;
          }else{
            myX = ((retLenght(Yloop,y_rdout)-intercept)/slopeY
                   + (retLenght(Yloop+1,y_rdout)-intercept)/slopeY)/2;
          }
          myY = (retLenght(Yloop,y_rdout) + retLenght(Yloop+1,y_rdout))/2;
        }


        MuonFrontEndID input;

        input.setFEGridX(PhNx);
        input.setFEGridY(PhNy);
        input.setFEIDX(Xloop);
        input.setFEIDY(Yloop);
        input.setReadout(m_readoutType[iRd]);

        keepTemporary.emplace_back( input, std::array{
                                              float(  myX - retLenght(Xloop,x_rdout) ),
                                              float(  myY - retLenght(Yloop,y_rdout) ),
                                              float(  retLenght(Xloop+1,x_rdout) - myX ),
                                              float(  retLenght(Yloop+1,y_rdout) - myY) });
        const auto& myBoundary = keepTemporary.back().second;

        if(debug) std::cout<< "Hit processing.  RT:: "
                           <<m_readoutType[iRd]<<" ; Xl = "
                           <<" "<<Xloop<<" ; Bd = "
                           <<" "<<myBoundary[0]<<" ; Bd2 = "
                           <<" "<<myBoundary[1]<<" ; Bd3 = "
                           <<" "<<myBoundary[2]<<" ; Bd4 = "
                           <<" "<<myBoundary[3]<<" ; Yl = "
                           <<Yloop<<"\n";

      }
    }
  }

  return keepTemporary;
}

double MuonChamberGrid::retLenght(int nLx, const std::vector<double>& my_list){
  int VctSize = my_list.size();
  if(nLx > VctSize) {
    nLx = VctSize;
    std::cout<<"MuonChamberGrid:: Vector index Out Of Range."<<std::endl;
  }
  return std::accumulate( begin(my_list), std::next(begin(my_list),nLx),
                          double{0} );
}

StatusCode MuonChamberGrid::getPCCenter(MuonFrontEndID fe,
                                        double& xcenter, double& ycenter){
  std::vector<double> x_rdout;
  std::vector<double> y_rdout;
  int readout=fe.getReadout();

  int TstRead =  m_x_pad_rdout2.size();
  int RdN = 1;
  if(TstRead > 1) RdN = 2;

  //get the correct grid
  for(int iRd=0;iRd<RdN;iRd++){
    if(m_readoutType[iRd]==readout){
      if(iRd) {
        x_rdout = m_x_pad_rdout2;
        y_rdout = m_y_pad_rdout2;
      } else {
        x_rdout = m_x_pad_rdout1;
        y_rdout = m_y_pad_rdout1;
      }
      break;
    }
  }
  unsigned int chx=fe.getFEIDX();
  unsigned int chy=fe.getFEIDY();
  if(chx>0){
    xcenter=(x_rdout[chx]+x_rdout[chx-1])/2;
  }else  xcenter=(x_rdout[chx])/2;

  if(chy>0){
    ycenter=(y_rdout[chy]+y_rdout[chy-1])/2;
  }else  ycenter=(y_rdout[chy])/2;

return StatusCode::SUCCESS;
}
