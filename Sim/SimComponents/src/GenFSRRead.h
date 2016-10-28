#ifndef GENFSRREAD_H
#define GENFSRREAD_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/IDataManagerSvc.h"

// from Event
#include "Event/GenFSR.h"

// from Kernel
#include "FSRAlgs/IFSRNavigator.h"

/** @class GenFSRRead GenFSRRead.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-23
 */
class GenFSRRead : public GaudiAlgorithm {
public:
  // Standard constructor
  GenFSRRead( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override;  // Algorithm initialization
  StatusCode execute   () override;  // Algorithm execution
  StatusCode finalize  () override;  // Algorithm finalization

private:

  virtual void dumpFSR();       // Dump the GenFSR informations

  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  std::string m_fileRecordName;     // location of FileRecords
  std::string m_FSRName;            // specific tag of summary data in FSR

  IFSRNavigator* m_navigatorTool = nullptr;   // tool to navigate FSR

};
#endif // GENFSRREAD_H
