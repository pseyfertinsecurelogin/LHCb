#ifndef UTDIGITUTOSTTELL1DATA_H
#define UTDIGITUTOSTTELL1DATA_H 1

#include "Kernel/UTAlgBase.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/UTTELL1Data.h"
#include "Event/UTDigit.h"

#include <vector>
#include <string>

/** @class RawBankToUTProcFull RawBankToUTProcFull.h
 *
 *  Algorithm to create UTTELL1Data (type ProcFull) from RawEvent object
 *
 *  @author A. Beiter (based on code by M. Needham)
 *  @date   2018-09-04
 */

struct IUTReadoutTool;
class DeUTDetector;

class UTDigitsToUTTELL1Data: public UT::AlgBase {

public:

  /// Standard constructor
  UTDigitsToUTTELL1Data( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:

  StatusCode createTell1Data(const LHCb::UTDigits* digits,
                             LHCb::UTTELL1Datas* outCont) const;


  std::string m_inputLocation;
  std::string m_outputLocation;


};

#endif // UTDigitsToUTTELL1Data_H
