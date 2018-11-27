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
/*
 * DQFilter.h
 *
 *  Created on: Jan 31, 2012
 *      Author: marcocle
 */

#ifndef DQFILTER_H_
#define DQFILTER_H_

// Include files
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/ToolHandle.h"
#include "Kernel/IAccept.h"

struct Condition;
class IIncidentSvc;

/** @class DQFilter
 *  Small algorithm to filter events according to the Data Quality flags stored
 *  in the conditions database on a run-by-run basis.
 *
 *  When initialized, the algorithm register itself as user of the DQ Flags
 *  conditions and, by default, it listens for BeginEvent incidents.
 *
 *  @author Marco Clemencic
 *  @date   Jan 31, 2012
 */
class DQFilter final : public extends1<GaudiAlgorithm, IIncidentListener>
{

public:

  /// Standard constructor
  DQFilter( const std::string& name, ISvcLocator* pSvcLocator );

  /// Initialize the algorithm.
  /// Register to the UpdateManagerSvc as user of the Data Quality flags condition
  /// and, if not requested otherwise, register as BeginEvent listener.
  StatusCode initialize() override;
  /// By default it does nothing, but if requested not to use the BeginEvent,
  /// it sets the
  StatusCode execute() override;
  /// If registered as BeginEvent listener, unregister.
  StatusCode finalize() override;

  void handle(const Incident&) override;

private:

  /// Flag to state if we have to filter at the level of the Begin Event
  /// incident or during the execute.
  bool m_beginEvent;

  /// Type/Name of the (public) IAccept tool used to choose if the event has to
  /// be accepted or not (default: DQAcceptTool).
  std::string m_acceptToolName;

  /// Pointer to the IAccept tool.
  IAccept *m_acceptTool = nullptr;

  /// Pointer to the IncidentSvc.
  SmartIF<IIncidentSvc> m_incSvc;
};

#endif // DQFILTERBYRUN_H_
