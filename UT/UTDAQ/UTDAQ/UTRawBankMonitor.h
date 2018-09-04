#ifndef _UTRawBankMonitor_H
#define _UTRawBankMonitor_H

#include <string>

#include "Event/RawBank.h"

// base class
#include "Kernel/UTHistoAlgBase.h"

/** @class UTRawBankMonitor UTRawBankMonitor.h
 *  UTCheckers/UTRawBankMonitor.h
 *
 *  Class for checking UT RAW buffer
 *
 *  @author M.Needham
 *  @date   21/11/2005
 */

class UTTell1ID;

class UTRawBankMonitor : public UT::HistoAlgBase{

public:

  /// constructor
  using UT::HistoAlgBase::HistoAlgBase;

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

#endif // _UTRawBankMonitor














