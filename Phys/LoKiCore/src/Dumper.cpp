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
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <iostream>
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Dumper.h"
// ============================================================================
/** @file
 *  Implementation file for classes form namesapce LoKi::Functors
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya Belyaev Ivan.BElyaev@itep.ru
 *  @date 2012-01-28
 */
// ============================================================================
template <>
bool LoKi::Functors::Dump1_<double,bool>::operator() ( double a ) const
{
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    m_stream << a ;
    m_stream << m_dump.close () ;
    return m_fun.fun ( a ) ;
  }
  //
  auto result = m_fun.fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  m_stream << a ;
  m_stream << m_dump.close () ;
  //
  return result ;
}
// ============================================================================
template <>
double LoKi::Functors::Dump1_<double,double>::operator()( double a ) const
{
  if ( m_right )
  {
    m_stream << m_dump.open  () ;
    m_stream << a ;
    m_stream << m_dump.close () ;
    return m_fun.fun ( a ) ;
  }
  //
  auto result = m_fun.fun ( a ) ;
  //
  m_stream << m_dump.open  () ;
  m_stream << a ;
  m_stream << m_dump.close () ;
  //
  return result ;
}
// ============================================================================
// The END
// ============================================================================
