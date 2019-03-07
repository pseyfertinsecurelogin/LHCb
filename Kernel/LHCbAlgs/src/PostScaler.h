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
#ifndef POSTSCALER_H
#define POSTSCALER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Property.h"
#include "Kernel/IEventCounter.h"

/** @class PostScaler PostScaler.h
 *  Allows to prescale events at a given rate suing a random number (as Prescaler.cpp)
 *  or to force the fraction of accepted events to be as given by options (or both).
 *
 *  @author P. Koppenburg
 *  @date   2004-06-15
 */

class PostScaler final : public GaudiAlgorithm {

public:
  /// Standard constructor
  PostScaler( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~PostScaler() = default; ///< Destructor

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  unsigned long long m_nEvents{0};    ///< Counter of events accepted
  unsigned long long m_nEventsAll{0}; ///< Counter of events entering
#ifdef GAUDI_PROPERTY_v2
  Gaudi::CheckedProperty<double>
#else
  DoubleProperty
#endif
                 m_percentPass;     ///< Minimal reduction rate to achieve (statistics mode)
  double         m_forcedReduction; ///< Percentage of events that should be passed (random number mode)
  double         m_margin;          ///< Safety margin (accept if acc events < m_event/m_forcedReduction + m_margin)
  IEventCounter* m_eventCounter = nullptr;
};

#endif // CMT_POSTSCALER_H
