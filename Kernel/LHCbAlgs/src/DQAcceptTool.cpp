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
#include "DetDesc/Condition.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IUpdateManagerSvc.h"
#include "GaudiKernel/ToolHandle.h"
#include "Kernel/IAccept.h" // IAccept
#include "Kernel/IDQFilter.h"
#include "Kernel/IDQScanner.h"

// ----------------------------------------------------------------------------
// Implementation file for class: DQAcceptTool
//
// 09/02/2012: Marco Clemencic
// ----------------------------------------------------------------------------

/** @class DQAcceptTool DQAcceptTool.h src/DQAcceptTool.h
 *
 *
 * @author Marco Clemencic
 * @date 09/02/2012
 */
class DQAcceptTool final : public extends<GaudiTool, IAccept> {

public:
  /// Standard constructor
  DQAcceptTool( const std::string& type, const std::string& name, const IInterface* parent );

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
  IDQFilter*  m_filter  = nullptr;
  IDQScanner* m_scanner = nullptr;
  /// @}

  /// Transient flag updated every time the run condition changes to state
  /// if the currently processed event is good or bad.
  bool m_accepted{true};

  /// Pointer to condition to use (filled by the UpdateManagerSvc).
  /// @see m_condPath
  Condition* m_cond = nullptr;
};
DECLARE_COMPONENT( DQAcceptTool )

#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define DEBUG_MSG ON_DEBUG debug()

// ============================================================================
// Standard constructor, initializes variables
// ============================================================================
DQAcceptTool::DQAcceptTool( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  declareProperty( "ConditionPath", m_condPath = "Conditions/Online/LHCb/RunParameters",
                   "Path in the Detector Transient Store where to find the run "
                   "condition." );
  declareProperty( "ByRun", m_byRun = true, "If the DQ Flags have to be considered by run or by event." );
  declareProperty( "Filter", m_filterName = "BasicDQFilter", "IDQFilter Tool defining the acceptance rules." );
  declareProperty( "Scanner", m_scannerName = "CondDBDQScanner",
                   "IDQScanner Tool to collect all the DQ Flags in a run." );
}

StatusCode DQAcceptTool::initialize() {
  StatusCode sc = GaudiTool::initialize();
  if ( sc.isFailure() ) return sc;

  if ( UNLIKELY( m_condPath.empty() ) ) {
    error() << "Cannot work with empty ConditionPath" << endmsg;
    return StatusCode::FAILURE;
  }

  m_filter  = tool<IDQFilter>( m_filterName, this );
  m_scanner = tool<IDQScanner>( m_scannerName, this );

  registerCondition( m_condPath, m_cond,
                     ( m_byRun ) ? &DQAcceptTool::i_checkFlagsByRun : &DQAcceptTool::i_checkFlagsByEvent );

  ON_DEBUG {
    debug() << "DQAcceptTool/" << name() << " initialized:" << endmsg;
    debug() << "  using condition at '" << m_condPath << "'";
    if ( m_byRun ) {
      debug() << " for run boundaries" << endmsg;
      debug() << "  scanning flags with " << m_scannerName << endmsg;
    } else {
      debug() << " DQ Flags" << endmsg;
    }
    debug() << "  analyze flags with " << m_filterName << endmsg;
  }

  // We do not want to run a scan during initialization, but we may be
  // instantiated during and event. So, if it is the case, we trigger an
  // immediate update.
  SmartIF<IStateful> stateMachine( serviceLocator() );
  if ( stateMachine->FSMState() >= Gaudi::StateMachine::RUNNING ) { sc = updMgrSvc()->update( this ); }

  return sc;
}

StatusCode DQAcceptTool::i_checkFlagsByRun() {
  if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
    verbose() << "Updating Data Quality flags for run " << m_cond->param<int>( "RunNumber" ) << endmsg;

  const IDQFilter::FlagsType& flags = m_scanner->scan( m_cond->validSince(), m_cond->validTill() );
  if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "-> " << flags << endmsg;

  m_accepted = m_filter->accept( flags );
  // we successfully updated the m_bad state
  return StatusCode::SUCCESS;
}

StatusCode DQAcceptTool::i_checkFlagsByEvent() {
  if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "Updating Data Quality flags" << endmsg;

  const auto& flags = m_cond->param<IDQFilter::FlagsType>( "map" );
  if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) ) verbose() << "-> " << flags << endmsg;

  m_accepted = m_filter->accept( flags );
  // we successfully updated the m_bad state
  return StatusCode::SUCCESS;
}

StatusCode DQAcceptTool::finalize() {
  if ( m_filter ) {
    releaseTool( m_filter ).ignore();
    m_filter = nullptr;
  }
  if ( m_scanner ) {
    releaseTool( m_scanner ).ignore();
    m_scanner = nullptr;
  }

  return GaudiTool::finalize();
}

bool DQAcceptTool::accept() const { return m_accepted; }

// ============================================================================
