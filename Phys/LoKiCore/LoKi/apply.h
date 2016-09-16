// ============================================================================
#ifndef LOKI_APPLY_H
#define LOKI_APPLY_H 1
// ============================================================================
// Include files
// ============================================================================
// STD& STL
// ============================================================================
#include <vector>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Range.h"
#include "GaudiKernel/NamedRange.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
#include "LoKi/BasicFunctors.h"
#include "LoKi/Const.h"
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** the direct use the functor
   *  essentially no gain is expected, but needed to terminate the
   *  template argument resolution
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE,TYPE2>& functor = ... ;
   *
   *   const TYPE& object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <typename TYPE,typename TYPE2, typename ... Args>
  inline typename LoKi::Functor<TYPE,TYPE2>::result_type
  apply  ( const LoKi::Functor<TYPE,TYPE2>& f, Args&&... a )
  { return f ( std::forward<Args>(a)... ) ; }
  // ==========================================================================
  /** the direct use the function (save one virtual call)
   *  For some (rare?) cases it could be a bit more efficient.
   *
   *  @code
   *
   *   const LoKi::FunctorFrinFunctor<TYPE,TYPE2>& functor = ... ;
   *
   *   TYPE* object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<TYPE,TYPE2>::result_type
  apply  ( const LoKi::FunctorFromFunctor<TYPE,TYPE2>&  f ,
           typename LoKi::Functor<TYPE,TYPE2>::argument a )
  { return apply ( f.fun() , a ) ; }
  // ==========================================================================
  /** Adapter: use the functor through the dereferencing of the argument
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE,TYPE2>& functor = ... ;
   *
   *   TYPE* object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<TYPE,TYPE2>::result_type
  apply  ( const LoKi::Functor<TYPE,TYPE2>&              f ,
           typename LoKi::Functor<TYPE*,TYPE2>::argument a )
  { return apply ( f , *a ) ; }
  // ==========================================================================
  /** Adapter: use the functor through dereferencing of the argument
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE,TYPE2>& functor = ... ;
   *
   *   const TYPE* object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<TYPE,TYPE2>::result_type
  apply  ( const LoKi::Functor<TYPE,TYPE2>&                    f ,
           typename LoKi::Functor<const TYPE*,TYPE2>::argument a )
  { return apply ( f , *a ) ; }
  // ==========================================================================
  /** Adapter: use the functor through adressing of the argument
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE*,TYPE2>& functor = ... ;
   *
   *   TYPE& object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<TYPE*,TYPE2>::result_type
  apply  ( const LoKi::Functor<TYPE*,TYPE2>&            f ,
           typename LoKi::Functor<TYPE,TYPE2>::argument a )
  { return apply ( f , &a ) ; }
  // ==========================================================================
  /** Adapter: use the functor through adressing of the argument
   *
   *  @code
   *
   *   const LoKi::Functor<const TYPE*,TYPE2>& functor = ... ;
   *
   *   const TYPE& object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<const TYPE*,TYPE2>::result_type
  apply  ( const LoKi::Functor<const TYPE*,TYPE2>&      f ,
           typename LoKi::Functor<TYPE,TYPE2>::argument a )
  { return apply ( f , &a ) ; }
  // ==========================================================================
  /** Adapter: use the vector-functor for scalar argument
   *
   *  @code
   *
   *   const LoKi::Functor<const TYPE*,TYPE2>& functor = ... ;
   *
   *   const TYPE& object = ... ;
   *
   *   TYPE2 result = LoKi::apply ( functor , object ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class TYPE,class TYPE2>
  inline typename LoKi::Functor<std::vector<TYPE>,TYPE2>::result_type
  apply  ( const LoKi::Functor<std::vector<TYPE>,TYPE2>& f ,
           typename LoKi::Functor<TYPE,TYPE2>::argument  a )
  { return apply ( f , std::vector<TYPE> ( 1 , a ) ) ; }
  // ==========================================================================
  /** Adapter: "vector-use" of the scalar function
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE,TYPE2>& functor = ... ;
   *
   *   SEQUENCE objects = ... ;
   *   std::vector<TYPE2> results ;
   *   results.reserve ( objects.end() - objects.begin() ) ;
   *
   *   LoKi::apply
   *         ( objects.begin () ,
   *           objects.end   () ,
   *           functor          ,
   *           std::back_inserter ( results ) ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class INPUT, class TYPE, class TYPE2, class OUTPUT>
  inline OUTPUT
  apply ( INPUT                            first ,
          INPUT                            last  ,
          const LoKi::Functor<TYPE,TYPE2>& fun   ,
          OUTPUT                           out   )
  {
    using arg_t = decltype(*first);
    std::transform(first,last,out,[&](arg_t arg) { return apply(fun,arg); } );
    return out ;
  }
  // ==========================================================================
  /** Adapter: "vector-use" of the scalar function and ignore the result
   *
   *  @code
   *
   *   const LoKi::Functor<TYPE,TYPE2>& functor = ... ;
   *
   *   SEQUENCE objects = ... ;
   *
   *   LoKi::apply
   *         ( objects.begin () ,
   *           objects.end   () ,
   *           functor          ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class INPUT, class TYPE, class TYPE2>
  inline void
  apply ( INPUT                            first ,
          INPUT                            last  ,
          const LoKi::Functor<TYPE,TYPE2>& fun   )
  {
    using arg_t = decltype(*first);
    std::for_each(first,last,[&](arg_t arg) { apply(fun,arg); } );
  }
  // ==========================================================================
  /** Adapter: "vector-use" of the scalar predicate/filter
   *  @code
   *
   *   const LoKi::Functor<TYPE,bool>& functor = ... ;
   *
   *   SEQUENCE objects = ... ;
   *   SEQUENCE results ;
   *   results.reserve ( objects.end() - objects.begin() ) ;
   *
   *   LoKi::apply_filter
   *         ( objects.begin () ,
   *           objects.end   () ,
   *           functor          ,
   *           std:back_inserter ( results ) ) ;
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2001-01-23
   */
  template <class INPUT, class TYPE, class OUTPUT>
  inline OUTPUT
  apply_filter ( INPUT                            first ,
                 INPUT                            last  ,
                 const LoKi::Functor<TYPE,bool>&  fun   ,
                 OUTPUT                           out   )
  {
    using arg_t = decltype(*first);
    std::copy_if( first, last, out,
                  [&](arg_t arg) { return LoKi::apply(fun,arg); } );
    return out ;
  }
  // ==========================================================================
  template <class INPUT, class CONTAINER, class OUTPUT>
  inline OUTPUT
  apply ( const LoKi::Functor<CONTAINER,OUTPUT>& o     ,
          INPUT                                  first ,
          INPUT                                  last  )
  {
    return LoKi::apply ( o , CONTAINER{ first, last } ) ;
  }
  // ==========================================================================
  template <class TYPE>
  inline
  std::vector<TYPE*>
  apply
  ( const LoKi::Functor<std::vector<const TYPE*>,std::vector<const TYPE*> >& pipe  ,
    const std::vector<TYPE*>&                                                input )
  {
    typedef  std::vector<typename LoKi::Const<TYPE>::Value*>  _CVector ;
    typedef  std::vector<                     TYPE*        >  _NVector ;
    // adapt the argument:
    const _CVector* _vct_ = reinterpret_cast<const _CVector*> ( &input ) ;
    // use functor
    const _CVector& _out  = pipe.evaluate ( *_vct_) ;
    // adapt result:
    const _NVector* _out_ = reinterpret_cast<const _NVector*>  ( &_out  ) ;
    //
    return *_out_;
  }
  // ==============================================================================
  template <class CONTAINER, class TYPEI, class TYPE2>
  inline
  typename LoKi::Functor<std::vector<TYPEI>,TYPE2>::result_type
  apply ( const LoKi::Functor<std::vector<TYPEI>,TYPE2>& o ,
          const Gaudi::Range_<CONTAINER>&                a )
  {
    return LoKi::apply ( o , a.begin() , a.end() ) ;
  }
  // ==========================================================================
  template <class CONTAINER, class TYPEI, class TYPE2>
  inline
  typename LoKi::Functor<std::vector<TYPEI>,TYPE2>::result_type
  apply ( const LoKi::Functor<std::vector<TYPEI>,TYPE2>& o ,
          const Gaudi::NamedRange_<CONTAINER>&           a )
  {
    return LoKi::apply ( o , a.begin() , a.end() ) ;
  }
  // ==========================================================================
  /** @struct Apply
   *
   *  The helper function to allow (adapt) "polymorphic arguments"
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-02
   */

  namespace details {
      template <typename F> struct Apply final {
          F f;
          template <typename ... Args>
          auto operator()(Args&& ... args) const
          -> decltype( LoKi::apply(f, std::forward<Args>(args)...) )
          { return LoKi::apply(f, std::forward<Args>(args)...); }
      };
  }
  template <typename F, typename = typename details::is_functor<F>>
  details::Apply<F> Apply(F&& fun) { return {std::forward<F>(fun)}; }
  // C++14 (gcc>=5): 
  // auto Apply = [](auto&& f) { return [f=decltype(f)(f)](auto&& ... arg) { return LoKi::apply(f,std::forward<decltype(arg)>(arg)...); } }

  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_APPLY_H
// ============================================================================
