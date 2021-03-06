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
#ifndef LOKI_FINDERDICT_H
#define LOKI_FINDERDICT_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SmartRefVector.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/DecayFinder.h"
#include "LoKi/GetType.h"
// ============================================================================
/** @file LoKi/FinderDict.h
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
  namespace Dicts {
    // ========================================================================
    /** @class FinderDict LoKi/FinderDict.h
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date   2009-06-28
     */
    template <class TYPE>
    struct FinderDicts_ {
      // ======================================================================
      /// get the actual "stripped" type
      typedef typename LoKi::GetType<TYPE>::Type Type;
      // ======================================================================
    public:
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::Container& input ) {
        return finder.hasDecay( input.begin(), input.end() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::ConstVector& input ) {
        return finder.hasDecay( input.begin(), input.end() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::Vector& input ) {
        return finder.hasDecay( input.begin(), input.end() );
      }
      // ======================================================================
      static bool __hasDecay__( const Decays::Finder_<TYPE>& finder, const SmartRefVector<Type>& input ) {
        return finder.hasDecay( input.begin(), input.end() );
      }
      // ======================================================================
    public:
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::Container& input,
                                   typename Type::ConstVector& output ) {
        return finder.findDecay( input.begin(), input.end(), output );
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::ConstVector& input,
                                   typename Type::ConstVector& output ) {
        return finder.findDecay( input.begin(), input.end(), output );
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const typename Type::Vector& input,
                                   typename Type::ConstVector& output ) {
        return finder.findDecay( input.begin(), input.end(), output );
      }
      // ======================================================================
      static size_t __findDecay__( const Decays::Finder_<TYPE>& finder, const SmartRefVector<Type>& input,
                                   typename Type::ConstVector& output ) {
        return finder.findDecay( input.begin(), input.end(), output );
      }
      // ======================================================================
    public:
      // ======================================================================
      static typename Type::ConstVector __findDecay__( const Decays::Finder_<TYPE>&    finder,
                                                       const typename Type::Container& input ) {
        typename Type::ConstVector output;
        finder.findDecay( input.begin(), input.end(), output );
        return output;
      }
      // ======================================================================
      static typename Type::ConstVector __findDecay__( const Decays::Finder_<TYPE>& finder,
                                                       const typename Type::Vector& input ) {
        typename Type::ConstVector output;
        finder.findDecay( input.begin(), input.end(), output );
        return output;
      }
      // ======================================================================
      static typename Type::ConstVector __findDecay__( const Decays::Finder_<TYPE>&      finder,
                                                       const typename Type::ConstVector& input ) {
        typename Type::ConstVector output;
        finder.findDecay( input.begin(), input.end(), output );
        return output;
      }
      // ======================================================================
    };
    // ========================================================================
  } // namespace Dicts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FINDERDICT_H
