// $Id: MCDump.cpp 134318 2012-01-29 17:46:59Z ibelyaev $
// ============================================================================
// Include files 
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Objects.h"
#include "LoKi/MCDump.h"
#include "LoKi/MCDecayChain.h"
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
 *                    $Revision: 134318 $
 *  Last modification $Date: 2012-01-29 18:46:59 +0100 (Sun, 29 Jan 2012) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
/*  template specialization of ``dumper''
 *  @see LoKi::Fuctors::Dump_
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */  
// ============================================================================
template <>
LoKi::Functors::Dump_<const LHCb::MCParticle*>::result_type 
LoKi::Functors::Dump_<const LHCb::MCParticle*>::operator() 
  ( LoKi::Functors::Dump_<const LHCb::MCParticle*>::argument a ) const 
{
  //
  const unsigned int mx = std::min ( m_dump.nMax() , a.size() ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::MCDecayChain printer ;
  printer.print
    ( a.begin ()                          , 
      a.begin () + mx                     , 
      m_stream                            , 
      '\n'                                , 
      LoKi::Objects::_VALID_              , 
      LoKi::Objects::_NONE_               , 
      " "                                 , 
      0                                   ) ;
  //
  m_stream << m_dump.close () ;
  //
  return a ;
}
// ============================================================================
template <>
LoKi::Functors::Dump1_<const LHCb::MCParticle*,bool>::result_type 
LoKi::Functors::Dump1_<const LHCb::MCParticle*,bool>::operator() 
  ( LoKi::Functors::Dump1_<const LHCb::MCParticle*,bool>::argument a ) const 
{
  //
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    //
    LoKi::MCDecayChain printer ;
    printer.print
      ( a                                   , 
        m_stream                            , 
        '\n'                                , 
        LoKi::Objects::_VALID_              , 
        LoKi::Objects::_NONE_               , 
        " "                                 , 
        0                                   ) ;
    //
    m_stream << m_dump.close () ;
    //
    return m_fun ( a ) ;
  }
  //
  result_type result = m_fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::MCDecayChain printer ;
  printer.print
    ( a                                   ,
      m_stream                            , 
      '\n'                                , 
      LoKi::Objects::_VALID_              , 
      LoKi::Objects::_NONE_               , 
        " "                                 , 
      0                                   ) ;
  //
  m_stream << m_dump.close () ;
  //
  return result ;
}
// ============================================================================
template <>
LoKi::Functors::Dump1_<const LHCb::MCParticle*,double>::result_type 
LoKi::Functors::Dump1_<const LHCb::MCParticle*,double>::operator() 
  ( LoKi::Functors::Dump1_<const LHCb::MCParticle*,double>::argument a ) const 
{
  //
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    //
    LoKi::MCDecayChain printer ;
    printer.print
      ( a                                   ,
        m_stream                            , 
        '\n'                                , 
        LoKi::Objects::_VALID_              , 
        LoKi::Objects::_NONE_               , 
        " "                                 , 
        0                                   ) ;
    //
    m_stream << m_dump.close () ;
    //
    return m_fun ( a ) ;
  }
  //
  result_type result = m_fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::MCDecayChain printer ;
  printer.print
    ( a                                   ,
      m_stream                            , 
      '\n'                                , 
      LoKi::Objects::_VALID_              , 
      LoKi::Objects::_NONE_               , 
        " "                                 , 
      0                                   ) ;
  //
  m_stream << m_dump.close () ;
  //
  return result ;
}




// ============================================================================
// The END 
// ============================================================================
