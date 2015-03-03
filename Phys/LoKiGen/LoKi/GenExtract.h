// $Id: GenExtract.h,v 1.6 2006-10-10 09:04:49 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ , version $Revision: 1.6 $ 
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.5  2006/06/09 09:01:46  cattanem
// fix typo
//
// Revision 1.4  2006/05/02 14:30:27  ibelyaev
//  censored
//
// ============================================================================
#ifndef LOKI_GENEXTRACT_H 
#define LOKI_GENEXTRACT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// Event 
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/select.h"
// ============================================================================

// ============================================================================
/** @file
 *
 *  Collection of useful function to extract the information from  HepMC 
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
 *  @date 2001-01-23 
 */
// ============================================================================

namespace LoKi 
{
  namespace Extract
  {
    /** @fn getGenParticles 
     *  simple function which allow to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const HepMC::GenEvent* event = ... ; 
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::getGenParticles 
     *     ( event                          , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see HepMC::GenEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT getGenParticles 
    ( const HepMC::GenEvent* event     , 
      OUTPUT                 output    , 
      const PREDICATE&       predicate )
    {
      if ( 0 == event ) { return output ; }
      for ( HepMC::GenEvent::particle_const_iterator p = 
              event->particles_begin() ; 
            event->particles_end() != p ; ++p ) 
      { 
        if ( !predicate ( *p ) ) { continue ; } // CONTINUE 
        //
        const HepMC::GenParticle* particle = *p ;
        *output = const_cast<HepMC::GenParticle*> ( particle ) ;
        ++output ;                                              // NB!
        //
      }
      return output ;
    };
    
    /** @fn getGenParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const LHCb::HepMCEvent* event = ... ; 
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::getGenParticles 
     *     ( event                          , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see LHCb::HepMCEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT getGenParticles 
    ( const LHCb::HepMCEvent*  event     , 
      OUTPUT                   output    , 
      const PREDICATE&         predicate )
    {
      if ( 0 == event ) { return output ; }
      return getGenParticles ( event->pGenEvt() , output , predicate ) ;
    };
    
    /** @fn getGenParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const LHCb::HepMCEvent* event = 
     *      get<LHCb::HepMCEvents>( LHCb::HepMcEventLocation::Default ) ;
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::getGenParticles 
     *     ( events                         , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see LHCb::HepMCEvent
     *  @see LHCb::HepMCEvents
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT getGenParticles 
    ( const LHCb::HepMCEvents* events    , 
      OUTPUT                   output    , 
      const PREDICATE&         predicate )
    {
      if ( 0 == events ) { return output ; }
      return getGenParticles 
        ( events->begin() , events->end () , output , predicate ) ; 
    };
    
    /** @fn getGenParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC vertex 
     *   
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT getGenParticles 
    ( const HepMC::GenVertex* vertex    , 
      HepMC::IteratorRange    range     ,
      OUTPUT                  output    , 
      const PREDICATE&        predicate ) 
    {
      if ( 0 == vertex ) { return output ; }            // RETURN
      HepMC::GenVertex* v = const_cast<HepMC::GenVertex*> ( vertex ) ;
      if ( 0 == v      ) { return output ; }            // RETURN
      //
      return  LoKi::select
        ( v->particles_begin( range ) , 
          v->particles_end  ( range ) , output , predicate ) ;
    };
    
    /** @fn getGenParticles 
     *  simple function which allow to extract a certain 
     *  particles from HepMC graph.
     *   
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see HepMC::GenEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class INPUT,class OUTPUT, class PREDICATE> 
    inline OUTPUT getGenParticles 
    ( INPUT            first     ,
      INPUT            last      , 
      OUTPUT           output    , 
      const PREDICATE& predicate )
    {
      for ( ; first != last ; ++first ) 
      { output = getGenParticles ( *first , output , predicate ) ; }  
      return output ;
    };

    /** @fn genParticles 
     *  simple function which allow to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const HepMC::GenEvent* event = ... ; 
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::genParticles 
     *     ( event                          , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see HepMC::GenEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT genParticles 
    ( const HepMC::GenEvent* event     , 
      OUTPUT                 output    , 
      const PREDICATE&       predicate )
    { return getGenParticles ( event , output , predicate ) ; }
    
    /** @fn genParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const LHCb::HepMCEvent* event = ... ; 
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::genParticles 
     *     ( event                          , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see LHCb::HepMCEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT genParticles 
    ( const LHCb::HepMCEvent*  event     , 
      OUTPUT                   output    , 
      const PREDICATE&         predicate )
    { return getGenParticles ( event , output , predicate ) ; } ;
    
    /** @fn genParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC graph.
     *   
     *  e.g. one can get all  b(and anti-b)quarks from 
     *  higgs decay
     *  
     *  @code 
     *
     *  const LHCb::HepMCEvent* event = 
     *      get<LHCb::HepMCEvents>( LHCb::HepMcEventLocation::Default ) ;
     *  
     *  SEQUENCE bquarks ;
     *  LoKi::Extract::genParticles 
     *     ( events                         , 
     *       std::back_inserter( bquarks )  , 
     *       ( "b" == GABSID ) && 
     *       0 != GNINTREE( "H_10" == GABSID , HepMC::parents ) ) ;
     *
     *  @endcode 
     * 
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see LHCb::HepMCEvent
     *  @see LHCb::HepMCEvents
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT genParticles 
    ( const LHCb::HepMCEvents* events    , 
      OUTPUT                   output    , 
      const PREDICATE&         predicate )
    { return getGenParticles ( events , output , predicate ) ; } ;
    
    /** @fn genParticles 
     *  simple function which allows to extract a certain 
     *  particles from HepMC vertex 
     *   
     *  @see HepMC::GenParticle
     *  @see HepMC::GenVertex
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class OUTPUT, class PREDICATE> 
    inline OUTPUT genParticles 
    ( const HepMC::GenVertex* vertex    , 
      HepMC::IteratorRange    range     ,
      OUTPUT                  output    , 
      const PREDICATE&        predicate ) 
    { return getGenParticles ( vertex , range , output , predicate ) ; } ;

    /** @fn genParticles 
     *  simple function which allow to extract a certain 
     *  particles from HepMC graph.
     *   
     *  @see LoKi::Cuts::GABSID
     *  @see LoKi::Cuts::GNINTREE
     *  @see HepMC::GenEvent
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-03-26
     */
    template <class INPUT,class OUTPUT, class PREDICATE> 
    inline OUTPUT genParticles 
    ( INPUT            first     ,
      INPUT            last      , 
      OUTPUT           output    , 
      const PREDICATE& predicate )
    { return getGenParticles ( first , last , output , predicate ) ; } ;    
  };
};

// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_GENEXTRACT_H
// ============================================================================
