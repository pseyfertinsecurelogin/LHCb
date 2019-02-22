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
// ============================================================================
#ifndef LOKI_L0CUTS_H
#define LOKI_L0CUTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/L0.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @typedef L0_CHANNEL
     *  Accessor to check the channel decision
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::vector<std::string>& channels = ... ;
     *
     *   const L0_CHANNEL d1 = L0_CHANNEL (  1             , 0 ) ;
     *   const L0_CHANNEL d2 = L0_CHANNEL ( 'channel name' , 0 ) ;
     *   const L0_CHANNEL d2 = L0_CHANNEL (  channels      , 0 ) ;
     *
     *   const bool dec1 = d1 ( l0 ) ;
     *   const bool dec2 = d2 ( l0 ) ;
     *   const bool dec3 = d3 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::L0_CHDECISION
     *  @see LoKi::L0::ChannelDecision
     *  @see LHCb::L0DUReport::channelDecision
     *  @see LHCb::L0DUReport::channelDecisionByName
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_CHANNEL = LoKi::L0::ChannelDecision;
    // ========================================================================
    /** @typedef L0_CHDECISION
     *  Accessor to check the channel decision
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::vector<std::string>& channels = ... ;
     *
     *   const L0_CHDECISION d1 = L0_CHDECISION (  1             , 0 ) ;
     *   const L0_CHDECISION d2 = L0_CHDECISION ( 'channel name' , 0 ) ;
     *   const L0_CHDECISION d3 = L0_CHDECISION (  channels      , 0 ) ;
     *
     *   const bool dec1 = d1 ( l0 ) ;
     *   const bool dec2 = d2 ( l0 ) ;
     *   const bool dec3 = d3 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::L0_CHANNEL
     *  @see LoKi::L0::ChannelDecision
     *  @see LHCb::L0DUReport::channelDecision
     *  @see LHCb::L0DUReport::channelDecisionByName
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_CHDECISION = LoKi::L0::ChannelDecision;
    // ========================================================================
    /** @typedef L0_CHPREDECISION
     *  Accessor to check the channel PRE-decision
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::vector<std::string>& channels = ... ;
     *
     *   const L0_CHPREDECISION d1 = L0_CHPREDECISION (  1             , 0 ) ;
     *   const L0_CHPREDECISION d2 = L0_CHPREDECISION ( 'channel name' , 0 ) ;
     *   const L0_CHPREDECISION d2 = L0_CHPREDECISION (  channels      , 0 ) ;
     *
     *   const bool dec1 = d1 ( l0 ) ;
     *   const bool dec2 = d2 ( l0 ) ;
     *   const bool dec3 = d3 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::ChannelPreDecision
     *  @see LHCb::L0DUReport::channelPreDecision
     *  @see LHCb::L0DUReport::channelPreDecisionByName
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_CHPREDECISION = LoKi::L0::ChannelPreDecision;
    // ========================================================================
    /** @typedef L0_CONDITION
     *  Accessor to check the condition value
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::vector<std::string>& conditions = ... ;
     *
     *   const L0_CONDITION c1 = L0_CONDITION (  1               , 0 ) ;
     *   const L0_CONDITION c2 = L0_CONDIITON ( 'condiiton name' , 0 ) ;
     *   const L0_CONDITION c3 = L0_CONDITION (  conditions      , 0 ) ;
     *
     *   const bool con1 = c1 ( l0 ) ;
     *   const bool con2 = c2 ( l0 ) ;
     *   const bool con3 = c3 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::ConditionValue
     *  @see LHCb::L0DUReport::conditionValue
     *  @see LHCb::L0DUReport::conditionValueByName
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_CONDITION = LoKi::L0::ConditionValue;
    // ========================================================================
    /** @typedef L0_DATA
     *  Simple accessor to 'dataValue' for L0
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::string& name = ... ;
     *
     *   const auto data = L0_DATA ( name  ) ;
     *
     *   double value = data ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::DataValue
     *  @see LHCb::L0DUReport::dataValue
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_DATA = LoKi::L0::DataValue;
    // ========================================================================
    /** @typedef L0_DECISION
     *  Simple accessor to LHCb::L0DUReport::decision
     *
     *  @code
     *
     *   const L0_DECISION decision = L0_DECISION ( 4) ;
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool ok = decision ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::Decision
     *  @see LHCb::L0DUReport::decision
     *  @see LHCb::L0DUDecision
     *  @see LoKi::Cuts::L0_DECISION_PHYSICS
     *  @see LoKi::Cuts::L0_DECISION_BEAM1
     *  @see LoKi::Cuts::L0_DECISION_BEAM2
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_DECISION = LoKi::L0::Decision;
    // ========================================================================
    /** @var L0_DECISION_BEAM1
     *  Simple accessor to LHCb::L0DUReport::decision
     *
     *  @code
     *
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool ok = L0_DECISION_BEAM1 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::Decision
     *  @see LoKi::Cuts::L0_DECISION
     *  @see LoKi::Cuts::L0_DECISION_PHYSICS
     *  @see LoKi::Cuts::L0_DECISION_BEAM2
     *  @see LHCb::L0DUReport::decision
     *  @see LHCb::L0DUDecision
     *  @see LHCb::L0DUDecision::Beam1
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_DECISION_BEAM1 = LoKi::L0::Decision{LHCb::L0DUDecision::Beam1};
    // ========================================================================
    /** @var L0_DECISION_BEAM2
     *  Simple accessor to LHCb::L0DUReport::decision
     *
     *  @code
     *
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool ok = L0_DECISION_BEAM2 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::Decision
     *  @see LoKi::Cuts::L0_DECISION
     *  @see LoKi::Cuts::L0_DECISION_PHYSICS
     *  @see LoKi::Cuts::L0_DECISION_BEAM1
     *  @see LHCb::L0DUReport::decision
     *  @see LHCb::L0DUDecision
     *  @see LHCb::L0DUDecision::Beam2
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_DECISION_BEAM2 = LoKi::L0::Decision{LHCb::L0DUDecision::Beam2};
    // ========================================================================
    /** @var L0_DECISION_PHYSICS
     *  Simple accessor to LHCb::L0DUReport::decision
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool ok = L0_DECISION_PHYSICS ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::Decision
     *  @see LoKi::Cuts::L0_DECISION
     *  @see LoKi::Cuts::L0_DECISION_BEAM1
     *  @see LoKi::Cuts::L0_DECISION_BEAM2
     *  @see LHCb::L0DUReport::decision
     *  @see LHCb::L0DUDecision
     *  @see LHCb::L0DUDecision::Physics
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_DECISION_PHYSICS = LoKi::L0::Decision{LHCb::L0DUDecision::Physics};
    // ========================================================================
    /** @typedef L0_DIGIT
     *  Simple accessor to 'dataDigit' for L0
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::string& name = ... ;
     *
     *   const L0_DIGIT digit = L0_DIGIT ( name  ) ;
     *
     *   double value = digit  ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::DataDigit
     *  @see LHCb::L0DUReport::dataDigit
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_DIGIT = LoKi::L0::DataDigit;
    // ========================================================================
    /** @var L0_FORCEBIT
     *  Simple accessor to 'force bit'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool force = L0_FORCEBIT( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::ForceBit
     *  @see LHCb::L0DUReport::forceBit
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_FORCEBIT = LoKi::L0::ForceBit{};
    // ========================================================================
    /** @typedef L0_SUMET
     *  Simple accessor to 'sum-et' by L0
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const L0_SUMET sumEt = L0_SUMET( 0 ) ;
     *
     *   double energy = sumEt( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::L0_SUMET0
     *  @see LoKi::L0::SumEt
     *  @see LHCb::L0DUReport::sumEt
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_SUMET = LoKi::L0::SumEt;
    // ========================================================================
    /** @var L0_SUMET0
     *  Simple accessor to 'sum-et' by L0 in bx=0
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   double energy = L0_SUMET0 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::L0_SUMET
     *  @see LoKi::L0::SumEt
     *  @see LHCb::L0DUReport::sumEt
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_SUMET0 = LoKi::L0::SumEt{};
    // ========================================================================
    /** @typedef L0_SUMDECISION
     *  Simple accessor to 'decison form summary'
     *
     *  @code
     *
     *   const L0_SUMDECISION  dec = L0SUMDECISON ( mask , bx ) ;
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool decision = dec ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::SumDecision
     *  @see LoKi::Cuts::L0_SUMDECISION_PHYSICS
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM1
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM2
     *  @see LHCb::L0DUReport::decisionFromSummary
     *  @see LHCb::L0DUDecision::Type
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_SUMDECISION = LoKi::L0::SumDecision;
    // ========================================================================
    /** @var L0_SUMDECISION_BEAM1
     *  Simple accessor to 'decison from summary'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool decision = L0_SUMDECISION_BEAM1 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::SumDecision
     *  @see LoKi::Cuts::L0_SUMDECISION
     *  @see LoKi::Cuts::L0_SUMDECISION_PHYSICS
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM2
     *  @see LHCb::L0DUReport::decisionFromSummary
     *  @see LHCb::L0DUDecision::Beam1
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_SUMDECISION_BEAM1 = LoKi::L0::SumDecision{LHCb::L0DUDecision::Beam1};
    // ========================================================================
    /** @var L0_SUMDECISION_BEAM2
     *  Simple accessor to 'decison from summary'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool decision = L0_SUMDECISION_BEAM2 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::SumDecision
     *  @see LoKi::Cuts::L0_SUMDECISION
     *  @see LoKi::Cuts::L0_SUMDECISION_PHYSICS
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM1
     *  @see LHCb::L0DUReport::decisionFromSummary
     *  @see LHCb::L0DUDecision::Beam2
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_SUMDECISION_BEAM2 = LoKi::L0::SumDecision{LHCb::L0DUDecision::Beam2};
    // ========================================================================
    /** @var L0_SUMDECISION_PHYSICS
     *  Simple accessor to 'decison from summary'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool decision = L0_SUMDECISION_PHYSICS ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::SumDecision
     *  @see LoKi::Cuts::L0_SUMDECISION
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM1
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM2
     *  @see LHCb::L0DUReport::decisionFromSummary
     *  @see LHCb::L0DUDecision::Physics
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_SUMDECISION_PHYSICS = LoKi::L0::SumDecision{LHCb::L0DUDecision::Physics};
    // ========================================================================
    /** @var L0_TCK
     *  Simple accessor to 'trigger configuration key'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   double tck = L0_TCK ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::Tck
     *  @see LHCb::L0DUReport::tck
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_TCK = LoKi::L0::Tck{};
    // ========================================================================
    /** @var L0_TIMING
     *  Simple accessor to 'timing bit'
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   bool timingBit = L0_TIMINGBIT ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::TimingBit
     *  @see LHCb::L0DUReport::timingTriggerBit
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    inline const auto L0_TIMINGBIT = LoKi::L0::TimingBit{};
    // ========================================================================
    /** @typedef L0_TRIGGER
     *  Accessor to check the trigger decision
     *
     *  @code
     *
     *   const LHCb::L0DUReport* l0 = ... ;
     *
     *   const std::vector<std::string>& triggers = ... ;
     *
     *   const L0_TRIGGER t1 = L0_TRIGGER (  1             , 0 ) ;
     *   const L0_TRIGGER t2 = L0_TRIGGER ( 'trigger name' , 0 ) ;
     *   const L0_TRIGGER t3 = L0_TRIGGER (  triggers      , 0 ) ;
     *
     *   const bool trig1 = t1 ( l0 ) ;
     *   const bool trig2 = t2 ( l0 ) ;
     *   const bool trig3 = t3 ( l0 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::L0::TriggerDecision
     *  @see LHCb::L0DUReport::triggerDecision
     *  @see LHCb::L0DUReport::triggerDecisionByName
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    using L0_TRIGGER = LoKi::L0::TriggerDecision;
    // ========================================================================
    /** @var L0_VALID
     *  Tricial functor to check th wvalidity of L0DUReport object.
     *  Unlike all other L0-functors, is also accepts zero pointer.
     *  It acts according the rule:
     *  @code
     *     return 0 != l0 && l0->valid()
     *  @endcode
     *
     *  @code
     *
     *    const LHCb::L0DUReport* l0 = ... ;
     *
     *    const bool valid = L0_VALID ( l0 ) ;
     *
     *  @code
     *
     *  @see LoKi::L0::Valid
     *  @see LHCb::L0DUReport
     *  @see LHCb::L0DUReport::valid
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-08
     */
    inline const auto L0_VALID = LoKi::L0::Valid{};
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_L0CUTS_H
