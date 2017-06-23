#ifndef _STRawBankMonitor_H
#define _STRawBankMonitor_H

#include <string>

#include "Event/RawBank.h"

// base class
#include "Kernel/STHistoAlgBase.h"

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

  /// constructor
  using ST::HistoAlgBase::HistoAlgBase;

  /// initialize
  StatusCode initialize() override;

  /// execute
  StatusCode execute() override;

private:

  StatusCode configureBankType();

  LHCb::RawBank::BankType m_bankType = LHCb::RawBank::LastType;

  /// List of locations in the transient store to search the RawEvent object.
  Gaudi::Property<std::vector<std::string>> m_rawEventLocations
  { this, "RawEventLocations", { },
                   "List of possible locations of the RawEvent object in the"
                   " transient store. By default it is LHCb::RawEventLocation::Other,"
                   " LHCb::RawEventLocation::Default." };

};

#endif // _STRawBankMonitor














