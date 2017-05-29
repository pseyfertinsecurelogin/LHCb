#ifndef FSRNAVIGATOR_H
#define FSRNAVIGATOR_H 1

#include <string>
#include <vector>

#include "FSRAlgs/IFSRNavigator.h"            // Interface
// Include files
// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IDataProviderSvc.h"
#include "GaudiKernel/IOpaqueAddress.h"
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/extends.h"

class IFSRNavigator;
class IInterface;
class IRegistry;

/** @class FSRNavigator FSRNavigator.h
 *
 *
 *  @author Jaap Panman
 *  @date   2010-10-05
 */
class FSRNavigator : public extends<GaudiTool, IFSRNavigator> {
public:
  /// Standard constructor
  FSRNavigator( const std::string& type,
                  const std::string& name,
                  const IInterface* parent);

  StatusCode initialize() override;
  StatusCode finalize() override;

  // return list of addresses found below the address
  std::vector< std::string > navigate(std::string rootname, std::string tag) override;

protected:
  void explore(IRegistry* pObj, std::string tag, std::vector< std::string >& addresses) override;

private:
  std::string m_ToolName;                       ///> name of tool for normalization
  SmartIF<IDataProviderSvc> m_fileRecordSvc ;  ///> Reference to file records data service

};
#endif // FSRNAVIGATOR_H
