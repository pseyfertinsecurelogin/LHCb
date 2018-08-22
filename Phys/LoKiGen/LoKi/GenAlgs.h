// ============================================================================
#ifndef LOKI_GENALGS_H
#define LOKI_GENALGS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <numeric>
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Algs.h"
// ============================================================================
// Event
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-23
 *
 */
// ============================================================================
namespace LoKi
{
  /** @namespace  LoKi::GenAlgs  LoKi/GenAlgs.h
   *
   *  Collection of  few helper functions/algoritgms  "a'la STL"
   *  to simplify the operations with HepMC graphs
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-01-23
   */
  namespace GenAlgs
  {
    template < class OBJECT    ,
               class PREDICATE >
    inline size_t count_if
    ( OBJECT           first ,
      OBJECT           last  ,
      const PREDICATE& cut   ) ;
    // ========================================================================
    /** useful helper function (a'la STL) to count a number of
     *  (HepMC) particles, which satisfies the certain criteria,
     *  e.e. count number of leptons in LHCb acceptance :
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const HepMC::GenEvent* event = ... ;
     *
     *  const size_t nLeptons =
     *    count_if ( event , GLEPTON && GTHETA < 400 * mrad ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GLEPTON
     *  @see LoKi::Cuts::GTHETA
     *
     *  @param event pointer to HepMC event
     *  @param cut   the condidion/predicate/criterion
     *  @return number of elements which satisfy the criterion
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template <class PREDICATE>
    inline size_t count_if
    ( const HepMC::GenEvent* event ,
      const PREDICATE&       cut   )
    {
      return ( 0 == event ) ?  0 : LoKi::Algs::count_if
        ( event->particles_begin () , event->particles_end () , cut ) ;
    }
    // ========================================================================
    /** useful helper function (a'la STL) to count a number of
     *  (HepMC) particles, which satisfies the certain criteria,
     *  e.e. count number of muons with large transverse momentum
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const LHCb::HepMCEvent* event = ... ;
     *
     *  const size_t nLeptons =
     *    count_if ( event , "mu+" == GABSID  && GPT > 4 * GeV  ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GPT
     *
     *  @param event pointer to HepMC event
     *  @param cut   the condidion/predicate/criterion
     *  @return number of elements which satisfy the criterion
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template <class PREDICATE>
    inline size_t count_if
    ( const LHCb::HepMCEvent* event ,
      const PREDICATE&        cut   )
    {
      return ( 0 == event ) ? 0 :
        LoKi::GenAlgs::count_if ( event -> pGenEvt() , cut ) ;
    }
    // ========================================================================
    /** useful helper function (a'la STL) to count a number of
     *  (HepMC) particles, which satisfy certain criteria,
     *  e.e. count number of beauty particles
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const LHCb::HepMCEvent* events =
     *   get<LHCb::HepMCEvents> ( LHCb::HepMCEvenLocation::Default ) ;
     *
     *  const size_t nBeauty  =
     *    count_if ( events , BEAUTY ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::BEAUTY
     *
     *  @param events pointer to HepMC event
     *  @param cut    the condidion/predicate/criterion
     *  @return number of elements which satisfy the criterion
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template <class PREDICATE>
    inline size_t count_if
    ( const LHCb::HepMCEvent::Container* events ,
      const PREDICATE&                   cut    )
    {
      return ( 0 == events ) ? 0 :
        LoKi::GenAlgs::count_if ( events->begin () , events->end   () , cut ) ;
    }
    // ========================================================================
    /** simple function to count number of "good" paricles in the HepMC-graph
     *  @param vertex pointer to the vertex
     *  @param cut    predicate which defines "good" particle
     *  @param range  range of HepMC-graph
     *  @see HepMC::IteratorRange
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-03
     */
    template <class PREDICATE>
    inline size_t count_if
    ( const HepMC::GenVertex*   vertex   ,
      const PREDICATE&          cut      ,
      HepMC::IteratorRange      range    )
    {
      if ( 0 == vertex ) { return 0 ; }                              // RETURN
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      return LoKi::Algs::count_if
        ( _v -> particles_begin ( range ) ,
          _v -> particles_end   ( range ) , cut ) ;                  // RETURN
    }
    // ========================================================================
    /** simple function to count number of "good" paricles in the decay tree
     *  of th egiven particlee
     *  @param particle pointer to the mother particle
     *  @param cut    predicate which defines "good" particle
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-03
     */
    template <class PREDICATE>
    inline size_t count_if
    ( const HepMC::GenParticle* particle ,
      const PREDICATE&          cut      )
    {
      size_t result = 0 ;
      if ( 0 != particle )
      {
        result = LoKi::GenAlgs::count_if
          ( particle->end_vertex() , cut , HepMC::descendants ) ;
      }
      if ( cut ( particle ) ) { ++result ;}
      // check the particle itself
      return result ;
    }
    // ========================================================================
    /** useful helper function (a'la STL) to count a number of
     *  (HepMC) particles, which satisfy certain criteria,
     *  e.e. count number of beauty particles
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  // any sequence of objects, convertible to
     *  // const LHCb::HepMCEvents*, const LHCb::HepMCEvent*
     *  // or const HepMC::GenEvent*
     *  SEQUENCE objects = ... ;
     *
     *  const size_t nBeauty  =
     *    count_if ( objects.begin() , objects.end() , BEAUTY ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::BEAUTY
     *
     *  @param  first begin of the sequence
     *  @param  last  end of the sequence
     *  @param  cut   the predicate
     *  @return number of elements which satisfy the criterion
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template <class OBJECT,class PREDICATE>
    inline size_t count_if
    ( OBJECT           first ,
      OBJECT           last  ,
      const PREDICATE& cut   )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, size_t(0),
                              [&](size_t n, arg_t arg)
                              { return n+LoKi::GenAlgs::count_if ( arg , cut ); } );
    }
    // ========================================================================
    /** usefull helper function (a'la STL) to efficiently check the
     *  presence of at least one (HepMC) particle, which satisfies the
     *  certain criteria, e.g. b-buarks from Higgs decays:
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const HepMC::GenEvent* event = ... ;
     *
     *  const bool good =
     *    found ( event , ( "b" == GABSID ) &&
     *       0 < GNINTREE ( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *
     *  @param event pointer to HepMC event
     *  @param cut   the condidion/predicate/criterion
     *  @return true if the particle, which satisfies the criterion is found
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template<class PREDICATE>
    inline bool found
    ( const HepMC::GenEvent* event ,
      const PREDICATE&       cut   )
    {
      return event && ( event->particles_end() != LoKi::Algs::find_if
        ( event->particles_begin () , event->particles_end () , cut ) );
    }
    // ========================================================================
    /** useful helper function (a'la STL) to efficiently check the
     *  presence of at least one (HepMC) particle, which satisfies the
     *  certain criteria, e.e. muon in LHCb acceptance :
     *
     *  @code
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const LHCb::HepMCEvent* event = ... ;
     *
     *  const bool good =
     *    found ( event , "mu+" == GABSID && GTHETA < 400 * mrad ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GTHETA
     *
     *  @param event pointer to HepMC event
     *  @param cut   the condidion/predicate/criterion
     *  @return true if the particle, which satisfies the criterion is found
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template<class PREDICATE>
    inline bool found
    ( const LHCb::HepMCEvent* event ,
      const PREDICATE&        cut   )
    {
      return event && LoKi::GenAlgs::found ( event->pGenEvt() , cut ) ;
    }
    // ========================================================================
    /** useful helper function (a'la STL) to efficiently check the
     *  presence of at least one (HepMC) particle, which satisfies the
     *  certain criteria, e.g. for beauty charm meson
     *
     *  @code
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  const LHCb::HepMCEvent* events =
     *   get<LHCb::HepMCEvents> ( LHCb::HepMCEvenLocation::Default ) ;
     *
     *  const bool good = found ( events , GBEAUTY && GCHARM && GMESON ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::GBEAUTY
     *  @see LoKi::Cuts::GCHARM
     *  @see LoKi::Cuts::GMESON
     *
     *  @param events pointer to HepMC event
     *  @param cut    the condidion/predicate/criterion
     *  @return true if the particle, which satisfies the criterion is found
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template<class PREDICATE>
    inline bool found
    ( const LHCb::HepMCEvents* events ,
      const PREDICATE&         cut   )
    {
      return events && std::any_of( events->begin(), events->end(),
                                    [&](LHCb::HepMCEvents::const_reference ev)
                                    { return LoKi::GenAlgs::found(ev,cut); } );
    }
    // ========================================================================
    /** simple function to check the presence of "good" particle in HepMC-graph
     *  @param vertex pointer to the vertex
     *  @param cut defintion of "good" particle
     *  @param range HepMC-graph
     *  @return true if at least one "good" particle is in a graph.
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class PREDICATE>
    inline bool found
    ( const HepMC::GenVertex*   vertex   ,
      const PREDICATE&          cut      ,
      HepMC::IteratorRange      range    )
    {
      auto v = const_cast<HepMC::GenVertex*>(vertex);
      return v && LoKi::Algs::found( v->particles_begin( range ),
                                     v->particles_end  ( range ), cut ) ;
    }
    // ========================================================================
    /** simple function to check the presence of "good" particle in the decay
     *  tree of the mother particle
     *  @param particle pointer to mother particle
     *  @param cut defintion of "good" particle
     *  @return true if at least one "good" particle is in a decay tree
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class PREDICATE>
    inline bool found
    ( const HepMC::GenParticle* particle ,
      const PREDICATE&          cut      )
    {
      return cut( particle ) ||
           ( particle && LoKi::GenAlgs::found( particle->end_vertex() ,
                                               cut , HepMC::descendants ) );
    }
    // =====================================================================
    template <class PREDICATE>
    inline
    const HepMC::GenParticle*
    foundFirst
    ( const HepMC::GenVertex*   vertex ,
      const PREDICATE&          cut    ) ;
    // ======================================================================
    template <class PREDICATE>
    inline
    const HepMC::GenParticle*
    foundFirst
    ( const HepMC::GenParticle* particle ,
      const PREDICATE&          cut    ) ;
    // ======================================================================
    template <class PARTICLE, class PREDICATE>
    inline
    const HepMC::GenParticle*
    foundFirst
    ( PARTICLE         begin ,
      PARTICLE         end   ,
      const PREDICATE& cut   )
    {
      //
      const HepMC::GenParticle* result = nullptr ;
      //
      for ( ; !result && begin != end ; ++begin )
      { result = foundFirst ( *begin , cut ) ; }
      //
      return result ;
    }
    // ======================================================================
    /** simple function to get the first matched element in tree
     *  tree of the mother particle
     *  @param particle pointer to mother particle
     *  @param cut defintion of "good" particle
     *  @return good particle
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class PREDICATE>
    inline
    const HepMC::GenParticle*
    foundFirst
    ( const HepMC::GenParticle* particle ,
      const PREDICATE&          cut      )
    {
      if ( 0 ==  particle   ) { return nullptr  ; }      // RETURN
      if ( cut ( particle ) ) { return particle ; }      // RETURN
      //
      if ( 0 == particle->end_vertex()  ) { return nullptr ; } // RETURN
      //
      return foundFirst ( particle->end_vertex() , cut ) ;
    }
    // ========================================================================
    template <class PREDICATE>
    inline
    const HepMC::GenParticle*
    foundFirst
    ( const HepMC::GenVertex* vertex ,
      const PREDICATE&        cut    )
    {
      //
      if ( !vertex ) { return nullptr ; } // RETURN
      //
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      //
      return foundFirst
        ( _v -> particles_begin ( HepMC::children ) ,
          _v -> particles_end   ( HepMC::children ) , cut ) ;
    }
    // ========================================================================
    /** useful helper function (a'la STL) to efficiently check the
     *  presence of at least one (HepMC) particle, which satisfies the
     *  certain criteria, e.g. for beauty particles
     *
     *  @code
     *
     *  using namespace LoKi::GenAlgs
     *  using namespace LoKi::Cuts
     *
     *  // any sequence of objects, convertible to
     *  // const LHCb::HepMCEvents*, const LHCb::HepMCEvent*
     *  // or const HepMC::GenEvent*
     *  SEQUENCE objects = ... ;
     *
     *  const bool good =
     *    found ( objects.begin() , objects.end() , BEAUTY ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::BEAUTY
     *
     *  @param  first begin of the sequence
     *  @param  last  end of the sequence
     *  @param  cut   the predicate
     *  @return number of elements which satisfy the criterion
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-23
     */
    template<class OBJECT,class PREDICATE>
    inline bool found
    ( OBJECT           first ,
      OBJECT           last  ,
      const PREDICATE& cut   )
    {
      using arg_t = decltype(*first);
      return std::any_of( first, last,
                          [&](arg_t arg) { return found(arg,cut); } );
    }
    // ========================================================================
    /** Simple algorithm for accumulation of
     *  the function value through the  HepMC graph
     *
     *  E.g. one can 'accumulate' the total
     *  4-momentum of all daughter muons:
     *
     *  @code
     *
     *  const HepMC::GenEvent* event = ... ;
     *
     *  LoKi::LorentzVector result = LoKi::GenAlgs::accumulate
     *   ( event                            ,   // the event
     *     LoKi::Kinematics::Gen4Momentum() ,   // get 4-mometumm
     *     "mu+" == GABSID                  ,   // consider only muons
     *     LoKi::LorentzVector()            ,   // the initial value
     *     std::plus<LoKi::LorentzVector>() ) ; // operation
     *
     *  @endcode
     *
     *  @see HepMC::GenEvent
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::LorentzVector
     *  @see LoKi::Kinematics::Gen4Momentum
     *
     *  @param  event     pointer to HepMC::GenEvent
     *  @param  functor   function to be evaluated
     *  @param  predicate selection criteria
     *  @param  result    the initial-value for the result
     *  @return the result of accumulation
     *
     *  @author Vanya BELYAEV ibelyaev@physucs.syr.edu
     *  @date 2006-02-09
     */
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE ,
               class OPERATION >
    inline RESULT accumulate
    ( const HepMC::GenEvent*       event     ,
      const FUNCTOR&               functor   ,
      const PREDICATE&             predicate ,
      RESULT                       result    ,
      OPERATION                    binop     )
    {
      return event ? LoKi::Algs::accumulate( event->particles_begin() ,
                                             event->particles_end  () ,
                                             functor , predicate , result , binop )
                   : result;
    }
    // ========================================================================
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE >
    inline RESULT accumulate
    ( const HepMC::GenEvent*       event     ,
      const FUNCTOR&               functor   ,
      const PREDICATE&             predicate ,
      RESULT                       result    )
    {
      return accumulate
        ( event , functor , predicate , result , std::plus<RESULT>() ) ;
    }
    // ========================================================================
    /** Simple algorithm for accumulation of
     *  the function value through the  HepMC graph
     *
     *  E.g. one can 'accumulate' the total
     *  4-momentum of all daughter muons:
     *
     *  @code
     *
     *  const LHCb::HepMCEvent* event = ... ;
     *
     *  LoKi::LorentzVector result = LoKi::GenAlgs::accumulate
     *   ( event                            ,   // the event
     *     LoKi::Kinematics::Gen4Momentum() ,   // get 4-mometumm
     *     "mu+" == GABSID                  ,   // consider only muons
     *     LoKi::LorentzVector()            ,   // the initial value
     *     std::plus<LoKi::LorentzVector>() ) ; // operation
     *
     *  @endcode
     *
     *  @see LHCb::HepMCEvent
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::LorentzVector
     *  @see LoKi::Kinematics::Gen4Momentum
     *
     *  @param  event     pointer to HepMC::GenEvent
     *  @param  functor   function to be evaluated
     *  @param  predicate selection criteria
     *  @param  result    the initial-value for the result
     *  @return the result of accumulation
     *
     *  @author Vanya BELYAEV ibelyaev@physucs.syr.edu
     *  @date 2006-02-09
     */
    template <class RESULT,
              class FUNCTOR,
              class PREDICATE,
              class OPERATION>
    inline RESULT accumulate
    ( const LHCb::HepMCEvent* event     ,
      const FUNCTOR&          functor   ,
      const PREDICATE&        predicate ,
      RESULT                  result    ,
      OPERATION               binop     )
    {
      return event ? LoKi::GenAlgs::accumulate( event->pGenEvt() , functor ,
                                                predicate , result , binop )
                   : result;
    }
    // ========================================================================
    template <class RESULT,
              class FUNCTOR,
              class PREDICATE>
    inline RESULT accumulate
    ( const LHCb::HepMCEvent* event     ,
      const FUNCTOR&          functor   ,
      const PREDICATE&        predicate ,
      RESULT                  result    )
    {
      return accumulate
        ( event , functor , predicate , result , std::plus<RESULT>() ) ;
    }
    // ========================================================================
    /** Simple algorithm for accumulation of
     *  the function value through the  HepMC graph
     *
     *  E.g. one can 'accumulate' the total
     *  4-momentum of all daughter muons:
     *
     *  @code
     *
     *  const LHCb::HepMCEvents* events =
     *    get<LHCb::HEpMCEvents> ( LHCb::HepMCEventLocation::Default );
     *
     *  LoKi::LorentzVector result = LoKi::GenAlgs::accumulate
     *   ( events                           ,   // the event
     *     LoKi::Kinematics::Gen4Momentum() ,   // get 4-mometumm
     *     "mu+" == GABSID                  ,   // consider only muons
     *     LoKi::LorentzVector()            ,  // the initial value
     *     std::plus<LoKi::LorentzVector>() ) ; // operation
     *
     *  @endcode
     *
     *  @see LHCb::HepMCEvents
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::LorentzVector
     *  @see LoKi::Kinematics::Gen4Momentum
     *
     *  @param  events    pointer to HepMC::GenEvent
     *  @param  functor   function to be evaluated
     *  @param  predicate selection criteria
     *  @param  result    the initial-value for the result
     *  @return the result of accumulation
     *
     *  @author Vanya BELYAEV ibelyaev@physucs.syr.edu
     *  @date 2006-02-09
     */
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE ,
               class OPERATION >
    inline RESULT accumulate
    ( const LHCb::HepMCEvents* events    ,
      const FUNCTOR&           functor   ,
      const PREDICATE&         predicate ,
      RESULT                   result    ,
      OPERATION                binop     )
    {
      if ( !events ) { return  result ; }
      return std::accumulate( events->begin(), events->end(), result,
                              [&](RESULT r, const LHCb::HepMCEvent* ievent)
                              { return accumulate( ievent , functor , predicate , r, binop ) ; } );
    }
    // ========================================================================
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE >
    inline RESULT accumulate
    ( const LHCb::HepMCEvents* events    ,
      const FUNCTOR&           functor   ,
      const PREDICATE&         predicate ,
      RESULT                   result    )
    {
      return accumulate
        ( events , functor , predicate , result , std::plus<RESULT> () ) ;
    }
    // ========================================================================
    /** simple function for accumulatio throught the HepMC-graph
     *  @param vertex pointer to HepMC-graph
     *  @param functor    function to be accumulated
     *  @param predicate  restriction/selector for particles
     *  @param result     the initial value for accumulation
     *  @param range      the type of HepMC-graph
     *  @param binop      the operation used for accumulation
     *  @return updated accumulation result
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE ,
               class OPERATION >
    inline RESULT accumulate
    ( const HepMC::GenVertex*   vertex    ,
      const FUNCTOR&            functor   ,
      const PREDICATE&          predicate ,
      RESULT                    result    ,
      HepMC::IteratorRange      range     ,
      OPERATION                 binop     )
    {
      if ( !vertex ) { return result ; }
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      return LoKi::Algs::accumulate
        ( _v -> particles_begin ( range ) ,
          _v -> particles_end   ( range ) ,
          functor , predicate , result , binop ) ;
    }
    // ========================================================================
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE >
    inline RESULT accumulate
    ( const HepMC::GenVertex*   vertex    ,
      const FUNCTOR&            functor   ,
      const PREDICATE&          predicate ,
      RESULT                    result    ,
      HepMC::IteratorRange      range     )
    {
      return accumulate
        ( vertex , functor , predicate , result , range , std::plus<RESULT>() ) ;
    }
    // ========================================================================
    /** simple function for accumulatio throught the decay tree of the particle
     *  @param particle the pointer to mother particle
     *  @param functor    function to be accumulated
     *  @param predicate  restriction/selector for particles
     *  @param result     the initial value for accumulation
     *  @param binop      the operation used for accumulation
     *  @return updated accumulation result
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE ,
               class OPERATION >
    inline RESULT accumulate
    ( const HepMC::GenParticle* particle  ,
      const FUNCTOR&            functor   ,
      const PREDICATE&          predicate ,
      RESULT                    result    ,
      OPERATION                 binop     )
    {
      if ( particle )
      {
        result = LoKi::GenAlgs::accumulate
          ( particle->end_vertex() ,
            functor , predicate , result , HepMC::descendants , binop ) ;
      }
      if ( predicate ( particle ) )
      { result = binop ( result , functor ( particle ) ) ; }
      return result ;
    }
    // ========================================================================
    template < class RESULT    ,
               class FUNCTOR   ,
               class PREDICATE >
    inline RESULT accumulate
    ( const HepMC::GenParticle* particle  ,
      const FUNCTOR&            functor   ,
      const PREDICATE&          predicate ,
      RESULT                    result    )
    {
      return accumulate
        ( particle , functor , predicate , result , std::plus<RESULT> () ) ;
    }
    // ========================================================================
    /** simple function to extract the minimum value of certain function
     *  form HepMC-graph
     *  @param vertex     pointer to HepMC-graph
     *  @param functor    function to be evaluated
     *  @param predicate  restriction/selector for particles
     *  @param range      the type of HepMC-graph
     *  @param minval     the initial value
     *  @return updated accumulation result
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE, class RESULT>
    inline RESULT min_value
    ( const HepMC::GenVertex*   vertex   ,
      const FUNCTION&           fun      ,
      const PREDICATE&          cut      ,
      HepMC::IteratorRange      range    ,
      RESULT                    minval   )
    {
      if ( !vertex ) { return minval ; }                          // RETURN
      // investigate the vertex
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      auto ifound = LoKi::Algs::select_min( _v->particles_begin ( range ) ,
                                            _v->particles_end   ( range ) ,
                                            fun , cut ) ;
      if ( _v->particles_end ( range ) == ifound ) { return minval ; } // RETURN
      // check the minimum:
      minval = std::min ( minval , fun ( *ifound ) ) ;
      return minval ;                                                  // RETURN
    }
    // ========================================================================
    /** The trivial algorithm which scans the decay
     *  tree of the particle and searches for the the
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria
     *
     *  @param particle the particle
     *  @param fun      function to be evaluated
     *  @param cut      the criteria
     *  @param minval   minimal value
     *  @return updated minimal value
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-02-20
     */
    template <class FUNCTION,class PREDICATE, class RESULT>
    inline RESULT min_value
    ( const HepMC::GenParticle* particle ,
      const FUNCTION&           fun      ,
      const PREDICATE&          cut      ,
      RESULT                    minval   )
    {
      // (1) traverse the tree if possible
      if ( particle )
      {
        // check all end-vertices :
        minval = LoKi::GenAlgs::min_value
          ( particle -> end_vertex () ,
            fun , cut , HepMC::descendants , minval ) ; // RECURSION!
      }
      // (2) check itself
      if ( cut ( particle ) )
      { minval= std::min ( minval , fun ( particle ) ) ; }
      //
      return minval ;                                        // RETURN
    }
    // ========================================================================
    /** simple function to extract the maximum value of certain function
     *  form HepMC-graph
     *  @param vertex     pointer to HepMC-graph
     *  @param functor    function to be evaluated
     *  @param predicate  restriction/selector for particles
     *  @param range      the type of HepMC-graph
     *  @param minval     the initial value
     *  @return updated maximum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE, class RESULT>
    inline RESULT max_value
    ( const HepMC::GenVertex*   vertex   ,
      const FUNCTION&           fun      ,
      const PREDICATE&          cut      ,
      HepMC::IteratorRange      range    ,
      RESULT                    maxval   )
    {
      if ( !vertex ) { return maxval ; }                          // RETURN
      // investigate the vertex
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      auto ifound = LoKi::Algs::select_max( _v->particles_begin ( range ) ,
                                            _v->particles_end   ( range ) ,
                                            fun , cut ) ;
      // check the maximum
      return  ( _v->particles_end(range)!=ifound) ? std::max ( maxval , fun ( *ifound ) )
                                                  : maxval ;
    }
    // ========================================================================
    /** The trivial algorithm which scans the decay
     *  tree of the particle and searches for the the
     *  maximal value for some functions for
     *  particles which satisfy the certain criteria
     *
     *  @param particle the particle
     *  @param fun      function to be evaluated
     *  @param cut      the criteria
     *  @param minval   minimal value
     *  @return updated minimal value
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-02-20
     */
    template <class FUNCTION,class PREDICATE, class RESULT>
    inline RESULT max_value
    ( const HepMC::GenParticle* particle ,
      const FUNCTION&           fun      ,
      const PREDICATE&          cut      ,
      RESULT                    maxval   )
    {
      // (1) traverse the tree if possible
      if ( particle )
      {
        // check for endvertices
        maxval = LoKi::GenAlgs::max_value
          ( particle -> end_vertex () ,
            fun , cut , HepMC::descendants , maxval ) ; // RECURSION!
      }
      // (2) check itself
      return cut(particle) ? std::max( maxval, fun(particle))
                           : maxval;
    }
    // ========================================================================
    /** simple function to extract the mminimul value of certain function
     *  for whole HepMC-event
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param minval     the initial value
     *  @return updated minimum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT min_value
    ( const HepMC::GenEvent* event  ,
      const FUNCTION&        fun    ,
      const PREDICATE&       cut    ,
      RESULT                 minval )
    {
      if ( !event ) { return minval ; }                    // RETURN
      //
      auto ifound = LoKi::Algs::select_min( event->particles_begin () ,
                                            event->particles_end   () ,
                                            fun , cut ) ;
      return ifound != event->particles_end() ? std::min( minval, fun(*ifound) )
                                              : minval ;
    }
    // ========================================================================
    /** simple function to extract the mminimul value of certain function
     *  for whole HepMC-event
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param minval     the initial value
     *  @return updated minimum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT min_value
    ( const LHCb::HepMCEvent* event  ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      RESULT                  minval )
    {
      return event ? LoKi::GenAlgs::min_value ( event->pGenEvt() , fun , cut , minval )
                   : minval;
    }
    // ========================================================================
    /** simple function to extract the mminimul value of certain function
     *  for whole HepMC-super-event
     *  @param event      pointer to super-event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param minval     the initial value
     *  @return updated minimum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT min_value
    ( const LHCb::HepMCEvent::Container* events ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      RESULT                  minval )
    {
      if ( !events ) { return minval ; }                    // RETURN
      return std::accumulate( events->begin(), events->end(), minval,
                              [&](RESULT m, const LHCb::HepMCEvent* iev) {
            return LoKi::GenAlgs::min_value ( iev , fun , cut , m);
      });
    }
    // ========================================================================
    /** simple function to extract the maximum value of certain function
     *  for whole HepMC event
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param maxval     the initial value of maximum
     *  @return updated maximum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT max_value
    ( const HepMC::GenEvent* event  ,
      const FUNCTION&        fun    ,
      const PREDICATE&       cut    ,
      RESULT                 maxval )
    {
      if ( event ) { return maxval ; }                    // RETURN
      //
      auto ifound = LoKi::Algs::select_max( event->particles_begin () ,
                                            event->particles_end   () ,
                                            fun , cut ) ;
      return ifound!=event->particles_end() ? std::max( maxval, fun(*ifound) )
                                            : maxval;
    }
    // ========================================================================
    /** simple function to extract the maximum value of certain function
     *  for whole HepMC event
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param maxval     the initial value of maximum
     *  @return updated maximum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT max_value
    ( const LHCb::HepMCEvent* event  ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      RESULT                  maxval )
    {
      return event ? LoKi::GenAlgs::max_value ( event->pGenEvt() , fun , cut , maxval )
                   : maxval;
    }
    // ========================================================================
    /** simple function to extract the maximum value of certain function
     *  for whole HepMC super-event
     *  @param events     pointer to super-event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param maxval     the initial value of maximum
     *  @return updated maximum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE,class RESULT>
    inline RESULT max_value
    ( const LHCb::HepMCEvent::Container* events ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      RESULT                  maxval )
    {
      if ( !events ) { return maxval ; }                    // RETURN
      return std::accumulate( events->begin(), events->end(), maxval,
                              [&](RESULT m, const LHCb::HepMCEvent* iev) {
            return LoKi::GenAlgs::max_value ( iev , fun , cut , m) ;
      });
    }
    // ========================================================================
    /** simple function to extract the particle, which minimizes a
     *  certain function
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @return particle which mimimizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  min_element
    ( const HepMC::GenEvent* event ,
      const FUNCTION&        fun   ,
      const PREDICATE&       cut   )
    {
      if ( !event ) { return nullptr ; }                          // RETURN
      auto ifound = LoKi::Algs::select_min( event->particles_begin () ,
                                            event->particles_end   () ,
                                            fun , cut ) ;
      return ifound != event->particles_end() ? *ifound : nullptr;
    }
    // ========================================================================
    /** simple function to extract the particle, which minimizes a
     *  certain function
     *  @param event      pointer to event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @return particle which mimimizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  min_element
    ( const LHCb::HepMCEvent* event ,
      const FUNCTION&         fun   ,
      const PREDICATE&        cut   )
    {
      if ( !event ) { return nullptr ; }                          // RETURN
      return LoKi::GenAlgs::min_element ( event->pGenEvt() , fun , cut ) ;
    }
    // ========================================================================
    /** simple function to extract the particle, which minimizes a
     *  certain function
     *  @param event      pointer to super-event
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @return particle which mimimizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  min_element
    ( const LHCb::HepMCEvent::Container* event ,
      const FUNCTION&         fun   ,
      const PREDICATE&        cut   )
    {
      if ( !event ) { return nullptr ; }                          // RETURN
      const HepMC::GenParticle* result = nullptr ;
      return std::accumulate( event->begin(), event->end(), result,
                              [&](const HepMC::GenParticle* r, const LHCb::HepMCEvent* iev) {
          const auto* tmp = LoKi::GenAlgs::min_element ( iev , fun , cut ) ;
          return ( !r || fun( tmp ) < fun( r ) ) ? tmp : r;
      });
    }
    // ========================================================================
    /** simple function to extract the particle, which minimizes a
     *  certain function
     *  @param vertex     pointer to HepMC-graph
     *  @param fun        function to be evaluated
     *  @param cut        restriction/selector for particles
     *  @param range      type of HepMC-graph
     *  @return particle which mimimizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  min_element
    ( const HepMC::GenVertex* vertex ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      HepMC::IteratorRange    range  )
    {
      if ( !vertex  ) { return nullptr ; }                            // RETURN
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      auto ifound = LoKi::Algs::select_min( _v -> particles_begin ( range ) ,
                                            _v -> particles_end   ( range ) ,
                                            fun , cut ) ;
      return ifound!=_v->particles_end(range) ? *ifound : nullptr;    // RETURN
    }
    // ========================================================================
    /** simple function to extract the particle, which minimizes a
     *  certain function
     *  @param particle pointer to mother particle
     *  @param fun      function to be evaluated
     *  @param cut      restriction/selector for particles
     *  @return particle which mimimizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  min_element
    ( const HepMC::GenParticle* particle ,
      const FUNCTION&           fun    ,
      const PREDICATE&          cut    )
    {
      if ( !particle                ) { return nullptr  ; } // RETURN
      const HepMC::GenParticle* tmp = LoKi::GenAlgs::min_element
        ( particle->end_vertex() , fun , cut , HepMC::descendants ) ;
      if ( !tmp && cut ( particle ) ) { return particle ; } // RETURN
      if ( !tmp                     ) { return nullptr  ; } // RETURN
      HepMC::GenParticle* _t1 = const_cast<HepMC::GenParticle*> ( tmp      ) ;
      HepMC::GenParticle* _t2 = const_cast<HepMC::GenParticle*> ( particle ) ;
      return fun(_t1) < fun(_t2) ? tmp : particle ;
    }
    // ========================================================================
    /** simple function to extract the particle, which maximizes a
     *  certain function
     *  @param event pointer to the event
     *  @param fun   function to be evaluated
     *  @param cut   restriction/selector for particles
     *  @return particle which maximizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  max_element
    ( const HepMC::GenEvent* event ,
      const FUNCTION&        fun   ,
      const PREDICATE&       cut   )
    {
      if ( !event ) { return nullptr ; }                          // RETURN
      auto ifound = LoKi::Algs::select_max( event->particles_begin () ,
                                            event->particles_end   () ,
                                            fun , cut ) ;
      return ifound!=event->particles_end() ? *ifound : nullptr; // RETURN
    }
    // ========================================================================
    /** simple function to extract the particle, which maximizes a
     *  certain function
     *  @param event pointer to the event
     *  @param fun   function to be evaluated
     *  @param cut   restriction/selector for particles
     *  @return particle which maximizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  max_element
    ( const LHCb::HepMCEvent* event ,
      const FUNCTION&         fun   ,
      const PREDICATE&        cut   )
    {
      return event ? LoKi::GenAlgs::max_element ( event->pGenEvt() , fun , cut )
                   : nullptr;
    }
    // ========================================================================
    /** simple function to extract the particle, which maximizes a
     *  certain function
     *  @param event pointer to the super-event
     *  @param fun   function to be evaluated
     *  @param cut   restriction/selector for particles
     *  @return particle which maximizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  max_element
    ( const LHCb::HepMCEvent::Container* event ,
      const FUNCTION&         fun   ,
      const PREDICATE&        cut   )
    {
      if ( !event ) { return nullptr ; }                          // RETURN
      HepMC::GenParticle* result = nullptr ;
      for ( auto iev = event->begin() ; event->end() != iev ; ++iev )
      {
        const HepMC::GenParticle* tmp =
          LoKi::GenAlgs::max_element ( *iev , fun , cut ) ;    // RECURSION
        auto _tmp = const_cast<HepMC::GenParticle*> ( tmp ) ;
        if ( !result || fun ( result ) < fun ( _tmp ) ) { result  = _tmp ; }
      }
      return result ;
    }
    // ========================================================================
    /** simple function to extract the particle, which maximizes a
     *  certain function
     *  @param vertex pointer to HepMC-graph
     *  @param fun    function to be evaluated
     *  @param cut    restriction/selector for particles
     *  @param range      type of HepMC-graph
     *  @return particle which maximizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  max_element
    ( const HepMC::GenVertex* vertex ,
      const FUNCTION&         fun    ,
      const PREDICATE&        cut    ,
      HepMC::IteratorRange    range  )
    {
      if ( 0 == vertex  ) { return 0 ; }                            // RETURN
      HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      HepMC::GenVertex::particle_iterator ifound = LoKi::Algs::select_max
        ( _v -> particles_begin ( range ) ,
          _v -> particles_end   ( range ) , fun , cut ) ;
      if ( _v -> particles_end ( range ) == ifound ) { return 0 ; } // RETURN
      return *ifound ;                                              // RETURN
    }
    // ========================================================================
    /** simple function to extract the particle, which maximizes a
     *  certain function
     *  @param particle pointer to the mother particle
     *  @param fun      function to be evaluated
     *  @param cut      restriction/selector for particles
     *  @return particle which maximizes the function
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     */
    template <class FUNCTION,class PREDICATE>
    inline const HepMC::GenParticle*  max_element
    ( const HepMC::GenParticle* particle ,
      const FUNCTION&           fun    ,
      const PREDICATE&          cut    )
    {
      if ( 0 == particle                ) { return 0        ; } // RETURN
      const HepMC::GenParticle* tmp = LoKi::GenAlgs::max_element
        ( particle->end_vertex() , fun , cut , HepMC::descendants ) ;
      if ( 0 == tmp && cut ( particle ) ) { return particle ; } // RETURN
      if ( 0 == tmp                     ) { return 0        ; } // RETURN
      HepMC::GenParticle* _t1 = const_cast<HepMC::GenParticle*> ( tmp      ) ;
      HepMC::GenParticle* _t2 = const_cast<HepMC::GenParticle*> ( particle ) ;
      return fun(_t2) < fun(_t1) ? tmp : particle ;
    }
    // ========================================================================
  } //                                       The end of namespace LoKi::GenAlgs
  // ==========================================================================
} //                                                  The end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENALGS_H
// ============================================================================
