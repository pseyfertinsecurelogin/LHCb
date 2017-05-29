// ============================================================================
// Include files 
// ============================================================================
// STD & SLT
// ============================================================================
#include <boost/regex/v4/basic_regex.hpp>
#include <boost/regex/v4/match_flags.hpp>
#include <boost/regex/v4/match_results.hpp>
#include <boost/regex/v4/perl_matcher_common.hpp>
#include <boost/regex/v4/perl_matcher_non_recursive.hpp>
#include <boost/regex/v4/regex.hpp>
#include <boost/regex/v4/regex_fwd.hpp>
#include <boost/regex/v4/regex_match.hpp>
#include <boost/regex/v4/sub_match.hpp>
#include <algorithm>
#include <ostream>
#include <string>
#include <vector>

#include "LoKi/Assert.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/Param.h"

// ============================================================================
/*/** @file 
 *  Implementation file for class LoKi::Param
 *  @see LoKi::Param
 *
 *  @date 2014-02-02 
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @thanks Alexander MAZUROV for regex parsing code 
 *  @todo   replace Boost-regex with STD/STL-regex 
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 */
// ============================================================================
namespace 
{
  // ==========================================================================
  /** the pattern to imporved for more felxibility 
   *  @toto imporove it for more felxibility 
   *  @thanks Alexander MAZUROV 
   */
  const boost::regex s_pattern ( "(\\w+)\\s*(?:\\[\\s*(\\w+)\\s*\\])?\\s*(?:/\\s*(\\w+))?" );
  //
  bool parse 
  ( const std::string& input    , 
    std::string&       property , 
    std::string&       key      , 
    std::string&       alg      ) 
  {
    std::string::size_type r = input.rfind ( '/' ) ;
    if ( std::string::npos != r ) 
    {
      std::string input2 ( input.begin()         , input.begin () + r ) ;
      alg = std::string  ( input.begin() + r + 1 , input.end   ()     ) ;
      std::string alg2 ;
      return parse ( input2 , property , key , alg2 ) ;
    }
    //
    boost::smatch what;
    if( !boost::regex_match ( input , what, s_pattern ) ) { return false ; }
    //
    
    property = std::string ( what[1].first, what[1].second ) ;
    key      = std::string ( what[2].first, what[2].second ) ;
    //
    // alg = std::string(what[3].first, what[3].second);
    //
    return true ;
  } 
  // ==========================================================================
}
// ============================================================================
// Constructor
// ============================================================================
LoKi::Param::Param ( const std::string& property ) 
{
  LoKi::Assert ( ::parse ( property , m_property , m_key , m_algorithm ) ,
                 "Invalid pattern for parameter name '" + property + "'" ) ; 
}
// ============================================================================
// printout 
// ============================================================================
std::ostream& LoKi::Param::fillStream ( std::ostream& s ) const 
{
  s << "PARAM('";
  //
  if ( !property  () . empty () ) { s        << property  ()        ; }
  if ( !key       () . empty () ) { s << "[" << key       () << "]" ; }
  //
  return s << "')" ;
}
// ============================================================================
std::string Gaudi::Utils::toCpp ( const LoKi::Param& o ) 
{
  return 
    "LoKi::Param(\"" 
    + o.property  () + "[" 
    + o.key       () + "]/" 
    + o.algorithm () + "\")" ;
}
// ============================================================================
// The END 
// ============================================================================
