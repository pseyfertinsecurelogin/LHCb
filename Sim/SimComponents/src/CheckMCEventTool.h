#ifndef CHECKMCEVENTTOOL_H
#define CHECKMCEVENTTOOL_H 1

// Include files

// from Gaudi
#include "GaudiAlg/GaudiTool.h"

// interface
#include "Kernel/ICheckTool.h"


/** @class CheckMCEventTool CheckMCEventTool.h
 *  Tool to check integrity of MCEvent structure
 *
 *  @author Marco Cattaneo
 *  @date   2003-02-24
 */
class CheckMCEventTool : public extends<GaudiTool, ICheckTool> {
public:
  /// Standard constructor
  CheckMCEventTool( const std::string& type,
                    const std::string& name,
                    const IInterface* parent);

  StatusCode check() override; ///< Perform the check

private:
  long long m_event = -1;    ///< last event number
  StatusCode m_status = StatusCode::SUCCESS; ///< Status of last check
};
#endif // CHECKMCEVENTTOOL_H
