#ifndef _STRawBankMonitor_H
#define _STRawBankMonitor_H

#include <string>
#include <vector>

#include "Event/RawBank.h"
#include "GaudiKernel/StatusCode.h"
// base class
#include "Kernel/STHistoAlgBase.h"

class ISvcLocator;

/** @class STRawBankMonitor STRawBankMonitor.h
 * ITCheckers/STRawBankMonitor.h
 *
 *  Class for checking ST RAW buffer
 *
 *  @author M.Needham
 *  @date   21/11/2005
 */

class STTell1ID;

class STRawBankMonitor : public ST::HistoAlgBase{

public:

  /// constructer
  STRawBankMonitor(const std::string& name,
                   ISvcLocator *svcloc );

  /// destructer
  virtual ~STRawBankMonitor();

  /// initialize
  StatusCode initialize() override;

  /// execute
  StatusCode execute() override;

private:

  StatusCode configureBankType();

  LHCb::RawBank::BankType m_bankType;

  /// List of locations in the transient store to search the RawEvent object.
  std::vector<std::string> m_rawEventLocations;

};

#endif // _STRawBankMonitor














