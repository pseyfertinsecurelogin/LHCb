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
#ifndef LOKI_L0_H
#define LOKI_L0_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
// ============================================================================
// L0Event
// ============================================================================
#include "Event/L0DUReport.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/regex.hpp"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::L0
   *  The namespace to keep all L0-related LoKi functors
   *
   *  @attention All functors, but L0_VALID,
   *    throws the exception for zero pointer to L0DUReport Objects
   *
   *  @see LHCb::L0DUReport
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-09-19
   */
  namespace L0
  {
    // ========================================================================
    /** @class Valid
     *  Simple functor to check the validity of L0DUReport object.
     *  Unlike other functors is also accept the zero pointer.
     *
     *  @see LHCb::L0DUReport
     *  @see LHCb::L0DUReport::valid
     *  @see LoKi::Cuts::L0_VALID
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-01-08
     */
    struct GAUDI_API Valid final
      : LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Valid* clone () const override;
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class SumEt
     *  @see LHCb::L0DUReport
     *  @see LHCb::L0DUReport::sumEt
     *  @see LoKi::Cuts::L0_SUMET
     *  @see LoKi::Cuts::L0_SUMET0
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API SumEt final
      : public LoKi::BasicFunctors<const LHCb::L0DUReport*>::Function
    {
    public:
      // ======================================================================
      /// constructor from bx-id
      SumEt ( const int bx = 0  ) ;
      /// MANDATORY: clone method ("virtual constructor")
      SumEt* clone () const override { return new SumEt ( *this ) ; }
      // ======================================================================
      /// MANDATORY: the only one essential method
      double operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// BX-id
      int m_bx ;                                                       // BX-id
      // ======================================================================
    };
    // ========================================================================
    /** @class DataValue
     *  Simple accessor to the data value
     *  @see LHCb::L0DUReport
     *  @see LHCb::L0DUReport::dataValue
     *  @see LoKi::Cuts::L0_DATA
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API DataValue
      : public LoKi::BasicFunctors<const LHCb::L0DUReport*>::Function
    {
    public:
      // ======================================================================
      /// constructor form the data name
      DataValue ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DataValue* clone() const override { return new DataValue ( *this ) ; }
      /// MANDATORY: the only one essential method
      double operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
      const std::string& name() const { return m_name ; }
      // ======================================================================
    private:
      // ======================================================================
      /// data name
      std::string m_name ;                                         // data name
      // ======================================================================
    };
    // ========================================================================
    /** @class DataDigit
     *  Simple accessor to the data digit
     *  @see LHCb::L0DUReport
     *  @see LHCb::L0DUReport::dataDigit
     *  @see LoKi::Cuts::L0_DIGIT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API DataDigit final : DataValue
    {
      // ======================================================================
      /// constructor form the data name
      DataDigit ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      DataDigit* clone() const override { return new DataDigit ( *this ) ; }
      /// MANDATORY: the only one essential method
      double operator() ( const LHCb::L0DUReport* a ) const override ;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    };
    // ========================================================================
    /** @class SumDecision
     *  check the decision from the summary
     *  @see LHCb::L0DUReport::decisionFromSummary
     *  @see LoKi::Cuts::L0_SUMDECISION
     *  @see LoKi::Cuts::L0_SUMDECISION_PHYSICS
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM1
     *  @see LoKi::Cuts::L0_SUMDECISION_BEAM2
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API SumDecision final
      : public LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from decision mask
      SumDecision ( const int mask , const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      SumDecision* clone () const override
      { return new SumDecision ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the decision type
      int m_mask ; //  the decision type
      /// ?
      int m_bx   ;  // ?
      // ======================================================================
    };
    // ========================================================================
    /** @class Decision
     *  check the L0-decision
     *  @see LHCb::L0DUReport::decision
     *  @see LoKi::Cuts::L0_DECISION
     *  @see LoKi::Cuts::L0_DECISION_PHYSICS
     *  @see LoKi::Cuts::L0_DECISION_BEAM1
     *  @see LoKi::Cuts::L0_DECISION_BEAM2
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API Decision final
      : public LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from decision mask
      Decision ( const int mask ) ;
      /// MANDATORY: clone method ('virtual constructor')
      Decision* clone () const override;
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the decision mask
      int m_mask ;                                         // the decision mask
      // ======================================================================
    };
    // ========================================================================
    /** @class ForceBit
     *  check the force-bit
     *  @see LHCb::L0DUReport::forceBit
     *  @see LoKi::Cuts::L0_FORCEBIT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API ForceBit final
      : LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
      // ======================================================================
      /// MANDATORY: clone method ('virtual constructor')
      ForceBit* clone () const override
      { return new ForceBit ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Tck
     *  get the trigger configuration key
     *  @see LHCb::L0DUReport::tck
     *  @see LoKi::Cuts::L0_TCK
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API Tck final
      : LoKi::BasicFunctors<const LHCb::L0DUReport*>::Function
    {
      // ======================================================================
      /// MANDATORY: clone method ('virtual constructor')
      Tck* clone () const override { return new Tck ( *this ) ; }
      /// MANDATORY: the only one essential method
      double operator() ( const LHCb::L0DUReport* a ) const override ;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    };
    // ========================================================================
    /** @class TimingBit
     *  check the timing-bit
     *  @see LHCb::L0DUReport::timingTriggerBit
     *  @see LoKi::Cuts::L0_TIMINGBIT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API TimingBit final
      : LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
      // ======================================================================
      /// MANDATORY: clone method ('virtual constructor')
      TimingBit* clone () const override
      { return new TimingBit ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ChannelDecision
     *  Accessor to the channel decision
     *  @see LHCb::L0DUReport::channelDecision
     *  @see LHCb::L0DUReport::channelDecisionNyName
     *  @see LoKi::Cuts::L0_CHDECISION
     *  @see LoKi::Cuts::L0_CHANNEL
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API ChannelDecision
      : public LoKi::BasicFunctors<const LHCb::L0DUReport*>::Predicate
    {
    protected:
      // ======================================================================
      typedef std::vector<std::string>                                  Names ;
      typedef std::vector<unsigned int>                              Channels ;
      // ======================================================================
    public:
      // ======================================================================
      /// channel decision by channel name
      ChannelDecision ( const std::string& channel , const int bx = 0 )
          : ChannelDecision( std::vector<std::string>{ channel }, bx )  {}
      /// channel decision by channel names  ("OR")
      ChannelDecision ( const std::vector<std::string>& channels ,
                        const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      ChannelDecision* clone() const override
      { return new ChannelDecision ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    public:
      // ======================================================================
      const Channels&    channels () const { return m_channels ; }
      const Names&       names    () const { return m_names    ; }
      // ======================================================================
      int  bx   () const { return m_bx   ; }
      // ======================================================================
    protected:
      // ======================================================================
      unsigned int tckPrev   ()                 const { return m_tckPrev   ; }
      void     setTckPrev    ( const int tck  ) const { m_tckPrev = tck    ; }
      void     clearChannels ()                 const { m_channels.clear() ; }
      void     addChannel    ( unsigned int c ) const
      { m_channels.push_back ( c ) ; }
      void     clearNames    ()                 const { m_names.clear() ; }
      void     addName       ( const std::string& c ) const
      { m_names.push_back ( c ) ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the channel names
      mutable Names m_names  ;                             // the channel names
      /// channels
      mutable Channels m_channels ; // the channels
      /// the bx-id
      int          m_bx      ;                                     // the bx-id
      // cached TCK
      mutable unsigned int m_tckPrev ;                           // cached  TCK
      // ======================================================================
    } ;
    // ========================================================================
    /** @class ChannelPreDecision
     *  Accessor to the channel predecision
     *  @see LHCb::L0DUReport::channelPreDecision
     *  @see LHCb::L0DUReport::channelPreDecisionNyName
     *  @see LoKi::Cuts::L0_CHPREDECISION
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API ChannelPreDecision : ChannelDecision
    {
      // ======================================================================
      /// channel decision by channel name
      ChannelPreDecision ( const std::string& channel , const int bx = 0 ) ;
      /// channel decision by channel names  ("OR")
      ChannelPreDecision ( const std::vector<std::string>& channels ,
                           const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      ChannelPreDecision* clone() const override
      { return new ChannelPreDecision ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class TriggerDecision
     *  Accessor to the trigger decision
     *  @see LHCb::L0DUReport::triggerDecision
     *  @see LHCb::L0DUReport::triggerDecisionByName
     *  @see LoKi::Cuts::L0_TRIGGER
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API TriggerDecision : ChannelDecision
    {
      // ======================================================================
      /// channel decision by channel name
      TriggerDecision ( const std::string& channel , const int bx = 0 ) ;
      /// channel decision by channel names  ("OR")
      TriggerDecision ( const std::vector<std::string>& channels ,
                       const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      TriggerDecision* clone() const override
      { return new TriggerDecision ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class ConditionValue
     *  Accessor to the condition value
     *  @see LHCb::L0DUReport::conditionValue
     *  @see LHCb::L0DUReport::conditionValueByName
     *  @see LoKi::Cuts::L0_CONDITION
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API ConditionValue : ChannelDecision
    {
      // ======================================================================
      /// channel decision by channel name
      ConditionValue ( const std::string& channel , const int bx = 0 ) ;
      /// channel decision by channel names  ("OR")
      ConditionValue ( const std::vector<std::string>& channels ,
                       const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      ConditionValue* clone() const override
      { return new ConditionValue ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    class GAUDI_API ChannelDecisionSubString : public ChannelDecision
    {
    public:
      // ======================================================================
      /// constructor from substring
      ChannelDecisionSubString
      ( const std::string& substr     ,
        const int          bx     = 0 ) ;
      /// MANDATORY: clone method ("virtual constructor")
      ChannelDecisionSubString* clone() const override
      { return new ChannelDecisionSubString ( *this ) ; }
      /// MANDATORY: the only one essential methos
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
      const std::string& substr() const { return m_substr ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the substring
      std::string m_substr ;                                   // the substring
      // ======================================================================
    } ;
    // ========================================================================
    class GAUDI_API ChannelDecisionRegex : public ChannelDecisionSubString
    {
    public:
      // ======================================================================
      /// constructor from substring
      ChannelDecisionRegex
      ( const std::string& substr     ,
        const int          bx     = 0 ) ;
      /// MANDATORY: clone method ("virtual constructor")
      ChannelDecisionRegex* clone() const override
      { return new ChannelDecisionRegex ( *this ) ; }
      /// MANDATORY: the only one essential methos
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
      const boost::regex& expression() const { return m_expression ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the regular expression
      boost::regex m_expression ;                     // the regular expression
      // ======================================================================
    } ;
    // ========================================================================
    class GAUDI_API ChannelPreDecisionSubString : public ChannelDecisionSubString
    {
    public:
      // ======================================================================
      /// constructor from substring
      ChannelPreDecisionSubString
      ( const std::string& substr     ,
        const int          bx     = 0 ) ;
      /// MANDATORY: clone method ("virtual constructor")
      ChannelPreDecisionSubString* clone() const override
      { return new ChannelPreDecisionSubString ( *this ) ; }
      /// MANDATORY: the only one essential methos
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    class GAUDI_API ChannelPreDecisionRegex : public ChannelDecisionRegex
    {
    public:
      // ======================================================================
      /// constructor from substring
      ChannelPreDecisionRegex
      ( const std::string& substr     ,
        const int          bx     = 0 ) ;
      /// MANDATORY: clone method ("virtual constructor")
      ChannelPreDecisionRegex* clone() const override
      { return new ChannelPreDecisionRegex ( *this ) ; }
      /// MANDATORY: the only one essential methos
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class TriggerDecisionSubString
     *  Accessor to the trigger decision
     *  @see LHCb::L0DUReport::triggerDecision
     *  @see LHCb::L0DUReport::triggerDecisionByName
     *  @see LoKi::Cuts::L0_TRIGGER_SUBSTR
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API TriggerDecisionSubString : public ChannelDecisionSubString
    {
    public:
      // ======================================================================
      /// trigger decision by name
      TriggerDecisionSubString ( const std::string& channel , const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      TriggerDecisionSubString* clone() const override
      { return new TriggerDecisionSubString ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
    /** @class TriggerDecisionRegex
     *  Accessor to the trigger decision
     *  @see LHCb::L0DUReport::triggerDecision
     *  @see LHCb::L0DUReport::triggerDecisionByName
     *  @see LoKi::Cuts::L0_TRIGGER_SUBSTR
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API TriggerDecisionRegex : public ChannelDecisionRegex
    {
    public:
      // ======================================================================
      /// trigger decision by name
      TriggerDecisionRegex ( const std::string& channel , const int bx = 0 ) ;
      /// MANDATORY: clone method ('virtual constructor')
      TriggerDecisionRegex* clone() const override
      { return new TriggerDecisionRegex ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() ( const LHCb::L0DUReport* a ) const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& s ) const  override;
      // ======================================================================
    } ;
    // ========================================================================
  } //                                                end of namespace LoKi::L0
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_L0_H
