// $Id: Operators.h,v 1.9 2007-12-03 12:03:22 ibelyaev Exp $
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
template <class TYPE,class TYPE2>
inline   LoKi::And<TYPE,TYPE2> 
operator&&  ( const  LoKi::Functor<TYPE,TYPE2>& fun1 , 
              const  LoKi::Functor<TYPE,TYPE2>& fun2 ) 
{ return LoKi::And<TYPE,TYPE2>  ( fun1   , fun2   ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Or<TYPE,TYPE2> 
operator|| ( const  LoKi::Functor<TYPE,TYPE2>& fun1 , 
             const  LoKi::Functor<TYPE,TYPE2>& fun2 ) 
{ return LoKi::Or<TYPE,TYPE2> ( fun1   , fun2 ) ; } 
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
template <class TYPE, class TYPE2>
inline LoKi::Not<TYPE,TYPE2> 
operator!  ( const  LoKi::Functor<TYPE,TYPE2>& fun  ) 
{ return LoKi::Not<TYPE,TYPE2> ( fun )  ; } 
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
template <class TYPE, class TYPE2>
inline LoKi::Negate<TYPE,TYPE2> 
operator-( const  LoKi::Functor<TYPE,TYPE2>& fun  ) 
{ return LoKi::Negate<TYPE,TYPE2> ( fun )  ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Less<TYPE,TYPE2> 
operator<  ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 , 
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Less<TYPE,TYPE2> ( fun1   , fun2   ) ; } ;
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
template <class TYPE,class TYPE2>
inline   LoKi::Less<TYPE,TYPE2> 
operator< ( const  LoKi::Functor<TYPE,TYPE2>&       fun , 
            typename LoKi::Constant<TYPE,TYPE2>::T2 val ) 
{ return LoKi::Less<TYPE,TYPE2>( fun , val ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Less<TYPE,TYPE2> 
operator< ( typename LoKi::Constant<TYPE,TYPE2>::T2 val , 
            const  LoKi::Functor<TYPE,TYPE2>&       fun ) 
{ return LoKi::Less<TYPE,TYPE2>( val , fun ) ; }
// ============================================================================
/** inline construction for operator ">" 
 *
 *  @code 
 *
 *  typedef Functor<SomeType,double>          Func ;
 *  typedef FunctorFrinFunctor<SomeType,bool> PfP  ;
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
template <class TYPE, class TYPE2>
inline   LoKi::Less<TYPE, TYPE2> 
operator>  ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 , 
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Less<TYPE,TYPE2> ( fun2 , fun1 ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Less<TYPE,TYPE2> 
operator>  ( const  LoKi::Functor<TYPE,TYPE2>&       fun  , 
             typename LoKi::Constant<TYPE,TYPE2>::T2 val ) 
{ return LoKi::Less<TYPE,TYPE2>( val , fun ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Less<TYPE,TYPE2> 
operator> ( typename LoKi::Constant<TYPE,TYPE2>::T2 val , 
            const  LoKi::Functor<TYPE,TYPE2>&       fun  ) 
{ return LoKi::Less<TYPE,TYPE2>( fun , val ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Equal<TYPE,TYPE2> 
operator== ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
             const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Equal<TYPE,TYPE2>  ( fun1   , fun2   ) ; }
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
 *    PfP f = A == B             ; // operator form
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
template <class TYPE, class TYPE2>
inline   LoKi::EqualToValue<TYPE,TYPE2> 
operator== ( const LoKi::Functor<TYPE,TYPE2>&        fun  , 
             typename LoKi::Constant<TYPE,TYPE2>::T2 val  ) 
{ return LoKi::EqualToValue<TYPE,TYPE2> ( fun   , val  ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::EqualToValue<TYPE,TYPE2> 
operator== ( typename LoKi::Constant<TYPE,TYPE2>::T2 val  ,
             const LoKi::Functor<TYPE,TYPE2>&        fun  ) 
{ return LoKi::EqualToValue<TYPE,TYPE2> ( val , fun  ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2> 
operator<= ( const  LoKi::Functor<TYPE,TYPE2>&  fun1 , 
             const  LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::LessOrEqual<TYPE,TYPE2> ( fun1 , fun2 ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2> 
operator<= ( const  LoKi::Functor<TYPE,TYPE2>&       fun ,
             typename LoKi::Constant<TYPE,TYPE2>::T2 val ) 
{ return LoKi::LessOrEqual<TYPE,TYPE2>( fun , val ) ; }
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
template <class TYPE,class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2>
operator<= ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
             const  LoKi::Functor<TYPE,TYPE2>&       fun )
{ return LoKi::LessOrEqual<TYPE,TYPE2>( val , fun ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2> 
operator>= ( const  LoKi::Functor<TYPE,TYPE2>& fun1 , 
             const  LoKi::Functor<TYPE,TYPE2>& fun2 ) 
{ return LoKi::LessOrEqual<TYPE,TYPE2> ( fun2 , fun1 ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2> 
operator>= ( const  LoKi::Functor<TYPE,TYPE2>&       fun , 
             typename LoKi::Constant<TYPE,TYPE2>::T2 val ) 
{ return LoKi::LessOrEqual<TYPE,TYPE2> ( val , fun ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::LessOrEqual<TYPE,TYPE2> 
operator>= ( typename LoKi::Constant<TYPE,TYPE2>::T2 val , 
             const  LoKi::Functor<TYPE,TYPE2>&       fun )
{ return LoKi::LessOrEqual<TYPE,TYPE2> ( fun , val ) ; }
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
template <class TYPE, class TYPE2>
inline LoKi::NotEqual<TYPE,TYPE2> 
operator!= ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
             const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::NotEqual<TYPE,TYPE2> ( fun1 , fun2 ) ; } 
// ============================================================================
/** inline construction of operator "!=" 
 *
 *  @code 
 *
 *  typedef Functor<SomeType,double>          Func ;
 *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
 *
 *  void func( const Func& A , const double B )
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
template <class TYPE, class TYPE2>
inline LoKi::NotEqualToValue<TYPE,TYPE2> 
operator!= ( const LoKi::Functor<TYPE,TYPE2>&        fun , 
             typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return LoKi::NotEqualToValue<TYPE,TYPE2> ( fun , val ) ; }
// ============================================================================
/** inline construction of operator "!=" 
 *
 *  @code 
 *
 *  typedef Functor<SomeType,double>          Func ;
 *  typedef FunctorFromFunctor<SomeType,bool> PfP  ;
 *
 *  void func( const Func& A , const double B )
 *  {
 *    PfP f = B != A             ; // operator form
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
template <class TYPE, class TYPE2>
inline LoKi::NotEqualToValue<TYPE,TYPE2> 
operator!= ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
             const LoKi::Functor<TYPE,TYPE2>&        fun )
{ return LoKi::NotEqualToValue<TYPE,TYPE2> ( val , fun ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Plus<TYPE,TYPE2> 
operator+ ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
            const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Plus<TYPE,TYPE2> ( fun1  , fun2 ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Plus<TYPE,TYPE2> 
operator+ ( const LoKi::Functor<TYPE,TYPE2>&        fun , 
            typename LoKi::Constant<TYPE,TYPE2>::T2 val ) 
{ return LoKi::Plus<TYPE,TYPE2>( fun , val ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Plus<TYPE,TYPE2> 
operator+ ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            const LoKi::Functor<TYPE,TYPE2>&        fun )
{ return LoKi::Plus<TYPE,TYPE2>( val , fun ) ; }
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
template <class TYPE,class TYPE2>
inline   LoKi::Minus<TYPE,TYPE2> 
operator- ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
            const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Minus<TYPE,TYPE2> ( fun1  , fun2 ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Minus<TYPE,TYPE2> 
operator- ( const LoKi::Functor<TYPE,TYPE2>&        fun , 
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return LoKi::Minus<TYPE,TYPE2> ( fun  , val ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Minus<TYPE,TYPE2> 
operator- ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            const LoKi::Functor<TYPE,TYPE2>&        fun )
{ return LoKi::Minus<TYPE,TYPE2> ( val , fun ) ; } 
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
template <class TYPE, class TYPE2>
inline   LoKi::Divide<TYPE,TYPE2> 
operator/ ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
            const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Divide<TYPE,TYPE2>  ( fun1  , fun2 ) ; } 
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
template <class TYPE,class TYPE2>
inline   LoKi::Divide<TYPE,TYPE2> 
operator/ ( const LoKi::Functor<TYPE,TYPE2>&        fun , 
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return LoKi::Divide<TYPE,TYPE2>  ( fun  , val ) ; } 
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
template <class TYPE,class TYPE2>
inline   LoKi::Divide<TYPE,TYPE2> 
operator/ ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            const LoKi::Functor<TYPE,TYPE2>&        fun )
{ return LoKi::Divide<TYPE,TYPE2>  ( val , fun ) ; }
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
template <class TYPE, class TYPE2>
inline   LoKi::Multiply<TYPE,TYPE2> 
operator* ( const LoKi::Functor<TYPE,TYPE2>&  fun1 , 
            const LoKi::Functor<TYPE,TYPE2>&  fun2 ) 
{ return LoKi::Multiply<TYPE,TYPE2>  ( fun1  , fun2 ) ; } 
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
template <class TYPE,class TYPE2>
inline   LoKi::Multiply<TYPE,TYPE2> 
operator* ( const LoKi::Functor<TYPE,TYPE2>&        fun , 
            typename LoKi::Constant<TYPE,TYPE2>::T2 val )
{ return LoKi::Multiply<TYPE,TYPE2> ( fun , val ) ; }
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
template <class TYPE,class TYPE2>
inline   LoKi::Multiply<TYPE,TYPE2> 
operator* ( typename LoKi::Constant<TYPE,TYPE2>::T2 val ,
            const LoKi::Functor<TYPE,TYPE2>&        fun )
{ return LoKi::Multiply<TYPE,TYPE2> ( val , fun ) ; }

// ============================================================================

// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_OPERATORS_H
// ============================================================================
