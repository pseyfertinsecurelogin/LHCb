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
#ifndef HLTCOMMON_HLTROUTINGBITSWRITER_H
#define HLTCOMMON_HLTROUTINGBITSWRITER_H 1

// Include files
#include <array>

// boost
#include <boost/variant.hpp>

// from Gaudi
#include "DetDesc/Condition.h"
#include "GaudiAlg/GaudiHistoAlg.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IUpdateManagerSvc.h"

#include "Kernel/IHltMonitorSvc.h"

#include "LoKi/HLTTypes.h"
#include "LoKi/L0Types.h"
#include "LoKi/OdinTypes.h"

#include "Event/RawEvent.h"
#include "HltDAQ/HltEvaluator.h"

/** @class HltRoutingBitsWriter HltRoutingBitsWriter.h
 *
 *
 *  @author Gerhard Raven
 *  @date   2008-07-29
 */
class HltRoutingBitsWriter : public HltEvaluator {
public:
  /// Standard constructor
  using HltEvaluator::HltEvaluator;

  StatusCode execute() override; ///< Algorithm execution

private:
  /// Decode
  StatusCode decode() override;

  void zeroEvaluators();

  std::unordered_map<unsigned int, EvalVariant> m_evaluators;
  Gaudi::Property<bool>                         m_updateBank{this, "UpdateExistingRawBank", false};
  Gaudi::Property<std::string> m_raw_location{this, "RawEventLocation", LHCb::RawEventLocation::Default};
  Gaudi::Property<std::map<unsigned int, std::string>> m_bits{
      this, "RoutingBits", {}, [=]( auto& ) {
        /// mark as "to-be-updated"
        m_evals_updated = true;
        // no action if not yet initialized
        if ( Gaudi::StateMachine::INITIALIZED > FSMState() ) return;
        // postpone the action
        if ( !m_preambulo_updated ) { return; }
        // perform the actual immediate decoding
        StatusCode sc = decode();
        Assert( sc.isFailure(), "Error from HltRoutingBitsWriter::decode()", sc );
      }};
};
#endif // HLTROUTINGBITSWRITER_H
