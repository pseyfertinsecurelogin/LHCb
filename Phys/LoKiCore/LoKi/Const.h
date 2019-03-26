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
#ifndef LOKI_CONST_H
#define LOKI_CONST_H 1

#include <type_traits>

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
 *  @date 2006-03-29
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @struct Const LoKi/Const.h
   *  Auxillary structure to define "const-traits"
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-03-29
   */
  template <class TYPE>
  struct Const {
    typedef std::add_const_t<TYPE> Value;
  };
  // ==========================================================================
  template <class TYPE>
  struct Const<TYPE*> {
    typedef std::add_const_t<TYPE>* const Value;
  };
  // ==========================================================================
  template <class TYPE>
  struct Const<TYPE&> {
    typedef std::add_const_t<TYPE> const& Value;
  };
  // ==========================================================================
  /** @struct NonConst LoKi/Const.h
   *  Auxillary structure to define "const-traits"
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date   2010-07-30
   */
  template <class TYPE>
  struct NonConst {
    typedef std::remove_const_t<TYPE> Value;
  };
  // ==========================================================================
  template <class TYPE>
  struct NonConst<TYPE*> {
    typedef std::remove_const_t<TYPE>* Value;
  };
  // ==========================================================================
  template <class TYPE>
  struct NonConst<TYPE&> {
    typedef std::remove_const_t<TYPE>& Value;
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_CONST_H
