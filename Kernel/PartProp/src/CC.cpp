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
// PartProp
// ============================================================================
#include "Kernel/CC.h"
// ============================================================================
/** @file 
 *  Implementation file for namespace Decays::CC
 *  @date 2008-12-13 
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 */
// ============================================================================
namespace 
{
  // ========================================================================
  /** simple function to make charge conjugated inside the original string. 
   *  All substrings are subsutututed by their charge conjugates 
   *  @param decay the original sring 
   *  @param map_  the full map of substitutions 
   *  @param pos   the starting position 
   *  @return charge-conjugated string 
   */
  inline std::string cc_
  ( const std::string&           decay   , 
    const Decays::CC::MapCC&     map_    ,
    const std::string::size_type pos = 0 ) 
  {
    // check the validity of position 
    if  ( pos >= decay.size() ) { return decay ; }                  // RETURN
    // find the match:
    auto _p = std::string::npos ;
    Decays::CC::MapCC::const_iterator  _i = map_.end () ;
    // look for the "nearest" and "longest" match 
    for ( auto ic = map_.begin() ; map_.end() != ic ; ++ic ) 
    {
      // find the particle 
      std::string::size_type p = decay.find ( ic->first , pos ) ;
      // match? 
      if ( std::string::npos == p ) { continue ; }                 // CONTINUE 
      // find the nearest match
      if ( p < _p )
      {
        _p = p  ;
        _i = ic ;
      }
    }
    // no match at all.
    if ( std::string::npos == _p || 
         map_.end ()       == _i ) { return decay ; }          // RETURN 
    
    // replace 
    std::string aux = decay ;
    aux.replace ( _p , _i -> first.size() , _i -> second ) ;      
    
    // advance the position 
    _p +=  _i -> second.size() ;
    // ... and start the recursion here 
    return _p < aux.size() ? cc_ ( aux , map_ , _p ) : aux ;       // RETURN
  }
  // ==========================================================================
} // end of anonymous namespace 
// ============================================================================
/* simple function to make charge conjugated inside the original string. 
 *  All substrings are subsutututed by their charge conjugates 
 *  @param decay the original sring 
 *  @param map_  the full map of substitutions 
 *  @return charge-conjugated string 
 */
// ============================================================================
std::string Decays::CC::cc
( const std::string&           decay   , 
  const Decays::CC::MapCC&     map_    ) { return cc_ ( decay , map_ ) ; }
// ============================================================================
std::string Decays::CC::cc
( const std::string&                       decay   , 
  const std::map<std::string,std::string>& map_    ) 
{
  return cc_ ( decay ,{ map_.begin(), map_.end() } ) ;
}
// ============================================================================
// The END 
// ============================================================================
