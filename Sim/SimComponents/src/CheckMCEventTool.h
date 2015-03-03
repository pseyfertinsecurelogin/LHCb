// $Id: CheckMCEventTool.h,v 1.2 2008-10-28 15:34:08 cattanem Exp $
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
class CheckMCEventTool : public GaudiTool, virtual public ICheckTool {
public:
  /// Standard constructor
  CheckMCEventTool( const std::string& type, 
                    const std::string& name,
                    const IInterface* parent);

  virtual ~CheckMCEventTool( ); ///< Destructor

  virtual StatusCode check(); ///< Perform the check
  
protected:

private:
  int m_event;         ///< last event number
  StatusCode m_status; ///< Status of last check
};
#endif // CHECKMCEVENTTOOL_H
