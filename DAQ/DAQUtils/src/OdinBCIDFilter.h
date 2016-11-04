// $Id: OdinBCIDFilter.h,v 1.1 2008/10/28 11:20:32 odescham Exp $
#ifndef COMPONENT_ODINBCIDFILTER_H
#define COMPONENT_ODINBCIDFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class OdinBCIDFilter OdinBCIDFilter.h component/OdinBCIDFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2008-02-05
 */
class OdinBCIDFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  OdinBCIDFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~OdinBCIDFilter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:
  bool rule(long bx);
  long m_mask;
  long m_value;
  std::string m_comparator;
  bool m_revert;

  long m_all;
  long m_acc;

};
#endif // COMPONENT_ODINBCIDFILTER_H
