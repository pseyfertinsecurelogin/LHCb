#ifndef STDIGITSTOSTTELL1DATA_H
#define STDIGITSTOSTTELL1DATA_H 1

#include "Kernel/STAlgBase.h"
#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/STTELL1Data.h"
#include "Event/STDigit.h"

#include <vector>
#include <string>

/** @class RawBankToSTProcFull RawBankToSTProcFull.h
 *
 *  Algorithm to create STTELL1Data (type ProcFull) from RawEvent object
 *
 *  @author M. Needham
 *  @date   2007-10-10
 */

struct ISTReadoutTool;
class DeSTDetector;

class STDigitsToSTTELL1Data: public ST::AlgBase {

public:

  /// Standard constructor
  STDigitsToSTTELL1Data( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override;    ///< Algorithm execution

private:

  StatusCode createTell1Data(const LHCb::STDigits* digits,
                             LHCb::STTELL1Datas* outCont) const;


  std::string m_inputLocation;
  std::string m_outputLocation;


};

#endif // STDigitsToSTTELL1Data_H
