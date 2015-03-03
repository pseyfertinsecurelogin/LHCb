// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// LoKiCore 
// ============================================================================
#include "LoKi/Dump.h"
#include "LoKi/ToCpp.h"
// ============================================================================
// Boots 
// ============================================================================
#include "boost/integer_traits.hpp"
#include "boost/static_assert.hpp"
// ============================================================================
/** @file 
 *  Implementation fiel for class LoKi::Dump
 *  @see LoKi::Dump
 *
 *  This file is a part of LoKi project - 
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas, 
 *  contributions and advices from G.Raven, J.van Tilburg, 
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2010-11-18
 */
// ============================================================================
LoKi::Dump::Dump
( const std::string& open  ,
  const std::string& close )
  : m_open  ( open ) 
  , m_close ( close ) 
  , m_nmax  ( boost::integer_traits<std::size_t>::const_max )
{
  BOOST_STATIC_ASSERT( boost::integer_traits<std::size_t> :: is_specialized && 
                       boost::integer_traits<std::size_t> :: is_integral    && 
                       boost::integer_traits<std::size_t> :: const_min == 0 && 
                       boost::integer_traits<std::size_t> :: const_max  > 0 ) ; 
}
// =============================================================================
LoKi::Dump::Dump
( const std::size_t  nmax  , 
  const std::string& open  ,
  const std::string& close )
  : m_open  ( open  ) 
  , m_close ( close ) 
  , m_nmax  ( nmax  )
{
  BOOST_STATIC_ASSERT( boost::integer_traits<std::size_t> :: is_specialized && 
                       boost::integer_traits<std::size_t> :: is_integral    && 
                       boost::integer_traits<std::size_t> :: const_min == 0 && 
                       boost::integer_traits<std::size_t> :: const_max  > 0 ) ; 
}// =============================================================================
LoKi::Dump::Dump
( const std::string& open  ,
  const std::string& close ,
  const std::size_t  nmax  )
  : m_open  ( open  ) 
  , m_close ( close ) 
  , m_nmax  ( nmax  )
{
  BOOST_STATIC_ASSERT( boost::integer_traits<std::size_t> :: is_specialized && 
                       boost::integer_traits<std::size_t> :: is_integral    && 
                       boost::integer_traits<std::size_t> :: const_min == 0 && 
                       boost::integer_traits<std::size_t> :: const_max  > 0 ) ; 
}
// =============================================================================
LoKi::Dump::Dump
( const std::string& open  ,
  const std::size_t  nmax  , 
  const std::string& close )
  : m_open  ( open  ) 
  , m_close ( close ) 
  , m_nmax  ( nmax  )
{
  BOOST_STATIC_ASSERT( boost::integer_traits<std::size_t> :: is_specialized && 
                       boost::integer_traits<std::size_t> :: is_integral    && 
                       boost::integer_traits<std::size_t> :: const_min == 0 && 
                       boost::integer_traits<std::size_t> :: const_max  > 0 ) ; 
}
// ============================================================================
LoKi::Dump::~Dump(){}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::Dump& dump ) 
{
  return 
    "LoKi::Dump(" 
    + Gaudi::Utils::toCpp ( dump.open  () ) + ", "
    + Gaudi::Utils::toCpp ( dump.close () ) + ", "
    + Gaudi::Utils::toCpp ( dump.nMax  () ) + ") " ;
}
// ============================================================================
// The END 
// ============================================================================
