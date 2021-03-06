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
#ifndef LOKI_INFO_H
#define LOKI_INFO_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <cassert>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/VectorMap.h"
// ============================================================================
/** @file LoKi/Info.h
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
 */
namespace LoKi {
  // ===========================================================================
  /** @namespace LoKi::ExtraInfo
   *
   *  Collection of functions to deal with "ExtraInfo"
   *  Essentially these functions are very
   *  useful for implementation of the generic HLt-effcient
   *   functors:
   *
   *  @code
   *
   *    class SomeFunction : public LoKi::Function<LHCb::Track>
   *     {
   *     ...
   *     double operator() ( argument t ) const override
   *       {
   *         // check the existence of the information
   *         if ( hasInfo ( t , SomeIndex ) )
   *           {
   *             // return the existing information
   *             return info ( t , SomeIndex , -1 ) ;  // RETURN
   *           }
   *         //  calculate new value :
   *         const resulkt_type value = ...;
   *         // update extra info (if needed)
   *         addInfo ( t , SomeIndex , value ) ;
   *         // return the calculated result:
   *        return value ;                        // RETURN
   *       }
   *     }
   *
   *  @endcode
   *
   *  Btw, in the case of known range  of the result, one can avoid
   *  the double lookup, e.g. "significance" is positive, therefore:
   *  one can combine hasInfo and info into one call:
   *
   *  @code
   *
   *    class SomeSignificance : public LoKi::Function<LHCb::Track>
   *     {
   *     ...
   *     double operator() ( argument t ) const override
   *       {
   *          .. check the existence of the information:
   *         auto value = info ( t , SomIndex , -1 ) ;
   *         if ( 0 < value   ) { return value ; }         // RETURN
   *         // calculate new value :
   *         auto value = ...;
   *         // update extra info
   *         addInfo ( t , SomeIndex , value ) ;
   *         // return the calculated result:
   *         return value ;                        // RETURN
   *       }
   *     }
   *
   *  @endcode
   *
   *  @see LHCb::Track
   *  @see LHCb::Particle
   *  @see GaudiUtils::VectorMap
   *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  namespace ExtraInfo {
    // ========================================================================
    /** check the existence of the key in the "extraInfo" data member
     *
     *  @code
     *
     *   const LHCb::ProtoParticle& pp = ... ;
     *
     *   const bool inEcalAcceptance =
     *      hasInfo ( pp.extraInfo() , LHCb::ProtoParticle::InEcalAcc ) ;
     *
     *  @endcode
     *
     *  @param vmap "vector-map" itself
     *  @param key   the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class K, class V, class A, class C>
    inline bool hasInfo( const GaudiUtils::VectorMap<K, V, A, C>& vmap, const int key ) {
      return vmap.end() != vmap.find( key );
    }
    // ========================================================================
    /** check the existence of the key in the "extraInfo" data member
     *
     *  @code
     *
     *   const LHCb::ProtoParticle& pp = ... ;
     *
     *   const bool inEcalAcceptance =
     *      hasInfo ( pp , LHCb::ProtoParticle::InEcalAcc ) ;
     *
     *  @endcode
     *
     *  @param object "vector-map" itself
     *  @param key   the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline bool hasInfo( const TYPE& object, const int key ) {
      return hasInfo( object.extraInfo(), key );
    }
    // ========================================================================
    /** check the existence of the key in the "extraInfo" data member
     *
     *  @code
     *
     *   const LHCb::ProtoParticle* pp = ... ;
     *
     *   const bool inEcalAcceptance =
     *      hasInfo ( pp , LHCb::ProtoParticle::InEcalAcc ) ;
     *
     *  @endcode
     *
     *  @param object "vector-map" itself
     *  @param key   the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline bool hasInfo( const TYPE* object, const int key ) {
      return 0 == object ? false : hasInfo( *object, key );
    }
    // ========================================================================
    /** get info from "extraInfo" object:
     *
     *  @code
     *
     *   const LHCb::ProtoParticle& pp = ... ;
     *
     *   const bool inEcalAcceptance =
     *      info ( pp.extraInfo() , LHCb::ProtoParticle::InEcalAcc , false ) ;
     *
     *  @endcode
     *
     *  @param vmap  vector map itself
     *  @param key   the key
     *  @param def   the default value to be returned in the absence of the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class K, class V, class A, class C>
    inline double info( const GaudiUtils::VectorMap<K, V, A, C>& vmap, const int key, const double def ) {
      typename GaudiUtils::VectorMap<K, V, A, C>::const_iterator ifind = vmap.find( key );
      return vmap.end() != ifind ? ifind->second : def;
    }
    // ========================================================================
    /** get info from the object
     *
     *  @code
     *
     *  const LHCb::ProtoParticle& pp = ... ;
     *
     *  const double DLLK = info ( pp , LHCb::ProtoParticle::CombDLLK , -1000. ) ;
     *
     *  @endcode
     *
     *  @see LHCb::Track
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::Vertex
     *  @param object reference to the object
     *  @param key    the key
     *  @param def    the default value to be returned in the absence of the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline double info( const TYPE& object, const int key, const double def ) {
      return info( object.extraInfo(), key, def );
    }
    // ========================================================================
    /** get info from the object
     *
     *  @code
     *
     *  const LHCb::ProtoParticle* pp = ... ;
     *
     *  const double DLLK = info ( pp , LHCb::ProtoParticle::CombDLLK , -1000. ) ;
     *
     *  @endcode
     *
     *  @see LHCb::Track
     *  @see LHCb::Particle
     *  @see LHCb::ProtoParticle
     *  @see LHCb::Vertex
     *  @param object reference to the object
     *  @param key    the key
     *  @param def    the default value to be returned in the absence of the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline double info( const TYPE* object, const int key, const double def ) {
      return 0 == object ? def : info( *object, key, def );
    }
    // ========================================================================
    /** set/update the info of "extraInfo" object:
     *
     *  @code
     *
     *   LHCb::ProtoParticle& pp = ... ;
     *
     *   addInfo ( pp.extraInfo() , LHCb::ProtoParticle::InEcalAcc , false ) ;
     *
     *  @endcode
     *
     *  @param  vmap vector map itself
     *  @param  key  the key
     *  @param  val  the value associated with the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class K, class V, class A, class C>
    inline void addInfo( GaudiUtils::VectorMap<K, V, A, C>& vmap, const int key, const double val ) {
      vmap.update( key, val );
    }
    // ========================================================================
    /** set/update the info of "extraInfo" object:
     *  @attention const_cast is used!
     *  @param  vmap vector map itself
     *  @param  key  the key
     *  @param  val  the value associated with the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class K, class V, class A, class C>
    inline void addInfo( const GaudiUtils::VectorMap<K, V, A, C>& vmap, const int key, const double val ) {
      GaudiUtils::VectorMap<K, V, A, C>& aux = const_cast<GaudiUtils::VectorMap<K, V, A, C>&>( vmap );
      addInfo( aux, key, val );
    }
    // ========================================================================
    /** set/update the info of "extraInfo" object:
     *
     *  @code
     *
     *    LCHb::ProtoParticle& pp = ... ;
     *
     *    addInfo ( pp , LHCb::ProtoParticle::InEcalAcceptance , true ) ;
     *
     *  @endcode
     *
     *  @param  object object to be modified
     *  @param  key    the key
     *  @param  value  the value associated with the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline void addInfo( const TYPE& object, const int key, const double value ) {
      addInfo( object.extraInfo(), key, value );
    }
    // ========================================================================
    /** set/update the info of "extraInfo" object:
     *
     *  @code
     *
     *    LCHb::ProtoParticle& pp = ... ;
     *
     *    addInfo ( pp , LHCb::ProtoParticle::InEcalAcceptance , true ) ;
     *
     *  @endcode
     *
     *  @attention There no action for invalid pointer!
     *  @param  object object to be modified
     *  @param  key  the key
     *  @param  val  the value associated with the key
     *  @author Vanya BELAYEV ibelyaev@physics.syr.edu
     *  @date 2007-06-14
     */
    template <class TYPE>
    inline void addInfo( const TYPE* object, const int key, const double value ) {
      assert( 0 != object && "LoKi::Info::addInfo: invalid pointer " );
      if ( 0 != object ) { addInfo( *object, key, value ); }
    }
    // ========================================================================
  } // namespace ExtraInfo
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_INFO_H
