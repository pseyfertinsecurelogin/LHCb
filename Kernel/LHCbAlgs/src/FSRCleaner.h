// $Id: $
#ifndef FSRCleaner_H
#define FSRCleaner_H 1

// #include "GaudiAlg/GaudiAlgorithm.h"

#include <string>

// #include "GaudiKernel/IDataManagerSvc.h"
// #include "GaudiKernel/SmartIF.h"
#include "EmptyEventNodeCleaner.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class FSRCleaner FSRCleaner.h
 *
 *  Searches for and removes empty data nodes in the FSR tree
 *
 *  @author Rob Lambert
 *  @date   2012-08-01
 */
class FSRCleaner final : public EmptyEventNodeCleaner
{

public:

  /// Standard constructor
  FSRCleaner( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~FSRCleaner( ) = default; ///< Destructor

  StatusCode execute() override { return StatusCode::SUCCESS; } ///< Algorithm execution, do nothing
  StatusCode finalize() override;    ///< Real algorithm execution in finalize

};

#endif // FSRCleaner_H
