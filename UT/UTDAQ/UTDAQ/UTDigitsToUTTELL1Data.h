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
#ifndef UTDIGITUTOSTTELL1DATA_H
#define UTDIGITUTOSTTELL1DATA_H 1

#include "Event/RawBank.h"
#include "Event/RawEvent.h"
#include "Event/UTDigit.h"
#include "Event/UTTELL1Data.h"
#include "Kernel/UTAlgBase.h"

#include <string>
#include <vector>

/** @class RawBankToUTProcFull RawBankToUTProcFull.h
 *
 *  Algorithm to create UTTELL1Data (type ProcFull) from RawEvent object
 *
 *  @author A. Beiter (based on code by M. Needham)
 *  @date   2018-09-04
 */

struct IUTReadoutTool;
class DeUTDetector;

class UTDigitsToUTTELL1Data : public UT::AlgBase {

public:
  /// Standard constructor
  UTDigitsToUTTELL1Data( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode execute() override; ///< Algorithm execution

private:
  StatusCode createTell1Data( const LHCb::UTDigits* digits, LHCb::UTTELL1Datas* outCont ) const;

  std::string m_inputLocation;
  std::string m_outputLocation;
};

#endif // UTDigitsToUTTELL1Data_H
