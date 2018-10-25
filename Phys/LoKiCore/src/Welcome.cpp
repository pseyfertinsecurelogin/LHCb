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
// ===========================================================================
#include <string>
#include <iostream>
// ============================================================================
// local
// ============================================================================
#include "LoKi/Welcome.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/** @file
 *
 *  Implementation file for class LoKi::Welcome
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
// Meyers's singleton
// ============================================================================
const LoKi::Welcome& LoKi::Welcome::instance ()
{
  static const LoKi::Welcome s_welcome{};
  return s_welcome ;
}
// ============================================================================
// standard constructor
// ============================================================================
LoKi::Welcome::Welcome ()
  : m_len1   ( 103      )
  , m_str1   ( "LoKi"   )
  , m_fmt1   ( "%|-5|"  )
  , m_fmt2   ( "%|-94|" )
  , m_fmt3   ( "%|=94|" )
{}
// ============================================================================
LoKi::Welcome::~Welcome() { goodbye () ; }
// ============================================================================
void LoKi::Welcome::welcome() const { welcome ( std::cout ) ; }
// ============================================================================
void LoKi::Welcome::welcome( std::ostream& stream ) const
{

  int expected = 0x0;
  if (!m_printed.compare_exchange_strong(expected,0x1)) return;// only print if welcome not yet printed...

  std::string m_str2;
  while ( m_str2.size() < m_len1 ) { m_str2 += "Welcome " ; }
  m_str2 = std::string( m_str2.begin() , m_str2.begin() + m_len1 ) ;

  stream << '\n' << '\n' ;
  stream << m_str2    << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Welcome to LoKi!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "(LOops & KInematics)" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Smart & Friendly C++ Physics Analysis Tool Kit" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Author:  Vanya BELYAEV Ivan.Belyaev@itep.ru " ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "With the kind help of Galina Pakhlova & Sergey Barsuk " ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt2 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Have fun and enjoy!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt2 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  stream << m_str2    << '\n' ;

  stream << m_str2    << '\n' ;
  stream << '\n' << std::endl ;

}
// ============================================================================
void LoKi::Welcome::goodbye () const { goodbye ( std::cout ) ; }
// ============================================================================
void LoKi::Welcome::goodbye ( std::ostream& stream ) const
{
  int expected = 0x1;
  if (!m_printed.compare_exchange_strong(expected,0x2)) return;// only print if welcome printed, and goodye not yet printed

  std::string m_str2 = "" ;
  while ( m_str2.size() < m_len1 ) { m_str2 += "Good Bye " ; }
  m_str2 = std::string ( m_str2.begin() ,  m_str2.begin() + m_len1 ) ;

  stream << '\n' << '\n' ;

  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Good Bye from LoKi!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "(LOops & KInematics)" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Smart & Friendly C++ Physics Analysis Tool Kit" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt2 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "LoKi mailing list ('LoKi-club') : lhcb-loki@cern.ch ";
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  std::string _s("Comments, suggestions, criticism, ideas, ") ;
  _s += "questions and requests are *ALWAYS* welcome";
  stream << boost::format ( m_fmt3 ) % _s ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;
  stream << '\n' << std::endl ;
}
// ============================================================================

// ============================================================================
// Meyers's singleton
// ============================================================================
const Bender::Welcome& Bender::Welcome::instance()
{
  // make sure that LoKi is instantiated *BEFORE*
  LoKi::Welcome::instance() ;
  static Bender::Welcome s_welcome;
  return s_welcome ;
}
// ============================================================================
// standard constructor
// ============================================================================
Bender::Welcome::Welcome ()
  : m_len1   ( 103      )
  , m_str1   ( "Bender" )
  , m_fmt1   ( "%|-7|"  )
  , m_fmt2   ( "%|-90|" )
  , m_fmt3   ( "%|=90|" )
{
  // own welcome
  welcome  () ;
}
// ============================================================================
// destructor
// ============================================================================
Bender::Welcome::~Welcome() { goodbye () ; }
// ============================================================================
// print welcome message
// ============================================================================
void Bender::Welcome::welcome() const { return welcome ( std::cout ) ; }
// ============================================================================
void Bender::Welcome::welcome( std::ostream& stream ) const
{
  // LoKi welcome
  LoKi::Welcome::instance().welcome ( stream ) ;
  //
  int expected = 0x0;
  if (!m_printed.compare_exchange_strong(expected,0x1)) return;// only print if welcome not yet printed...
  //
  std::string m_str2 = "" ;
  while ( m_str2.size() < m_len1 ) { m_str2 += "Welcome " ; }
  m_str2 = std::string( m_str2.begin() , m_str2.begin() + m_len1 ) ;
  //
  stream << '\n' << '\n' ;
  //
  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Welcome to Bender!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Python-based Physics Analysis Application" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Author:  Vanya BELYAEV Ivan.Belyaev@itep.ru " ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "With the kind help of Pere Mato & Andrey Tsaregorodtsev " ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt2 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Have fun and enjoy! Good Luck!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt2 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;
  //
  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;
  stream << '\n' << std::endl ;
}
// ============================================================================
// print good bye message
// ============================================================================
void Bender::Welcome::goodbye () const { goodbye ( std::cout ) ; }
// ============================================================================
void Bender::Welcome::goodbye ( std::ostream& stream ) const
{
  int expected = 0x1;
  if (!m_printed.compare_exchange_strong(expected,0x2)) return;// only print if welcome printed, and goodye not yet printed
  //
  std::string m_str2;
  while ( m_str2.size() < m_len1 ) { m_str2 += "Good Bye " ; }
  m_str2 = std::string ( m_str2.begin() ,  m_str2.begin() + m_len1 ) ;

  stream << '\n' << '\n' ;

  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "Good Bye from Bender!" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Python-based Physics Analysis Application" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) %
    "Bender mailing list : lhcb-bender@cern.ch ";
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  std::string _s("Comments, suggestions, criticism, ideas, ") ;
  _s += "questions and requests are *ALWAYS* welcome";
  stream << boost::format ( m_fmt3 ) % _s ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << boost::format ( m_fmt1 ) % m_str1 ;
  stream << boost::format ( m_fmt3 ) % "" ;
  stream << boost::format ( m_fmt1 ) % m_str1 << '\n' ;

  stream << m_str2    << '\n' ;
  stream << m_str2    << '\n' ;
  stream << '\n' << std::endl ;
}
// ============================================================================
// The END
// ============================================================================
