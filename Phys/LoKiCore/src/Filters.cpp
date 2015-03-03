// $Id: Filters.cpp 134317 2012-01-29 17:46:17Z ibelyaev $
// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Primitives.h"
#include "LoKi/Filters.h"
#include "LoKi/Scalers.h"
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
 *  By usage of this code one clearly states the disagreement 
 *  with the smear campaign of Dr.O.Callot et al.: 
 *  ``No Vanya's lines are allowed in LHCb/Gaudi software.''
 *
 *  @author Vanya Belyaev Ivan.BElyaev@cern.ch
 *  @date 2010-11-17 
 *
 *                    $Revision: 134317 $
 *  Last modification $Date: 2012-01-29 18:46:17 +0100 (Sun, 29 Jan 2012) $
 *                 by $Author: ibelyaev $
 */
// ============================================================================
template <>
LoKi::Functors::Dump_<double>::result_type 
LoKi::Functors::Dump_<double>::operator() 
  ( LoKi::Functors::Dump_<double>::argument a ) const 
{
  m_stream << m_dump.open  () ;
  //
  if ( a.size() <= m_dump.nMax() ) 
  {
    Gaudi::Utils::toStream 
      ( a.begin() , a.end() , m_stream , "[ " , " ]" , " , " ) ;
  }
  else 
  {
    Gaudi::Utils::toStream 
      ( a.begin() , a.begin() + m_dump.nMax() , 
        m_stream , "[ " , " , ... ]" , " , " ) ;
  }
  //
  m_stream << m_dump.close () ;
  return a ;
}
// ============================================================================
// The END 
// ============================================================================
