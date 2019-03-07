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
#ifndef EVENTCOUNTHISTO_H
#define EVENTCOUNTHISTO_H 1

// Include files
#include "GaudiAlg/GaudiHistoAlg.h"

/** @class EventCountHisto EventCountHisto.h
 *
 *
 *  @author Robert Lambert
 *  @date   2007-01-08
 */
class EventCountHisto final : public GaudiHistoAlg {

public:
  /// Standard constructor
  EventCountHisto( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~EventCountHisto() = default; ///< Destructor

  StatusCode execute() override;  ///< Algorithm execution
  StatusCode finalize() override; ///< Algorithm finalization

private:
  unsigned long long m_nEvents{0}; // number of events processed
};

#endif // EVENTCOUNTHISTO_H
