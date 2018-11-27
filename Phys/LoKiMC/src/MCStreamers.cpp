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
#include <set>
// ============================================================================
// local
// ============================================================================
#include "LoKi/MCTypes.h"
#include "LoKi/MCExtract.h"
// ============================================================================
/** @file
 *
 *  implementation of some MC-streamers
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-05-24
 *
 */
// ============================================================================
namespace
{
  // ==========================================================================
  /// get particles
  template <class PARTICLE>
  inline
  LHCb::MCParticle::ConstVector
  _get_ ( PARTICLE                   first ,
          PARTICLE                   last  ,
          const LoKi::Types::MCCuts& cuts  )
  {
    //
    LHCb::MCParticle::ConstVector r ; r.reserve ( 100 ) ;
    //
    LoKi::Extract::mcParticles ( first ,
                                 last  ,
                                 std::back_inserter ( r ) , cuts ) ;
    //
    // eliminate duplicates
    std::set<const LHCb::MCParticle*>  s ( r.begin () , r.end () ) ;
    //
    return LHCb::MCParticle::ConstVector ( s.begin () , s.end () ) ;
  }
  // ==========================================================================
} //                                                  end of anonymos namespace
// ============================================================================
LHCb::MCParticle::ConstVector
operator>>
( const LHCb::MCParticle::ConstVector&    input ,
  const LoKi::Types::MCCuts&              cuts  )
{ return _get_ ( input.begin () , input.end() , cuts ) ; }
// ============================================================================
LHCb::MCParticle::ConstVector
operator>>
( const LHCb::MCParticle::Container*      input ,
  const LoKi::Types::MCCuts&              cuts  )
{
  if ( 0 == input ) { return LHCb::MCParticle::ConstVector() ; }
  //
  return _get_ ( input->begin () , input->end() , cuts ) ;
}
// ============================================================================
LHCb::MCParticle::ConstVector
operator>>
( const LoKi::MCTypes::MCRange&           input ,
  const LoKi::Types::MCCuts&              cuts  )
{ return _get_ ( input.begin () , input.end() , cuts ) ; }
// ============================================================================
LHCb::MCParticle::ConstVector
operator>>
( const SmartRefVector<LHCb::MCParticle>& input ,
  const LoKi::Types::MCCuts&              cuts  )
{ return _get_ ( input.begin () , input.end() , cuts ) ; }
// ============================================================================


// ============================================================================
// The END
// ============================================================================
