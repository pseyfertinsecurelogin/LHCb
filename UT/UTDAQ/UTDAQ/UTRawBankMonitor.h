/*****************************************************************************\
* (c) Copyright 2000-2018 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

class UTTell1ID;

class UTRawBankMonitor : public UT::HistoAlgBase {

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
  Gaudi::Property<std::vector<std::string>> m_rawEventLocations{
      this,
      "RawEventLocations",
      {},
      "List of possible locations of the RawEvent object in the"
      " transient store. By default it is LHCb::RawEventLocation::Other,"
      " LHCb::RawEventLocation::Default."};
};

#endif // _UTRawBankMonitor
