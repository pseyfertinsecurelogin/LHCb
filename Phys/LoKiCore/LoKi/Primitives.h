// ============================================================================
#ifndef LOKI_PRIMITIVES_H
#define LOKI_PRIMITIVES_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <functional>
#include <type_traits>
#include <iterator>
#include "GaudiKernel/SerializeSTL.h"
// ============================================================================
// LHCb
// ============================================================================
#include "LHCbMath/LHCbMath.h"
#include "LHCbMath/Bit.h"
#include "LHCbMath/Digit.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/BiFunctions.h"
#include "LoKi/Reference.h"
#include "LoKi/Field.h"
#include "LoKi/valid.h"
#include "LoKi/same.h"
#include "LoKi/apply.h"
#include "LoKi/Operations.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/integer_traits.hpp"
// ============================================================================
// Original Primitives first
// ============================================================================
#define _GEN_LOKI_PRIMITIVES 1
#define typedef_void_TYPE
#define argument_a_unless_void argument a
#define typename_v typename
#define a_unless_void a
#define class_TYPE_unless_void class TYPE,
#endif
#ifdef _GEN_LOKI_PRIMITIVES
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
namespace LoKi
{
#ifndef _GEN_LOKI_VOIDPRIMITIVES
  // ==========================================================================
  /** @class Functors
   *  helper class to keep N functors of the same signature
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-01
   */
  template <typename TYPE,typename TYPE2, size_t N>
  class FunctorArray final
  {
  public:
    // ========================================================================
    template <typename... Fs>
    FunctorArray( Fs&&... fs ) : m_funs{ std::forward<Fs>(fs)... }
    {}
    // ========================================================================
    /// evaluate the i-th functor
    template <size_t i, typename... Args> TYPE2 fun( Args&&... args ) const
    { return std::get<i>(m_funs).fun( std::forward<Args>(args)... ) ; }

    template <typename... Args> TYPE2 fun1( Args&&... args ) const
    { return fun<0>( std::forward<Args>(args)... ) ; }
    template <typename... Args> TYPE2 fun2( Args&&... args ) const
    { return fun<1>( std::forward<Args>(args)... ) ; }
    // ========================================================================
    /// get the i-th functor
    template <size_t i> const auto& func () const { return std::get<i>(m_funs).func () ; }
    /// get the first functor
    const auto& func1 ()           const { return func<0>(); }
    /// get the second functor
    const auto& func2 ()           const { return func<1>(); }
    // ========================================================================
  private:
    // ========================================================================
    /// the functors
    std::array< LoKi::FunctorFromFunctor<TYPE,TYPE2>, N > m_funs ; // the functors
    // ========================================================================
  } ;

  template <typename TYPE, typename TYPE2>
  using TwoFunctors = FunctorArray<TYPE,TYPE2,2>;

#endif
  namespace details {
      // =======================================================================
      /** @class UnaryOp
       *  The helper function to implement Unary operation of one function
       */
#ifndef _GEN_LOKI_VOIDPRIMITIVES
      template<typename TYPE, typename TYPE2, typename Result, typename Traits_>
      class UnaryOp final : public LoKi::Functor<TYPE,Result>
#else
      template<typename TYPE2, typename Result, typename Traits_>
      class UnaryOp<void,TYPE2,Result,Traits_> final : public LoKi::Functor<void,Result>
#endif
      {
        // ========================================================================
        typedef_void_TYPE
        /// argument type
        typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
      public:
        // =====================================================================
        /// constructor
        UnaryOp( LoKi::FunctorFromFunctor<TYPE,TYPE2> f )
          : LoKi::AuxFunBase{ std::tie ( f  ) }
          , m_fun{ std::move(f) }
        { }
        /// clone method (mandatory)
        UnaryOp* clone() const override { return new UnaryOp( *this ); }
        /// the only one essential method ("function")
        Result operator()( argument_a_unless_void ) const override
        {
#ifndef _GEN_LOKI_VOIDPRIMITIVES
          return Traits_::unaryOp( this->fun(),  a_unless_void );
#else
          return Traits_::unaryOp( this->fun() );
#endif
        }
        /// the basic printout method
        std::ostream& fillStream( std::ostream& s ) const override
        { return s << " (" <<  Traits_::name() << this->fun() << ") " ; }
        // =====================================================================
      private:
        // =====================================================================
        /// the functor type
        /// get the first functor
        const auto& fun() const { return m_fun; }
        // =====================================================================
        /// the storage of the functor
        LoKi::FunctorFromFunctor<TYPE,TYPE2>  m_fun ; // the storage of the functor
        // =====================================================================
      };

#ifndef _GEN_LOKI_VOIDPRIMITIVES
      template <typename Op>
      struct SimpleUnary {
           template <typename F,typename... Args>
           static auto unaryOp(const F& f, const Args&... args)
           -> decltype(auto)
           { return Op{}( f(args...)); }
      };
#endif
  }

// 2nd pass only -- after specialization of UnaryOp is defined
#ifdef _GEN_LOKI_VOIDPRIMITIVES
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
       struct Not : details::SimpleUnary<std::logical_not<>> {
           static constexpr const char* name() { return "~"; }
       };
   }
   template<class TYPE, class TYPE2=bool>
   using Not = details::UnaryOp<TYPE,TYPE2,bool,Traits::Not>;

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
       struct Negate : details::SimpleUnary<std::negate<>> {
           static constexpr const char* name() { return "-"; }
       };
   }
   template<class TYPE, class TYPE2=double>
   using Negate = details::UnaryOp<TYPE,TYPE2,TYPE2,Traits::Negate>;
#endif

   namespace details {
      // =======================================================================
      /** @class BinaryOp
       *  The helper function to implement Binary operation of two functions
       */
#ifndef _GEN_LOKI_VOIDPRIMITIVES
      template<typename TYPE, typename TYPE2, typename Result, typename Traits_>
      class BinaryOp final : public LoKi::Functor<TYPE,Result>
#else
      template<typename TYPE2, typename Result, typename Traits_>
      class BinaryOp<void,TYPE2,Result,Traits_> final : public LoKi::Functor<void,Result>
#endif
      {
      private:
        // =====================================================================
        typedef_void_TYPE
        /// the constant type
        typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
        /// argument type
        typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
        // =====================================================================
      public:
        // =====================================================================
        /// constructor from two functors
        BinaryOp ( LoKi::FunctorFromFunctor<TYPE,TYPE2> f1,
                   LoKi::FunctorFromFunctor<TYPE,TYPE2> f2 )
          : LoKi::AuxFunBase{ std::tie ( f1 , f2 ) }
          , m_two { std::move(f1) , std::move(f2) }
        { }
        /// clone method (mandatory)
        BinaryOp* clone() const override { return new BinaryOp( *this ); }
        /// the only one essential method ("function")
        Result operator() ( argument_a_unless_void ) const override
        {
#ifndef _GEN_LOKI_VOIDPRIMITIVES
          return Traits_::binaryOp( m_two.func1(), m_two.func2(),  a_unless_void );
#else
          return Traits_::binaryOp( m_two.func1(), m_two.func2() );
#endif
        }
        /// the basic printout method
        std::ostream& fillStream( std::ostream& s ) const override
        { return s << " (" << m_two.func1() << Traits_::name() << m_two.func2() << ") " ; }
        // =====================================================================
      private:
        // =====================================================================
        /// the storage of two functors
        LoKi::TwoFunctors<TYPE,TYPE2> m_two ;    // the storage of two functors
        // =====================================================================
      };

#ifndef _GEN_LOKI_VOIDPRIMITIVES
      template <typename Op>
      struct SimpleBinary {
           template <typename F1, typename F2, typename... Args>
           static auto binaryOp(const F1& f1, const F2& f2, const Args&... args)
           -> decltype(auto)
           { return Op{}( f1(args...) , f2(args...)); }
      };
#endif
   }

// 2nd pass only -- after specialization of BinaryOp is defined
#ifdef _GEN_LOKI_VOIDPRIMITIVES
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
       struct And : details::SimpleBinary< std::logical_and<> > {
           static constexpr const char* name() { return "&&"; }
       };
   }

   template<class TYPE, class TYPE2=bool>
   using And = details::BinaryOp<TYPE,TYPE2,bool,Traits::And>;

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
       struct Or : details::SimpleBinary< std::logical_or<> > {
           static constexpr const char* name() { return "||"; }
       };
   }
   template<class TYPE, class TYPE2=double>
   using Or = details::BinaryOp<TYPE,TYPE2,bool,Traits::Or>;

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
       struct Less : details::SimpleBinary< std::less<> >{
           static constexpr const char* name() { return "<"; }
       };
   }
   template<class TYPE, class TYPE2=double>
   using Less = details::BinaryOp<TYPE,TYPE2,bool,Traits::Less>;

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
       struct Equal : details::SimpleBinary< LHCb::Math::Equal_To<TYPE2> > {
            static constexpr const char* name() { return "=="; }
       };
   }
   template<class TYPE, class TYPE2=double>
   using Equal = details::BinaryOp<TYPE,TYPE2,bool,Traits::Equal<TYPE2>>;

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
       struct LessOrEqual : details::SimpleBinary< std::less_equal<> > {
            static constexpr const char* name() { return "<="; }
       };
   }
   template<class TYPE, class TYPE2=double>
   using LessOrEqual = details::BinaryOp<TYPE,TYPE2,bool,Traits::LessOrEqual>;

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
           static constexpr const char* name() { return "!="; }
           template <typename F1, typename F2, typename... Args>
           static bool binaryOp(const F1& f1, const F2& f2, const Args&... args)
           { LHCb::Math::Equal_To<TYPE2> op{}; return !op(f1(args...),f2(args...)); }
       };
   }

   template<class TYPE, class TYPE2=double>
   using NotEqual = details::BinaryOp<TYPE,TYPE2,bool,Traits::NotEqual<TYPE2>>;

#endif


   namespace details {
      // ==========================================================================
      /** @class Combination
       *  The helper function to implement combination of 2 function
       */
#ifndef _GEN_LOKI_VOIDPRIMITIVES
          template<class TYPE, class TYPE2, typename Traits_>
          class Combination final : public LoKi::Functor<TYPE,TYPE2>
#else
          template<class TYPE2,typename Traits_>
          class Combination<void,TYPE2,Traits_> final : public LoKi::Functor<void,TYPE2>
#endif
         {
         private:
            // ========================================================================
            typedef_void_TYPE
            /// argument type
            typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
            // ========================================================================
          public:
            // ========================================================================
            /// constructor from two functors
            Combination ( LoKi::FunctorFromFunctor<TYPE,TYPE2> f1,
                          LoKi::FunctorFromFunctor<TYPE,TYPE2> f2 )
              : LoKi::AuxFunBase ( std::tie ( f1 , f2 ) )
              , m_two ( std::move(f1) , std::move(f2) )
            {}
            /// clone method (mandatory)
            Combination* clone() const override { return new Combination ( *this ) ; }
            /// the only one essential method ("function")
            TYPE2 operator() ( argument_a_unless_void ) const override
            {
              typename Traits_::BinaryOp binOp{} ;
              return binOp( m_two.func1()( a_unless_void ) , m_two.func2()( a_unless_void ) ) ;
            }
            /// the basic printout method
            std::ostream& fillStream( std::ostream& s ) const override
            { return s << " (" << m_two.func1() << Traits_::name() << m_two.func2() << ") " ; }
            // ========================================================================
          private:
            // ========================================================================
            /// the functor type
            typedef typename LoKi::Functor<TYPE,TYPE2>                        functor ;
            // ========================================================================
            /// the storage of two functors
            LoKi::TwoFunctors<TYPE,TYPE2> m_two ;        // the storage of two functors
            // ========================================================================

        };

   }

#ifdef _GEN_LOKI_VOIDPRIMITIVES
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
       struct Plus {
            using BinaryOp = std::plus<>;
            static constexpr const char* name() { return "+"; }
       };
   }

  template<class TYPE, class TYPE2=double>
  using Plus =  details::Combination<TYPE,TYPE2,Traits::Plus>;

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
       struct Minus {
            using BinaryOp = std::minus<>;
            static constexpr const char* name() { return "-"; }
       };
   }

  template<class TYPE, class TYPE2=double>
  using Minus = details::Combination<TYPE,TYPE2,Traits::Minus>;

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
       struct Divide {
            using BinaryOp = std::divides<>;
            static constexpr const char* name() { return "/"; }
       };
   }

  template<class TYPE, class TYPE2=double>
  using Divide =  details::Combination<TYPE,TYPE2,Traits::Divide>;

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
       struct Multiply {
            using BinaryOp = std::multiplies<>;
            static constexpr const char* name() { return "*"; }
       };
   }

  template<class TYPE, class TYPE2=double>
  using Multiply =  details::Combination<TYPE,TYPE2,Traits::Multiply>;

#endif
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<class TYPE2>
  class Min<void,TYPE2> : public LoKi::Functor<void,TYPE2>
#else
  template<class TYPE, class TYPE2=double>
  class Min : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// the constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from 2 functions
     *  @param fun1 the first  function
     *  @param fun2 the second function
     */
    Min ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1 ,
          LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2 )
      : LoKi::AuxFunBase ( std::tie ( fun1 , fun2 ) )
      , m_two { std::move(fun1) , std::move(fun2) }
    {}
    /// constructor from the function and constant
    Min ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1 , T2 fun2 )
      : LoKi::AuxFunBase ( std::tie ( fun1 , fun2 ) )
      , m_two ( std::move(fun1) , LoKi::Constant<TYPE,TYPE2> ( std::move(fun2) )  )
    {}
    /// constructor from the function and constant
    Min ( T2 fun1 , LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2 )
      : LoKi::AuxFunBase ( std::tie ( fun1 , fun2 ) )
      , m_two ( LoKi::Constant<TYPE,TYPE2> ( std::move(fun1) ) , std::move(fun2) )
    {}
    /** constructor from >=3 functions
     *  @param fun1 the first  function
     *  @param fun2 the second function
     *  @param funs the remaining functions
     */
    template <typename... Fs>
    Min ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1 ,
          LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2 ,
          Fs&&... funs )
      : Min( Min{ std::move(fun1) , std::move(fun2) },
             std::forward<Fs>(funs)... )
    {}
    /// clone method (mandatory)
    Min* clone() const override { return new Min ( *this ) ; }
    /// the only one essential method ("function")
    TYPE2 operator() ( argument_a_unless_void ) const override
    {
      return  std::min ( m_two.fun1 ( a_unless_void ) ,
                         m_two.fun2 ( a_unless_void ) , std::less<>() ) ;
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " min(" << m_two.func1() << "," << m_two.func2() << ") " ; }
    // ========================================================================
  protected:
    // ========================================================================
    /// the storage of two functors
    LoKi::TwoFunctors<TYPE,TYPE2> m_two ; // the storage of two functors
    // ========================================================================
  };
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<class TYPE2>
  class Max<void,TYPE2> : public LoKi::Min<void,TYPE2>
#else
  template<class TYPE, class TYPE2=double>
  class Max : public LoKi::Min<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// the constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from 2 functions
     *  @param fun1 the first  function
     *  @param fun2 the second function
     */
    using Min<TYPE,TYPE2>::Min;

    /// MANDATORY: clone method ("virtual constructor")
    Max* clone() const override { return new Max ( *this ) ; }
    /// MANDATORY: the only one essential method ("function")
    TYPE2 operator() ( argument_a_unless_void ) const override
    {
      return std::max ( this->m_two.fun1 ( a_unless_void ) ,
                        this->m_two.fun2 ( a_unless_void ) , std::less<>() ) ;
    }
    /// OPTIONAL: the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " max(" << this->m_two.func1() << "," << this->m_two.func2() << ") " ; }
    // ========================================================================
  };
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class SimpleSwitch<void,TYPE2> : public LoKi::Functor<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class SimpleSwitch : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// the constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
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
    SimpleSwitch( LoKi::FunctorFromFunctor <TYPE,bool> cut,
                  T2 val1 , T2 val2 )
      : LoKi::AuxFunBase ( std::tie ( cut , val1 , val2 ) )
      , m_cut  ( std::move(cut)  )
      , m_val1 ( val1 )
      , m_val2 ( val2 )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    SimpleSwitch* clone() const override { return new SimpleSwitch ( *this ) ; }
    /// MANDATORY: the only one essential method:
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return m_cut( a_unless_void ) ? m_val1 : m_val2 ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " switch("
               << this->m_cut  << ","
               << this->m_val1 << ","
               << this->m_val2 << ") " ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the condition
    LoKi::FunctorFromFunctor <TYPE,bool> m_cut  ; // the condiftion
    /// the first value
    TYPE2                                m_val1 ; // the first value
    /// the second value
    TYPE2                                m_val2 ; // the second value
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<class TYPE2>
  class Switch<void,TYPE2> final : public LoKi::Functor<void,TYPE2>
#else
  template<class TYPE, class TYPE2=double>
  class Switch final : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// the constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
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
     *  @param fun1 the first  fuction
     *  @param fun2 the second function
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-11
     */
    Switch( LoKi::FunctorFromFunctor<TYPE,bool> cut,
            LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1 ,
            LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2 )
      : LoKi::AuxFunBase ( std::tie ( cut , fun1 , fun2 ) )
      , m_cut  ( std::move(cut)  )
      , m_two  ( std::move(fun1) , std::move(fun2) )
    {}
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
    Switch( LoKi::FunctorFromFunctor<TYPE,bool> cut  ,
            LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1 , T2 fun2 )
      : LoKi::AuxFunBase ( std::tie ( cut , fun1 , fun2 ) )
      , m_cut  ( std::move(cut) )
      , m_two  ( std::move(fun1) , LoKi::Constant<TYPE,TYPE2>( std::move(fun2) ) )
    {}
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
    Switch( LoKi::FunctorFromFunctor<TYPE,bool> cut, T2 fun1,
            LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2)
      : LoKi::AuxFunBase ( std::tie ( cut , fun1 , fun2 ) )
      , m_cut( std::move(cut) )
      , m_two( LoKi::Constant<TYPE,TYPE2>( fun1 ), std::move(fun2) )
    {}
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
     *  @param fun1 the first constant
     *  @param fun2 the second constant
     *
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-11
     */
    Switch( LoKi::FunctorFromFunctor<TYPE,bool> cut, T2 fun1, T2 fun2 )
      : LoKi::AuxFunBase ( std::tie ( cut , fun1 , fun2 ) )
      , m_cut  ( std::move(cut) )
      , m_two  ( LoKi::Constant<TYPE,TYPE2>( std::move(fun1) ) ,
                 LoKi::Constant<TYPE,TYPE2>( std::move(fun2) ) )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    Switch* clone() const override { return new Switch ( *this ) ; }
    /// MANDATORY: the only one essential method:
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return m_cut.fun( a_unless_void ) ? m_two.fun1 ( a_unless_void )
                                        : m_two.fun2 ( a_unless_void ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " switch("
               << this->m_cut          << ","
               << this->m_two.func1()  << ","
               << this->m_two.func2()  << ") "  ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the condition
    LoKi::FunctorFromFunctor<TYPE,bool> m_cut  ;               // the condition
    /// the actual storage of two functors
    LoKi::TwoFunctors<TYPE,TYPE2>       m_two ;      // the storage of functors
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class ComposeFunction<void,TYPE2> : public LoKi::Functor<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class ComposeFunction : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
  protected:
    // ========================================================================
    /// the actual type for the function
    typedef double (*Func) ( double ) ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    ComposeFunction ( Func                  func           ,
                      LoKi::FunctorFromFunctor<TYPE,TYPE2> fun,
                      const std::string&    desc = "'fun'" )
      : m_fun  ( std::move(fun) )
      , m_func ( func )
      , m_desc ( desc )
    {}
    /// clone method (mandatory!)
    ComposeFunction*  clone () const override
    { return new ComposeFunction ( *this ) ; }
    /// the only one essential method ("function")
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return (*m_func) ( m_fun . fun ( a_unless_void ) ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " " << this->m_desc << "("  << this->m_fun << ") " ; };
    // to C++
    std::string   toCpp() const override
    { return "LoKi::" + this->m_desc + "("  + Gaudi::Utils::toCpp ( this->m_fun ) + ") " ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun  ; // the functor
    /// the function:
    Func                                 m_func ; // the function:
    /// the function description:
    std::string                          m_desc ; // description
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class ComposeFunction2<void,TYPE2> : public LoKi::Functor<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class ComposeFunction2 : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    /// the actual type of the function
    typedef double (*Func) ( double , double )      ;
    /// constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    ComposeFunction2( Func func,
                      LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1,
                      LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2,
                      const std::string& desc = "'fun'" )
      : m_func ( func )
      , m_two  ( std::move(fun1) ,std::move(fun2) )
      , m_desc ( desc )
    {}
    /// constructor
    ComposeFunction2( Func func,
                      LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1,
                      T2 val2, const std::string& desc  = "'fun'" )
      : ComposeFunction2( func, std::move(fun1),
                          LoKi::Constant<TYPE,TYPE2>{std::move(val2)}, desc )
    {}
    /// constructor
    ComposeFunction2( Func func, T2 val1,
                      LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2,
                      const std::string& desc  = "'fun'" )
      : ComposeFunction2( func, LoKi::Constant<TYPE,TYPE2>{std::move(val1)},
                          std::move(fun2), desc )
    {}
    /// constructor
    ComposeFunction2( Func func, T2 val1, T2 val2, const std::string& desc  = "'fun'" )
      : ComposeFunction2( func, LoKi::Constant<TYPE,TYPE2>{std::move(val1)},
                          LoKi::Constant<TYPE,TYPE2>{std::move(val2)}, desc )
    {}
    /// clone method (mandatory!)
    ComposeFunction2*  clone() const override
    { return new ComposeFunction2( *this ); }
    /// the only one essential method ("function")
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return (*m_func) ( m_two. fun1 ( a_unless_void ) , m_two.fun2( a_unless_void ) ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " "
               << m_desc         << "("
               << m_two.func1 () << ","
               << m_two.func2 () << ") " ; }
    /// to C++
    std::string toCpp() const override
    { return "LoKi::" + this->m_desc + "("
        + Gaudi::Utils::toCpp ( this->m_two.func1 () ) + ", "
        + Gaudi::Utils::toCpp ( this->m_two.func2 () ) + ") " ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the function itself
    Func m_func        ; // the function itself
    /// the storage of functors
    LoKi::TwoFunctors<TYPE,TYPE2> m_two ; // the storage of functors
    /// the funtion descrition
    std::string m_desc ; // the funtion descrition
    // ========================================================================
  };
  // ==========================================================================
  /** @class Compose
   *  the general case of fun2(fun1) function:
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE1, class TYPE2, class TYPE3>
  class Compose<void,TYPE1,TYPE2,TYPE3> : public LoKi::Functor<void,TYPE2>
#else
  template <class TYPE,class TYPE1, class TYPE2, class TYPE3=TYPE1>
  class Compose : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// contructor
    Compose ( LoKi::FunctorFromFunctor<TYPE,TYPE1> fun1,
              LoKi::FunctorFromFunctor<TYPE3,TYPE2> fun2 )
      : LoKi::AuxFunBase ( std::tie ( fun1 , fun2 ) )
      , m_fun1 ( std::move(fun1) )
      , m_fun2 ( std::move(fun2) )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    Compose* clone() const override { return new Compose ( *this ) ; }
    /// the only one essential method ("function")
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return m_fun2(  m_fun1( a_unless_void ) ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " (" << this->m_fun1 << ">>" << this->m_fun2  << ") " ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the first functor
    LoKi::FunctorFromFunctor<TYPE,TYPE1>  m_fun1  ;        // the first functor
    /// the second functor
    LoKi::FunctorFromFunctor<TYPE3,TYPE2> m_fun2  ;       // the second functor
    // ========================================================================
  } ;
  // ==========================================================================
#ifndef _GEN_LOKI_VOIDPRIMITIVES
  // ==========================================================================
  /** @class Valid
   *  The trivial predicate to check the validity of argument.
   *  It is OK for any pointer-like types or for types with
   *  implemented implicit conversion to "bool"
   *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
   *  @date 2004-02-11
   */
  template <class TYPE>
  class Valid : public LoKi::Functor<TYPE,bool>
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor
    Valid() = default;
    /// MANDATORY: clone method ("virtual constructor")
    Valid* clone() const override { return new Valid( *this ) ; }
    /// MANDATORY: the only one essential method
    bool operator() ( argument_a_unless_void ) const override
    { return LoKi::valid ( a_unless_void ) ? true : false  ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " (Valid?)" ; }
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
  template <class TYPE>
  class TheSame : public LoKi::Functor<TYPE,bool>
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor form the value
    explicit TheSame ( TYPE value )
      : m_value ( std::move(value) )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    TheSame* clone() const override { return new TheSame( *this ) ; }
    /// MANDATORY: the only one essential method
    bool operator() ( argument object ) const override
    { return LoKi::same ( m_value , object ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " (SAME?) "; }
    // ========================================================================
  private:
    // ========================================================================
    // the predefined value
    TYPE m_value ;
    // ========================================================================
  };
#endif
  // ==========================================================================
  /** @class EqualToValue
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class EqualToValue<void,TYPE2> : public LoKi::Functor<void,bool>
#else
  template <class TYPE, class TYPE2=double>
  class EqualToValue : public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    EqualToValue( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun, T2 val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , m_fun ( std::move(fun) )
      , m_val ( std::move(val) )
    {}
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    EqualToValue( T2 val , LoKi::FunctorFromFunctor<TYPE,TYPE2> fun )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , m_fun ( std::move(fun) )
      , m_val ( val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    EqualToValue* clone() const override { return new EqualToValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return equal_to ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this->func ()
               << "==" << this->val  () << ") " ; }
    // ========================================================================
  public:
    // ========================================================================
    inline bool equal_to ( argument_a_unless_void ) const
    {
      // the comparator
      LHCb::Math::Equal_To<TYPE2> _cmp ;
      return _cmp ( this->m_fun.fun ( a_unless_void ) , this->m_val ) ;
    }
    // ========================================================================
    inline bool not_equal_to ( argument_a_unless_void ) const
    { return ! this->equal_to ( a_unless_void ) ; }
    // ========================================================================
    inline bool less    ( argument_a_unless_void ) const
    {
      // the comparator
      std::less<> _cmp ;
      return _cmp ( this->m_fun.fun ( a_unless_void ) , this->m_val ) ;
    }
    // ========================================================================
    inline bool greater ( argument_a_unless_void ) const
    {
      // the comparator
      std::less<> _cmp ;
      return _cmp ( this->m_val , this->m_fun.fun ( a_unless_void ) ) ;
    }
    // ========================================================================
    inline bool less_or_equal ( argument_a_unless_void ) const
    {
      // the comparator
      std::less<>            _cmp1 ;
      LHCb::Math::Equal_To<TYPE2> _cmp2 ;
      auto _r = this->m_fun.fun ( a_unless_void ) ;
      return _cmp1 ( _r , this->m_val ) || _cmp2 ( _r , this->m_val ) ;
    }
    // ========================================================================
    inline bool greater_or_equal ( argument_a_unless_void ) const
    {
      // the comparator
      std::less<>            _cmp1 ;
      LHCb::Math::Equal_To<TYPE2> _cmp2 ;
      auto _r = this->m_fun.fun ( a_unless_void ) ;
      return _cmp1 ( this->m_val , _r ) || _cmp2 ( _r , this->m_val ) ;
    }
    // ========================================================================
  public:
    // ========================================================================
    const auto& func () const { return m_fun.func() ; }
    const auto& val  () const { return m_val        ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ;                 // the functor
    /// the value
    TYPE2  m_val ;                                                 // the value
    // ========================================================================
  };
  // ==========================================================================
  /** @class NotEqualToValue
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class NotEqualToValue<void,TYPE2> : public LoKi::EqualToValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class NotEqualToValue : public LoKi::EqualToValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    /// the constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor for the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    NotEqualToValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun , T2 val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2>( std::move(fun) , val )
    {}
    /** constructor from the function and the value
     *  @param val the reference value
     *  @param fun the function
     *  @param eps the relative precision
     */
    NotEqualToValue
    ( T2 val , LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2>( val , std::move(fun) )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    NotEqualToValue* clone() const override { return new NotEqualToValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this->not_equal_to ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this -> func ()
               << "!=" << this -> val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class LessThanValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-19
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class LessThanValue<void,TYPE2> : public LoKi::EqualToValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class LessThanValue             : public LoKi::EqualToValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    LessThanValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase   ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2>( std::move(fun) , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    LessThanValue* clone() const override { return new LessThanValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this->less ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this->func ()
               << "<"  << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class LessOrEqualValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-19
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class LessOrEqualValue<void,TYPE2> : public LoKi::EqualToValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class LessOrEqualValue             : public LoKi::EqualToValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    LessOrEqualValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase    ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2> ( std::move(fun) , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    LessOrEqualValue* clone() const override { return new LessOrEqualValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this->less_or_equal ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("  << this->func ()
               << "<="  << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class GreaterThanValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-19
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class GreaterThanValue<void,TYPE2> : public LoKi::EqualToValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class GreaterThanValue             : public LoKi::EqualToValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    GreaterThanValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase   ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2>( std::move(fun) , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    GreaterThanValue* clone() const override { return new GreaterThanValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this->greater ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this->func ()
               << ">"  << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class GreaterOrEqualValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-19
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class GreaterOrEqualValue<void,TYPE2> : public LoKi::EqualToValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class GreaterOrEqualValue             : public LoKi::EqualToValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,bool>::argument argument  ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    GreaterOrEqualValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase   ( std::tie ( fun , val ) )
      , LoKi::EqualToValue<TYPE,TYPE2>( std::move(fun) , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    GreaterOrEqualValue* clone() const override { return new GreaterOrEqualValue(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this->greater_or_equal ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("  << this->func ()
               << ">="  << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class MultiplyByValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class MultiplyByValue<void,TYPE2> : public LoKi::Functor<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class MultiplyByValue             : public LoKi::Functor<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    MultiplyByValue
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , m_fun ( std::move(fun) )
      , m_val ( val )
    {}
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    MultiplyByValue
    ( T2                                val ,
      LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun )
      : LoKi::AuxFunBase ( std::tie ( val , fun ) )
      , m_fun ( std::move(fun) )
      , m_val ( val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    MultiplyByValue* clone() const override { return new MultiplyByValue(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->mult ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->func ()
               << "*"    << this->val  () << ") " ; }
    // ========================================================================
  public:
    // ========================================================================
    const LoKi::Functor<TYPE,TYPE2>& func () const { return m_fun.func() ; }
    const TYPE2&                     val  () const { return m_val        ; }
    // ========================================================================
  protected :
    // ========================================================================
    inline TYPE2 mult    ( argument_a_unless_void ) const
    { return ( this->m_fun.fun ( a_unless_void ) ) * ( this->m_val ) ; }
    // ========================================================================
    inline TYPE2 sum     ( argument_a_unless_void ) const
    { return ( this->m_fun.fun ( a_unless_void ) ) + ( this->m_val ) ; }
    // ========================================================================
    inline TYPE2 divide1 ( argument_a_unless_void ) const
    { return ( this->m_fun.fun ( a_unless_void ) ) / ( this->m_val ) ; }
    // ========================================================================
    inline TYPE2 divide2 ( argument_a_unless_void ) const
    { return ( this->m_val ) / ( this->m_fun.fun ( a_unless_void ) ) ; }
    // ========================================================================
    inline TYPE2 minus1  ( argument_a_unless_void ) const
    { return ( this->m_fun.fun ( a_unless_void ) ) - ( this->m_val ) ; }
    // ========================================================================
    inline TYPE2 minus2  ( argument_a_unless_void ) const
    { return ( this->m_val ) - ( this->m_fun.fun ( a_unless_void ) ) ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ;                 // the functor
    /// the value
    TYPE2  m_val ;                                                 // the value
    // ========================================================================
  };
  // ==========================================================================
  /** @class SumByValue
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class SumByValue<void,TYPE2> : public LoKi::MultiplyByValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class SumByValue             : public LoKi::MultiplyByValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    SumByValue
    ( const LoKi::Functor<TYPE,TYPE2>&  fun ,
      T2                                val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( fun , val )
    {}
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    SumByValue
    ( T2                                val ,
      const LoKi::Functor<TYPE,TYPE2>&  fun )
      : LoKi::AuxFunBase ( std::tie ( val , fun ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( val , fun )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    SumByValue* clone() const override { return new SumByValue(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->sum ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->func ()
               << "+"    << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class Minus1
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class Minus1<void,TYPE2> : public LoKi::MultiplyByValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class Minus1             : public LoKi::MultiplyByValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    Minus1
    ( const LoKi::Functor<TYPE,TYPE2>&  fun ,
      T2                                val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( fun , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    Minus1* clone() const override { return new Minus1(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->minus1 ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->func ()
               << "-"    << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class Minus2
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class Minus2<void,TYPE2> : public LoKi::MultiplyByValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class Minus2             : public LoKi::MultiplyByValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    Minus2
    ( T2                                val ,
      LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun )
      : LoKi::AuxFunBase ( std::tie ( val , fun ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( val , std::move(fun) )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    Minus2* clone() const override { return new Minus2(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->minus2 ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->val  ()
               << "-"    << this->func () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class Divide1
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class Divide1<void,TYPE2> : public LoKi::MultiplyByValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class Divide1             : public LoKi::MultiplyByValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    Divide1
    ( LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun ,
      T2                                val )
      : LoKi::AuxFunBase ( std::tie ( fun , val ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( std::move(fun) , val )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    Divide1* clone() const override { return new Divide1(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->divide1 ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->func ()
               << "/"    << this->val  () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class Divide2
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2015-01-27
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <class TYPE2>
  class Divide2<void,TYPE2> : public LoKi::MultiplyByValue<void,TYPE2>
#else
  template <class TYPE, class TYPE2=double>
  class Divide2             : public LoKi::MultiplyByValue<TYPE,TYPE2>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument    argument    ;
    // constant type
    typedef typename LoKi::Constant<TYPE,TYPE2>::T2 T2 ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param val the reference value
     *  @param eps the relative precision
     */
    Divide2
    ( T2                                val ,
      LoKi::FunctorFromFunctor<TYPE,TYPE2>  fun )
      : LoKi::AuxFunBase ( std::tie ( val , fun ) )
      , LoKi::MultiplyByValue<TYPE,TYPE2>( val , std::move(fun) )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    Divide2* clone() const override { return new Divide2(*this); }
    /// MANDATORY: the only one essential method :
    TYPE2 operator() ( argument_a_unless_void ) const override
    { return this->divide2 ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " ("   << this->val  ()
               << "/"    << this->func () << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** compare 2 objects using the comparison criteria CMP ,
   *  applied to functions:
   *  <c> cmp( f1(a) , f2(b) ) </c>
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-24
   */
#ifndef _GEN_LOKI_VOIDPRIMITIVES
  template<class TYPE , class CMP=std::less<> , class TYPE2=double>
  class Compare
  {
  public:
    // ========================================================================
    typedef_void_TYPE
    /// typedef for actual function
    typedef LoKi::Functor<TYPE,TYPE2>   function ;
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
    // typedef for comparison criteria
    typedef const CMP            compare  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor
     *  @param fun1 the first functor
     *  @param fun2 the second functor
     *  @param cmp the comparison criteria
     */
    Compare ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun1,
              LoKi::FunctorFromFunctor<TYPE,TYPE2> fun2, CMP  cmp = {}  )
      : m_two ( std::move(fun1) , std::move(fun2) )
      , m_cmp ( std::move(cmp) )
    {}
    // ========================================================================
    /** constructor
     *  @param fun1 the first functor
     *  @param fun2 the second functor
     *  @param cmp the comparison criteria
     */
    Compare ( const LoKi::Functor<TYPE,TYPE2>& fun1 ,
              const compare&  cmp  = {} )
      : m_two ( fun1 , fun1 )
      , m_cmp ( cmp )
    {}
    /// the only one essential method
    bool operator() ( argument a1 , argument a2 ) const
    { return m_cmp ( m_two.fun1 ( a1 ) , m_two.fun2 ( a2 ) ) ; }
    // ========================================================================
  public:
    // ========================================================================
    /// get the first functor
    const function& func1 () const { return m_two.func1 () ; }
    /// get the second functor
    const function& func2 () const { return m_two.func2 () ; }
    // ========================================================================
  private:
    // ========================================================================
    LoKi::TwoFunctors<TYPE,TYPE2> m_two ;
    compare   m_cmp  ;
    // ========================================================================
  };
  // ==========================================================================
  template <class TYPE,class TYPE2,bool> struct Cmp ;
  // ==========================================================================
  template <class TYPE,class TYPE2>
  struct Cmp<TYPE,TYPE2,true>
  {
    typedef Compare<TYPE,std::less<>,TYPE2>     Type ;
  } ;
  // ==========================================================================
  template <class TYPE,class TYPE2>
  struct Cmp<TYPE,TYPE2,false>
  {
    typedef Compare<TYPE,std::greater<>,TYPE2>  Type ;
  } ;
  // ==========================================================================
  /** @class Identity
   *  the simple trivial functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
  template <class TYPE,class TYPE2=TYPE>
  class Identity : public LoKi::Functor<TYPE,TYPE2>
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,TYPE2>::argument argument  ;
  public :
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    Identity* clone () const override { return new Identity(*this) ; }
    /// MANDATORY": the only one essential method
    TYPE2 operator () ( argument_a_unless_void ) const override { return a_unless_void ; }
    /// OPTIONAL: the nice printout
    std::ostream& fillStream ( std::ostream& s ) const override;
    // ========================================================================
  } ;
  // ==========================================================================
  /** @class PrintOut
   *  the simple functor, which "converts" the objects to strings
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2006-04-07
   */
  template <class TYPE>
  class PrintOut final : public LoKi::Functor<TYPE,std::string>
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename LoKi::Functor<TYPE,std::string>::argument argument  ;
  public:
    // ========================================================================
    /// MANDATORY: clone method ("virtual constructor")
    PrintOut* clone () const override { return new PrintOut ( *this ) ; }
    /// MANDATORY": the only one essential method
    std::string operator () ( argument_a_unless_void ) const override
    { return Gaudi::Utils::toString ( a_unless_void  ) ; }
    // ========================================================================
  };
#endif
  // ==========================================================================
  /** @class InRange
   *  Helper predicate to represent that the result of functor
   *  is within the certain "range"
   *  @author Vanya BELYAEV Ivan/Belyaev@nikhef.nl
   *  @date 2009-11-21
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<>
  class InRange<void>: public LoKi::Functor<void,bool>
#else
  template<class TYPE>
  class InRange: public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument              argument ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the functor and edges
     *  @param fun the functor
     *  @param low the low edge
     *  @param high the high edge
     */
    InRange
    ( const double                      low  ,
      LoKi::FunctorFromFunctor<TYPE,double> fun  ,
      const double                      high )
      : LoKi::AuxFunBase ( std::tie ( low , fun , high ) )
      , m_low  ( low  )
      , m_fun  ( std::move(fun) )
      , m_high ( high )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    InRange* clone() const override { return new InRange ( *this ) ; }
    /// MANDATORY: the only one essential method
    bool operator() ( argument_a_unless_void ) const override
    {
      const double r = m_fun.fun ( a_unless_void ) ;
      return m_low <= r && r <= m_high ;
    }
    /// OPTIONAL: the nice printout
    std::ostream& fillStream ( std::ostream& s ) const override
    {
      return s << " in_range(" << m_low
               << ","          << m_fun
               << ","          << m_high << ") " ;

    }
    // ========================================================================
  private:
    // ========================================================================
    /// the low edge
    double                                m_low  ;        //       the low edge
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun  ;        // the functor itself
    /// the high edge
    double                                m_high ;        //       the low edge
    // ========================================================================
  } ;
  // ==========================================================================
  /** @class InRange2
   *  Helper predicate to represent that the result of functor
   *  is within the certain "range"
   *  @author Vanya BELYAEV Ivan/Belyaev@nikhef.nl
   *  @date 2009-11-21
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<>
  class InRange2<void> final : public LoKi::Functor<void,bool>
#else
  template<class TYPE>
  class InRange2 final : public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument              argument ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the functor and edges
     *  @param low the low edge
     *  @param fun the functor
     *  @param high the high edge
     */
    InRange2
    ( const LoKi::Functor<TYPE,double>& low  ,
      const LoKi::Functor<TYPE,double>& fun  ,
      const LoKi::Functor<TYPE,double>& high )
      : LoKi::AuxFunBase ( std::tie ( low , fun , high ) )
      , m_low  ( low  )
      , m_fun  ( fun  )
      , m_high ( high )
    {}
    /** constructor from the functor and edges
     *  @param low the low edge
     *  @param fun the functor
     *  @param high the high edge
     */
    InRange2
    ( const double                      low  ,
      const LoKi::Functor<TYPE,double>& fun  ,
      const LoKi::Functor<TYPE,double>& high )
      : LoKi::AuxFunBase ( std::tie ( low , fun , high ) )
      , m_low  ( LoKi::Constant<TYPE,double> ( low ) )
      , m_fun  ( fun  )
      , m_high ( high )
    {}
    /** constructor from the functor and edges
     *  @param low the low edge
     *  @param fun the functor
     *  @param high the high edge
     */
    InRange2
    ( const LoKi::Functor<TYPE,double>& low  ,
      const LoKi::Functor<TYPE,double>& fun  ,
      const double                      high )
      : LoKi::AuxFunBase ( std::tie ( low , fun , high ) )
      , m_low  ( low  )
      , m_fun  ( fun  )
      , m_high ( LoKi::Constant<TYPE,double> ( high ) )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    InRange2* clone() const override { return new InRange2 ( *this ) ; }
    /// MANDATORY: the only one essential method
    bool operator() ( argument_a_unless_void ) const override
    {
      const double low  = m_low .fun ( a_unless_void ) ;
      const double res  = m_fun .fun ( a_unless_void ) ;
      return low <= res && res <= m_high.fun ( a_unless_void );
    }
    /// OPTIONAL: the nice printout
    std::ostream& fillStream ( std::ostream& s ) const override
    {
      return s << " in_range(" << m_low
               << ","          << m_fun
               << ","          << m_high << ") " ;

    }
    // ========================================================================
  private:
    // ========================================================================
    /// the low edge
    LoKi::FunctorFromFunctor<TYPE,double> m_low  ;        //       the low edge
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun  ;        // the functor itself
    /// the high edge
    LoKi::FunctorFromFunctor<TYPE,double> m_high ;        //      the high edge
    // ========================================================================
  } ;
  // ==========================================================================
  /** @class EqualToList
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-12-06
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class EqualToList<void> : public LoKi::Functor<void,bool>
#else
  template <class TYPE>
  class EqualToList       : public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    EqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      std::vector<double>                vct )
      : LoKi::AuxFunBase ( std::tie ( fun , vct ) )
      , m_fun ( fun )
      , m_vct ( std::move(vct) )
    {
      std::sort ( m_vct.begin() , m_vct.end() ) ;
      auto ilast = std::unique ( m_vct.begin() , m_vct.end() ) ;
      m_vct.erase  ( ilast , m_vct.end() ) ;
    }
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    EqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      const std::vector<int>&            vct )
      : EqualToList( fun,  vct.begin(), vct.end() )
    {}
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    EqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      const std::vector<unsigned int>&   vct )
      : EqualToList( fun, vct.begin(), vct.end() )
    {}
    /** constructor from the function and range of values
     *  @param fun the function
     *  @param first start of range
     *  @param last end of range
     */
    template <typename ITERATOR,
              typename = std::enable_if_t< std::is_base_of< std::input_iterator_tag,
                                                            typename std::iterator_traits<ITERATOR>::iterator_category
                                                                 >::value>>
    EqualToList
    ( const LoKi::Functor<TYPE,double>&  fun    ,
      ITERATOR                           first  ,
      ITERATOR                           last   )
      : EqualToList( fun, std::vector<double>{ first, last } )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    EqualToList* clone() const override { return new EqualToList(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return equal_to ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this->func() << "=="
               << Gaudi::Utils::toString ( m_vct ) << ") " ; }
    // ========================================================================
  public:
    // ========================================================================
    inline bool equal_to ( argument_a_unless_void ) const
    {
      if ( m_vct.empty() ) { return  false ; }
      //
      const double r = m_fun.fun ( a_unless_void ) ;
      //
      LHCb::Math::Equal_To<double> cmp ;
      return std::any_of( m_vct.begin(), m_vct.end(),
                          [&](double item) { return cmp(item,r); } );
    }
    // ========================================================================
    inline bool not_equal_to ( argument_a_unless_void ) const
    { return !this->equal_to ( a_unless_void ) ; }
    // ========================================================================
  public:
    // ========================================================================
    const LoKi::Functor<TYPE,double>& func () const { return m_fun.func() ; }
    /// get the vector
    const std::vector<double>&        vect () const { return m_vct ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    /// the list
    std::vector<double>  m_vct ;                                 //    the list
    // ========================================================================
  };
  // ==========================================================================
  /** @class NotEqualToList
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-12-06
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class NotEqualToList<void> final : public LoKi::EqualToList<void>
#else
  template <class TYPE>
  class NotEqualToList       final : public LoKi::EqualToList<TYPE>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    NotEqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      const std::vector<double>&         vct )
      : LoKi::AuxFunBase ( std::tie ( fun , vct ) )
      , LoKi::EqualToList<TYPE>( fun , vct )
    {}
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    NotEqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      const std::vector<int>&            vct )
      : LoKi::AuxFunBase ( std::tie ( fun , vct ) )
      , LoKi::EqualToList<TYPE>( fun , vct )
    {}
    /** constructor from the function and the value
     *  @param fun the function
     *  @param vct the vector of values
     */
    NotEqualToList
    ( const LoKi::Functor<TYPE,double>&  fun ,
      const std::vector<unsigned int>&   vct )
      : LoKi::AuxFunBase ( std::tie ( fun , vct ) )
      , LoKi::EqualToList<TYPE>( fun , vct )
    {}
    /** constructor from the function and the value
     *  @param fun the function
     *  @param first start of range
     *  @param last end of range
     */
    template <typename ITERATOR,
              typename = std::enable_if_t< std::is_base_of< std::input_iterator_tag,
                                                            typename std::iterator_traits<ITERATOR>::iterator_category
                                                          >::value>>
    NotEqualToList
    ( const LoKi::Functor<TYPE,double>&  fun    ,
      ITERATOR                           first  ,
      ITERATOR                           last   )
      : LoKi::EqualToList<TYPE>( fun , first , last  )
    {}
    // ========================================================================
    /// MANDATORY: clone method ("virtual construcor")
    NotEqualToList* clone() const override { return new NotEqualToList(*this); }
    /// MANDATORY: the only one essential method :
    bool operator() ( argument_a_unless_void ) const override
    { return this -> not_equal_to ( a_unless_void ) ; }
    /// OPTIONAL: the specific printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " (" << this->func() << "!="
               << Gaudi::Utils::toString ( this->vect() ) << ") " ; }
    // ========================================================================
  };
  // ==========================================================================
  /** @class XScaler
   *  Simple scaler for predicates
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2009-12-06
   */
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class XScaler<void> final : public LoKi::Functor<void,bool>
#else
  template <class TYPE>
  class XScaler final : public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /** constructor from predicate and scale
     *  @param cut the predicate
     *  @param scale the scaler
     */
    XScaler
    ( const LoKi::Functor<TYPE,bool>& cut   ,
      const LoKi::Functor<void,bool>& scale )
      : LoKi::AuxFunBase ( std::tie ( cut , scale ) )
      , m_cut    ( cut   )
      , m_scaler ( scale )
    {}
    /// MANDATORY: clone method ("virtual constructor")
    XScaler* clone() const override { return new XScaler ( *this ) ; }
    /// MANDATORY: the only one essential method
    bool operator() ( argument_a_unless_void ) const override
    {
      return m_cut.fun ( a_unless_void ) && m_scaler.fun( /* void */ ) ;
    }
    /// OPTIONAL: nice printout
    std::ostream& fillStream ( std::ostream& s ) const override
    { return s << " scale(" << m_cut << "," << m_scaler << ") " ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the predicate
    LoKi::FunctorFromFunctor<TYPE,bool>  m_cut    ;            // the predicate
    /// the scaler
    LoKi::FunctorFromFunctor<void,bool>  m_scaler ;            // the scaler
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template<>
  class Modulo<void> final : public LoKi::Functor<void,double>
#else
  template<class TYPE>
  class Modulo final : public LoKi::Functor<TYPE,double>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,double>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor
    Modulo ( const LoKi::Functor<TYPE,double>& divident  ,
             const unsigned int                divisor   )
      : LoKi::AuxFunBase ( std::tie ( divident , divisor ) )
      , m_divident ( divident  )
      , m_divisor  ( divisor   )
    {}
    /// clone method (mandatory)
    Modulo* clone() const override { return new Modulo ( *this ) ; }
    /// the only one essential method ("function")
    double operator() ( argument_a_unless_void ) const override
    { return LHCb::Math::round ( m_divident.fun ( a_unless_void ) ) % m_divisor ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " ("  << m_divident << " % "  << m_divisor << ") "; }
    // ========================================================================
  private:
    // ========================================================================
    /// the divident
    LoKi::FunctorFromFunctor<TYPE,double> m_divident ; // the divident
    /// the divisor
    const unsigned int                    m_divisor  ; // the divisor
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class Round<void> final : public LoKi::Functor<void,double>
#else
  template <class TYPE>
  class Round final : public LoKi::Functor<TYPE,double>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,double>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor and the fake argument
    Round ( const LoKi::Functor<TYPE,double>&    fun      ,
            const unsigned int                   fake     )
      : LoKi::AuxFunBase ( std::tie ( fun , fake ) )
      , m_fun  ( fun )
    {}
    /// clone method (mandatory)
    Round* clone() const override { return new Round ( *this ) ; }
    /// the only one essential method ("function")
    double operator() ( argument_a_unless_void ) const override
    { return LHCb::Math::round ( this->m_fun.fun ( a_unless_void ) ) ; }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " round("  << this->m_fun<< ") "; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    // ========================================================================
  };
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class JBit<void> final : public LoKi::Functor<void,bool>
#else
  template <class TYPE>
  class JBit final : public LoKi::Functor<TYPE,bool>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor
    JBit ( const LoKi::Functor<TYPE,double>&    fun ,
           const unsigned short                 j   )
      : LoKi::AuxFunBase ( std::tie ( fun , j ) )
      , m_fun  ( fun )
      , m_j    ( j   )
    {
      //
      static_assert( boost::integer_traits<unsigned long>::is_specialized
                  && boost::integer_traits<unsigned long>::is_integral
                  &&!boost::integer_traits<unsigned long>::is_signed,
                     "must have a unsigned integral type" );
      //
      this -> Assert ( j < (unsigned long) boost::integer_traits<unsigned long>::digits ,
                       "Invalid bit index" ) ;
      //
    }
    /// clone method (mandatory)
    JBit* clone() const override { return new JBit ( *this ) ; }
    /// the only one essential method ("function")
    bool operator() ( argument_a_unless_void ) const override
    {
      const unsigned long _ulv =
        ::labs ( LHCb::Math::round ( this->m_fun.fun ( a_unless_void ) ) ) ;
      //
      return Gaudi::Math::bit ( _ulv , this->m_j ) ;
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " jbit("  << this->m_fun << "," << this->m_j << ") "; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    /// the index
    unsigned short                        m_j   ;                  // the index
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class JBits<void> final : public LoKi::Functor<void,double>
#else
  template <class TYPE>
  class JBits final : public LoKi::Functor<TYPE,double>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,double>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor
    JBits ( const LoKi::Functor<TYPE,double>&     fun ,
            const unsigned short                  j1  ,
            const unsigned short                  j2  )
      : LoKi::AuxFunBase ( std::tie ( fun , j1 , j2 ) )
      , m_fun  ( fun )
      , m_j1  ( j1  )
      , m_j2  ( j2  )
    {
      //
      static_assert( boost::integer_traits<unsigned long>::is_specialized
                     && boost::integer_traits<unsigned long>::is_integral
                     &&!boost::integer_traits<unsigned long>::is_signed,
                     "must have a unsigned integral type" );
      //
      this -> Assert ( j1 <  (unsigned long) boost::integer_traits<unsigned long>::digits ,
                       "Invalid bit index-1" ) ;
      this -> Assert ( j2 <= (unsigned long) boost::integer_traits<unsigned long>::digits ,
                       "Invalid bit index-2" ) ;
      this -> Assert ( j1 < j2 , "Invalid bit indices" ) ;
      //
    }
    /// clone method (mandatory)
    JBits* clone() const override { return new JBits ( *this ) ; }
    /// the only one essential method ("function")
    double operator() ( argument_a_unless_void ) const override
    {
      const unsigned long _ulv =
        ::labs ( LHCb::Math::round ( this->m_fun.fun ( a_unless_void ) ) ) ;
      //
      return Gaudi::Math::bits ( _ulv , this->m_j1 , this -> m_j2 ) ;
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " jbits("  << this->m_fun
               << "," << this->m_j1
               << "," << this->m_j2
               << ") "               ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    /// the index1
    unsigned short                       m_j1  ;                  // the index
    /// the index2
    unsigned short                       m_j2  ;                  // the index
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class JDigit<void> final : public LoKi::Functor<void,double>
#else
  template <class TYPE>
  class JDigit       final : public LoKi::Functor<TYPE,double>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,bool>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor
    JDigit ( LoKi::FunctorFromFunctor<TYPE,double> fun ,
             const unsigned short                  j   )
      : LoKi::AuxFunBase ( std::tie ( fun , j ) )
      , m_fun  ( std::move(fun) )
      , m_j    ( j   )
    {
      //
      static_assert( boost::integer_traits<unsigned long>::is_specialized
                  && boost::integer_traits<unsigned long>::is_integral
                  &&!boost::integer_traits<unsigned long>::is_signed,
                     "must have a unsigned integral type" );
      //
      this -> Assert ( j < (unsigned long) boost::integer_traits<unsigned long>::digits10 ,
                       "Invalid digit index" ) ;
      //
    }
    /// clone method (mandatory)
    JDigit* clone() const override { return new JDigit ( *this ) ; }
    /// the only one essential method ("function")
    double operator() ( argument_a_unless_void ) const override
    {
      const unsigned long _ulv =
        ::labs ( LHCb::Math::round ( this->m_fun.fun ( a_unless_void ) ) ) ;
      //
      return Gaudi::Math::digit ( _ulv , this->m_j ) ;
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " jdigit("  << this->m_fun << "," << this->m_j << ") "; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    /// the index
    unsigned short                        m_j   ;                  // the index
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
#ifdef _GEN_LOKI_VOIDPRIMITIVES
  template <>
  class JDigits<void> final : public LoKi::Functor<void,double>
#else
  template <class TYPE>
  class JDigits       final : public LoKi::Functor<TYPE,double>
#endif
  {
  private:
    // ========================================================================
    typedef_void_TYPE
    /// argument type
    typedef typename_v LoKi::Functor<TYPE,double>::argument argument  ;
    // ========================================================================
  public:
    // ========================================================================
    /// constructor from the functor
    JDigits ( LoKi::FunctorFromFunctor<TYPE,double> fun ,
              const unsigned short                 j1  ,
              const unsigned short                 j2  )
      : LoKi::AuxFunBase ( std::tie ( fun , j1 , j2 ) )
      , m_fun  ( std::move(fun) )
      , m_j1  ( j1  )
      , m_j2  ( j2  )
    {
      //
      static_assert( boost::integer_traits<unsigned long>::is_specialized
                     && boost::integer_traits<unsigned long>::is_integral
                     &&!boost::integer_traits<unsigned long>::is_signed,
                     "must have a unsigned integral type" );
      //
      this -> Assert ( j1 <  (unsigned long) boost::integer_traits<unsigned long>::digits10 ,
                       "Invalid digit index-1" ) ;
      this -> Assert ( j2 <= (unsigned long) boost::integer_traits<unsigned long>::digits10 ,
                       "Invalid digit index-2" ) ;
      this -> Assert ( j1 < j2 , "Invalid digit indices" ) ;
      //
    }
    /// clone method (mandatory)
    JDigits* clone() const override { return new JDigits ( *this ) ; }
    /// the only one essential method ("function")
    double operator() ( argument_a_unless_void ) const override
    {
      const unsigned long _ulv =
        ::labs ( LHCb::Math::round ( this->m_fun.fun ( a_unless_void ) ) ) ;
      //
      return Gaudi::Math::digits ( _ulv , this->m_j1 , this -> m_j2 ) ;
    }
    /// the basic printout method
    std::ostream& fillStream( std::ostream& s ) const override
    { return s << " jdigits("  << this->m_fun
               << "," << this->m_j1
               << "," << this->m_j2
               << ") "               ; }
    // ========================================================================
  private:
    // ========================================================================
    /// the functor itself
    LoKi::FunctorFromFunctor<TYPE,double> m_fun ;                // the functor
    /// the index1
    unsigned short                        m_j1  ;                  // the index
    /// the index2
    unsigned short                        m_j2  ;                  // the index
    // ========================================================================
  };
  // ==========================================================================
#ifndef _GEN_LOKI_VOIDPRIMITIVES
  // ==========================================================================
  // OPTIONAL: the nice printout
  // ==========================================================================
  template <class TYPE, class TYPE2>
  std::ostream& Identity<TYPE,TYPE2>::fillStream ( std::ostream& s ) const
  { return s << "I" ; }
#endif
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                           specializations for void-arguments
// ============================================================================
#ifndef _GEN_LOKI_VOIDPRIMITIVES
#define _GEN_LOKI_VOIDPRIMITIVES 1
#undef typedef_void_TYPE
#undef argument_a_unless_void
#undef typename_v
#undef a_unless_void
#undef class_TYPE_unless_void
#define typedef_void_TYPE typedef void TYPE;
#define argument_a_unless_void
#define typename_v
#define a_unless_void
#define class_TYPE_unless_void
#include "LoKi/Primitives.h"
#undef _GEN_LOKI_VOIDPRIMITIVES
#endif
// ============================================================================
// The END
// ============================================================================
#undef _GEN_LOKI_PRIMITIVES
#endif // _GEN_LOKI_PRIMITIVES
// ============================================================================
