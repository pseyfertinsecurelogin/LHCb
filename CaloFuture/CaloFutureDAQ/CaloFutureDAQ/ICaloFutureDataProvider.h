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
#ifndef CALOFUTUREDAQ_ICALOFUTUREDATAPROVIDER_H 
#define CALOFUTUREDAQ_ICALOFUTUREDATAPROVIDER_H 1

// Include files
// from STL
#include <string>

#include "CaloFutureDAQ/ICaloFutureReadoutTool.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"
#include "CaloKernel/CaloVector.h"


/** @class ICaloFutureDataProvider ICaloFutureDataProvider.h CaloFutureDAQ/ICaloFutureDataProvider.h
 *  Interface to the tool for fast decoding of CaloFutureBanks
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-27
 */
struct ICaloFutureDataProvider : extend_interfaces<ICaloFutureReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloFutureDataProvider, 5, 0 );

  virtual int adc(LHCb::CaloCellID id,int def=0)=0;
  virtual double digit(LHCb::CaloCellID id,double def=0.)=0;
  virtual unsigned int nTell1s()=0;
  virtual const CaloVector<LHCb::CaloAdc>& adcs(int source=-1,bool clean=true)=0;
  virtual const CaloVector<LHCb::CaloDigit>& digits(int source=-1,bool clean=true)=0;
  virtual const CaloVector<LHCb::CaloAdc>&  adcs(std::vector<int> sources,bool clean=true)=0;
  virtual const CaloVector<LHCb::CaloDigit>& digits(std::vector<int> sources,bool clean=true)=0;
  typedef std::pair<LHCb::CaloAdc,LHCb::CaloAdc> CaloAdcPair;
  virtual CaloAdcPair adcRange() = 0;
  virtual CaloAdcPair pinRange() = 0;


};
#endif // CALOFUTUREDAQ_ICALOFUTUREDATAPROVIDER_H
