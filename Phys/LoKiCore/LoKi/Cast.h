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
#ifndef LOKI_CAST_H
#define LOKI_CAST_H 1
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
 *  @date 2006-02-20
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::Cast Cast.h LoKi/Cast.h
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-17
   */
  namespace Cast {
    // ========================================================================
    /** @struct ConstAway
     *  The trivial function which performs
     *  const_cast
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-17
     */
    template <class TYPE>
    struct ConstAway {
      inline TYPE* operator()( const TYPE* o ) const { return const_cast<TYPE*>( o ); }
    };
    // ========================================================================
    template <class TYPE>
    struct ConstAway<const TYPE> {
      inline TYPE* operator()( const TYPE* o ) const { return const_cast<TYPE*>( o ); }
    };
    // ========================================================================
    template <class TYPE>
    struct ConstAway<TYPE*> {
      inline TYPE* operator()( TYPE* o ) const { return o; }
    };
    // ========================================================================
    template <class TYPE>
    struct ConstAway<const TYPE*> {
      inline TYPE* operator()( const TYPE* o ) const { return const_cast<TYPE*>( o ); }
    };
    // ========================================================================
    template <class TYPE>
    struct ConstAway<const TYPE&> {
      inline TYPE& operator()( const TYPE& o ) const { return const_cast<TYPE&>( o ); }
    };
    // ========================================================================
    template <class TYPE>
    struct ConstAway<TYPE&> {
      inline TYPE& operator()( TYPE& o ) const { return o; }
    };
    // ========================================================================
  } // namespace Cast
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_CAST_H
