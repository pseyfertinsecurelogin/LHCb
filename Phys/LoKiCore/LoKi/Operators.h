// ============================================================================
#ifndef LOKI_OPERATORS_H
#define LOKI_OPERATORS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Primitives.h"
#include "LoKi/apply.h"
#include "LoKi/Streamers.h"
// ============================================================================
/** @file
 *
 *  Inline operators to deal with LoKi functions and predicates
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
 */
// ============================================================================
namespace LoKi
{

  namespace details {

      template <typename T, typename U>
      using decays_to = std::is_convertible<typename std::decay<T>::type*, U*>;

      // is Derived derived from Base<Args...>?
      template <template <typename ...> class Base, typename Derived>
      struct is_derived_from_template_helper {
          template <typename ... Args>
          static std::true_type  test(Base<Args...>*);
          static std::false_type test(void*);

          using type = decltype(test(std::declval<typename std::decay<Derived>::type*>()));
      };

      template <typename T>
      using is_functor = typename is_derived_from_template_helper<LoKi::Functor,T>::type;

      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE?
      // If T derives from LoKi::Functor<TYPE,TYPE2> what is TYPE2?
      template <typename T,
                typename = typename std::enable_if<is_functor<T>::value>::type>
      struct LF {
          using U = typename std::decay<T>::type;
          template <typename T1, typename T2> static T1 test1( LoKi::Functor<T1,T2>* );
          template <typename T1, typename T2> static T2 test2( LoKi::Functor<T1,T2>* );
          using type1 = decltype( test1(std::declval<U*>()) );
          using type2 = decltype( test2(std::declval<U*>()) );
      };

      // ( F1 = LoKi::Functor<TYPE,TYPE2> , F2 = LoKi::Functor<TYPE,TYPE2> ) -> TYPE
      // ( F1 = LoKi::Functor<TYPE,TYPE2> , F2 = LoKi::Functor<TYPE,TYPE2> ) -> TYPE2
      template <typename F1, typename F2,
                typename = typename std::enable_if<is_functor<F1>::value
                                               &&  is_functor<F2>::value
                                               // the following is not strictly neccessary anymore...
                                               &&  std::is_same< typename LF<F1>::type1, typename LF<F2>::type1 >::value
                                               &&  std::is_same< typename LF<F1>::type2, typename LF<F2>::type2 >::value
                                               >::type>
      struct LF2 {
          using type1 = typename std::common_type< typename LF<F1>::type1,
                                                   typename LF<F2>::type1 >::type;
          using type2 = typename std::common_type< typename LF<F1>::type2,
                                                   typename LF<F2>::type2 >::type;
      };
  }
  // ============================================================================
  /** inline construction of logical 'AND'
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A , const Pred& B )
   *  {
   *    PfP p1 = A && B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::And
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::And<TYPE,TYPE2>
  operator&&  ( F1&& fun1 , F2&& fun2 )
  {
      return { std::forward<F1>(fun1), std::forward<F2>(fun2) };
  }
  // ============================================================================
  /** inline construction of logical 'OR'
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A , const Pred& B )
   *  {
   *    PfP p1 = A || B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Or
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::Or<TYPE,TYPE2>
  operator|| ( F1&& fun1 , F2&& fun2 )
  {
      return { std::forward<F1>(fun1), std::forward<F2>(fun2) };
  }
  // ============================================================================
  /** inline construction for logical negate
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,bool>             Pred ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool>  PfP  ;
   *
   *  void func( const Pred& A  )
   *  {
   *    PfP p1 = !A            ; // operator form
   *  }
   *
   *  @endcode
   *
   *
   *  @see std::logical_not
   *  @see LoKi::Not
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline LoKi::Not<TYPE,TYPE2>
  operator!  ( F&& fun  )
  { return { std::forward<F>( fun ) }  ; }
  // ============================================================================
  /** inline construction for unary negate
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>             Func;
   *  typedef LoKi::FunctorFromFunctor<SomeType,double>  FfF  ;
   *
   *  void func( const Func& A  )
   *  {
   *    FfF p1 = -A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *
   *  @see std::negate
   *  @see LoKi::Negate
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline LoKi::Negate<TYPE,TYPE2>
  operator-( F&& fun  )
  { return { std::forward<F>( fun ) }; }
  // ============================================================================
  /** inline construction of operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func& B )
   *  {
   *    PfP f = A < B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::Less<TYPE,TYPE2>
  operator<  ( F1&& fun1 , F2&& fun2 )
  { return { std::forward<F1>(fun1), std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double B  )
   *  {
   *    PfP f = A < B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::LessThanValue<TYPE,TYPE2>
  operator< ( F&&       fun ,
              typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::forward<F>(fun) , val } ; }
  // ============================================================================
  /** inline construction for operator "<"
   *
   *  @code
   *
   *  typedef LoKi::Functor<SomeType,double>          Func ;
   *  typedef LoKi::FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double B  )
   *  {
   *    PfP f = B < A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::GreaterThanValue<TYPE,TYPE2>
  operator< ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
              F&&       fun )
  { return { std::forward<F>(fun) , val } ; }
  // ============================================================================
  /** inline construction for operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func& B )
   *  {
   *    PfP f = A > B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::Less<TYPE, TYPE2>
  operator>  ( F1&& fun1 , F2&& fun2 )
  { return { std::forward<F2>(fun2), std::forward<F1>(fun1) } ; }
  // ============================================================================
  /** inline construction for operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFrinFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double B )
   *  {
   *    PfP f = A > B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::GreaterThanValue<TYPE,TYPE2>
  operator>  ( F&&       fun  ,
               typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::forward<F>(fun) , val } ; }
  // ============================================================================
  /** inline construction of operator ">"
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFrinFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double B )
   *  {
   *    PfP f = B > A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Less
   *  @see LoKi::Constant
   *  @return functor
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::LessThanValue<TYPE,TYPE2>
  operator> ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
              F&& fun  )
  { return { std::forward<F>(fun) , val } ; }
  // ============================================================================
  /** inline construction of operator "=="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func& B )
   *  {
   *    PfP f = A == B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::Equal
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::Equal<TYPE,TYPE2>
  operator== ( F1&& fun1 ,
               F2&& fun2 )
  { return  { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "=="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double B )
   *  {
   *    PfP f = B == A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::EqualToValue
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::EqualToValue<TYPE,TYPE2>
  operator== ( typename LoKi::Constant<TYPE,TYPE2>::T2 val  , F&& fun  )
  { return { std::move(val) , std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline LoKi::EqualToValue<TYPE,TYPE2>
  operator== ( F&& fun , typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::move(val) , std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline LoKi::NotEqualToValue<TYPE,TYPE2>
  operator!= ( typename  LoKi::Constant<TYPE,TYPE2>::T2 val , F&& fun  )
  { return { std::move(val) , std::forward<F>(fun) } ; }
  // ============================================================================
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline LoKi::NotEqualToValue<TYPE,TYPE2>
  operator!= ( F&& fun , typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::move(val) , std::forward<F>(fun) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func& B )
   *  {
   *    PfP f = A <= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::LessOrEqual<TYPE,TYPE2>
  operator<= ( F1&&  fun1 , F2&&  fun2 )
  { return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double  B )
   *  {
   *    PfP f = A <= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::LessOrEqualValue<TYPE,TYPE2>
  operator<= ( F&&       fun ,
               typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::forward<F>(fun) , std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "<="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double  B )
   *  {
   *    PfP f = B <= A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @see LoKi::Constant
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::GreaterOrEqualValue<TYPE,TYPE2>
  operator<= ( typename LoKi::Constant<TYPE,TYPE2>::T2 val , F&& fun )
  { return { std::forward<F>(fun) , std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func&  B )
   *  {
   *    PfP f = A >= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline   LoKi::LessOrEqual<TYPE,TYPE2>
  operator>= ( F1&& fun1 , F2&& fun2 )
  { return { std::forward<F2>(fun2) , std::forward<F1>(fun1) } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double  B )
   *  {
   *    PfP f = A >= B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::GreaterOrEqualValue<TYPE,TYPE2>
  operator>= ( F&&       fun ,
               typename LoKi::Constant<TYPE,TYPE2>::T2 val )
  { return { std::forward<F>(fun) , val } ; }
  // ============================================================================
  /** inline construction of operator ">="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const double  B )
   *  {
   *    PfP f = B >= A             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::LessOrEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F,
            typename TYPE  = typename details::LF<F>::type1,
            typename TYPE2 = typename details::LF<F>::type2>
  inline   LoKi::LessOrEqualValue<TYPE,TYPE2>
  operator>= ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
               F&&     fun )
  { return { std::forward<F>(fun) , std::move(val) } ; }
  // ============================================================================
  /** inline construction of operator "!="
   *
   *  @code
   *
   *  typedef Functor<SomeType,double>          Func ;
   *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
   *
   *  void func( const Func& A , const Func& B )
   *  {
   *    PfP f = A != B             ; // operator form
   *  }
   *
   *  @endcode
   *
   *  @see LoKi::NotEqual
   *  @return functor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename F1, typename F2,
            typename TYPE  = typename details::LF2<F1,F2>::type1,
            typename TYPE2 = typename details::LF2<F1,F2>::type2>
  inline LoKi::NotEqual<TYPE,TYPE2>
  operator!= ( F1&&  fun1 , F2&&  fun2 )
  { return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
  // ============================================================================
  /** inline construction of operator "+"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func( const Func& A , const Func& B )
 *  {
 *    FfF f = A + B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Plus
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE  = typename details::LF2<F1,F2>::type1,
          typename TYPE2 = typename details::LF2<F1,F2>::type2>
inline   LoKi::Plus<TYPE,TYPE2>
operator+ ( F1&&  fun1 ,
            F2&&  fun2 )
{ return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
// ============================================================================
/** inline construction of operator "+"
 *
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func( const Func& A , const double B )
 *  {
 *    FfF f = A + B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Plus
 *  @see LoKi::Constant
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::SumByValue<TYPE,TYPE2>
operator+ ( F&&        fun ,
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return { std::forward<F>(fun) , std::move(val) } ; }
// ============================================================================
/** inline construction of operator "+"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func( const Func& A , const double B )
 *  {
 *    FfF f = B + A            ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Plus
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::SumByValue<TYPE,TYPE2>
operator+ ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            F&&        fun )
{ return { std::move(val) , std::forward<F>(fun) } ; }
// ============================================================================
/** inline construction of operator "-"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func( const Func& A , const Func& B )
 *  {
 *    FfF f = A - B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Minus
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE  = typename details::LF2<F1,F2>::type1,
          typename TYPE2 = typename details::LF2<F1,F2>::type2>
inline   LoKi::Minus<TYPE,TYPE2>
operator- ( F1&&  fun1 ,
            F2&&  fun2 )
{ return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
// ============================================================================
/** inline construction of operator "-"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = A - B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Minus
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::Minus1<TYPE,TYPE2>
operator- ( F&&        fun ,
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return { std::forward<F>(fun) , std::move(val) } ; }
// ============================================================================
/** inline construction of operator "-"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = B - A             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Minus
 *  @see LoKi::Constant
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::Minus2<TYPE,TYPE2>
operator- ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            F&&        fun )
{ return { std::move(val) , std::forward<F>(fun) } ; }
// ============================================================================
/** inline construction of operator "/"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const Func& B )
 *  {
 *    FfF f = A / B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Divide
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE  = typename details::LF2<F1,F2>::type1,
          typename TYPE2 = typename details::LF2<F1,F2>::type2>
inline   LoKi::Divide<TYPE,TYPE2>
operator/ ( F1&&  fun1 ,
            F2&&  fun2 )
{ return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
// ============================================================================
/** inline construction of operator "/"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = A / B              ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Divide
 *  @see LoKi::Constant
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::Divide1<TYPE,TYPE2>
operator/ ( F&&        fun ,
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return { std::forward<F>(fun) , std::move(val) } ; }
// ============================================================================
/** inline construction of operator "/"
 *
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = B / A             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Divide
 *  @see LoKi::Constant
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::Divide2<TYPE,TYPE2>
operator/ ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            F&&        fun )
{ return { std::move(val) , std::forward<F>(fun) } ; }
// ============================================================================
/** inline construction of operator "*"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const Func& B  )
 *  {
 *    FfF f = A * B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Multiply
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE  = typename details::LF2<F1,F2>::type1,
          typename TYPE2 = typename details::LF2<F1,F2>::type2>
inline   LoKi::Multiply<TYPE,TYPE2>
operator* ( F1&&  fun1 ,
            F2&&  fun2 )
{ return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
// ============================================================================
/** inline construction of operator "*"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = A * B             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Multiply
 *  @see LoKi::Constant
 *  @return functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::MultiplyByValue<TYPE,TYPE2>
operator* ( F&&        fun ,
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return { std::forward<F>(fun) , std::move(val) } ; }
// ============================================================================
/** inline construction of operator "*"
 *
 *  @code
 *
 *  typedef Functor<SomeType,double>            Func ;
 *  typedef FunctorFromFunctor<SomeType,double> FfF  ;
 *
 *  void func ( const Func& A , const double B )
 *  {
 *    FfF f = B * A             ; // operator form
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Multiply
 *  @see LoKi::Constant
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F,
          typename TYPE  = typename details::LF<F>::type1,
          typename TYPE2 = typename details::LF<F>::type2>
inline   LoKi::MultiplyByValue<TYPE,TYPE2>
operator* ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            F&&        fun )
{ return { std::move(val) , std::forward<F>(fun) } ; }
// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<double>&        vct )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<int>&           vct )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<unsigned int>&  vct )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<double>&        vct )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<int>&           vct )
{ return { fun , vct } ; }
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const LoKi::Functor<TYPE,double>& fun ,
              const std::vector<unsigned int>&  vct )
{ return { fun , vct } ; }


// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const std::vector<double>&        vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const std::vector<int>&           vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::EqualToList<TYPE>
operator == ( const std::vector<unsigned int>&  vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const std::vector<double>&        vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const std::vector<int>&           vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================
template <class TYPE>
inline LoKi::NotEqualToList<TYPE>
operator != ( const std::vector<unsigned int>&  vct ,
              const LoKi::Functor<TYPE,double>& fun )
{ return { fun , vct } ; }
// ============================================================================

// ============================================================================
/** inline construction of "modulo" operator
 *
 *  @see LoKi::Modulo
 *
 *  @author Vanya BELYAEV ibelyaev@cern.ch
 *  @date 2011-03-30
 */
template <class TYPE>
inline   LoKi::Modulo<TYPE>
operator% ( const LoKi::Functor<TYPE,double>& divident ,
            unsigned int                divisor  )
{ return { divident , divisor } ; }
// ============================================================================
} // namespace LoKi


// ============================================================================
#include "LoKi/Value.h"
// ============================================================================
// mixed types: TYPE & void
// ============================================================================
namespace LoKi {
// less
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Less<TYPE,TYPE2>
operator<  ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const  LoKi::Functor<void,TYPE2>&  fun2 )
{ return { fun1   , LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Less<TYPE,TYPE2>
operator<  ( const  LoKi::Functor<void,TYPE2>&  fun1 ,
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun1 ) , fun2 } ; }
// less or equal
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::LessOrEqual<TYPE,TYPE2>
operator<= ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const  LoKi::Functor<void,TYPE2>&  fun2 )
{ return { fun1   , LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::LessOrEqual<TYPE,TYPE2>
operator<= ( const  LoKi::Functor<void,TYPE2>&  fun1 ,
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun1 ) , fun2 } ; }
// greater
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Less<TYPE,TYPE2>
operator>  ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const  LoKi::Functor<void,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun2 ) , fun1 } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Less<TYPE,TYPE2>
operator>  ( const  LoKi::Functor<void,TYPE2>&  fun1 ,
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { fun2 , LoKi::Value<TYPE,TYPE2> ( fun1 ) } ; }
// greater or equal
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::LessOrEqual<TYPE,TYPE2>
operator>= ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const  LoKi::Functor<void,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun2 ) , fun1 } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::LessOrEqual<TYPE,TYPE2>
operator>= ( const  LoKi::Functor<void,TYPE2>&  fun1 ,
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { fun2 , LoKi::Value<TYPE,TYPE2> ( fun1 ) } ; }
// equality
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Equal<TYPE,TYPE2>
operator== ( const LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const LoKi::Functor<void,TYPE2>&  fun2 )
{ return { fun1 , LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline   LoKi::Equal<TYPE,TYPE2>
operator== ( const LoKi::Functor<void,TYPE2>&  fun1 ,
             const LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun1 ) , fun2 } ; }
// non-equality
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline LoKi::NotEqual<TYPE,TYPE2>
operator!= ( const LoKi::Functor<TYPE,TYPE2>&  fun1 ,
             const LoKi::Functor<void,TYPE2>&  fun2 )
{ return { fun1 , LoKi::Value<TYPE,TYPE2> ( fun2 ) } ; }
//
template <class TYPE, class TYPE2, typename = typename std::enable_if< !std::is_void<TYPE>::value>::type>
inline LoKi::NotEqual<TYPE,TYPE2>
operator!= ( const LoKi::Functor<void,TYPE2>&  fun1 ,
             const LoKi::Functor<TYPE,TYPE2>&  fun2 )
{ return { LoKi::Value<TYPE,TYPE2> ( fun1 ) , fun2 } ; }


// ============================================================================
} // namespace LoKi


// ============================================================================
// The END
// ============================================================================
#endif // LOKI_OPERATORS_H
// ============================================================================
