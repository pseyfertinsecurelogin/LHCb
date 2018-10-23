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
#ifndef DQFILTERSVC_H
#define DQFILTERSVC_H

#include "GaudiKernel/Service.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "Kernel/IAccept.h"

/** Simple service installing itself in the IncidentSvc to filter events on
 * DQ Flags via an IAccept tool (by default DQAcceptTool).
 *
 * @author Marco Clemencic
 * @date 2012-02-10
 */
class DQFilterSvc final : public extends1<Service, IIncidentListener>
{

public:

  /// Constructor
  DQFilterSvc(const std::string& name, ISvcLocator* svc);

  /// Initialize the service
  StatusCode initialize() override;

  /// Finalize the service
  StatusCode finalize() override;

  /// @see IIncidentListener::handle
  void handle( const Incident& ) override;

  /// Destructor
  virtual ~DQFilterSvc() = default;

private:

  /// Type/Name of the (public) IAccept tool used to choose if the event has to
  /// be accepted or not (default: DQAcceptTool).
  std::string m_acceptToolName;

  /// Pointer to the IAccept tool.
  IAccept *m_acceptTool = nullptr;

  /// Pointer to the ToolSvc.
  SmartIF<IToolSvc> m_toolSvc;

  /// Pointer to the IncidentSvc.
  SmartIF<IIncidentSvc> m_incSvc;

};

#endif
