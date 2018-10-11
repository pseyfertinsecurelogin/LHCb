/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#ifndef TESTS_RAWEVENTDUMP_H
#define TESTS_RAWEVENTDUMP_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "DAQKernel/DecoderAlgBase.h"


/** @class RawEventDump RawEventDump.h tests/RawEventDump.h
 *  Dump a RawEvent
 *
 *  @author Markus Frank
 *  @date   2005-10-13
 */
class RawEventDump : public Decoder::AlgBase {
public:
  /// Standard constructor
  RawEventDump( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute   () override;    ///< Algorithm execution

private:
  bool acceptBank(LHCb::RawBank::BankType i);
  Gaudi::Property<bool> m_dump{ this, "DumpData", false };///< Property "DumpData". If true, full bank contents are dumped
  Gaudi::Property<std::vector<std::string>> m_banks{ this, "RawBanks" }; // RawBanks to be dumped  (default ALL banks)
};
#endif // TESTS_RAWEVENTDUMP_H
