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
// ============================================================================
#define MUONDET_DEMUONCHAMBER_CPP 1

// Include files
#include "MuonDet/DeMuonChamber.h"
#include "DetDesc/IGeometryInfo.h"
//Detector descriptions
#include "DetDesc/Condition.h"

#include "boost/algorithm/string/predicate.hpp"

/** @file DeMuonChamber.cpp
 *
 * Implementation of class : DeMuonChamber
 *
 * @author David Hutchcroft, David.Hutchcroft@cern.ch
 *
 */

/// Constructor setting pad sizes and number of gas gaps and chamber number
DeMuonChamber::DeMuonChamber( int nStation,
                              int nRegion,
                              int nChamber)
  : m_StationNumber(nStation),
    m_RegionNumber(nRegion),
    m_ChamberNumber(nChamber)
{
}


StatusCode DeMuonChamber::initialize()
{
  StatusCode sc = DetectorElement::initialize();
  if( sc.isFailure() ) {
    msgStream() << MSG::ERROR << "Failure to initialize DetectorElement" << endmsg;
    return sc ;
  }
  int sta(0),reg(0),chm(0);
  const std::string& name=this->name();
  int len=name.size();
  int start=(DeMuonLocation::Default).size();
  std::string substring;
  substring.assign(name,start,len);
  char patt[400];
  sprintf(patt,"%s",substring.c_str());
  std::string stanum;
  stanum.assign(name,start,3);
  sscanf(stanum.c_str(),"/M%d",&sta);
  std::string regnum;
  regnum.assign(name,start+11,3);
  sscanf(regnum.c_str(),"/R%d",&reg);
  std::string chnum;
  chnum.assign(name,start+19,10);
  sscanf(chnum.c_str(),"/Cham%d",&chm);
  //  this->setStationNumber(sta-1); // error !!!!!! when M1 is not present station number =1 for M2 !!!
  this->setStationNumber(sta-1);
  this->setRegionNumber(reg-1);
  this->setChamberNumber(chm-1);

  // get resolution parameters
  m_chmbGrid     = param<std::string>("Grid");

  // for now with MWPCs and RPCs this is a good formula
  setGridName(m_chmbGrid);
  if( UNLIKELY( msgStream().level() <= MSG::DEBUG ) )
    msgStream()<<MSG::DEBUG<<"Initialising DeMuonChamber: "<<name <<" "<<sta<<" "<<reg<<" "<<chm<<endmsg;
  return sc;
}

IPVolume* DeMuonChamber::getFirstGasGap(){

  for (auto pvIterator =geometry()->lvolume()->pvBegin();
            pvIterator!=geometry()->lvolume()->pvEnd();++pvIterator){

    const ILVolume*  geoCh=(*pvIterator)->lvolume();

    if( boost::ends_with( geoCh->name(), "/lvGasGap" )){
      //loop un variys gap layer to get the real gas gap volume
      for (auto pvGapIterator=(geoCh->pvBegin());pvGapIterator!=(geoCh->pvEnd());++pvGapIterator){
	if(!((*pvGapIterator)->lvolume()->sdName().empty())){
	  return (*pvGapIterator);
	}
      }
    }
  }
  return nullptr;
}

IPVolume* DeMuonChamber::getGasGap(int number){
  int loopnumber=0;
  for (auto pvIterator=geometry()->lvolume()->pvBegin();
           pvIterator!=geometry()->lvolume()->pvEnd();++pvIterator){

    const ILVolume*  geoCh=(*pvIterator)->lvolume();

    //msg<<MSG::ERROR<<"test del nome "<<mystring<<endmsg;
    if( boost::ends_with( geoCh->name(),"/lvGasGap") ){
      //loop un variys gap layer to get the real gas gap volume
      for (auto pvGapIterator=(geoCh->pvBegin());pvGapIterator!=(geoCh->pvEnd());pvGapIterator++){
        if(!((*pvGapIterator)->lvolume()->sdName().empty())){
          if(loopnumber==number) return *pvGapIterator;
          loopnumber++;

        }
      }
    }
  }
  return nullptr;
}



IPVolume* DeMuonChamber::getGasGapLayer(int number){
  int loopnumber=0;
  for (auto pvIterator=((geometry()->lvolume())->pvBegin());
       pvIterator!=(((geometry())->lvolume())->pvEnd());pvIterator++){

    const ILVolume*  geoCh=(*pvIterator)->lvolume();

    //msg<<MSG::ERROR<<"test del nome "<<mystring<<endmsg;
    if(boost::ends_with( geoCh->name(), "/lvGasGap")){
      if(loopnumber==number) return (*pvIterator);
      loopnumber++;

    }

  }
  return nullptr;
}


StatusCode  DeMuonChamber::isPointInGasGap( Gaudi::XYZPoint pointInChamber,
                                            Gaudi::XYZPoint& pointInGap,
                                            int& number,
                                            IPVolume* & gasVolume){

 // StatusCode sc;
  int loopnumber=0;
  for (auto pvIterator=((geometry()->lvolume())->pvBegin());
       pvIterator!=(((geometry())->lvolume())->pvEnd());pvIterator++){

    const ILVolume*  geoCh=(*pvIterator)->lvolume();

    if(boost::ends_with(geoCh->name(),"/lvGasGap")){
      //loop un variys gap layer to get the real gas gap volume

      bool isIn = (*pvIterator)->isInside(pointInChamber);

      if(isIn){
        Gaudi::XYZPoint myPointInGasFrame=(*pvIterator)->toLocal(pointInChamber);
        for (auto pvGapIterator=(geoCh->pvBegin());pvGapIterator!=(geoCh->pvEnd());pvGapIterator++){
          if(!((*pvGapIterator)->lvolume()->sdName().empty())){
            bool isInGap = (*pvGapIterator)->isInside(myPointInGasFrame);
            if(isInGap){
              pointInGap= (*pvGapIterator)->toLocal(myPointInGasFrame);
              gasVolume=(*pvGapIterator);
              number=loopnumber;
              return StatusCode::SUCCESS;
            }else{
              return StatusCode::FAILURE;
            }
          }
        }
      }
      loopnumber++;
    }
  }

  return StatusCode::FAILURE;

}

StatusCode  DeMuonChamber::isPointInGasGap(Gaudi::XYZPoint
pointInChamber,Gaudi::XYZPoint& pointInGap,IPVolume* & gasVolume){
  int number=0;
return isPointInGasGap(pointInChamber,pointInGap,number,gasVolume);
}




int DeMuonChamber::getGasGapNumber(){
  int loopnumber=0;
  for (auto pvIterator=((geometry()->lvolume())->pvBegin());
       pvIterator!=(((geometry())->lvolume())->pvEnd());pvIterator++){

    const ILVolume*  geoCh=(*pvIterator)->lvolume();
    //msg<<MSG::ERROR<<"test del nome "<<mystring<<endmsg;
    if(boost::ends_with( geoCh->name(), "/lvGasGap")){
      //loop un variys gap layer to get the real gas gap volume
      for (auto pvGapIterator=(geoCh->pvBegin());pvGapIterator!=(geoCh->pvEnd());pvGapIterator++){
        if(!((*pvGapIterator)->lvolume()->sdName().empty())){
          loopnumber++;
        }
      }
    }
  }
  return loopnumber;
}
