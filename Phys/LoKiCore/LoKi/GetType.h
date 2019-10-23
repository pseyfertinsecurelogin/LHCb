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
#ifndef LOKI_GETTYPE_H
#define LOKI_GETTYPE_H 1
// ============================================================================
// Include files
// ============================================================================
/** @file LoKi/GetType.h
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
  // ==========================================================================
  /** @struct GetType GetType.h LoKi/GetType.h
   *
   *  Helper utility to get "raw" type
   *
   *  @author Ivan BELYAEV
   *  @date   2006-10-07
   */
  template <class TYPE>
  struct GetType {
    using Type = TYPE;
  };
  // ==========================================================================
  template <class TYPE>
  struct GetType<const TYPE> {
    using Type = typename GetType<TYPE>::Type;
  };
  // ==========================================================================
  template <class TYPE>
  struct GetType<TYPE&> {
    using Type = typename GetType<TYPE>::Type;
  };
  // ==========================================================================
  template <class TYPE>
  struct GetType<TYPE*> {
    using Type = typename GetType<TYPE>::Type;
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_GETTYPE_H
