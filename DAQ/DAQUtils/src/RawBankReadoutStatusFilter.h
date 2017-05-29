// $Id: RawBankReadoutStatusFilter.h,v 1.1 2007/12/05 13:59:18 odescham Exp $
#ifndef COMPONENT_RAWBANKREADOUTSTATUSFILTER_H
#define COMPONENT_RAWBANKREADOUTSTATUSFILTER_H 1

#include <string>

#include "Event/RawBank.h"
#include "Event/RawBankReadoutStatus.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;

/** @class RawBankReadoutStatusFilter RawBankReadoutStatusFilter.h component/RawBankReadoutStatusFilter.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2007-12-05
 */
class RawBankReadoutStatusFilter : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawBankReadoutStatusFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawBankReadoutStatusFilter( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:
  int m_type;
  long m_mask;
  bool m_invert;
};
#endif // COMPONENT_RAWBANKREADOUTSTATUSFILTER_H
