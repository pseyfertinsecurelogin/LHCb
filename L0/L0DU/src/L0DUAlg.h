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
#ifndef L0DUAlg_H
#define L0DUAlg_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
//Event
#include "Event/RawEvent.h"
//Base
#include "L0Base/L0AlgBase.h"
// Interface
#include "L0Interfaces/IL0DUConfigProvider.h"
#include "L0Interfaces/IL0DUEmulatorTool.h"

/** @class L0DUAlg L0DUAlg.h
 *  L0DUAlg report
 *
 *
 *  @author Olivier Deschamps
 *  @date   2005-01-28
 */



class L0DUAlg : public L0AlgBase {
public:
  /// Standard constructor
  L0DUAlg( const std::string& name, ISvcLocator* pSvcLocator );
  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution

private:
  // I/O Locations
  std::vector<std::string> m_dataLocations;
  std::vector<std::string> m_locs;

  // parameters
  unsigned int m_rawSrcID;
  std::string m_emulatorType;
  std::string m_reportLocation;
  std::string m_tck;
  std::string m_configName;
  std::string m_configType;

//
  unsigned int m_rawVsn ;
  LHCb::RawBank::BankType m_rawBankType;

  // Configuration
  LHCb::L0DUConfig* m_config;

  // Tools
  IL0DUConfigProvider* m_confTool;
  IL0DUEmulatorTool*   m_emulator;

  // PGA3 block building
  std::map<std::string,double> m_muonMap;
  bool m_addHC;
};
#endif // L0DUAlg_H
