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
#ifndef SRC_DQACCEPTTOOL_H
#define SRC_DQACCEPTTOOL_H 1
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/ToolHandle.h"
#include "Kernel/IDQFilter.h"
#include "Kernel/IDQScanner.h"

// Implemented interfaces
#include "Kernel/IAccept.h" // IAccept

struct Condition;

/** @class DQAcceptTool DQAcceptTool.h src/DQAcceptTool.h
  *
  *
  * @author Marco Clemencic
  * @date 09/02/2012
  */
class DQAcceptTool final : public extends1<GaudiTool, IAccept>
{

public:

  /// Standard constructor
  DQAcceptTool(const std::string& type, const std::string& name, const IInterface* parent);

  /// Initialize the tool.
  StatusCode initialize() override;

  /// Finalize the tool.
  StatusCode finalize() override;

  /// Tells if the current event has to be accepted or not, according to the
  virtual bool accept() const override;

 private:

  /// Call-back function passed to the UpdateManagerSvc to update the current
  /// filtering status (good or bad).
  StatusCode i_checkFlagsByRun();

  /// Call-back function passed to the UpdateManagerSvc to update the current
  /// filtering status (good or bad).
  StatusCode i_checkFlagsByEvent();

private:

  /// Path to the used condition object.
  /// Depending on the "ByRun" property it defined the condition to get
  /// the run boundaries or the one for the DQ Flags.
  std::string m_condPath;

  /// Tell if the DQ flags have to be used by run or by event.
  bool m_byRun;

  /// Type/Name of the (private) tool to filter the DQ Flags (default: BasicDQFilter).
  std::string m_filterName;
  /// Type/Name of the (private) tool to scan the CondDB for the DQ Flags of a run (default: CondDBDQScanner).
  std::string m_scannerName;

  /// @{
  /// Pointers to the tools.
  IDQFilter *m_filter = nullptr;
  IDQScanner *m_scanner = nullptr;
  /// @}

  /// Transient flag updated every time the run condition changes to state
  /// if the currently processed event is good or bad.
  bool m_accepted{true};

  /// Pointer to condition to use (filled by the UpdateManagerSvc).
  /// @see m_condPath
  Condition *m_cond = nullptr;

};

#endif // SRC_DQACCEPTTOOL_H
