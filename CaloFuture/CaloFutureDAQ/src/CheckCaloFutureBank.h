#ifndef   CHECKCALOFUTUREBANK_H
#define   CHECKCALOFUTUREBANK_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
// from Event
#include "Event/RawEvent.h"

/** @class CheckCaloFutureBank CheckCaloFutureBank.h
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */

class CheckCaloFutureBank : public GaudiAlgorithm {
public:

  CheckCaloFutureBank( const std::string& name, ISvcLocator* pSvcLocator);
  StatusCode initialize() override;
  StatusCode execute   () override;


private:
  LHCb::RawBank::BankType m_trig;
  LHCb::RawBank::BankType m_bank;
  std::string m_typ;

  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;

};
#endif //    CHECKCALOFUTUREBANK_H
