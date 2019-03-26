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
#ifndef LOKI_BIFUNCTIONS_H
#define LOKI_BIFUNCTIONS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/Holder.h"
// ============================================================================
/** @file LoKi/BiFunctions.h
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
 *  @date 2012-01-17
 */
namespace LoKi {
  // ==========================================================================
  /** mimic 2-argument function
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-08
   */
  template <class TYPE1, class TYPE2, class TYPE3>
  class Functor<LoKi::Holder<TYPE1, TYPE2>, TYPE3> : virtual public LoKi::AuxFunBase {
  public:
    // ========================================================================
    /// parameters: argument
    typedef LoKi::Holder<TYPE1, TYPE2> Type1; // parameters: argument
    /// parameters: return value
    typedef TYPE3 Type2; // parameters: return value
    // ========================================================================
    /// the type of the argument
    typedef LoKi::Holder<TYPE1, TYPE2> Type;
    // ========================================================================
    //     // the actual signature
    // ========================================================================
    /// the actual type for the argument
    typedef typename boost::call_traits<Type>::param_type argument;
    /// the actual type for the first argument
    typedef typename boost::call_traits<TYPE1>::param_type first_argument;
    /// teh actual type for the second argument
    typedef typename boost::call_traits<TYPE2>::param_type second_argument;
    // ========================================================================
  public:
    // ========================================================================
    /// the native interface with 1 argument:
    virtual TYPE3 operator()( argument a ) const = 0;
    /// the only one essential method ("function")
    virtual TYPE3 evaluate( argument a ) const { return ( *this )( a ); }
    /// the only one essential method ("function")
    virtual TYPE3 eval( argument a ) const { return ( *this )( a ); }
    /// the only one essential method ("function")
    virtual TYPE3 operator()( first_argument a1, second_argument a2 ) const { return ( *this )( Type( a1, a2 ) ); }
    /// the only one essential method ("function")
    virtual TYPE3 evaluate( first_argument a1, second_argument a2 ) const { return ( *this )( a1, a2 ); }
    /// the only one essential method ("function")
    virtual TYPE3 eval( first_argument a1, second_argument a2 ) const { return ( *this )( a1, a2 ); }
    /// MANDATORY: clone method
    virtual Functor* clone() const = 0;
    /// virtual destructor
    virtual ~Functor() = default;
    // ========================================================================
  public:
    // ========================================================================
    /// protected default constructor
    Functor() = default;
    /// protected copy constructor
    Functor( const Functor& fun ) = default;
    // ========================================================================
    /// assignement is disallowed
    Functor& operator=( const Functor& ) = delete;
    // ========================================================================
  };
  // ==========================================================================
  /// Specialization for 2-argument function
  template <class TYPE1, class TYPE2, class TYPE3>
  class FunctorFromFunctor<LoKi::Holder<TYPE1, TYPE2>, TYPE3>
      : public LoKi::Functor<LoKi::Holder<TYPE1, TYPE2>, TYPE3> {
  public:
    // ========================================================================
    /// the underlying type of functor
    typedef LoKi::Functor<LoKi::Holder<TYPE1, TYPE2>, TYPE3> functor;
    typedef LoKi::Functor<LoKi::Holder<TYPE1, TYPE2>, TYPE3> Base;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    FunctorFromFunctor( const functor& right ) : m_fun( right.clone() ) {}
    /// copy constructor (deep copy)
    FunctorFromFunctor( const FunctorFromFunctor& right )
        : LoKi::AuxFunBase( right ), Base( right ), m_fun( right.m_fun->clone() ) {}
    /// move constructor (avoid cloning)
    FunctorFromFunctor( FunctorFromFunctor&& right ) = default;
    // ========================================================================
  public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    FunctorFromFunctor* clone() const override { return new FunctorFromFunctor( *this ); }
    /// MANDATORY: the only one essential method
    TYPE3 operator()( typename functor::argument a ) const override { return fun( a ); }
    /// the only one essential method ("function")
    TYPE3 operator()( typename functor::first_argument a, typename functor::second_argument b ) const override {
      return fun( a, b );
    }
    /// OPTIONAL: the basic printout method, delegate to the underlying object
    std::ostream& fillStream( std::ostream& s ) const override { return m_fun->fillStream( s ); };
    /// OPTIONAL: unique function ID, delegate to the underlying objects
    std::size_t id() const override { return m_fun->id(); }
    /// OPTIONAL: delegate the object type
    std::string objType() const override { return m_fun->objType(); }
    std::string toCpp() const override { return m_fun->toCpp(); }
    // ========================================================================
  public:
    // ========================================================================
    /// the assignement operator is enabled
    FunctorFromFunctor& operator=( FunctorFromFunctor rhs ) {
      swap( rhs, *this );
      return *this; // RETURN
    }
    // ========================================================================
    friend void swap( FunctorFromFunctor& lhs, FunctorFromFunctor& rhs ) noexcept {
      using std::swap;
      swap( static_cast<AuxFunBase&>( lhs ), static_cast<AuxFunBase&>( rhs ) );
      swap( lhs.m_fun, rhs.m_fun );
    }
    // ========================================================================
  public:
    // ========================================================================
    /// evaluate unary  function
    inline TYPE3 fun( typename functor::argument a ) const { return ( *m_fun )( a ); }
    /// evaluate binary function
    inline TYPE3 fun( typename functor::first_argument a, typename functor::second_argument b ) const {
      return ( *m_fun )( a, b );
    }
    /// accessor to the function itself
    inline const functor& func() const { return *m_fun; }
    // ========================================================================
  private:
    // ========================================================================
    /// the underlaying function
    std::unique_ptr<const functor> m_fun; // the underlaying functor
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_BIFUNCTIONS_H
