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
#ifndef L0PATTERN_H
#define L0PATTERN_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiTupleAlg.h"
// Interface
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0DUEmulatorTool.h"
#include "L0Interfaces/IL0DUFromRawTool.h"

/** @class L0Pattern L0Pattern.h
 *  L0 Processors (L0Calo/L0Muon/PUVETO) -> L0DU
 *  bit assignment from EDMS 528259
 *
 *  @author Olivier Deschamps
 *  @date   2005-01-28
 */
class L0Pattern : public GaudiTupleAlg{
public:
  /// Standard constructor
  using GaudiTupleAlg::GaudiTupleAlg;

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  void encode(unsigned int data ,  const std::array<unsigned int, L0DUBase::Index::Size> &base);


  // Tools
  IL0DUConfigProvider* m_config = nullptr;
  IL0DUEmulatorTool*   m_emulator = nullptr;
  IL0DUFromRawTool*    m_fromRaw = nullptr;
  LHCb::L0ProcessorDatas* m_datas = nullptr;
  unsigned long m_bcid = 0;
  //
  Gaudi::Property<std::string> m_emulatorTool
  { this, "L0DUEmulatorTool"  ,"L0DUEmulatorTool" };
  Gaudi::Property<std::string> m_fromRawTool
  { this, "L0DUFromRawTool"   , "L0DUFromRawTool" };
  Gaudi::Property<std::string> m_configTool
  { this, "L0DUConfigTool"    , "L0PatternConfig" };
  Gaudi::Property<std::vector<std::string>> m_list
  { this, "TCKList"           };
  Gaudi::Property<bool> m_setbcid
  { this, "resetBCID", true};


};
#endif // L0PATTERN_H
