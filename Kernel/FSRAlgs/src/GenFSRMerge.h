#ifndef GENFSRMERGE_H
#define GENFSRMERGE_H 1

#include <string>

// event model
#include "Event/GenFSR.h"
// local
#include "FSRNavigator.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/IGenericTool.h"            // Interface
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"

class IFSRNavigator;
class ISvcLocator;

/** @class GenMergeFSR GenMergeFSR.h
 *
 *
 *  @author Davide Fazzini
 *  @date   2015-06-25
 */

class GenFSRMerge : public GaudiAlgorithm
{
 public:
  // Standard constructor
  GenFSRMerge(const std::string& name, ISvcLocator* pSvcLocator);

  // Algorithm initialization, execution and finalization
  StatusCode initialize() override;
  StatusCode execute() override;
  StatusCode finalize() override;

 protected:
  // Merge the FSRs of one input file
  virtual StatusCode merge();

  // Reference to run records data service
  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  // Location of FileRecords
  std::string m_FileRecordName;
  // Specific tag of summary data in FSR
  std::string m_FSRName;

 private:
  // Tool navigate FSR
  IFSRNavigator *m_navigatorTool = nullptr;

  int m_evtType;
  std::string m_genName;
  std::string m_method;

};

#endif // GENFSRMERGE_H
