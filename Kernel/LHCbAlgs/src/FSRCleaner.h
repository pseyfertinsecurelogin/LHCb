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
#ifndef FSRCleaner_H
#define FSRCleaner_H 1

// #include "GaudiAlg/GaudiAlgorithm.h"

// #include "GaudiKernel/IDataManagerSvc.h"
// #include "GaudiKernel/SmartIF.h"
#include "EmptyEventNodeCleaner.h"

/** @class FSRCleaner FSRCleaner.h
 *
 *  Searches for and removes empty data nodes in the FSR tree
 *
 *  @author Rob Lambert
 *  @date   2012-08-01
 */
class FSRCleaner final : public EmptyEventNodeCleaner {

public:
  /// Standard constructor
  FSRCleaner( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~FSRCleaner() = default; ///< Destructor

  StatusCode execute() override { return StatusCode::SUCCESS; } ///< Algorithm execution, do nothing
  StatusCode finalize() override;                               ///< Real algorithm execution in finalize
};

#endif // FSRCleaner_H
