#ifndef LOADDDDB_H
#define LOADDDDB_H 1

#include <string>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class LoadDDDB LoadDDDB.h
 *
 *  Load entries in the detector transient store using IDataSvc::preLoad().
 *  The node to be loaded is set with the option LoadDDDB.Node.
 *
 *  @author Marco Clemencic
 *  @date   2005-10-14
 */
class LoadDDDB : public GaudiAlgorithm {
public:
  /// Standard constructor
  LoadDDDB( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~LoadDDDB( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:

  std::string m_treeToLoad;

};
#endif // LOADDDDB_H
