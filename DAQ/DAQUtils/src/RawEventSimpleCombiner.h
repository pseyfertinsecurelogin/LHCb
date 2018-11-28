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
// $Id: RawEventCombiner.h,v 1.1 2009/06/22 15:12:04 tskwarni Exp $
#ifndef LHCB_RAWEVENTSIMPLECOMBINER_H
#define LHCB_RAWEVENTSIMPLECOMBINER_H 1

#include "Event/RawBank.h"

#include "GaudiAlg/GaudiAlgorithm.h"

#include <vector>
#include <string>

/** @class RawEventSimpleCombiner RawEventSimpleCombiner.h
 *  Combines disparate raw events into one new location
 *  Based on RawEventSelectiveCopy from Tomasz
 *  Use if the split locations hold no raw event bank duplicates
 *
 *  @author Rob Lambert
 *  @date   2013-04-03
 *
 *  2018-07-06 Added functionality that allows to add raw events to an existing one @rcenci  
 */
class RawEventSimpleCombiner : public GaudiAlgorithm {
public:
  /// Standard constructor
  RawEventSimpleCombiner( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~RawEventSimpleCombiner( );     ///< Destructor
  StatusCode initialize() override;      ///< Algorithm initialization
  StatusCode execute() override;         ///< Algorithm execution

protected:

private:

  std::vector<std::string> m_banksToCopy; //which banks to recombine, default ALL
  std::vector<std::string> m_inputLocations; //get the banks from where
  std::string              m_outputLocation; //where to put them, DEFAULT /Event/RawEvent

  std::vector<LHCb::RawBank::BankType> m_bankTypes;

  Gaudi::Property<bool> m_enableIncrementalMode{ this, "EnableIncrementalMode",false, 
      "Enable incremental mode, where raw events are added to output raw event (already created)"};

};


#endif // TESTS_RAWEVENTSIMPLECOMBINER_H
