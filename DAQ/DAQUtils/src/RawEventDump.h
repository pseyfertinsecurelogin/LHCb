// $Id: RawEventDump.h,v 1.1 2005/10/14 12:48:50 cattanem Exp $
#ifndef TESTS_RAWEVENTDUMP_H
#define TESTS_RAWEVENTDUMP_H 1

#include <string>
#include <vector>

#include "DAQKernel/DecoderAlgBase.h"
#include "Event/RawBank.h"
// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/StatusCode.h"

class ISvcLocator;


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

  virtual ~RawEventDump( ); ///< Destructor

  StatusCode execute   () override;    ///< Algorithm execution

protected:

private:
  bool acceptBank(LHCb::RawBank::BankType i);
  bool m_dump; ///< Property "DumpData". If true, full bank contents are dumped
  std::vector<std::string> m_banks; // RawBanks to be dumped  (default ALL banks)
  //std::vector<std::string> m_rawEventLocations;

};
#endif // TESTS_RAWEVENTDUMP_H
