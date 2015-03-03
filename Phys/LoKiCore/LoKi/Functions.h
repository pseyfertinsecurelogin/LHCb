// $Id: Functions.h,v 1.18 2007-06-10 19:54:05 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_FUNCTIONS_H 
#define LOKI_FUNCTIONS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL 
// ============================================================================
#include <functional>
#include <vector>
#include <string>
#include <ostream>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatusCode.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AuxFunBase.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/call_traits.hpp"
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
 */
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @class Function
   *  basic abstract type for LoKi function 
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-15
   */
  template <class TYPE>
  class Function : 
    public    std::unary_function<const TYPE,double> , 
    virtual   public LoKi::AuxFunBase
  {
  public:
    /// the type of the argument
    typedef TYPE        Type ;
    /// the actual type of base 
    typedef typename std::unary_function<const TYPE,double>  Base_F ;
    typedef typename std::unary_function<const TYPE,double>  Base_1 ;
    /// the result value 
    typedef typename Base_F::result_type   result_type   ;
    /// the basic argument type 
    typedef typename Base_F::argument_type argument_type ;
    /// vector or results 
    typedef std::vector<result_type>       vector_result ;
    /// type for the argument 
    typedef typename boost::call_traits<const TYPE>::param_type argument;
    /// common FunB class 
    typedef Function<TYPE>  FunB ;
    /// own type 
    typedef Function<TYPE>  Self ;     
    /// the only one essential method ("function")
    virtual result_type operator () ( argument    ) const = 0 ;
    /// the only one essential method ("function")
    virtual result_type evaluate    ( argument  a ) const 
    { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual result_type eval        ( argument  a ) const 
    { return (*this)( a ) ; }
    /// clone method 
    virtual FunB*  clone    ()                 const = 0 ;
    /// virtual destructor 
    virtual ~Function(){}
  public:
    /** apply the function to the sequence of arguments 
     *  and produce the sequence of 
     *  "results" - similar to Python's "map" function
     *  @code 
     *  
     *   CONTAINER                 objects  = ... ; 
     *   const Function<SOMETYPE>& function = ... ;
     *   std:vector<double>        result ;
     *   function.evaluate ( objects.begin     ()       , 
     *                       objects.end       ()       , 
     *                       std::back_inserter( result ) ) ;
     *  @endcode
     *  @param  first  begin iterator of the argument sequence
     *  @param  last   end iterator of the argument sequence
     *  @param  output output iterator for the result 
     *  @return length of the result sequence
     */
    template <class INPUT,class OUTPUT>
    inline size_t evaluate
    ( INPUT  first  , 
      INPUT  last   , 
      OUTPUT output ) const 
    {
      // overall length 
      size_t length = 0  ;
      // 'transform'
      for ( ; first != last ; ++first , ++output , ++length ) 
      { *output = (*this)( *first ) ; }
      return length ;
    };   
    /** apply the function to the sequence of arguments 
     *  and produce the sequence of 
     *  "results" - similar to Python's "map" function
     *  @code 
     *   CONTAINER                 objects  = ... ; 
     *   const Function<SOMETYPE>& function = ... ;
     *   std::vector<double> result  = 
     *     function.evaluate ( objects.begin () ,
     *                         objects.end   () ) ;
     *  @endcode
     *  @param  first  begin iterator of the argument sequence
     *  @param  last   end iterator of the argument sequence
     *  @return container of results
     */ 
    template <class INPUT>
    inline vector_result evaluate
    ( INPUT  first  , 
      INPUT  last   ) const 
    {
      vector_result result ( std::distance ( first , last ) );
      evaluate( first , last , result.begin() ) ;
      return result ;
    };
  protected:
    /// protected default constructor 
    Function(): AuxFunBase() {};
    /// protected copy constructor
    Function( const Function& fun ) : AuxFunBase( fun ) {};
  private:
    /// assignement         is private 
    Self& operator=( const Self& );
  };  
  // ==========================================================================
  /** @class Predicate
   *  The basic abstract for LoKi predicate 
   *  (the function with boolean return value)
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   2002-07-15
   */
  template <class TYPE>
  class Predicate 
    : public std::unary_function<const TYPE,bool> 
    , virtual public LoKi::AuxFunBase 
  {
  public:
    /// the type of the argument
    typedef TYPE        Type ;
    /// the actual type of base 
    typedef typename std::unary_function<const TYPE,bool>  Base_P ;
    typedef typename std::unary_function<const TYPE,bool>  Base_1 ;
    /// the result value 
    typedef typename Base_P::result_type   result_type   ;
    /// the basic argument type 
    typedef typename Base_P::argument_type argument_type ;
    /// vector of results 
    typedef std::vector<result_type>       vector_result ;
    /// type for argument 
    typedef typename boost::call_traits<const TYPE>::param_type argument;
    /// common FunB class 
    typedef Predicate<TYPE>  FunB     ;
    /// own type 
    typedef Predicate<TYPE>  Self     ;     
    /// the only one essential method ("function")
    virtual bool  operator () ( argument   ) const = 0 ;
    /// the only one essential method ("function")
    virtual bool  evaluate    ( argument a ) const 
    { return (*this)( a ) ; }
    /// the only one essential method ("function")
    virtual bool  eval        ( argument a ) const 
    { return (*this)( a ) ; }
    /// clone method 
    virtual FunB* clone    ()              const = 0 ;
    /// virtual destructor 
    virtual ~Predicate(){}
  public:
    /** apply the predicate to the sequence of arguments 
     *  and produce the sequence of 
     *  "results" - similar to Python's "map" function
     *  @code 
     *  
     *   CONTAINER                  objects  = ... ; 
     *   const Predicate<SOMETYPE>& predicate= ... ;
     *   std:vector<double>         result ;
     *   predicate.evaluate ( objects.begin     ()       , 
     *                        objects.end       ()       , 
     *                        std::back_inserter( result ) ) ;
     *  @endcode
     *  @param  first  begin iterator of the argument sequence
     *  @param  last   end iterator of the argument sequence
     *  @param  output output iterator for the result 
     *  @return length of the result sequence
     */
    template <class INPUT,class OUTPUT>
    inline size_t evaluate
    ( INPUT  first  , 
      INPUT  last   , 
      OUTPUT output ) const 
    {
      // overall length 
      size_t length = 0 ;
      // 'transform'
      for ( ; first != last ; ++first , ++output , ++length ) 
      { *output = (*this)( *first ) ; }
      return length ;
    };
    /** apply the predicate to the sequence of arguments 
     *  and produce the sequence of 
     *  "results" - similar to Python's "map" function
     *  @code 
     *  
     *   CONTAINER                  objects  = ... ; 
     *   const Predicate<SOMETYPE>& predicate = ... ;
     *   std:vector<bool> result = 
     *   predicate.evaluate ( objects.begin () ,
     *                        objects.end   () ) ;
     *  @endcode
     *  @param  first  begin iterator of the argument sequence
     *  @param  last   end iterator of the argument sequence
     *  @return container of results
     */ 
    template <class INPUT>
    inline vector_result evaluate
    ( INPUT  first  , 
      INPUT  last   ) const 
    {
      vector_result result ( std::distance ( first , last ) );
      evaluate ( first , last , result.begin() ) ;
      return result ;
    } ;
  protected:
    /// protected default constructor 
    Predicate(): AuxFunBase() {};
    /// protected copy constructor
    Predicate ( const Predicate& pred ) : AuxFunBase( pred ) {};
  private:
    /// assignement         is private 
    Predicate& operator=( const Predicate& );
  } ;
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BiFunctions.h"
#include "LoKi/Primitives.h"
#include "LoKi/Math.h"
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_FUNCTIONS_H
// ============================================================================
