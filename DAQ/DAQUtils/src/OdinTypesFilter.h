// $Id: OdinTypesFilter.h,v 1.2 2008/08/21 16:33:08 odescham Exp $
#ifndef COMPONENT_ODINTYPESFILTER_H
#define COMPONENT_ODINTYPESFILTER_H 1

#include <string>
#include <vector>

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


/** @class OdinTypesFilter OdinTypesFilter.h component/OdinTypesFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinTypesFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  OdinTypesFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~OdinTypesFilter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:
  std::string m_log;
  std::vector<std::string> m_trs;
  std::vector<std::string> m_bxs;
  std::vector<std::string> m_ros;
  std::vector<std::string> m_cls;
  int m_winmin,m_winmax;
  long m_all;
  long m_acc;


};
#endif // COMPONENT_ODINTYPESFILTER_H
