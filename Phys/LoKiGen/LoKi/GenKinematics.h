// $Id: GenKinematics.h,v 1.5 2006-11-25 19:14:19 ibelyaev Exp $
// ============================================================================
// CVS tag $Name: not supported by cvs2svn $ 
// ============================================================================
// $Log: not supported by cvs2svn $
// Revision 1.4  2006/05/02 14:30:27  ibelyaev
//  censored
//
// ============================================================================
#ifndef LOKI_GENKINEMATICS_H 
#define LOKI_GENKINEMATICS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore 
// ============================================================================
#include "LoKi/Kinematics.h"
// ============================================================================
// HepMC 
// ============================================================================
#include "HepMC/GenParticle.h"
// ============================================================================


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
 *  @date 2001-01-23 
 */
// ============================================================================

// ============================================================================
namespace LoKi 
{  
  namespace Kinematics 
  { 
    /** @fn momentum
     *  The most trivial function.
     *  It seems to be almost useless from the first sight, but 
     *  effectivel it is useful in conjunction with 
     *  algorithms, acting as "converter" of the particle 
     *  into the 4-momentum 
     *  @param p particle 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    inline LoKi::LorentzVector momentum
    ( const HepMC::GenParticle* p )
    {
      if ( 0 == p ) { return LoKi::LorentzVector() ; }
      return LoKi::LorentzVector ( p->momentum () ) ;
    };
    
    /** @struct Gen4Momentum 
     *  the simple object which acts as a converter of HepMC::GenPartile
     *  to LoKi::LorentzVector 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    struct Gen4Momentum : 
      public std::unary_function<const HepMC::GenParticle*,LoKi::LorentzVector>
    {
      inline LoKi::LorentzVector operator() 
        ( const HepMC::GenParticle* p ) const { return momentum ( p ) ; }  
    };
    
    /** @fn mass 
     *  trivial function to evaluate the mass HepMC::GenParticle
     *  @param  p particle 
     *  @return invariant mass 
     *  @see HepMC::GenParticle 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double mass 
    ( const HepMC::GenParticle* p ) ;
    
    /** @fn mass 
     *  trivial function to evaluate the mass HepMC::GenParticle
     *  @param  p1 the first particle 
     *  @param  p2 the second particle 
     *  @return invariant mass 
     *  @see HepMC::GenParticle 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double mass 
    ( const HepMC::GenParticle* p1 , 
      const HepMC::GenParticle* p2 ) ;

    /** @fn mass 
     *  trivial function to evaluate the mass HepMC::GenParticle
     *  @param  p1 the first particle 
     *  @param  p2 the third particle 
     *  @param  p3 the second particle 
     *  @return invariant mass 
     *  @see HepMC::GenParticle 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double mass 
    ( const HepMC::GenParticle* p1 , 
      const HepMC::GenParticle* p2 ,
      const HepMC::GenParticle* p3 ) ;
    
    /** @fn mass 
     *  trivial function to evaluate the mass HepMC::GenParticle
     *  @param  p1 the first particle 
     *  @param  p2 the second particle 
     *  @param  p3 the third  particle 
     *  @param  p3 the fourth  particle 
     *  @return invariant mass 
     *  @see HepMC::GenParticle 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-17
     */
    double mass 
    ( const HepMC::GenParticle* p1 , 
      const HepMC::GenParticle* p2 ,
      const HepMC::GenParticle* p3 ,
      const HepMC::GenParticle* p4 ) ;

  } ; // end of namespace LoKi::Kinematics 
  
} ; // end of namespace LoKi 
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_GENKINEMATICS_H
// ============================================================================
