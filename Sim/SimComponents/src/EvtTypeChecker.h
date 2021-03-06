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
#ifndef EVTTYPECHECKER_H
#define EVTTYPECHECKER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// from LHCb
#include "Kernel/IEvtTypeSvc.h"

struct IMCDecayFinder;

/** @class EvtTypeChecker EvtTypeChecker.h
 *  Algorithm that verifies if at least one decay of the desired type
 *  is present in the event and counts for how many this occurs.
 *
 *  @author Gloria CORTI
 *  @date   2005-08-08
 */
class EvtTypeChecker : public GaudiAlgorithm {
public:
  /// Standard constructor
  EvtTypeChecker( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

protected:
  /** Set the description of the decay to be found
   *  @param evtCode 7-digits code to identify event types
   */
  StatusCode setDecayToFind( const int evtCode );

  //   IMCDecayFinder* decayFinder();      ///< Method to return pointer to tool

  //   int decayCode();                    ///< Method returning decay code used

private:
  bool            m_fromData;   ///< flag read event code from data
  int             m_evtCode;    ///< event code to test
  unsigned int    m_nEvents;    ///< Total number of events processed
  unsigned int    m_nMCFound;   ///< Total number of decays found
  bool            m_setDecay;   ///< Flag is decay has been set
  IEvtTypeSvc*    m_evtTypeSvc; ///< Pointer to service
  IMCDecayFinder* m_mcFinder;   ///< Pointer to tool
};
#endif // EVTTYPECHECKER_H
