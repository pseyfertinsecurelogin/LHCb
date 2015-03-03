// $Id: GenDump.cpp 124235 2011-06-05 12:25:53Z ibelyaev $
// ============================================================================
// Include files 
// ============================================================================
// STD & STL 
// ============================================================================
#include <sstream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Objects.h"
#include "LoKi/GenDump.h"
#include "LoKi/GenDecayChain.h"
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
 *  By usage of this code one clearly states the disagreement 
 *  with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 * 
 *                    $Revision: 124235 $
 *  Last modification $Date: 2011-06-05 14:25:53 +0200 (Sun, 05 Jun 2011) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
/*  template specialization of ``dumper''
 *  @see LoKi::Functors::Dump_
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */  
// ============================================================================
template <>
LoKi::Functors::Dump_<const HepMC::GenParticle*>::result_type 
LoKi::Functors::Dump_<const HepMC::GenParticle*>::operator() 
  ( LoKi::Functors::Dump_<const HepMC::GenParticle*>::argument a ) const 
{
  //
  const unsigned int mx = std::min ( m_dump.nMax() , a.size() ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::GenDecayChain printer ;
  printer.print
    ( a.begin ()             , 
      a.begin () + mx        , 
      m_stream               , 
      '\n'                   , 
      LoKi::Objects::_VALID_ , 
      LoKi::Objects::_NONE_  , 
      " "                    , 
      0                      ) ;
  //
  m_stream << m_dump.close () ;
  //
  return a ;
}
// ============================================================================
// The END 
// ============================================================================
