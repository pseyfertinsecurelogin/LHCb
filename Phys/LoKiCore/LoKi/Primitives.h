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
#ifndef LOKI_PRIMITIVES_H
#define LOKI_PRIMITIVES_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include "GaudiKernel/SerializeSTL.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <type_traits>
// ============================================================================
// LHCb
// ============================================================================
#include "LHCbMath/Bit.h"
#include "LHCbMath/Digit.h"
#include "LHCbMath/LHCbMath.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BiFunctions.h"
#include "LoKi/Field.h"
#include "LoKi/Functions.h"
#include "LoKi/Operations.h"
#include "LoKi/Reference.h"
#include "LoKi/apply.h"
#include "LoKi/same.h"
#include "LoKi/valid.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/integer_traits.hpp"
// ============================================================================
#define LOKI_REQUIRES( ... ) std::enable_if_t<( __VA_ARGS__ ), bool> = true
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
 *
 */
// ============================================================================
namespace LoKi {
  namespace V2 {

    // ==========================================================================
    /** @class Functors
     *  helper class to keep N functors of the same signature
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-01
     */

    template <typename Signature, size_t N>
    class FunctorArray final {
    public:
      // ========================================================================
      // avoid matching the various special members
      template <typename F, typename... Fs,
                LOKI_REQUIRES( sizeof...( Fs ) != 0 ||
                               !std::is_base_of_v<FunctorArray<Signature, N>, std::remove_reference_t<F>> )>
      FunctorArray( F&& f, Fs&&... fs ) : m_funs{std::forward<F>( f ), std::forward<Fs>( fs )...} {}
      // ========================================================================
      /// evaluate the i-th functor
      template <size_t i, typename... Args>
      auto fun( Args&&... args ) const {
        return std::get<i>( m_funs ).fun( std::forward<Args>( args )... );
      }

      template <typename... Args>
      auto fun1( Args&&... args ) const {
        return fun<0>( std::forward<Args>( args )... );
      }
      template <typename... Args>
      auto fun2( Args&&... args ) const {
        return fun<1>( std::forward<Args>( args )... );
      }
      // ========================================================================
      /// get the i-th functor
      template <size_t i>
      const auto& func() const {
        return std::get<i>( m_funs ).func();
      }
      /// get the first functor
      const auto& func1() const { return func<0>(); }
      /// get the second functor
      const auto& func2() const { return func<1>(); }
      // ========================================================================
    private:
      // ========================================================================
      /// the functors
      std::array<FunctorFromFunctor<Signature>, N> m_funs; // the functors
      // ========================================================================
    };
    template <typename Signature>
    using TwoFunctors = FunctorArray<Signature, 2>;

    namespace details {

      // =======================================================================
      /** @class UnaryOp
       *  The helper function to implement Unary operation of one function
       */

      template <typename Traits, typename Signature, typename Result = details::result_of_t<Signature>>
      class UnaryOp;

      template <typename... TYPE, typename TYPE2, typename Result, typename Traits_>
      class UnaryOp<Traits_, TYPE2( TYPE... ), Result> final : public Functor<Result( TYPE... )> {
        // ========================================================================
      public:
        // =====================================================================
        /// constructor
        UnaryOp( FunctorFromFunctor<TYPE2( TYPE... )> f ) : LoKi::AuxFunBase{std::tie( f )}, m_fun{std::move( f )} {}
        /// clone method (mandatory)
        UnaryOp* clone() const override { return new UnaryOp( *this ); }

        /// the only one essential method ("function") -- either withour argument
        Result operator()( Param_t<TYPE>... a ) const override { return Traits_::unaryOp( this->m_fun, a... ); }

        /// the basic printout method
        std::ostream& fillStream( std::ostream& s ) const override { return Traits_::fillStream( s, this->m_fun ); }
        // =====================================================================
      private:
        // =====================================================================
        /// the storage of the functor
        FunctorFromFunctor<TYPE2( TYPE... )> m_fun; // the storage of the functor
        // =====================================================================
      };

      template <typename Op, typename Prefix>
      struct SimpleUnary {
        template <typename F, typename... Args>
        static auto unaryOp( const F& f, const Args&... args ) -> decltype( auto ) {
          return Op{}( f( args... ) );
        }
        template <typename F>
        static std::ostream& fillStream( std::ostream& os, const F& f ) {
          return os << " (" << Prefix::name() << f << ") ";
        }
      };
    } // namespace details

    // ==========================================================================
    /** @class Not
     *
     *  The helper function to implement logical negation
     *  of the predicate
     *
     *  @code
     *
     *  typedef Functor<SomeType,bool>            Pred ;
     *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
     *
     *  void func( const Pred& A )
     *  {
     *    PfP p2 = LoKi::Not( A ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    namespace Traits {
      struct Not : details::SimpleUnary<std::logical_not<>, Not> {
        static constexpr const char* name() { return "~"; }
      };
    } // namespace Traits
    template <typename Signature>
    using Not = details::UnaryOp<Traits::Not, Signature, bool>;

    // ==========================================================================
    /** @class Negate
     *
     *  The helper function to implement unary negate
     *  of the function
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A )
     *  {
     *    FfF p1 = -A                ; // operator form
     *    FfF p2 = LoKi::Negate( A ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    namespace Traits {
      struct Negate : details::SimpleUnary<std::negate<>, Negate> {
        static constexpr const char* name() { return "-"; }
      };
    } // namespace Traits
    template <typename Signature>
    using Negate = details::UnaryOp<Traits::Negate, Signature>;

    namespace details {

      // =======================================================================
      /** @class BinaryOp
       *  The helper function to implement Binary operation of two functions
       */

      template <typename Traits_, typename Signature, typename Result = details::result_of_t<Signature>>
      class BinaryOp;

      template <typename Traits_, typename TYPE2, typename... TYPE, typename Result>
      class BinaryOp<Traits_, TYPE2( TYPE... ), Result> final : public Functor<Result( TYPE... )> {
        // =====================================================================
      public:
        // =====================================================================
        /// constructor from two functors
        BinaryOp( FunctorFromFunctor<TYPE2( TYPE... )> f1, FunctorFromFunctor<TYPE2( TYPE... )> f2 )
            : LoKi::AuxFunBase{std::tie( f1, f2 )}, m_two{std::move( f1 ), std::move( f2 )} {}
        /// clone method (mandatory)
        BinaryOp* clone() const override { return new BinaryOp( *this ); }

        /// the only one essential method ("function")
        Result operator()( details::Param_t<TYPE>... a ) const override {
          return Traits_::binaryOp( m_two.func1(), m_two.func2(), a... );
        }

        /// the basic printout method
        std::ostream& fillStream( std::ostream& s ) const override {
          return Traits_::fillStream( s, m_two.func1(), m_two.func2() );
        }
        // =====================================================================
      private:
        // =====================================================================
        /// the storage of two functors
        TwoFunctors<TYPE2( TYPE... )> m_two; // the storage of two functors
        // =====================================================================
      };

      template <typename Op, typename Infix>
      struct SimpleBinary {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, const Args&... args ) -> decltype( auto ) {
          return Op{}( f1( args... ), f2( args... ) );
        }

        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " (" << f1 << Infix::name() << f2 << ") ";
        }
      };
    } // namespace details

    // ==========================================================================
    /** @class And
     *  helper function to implement logical AND of 2 predicates
     *
     *  It is used by LoKi for implementation of logical
     *  binary <tt>&&</tt> operator for predicates
     *
     *  @code
     *
     *  typedef Functor<SomeType,bool>            Pred ;
     *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
     *
     *  void func( const Pred& A , const Pred& B )
     *  {
     *    PfP p1 = A && B             ; // operator form
     *    PfP p2 = LoKi::And( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      struct And {
        template <typename F1, typename F2, typename... Args>
        static bool binaryOp( const F1& f1, const F2& f2, const Args&... args ) {
          return f1( args... ) && f2( args... );
        }

        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " (" << f1 << "&" << f2 << ") ";
        }
      };
    } // namespace Traits
    template <typename Signature>
    using And = details::BinaryOp<Traits::And, Signature, bool>;

    // ==========================================================================
    /** @struct Or
     *  helper function to implement logical OR of 2 predicates
     *
     *  It is used by LoKi for implementation of logical
     *  binary <tt>||</tt> operator for predicates
     *
     *  @code
     *
     *  typedef Functor<SomeType,bool>            Pred ;
     *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
     *
     *  void func( const Pred& A , const Pred& B )
     *  {
     *    PfP p1 = A || B            ;  // operator form
     *    PfP p2 = LoKi::Or( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    // ==========================================================================

    namespace Traits {
      struct Or {
        template <typename F1, typename F2, typename... Args>
        static bool binaryOp( const F1& f1, const F2& f2, const Args&... args ) {
          return f1( args... ) || f2( args... );
        }

        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " (" << f1 << "|" << f2 << ") ";
        }
      };
    } // namespace Traits
    template <typename Signature>
    using Or = details::BinaryOp<Traits::Or, Signature, bool>;

    // ==========================================================================
    /** @class Less
     *  The helper function to implement Less of 2 functions
     *
     *  It is used by LoKi for implementation of comparison
     *  operator for functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,bool>   PfP  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    PfP p1 = A < B               ;  // operator form
     *    PfP p2 = LoKi::Less( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FuctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    namespace Traits {
      struct Less : details::SimpleBinary<std::less<>, Less> {
        static constexpr const char* name() { return "<"; }
      };
    } // namespace Traits
    template <typename Signature>
    using Less = details::BinaryOp<Traits::Less, Signature, bool>;

    // ==========================================================================
    /** @class Equal
     *  The helper function to implement Equal of 2 functions
     *
     *  It is used by LoKi for implementation of comparison
     *  operator for functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,bool>   PfP   ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    PfP p1 = A == B               ; // operator form
     *    PfP p2 = LoKi::Equal( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    namespace Traits {
      template <typename TYPE2>
      struct Equal : details::SimpleBinary<LHCb::Math::Equal_To<TYPE2>, Equal<TYPE2>> {
        static constexpr const char* name() { return "=="; }
      };
    } // namespace Traits
    template <typename Signature>
    using Equal = details::BinaryOp<Traits::Equal<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @struct LessOrEqual
     *  The helper function to implement Less of 2 functions
     *
     *  It is used by LoKi for implementation of comparison
     *  operator for functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,bool>   PfP  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    PfP p1 = A <= B                ;  // operator form
     *    PfP p2 = LoKi::LessOrEqual( A , B ) ; // explicit form
     *  }
     *  @endcode
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *  @see LoKi::Function
     *  @see LoKi::PredicateFromPredicate
     *  @see LoKi::PredicateFromTwoFunctions
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    namespace Traits {
      struct LessOrEqual : details::SimpleBinary<std::less_equal<>, LessOrEqual> {
        static constexpr const char* name() { return "<="; }
      };
    } // namespace Traits
    template <typename Signature>
    using LessOrEqual = details::BinaryOp<Traits::LessOrEqual, Signature, bool>;

    // ==========================================================================
    /** @class NotEqual
     *  The helper function to implement NotEqual of 2 functions
     *
     *  It is used by LoKi for implementation of comparison
     *  operator for functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,bool>   PfP  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    PfP p1 = A != B                  ; // operator form
     *    PfP p2 = LoKi::NotEqual( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      template <typename TYPE2>
      struct NotEqual {
        template <typename F1, typename F2, typename... Args>
        static bool binaryOp( const F1& f1, const F2& f2, const Args&... args ) {
          LHCb::Math::Equal_To<TYPE2> op{};
          return !op( f1( args... ), f2( args... ) );
        }
        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " (" << f1 << "!=" << f2 << ") ";
        }
      };
    } // namespace Traits
    template <typename Signature>
    using NotEqual = details::BinaryOp<Traits::NotEqual<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @class Plus
     *  The helper function to implement addition of 2 function
     *
     *  It is used by LoKi for implementation of addition of
     *  2 functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = A  + B              ; // operator form
     *    FfF f2 = LoKi::Plus( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      struct Plus : details::SimpleBinary<std::plus<>, Plus> {
        static constexpr const char* name() { return "+"; }
      };
    } // namespace Traits

    template <typename Signature>
    using Plus = details::BinaryOp<Traits::Plus, Signature>;
    // ==========================================================================
    /** @class Minus
     *  The helper function to implement subtraction of 2 function
     *
     *  It is used by LoKi for implementation of subtraction of
     *  2 functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = A - B                ; // operator form
     *    FfF f2 = LoKi::Minus( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      struct Minus : details::SimpleBinary<std::minus<>, Minus> {
        static constexpr const char* name() { return "-"; }
      };
    } // namespace Traits

    template <typename Signature>
    using Minus = details::BinaryOp<Traits::Minus, Signature>;

    // ==========================================================================
    /** @class Divide
     *  The helper function to implement division of 2 function
     *
     *  It is used by LoKi for implementation of division of
     *  2 functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = A / B                 ; // operator form
     *    FfF f2 = LoKi::Divide( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      struct Divide : details::SimpleBinary<std::divides<>, Divide> {
        static constexpr const char* name() { return "/"; }
      };
    } // namespace Traits

    template <typename Signature>
    using Divide = details::BinaryOp<Traits::Divide, Signature>;
    // ==========================================================================
    /** @class Multiply
     *  The helper function to implement multiplication of 2 functions
     *
     *  It is used by LoKi for implementation of multiplication of
     *  2 functions:
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = A * B                   ; // operator form
     *    FfF f2 = LoKi::Multiply( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */

    namespace Traits {
      struct Multiply : details::SimpleBinary<std::multiplies<>, Multiply> {
        static constexpr const char* name() { return "*"; }
      };
    } // namespace Traits

    template <typename Signature>
    using Multiply = details::BinaryOp<Traits::Multiply, Signature>;
    // ==========================================================================
    /** @class Min
     *
     *  Simple functor to find the minimum for the functors
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f2 = LoKi::Min( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFomrFunctor
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-01
     */
    namespace Traits {
      struct Min {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, const Args&... args ) {
          return std::min( f1( args... ), f2( args... ) );
        }

        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " min(" << f1 << "," << f2 << ") ";
        }
      };
    } // namespace Traits
    template <typename Signature>
    using Min = details::BinaryOp<Traits::Min, Signature>;
    // ==========================================================================
    /** @class Max
     *  Return  the maximum from functions
     *
     *  Simple functor to find the inimum for the functors
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f2 = LoKi::Max( A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFomrFunctor
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-01
     */
    namespace Traits {
      struct Max {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, const Args&... args ) {
          return std::max( f1( args... ), f2( args... ) );
        }

        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& os, const F1& f1, const F2& f2 ) {
          return os << " max(" << f1 << "," << f2 << ") ";
        }
      };
    } // namespace Traits
    template <typename Signature>
    using Max = details::BinaryOp<Traits::Max, Signature>;

    // ==========================================================================
    /** @class SimpleSwitch
     *  It is a function with acts similar to switch:
     *
     *  result = condition ? value1 : value2 ;
     *
     *  In particular it is useful for "conversion"
     *   of "predicates" into "functions"
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-11
     */

    template <typename Signature>
    class SimpleSwitch;

    template <typename... TYPE, typename TYPE2>
    class SimpleSwitch<TYPE2( TYPE... )> final : public Functor<TYPE2( TYPE... )> {
    public:
      // ========================================================================
      /** constructor from the predicate and 2 constants
       *
       *  The function return a value of the first constant
       *  if predicate evaluated to "true", otherwise the
       *  second constant is returned
       *
       *  @param cut predicate for branch
       *  @param val1 the first constant
       *  @param val2 the second constant
       *
       *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
       *  @date 2005-02-11
       */
      SimpleSwitch( FunctorFromFunctor<bool( TYPE... )> cut, TYPE2 val1, TYPE2 val2 )
          : LoKi::AuxFunBase( std::tie( cut, val1, val2 ) )
          , m_cut( std::move( cut ) )
          , m_val1( std::move( val1 ) )
          , m_val2( std::move( val2 ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      SimpleSwitch* clone() const override { return new SimpleSwitch( *this ); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()( details::Param_t<TYPE>... a ) const override { return m_cut( a... ) ? m_val1 : m_val2; }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " switch(" << this->m_cut << "," << this->m_val1 << "," << this->m_val2 << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the condition
      FunctorFromFunctor<bool( TYPE... )> m_cut; // the condiftion
      /// the first value
      TYPE2 m_val1; // the first value
      /// the second value
      TYPE2 m_val2; // the second value
      // ========================================================================
    };

    // ==========================================================================
    /** @class Switch
     *
     *  It is a bit advances version fo SimpleSwitch function
     *   with acts similar to switch:
     *
     *  result = condition ? function1 : function2 ;
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-11
     */
    template <typename Signature>
    class Switch;

    template <typename... TYPE, typename TYPE2> // def TYPE2=double
    class Switch<TYPE2( TYPE... )> final : public Functor<TYPE2( TYPE... )> {
      using T2 = typename Constant<TYPE2( TYPE... )>::T2;
      // ========================================================================
    public:
      // ========================================================================
      /** constructor from predicate and 2 functions
       *
       *  The function return a value of the first function
       *  if predicate evaluates to "true", otherwise the
       *  value of second function is returned
       *
       *  @param cut predicate for branch
       *  @param fun1 the first  function
       *  @param fun2 the second function
       *
       *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
       *  @date 2005-02-11
       */
      Switch( FunctorFromFunctor<bool( TYPE... )> cut, FunctorFromFunctor<TYPE2( TYPE... )> fun1,
              FunctorFromFunctor<TYPE2( TYPE... )> fun2 )
          : LoKi::AuxFunBase( std::tie( cut, fun1, fun2 ) )
          , m_cut( std::move( cut ) )
          , m_two( std::move( fun1 ), std::move( fun2 ) ) {}
      /** constructor from predicate ,function and constant
       *
       *  The function return a value of the function
       *  if predicate evaluates to "true", otherwise the
       *  value of constant is returned
       *
       *  @param cut predicate for branch
       *  @param fun1 the function
       *  @param fun2 the constant
       *
       *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
       *  @date 2005-02-11
       */
      Switch( FunctorFromFunctor<bool( TYPE... )> cut, FunctorFromFunctor<TYPE2( TYPE... )> fun1, T2 fun2 )
          : Switch{std::move( cut ), std::move( fun1 ), Constant<TYPE2( TYPE... )>{std::move( fun2 )}} {}
      /** constructor from predicate ,function and constant
       *
       *  The function return a value of the constant
       *  if predicate evaluates to "true", otherwise the
       *  value of function is returned
       *
       *  @param cut predicate for branch
       *  @param fun1 the constant
       *  @param fun2 the function
       *
       *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
       *  @date 2005-02-11
       */
      Switch( FunctorFromFunctor<bool( TYPE... )> cut, T2 val1, FunctorFromFunctor<TYPE2( TYPE... )> fun2 )
          : Switch{std::move( cut ), Constant<TYPE2( TYPE... )>{std::move( val1 )}, std::move( fun2 )} {}
      /** constructor from predicate and 2 constants
       *
       *  The function return a value of the first constant
       *  if predicate evaluates to "true", otherwise the
       *  second constant is returned
       *
       *  The constructed fuction object has the same
       *  functionality as SimpleSwitch, but a bit
       *  less performant.
       *  @see SimpleSwitch
       *
       *  @param cut predicate for branch
       *  @param val1 the first constant
       *  @param val2 the second constant
       *
       *
       *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
       *  @date 2005-02-11
       */
      Switch( FunctorFromFunctor<bool( TYPE... )> cut, T2 val1, T2 val2 )
          : Switch{std::move( cut ), Constant<TYPE2( TYPE... )>{std::move( val1 )},
                   Constant<TYPE2( TYPE... )>{std::move( val2 )}} {}
      /// MANDATORY: clone method ("virtual constructor")
      Switch* clone() const override { return new Switch( *this ); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()( details::Param_t<TYPE>... a ) const override {
        return m_cut.fun( a... ) ? m_two.fun1( a... ) : m_two.fun2( a... );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " switch(" << this->m_cut << "," << this->m_two.func1() << "," << this->m_two.func2() << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the condition
      FunctorFromFunctor<bool( TYPE... )> m_cut; // the condition
      /// the actual storage of two functors
      TwoFunctors<TYPE2( TYPE... )> m_two; // the storage of functors
      // ========================================================================
    };
    // ==========================================================================
    /** @class ComposeFunction
     *  The helper class to implement function of function
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = sin( A )                         ; // short form
     *    FfF f2 = LoKi::ComposeFunction( sin , A ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFronFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    template <typename Signature>
    class ComposeFunction;

    template <typename... TYPE, typename TYPE2>
    class ComposeFunction<TYPE2( TYPE... )> final : public Functor<TYPE2( TYPE... )> {
      // ========================================================================
      /// the actual type for the function
      typedef double ( *Func )( double );
      // ========================================================================
    public:
      // ========================================================================
      /// constructor
      ComposeFunction( Func func, FunctorFromFunctor<TYPE2( TYPE... )> fun, const std::string& desc = "'fun'" )
          : m_fun( std::move( fun ) ), m_func( func ), m_desc( desc ) {}
      /// clone method (mandatory!)
      ComposeFunction* clone() const override { return new ComposeFunction( *this ); }
      /// the only one essential method ("function")
      TYPE2 operator()( details::Param_t<TYPE>... a ) const override { return ( *m_func )( m_fun.fun( a... ) ); }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " " << this->m_desc << "(" << this->m_fun << ") ";
      };
      // to C++
      std::string toCpp() const override {
        return "LoKi::" + this->m_desc + "(" + Gaudi::Utils::toCpp( this->m_fun ) + ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor
      FunctorFromFunctor<TYPE2( TYPE... )> m_fun; // the functor
      /// the function:
      Func m_func; // the function:
      /// the function description:
      std::string m_desc; // description
      // ========================================================================
    };
    // ==========================================================================
    /** @class ComposeFunction2
     *
     *  The helper structure to implement function of function
     *
     *  @code
     *
     *  typedef Functor<SomeType,double>            Func ;
     *  typedef FunctorFromFunctor<Sometype,double> FfF ;
     *
     *  void func( const Func& A , const Func& B )
     *  {
     *    FfF f1 = pow( A , B )                          ; // short form
     *    FfF f2 = LoKi::ComposeFunction2 ( pow , A , B ) ; // explicit form
     *  }
     *
     *  @endcode
     *
     *  @see LoKi/Operators.h
     *  @see LoKi::Functor
     *  @see LoKi::FunctorFromFunctor
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    template <typename Signature>
    class ComposeFunction2;

    template <typename... TYPE, typename TYPE2>
    class ComposeFunction2<TYPE2( TYPE... )> final : public Functor<TYPE2( TYPE... )> {
      /// the actual type of the function
      typedef double ( *Func )( double, double );
      /// constant type
      using T2 = typename Constant<TYPE2( TYPE... )>::T2;
      // ========================================================================
    public:
      // ========================================================================
      /// constructor
      ComposeFunction2( Func func, FunctorFromFunctor<TYPE2( TYPE... )> fun1, FunctorFromFunctor<TYPE2( TYPE... )> fun2,
                        const std::string& desc = "'fun'" )
          : m_func( func ), m_two( std::move( fun1 ), std::move( fun2 ) ), m_desc( desc ) {}
      /// constructor
      ComposeFunction2( Func func, FunctorFromFunctor<TYPE2( TYPE... )> fun1, T2 val2,
                        const std::string& desc = "'fun'" )
          : ComposeFunction2( func, std::move( fun1 ), Constant<TYPE2( TYPE... )>{std::move( val2 )}, desc ) {}
      /// constructor
      ComposeFunction2( Func func, T2 val1, FunctorFromFunctor<TYPE2( TYPE... )> fun2,
                        const std::string& desc = "'fun'" )
          : ComposeFunction2( func, Constant<TYPE2( TYPE... )>{std::move( val1 )}, std::move( fun2 ), desc ) {}
      /// constructor
      ComposeFunction2( Func func, T2 val1, T2 val2, const std::string& desc = "'fun'" )
          : ComposeFunction2( func, Constant<TYPE2( TYPE... )>{std::move( val1 )},
                              Constant<TYPE2( TYPE... )>{std::move( val2 )}, desc ) {}
      /// clone method (mandatory!)
      ComposeFunction2* clone() const override { return new ComposeFunction2( *this ); }
      /// the only one essential method ("function")
      TYPE2 operator()( details::Param_t<TYPE>... a ) const override {
        return ( *m_func )( m_two.fun1( a... ), m_two.fun2( a... ) );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " " << m_desc << "(" << m_two.func1() << "," << m_two.func2() << ") ";
      }
      /// to C++
      std::string toCpp() const override {
        return "LoKi::" + this->m_desc + "(" + Gaudi::Utils::toCpp( this->m_two.func1() ) + ", " +
               Gaudi::Utils::toCpp( this->m_two.func2() ) + ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the function itself
      Func m_func; // the function itself
      /// the storage of functors
      TwoFunctors<TYPE2( TYPE... )> m_two; // the storage of functors
      /// the funtion descrition
      std::string m_desc; // the funtion descrition
      // ========================================================================
    };

    // ==========================================================================
    /** @class Compose
     *  the general case of fun2(fun1) function:
     */
    template <typename Sig1, typename Sig2>
    class Compose;

    template <typename... TYPE, typename TYPE1, typename TYPE2, typename TYPE3>
    class Compose<TYPE1( TYPE... ), TYPE2( TYPE3 )> final : public Functor<TYPE2( TYPE... )> {
    public:
      // ========================================================================
      /// contructor
      Compose( FunctorFromFunctor<TYPE1( TYPE... )> fun1, FunctorFromFunctor<TYPE2( TYPE3 )> fun2 )
          : AuxFunBase( std::tie( fun1, fun2 ) ), m_fun1( std::move( fun1 ) ), m_fun2( std::move( fun2 ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Compose* clone() const override { return new Compose( *this ); }
      /// the only one essential method ("function")
      TYPE2 operator()( details::Param_t<TYPE>... a ) const override { return m_fun2( m_fun1( a... ) ); }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " (" << this->m_fun1 << ">>" << this->m_fun2 << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the first functor
      FunctorFromFunctor<TYPE1( TYPE... )> m_fun1; // the  first functor
      /// the second functor
      FunctorFromFunctor<TYPE2( TYPE3 )> m_fun2; // the second functor
      // ========================================================================
    };
  } // namespace V2

  template <typename TYPE, typename TYPE1, typename TYPE2, typename TYPE3 = TYPE1>
  using Compose = V2::Compose<details::sig_t<TYPE, TYPE1>, TYPE2( TYPE3 )>;

  // ==========================================================================
  /** @class Valid
   *  The trivial predicate to check the validity of argument.
   *  It is OK for any pointer-like types or for types with
   *  implemented implicit conversion to "bool"
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date 2004-02-11
   */
  template <typename TYPE>
  struct Valid final : Functor<TYPE, bool> {
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    Valid* clone() const override { return new Valid( *this ); }
    /// MANDATORY: the only one essential method
    bool operator()( V2::details::Param_t<TYPE> a ) const override { return LoKi::valid( a ); }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override { return s << " (Valid?)"; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class TheSame
   *  Trivial predicate to check if the argument
   *  is equal to some predefined value.
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date 2004-02-11
   *  @warning this functor does not have a valid <c>toCpp</c> method
   */
  template <typename TYPE>
  class TheSame final : public Functor<TYPE, bool> {
  public:
    // ========================================================================
    /// constructor form the value
    explicit TheSame( TYPE value ) : m_value( std::move( value ) ) {}
    /// MANDATORY: clone method ("virtual constructor")
    TheSame* clone() const override { return new TheSame( *this ); }
    /// MANDATORY: the only one essential method
    bool operator()( V2::details::Param_t<TYPE> arg ) const override { return LoKi::same( m_value, arg ); }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override { return s << " (SAME?) "; }
    // ========================================================================
  private:
    // ========================================================================
    // the predefined value
    TYPE m_value;
    // ========================================================================
  };

  namespace V2 {
    namespace details {
      namespace Bind {
        struct Second_t {};                            // fun is first, bound value is second
        struct First_t {};                             // value is first, fun is second
        struct FirstOrSecond_t : First_t, Second_t {}; // order doesn't matter...

        template <typename T>
        inline constexpr bool Second_v = std::is_base_of_v<Second_t, typename T::Bind_t>;
        template <typename T>
        inline constexpr bool First_v = std::is_base_of_v<First_t, typename T::Bind_t>;

      } // namespace Bind

      template <typename Traits_, typename Signature, typename Result = details::result_of_t<Signature>>
      class BindBinary;

      template <typename Traits_, typename... TYPE, typename TYPE2, typename Result>
      class BindBinary<Traits_, TYPE2( TYPE... ), Result> final : public Functor<Result( TYPE... )> {
      public:
        // ========================================================================
        /** constructor from the function and the value
         *  @param fun the function
         *  @param val the reference value
         */
        template <typename T, LOKI_REQUIRES( std::is_constructible_v<TYPE2, T>&& Bind::Second_v<Traits_> )>
        BindBinary( FunctorFromFunctor<TYPE2( TYPE... )> fun, T&& val )
            : AuxFunBase( std::tie( fun, val ) ), m_fun( std::move( fun ) ), m_val( std::forward<T>( val ) ) {}
        template <typename T, LOKI_REQUIRES( std::is_constructible_v<TYPE2, T>&& Bind::First_v<Traits_> )>
        BindBinary( T&& val, FunctorFromFunctor<TYPE2( TYPE... )> fun )
            : AuxFunBase( std::tie( val, fun ) ), m_fun( std::move( fun ) ), m_val( std::forward<T>( val ) ) {}
        // ========================================================================
        /// MANDATORY: clone method ("virtual construcor")
        BindBinary* clone() const override { return new BindBinary( *this ); }
        /// MANDATORY: the only one essential method :
        Result operator()( details::Param_t<TYPE>... a ) const override {
          return Bind::Second_v<Traits_> ? Traits_::binaryOp( this->m_fun.fun( a... ), this->m_val )
                                         : Traits_::binaryOp( this->m_val, this->m_fun.fun( a... ) );
        }
        /// OPTIONAL: the specific printout
        std::ostream& fillStream( std::ostream& s ) const override {
          return Bind::Second_v<Traits_> ? ( s << " (" << this->m_fun << Traits_::infix() << this->m_val << ") " )
                                         : ( s << " (" << this->m_val << Traits_::infix() << this->m_fun << ") " );
        }
        // ========================================================================
      private:
        // ========================================================================
        /// the functor
        FunctorFromFunctor<TYPE2( TYPE... )> m_fun; // the functor
        /// the value
        TYPE2 m_val; // the value
        // ========================================================================
      };

    } // namespace details

    // ==========================================================================
    /** @class EqualToValue
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-04-07
     */
    namespace Traits {
      template <typename TYPE2>
      struct EqualToValue {
        using Bind_t = details::Bind::FirstOrSecond_t;
        static constexpr const char* infix() { return "=="; }
        template <typename V1, typename V2>
        static bool binaryOp( const V1& v1, const V2& v2 ) {
          LHCb::Math::Equal_To<TYPE2> _cmp;
          return _cmp( v1, v2 );
        }
      };
    } // namespace Traits
    template <typename Signature>
    using EqualToValue = details::BindBinary<Traits::EqualToValue<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @class NotEqualToValue
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-04-07
     */
    namespace Traits {
      template <typename TYPE2>
      struct NotEqualToValue {
        using Bind_t = typename EqualToValue<TYPE2>::Bind_t;
        static constexpr const char* infix() { return "!="; }
        template <typename V1, typename V2>
        static bool binaryOp( const V1& v1, const V2& v2 ) {
          return !EqualToValue<TYPE2>::binaryOp( v1, v2 );
        }
      };
    } // namespace Traits
    template <typename Signature>
    using NotEqualToValue =
        details::BindBinary<Traits::NotEqualToValue<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @class LessThanValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-19
     */
    namespace Traits {
      struct LessThanValue {
        using Bind_t = details::Bind::Second_t;
        static constexpr const char* infix() { return "<"; }
        template <typename V1, typename V2>
        static constexpr bool binaryOp( const V1& v1, const V2& v2 ) {
          return v1 < v2;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using LessThanValue = details::BindBinary<Traits::LessThanValue, Signature, bool>;

    // ==========================================================================
    /** @class LessOrEqualValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-19
     */
    namespace Traits {
      template <typename TYPE2>
      struct LessOrEqualValue {
        using Bind_t = details::Bind::Second_t;
        static constexpr const char* infix() { return "<="; }
        template <typename V1, typename V2>
        static bool binaryOp( const V1& v1, const V2& v2 ) {
          return LessThanValue::binaryOp( v1, v2 ) || EqualToValue<TYPE2>::binaryOp( v1, v2 );
        }
      };
    } // namespace Traits
    template <typename Signature>
    using LessOrEqualValue =
        details::BindBinary<Traits::LessOrEqualValue<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @class GreaterThanValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-19
     */
    namespace Traits {
      struct GreaterThanValue {
        using Bind_t = details::Bind::Second_t;
        static constexpr const char* infix() { return ">"; }
        template <typename V1, typename V2>
        static constexpr bool binaryOp( const V1& v1, const V2& v2 ) {
          return v2 < v1;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using GreaterThanValue = details::BindBinary<Traits::GreaterThanValue, Signature, bool>;

    // ==========================================================================
    /** @class GreaterOrEqualValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-19
     */
    namespace Traits {
      template <typename TYPE2>
      struct GreaterOrEqualValue {
        using Bind_t = details::Bind::Second_t;
        static constexpr const char* infix() { return ">="; }
        template <typename V1, typename V2>
        static bool binaryOp( const V1& v1, const V2& v2 ) {
          return GreaterThanValue::binaryOp( v1, v2 ) || EqualToValue<TYPE2>::binaryOp( v1, v2 );
        }
      };
    } // namespace Traits
    template <typename Signature>
    using GreaterOrEqualValue =
        details::BindBinary<Traits::GreaterOrEqualValue<details::result_of_t<Signature>>, Signature, bool>;

    // ==========================================================================
    /** @class MultiplyByValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    namespace Traits {
      struct MultiplyByValue {
        using Bind_t = details::Bind::FirstOrSecond_t;
        static constexpr const char* infix() { return "*"; }
        template <typename V1, typename V2>
        static constexpr auto binaryOp( const V1& v1, const V2& v2 ) {
          return v1 * v2;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using MultiplyByValue = details::BindBinary<Traits::MultiplyByValue, Signature>;

    // ==========================================================================
    /** @class SumByValue
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    namespace Traits {
      struct SumByValue {
        using Bind_t = details::Bind::FirstOrSecond_t;
        static constexpr const char* infix() { return "+"; }
        template <typename V1, typename V2>
        static constexpr auto binaryOp( const V1& v1, const V2& v2 ) {
          return v1 + v2;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using SumByValue = details::BindBinary<Traits::SumByValue, Signature>;

    // ==========================================================================
    /** @class Minus1
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    namespace Traits {
      template <typename FirstOrSecond>
      struct BindMinus {
        using Bind_t = FirstOrSecond;
        static constexpr const char* infix() { return "-"; }
        template <typename V1, typename V2>
        static constexpr auto binaryOp( const V1& v1, const V2& v2 ) {
          return v1 - v2;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using Minus1 = details::BindBinary<Traits::BindMinus<details::Bind::Second_t>, Signature>;

    // ==========================================================================
    /** @class Minus2
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    template <typename Signature>
    using Minus2 = details::BindBinary<Traits::BindMinus<details::Bind::First_t>, Signature>;

    // ==========================================================================
    /** @class Divide1
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    namespace Traits {
      template <typename FirstOrSecond>
      struct BindDivide {
        using Bind_t = FirstOrSecond;
        static constexpr const char* infix() { return "/"; }
        template <typename V1, typename V2>
        static constexpr auto binaryOp( const V1& v1, const V2& v2 ) {
          return v1 / v2;
        }
      };
    } // namespace Traits
    template <typename Signature>
    using Divide1 = details::BindBinary<Traits::BindDivide<details::Bind::Second_t>, Signature>;

    // ==========================================================================
    /** @class Divide2
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2015-01-27
     */
    template <typename Signature>
    using Divide2 = details::BindBinary<Traits::BindDivide<details::Bind::First_t>, Signature>;

  } // namespace V2

  template <typename TYPE, typename TYPE2>
  using TwoFunctors = V2::TwoFunctors<details::sig_t<TYPE, TYPE2>>;

  // ==========================================================================
  /** compare 2 objects using the comparison criteria CMP ,
   *  applied to functions:
   *  <c> cmp( f1(a) , f2(b) ) </c>
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
  template <typename TYPE, typename CMP = std::less<>, typename TYPE2 = double>
  class Compare final {
  public:
    // ========================================================================
    /// argument type
    typedef typename Functor<TYPE, TYPE2>::argument argument;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor
     *  @param fun1 the first functor
     *  @param fun2 the second functor
     *  @param cmp the comparison criteria
     */
    Compare( FunctorFromFunctor<TYPE, TYPE2> fun1, FunctorFromFunctor<TYPE, TYPE2> fun2, CMP cmp = {} )
        : m_two( std::move( fun1 ), std::move( fun2 ) ), m_cmp( std::move( cmp ) ) {}
    // ========================================================================
    /** constructor
     *  @param fun1 the first functor
     *  @param fun2 the second functor
     *  @param cmp the comparison criteria
     */
    Compare( const Functor<TYPE, TYPE2>& fun1, CMP cmp = {} ) : m_two( fun1, fun1 ), m_cmp( std::move( cmp ) ) {}
    /// the only one essential method
    bool operator()( argument a1, argument a2 ) const { return m_cmp( m_two.fun1( a1 ), m_two.fun2( a2 ) ); }
    // ========================================================================
  public:
    // ========================================================================
    /// get the first functor
    const auto& func1() const { return m_two.func1(); }
    /// get the second functor
    const auto& func2() const { return m_two.func2(); }
    // ========================================================================
  private:
    // ========================================================================
    TwoFunctors<TYPE, TYPE2> m_two;
    CMP                      m_cmp;
    // ========================================================================
  };
  // ==========================================================================
  template <typename TYPE, typename TYPE2, bool>
  struct Cmp;
  // ==========================================================================
  template <typename TYPE, typename TYPE2>
  struct Cmp<TYPE, TYPE2, true> {
    typedef Compare<TYPE, std::less<>, TYPE2> Type;
  };
  // ==========================================================================
  template <typename TYPE, typename TYPE2>
  struct Cmp<TYPE, TYPE2, false> {
    typedef Compare<TYPE, std::greater<>, TYPE2> Type;
  };
  // ==========================================================================
  /** @class Identity
   *  the simple trivial functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
  template <typename TYPE, typename TYPE2 = TYPE>
  struct Identity final : V2::Functor<TYPE2( TYPE )> {
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    Identity* clone() const override { return new Identity( *this ); }
    /// MANDATORY": the only one essential method
    TYPE2 operator()( V2::details::Param_t<TYPE> a ) const override { return a; }
    /// OPTIONAL: the nice printout
    std::ostream& fillStream( std::ostream& s ) const override;
    // ========================================================================
  };
  // ==========================================================================
  /** @class PrintOut
   *  the simple functor, which "converts" the objects to strings
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
  template <typename TYPE>
  struct PrintOut final : V2::Functor<std::string( TYPE )> {
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    PrintOut* clone() const override { return new PrintOut( *this ); }
    /// MANDATORY": the only one essential method
    std::string operator()( V2::details::Param_t<TYPE> a ) const override { return Gaudi::Utils::toString( a ); }
    // ========================================================================
  };
  // ==========================================================================
  /** @class InRange
   *  Helper predicate to represent that the result of functor
   *  is within the certain "range"
   *  @author Vanya BELYAEV Ivan/Belyaev@nikhef.nl
   *  @date 2009-11-21
   */
  namespace V2 {
    template <typename Signature>
    class InRange;

    template <typename... TYPE>
    class InRange<double( TYPE... )> final : public Functor<bool( TYPE... )> {
      // ========================================================================
    public:
      // ========================================================================
      /** constructor from the functor and edges
       *  @param fun the functor
       *  @param low the low edge
       *  @param high the high edge
       */
      InRange( const double low, FunctorFromFunctor<double( TYPE... )> fun, const double high )
          : LoKi::AuxFunBase( std::tie( low, fun, high ) ), m_low( low ), m_fun( std::move( fun ) ), m_high( high ) {}
      /// MANDATORY: clone method ("virtual constructor")
      InRange* clone() const override { return new InRange( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( details::Param_t<TYPE>... a ) const override {
        const auto r = m_fun.fun( a... );
        return m_low <= r && r <= m_high;
      }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " in_range(" << m_low << "," << m_fun << "," << m_high << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the low edge
      double m_low; //       the low edge
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor itself
      /// the high edge
      double m_high; //       the low edge
      // ========================================================================
    };

    // ==========================================================================
    /** @class InRange2
     *  Helper predicate to represent that the result of functor
     *  is within the certain "range"
     *  @author Vanya BELYAEV Ivan/Belyaev@nikhef.nl
     *  @date 2009-11-21
     */

    template <typename Signature>
    class InRange2;

    template <typename... TYPE>
    class InRange2<double( TYPE... )> final : public Functor<bool( TYPE... )> {
      // ========================================================================
    public:
      // ========================================================================
      /** constructor from the functor and edges
       *  @param low the low edge
       *  @param fun the functor
       *  @param high the high edge
       */
      InRange2( FunctorFromFunctor<double( TYPE... )> low, FunctorFromFunctor<double( TYPE... )> fun,
                FunctorFromFunctor<double( TYPE... )> high )
          : LoKi::AuxFunBase( std::tie( low, fun, high ) )
          , m_low( std::move( low ) )
          , m_fun( std::move( fun ) )
          , m_high( std::move( high ) ) {}
      /** constructor from the functor and edges
       *  @param low the low edge
       *  @param fun the functor
       *  @param high the high edge
       */
      InRange2( const double low, FunctorFromFunctor<double( TYPE... )> fun,
                FunctorFromFunctor<double( TYPE... )> high )
          : InRange2{Constant<double( TYPE... )>{low}, std::move( fun ), std::move( high )} {}
      /** constructor from the functor and edges
       *  @param low the low edge
       *  @param fun the functor
       *  @param high the high edge
       */
      InRange2( FunctorFromFunctor<double( TYPE... )> low, FunctorFromFunctor<double( TYPE... )> fun,
                const double high )
          : InRange2{std::move( low ), std::move( fun ), Constant<double( TYPE... )>{high}} {}
      /// MANDATORY: clone method ("virtual constructor")
      InRange2* clone() const override { return new InRange2( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( details::Param_t<TYPE>... a ) const override {
        const auto res = m_fun.fun( a... );
        return m_low.fun( a... ) <= res && res <= m_high.fun( a... );
      }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " in_range(" << m_low << "," << m_fun << "," << m_high << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the low edge
      FunctorFromFunctor<double( TYPE... )> m_low; //       the low edge
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor itself
      /// the high edge
      FunctorFromFunctor<double( TYPE... )> m_high; //      the high edge
      // ========================================================================
    };

    // ==========================================================================
    /** @class EqualToList
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-06
     */
    template <typename Signature>
    class EqualToList;

    template <typename... TYPE>
    class EqualToList<double( TYPE... )> : public Functor<bool( TYPE... )> {
    public:
      // ========================================================================
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      EqualToList( FunctorFromFunctor<double( TYPE... )> fun, std::vector<double> vct )
          : LoKi::AuxFunBase( std::tie( fun, vct ) ), m_fun( std::move( fun ) ), m_vct( std::move( vct ) ) {
        std::sort( m_vct.begin(), m_vct.end() );
        auto ilast = std::unique( m_vct.begin(), m_vct.end() );
        m_vct.erase( ilast, m_vct.end() );
      }
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      EqualToList( FunctorFromFunctor<double( TYPE... )> fun, const std::vector<int>& vct )
          : EqualToList( std::move( fun ), vct.begin(), vct.end() ) {}
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      EqualToList( FunctorFromFunctor<double( TYPE... )> fun, const std::vector<unsigned int>& vct )
          : EqualToList( std::move( fun ), vct.begin(), vct.end() ) {}
      /** constructor from the function and range of values
       *  @param fun the function
       *  @param first start of range
       *  @param last end of range
       */
      template <typename Iterator,
                LOKI_REQUIRES( std::is_base_of_v<std::input_iterator_tag,
                                                 typename std::iterator_traits<Iterator>::iterator_category> )>
      EqualToList( FunctorFromFunctor<double( TYPE... )> fun, Iterator first, Iterator last )
          : EqualToList( std::move( fun ), std::vector<double>{first, last} ) {}
      // ========================================================================
      /// MANDATORY: clone method ("virtual construcor")
      EqualToList* clone() const override { return new EqualToList( *this ); }
      /// MANDATORY: the only one essential method :
      bool operator()( details::Param_t<TYPE>... a ) const override { return equal_to( a... ); }
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " (" << this->func() << "==" << Gaudi::Utils::toString( m_vct ) << ") ";
      }
      // ========================================================================
      inline bool equal_to( details::Param_t<TYPE>... a ) const {
        if ( m_vct.empty() ) { return false; }
        //
        const double r = m_fun.fun( a... );
        //
        LHCb::Math::Equal_To<double> cmp;
        return std::any_of( m_vct.begin(), m_vct.end(), [&]( double item ) { return cmp( item, r ); } );
      }
      // ========================================================================
    public:
      // ========================================================================
      inline bool not_equal_to( details::Param_t<TYPE>... a ) const { return !this->equal_to( a... ); }
      // ========================================================================
    public:
      // ========================================================================
      const auto& func() const { return m_fun.func(); }
      /// get the vector
      const std::vector<double>& vect() const { return m_vct; }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor
      /// the list
      std::vector<double> m_vct; //    the list
      // ========================================================================
    };

    // ==========================================================================
    /** @class NotEqualToList
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-06
     */
    template <typename Signature>
    class NotEqualToList;
    template <typename... TYPE>
    class NotEqualToList<double( TYPE... )> final : public EqualToList<double( TYPE... )> {
    public:
      // ========================================================================
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      NotEqualToList( FunctorFromFunctor<double( TYPE... )> fun, std::vector<double> vct )
          : LoKi::AuxFunBase( std::tie( fun, vct ) )
          , EqualToList<double( TYPE... )>( std::move( fun ), std::move( vct ) ) {}
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      NotEqualToList( FunctorFromFunctor<double( TYPE... )> fun, const std::vector<int>& vct )
          : LoKi::AuxFunBase( std::tie( fun, vct ) ), EqualToList<double( TYPE... )>( std::move( fun ), vct ) {}
      /** constructor from the function and the value
       *  @param fun the function
       *  @param vct the vector of values
       */
      NotEqualToList( FunctorFromFunctor<double( TYPE... )> fun, const std::vector<unsigned int>& vct )
          : LoKi::AuxFunBase( std::tie( fun, vct ) ), EqualToList<double( TYPE... )>( std::move( fun ), vct ) {}
      /** constructor from the function and the value
       *  @param fun the function
       *  @param first start of range
       *  @param last end of range
       */
      template <typename Iterator,
                LOKI_REQUIRES( std::is_base_of_v<std::input_iterator_tag,
                                                 typename std::iterator_traits<Iterator>::iterator_category> )>
      NotEqualToList( FunctorFromFunctor<double( TYPE... )> fun, Iterator first, Iterator last )
          : EqualToList<double( TYPE... )>( std::move( fun ), first, last ) {}
      // ========================================================================
      /// MANDATORY: clone method ("virtual construcor")
      NotEqualToList* clone() const override { return new NotEqualToList( *this ); }
      /// MANDATORY: the only one essential method :
      bool operator()( details::Param_t<TYPE>... a ) const override { return this->not_equal_to( a... ); }
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " (" << this->func() << "!=" << Gaudi::Utils::toString( this->vect() ) << ") ";
      }
      // ========================================================================
    };
    // ==========================================================================
    /** @class XScaler
     *  Simple scaler for predicates
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-06
     */
    template <typename Signature>
    class XScaler;
    template <typename... TYPE>
    class XScaler<bool( TYPE... )> final : public Functor<bool( TYPE... )> {
    public:
      // ========================================================================
      /** constructor from predicate and scale
       *  @param cut the predicate
       *  @param scale the scaler
       */
      XScaler( FunctorFromFunctor<bool( TYPE... )> cut, FunctorFromFunctor<bool()> scale )
          : LoKi::AuxFunBase( std::tie( cut, scale ) ), m_cut( std::move( cut ) ), m_scaler( std::move( scale ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      XScaler* clone() const override { return new XScaler( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( details::Param_t<TYPE>... a ) const override { return m_cut.fun( a... ) && m_scaler.fun(); }
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " scale(" << m_cut << "," << m_scaler << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the predicate
      FunctorFromFunctor<bool( TYPE... )> m_cut; // the predicate
      /// the scaler
      FunctorFromFunctor<bool()> m_scaler; // the scaler
      // ========================================================================
    };

    // ==========================================================================
    /** @class Modulo
     *
     *  The helper function to implement "modulo"-operation
     *  of the function
     *
     *  http://en.wikipedia.org/wiki/Modulo_operation
     *
     *  @author Vanya Belyaev Ivan.Belyaev@cern.ch
     *  @date   2011-03-30
     */

    template <typename Signature>
    class Modulo;
    template <typename... TYPE>
    class Modulo<double( TYPE... )> final : public Functor<double( TYPE... )> {
    public:
      // ========================================================================
      /// constructor from the functor
      Modulo( FunctorFromFunctor<double( TYPE... )> fun, const unsigned int divisor )
          : LoKi::AuxFunBase( std::tie( fun, divisor ) ), m_fun( std::move( fun ) ), m_divisor( divisor ) {}
      /// clone method (mandatory)
      Modulo* clone() const override { return new Modulo( *this ); }
      /// the only one essential method ("function")
      double operator()( details::Param_t<TYPE>... a ) const override {
        return LHCb::Math::round( m_fun.fun( a... ) ) % m_divisor;
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " (" << m_fun << " % " << m_divisor << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the divident
      FunctorFromFunctor<double( TYPE... )> m_fun; // the divident
      /// the divisor
      const unsigned int m_divisor; // the divisor
      // ========================================================================
    };

    // ==========================================================================
    /** @class Round
     *  get the proper rounding for the floating value
     *  @see LHCb::Math::round
     *  The actual rounding policy is defined by function LHCb::Math::round
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-04-02
     */

    namespace Traits {
      struct Round {
        template <typename F, typename... Args>
        static double unaryOp( const F& f, const Args&... args ) {
          return LHCb::Math::round( f( args... ) );
        }
        template <typename F>
        static std::ostream& fillStream( std::ostream& os, const F& f ) {
          return os << " round(" << f << ") ";
        }
      };
    } // namespace Traits
    template <typename TYPE>
    using Round = details::UnaryOp<Traits::Round, LoKi::details::sig_t<TYPE, double>>;
    // ==========================================================================
    /** @class JBit
     *  get the jth bit of value.
     *  The action :
     *   - 1. f -> round ( f )
     *   - 2. f -> abs   ( f )
     *   - 3. bit ( f , j )
     *  @see Gaudi::Math::bit
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-04-02
     */
    template <typename Signature>
    class JBit;
    template <typename... TYPE>
    class JBit<bool( TYPE... )> final : public Functor<bool( TYPE... )> {
    public:
      // ========================================================================
      /// constructor from the functor
      JBit( FunctorFromFunctor<double( TYPE... )> fun, const unsigned short j )
          : LoKi::AuxFunBase( std::tie( fun, j ) ), m_fun( std::move( fun ) ), m_j( j ) {
        //
        static_assert( boost::integer_traits<unsigned long>::is_specialized &&
                           boost::integer_traits<unsigned long>::is_integral &&
                           !boost::integer_traits<unsigned long>::is_signed,
                       "must have a unsigned integral type" );
        //
        this->Assert( j < (unsigned long)boost::integer_traits<unsigned long>::digits, "Invalid bit index" );
        //
      }
      /// clone method (mandatory)
      JBit* clone() const override { return new JBit( *this ); }
      /// the only one essential method ("function")
      bool operator()( details::Param_t<TYPE>... a ) const override {
        const unsigned long _ulv = ::labs( LHCb::Math::round( this->m_fun.fun( a... ) ) );
        //
        return Gaudi::Math::bit( _ulv, this->m_j );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " jbit(" << this->m_fun << "," << this->m_j << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor
      /// the index
      unsigned short m_j; // the index
      // ========================================================================
    };

    // ==========================================================================
    /** @class JBits
     *  get the content between j1 and j2  bit of value.
     *  The action :
     *   - 1. f -> round ( f )
     *   - 2. f -> abs   ( f )
     *   - 3. Gaudi::Math::bits ( f , j1, j2  )
     *  @see Gaudi::Math::bits
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-04-02
     */
    template <typename Signature>
    class JBits;
    template <typename... TYPE>
    class JBits<double( TYPE... )> final : public Functor<double( TYPE... )> {
    public:
      // ========================================================================
      /// constructor from the functor
      JBits( FunctorFromFunctor<double( TYPE... )> fun, const unsigned short j1, const unsigned short j2 )
          : LoKi::AuxFunBase( std::tie( fun, j1, j2 ) ), m_fun( std::move( fun ) ), m_j1( j1 ), m_j2( j2 ) {
        //
        static_assert( boost::integer_traits<unsigned long>::is_specialized &&
                           boost::integer_traits<unsigned long>::is_integral &&
                           !boost::integer_traits<unsigned long>::is_signed,
                       "must have a unsigned integral type" );
        //
        this->Assert( j1 < (unsigned long)boost::integer_traits<unsigned long>::digits, "Invalid bit index-1" );
        this->Assert( j2 <= (unsigned long)boost::integer_traits<unsigned long>::digits, "Invalid bit index-2" );
        this->Assert( j1 < j2, "Invalid bit indices" );
        //
      }
      /// clone method (mandatory)
      JBits* clone() const override { return new JBits( *this ); }
      /// the only one essential method ("function")
      double operator()( details::Param_t<TYPE>... a ) const override {
        const unsigned long _ulv = ::labs( LHCb::Math::round( this->m_fun.fun( a... ) ) );
        //
        return Gaudi::Math::bits( _ulv, this->m_j1, this->m_j2 );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " jbits(" << this->m_fun << "," << this->m_j1 << "," << this->m_j2 << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor
      /// the index1
      unsigned short m_j1; // the index
      /// the index2
      unsigned short m_j2; // the index
      // ========================================================================
    };

    // ==========================================================================
    /** @class JDigit
     *  get the jth decimal digit of value.
     *  The action :
     *   - 1. f -> round ( f )
     *   - 2. f -> abs   ( f )
     *   - 3. digit ( f , j )
     *  @see Gaudi::Math::digit
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2017-02-16
     */
    template <typename Signature>
    class JDigit;
    template <typename... TYPE>
    class JDigit<double( TYPE... )> final : public Functor<double( TYPE... )> {
    public:
      // ========================================================================
      /// constructor from the functor
      JDigit( FunctorFromFunctor<double( TYPE... )> fun, const unsigned short j )
          : LoKi::AuxFunBase( std::tie( fun, j ) ), m_fun( std::move( fun ) ), m_j( j ) {
        //
        static_assert( boost::integer_traits<unsigned long>::is_specialized &&
                           boost::integer_traits<unsigned long>::is_integral &&
                           !boost::integer_traits<unsigned long>::is_signed,
                       "must have a unsigned integral type" );
        //
        this->Assert( j < (unsigned long)boost::integer_traits<unsigned long>::digits10, "Invalid digit index" );
        //
      }
      /// clone method (mandatory)
      JDigit* clone() const override { return new JDigit( *this ); }
      /// the only one essential method ("function")
      double operator()( details::Param_t<TYPE>... a ) const override {
        const unsigned long _ulv = ::labs( LHCb::Math::round( this->m_fun.fun( a... ) ) );
        //
        return Gaudi::Math::digit( _ulv, this->m_j );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " jdigit(" << this->m_fun << "," << this->m_j << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor
      /// the index
      unsigned short m_j; // the index
      // ========================================================================
    };

    // ==========================================================================
    /** @class JDigits
     *  get the content between j1 and j2 decimal digits of value.
     *  The action :
     *   - 1. f -> round ( f )
     *   - 2. f -> abs   ( f )
     *   - 3. Gaudi::Math::digits ( f , j1, j2  )
     *  @see Gaudi::Math::digits
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-04-02
     */
    template <typename Signature>
    class JDigits;
    template <typename... TYPE>
    class JDigits<double( TYPE... )> final : public Functor<double( TYPE... )> {
    public:
      // ========================================================================
      /// constructor from the functor
      JDigits( FunctorFromFunctor<double( TYPE... )> fun, const unsigned short j1, const unsigned short j2 )
          : LoKi::AuxFunBase( std::tie( fun, j1, j2 ) ), m_fun( std::move( fun ) ), m_j1( j1 ), m_j2( j2 ) {
        //
        static_assert( boost::integer_traits<unsigned long>::is_specialized &&
                           boost::integer_traits<unsigned long>::is_integral &&
                           !boost::integer_traits<unsigned long>::is_signed,
                       "must have a unsigned integral type" );
        //
        this->Assert( j1 < (unsigned long)boost::integer_traits<unsigned long>::digits10, "Invalid digit index-1" );
        this->Assert( j2 <= (unsigned long)boost::integer_traits<unsigned long>::digits10, "Invalid digit index-2" );
        this->Assert( j1 < j2, "Invalid digit indices" );
        //
      }
      /// clone method (mandatory)
      JDigits* clone() const override { return new JDigits( *this ); }
      /// the only one essential method ("function")
      double operator()( details::Param_t<TYPE>... a ) const override {
        const unsigned long _ulv = ::labs( LHCb::Math::round( this->m_fun.fun( a... ) ) );
        //
        return Gaudi::Math::digits( _ulv, this->m_j1, this->m_j2 );
      }
      /// the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " jdigits(" << this->m_fun << "," << this->m_j1 << "," << this->m_j2 << ") ";
      }
      // ========================================================================
    private:
      // ========================================================================
      /// the functor itself
      FunctorFromFunctor<double( TYPE... )> m_fun; // the functor
      /// the index1
      unsigned short m_j1; // the index
      /// the index2
      unsigned short m_j2; // the index
      // ========================================================================
    };
  } // namespace V2

  // ==========================================================================
  // Temporarily for backwards compatibility / adiabatic transition ...
  // ==========================================================================
  namespace details {
    template <template <typename> class F, typename... Ts>
    struct import_helper;
    template <template <typename> class F, typename... Arg, typename Res>
    struct import_helper<F, Res( Arg... )> {
      using type = F<Res( Arg... )>;
    };
    template <template <typename> class F, typename... Arg, typename Res, typename Default>
    struct import_helper<F, Res( Arg... ), Default> {
      using type = F<Res( Arg... )>;
    };
    template <template <typename> class F, typename T1, typename T2, typename Default>
    struct import_helper<F, T1, T2, Default> {
      using type = F<sig_t<T1, T2>>;
    };
    template <template <typename> class F, typename T1, typename T2>
    struct import_helper<F, T1, T2> {
      using type = F<sig_t<T1, T2>>;
    };

    template <template <typename> class F, typename... Ts>
    using import_t = typename import_helper<F, Ts...>::type;
  } // namespace details

  template <typename T, typename T2 = double>
  using SimpleSwitch = V2::SimpleSwitch<details::sig_t<T, T2>>;
  template <typename T, typename T2 = double>
  using Switch = V2::Switch<details::sig_t<T, T2>>;
  using V2::ComposeFunction;
  using V2::ComposeFunction2;

  using V2::And;
  using V2::Negate;
  using V2::Not;
  using V2::Or;
  template <typename... Ts>
  using Less = details::import_t<V2::Less, Ts...>;
  using V2::Divide;
  using V2::Equal;
  using V2::LessOrEqual;
  using V2::Minus;
  using V2::Multiply;
  using V2::NotEqual;
  using V2::Plus;
  template <typename... Ts>
  using Min = details::import_t<V2::Min, Ts..., double>;
  template <typename... Ts>
  using Max = details::import_t<V2::Max, Ts..., double>;

  template <typename... Ts>
  using EqualToValue = details::import_t<V2::EqualToValue, Ts..., double>;
  template <typename... Ts>
  using NotEqualToValue = details::import_t<V2::NotEqualToValue, Ts...>;

  template <typename... Ts>
  using LessThanValue = details::import_t<V2::LessThanValue, Ts...>;
  template <typename... Ts>
  using LessOrEqualValue = details::import_t<V2::LessOrEqualValue, Ts...>;
  template <typename... Ts>
  using GreaterThanValue = details::import_t<V2::GreaterThanValue, Ts...>;
  using V2::Divide1;
  using V2::Divide2;
  using V2::GreaterOrEqualValue;
  using V2::Minus1;
  using V2::Minus2;
  using V2::MultiplyByValue;
  using V2::SumByValue;

  template <typename T>
  using InRange = V2::InRange<details::sig_t<T, double>>;
  template <typename T>
  using InRange2 = V2::InRange2<details::sig_t<T, double>>;
  template <typename T>
  using EqualToList = V2::EqualToList<details::sig_t<T, double>>;
  template <typename T>
  using NotEqualToList = V2::NotEqualToList<details::sig_t<T, double>>;
  template <typename T>
  using XScaler = V2::XScaler<details::sig_t<T, bool>>;
  template <typename T>
  using Modulo = V2::Modulo<details::sig_t<T, double>>;
  using V2::Round;
  template <typename T>
  using JBit = V2::JBit<details::sig_t<T, bool>>;
  template <typename T>
  using JBits = V2::JBits<details::sig_t<T, double>>;
  template <typename T>
  using JDigit = V2::JDigit<details::sig_t<T, double>>;
  template <typename T>
  using JDigits = V2::JDigits<details::sig_t<T, double>>;
  // ==========================================================================
  // OPTIONAL: the nice printout
  // ==========================================================================
  template <typename TYPE, typename TYPE2>
  std::ostream& Identity<TYPE, TYPE2>::fillStream( std::ostream& s ) const {
    return s << "I";
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // _GEN_LOKI_PRIMITIVES
