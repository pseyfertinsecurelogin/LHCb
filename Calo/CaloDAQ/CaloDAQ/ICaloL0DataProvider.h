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
#ifndef CALODAQ_ICALOL0DATAPROVIDER_H
#define CALODAQ_ICALOL0DATAPROVIDER_H 1

// Include files
// from STL
#include <string>

#include "CaloDAQ/ICaloReadoutTool.h"
#include "CaloKernel/CaloVector.h"
#include "Event/L0CaloAdc.h"

/** @class ICaloL0DataProvider ICaloL0DataProvider.h CaloDAQ/ICaloL0DataProvider.h
 *  Interface to the tool for fast decoding of CaloBanks
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-03
 */
struct ICaloL0DataProvider : extend_interfaces<ICaloReadoutTool> {

  DeclareInterfaceID( ICaloL0DataProvider, 4, 0 );

  virtual int                                l0Adc( LHCb::CaloCellID id, int def = 0 )             = 0;
  virtual unsigned int                       nTell1s() const                                       = 0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs( int source = -1, bool clean = true )          = 0;
  virtual const CaloVector<LHCb::L0CaloAdc>& l0Adcs( std::vector<int> sources, bool clean = true ) = 0;
};
#endif // CALODAQ_ICALOL0DATAPROVIDER_H
