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
#ifndef LOKI_VALUE_H
#define LOKI_VALUE_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Functor.h"
// ============================================================================
/** @file  LoKi/Value.h
 *  helper class to implement the adaptor
 *  for void->TYPE2 to TYPE->TYPE2 functor
 *
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 *  @date   2014-02-01
 *
 *  This file is a part of LoKi project -
 *  ``C++ ToolKit  for Smart and Friendly Physics Analysis''
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design
 *
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @class Value LoKi/Value.h
   *  Simple adaptor from void->TYPE2 to TYPE->TYPE2 functor
   *
   *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
   *  @date   2014-02-01
   */
  template <class TYPE, class TYPE2>
  class Value : public LoKi::Functor<TYPE, TYPE2> {
  public:
    // ========================================================================
    /** Standard constructor from "void"-functor
     *  @param fun  void-functor
     */
    explicit Value( LoKi::FunctorFromFunctor<void, TYPE2> fun )
        : LoKi::AuxFunBase( std::tie( fun ) ), m_void( std::move( fun ) ) {}
    /// clone method (mandatory)
    Value* clone() const override { return new Value( *this ); }
    /// the only one essential method ("function")
    TYPE2 operator()( typename LoKi::Functor<TYPE, TYPE2>::argument ) const override {
      return m_void.fun();
    } // bypass one virtual call
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override { return m_void.fillStream( s ); }
    // ========================================================================
  private:
    // ========================================================================
    /// the actual storage of void-functor
    LoKi::FunctorFromFunctor<void, TYPE2> m_void;
    // ========================================================================
  };
  // ==========================================================================
  /// specialization for void, essentially bypass all actions
  template <class TYPE2>
  class Value<void, TYPE2> : public LoKi::Functor<void, TYPE2> {
  public:
    // ========================================================================
    /** Standard constructor from "void"-functor
     *  @param fun  void-functor
     */
    Value( LoKi::FunctorFromFunctor<void, TYPE2> fun )
        : LoKi::AuxFunBase( std::tie( fun ) ), m_void( std::move( fun ) ) {}
    /// clone method (mandatory)
    Value* clone() const override { return new Value( *this ); }
    /// the only one essential method ("function")
    TYPE2 operator()( /* typename LoKi::Functor<void,TYPE2>::argument a */ ) const override {
      return m_void.fun();
    } // bypass one virtual call
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override { return m_void.fillStream( s ); }
    // ========================================================================
  private:
    // ========================================================================
    /// the actual storage of void-functor
    LoKi::FunctorFromFunctor<void, TYPE2> m_void;
    // ========================================================================
  };
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_VALUE_H
