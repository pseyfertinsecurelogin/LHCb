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
// GaudiKernel
// LoKiCore
// ============================================================================
#include <utility>

#include "LoKi/Dump.h"
#include "LoKi/ToCpp.h"
// ============================================================================
// Boots
// ============================================================================
#include "boost/integer_traits.hpp"
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
LoKi::Dump::Dump( std::string open, std::string close )
    : m_open( std::move( open ) )
    , m_close( std::move( close ) )
    , m_nmax( boost::integer_traits<std::size_t>::const_max ) {
  static_assert(
      boost::integer_traits<std::size_t>::is_specialized && boost::integer_traits<std::size_t>::is_integral &&
          boost::integer_traits<std::size_t>::const_min == 0 && boost::integer_traits<std::size_t>::const_max > 0,
      "size_t must be pos def integral type" );
}
// =============================================================================
LoKi::Dump::Dump( const std::size_t nmax, std::string open, std::string close )
    : m_open( std::move( open ) ), m_close( std::move( close ) ), m_nmax( nmax ) {
  static_assert(
      boost::integer_traits<std::size_t>::is_specialized && boost::integer_traits<std::size_t>::is_integral &&
          boost::integer_traits<std::size_t>::const_min == 0 && boost::integer_traits<std::size_t>::const_max > 0,
      "size_t must be pos def integral type" );
} // =============================================================================
LoKi::Dump::Dump( std::string open, std::string close, const std::size_t nmax )
    : m_open( std::move( open ) ), m_close( std::move( close ) ), m_nmax( nmax ) {
  static_assert(
      boost::integer_traits<std::size_t>::is_specialized && boost::integer_traits<std::size_t>::is_integral &&
          boost::integer_traits<std::size_t>::const_min == 0 && boost::integer_traits<std::size_t>::const_max > 0,
      "size_t must be pos def integral type" );
}
// =============================================================================
LoKi::Dump::Dump( std::string open, const std::size_t nmax, std::string close )
    : m_open( std::move( open ) ), m_close( std::move( close ) ), m_nmax( nmax ) {
  static_assert(
      boost::integer_traits<std::size_t>::is_specialized && boost::integer_traits<std::size_t>::is_integral &&
          boost::integer_traits<std::size_t>::const_min == 0 && boost::integer_traits<std::size_t>::const_max > 0,
      "size_t must be pos def integral type" );
}
// ============================================================================
std::string Gaudi::Utils::toCpp( const LoKi::Dump& dump ) {
  return "LoKi::Dump(" + Gaudi::Utils::toCpp( dump.open() ) + ", " + Gaudi::Utils::toCpp( dump.close() ) + ", " +
         Gaudi::Utils::toCpp( dump.nMax() ) + ") ";
}
// ============================================================================
// The END
// ============================================================================
