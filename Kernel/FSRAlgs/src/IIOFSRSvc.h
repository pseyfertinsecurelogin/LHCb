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
#ifndef GAUDIKERNEL_IIOFSRSVC_H
#define GAUDIKERNEL_IIOFSRSVC_H

// Include files
#include "GaudiKernel/IService.h"

/** @class ICounterSummarySvc ICounterSummarySvc.h
 *
 *  Simple service interface to return/store IOFSRS
 *
 *
 *  @see IOFSRSvc
 *
 *  @author Rob Lambert
 *  @date   2012-11-26
 */
struct GAUDI_API IIOFSRSvc : virtual IService {

  /// InterfaceID
  DeclareInterfaceID( IIOFSRSvc, 1, 0 );

  // merge existing IOFSRs into the maps of this service
  virtual StatusCode mergeIOFSRs() = 0;
  // remove any IOFSR at the top level. To be called before requesting a new FSR.
  virtual StatusCode cleanTopFSR() = 0;
  // Create a new IOFSR, store in the TES
  virtual StatusCode storeIOFSR( const std::string& outputName ) = 0;
  // Does the accounting all add up OK?
  virtual bool traceCounts() = 0;

  // print contained information
  virtual void print() = 0;
};

#endif
