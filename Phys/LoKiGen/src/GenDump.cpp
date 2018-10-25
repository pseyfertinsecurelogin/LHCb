/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 *
 */
// ============================================================================
/*  template specialization of ``dumper''
 *  @see LoKi::Functors::Dump_
 *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
 *  @date   2011-06-03
 */
// ============================================================================
template <>
std::vector<const HepMC::GenParticle*>
LoKi::Functors::Dump_<const HepMC::GenParticle*>::operator()
  ( const std::vector<const HepMC::GenParticle*>& a ) const
{
  //
  const unsigned int mx = std::min ( m_dump.nMax() , a.size() ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::GenDecayChain printer ;
  printer.print ( a.begin ()             ,
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
/*  template specialization of ``dumpers''
 *  @see LoKi::Functors::Dump1_
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2012-01-28
 */
// ============================================================================
template <>
bool
LoKi::Functors::Dump1_<const HepMC::GenParticle*,bool>::operator()
  ( const HepMC::GenParticle* a ) const
{
  //
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    //
    LoKi::GenDecayChain printer ;
    printer.print ( a                      ,
                    m_stream               ,
                    '\n'                   ,
                    LoKi::Objects::_VALID_ ,
                    LoKi::Objects::_NONE_  ,
                    " "                    ,
                    0                      ) ;
    //
    m_stream << m_dump.close () ;
    //
    return m_fun ( a ) ;
  }
  //
  auto result = m_fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::GenDecayChain printer ;
  printer.print ( a                      ,
                  m_stream               ,
                  '\n'                   ,
                  LoKi::Objects::_VALID_ ,
                  LoKi::Objects::_NONE_  ,
                  " "                    ,
                  0                      ) ;
  //
  m_stream << m_dump.close () ;
  //
  return result ;

}
// ============================================================================
/*  template specialization of ``dumpers''
 *  @see LoKi::Functors::Dump1_
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   2012-01-28
 */
// ============================================================================
template <>
double
LoKi::Functors::Dump1_<const HepMC::GenParticle*,double>::operator()
  ( const HepMC::GenParticle* a ) const
{
  //
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    //
    LoKi::GenDecayChain printer ;
    printer.print ( a                      ,
                    m_stream               ,
                    '\n'                   ,
                    LoKi::Objects::_VALID_ ,
                    LoKi::Objects::_NONE_  ,
                    " "                    ,
                    0                      ) ;
    //
    m_stream << m_dump.close () ;
    //
    return m_fun ( a ) ;
  }
  //
  auto result = m_fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  //
  LoKi::GenDecayChain printer ;
  printer.print ( a                      ,
                  m_stream               ,
                  '\n'                   ,
                  LoKi::Objects::_VALID_ ,
                  LoKi::Objects::_NONE_  ,
                  " "                    ,
                  0                      ) ;
  //
  m_stream << m_dump.close () ;
  //
  return result ;

}
// ============================================================================
// The END
// ============================================================================
