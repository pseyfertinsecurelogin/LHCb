#ifndef   CHECKCALOBANK_H
#define   CHECKCALOBANK_H 1

#include <string>
#include <vector>

#include "Event/RawBank.h"
// from Event
#include "Event/RawEvent.h"
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;
template <class T> class AlgFactory;

/** @class CheckCaloBank CheckCaloBank.h
 *
 *    @author: Olivier Deschamps
 *    @date:   O6 Septembre 2006
 */

class CheckCaloBank : public GaudiAlgorithm {
  friend class AlgFactory<CheckCaloBank>;
 public:

  CheckCaloBank( const std::string& name, ISvcLocator* pSvcLocator);

  StatusCode initialize() override;
  StatusCode execute   () override;

 private:
  LHCb::RawBank::BankType m_trig;
  LHCb::RawBank::BankType m_bank;
  std::string m_typ;

  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;

};
#endif //    CHECKCALOBANK_H
