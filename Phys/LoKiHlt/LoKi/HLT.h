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
#ifndef LOKI_HLT_H
#define LOKI_HLT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/StringKey.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/RoutingBits.h"
// ============================================================================
// L0Event
// ============================================================================
#include "Event/HltDecReports.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/regex.hpp"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::HLT
   *  The namespace to keep all HltDecReport-related LoKi functors
   *  @see LHCb::HltDecisionReports
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-09-19
   */
  // ==========================================================================
  namespace HLT {
    // ========================================================================
    /** @class HasDecision
     *  simple function which allows to check the existence
     *  of the decision with the given name
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_HAS
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API HasDecision : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
    protected:
      // ======================================================================
      typedef std::vector<Gaudi::StringKey> Names;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the decision name
      HasDecision( const std::string& name ) : HasDecision( Names{name} ) {}
      /// constructor from the decision names ("OR")
      HasDecision( const std::string& name1, const std::string& name2 ) : HasDecision( Names{name1, name2} ) {}
      /// constructor from the decision names ("OR")
      HasDecision( const std::string& name1, const std::string& name2, const std::string& name3 )
          : HasDecision( Names{name1, name2, name3} ) {}
      /// constructor from the decision names ("OR")
      HasDecision( const std::string& name1, const std::string& name2, const std::string& name3,
                   const std::string& name4 )
          : HasDecision( Names{name1, name2, name3, name4} ) {}
      /// constructor form the decision names ("OR")
      HasDecision( Names names );
      /// MANDATORY: clone method ("virtual constructor")
      HasDecision* clone() const override { return new HasDecision( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return print( s, "HLT_HAS" ); }
      // ======================================================================
    protected:
      // ======================================================================
      // get the decision names
      const Names& names() const { return m_names; }
      // ======================================================================
    protected:
      // ======================================================================
      Names::const_iterator begin() const { return m_names.begin(); }
      Names::const_iterator end() const { return m_names.end(); }
      // ======================================================================
      std::ostream& print( std::ostream& s, const std::string& name ) const;
      // ======================================================================
    private:
      // ======================================================================
      /// the list of decision names
      Names m_names; // the list of decision names
      // ======================================================================
    };
    // ========================================================================
    /** @class PassDecision
     *  simple function which allows to check if the decision
     *  has been passed
     *  of the decision with the given name
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_PASS
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API PassDecision : HasDecision {
      // ======================================================================
      /// constructor from the decision name
      PassDecision( const std::string& name ) : PassDecision( Names{name} ) {}
      /// constructor from the decision names ("OR")
      PassDecision( const std::string& name1, const std::string& name2 ) : PassDecision( Names{name1, name2} ) {}
      /// constructor from the decision names ("OR")
      PassDecision( const std::string& name1, const std::string& name2, const std::string& name3 )
          : PassDecision( Names{name1, name2, name3} ) {}
      /// constructor from the decision names ("OR")
      PassDecision( const std::string& name1, const std::string& name2, const std::string& name3,
                    const std::string& name4 )
          : PassDecision( Names{name1, name2, name3, name4} ) {}
      /// constructor form the decision names ("OR")
      PassDecision( Names names );
      /// MANDATORY: clone method ("virtual constructor")
      PassDecision* clone() const override { return new PassDecision( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return print( s, "HLT_PASS" ); }
      // ======================================================================
    };
    // ========================================================================
    /** @class Size
     *  simple function which returns the number of known decision
     *  @see LHCb::HltDecReports
     *  @see LHCb::HltDecReports::size
     *  @see LoKi::Cuts::HLT_SIZE
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API Size : LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
      // ======================================================================
      /// MANDATORY: default constructor
      Size() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual consructor")
      Size* clone() const override { return new Size( *this ); }
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "HLT_SIZE"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class NPass
     *  simple function which returnn the number of 'passes' decision
     *  @see LHCb::HltDecReports
     *  @see LHCb::HltDecReports::decReports
     *  @see LHCb::HltDecReport
     *  @see LHCb::HltDecReport::decision
     *  @see LoKi::Cuts::HLT_SIZE
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API NPass : LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
      // ======================================================================
      /// MANDATORY: default constructor
      NPass() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual consructor")
      NPass* clone() const override { return new NPass( *this ); }
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "HLT_NPASS"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class Decision
     *  simple function which checks the presence of at least one positive
     *  decision
     *  @see LHCb::HltDecReports
     *  @see LHCb::HltDecReports::decReports
     *  @see LHCb::HltDecReport
     *  @see LHCb::HltDecReport::decision
     *  @see LoKi::Cuts::HLT_DECISION
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    struct GAUDI_API Decision : LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
      // ======================================================================
      /// MANDATORY: default constructor
      Decision() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual consructor")
      Decision* clone() const override { return new Decision( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "HLT_DECISION"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class DecisionBut
     *  Simple predicate which checks the positive decisison, ignoring
     *  "the special" cases
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_PASSIGNORING
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-03
     */
    class GAUDI_API DecisionBut : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
    protected:
      // ======================================================================
      typedef std::vector<Gaudi::StringKey> Names;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from one "special" decicion
      DecisionBut( const std::string& name ) : DecisionBut( Names{name} ) {}
      /// constructor from two "special" decicions
      DecisionBut( const std::string& name1, const std::string& name2 ) : DecisionBut( Names{name1, name2} ) {}
      /// constructor from three "special" decicions
      DecisionBut( const std::string& name1, const std::string& name2, const std::string& name3 )
          : DecisionBut( Names{name1, name2, name3} ) {}
      /// constructor from four "special" decicions
      DecisionBut( const std::string& name1, const std::string& name2, const std::string& name3,
                   const std::string& name4 )
          : DecisionBut( Names{name1, name2, name3, name4} ) {}
      /// constructor from vector of "special" decisions
      DecisionBut( const std::vector<std::string>& names ) : DecisionBut( Names( names.begin(), names.end() ) ) {}
      /// constructor from vector of "special" decisions
      DecisionBut( Names names );
      /// MANDATORY: clone method ("virtual constructor")
      DecisionBut* clone() const override { return new DecisionBut( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual vector of "special" decision
      Names m_special; // the actual vector of "special" decision
      // ======================================================================
    };
    // ========================================================================
    /** @class HasDecisionSubString
     *  simple function which allows to check the existence
     *  of the decision with the given name
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_HAS_SUBSTR
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-09-19
     */
    class GAUDI_API HasDecisionSubString : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
    public:
      // ======================================================================
      /// constructor from substring
      HasDecisionSubString( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      HasDecisionSubString* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
      /// access to the substring
      const std::string& substr() const { return m_substr; }
      // ======================================================================
    private:
      // ======================================================================
      /// the substring to be looked for
      std::string m_substr; // the substring to be looked
      // ======================================================================
    };
    // ========================================================================
    /** @class PassDecisionSubString
     *  Simple class to check the presence of at least one passes selection
     *  which contains the substring
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_PASS_SUBSTR
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-06-16
     */
    struct GAUDI_API PassDecisionSubString : HasDecisionSubString {
      // ======================================================================
      /// constructor from substring
      PassDecisionSubString( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      PassDecisionSubString* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class DecisionButSubString
     *  Simple predicate which checks the positive decisison, ignoring
     *  "the special" cases
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_PASSIGNORING_SUBSTR
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-06-16
     */
    struct GAUDI_API DecisionButSubString : PassDecisionSubString {
      // ======================================================================
      /// constructor from substring
      DecisionButSubString( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      DecisionButSubString* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class HasDecisionRegex
     *  Simple class to check the presence of at least one passes selection
     *  which matches the regular expression
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_HAS_RE
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-06-16
     */
    class GAUDI_API HasDecisionRegex : public HasDecisionSubString {
    public:
      // ======================================================================
      /// constructor from substring
      HasDecisionRegex( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      HasDecisionRegex* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
      /// regurn the regular expression
      const boost::regex& expression() const { return m_expression; }
      // ======================================================================
    private:
      // ======================================================================
      /// the regular expression
      boost::regex m_expression; // the regular expression
      // ======================================================================
    };
    // ========================================================================
    /** @class PAssDecisionRegex
     *  Simple class to check the presence of at least one passes selection
     *  which matches the regular expression
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_HAS_RE
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-06-16
     */
    struct GAUDI_API PassDecisionRegex : HasDecisionRegex {
      // ======================================================================
      /// constructor from substring
      PassDecisionRegex( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      PassDecisionRegex* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class DecisionButRegex
     *  Simple predicate which checks the positive decisison, ignoring
     *  "the special" cases
     *  @see LHCb::HltDecReports
     *  @see LoKi::Cuts::HLT_PASSIGNORING_RE
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-06-16
     */
    struct GAUDI_API DecisionButRegex : PassDecisionRegex {
      // ======================================================================
      /// constructor from substring
      DecisionButRegex( const std::string& substr );
      /// MANDATORY: clone method ("virtual constructor")
      DecisionButRegex* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
    };
    // ========================================================================
    /** @class ErrorBits
     *  Simple functor to extract the error bits from the the given channel
     *  @see LoKi::Cuts::HLT_ERRORBITS
     *  @see LHCb::HltDecReport::errorBits
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    class GAUDI_API ErrorBits : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
    public:
      // ======================================================================
      /// constructor from the channel name
      ErrorBits( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      ErrorBits* clone() const override;
      /// MANDATORY: the only one essential methor
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
      const Gaudi::StringKey& channel() const { return m_key; }
      // ======================================================================
    private:
      // ======================================================================
      /// the channel key
      Gaudi::StringKey m_key; // the channle key
      // ======================================================================
    };
    // ========================================================================
    /** @class NonTurboPass
     *  Simple functor to determine whether a non-Turbo stream line has fired
     *  @see LoKi::Cuts::HLT_NONTURBOPASS_RE
     *  @author Sean Benson sean.benson@cern.ch
     *  @date 2015-01-12
     */
    struct GAUDI_API NonTurboPass final : HasDecisionRegex {
      // ======================================================================
      /// constructor from the channel name
      NonTurboPass( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      NonTurboPass* clone() const override;
      /// MANDATORY: the only one essential methor
      bool operator()( const LHCb::HltDecReports* a ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class TurboPass
     *  Simple functor to determine whether a Turbo stream line has fired
     *  @see LoKi::Cuts::HLT_TURBOPASS_RE
     *  @author Sean Benson sean.benson@cern.ch
     *  @date 2015-01-12
     */
    struct GAUDI_API TurboPass final : HasDecisionRegex {
      // ======================================================================
      /// constructor from the channel name
      TurboPass( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      TurboPass* clone() const override;
      /// MANDATORY: the only one essential methor
      bool operator()( const LHCb::HltDecReports* a ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ExecutionStage
     *  Simple functor to extract the execution stage for the given channel
     *  @see LoKi::Cuts::HLT_EXECUTIONSTAGE
     *  @see LHCb::HltDecReport::executionStage
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    struct GAUDI_API ExecutionStage final : ErrorBits {
      // ======================================================================
      /// constructor from the channel name
      ExecutionStage( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      ExecutionStage* clone() const override;
      /// MANDATORY: the only one essential methor
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class NumberOfCandidates
     *  Simple functor to extract the execution stage for the given channel
     *  @see LoKi::Cuts::HLT_NCANDIDATES
     *  @see LHCb::HltDecReport::numberOfCandidates
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    struct GAUDI_API NumberOfCandidates final : ErrorBits {
      // ======================================================================
      /// constructor from the channel name
      NumberOfCandidates( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      NumberOfCandidates* clone() const override;
      /// MANDATORY: the only one essential methor
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Saturated
     *  Simple functor to extract the execution stage for the given channel
     *  @see LoKi::Cuts::HLT_SATURATED
     *  @see LHCb::HltDecReport::numberOfCandidates
     *  @see LHCb::HltDecReport::saturatedSaturated
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    class GAUDI_API Saturated final : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
    public:
      // ======================================================================
      /// constructor from the channel name
      Saturated( const std::string& name );
      /// MANDATORY: clone method ( "virtual constructor")
      Saturated* clone() const override;
      /// MANDATORY: the only one essential methor
      bool operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
      const Gaudi::StringKey& channel() const { return m_key; }
      // ======================================================================
    private:
      // ======================================================================
      /// the channel key
      Gaudi::StringKey m_key; // the channle key
      // ======================================================================
    };
    // ========================================================================
    /** @class CountErrorBits
     *  @see LoKi::Cuts::HLT_COUNT_ERRORBITS
     *  @see LHCb::HltDecReport::errorBits
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    class GAUDI_API CountErrorBits final : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
    public:
      // =====================================================================
      /// constructor from the list of lines & mask
      CountErrorBits( const std::vector<std::string>& lines, const unsigned int mask );
      // =====================================================================
      /// constructor from the list of lines & mask
      CountErrorBits( const std::vector<Gaudi::StringKey>& lines, const unsigned int mask );
      // =====================================================================
      /// constructor from the lines & mask
      CountErrorBits( const std::string& line1, const std::string& line2, const unsigned int mask )
          : CountErrorBits( std::vector<std::string>{line1, line2}, mask ) {}
      // =====================================================================
      /// constructor from the lines & mask
      CountErrorBits( const std::string& line1, const std::string& line2, const std::string& line3,
                      const unsigned int mask )
          : CountErrorBits( std::vector<std::string>{line1, line2, line3}, mask ) {}
      // =====================================================================
      /// constructor from the lines & mask
      CountErrorBits( const std::string& line1, const std::string& line2, const std::string& line3,
                      const std::string& line4, const unsigned int mask )
          : CountErrorBits( std::vector<std::string>{line1, line2, line3, line4}, mask ) {}
      // =====================================================================
      /// MANDATORY: clone method ( "virtual constructor")
      CountErrorBits* clone() const override;
      /// MANDATORY: the only one essential methor
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// list of lines
      std::vector<Gaudi::StringKey> m_lines; // list of lines
      /// the mask
      unsigned int m_mask; // the mask
      // ======================================================================
    };
    // ========================================================================
    /** @class CountErrorBitsRegex
     *  @see LoKi::Cuts::HLT_COUNT_ERRORBITS
     *  @see LHCb::HltDecReport::errorBits
     *  @author Vanya BELYAEV  Ivan.Belyaev@cern.ch
     *  @date 2010-05-17
     */
    class GAUDI_API CountErrorBitsRegex final : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
    public:
      // =====================================================================
      /// constructor from the regular expression & mask
      CountErrorBitsRegex( const std::string& expression, const unsigned int mask );
      /// MANDATORY: clone method ( "virtual constructor")
      CountErrorBitsRegex* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::HltDecReports* a ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// regurn the regular expression
      const boost::regex& expression() const { return m_expression; }
      // ======================================================================
    private:
      // ======================================================================
      std::string m_pattern;
      /// the regular expression
      boost::regex m_expression; // the regular expression
      /// the mask
      unsigned int m_mask; // the mask
      // ======================================================================
    };
    // ========================================================================
    /** @class HltRoutingBits
     *  Simple predicate to check the hlt routing bits
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2010-05-17
     */
    class GAUDI_API HltRoutingBits final : public LoKi::BasicFunctors<const LHCb::HltDecReports*>::Predicate {
    public:
      // ======================================================================
      /// constructor from routing bits
      HltRoutingBits( const LoKi::HLT::RoutingBits& bits );
      /// MANDATORY: clone method ("virtual constructor")
      HltRoutingBits* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::HltDecReports* ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
      // ======================================================================
    public:
      // ======================================================================
      operator const LoKi::HLT::RoutingBits&() const { return m_bits; }
      // ======================================================================
    private:
      // ======================================================================
      /// the bits
      LoKi::HLT::RoutingBits m_bits; // the bits
      // ======================================================================
    };
    // ========================================================================
    /** @class TrgConfKey
     *  simple function which returns the TCK
     *  @see LHCb::HltDecReports
     *  @see LHCb::HltDecReports::configuredTCK
     *  @see LoKi::Cuts::HLT_TCK
     *  @author Pieter David <pieter.david@nikhef.nl>
     *  @date 2014-08-28
     */
    struct GAUDI_API TrgConfKey final : LoKi::BasicFunctors<const LHCb::HltDecReports*>::Function {
      // ======================================================================
      /// MANDATORY: default constructor
      TrgConfKey() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual consructor")
      TrgConfKey* clone() const override { return new TrgConfKey( *this ); }
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::HltDecReports* a ) const override { return a->configuredTCK(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "HLT_TCK"; }
      // ======================================================================
    };
    // ========================================================================
  } // namespace HLT
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_HLT_H
