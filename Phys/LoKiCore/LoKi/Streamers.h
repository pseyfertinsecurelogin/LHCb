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
#ifndef LOKI_STREAMERS_H
#define LOKI_STREAMERS_H 1
// ============================================================================
// Include files
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Primitives.h"
#include "LoKi/Operators.h"
#include "LoKi/Cast.h"
#include "LoKi/apply.h"
#include "LoKi/compose.h"
#include "LoKi/Filters.h"
#include "LoKi/Stat.h"
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
 */
// ============================================================================
/** compose/chain the function
 *
 *  @code
 *
 *  typedef Functor<Type1,Type2>             Func1 ;
 *  typedef Functor<Type2,Type3>             Func2 ;
 *  typedef Functor<Type3,Type4>             Func3 ;
 *  typedef Functor<Type4,Type5>             Func4 ;
 *
 *  typedef FunctorFromFunctor<Type1,Type5>  Fun   ;
 *
 *  void func ( const Func1& A ,
 *              const Func2& B ,
 *              const Func3& C ,
 *              const Func4& D )
 *  {
 *
 *    Fun  fun = A >> B >> C >> D  ;
 *
 *  }
 *
 *  @endcode
 *
 *  @see LoKi::Compose
 *
 *  @param fun1 the first functor to be chained
 *  @param fun2 the second functor to be chained
 *  @return the chained functor
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE  = LoKi::details::type1_t<F1>,
          typename TYPE1 = LoKi::details::type2_t<F1>,
          typename TYPE2 = LoKi::details::type2_t<F2>,
          typename = LoKi::details::require_signature<F1,TYPE,TYPE1>,
          typename = LoKi::details::require_signature<F2,TYPE1,TYPE2>>
LoKi::Compose<TYPE,TYPE1,TYPE2> operator >> ( F1&&  fun1 , F2&& fun2 )
{ return { std::forward<F1>(fun1) , std::forward<F2>(fun2) } ; }
// ============================================================================
/** the streamer for two predicates is "logical AND" for the predicates
 *
 *  @code
 *
 *   const Functor<TYPE,bool>& pred1 ;
 *   const Functor<TYPE,bool>& pred2 ;
 *
 *   SEQUENCE input = ... ;
 *
 *   SEQUENCE output1 = input >>   pred1 >> pred2   ;
 *
 *   /// the same, but a bit "old-fashined":
 *   SEQUENCE output2 = input >> ( pred1 && pred2 ) ;
 *
 *  @endcode
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename F1, typename F2,
          typename TYPE = LoKi::details::type1_t<F1,F2>,
          typename = LoKi::details::require_signature<F1,TYPE,bool>,
          typename = LoKi::details::require_signature<F2,TYPE,bool>>
auto operator >> ( F1&& cut1 , F2&& cut2 )
{ return std::forward<F1>(cut1) && std::forward<F2>(cut2); }
// ============================================================================
/** evaluate the function through stremer operation:
 *
 *  @code
 *
 *  typedef Functor<Type1,Type2>             Func ;
 *
 *  const Func&  fun   = ... ;
 *  const Type1& input = ... ;
 *
 *  Type2 output = input >> fun ;
 *
 *  @endcode
 *
 *  @see LoKi::apply
 *
 *  @param a argument
 *  @param o the functor
 *  @return the result of the functor evaluation
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <typename TYPE,typename TYPE2>
TYPE2 operator >> ( typename LoKi::Functor<TYPE,TYPE2>::argument a ,
                    const LoKi::Functor<TYPE,TYPE2>&             o )
{ return LoKi::apply ( o , a ) ; }
// ============================================================================
/** evaluate the vector function  ("map")
 *
 *  @code
 *
 *  typedef Functor<Type1,double>             Func ;
 *
 *  const std::vector<Type1> input = ...
 *
 *  std::vector<double> output = input >> fun ;
 *
 *  @endcode
 *
 *  @see LoKi::apply
 *
 *  @param input vector in input arguments
 *  @param funct the functor
 *  @return the vector of results
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <class TYPEI, class TYPE>
std::vector<double> operator >> ( const std::vector<TYPEI>&         input ,
                                  const LoKi::Functor<TYPE,double>& funct )
{
  std::vector<double> out ;
  out.reserve ( input.size() ) ;
  LoKi::apply( input.begin(), input.end(), funct, std::back_inserter( out ) );
  return out ;
}

// ============================================================================
/** evaluate the vector function with vector argument
 *
 *  @code
 *
 *  typedef Functor<std::vector<Type1>,Type2>             Func ;
 *
 *  const std::vector<Type3> input = ...
 *
 *  Type2 output = input >> fun ;
 *
 *  @endcode
 *
 *  @see LoKi::apply
 *
 *  @param input vector in input arguments
 *  @param funct the functor
 *  @return the vector of results
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <class TYPEI, class TYPE>
double operator >> ( const std::vector<TYPEI>&                      input ,
                     const LoKi::Functor<std::vector<TYPE>,double>& funct )
{
  return LoKi::apply ( funct , input );
}
// ============================================================================
/** evaluate/filter the vector function/predicate
 *
 *  @code
 *
 *  typedef Functor<Type1,bool>            Func ;
 *
 *  const std::vector<Type1> input = ...
 *
 *  std::vector<Type1> output = input >> fun ;
 *
 *  @endcode
 *
 *  @see LoKi::apply
 *
 *  @param input vector in input arguments
 *  @param pred the predicate to be evaluated
 *  @return the filtered inputs
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <class TYPEI, class TYPE>
std::vector<TYPEI> operator >> ( const std::vector<TYPEI>&       input ,
                                 const LoKi::Functor<TYPE,bool>& pred  )
{
  //
  std::vector<TYPEI> out ; out.reserve ( input.size() ) ;
  //
  LoKi::apply_filter
    ( input.begin() , input.end() , pred , std::back_inserter ( out ) ) ;
  //
  return out ;
}

template <class TYPEI, class TYPE>
std::vector<TYPEI> operator >> ( std::vector<TYPEI>&&       input ,
                                 const LoKi::Functor<TYPE,bool>& pred  )
{
  input.erase(std::remove_if( input.begin(), input.end(),
                              [&](const auto& i) { return !LoKi::apply(pred,i); } ),
              input.end());
  return input;
}
// ============================================================================
/** evaluate the vector function with scalar argument through
 *  the construction of the intermediate vector of length 1
 *
 *  @code
 *
 *  const Functor<std::vector<TYPE>, TYPE2> & functor = ... ;
 *
 *  const TYPE& object = ... ;
 *
 *  TYPE2 result = object >> functor ;
 *
 *  @endcode
 *
 *  @see LoKi::apply
 *
 *  @param a vector in input arguments
 *  @param f the predicate to be evaluated
 *  @return the filtered inputs
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2001-01-23
 */
template <class TYPE,class TYPE2>
TYPE2 operator>> ( typename LoKi::Functor<TYPE,TYPE2>::argument  a ,
                   const LoKi::Functor<std::vector<TYPE>,TYPE2>& f )
{
  return LoKi::apply ( f , a ) ;
}
// ============================================================================
/// use "light" range as input vector
template <class CONTAINER, class OUTPUT>
OUTPUT operator>> ( const Gaudi::Range_<CONTAINER>&        a ,
                    const LoKi::Functor<CONTAINER,OUTPUT>& o )
{
  return LoKi::apply ( o , a ) ;
}
// ============================================================================
/// use "light" range as input vector
template <class CONTAINER, class OUTPUT>
OUTPUT operator>> ( const Gaudi::NamedRange_<CONTAINER>&   a ,
                    const LoKi::Functor<CONTAINER,OUTPUT>& o )
{
  return LoKi::apply ( o , a ) ;
}
// ============================================================================
/** "source" : produce the sequence from nothing
 *
 *   @code
 *
 *    const LoKi::Functor<void, std::vector<TYPE> > source ;
 *
 *    std::vector<TYPE> dest ;
 *
 *    source >> dest ;
 *
 *   @endcode
 *
 *  The concept belongs to Gerhard Raven.
 *
 *   @param  source the source
 *   @param  dest   the destination
 *   @return the destination
 *
 *   @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *   @date 2001-01-23
 */
template <class TYPE>
std::vector<TYPE>&
operator >> ( const LoKi::Functor<void,std::vector<TYPE> >& source ,
              std::vector<TYPE>&                            dest   )
{
  dest = source.evaluate() ;
  return dest ;
}
// ============================================================================
/** "source": produce the sequence from nothing
 *
 *   @code
 *
 *    const LoKi::Functor<void, std::vector<TYPE*> > source ;
 *
 *    std::vector<const TYPE*> dest ;
 *
 *    source >> dest ;
 *
 *   @endcode
 *
 *  The concept belongs to Gerhard Raven.
 *
 *   @param  source the source
 *   @param  dest   the destination
 *   @return the destination
 *
 *   @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *   @date 2001-01-23
 */
template <class TYPE>
const std::vector<const TYPE*>&
operator >> ( const LoKi::Functor<void,std::vector<TYPE*> >& source ,
              std::vector<const TYPE*>&                      dest   )
{
  typedef std::vector<const TYPE*> _CVector ;
  typedef std::vector<      TYPE*> _NVector ;
  //
  const _NVector& aux = source.evaluate() ;
  const _CVector* out = reinterpret_cast<const _CVector*>( &aux ) ;
  //
  dest = *out ;
  //
  return dest ;
}
// ============================================================================
/** "source" : produce the sequence from nothing
 *
 *   @code
 *
 *    const LoKi::Functor<void, std::vector<const TYPE*> > source ;
 *
 *    std::vector<TYPE*> dest ;
 *
 *    source >> dest ;
 *
 *   @endcode
 *
 *  The concept belongs to Gerhard Raven.
 *
 *   @param  source the source
 *   @param  dest   the destination
 *   @return the destination
 *
 *   @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *   @date 2001-01-23
 */
template <class TYPE>
std::vector<TYPE*>&
operator >> ( const LoKi::Functor<void,std::vector<const TYPE*> >& source ,
              std::vector<TYPE*>&                                  dest   )
{
  typedef std::vector<const TYPE*> _CVector ;
  typedef std::vector<      TYPE*> _NVector ;
  //
  const _CVector& aux = source.evaluate() ;
  const auto* out = reinterpret_cast<const _NVector*>( &aux ) ;
  //
  dest = *out ;
  //
  return dest ;
}
// ============================================================================
// pipe with const-away cast
// ============================================================================
template <class TYPE>
std::vector<TYPE*>
operator>> ( const std::vector<TYPE*>&                                                 input ,
             const LoKi::Functor<std::vector<const TYPE*>, std::vector<const TYPE*> >& pipe  )
{
  return LoKi::apply ( pipe , input ) ;
}
// ============================================================================
/** chaining/composition of the vector-function and the scalar one
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2007-11-29
 */
//template <class TYPE,class TYPE1, class TYPE2>
//inline
//inline LoKi::Compose<TYPE,std::vector<TYPE1>,std::vector<TYPE2> >
//operator>> ( const LoKi::Functor<TYPE,std::vector<TYPE1> >& fun1 ,
//             const LoKi::Functor<TYPE1,TYPE2>&              fun2 )
//{
//  return LoKi::Compose<TYPE,std::vector<TYPE1>,std::vector<TYPE2> >
//    ( fun1 , LoKi::Yields<TYPE1,TYPE2,TYPE1> ( fun2 ) ) ;
//}


// ============================================================================
// source with the gate
// ============================================================================
template <typename F1, typename F2,
          typename TYPE  = typename LoKi::details::type1_t<F1>,
          typename TYPE2 = typename LoKi::details::type2_t<F1>::value_type,
          typename = LoKi::details::require_signature<F1,TYPE,std::vector<TYPE2>>,
          typename = LoKi::details::require_signature<F2,void,bool>>
LoKi::FunctorFromFunctor< TYPE , std::vector<TYPE2> >
operator>> ( F1&& source , F2&& gate   )
{
  return std::forward<F1>(source) >> LoKi::gate<TYPE2>( std::forward<F2>(gate) ) ;
}

// ============================================================================
// dumping to the stream
// ============================================================================
template <typename F,
          typename TYPE = typename LoKi::details::type1_t<F>,
          typename TYPE2 = typename LoKi::details::type2_t<F>::value_type,
          typename = LoKi::details::require_signature<F,TYPE,std::vector<TYPE2>>>
LoKi::FunctorFromFunctor< TYPE, std::vector<TYPE2> >
operator>> ( F&& pipe, std::ostream& stream )
{
  return std::forward<F>(pipe) >> LoKi::Functors::Dump_<TYPE2>( stream ) ;
}
// ============================================================================
// dumping to the stream
// ============================================================================
template <typename F,
          typename TYPE = typename LoKi::details::type1_t<F>,
          typename TYPE2 = typename LoKi::details::type2_t<F>::value_type,
          typename = LoKi::details::require_signature<F,TYPE,std::vector<TYPE2>>>
LoKi::FunctorFromFunctor< TYPE , std::vector<TYPE2> >
operator>> ( F&& pipe , const LoKi::Dump& dump  )
{
  return std::forward<F>(pipe) >> LoKi::Functors::Dump_<TYPE2>( dump ) ;
}

// ============================================================================
// The END
// ============================================================================
#endif // LOKI_STREAMERS_H
