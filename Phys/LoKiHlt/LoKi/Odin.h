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
#ifndef LOKI_ODIN_H
#define LOKI_ODIN_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <utility>
#include <variant>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Time.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/EvtNum.h"
#include "LoKi/RoutingBits.h"
// ============================================================================
// DAQEvent
// ============================================================================
#include "Event/ODIN.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::Odin
   * The namespace to keep all ODIN-related LoKi functors
   *  This file is a part of LoKi project -
   *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
   *
   *  The package has been designed with the kind help from
   *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
   *  contributions and advices from G.Raven, J.van Tilburg,
   *  A.Golutvin, P.Koppenburg have been used in the design.
   *
   *  @see LHCb::ODIN
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-09-16
   *
   */
  namespace Odin {
    // ======================================================================
    /// the private state
    enum Flag { One, Range, List }; // the private state
    // ========================================================================
    /** @class Run
     *  The trivial function with returnn run-number
     *  @see LHCb::ODIN::runNumber
     *  @see LoKi::Cuts::ODIN_RUN
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class Run : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      Run() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      Run* clone() const override { return new Run( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->runNumber(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_RUN"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class Evt1
     *  The trivial function with return even number-number (modulo)
     *  @see LHCb::ODIN::eventNumber % modulo
     *  @see LoKi::Cuts::ODIN_EVT1
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2012-04-03
     */
    class Evt1 : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// constructor from the modulo
      Evt1( unsigned long long modulo );
      /// MANDATORY: clone method ("virtual constructor")
      Evt1* clone() const override;
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
      unsigned long long modulo() const { return m_modulo; }
      // ======================================================================
    private:
      // ======================================================================
      /// the modulo
      unsigned long long m_modulo; // the modulo
      // ======================================================================
    };
    // ========================================================================
    /** @class Evt2
     *  The trivial function with return even number-number (modulo)
     *  @see LHCb::ODIN::eventNumber / modulo
     *  @see LoKi::Cuts::ODIN_EVT2
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2012-04-03
     */
    class Evt2 : public LoKi::Odin::Evt1 {
    public:
      // ======================================================================
      /// constructor from the modulo
      Evt2( unsigned long long modulo );
      /// MANDATORY: clone method ("virtual constructor")
      Evt2* clone() const override;
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Orbit
     *  The trivial function with return orbit-number
     *  @see LHCb::ODIN::orbitNumber
     *  @see LoKi::Cuts::ODIN_ORBIT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class Orbit : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      Orbit() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      Orbit* clone() const override { return new Orbit( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->orbitNumber(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_ORBIT"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class EventType
     *  The trivial function with return the event type
     *  @see LHCb::ODIN::eventType
     *  @see LoKi::Cuts::ODIN_EVTTYP
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class EventType : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      EventType() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      EventType* clone() const override { return new EventType( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->eventType(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_EVTTYP"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class BunchId
     *  The trivial function with return the event type
     *  @see LHCb::ODIN::bunchID
     *  @see LoKi::Cuts::ODIN_BUNCH
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class BunchId : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      BunchId() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      BunchId* clone() const override { return new BunchId( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->bunchId(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_BUNCH"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class ErrorBits
     *  The trivial function with return the error bits
     *  @see LHCb::ODIN::errorBits
     *  @see LoKi::Cuts::ODIN_ERRBITS
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class ErrorBits : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      ErrorBits() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      ErrorBits* clone() const override { return new ErrorBits( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->errorBits(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_ERRBITS"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class TriggerType
     *  The trivial function with return the trigger type
     *  @see LHCb::ODIN::triggerType
     *  @see LoKi::Cuts::ODIN_TRGTYP
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class TriggerType : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      TriggerType() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      TriggerType* clone() const override { return new TriggerType( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->triggerType(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_TRGTYP"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class ReadOutType
     *  The trivial function with return the readout type
     *  @see LHCb::ODIN::readoutType
     *  @see LoKi::Cuts::ODIN_ROTYP
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class ReadOutType : public LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      ReadOutType() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      ReadOutType* clone() const override { return new ReadOutType( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->readoutType(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_ROTYP"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class ForceBit
     *  The trivial predicate to check LHCb::ODIN::forceBit
     *  @see LHCb::ODIN
     *  @see LHCb::ODIN::forceBit
     *  @see LoKi::Cuts::ODIN_FORCEBIT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    struct ForceBit : LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
      // ======================================================================
      /// Default Constructor
      ForceBit() : AuxFunBase{std::tie()} {}
      // MANDATORY: clone method ("virtual constructor")
      ForceBit* clone() const override { return new ForceBit( *this ); }
      // MANDATORY: the only one essential method
      bool operator()( const LHCb::ODIN* o ) const override { return o->forceBit(); }
      // OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_FORCEBIT"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class BXType
     *  The trivial function with return the bunch crossing type
     *  @see LHCb::ODIN::nunchCrossingType
     *  @see LoKi::Cuts::ODIN_BXTYP
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    struct BXType : LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
      // ======================================================================
      /// Default Constructor
      BXType() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      BXType* clone() const override { return new BXType( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->bunchCrossingType(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_BXTYP"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class BunchCurrent
     *  The trivial function with return the bunch current
     *  @see LHCb::ODIN::bunchCurrent
     *  @see LoKi::Cuts::ODIN_BXCURRENT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    struct BunchCurrent : LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
    public:
      // ======================================================================
      /// Default Constructor
      BunchCurrent() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      BunchCurrent* clone() const override { return new BunchCurrent( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->bunchCurrent(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_BXCURRENT"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class CalibrationStep
     *  The trivial function with return the calibration step
     *  @see LHCb::ODIN::calibration step
     *  @see LoKi::Cuts::ODIN_CALSTEP
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    struct CalibrationStep : LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
      // ======================================================================
      /// Default Constructor
      CalibrationStep() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      CalibrationStep* clone() const override { return new CalibrationStep( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->calibrationStep(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_CALSTEP"; }
      // ======================================================================
    };
    // ========================================================================
    /** @class TrgConfKey
     *  The trivial function with return the trigger configuration key
     *  @see LHCb::ODIN::triggerConfigurationKey
     *  @see LoKi::Cuts::ODIN_TCK
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    struct TrgConfKey : LoKi::BasicFunctors<const LHCb::ODIN*>::Function {
      // ======================================================================
      /// Default Constructor
      TrgConfKey() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      TrgConfKey* clone() const override { return new TrgConfKey( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::ODIN* o ) const override { return o->triggerConfigurationKey(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override { return s << "ODIN_TCK"; }
      // ======================================================================
    };
    // ========================================================================
    /// the derived functions
    // ========================================================================
    /** @class InTime
     *  The trivial predicate whcih checks that the event time is within
     *  the certain time window
     *  @see LoKi::Cuts::ODIN_TIME
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class InTime : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// the constructor from the start/stop times
      InTime( const Gaudi::Time& start, const Gaudi::Time& stop );
      /// the constructor form the start & span
      InTime( const Gaudi::Time& start, const Gaudi::TimeSpan& span );
      /// MANDATORY: clone method ("virtual constructor")
      InTime* clone() const override { return new InTime( *this ); }
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      const Gaudi::Time& start() const { return m_start; }
      const Gaudi::Time& stop() const { return m_stop; }
      // ======================================================================
    private:
      // ======================================================================
      /// the start time
      Gaudi::Time m_start; // the start time
      Gaudi::Time m_stop;  // the stop time
      // ======================================================================
    };
    // ========================================================================
    /** @class EvtNumber
     *  Simple predicate to check the event number
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-03-07
     */
    class EvtNumber : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// the actual type for event_type
      typedef LoKi::Numbers::EvtNum event_type;
      /// the actual type of event list
      typedef LoKi::Numbers::EvtNumList event_list;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the run range
      EvtNumber( const event_type begin, const event_type end );
      /// constructor from event list
      EvtNumber( const event_list& events );
      /// constructor from the event number
      EvtNumber( const event_type evt );
      /// MANDATORY: clone method ("virtual constructor")
      EvtNumber* clone() const override;
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      using event_range = std::pair<event_type, event_type>;
      std::variant<event_range, event_list> m_evts;
      // ======================================================================
    };
    // ========================================================================
    /** @class RunNumber
     *  The trivial predicate which checks the run number,
     *  or range of run numbers or list of run numbers
     *  @see LoKi::Cuts::ODIN_RUNNUMBER
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class RunNumber : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// the actual type of the run
      typedef LoKi::Numbers::RunEvt::run_type run_type;
      typedef std::vector<run_type>           run_list;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the run number
      RunNumber( const run_type run );
      /// constructor from the run range
      RunNumber( const run_type begin, const run_type end );
      /// constructor from the run list
      RunNumber( const run_list& runs );
      /// MANDATORY: clone method ("virtual constructor")
      RunNumber* clone() const override;
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      using run_range = std::pair<run_type, run_type>;
      std::variant<run_range, run_list> m_runs;
      // ======================================================================
    };
    // ========================================================================
    /** @class RunEvtNumber
     *  The trivial predicate which checks (Run,Event)-identifiers
     *  @see LoKi::Cuts::ODIN_RUNEVT
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class RunEvtNumber : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// the actual type of run-number
      typedef LoKi::Numbers::RunEvt::run_type run_type;
      /// the actual type of event-number
      typedef LoKi::Numbers::RunEvt::evt_type evt_type;
      /// the actual type of run-event pair
      typedef LoKi::Numbers::RunEvt runevt_type;
      // the actual type of run-event list
      typedef LoKi::Numbers::RunEvtList runevt_list;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the run/event number
      RunEvtNumber( const run_type run, const evt_type evt );
      /// constructor from the run/event number
      RunEvtNumber( const runevt_type& runevt );
      /// constructor from the run/event range
      RunEvtNumber( const runevt_type& begin, const runevt_type& end );
      /// constructor from the run-event  list
      RunEvtNumber( const runevt_list& runevts );
      /// MANDATORY: clone method ("virtual constructor")
      RunEvtNumber* clone() const override;
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      using runevt_range = std::pair<runevt_type, runevt_type>;
      std::variant<runevt_range, runevt_list> m_runevts;
      // ======================================================================
    };
    // ========================================================================
    /** @class BXId
     *  The trivial predicate to deal with bunch-crossing IDs
     *  @see LHCb::ODIN::bunchID
     *  @see LoKi::Cuts::ODIN_BXID
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class BXId : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// constructor from the BX-Id
      explicit BXId( unsigned int bx );
      /// constructor from the run range
      BXId( unsigned int begin, unsigned int end );
      /// constructor from the run list
      BXId( std::vector<unsigned int> bxs );
      /// MANDATORY: clone method ("virtual constructor")
      BXId* clone() const override { return new BXId( *this ); }
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      using bx_range  = std::pair<unsigned int, unsigned int>;
      using bx_vector = std::vector<unsigned int>;
      std::variant<bx_range, bx_vector> m_bxs;
      // ======================================================================
    };
    // ========================================================================
    /** @class Tck
     *  The trivial predicate to deal with bunch-crossing IDs
     *  @see LHCb::ODIN::triggerConfigurationKey
     *  @see LoKi::Cuts::ODIN_TCKEYS
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-06-16
     */
    class Tck : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// constructor from a tck
      explicit Tck( unsigned int tck ) : Tck( std::vector<unsigned int>{tck} ) {}
      /// constructor from the tck list
      Tck( std::vector<unsigned int> tcks );
      /// MANDATORY: clone method ("virtual constructor")
      Tck* clone() const override { return new Tck( *this ); }
      /// MANDATORY: The only one essential method:
      bool operator()( const LHCb::ODIN* o ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      std::vector<unsigned int> m_tcks;
      // ======================================================================
    };
    // ========================================================================
    /** @class HltRoutingBits
     *  Simple predicate to check the hlt routing bits
     *  @see LoKi::Cuts::ODIN_ROUTINGBITS
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2010-05-17
     */
    class RoutingBits : public LoKi::BasicFunctors<const LHCb::ODIN*>::Predicate {
    public:
      // ======================================================================
      /// constructor from routing bits
      RoutingBits( const LoKi::HLT::RoutingBits& bits );
      /// MANDATORY: clone method ("virtual constructor")
      RoutingBits* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::ODIN* ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& ) const override;
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
  } // namespace Odin
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_ODIN_H
