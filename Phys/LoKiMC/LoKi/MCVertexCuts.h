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
#ifndef LOKI_MCVERTEXCUTS_H
#define LOKI_MCVERTEXCUTS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCTypes.h"
#include "LoKi/MCVertices.h"
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
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** Minimum from 2 functions
     *
     *  @code
     *
     *  MCVFun f1 = ... ;
     *  MCVFun f2 = ... ;
     *
     *  MCVFun f = MCVMIN( f1 , f2 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Min
     */
    typedef LoKi::Min<const LHCb::MCVertex*> MCVMIN;
    // ========================================================================
    /** Maxumum from 2 functions
     *
     *  @code
     *
     *  MCVFun f1 = ... ;
     *  MCVFun f2 = ... ;
     *
     *  MCVFun f = MCVMAX( f1 , f2 ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Max
     */
    typedef LoKi::Max<const LHCb::MCVertex*> MCVMAX;
    // ========================================================================
    /** Simple "switch"
     *  The function evaluated the predicate and returns
     *  one of the predefined values, depending on result on
     *  predicate evaluation.
     *  In some sense it is a conversion of "predicate" to "function"
     *
     *
     *  @see LoKi::SipleSwitch
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     */
    typedef LoKi::SimpleSwitch<const LHCb::MCVertex*> MCVSSWITCH;
    // ========================================================================
    /** switch"
     *  The function evaluates the predicate and returns
     *  the values of one of the predefined functions,
     *  depending on result on predicate evaluation.
     *
     *
     *  @see LoKi::Switch
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     */
    typedef LoKi::Switch<const LHCb::MCVertex*> MCVSWITCH;
    // ========================================================================
    /** @var MCVTYPE
     *  the type of MC vertex ( MCVertex::type )
     *
     *  @code
     *
     *   const LHCb::MCVertex* v  = ...         ;
     *   const double    t  = MCVTYPE (  v ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::TypeOfMCVertex
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    inline const auto MCVTYPE = LoKi::MCVertices::TypeOfMCVertex{};
    // ========================================================================
    /** @var MCVVALID
     *
     *  The trivial checker of "the validity" of the pointer to
     *   LHCb::MCVertex objects
     *
     *  @code
     *
     *   const MCVertex* v  = ...         ;
     *   const bool valid =   = MCVVALID ( v ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Valid
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-01-21
     */
    inline const auto MCVVALID = LoKi::Valid<const LHCb::MCVertex*>{};
    // ========================================================================
    /** @var MCVTOF
     *  the time of flight for  MC vertex ( MCVertex::timeOfFlight )
     *
     *  @code
     *
     *   const LHCb::MCVertex* v  = ...         ;
     *   const double    t  = MCVTOF (  v ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::TimeOfFLight
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    inline const auto MCVTOF = LoKi::MCVertices::TimeOfFlight{};
    // ========================================================================
    /** @var MCVX
     *  the x-position of       MC vertex
     *
     *  @code
     *
     *   const LHCb::MCVertex* v  = ...         ;
     *   const double    x  = MCVX   (  v ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::VertexPositionX
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    inline const auto MCVX = LoKi::MCVertices::VertexPositionX{};
    // ========================================================================
    /** @var MCVY
     *  the y-position of       MC vertex
     *
     *  @code
     *
     *   const LHCb::MCVertex* v  = ...         ;
     *   const double    y  = MCVY   (  v ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::VertexPositionY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    inline const auto MCVY = LoKi::MCVertices::VertexPositionY{};
    // ========================================================================
    /** @var MCVZ
     *  the z-position of       MC vertex
     *
     *  @code
     *
     *   const LHCb::MCVertex* v  = ...         ;
     *   const double    z  = MCVZ   (  v ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::VertexPositionZ
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    inline const auto MCVZ = LoKi::MCVertices::VertexPositionZ{};
    // ========================================================================
    /** @var MCVTIME
     *  the "time" of MC vertex
     *
     *  @code
     *
     *  const LHCb::MCVertex* vertex = ... ;
     *  const double time = MCVTIME ( vertex ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCVertices::VertexTime
     *  @see LHCb::MCVertex
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    inline const auto MCVTIME = LoKi::MCVertices::VertexTime{};
    // ========================================================================
    /** @var MCPRIMARY
     *  evaluates to "true" for  primary vertices
     *
     *  @code
     *
     *  const LHCb::MCVertex* vertex = ... ;
     *  const bool primary = MCPRIMARY ( vertex ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCVertices::Primary
     *  @see LHCb::MCVertex
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    inline const auto MCPRIMARY = LoKi::MCVertices::Primary{};
    // ========================================================================
    /** @var MCISPRIMARY
     *  evaluates to "true" for  primary vertices
     *
     *  @code
     *
     *  const LHCb::MCVertex* vertex = ... ;
     *  const bool primary = MCISPRIMARY ( vertex ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCVertices::Primary
     *  @see LHCb::MCVertex
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    inline const auto MCISPRIMARY = LoKi::MCVertices::Primary{};
    // ========================================================================
    /** @var MCISDECAY
     *  evaluates to "true" for 'decay' vertices
     *
     *  @code
     *
     *  const LHCb::MCVertex* vertex = ... ;
     *  const bool decay = MCISDECAY ( vertex ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCVertices::Decay
     *  @see LHCb::MCVertex
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-02-06
     */
    inline const auto MCISDECAY = LoKi::MCVertices::Decay{};
    // ========================================================================
    /** the distance for MC vertex from given point,
     *  given (MC)Vertex or collision
     *
     *  @code
     *
     *   const LoKi::Point3D& point = ... ;
     *   const LHCb::MCVertex*      v0    = ... ;
     *   const Collision*     coll  = ... ;
     *
     *
     *   const LHCb::MCVertex* vertex  = ...         ;
     *
     *   // evaluate the distance from point
     *   MCVFun       dist1 = MCVDIST ( point  ) ;
     *   const double d1    = dist1   ( vertex ) ;
     *
     *   // evaluate the distance from vertex
     *   MCVFun       dist2 = MCVDIST ( v0     ) ;
     *   const double d2    = dist2   ( vertex ) ;
     *
     *   // evaluate the distance from collision
     *   MCVFun       dist3 = MCVDIST ( coll   ) ;
     *   const double d3    = dist3   ( vertex ) ;
     *
     *  @endcode
     *
     *  @see MCVertex
     *  @see LoKi::MCVertices::MCVertexDistance
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2004-07-07
     */
    using MCVDIST = LoKi::MCVertices::MCVertexDistance;
    // ========================================================================
    /** the adaptor for MC vertex function to be interpreted as
     *  MC particle function
     *  it acts using the rule
     *
     *  @code
     *
     *  result = fun( MCParticle* p ) = vfun( p->originVertex() )
     *
     *  @endcode
     *
     *  Usage:
     *
     *  @code
     *
     *  const MCParticle* p = ... ;
     *  // z-coordinate of particle origin:
     *  MCFun fun = MCVXFUN ( MCVZ ) ;
     *  const double z = fun ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Adapters::VFuncAsFun
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-03-01
     */
    typedef LoKi::MCVertices::MCVFunAsMCFun MCVXFUN;
    // ========================================================================
    /** the adaptor for MC vertex function to be interpreted as
     *  MC particle function
     *  it acts using the rule
     *
     *  @code
     *
     *  result = fun( MCParticle* p ) = vfun( p->primaryVertex() )
     *
     *  @endcode
     *
     *  Usage:
     *
     *  @code
     *
     *  const MCParticle* p = ... ;
     *  // z-coordinate of particle origin:
     *  MCFun fun = MCVXFUN ( MCVZ ) ;
     *  const double z = fun ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Adapters::VFuncAsFun
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-03-01
     */
    typedef LoKi::MCVertices::MCVPFunAsMCFun MCVPXFUN;
    // ========================================================================
    /** the adaptor for MC vertex function to be interpreted as
     *  MC particle function
     *  it acts using the rule
     *
     *  @code
     *
     *  result = fun( MCParticle* p ) = vfun( p->primaryVertex() )
     *
     *  @endcode
     *
     *  Usage:
     *
     *  @code
     *
     *  const MCParticle* p = ... ;
     *  // z-coordinate of the first end-vertex:
     *  MCFun fun = MCVFUN ( 0 , MCVZ ) ;
     *  const double z = fun ( p ) ;
     *
     *  @endcode
     *
     *  @see LoKi::MCVertices::MCVFunction
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-10-21
     */
    typedef LoKi::MCVertices::MCVFunction MCVFUN;
    // ========================================================================
    inline const auto MCVTRUE = LoKi::Constant<const LHCb::MCVertex*, bool>{true};
    // ========================================================================
    inline const auto MCVFALSE = LoKi::Constant<const LHCb::MCVertex*, bool>{false};
    // ========================================================================
    inline const auto MCVALL = MCVTRUE;
    // ========================================================================
    inline const auto MCVNONE = MCVFALSE;
    // ========================================================================
    inline const auto MCVONE = LoKi::Constant<const LHCb::MCVertex*, double>{1.0};
    // ========================================================================
    inline const auto MCVZERO = LoKi::Constant<const LHCb::MCVertex*, double>{0.0};
    // ========================================================================
    /** @var MCVKEY
     *  get the key for the MC-vertex
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-02-14
     */
    inline const auto MCVKEY = LoKi::MCVertices::Key{};
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MCVERTEXCUTS_H
