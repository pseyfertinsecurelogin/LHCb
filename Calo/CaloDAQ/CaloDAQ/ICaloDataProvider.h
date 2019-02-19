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
#ifndef CALODAQ_ICALODATAPROVIDER_H
#define CALODAQ_ICALODATAPROVIDER_H 1

// Include files
// from STL
#include <string>

#include "CaloDAQ/ICaloReadoutTool.h"
#include "CaloKernel/CaloVector.h"
#include "Event/CaloAdc.h"
#include "Event/CaloDigit.h"

/** @class ICaloDataProvider ICaloDataProvider.h CaloDAQ/ICaloDataProvider.h
 *  Interface to the tool for fast decoding of CaloBanks
 *
 *  @author Olivier Deschamps
 *  @date   2007-02-27
 */
struct ICaloDataProvider : extend_interfaces<ICaloReadoutTool> {

  // Return the interface ID
  DeclareInterfaceID( ICaloDataProvider, 5, 0 );

  virtual int                                     adc( LHCb::CaloCellID id, int def = 0 )               = 0;
  virtual double                                  digit( LHCb::CaloCellID id, double def = 0. )         = 0;
  virtual unsigned int                            nTell1s()                                             = 0;
  virtual const CaloVector<LHCb::CaloAdc>&        adcs( int source = -1, bool clean = true )            = 0;
  virtual const CaloVector<LHCb::CaloDigit>&      digits( int source = -1, bool clean = true )          = 0;
  virtual const CaloVector<LHCb::CaloAdc>&        adcs( std::vector<int> sources, bool clean = true )   = 0;
  virtual const CaloVector<LHCb::CaloDigit>&      digits( std::vector<int> sources, bool clean = true ) = 0;
  typedef std::pair<LHCb::CaloAdc, LHCb::CaloAdc> CaloAdcPair;
  virtual CaloAdcPair                             adcRange() = 0;
  virtual CaloAdcPair                             pinRange() = 0;
};
#endif // CALODAQ_ICALODATAPROVIDER_H
