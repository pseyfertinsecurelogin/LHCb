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
#ifndef LOKI_GENVERTEXCUTS_H
#define LOKI_GENVERTEXCUTS_H 1
// ============================================================================
// Include files
// ============================================================================
// HepMC
// ============================================================================
#include "HepMC/GenVertex.h"
// ============================================================================
// LoKiGen
// ============================================================================
#include "LoKi/GenTypes.h"
#include "LoKi/GenVertices.h"
//
#include "LoKi/Filters.h"
// ============================================================================
/** @file LoKi/GenVertexCuts.h
 *
 *  Collection of useful functions/predicates to deal with
 *  HepMC generator information
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
 *  @date 2006-02-08
 *
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @var GVTRUE
     *  trivial predicate which always returns "true"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVTRUE = LoKi::BasicFunctors<const HepMC::GenVertex*>::BooleanConstant{ true };
    // ========================================================================
    /** @var GVFALSE
     *  trivial predicate which always returns "false"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVFALSE = LoKi::BasicFunctors<const HepMC::GenVertex*>::BooleanConstant{ false };
    // ========================================================================
    /** @var GVALL
     *  trivial predicate which always returns "true"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVALL = LoKi::BasicFunctors<const HepMC::GenVertex*>::BooleanConstant{ true };
    // ========================================================================
    /** @var GVNONE
     *  trivial predicate which always returns "false"
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVNONE = LoKi::BasicFunctors<const HepMC::GenVertex*>::BooleanConstant{ false };
    // ========================================================================
    /** @var GVONE
     *  trivial function which always returns 1
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVONE = LoKi::BasicFunctors<const HepMC::GenVertex*>::Constant{  1 } ;
    // ========================================================================
    /** @var GVNULL
     *  trivial function which always returns 0
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVNULL = LoKi::BasicFunctors<const HepMC::GenVertex*>::Constant{ 0 } ;
    // ========================================================================
    /** @var GVZERO
     *  trivial function which always returns 0
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVZERO = LoKi::BasicFunctors<const HepMC::GenVertex*>::Constant{ 0 } ;
    // ========================================================================
    /** Minimum from 2 functions
     *
     *  @code
     *
     *  GVFun f1 = ... ;
     *  GVFun f2 = ... ;
     *
     *  GVFun f = GVMIN( f1 , f2 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Min
     */
    typedef LoKi::Min<const HepMC::GenVertex*>             GVMIN ;
    // ========================================================================
    /** Maximum from 2 functions
     *
     *  @code
     *
     *  GVFun f1 = ... ;
     *  GVFun f2 = ... ;
     *
     *  GVFun f = GVMAX( f1 , f2 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MAX
     */
    typedef LoKi::Min<const HepMC::GenVertex*>             GVMAX ;
    // ========================================================================
    /** @var GVBAR
     *  trivial function which returns the "bar-code" for
     *  HepMC::GenVertex
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVBAR = LoKi::GenVertices::BarCode{};
    // ========================================================================
    /** @var GVBARCODE
     *  trivial function which returns the "bar-code" for
     *  HepMC::GenVertex
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVBARCODE = LoKi::GenVertices::BarCode{};
    // ========================================================================
    /** @var GVX
     *  trivial function which returns the X-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::PositionX
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVX = LoKi::GenVertices::PositionX{};
    // ========================================================================
    /** @var GVY
     *  trivial function which returns the X-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::PositionY
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVY = LoKi::GenVertices::PositionY{};
    // ========================================================================
    /** @var GVZ
     *  trivial function which returns the Z-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::PositionZ
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVZ = LoKi::GenVertices::PositionZ{};
    // ========================================================================
    /** @var GVRHO
     *  trivial function which returns the rho(zylindrical)-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::Rho
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2012-02-13
     */
    const auto GVRHO = LoKi::GenVertices::Rho{};
    // ========================================================================
    /** @var GVT
     *  trivial function which returns the T-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::PositionT
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVT = LoKi::GenVertices::PositionT{};
    // ========================================================================
    /** @var GVTIME
     *  trivial function which returns the T-position of
     *  HepMC::GenVertex
     *  @see HepMC::GenVertex
     *  @see LoKi::GenVertices::PositionT
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    const auto GVTIME = LoKi::GenVertices::PositionT{};
    // ========================================================================
    /** the adapter function which counts number of particles
     *  which satisfy certain criteria within the specified range
     *
     *  E.g. find the decay H -> b b~ X :
     *
     *  @code
     *
     *  // number of b-quarks withing children particles
     *  GVFun num1 =  GVCOUNT( "b"  == GID , HepMC::children ) ;
     *
     *  // number of b~-quarks withing children particles
     *  GVFun num2 =  GVCOUNT( "b~" == GID , HepMC::children ) ;
     *
     *  // use adapter function to End vertex
     *  GFun num = GFAEVX ( num1 + num2 , 0 ) ;
     *
     *  // find the decay H -> b b :
     *
     *  const LHCB::HepMCEvents* events = ... ;
     *
     *  GCut decay = ( 2 == num ) && ( "H_10" == GID ) ;
     *
     *  const bool good = LoKi::GenAlgs::found ( events , decay ) ;
     *
     *  @endcode
     *
     *  @see HepMC::GenVertex
     *  @see HepMC::GenParticle
     *  @see LoKi::GenVertices::CountIF
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    typedef LoKi::GenVertices::CountIF                     GVCOUNT ;
    // ========================================================================
    /** the adapter function which accumulated the value of
     *  given function over the particles which satisfy
     *  certain criteria within the specified range
     *
     *  Total energy of all stable daughter charged leptons:
     *
     *  @code
     *
     *  GVFun e1 = GVSUM( GE , HepMC::descendants ,
     *                           GCHARGED && GLEPTON ) ;
     *
     *  GFun eLep = GFAEVX( e1 )
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  const double eTot = eLep( p ) ;
     *
     *  @endcode
     *
     *  @see HepMC::GenVertex
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GFAEVX
     *  @see LoKi::GenVertices::SumIF
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-08
     */
    typedef LoKi::GenVertices::SumIF                     GVSUM ;
    // ========================================================================
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENVERTEXCUTS_H
// ============================================================================
