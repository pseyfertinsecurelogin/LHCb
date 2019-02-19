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
#ifndef LHCB_RAWEVENTMAPCOMBINER_H
#define LHCB_RAWEVENTMAPCOMBINER_H 1

#include "Event/RawBank.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/ISvcLocator.h"

#include <map>
#include <string>

/** @class RawEventMapCombiner RawEventMapCombiner.h
 *  Combines disparate raw events into one new location
 *  Based on RawEventSelectiveCopy from Tomasz
 *  Use if the split locations possibly hold duplicates of certain raw banks
 *
 *  @author Rob Lambert
 *  @date   2013-04-03
 */
class RawEventMapCombiner : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawEventMapCombiner( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution

private:
  std::map<std::string, std::string> m_banksToCopy;    // which banks to recombine, from where, (bank, location)
  std::string                        m_outputLocation; // where to put them, DEFAULT /Event/RawEvent

  std::map<LHCb::RawBank::BankType, std::string> m_bankTypes; // translation of m_banksToCopy
};

#endif // LHCB_RAWEVENTMAPCOMBINER_H
