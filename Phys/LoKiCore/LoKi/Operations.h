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
#ifndef LOKI_OPERATIONS_H
#define LOKI_OPERATIONS_H 1
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <vector>
// ============================================================================
// Include files
// ============================================================================
/** @file LoKi/Operations.h
 *  Collection of basic operations with containters
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-06-03
 */
namespace LoKi {
  // ==========================================================================
  namespace Operations {
    // ========================================================================
    /** @struct Unique
     *  Helper structure to define "unique" for container
     *  @see LoKi::BasicFunctors::Pipe
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct Unique {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const {
        if ( a.empty() ) { return {}; } // RETURN
        auto _a = a;
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        return {_a.begin(), _ia};
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct Union
     *  Helper structure to represent the union of two containters
     *  @see std::set_union
     *  @see LoKi::Operations::Union
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct Union {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return a; } // RETURN
        //
        LoKi::Operations::Unique _unique;
        if ( a.empty() ) {
          return _unique( b );
        } else if ( b.empty() ) {
          return _unique( a );
        }
        //
        auto _a( a );
        auto _b( b );
        //
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        //
        std::sort( _b.begin(), _b.end() );
        auto _ib = std::unique( _b.begin(), _b.end() );
        //
        std::vector<TYPE> _r;
        _r.reserve( ( _ia - _a.begin() ) + ( _ib - _b.begin() ) );
        //
        std::set_union( _a.begin(), _ia, _b.begin(), _ib, std::back_inserter( _r ) );
        //
        return _r;
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct Difference
     *  Helper structure to define "difference" for two containers
     *  @see LoKi::BasicFunctors::Map
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct Difference {
      // ======================================================================
      /// the main method
      template <class TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return std::vector<TYPE>(); } // RETURN
        //
        LoKi::Operations::Unique _unique;
        if ( a.empty() ) {
          return a;
        } else if ( b.empty() ) {
          return _unique( a );
        }
        //
        auto _a( a );
        auto _b( b );
        //
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        //
        std::sort( _b.begin(), _b.end() );
        auto _ib = std::unique( _b.begin(), _b.end() );
        //
        std::vector<TYPE> _r;
        _r.reserve( _ia - _a.begin() );
        //
        std::set_difference( _a.begin(), _ia, _b.begin(), _ib, std::back_inserter( _r ) );
        //
        return _r;
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct SymDifference
     *  Helper structure to define "symmetric-difference" for two containers
     *  @see LoKi::BasicFunctors::Map
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct SymDifference {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return std::vector<TYPE>(); } // RETURN
        //
        std::vector<TYPE> _r;
        std::vector<TYPE> _a( a );
        std::vector<TYPE> _b( b );
        //
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        //
        std::sort( _b.begin(), _b.end() );
        auto _ib = std::unique( _b.begin(), _b.end() );
        //
        _r.reserve( ( _ia - _a.begin() ) + ( _ib - _b.begin() ) );
        //
        std::set_symmetric_difference( _a.begin(), _ia, _b.begin(), _ib, std::back_inserter( _r ) );
        //
        return _r;
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct Intersection
     *  Helper structure to define "intersection" for two containers
     *  @see LoKi::BasicFunctors::Map
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct Intersection {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return a; } // RETURN
        //
        if ( a.empty() || b.empty() ) { return std::vector<TYPE>(); } // RETURN
        //
        auto _a( a );
        auto _b( b );
        //
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        //
        std::sort( _b.begin(), _b.end() );
        auto _ib = std::unique( _b.begin(), _b.end() );
        //
        std::vector<TYPE> _r;
        _r.reserve( std::min( _ia - _a.begin(), _ib - _b.begin() ) );
        //
        std::set_intersection( _a.begin(), _ia, _b.begin(), _ib, std::back_inserter( _r ) );
        //
        return _r;
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct Includes
     *  Helper structure to define "includes" for two containers
     *  @see LoKi::BasicFunctors::Map
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct Includes {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      bool operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return true; } // RETURN
        //
        auto _a( a );
        auto _b( b );
        //
        std::sort( _a.begin(), _a.end() );
        auto _ia = std::unique( _a.begin(), _a.end() );
        //
        std::sort( _b.begin(), _b.end() );
        auto _ib = std::unique( _b.begin(), _b.end() );
        //
        return std::includes( _a.begin(), _ia, _b.begin(), _ib );
        //
      }
      // ======================================================================
    };
    // ========================================================================
    /** @struct NonEmptyUnion
     *  Helper structure to represent the non-empty union of two containters
     *  It is empty if some of the container is empty
     *  @see LoKi::Operations::Union
     *  @see LoKi::BasicFunctors::Pipe
     *  @see LoKi::BasicFunctors::Source
     *  @author Vanya Belyaev Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    struct NoEmptyUnion {
      // ======================================================================
      /// the main method
      template <typename TYPE>
      std::vector<TYPE> operator()( const std::vector<TYPE>& a, const std::vector<TYPE>& b ) const {
        //
        if ( &a == &b ) { return a; } // RETURN
        //
        if ( a.empty() ) {
          return a;
        } // RETURN EMPTY
        else if ( b.empty() ) {
          return b;
        } // RETURN EMPTY
        //
        return m_union( a, b );
      }
      // ======================================================================
    private:
      // ======================================================================
      Union m_union;
      // = ====================================================================
    };
    // ========================================================================
  } // namespace Operations
  // ==========================================================================
} // namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_OPERATIONS_H
