#ifndef LOKI_ALGS_H
#define LOKI_ALGS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <utility>
#include <functional>
#include <set>
#include <vector>
#include <algorithm>
#include <numeric>
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
 *  @date 2006-02-10
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace  LoKi::Algs LoKi/Algs.h
   *
   *  Helper namespace with auxillary
   *  functions/algorithms of the general interest
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2006-02-10
   */
  namespace  Algs
  {
    // ========================================================================
    /** The trivial algorithm which accumulated the value
     *  of the function over the sequence
     *
     *  @param first    'begin'-iterator for the sequence
     *  @param last      'end'-iterator for the sequence
     *  @param functor   function to be evaluated
     *  @param result    the initial value
     *  @param binop     the accumulation predicate
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-10
     */
    template <class ITER,class RESULT,class FUNCTOR,class OPERATION>
    inline RESULT accumulate( ITER           first     ,
                              ITER           last      ,
                              const FUNCTOR& functor   ,
                              RESULT         result    ,
                              OPERATION      binop     )
    {
      using reference = typename std::iterator_traits<ITER>::reference;
      return std::accumulate( first,last, result,
                              [&](RESULT r, reference o)
                              { return binop(r, functor(o)); } );
    }
    // ========================================================================
    /** The trivial algorithm which accumulates the value
     *  of the function over the sequence for elements which
     *  satisfy certain criteria
     *
     *  @param first    'begin'-iterator for the sequence
     *  @param last      'end'-iterator for the sequence
     *  @param functor   function to be evaluated
     *  @param predicate the seelection criteria
     *  @param result    the initial value
     *  @param binop     the accumulation predicate
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-10
     */
    template <class ITER,class RESULT,class FUNCTOR,
              class PREDICATE,class OPERATION>
    inline RESULT accumulate( ITER             first     ,
                              ITER             last      ,
                              const FUNCTOR&   functor   ,
                              const PREDICATE& predicate ,
                              RESULT           result    ,
                              OPERATION        binop     )
    {
      // std::iterator_traits<ITER>::reference doesn't work here ;-(
      using value_type = typename std::iterator_traits<ITER>::value_type;
      using arg_t      = typename std::conditional< std::is_pointer<value_type>::value,
                                                    value_type,
                                                    typename std::iterator_traits<ITER>::reference>::type;
      return std::accumulate( first, last, result,
                              [&](RESULT r, arg_t o)
                              { return predicate(o) ? binop(r,functor(o)) : r; } );
    }
    // ========================================================================
    /** The (no longer) missing "copy_if" algorithm from STL
     *
     *  @param first 'begin'-iterator for the input sequence of objects
     *  @param last  'end'-iterator for the input sequence of objects
     *  @param output the position fo output iterator
     *  @param cut    the condition
     *  @return the updated position of output itrator
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-02-20
     */
    template <class OBJECT,class OUTPUT,class PREDICATE>
    inline OUTPUT copy_if( OBJECT    first  ,
                           OBJECT    last   ,
                           OUTPUT    output ,
                           PREDICATE cut    )
    {
      return std::copy_if(first,last,output,std::ref(cut));
    }
    // ========================================================================
    /// a bit modified version of std::count_if
    template <class OBJECT, class PREDICATE>
    inline std::size_t count_if( OBJECT            first ,
                                 OBJECT            last  ,
                                 const PREDICATE&  cuts  )
    {
      return std::count_if( first, last, std::cref(cuts) );
    }
    // ========================================================================
    /// a bit modified version of std::find_if
    template <class OBJECT, class PREDICATE>
    inline OBJECT find_if( OBJECT            first ,
                           OBJECT            last  ,
                           const PREDICATE&  cuts  )
    {
      return std::find_if( first,last, std::cref(cuts) );
    }
    // ========================================================================
    /// useful shortcut
    template <class OBJECT,class PREDICATE>
    inline bool found( OBJECT            first ,
                       OBJECT            last  ,
                       const PREDICATE&  cuts  )
    {
      return last != LoKi::Algs::find_if ( first , last , cuts ) ;
    }
    // ========================================================================
    /** helper function to find "extremum" (minimum or maximum)
     *  of the certain function over the sequence of values
     *  @see LoKi::select_min
     *  @see LoKi::select_max
     *  @author Vanya BELYAEV  ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template<class ITER,class FUNCTION,class BINOP>
    inline auto extremum( ITER           first ,
                          ITER           last  ,
                          const FUNCTION&  fun   ,
                          BINOP            binop )
    -> std::pair<ITER,decltype(fun(*first))>
    {
      // empty sequence?
      if ( first == last ) { return { last , 0 } ; }
      // store the first result:
      auto result = first        ;
      auto value  = fun(*result) ;
      for ( ; first != last ; ++first ) {
        auto tmp =  fun(*first) ;
        if ( binop ( tmp , value )  ) {
          result = first ;
          value  = std::move(tmp)   ;
        }
      }
      return { result , value } ;     // RETURN
    }
    // ========================================================================
    /** helper function to find "extremum" (minimum or maximum)
     *  of the certain function over the sequence of values
     *  @see LoKi::select_min
     *  @see LoKi::select_max
     *  @author Vanya BELYAEV  ibelyaev@physics.syr.edu
     *  @date 2007-08-17
     */
    template<class OBJECT,class FUNCTION,class PREDICATE,class BINOP>
    inline auto extremum( OBJECT           first ,
                          OBJECT           last  ,
                          const FUNCTION&  fun   ,
                          const PREDICATE& cut   ,
                          BINOP            binop )
    -> std::pair<OBJECT,decltype(fun(*first))>
    {
      using result_type = decltype(fun(*first));
      auto result = last ;
      result_type value = 0    ;
      first = std::find_if(first,last,std::cref(cut));
      if (first!=last) {
        // evaluate the  function
        value  = fun( *first ) ;
        result = first++;
        for (;;) {  // infinite loops are fast: https://www.youtube.com/watch?v=AxnotgLql0k
          first = std::find_if(first,last,std::cref(cut));
          if (first==last) break;                        // BREAK
          // evaluate the  function
          auto tmp = fun( *first ) ;
          // compare it!
          if ( binop ( tmp , value ) ) {
            result = first ;
            value  = std::move(tmp)   ;
          }
        }
      }
      // return the pair of result and  value
      return { result , value } ;    // RETURN
    }
    // ========================================================================
    /** select element from the sequence with maximal value of
     *  given function
     *
     *  It is just an extension of the STL <tt>std::max_element</tt>
     *  standard algorithm with additional conditions
     *
     *  @code
     *  SEQUENCE prts = ... ;
     *  SEQUENCE::iterator particle =
     *              select_max( prts.begin() ,
     *                          prts.end()   ,
     *                          PT           ,
     *                          P > 10 * GeV );
     *  @endcode
     *
     *  Where <tt>SEQUENCE</tt> could be any container or container-like
     *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
     *  This example illustrates the selection of particle with maximal
     *  transverse momentum  from "container" <tt>prts</tt>. Only
     *  particles with momentum in excess of 10 GeV are considered.
     *
     *  @see PT
     *  @see LoKi::Function
     *  @param first  'begin' iterator of the sequence
     *  @param last   'end'   iterator of the sequence
     *  @param fun    function
     *  @param cut    predicate which need to be satisfied
     *  @return iterator for the element with minimal value of function
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2005-03-09
     */
    template<class OBJECT,class FUNCTION,class PREDICATE>
    inline OBJECT select_min( OBJECT           first ,
                              OBJECT           last  ,
                              const FUNCTION&  fun   ,
                              const PREDICATE& cut   )
    {
      return LoKi::Algs::extremum
        ( first , last , fun , cut , std::less{} ).first  ;
    }
    // ========================================================================
    /** select element from the sequence with maximal value of
     *  given function
     *
     *  It is just an extension of the STL <tt>std::max_element</tt>
     *  standard algorithm with additional conditions
     *
     *  @code
     *  SEQUENCE prts = ... ;
     *  SEQUENCE::iterator particle =
     *              select_max( prts.begin() ,
     *                          prts.end()   ,
     *                          PT           ,
     *                          P > 10 * GeV );
     *  @endcode
     *
     *  Where <tt>SEQUENCE</tt> could be any container or container-like
     *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
     *  This example illustrates the selection of particle with maximal
     *  transverse momentum  from "container" <tt>prts</tt>. Only
     *  particles with momentum in excess of 10 GeV are considered.
     *
     *  @see PT
     *  @see LoKi::Function
     *  @param first  'begin' iterator of the sequence
     *  @param last   'end'   iterator of the sequence
     *  @param fun    function
     *  @param cut    predicate which need to be satisfied
     *  @return iterator for the element with minimal value of function
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2005-03-09
     */
    template<class OBJECT,class FUNCTION,class PREDICATE>
    inline OBJECT select_max( OBJECT           first ,
                              OBJECT           last  ,
                              const FUNCTION&  fun   ,
                              const PREDICATE& cut   )
    {
      return LoKi::Algs::extremum
        ( first , last , fun , cut , std::greater{} ).first ;
    }
    // ========================================================================
    /** select element from the sequence with maximal value of
     *  given function
     *
     *  It is just an extension of the STL <tt>std::max_element</tt>
     *  standard algorithm with additional conditions
     *
     *  @code
     *  SEQUENCE prts = ... ;
     *  SEQUENCE::iterator particle =
     *              select_max( prts.begin() ,
     *                          prts.end()   ,
     *                          PT           ) ;
     *  @endcode
     *
     *  Where <tt>SEQUENCE</tt> could be any container or container-like
     *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
     *  This example illustrates the selection of particle with maximal
     *  transverse momentum  from "container" <tt>prts</tt>.
     *
     *  @see PT
     *  @see LoKi::Function
     *  @param first  'begin' iterator of the sequence
     *  @param last   'end'   iterator of the sequence
     *  @param fun    function
     *  @return iterator for the element with minimal value of function
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2005-03-09
     */
    template<class OBJECT,class FUNCTION>
    inline OBJECT select_min( OBJECT           first ,
                              OBJECT           last  ,
                              const FUNCTION&  fun   )
    {
      return LoKi::Algs::extremum
        ( first , last , fun , std::less{} ).first ;
    }
    // ========================================================================
    /** select element form the sequence with maximal value of
     *  given function
     *
     *  It is just an extension of the STL <tt>std::max_element</tt>
     *  standard algorithm with additional conditions
     *
     *  @code
     *  SEQUENCE prts = ... ;
     *  SEQUENCE::iterator particle =
     *              select_max( prts.begin() ,
     *                          prts.end()   ,
     *                          PT           );
     *  @endcode
     *
     *  Where <tt>SEQUENCE</tt> could be any container or container-like
     *  object (like e.g. <tt>LoKi::Range</tt>) of <tt>const Particle*</tt>.
     *  This example illustrates the selection of particle with maximal
     *  transverse momentum  from "container" <tt>prts</tt>.
     *
     *  @see PT
     *  @see LoKi::Function
     *  @param first  'begin' iterator of the sequence
     *  @param last   'end'   iterator of the sequence
     *  @param fun    function
     *  @return iterator for the element with minimal value of function
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2005-03-09
     */
    template<class OBJECT,class FUNCTION>
    inline OBJECT select_max( OBJECT           first ,
                              OBJECT           last  ,
                              const FUNCTION&  fun   )
    {
      return LoKi::Algs::extremum
        ( first , last , fun , std::greater{} ).first ;
    }
    // ========================================================================
    /** very simple algorithm for minimum value of the function over the
     *  sequence
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-07-20
     */
    template <class OBJECT, class FUNCTION,class RESULT>
    inline RESULT min_value( OBJECT           first  ,
                             OBJECT           last   ,
                             const FUNCTION&  fun    ,
                             RESULT           result )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, result,
                              [&](RESULT r, arg_t v)
                              { return std::min( r, fun(v) ); } );
    }
    // ========================================================================
    /** very simple algorithm for maximum value of the function over the
     *  sequence
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-07-20
     */
    template <class OBJECT, class FUNCTION, class PREDICATE, class RESULT>
    inline RESULT min_value( OBJECT           first  ,
                             OBJECT           last   ,
                             const FUNCTION&  fun    ,
                             const PREDICATE& cut    ,
                             RESULT           result )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, result,
                              [&](RESULT r, arg_t v)
                              { return cut(v) ? std::min( r, fun(v) ) : r ; } );
    }
    // ========================================================================
    /** very simple algorithm for maximum value of the function over the
     *  sequence
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-07-20
     */
    template <class OBJECT, class FUNCTION,class RESULT>
    inline RESULT max_value( OBJECT           first  ,
                             OBJECT           last   ,
                             const FUNCTION&  fun    ,
                             RESULT           result )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, result,
                              [&](RESULT r, arg_t v)
                              { return std::max( r, fun(v) ); } );
    }
    // ========================================================================
    /** very simple algorithm for maximum value of the function over the
     *  sequence
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-07-20
     */
    template <class OBJECT, class FUNCTION, class PREDICATE, class RESULT>
    inline RESULT max_value( OBJECT           first  ,
                             OBJECT           last   ,
                             const FUNCTION&  fun    ,
                             const PREDICATE& cut    ,
                             RESULT           result )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, result,
                              [&](RESULT r, arg_t v)
                              { return cut(v) ? std::max(r,fun(v)) : r; } );
    }
    // ========================================================================
    // FoundN-family of algorithms
    // ========================================================================
    /** @typedef INDICES
     *  container to keep "exceptional" indices"
     */
    typedef std::set<std::ptrdiff_t> INDICES  ;
    // ========================================================================
    template <class OBJECT,class PREDICATE1,class PREDICATE2>
    inline bool _found_2( OBJECT            first                ,
                          OBJECT            last                 ,
                          const PREDICATE1& cut1                 ,
                          const PREDICATE2& cut2                 ,
                          const INDICES&    except  = INDICES()  )
    {
      const std::ptrdiff_t num = last - first ;
      if  ( num < 2 ) { return false ; }                      // RETURN
      for ( std::ptrdiff_t i = 0 ; i  < num ; ++i )
      {
        if ( except.end() != except.find ( i ) )  { continue ; }
        if ( !cut1 ( * ( first + i ) ) )                      { continue  ; }
        for  ( std::ptrdiff_t j = 0 ; j < num ; ++j )
        {
          if ( except.end() != except.find ( j ) ) { continue ; }
          if  ( i != j &&  cut2 ( *(first + j ) ) ) { return true ; }
        }
      }
      return false ;
    }
    // ========================================================================
    /** simple algorithm to check the presence of at least
     *  two *different* objects in the sequence which satisfy
     *  two predicates
     *  @param first 'begin'-iterator for the sequence
     *  @param last  'end'-iterator for the sequence
     *  @param cut1 the first predicate
     *  @param cut2 the second predicate
     *  @return true if there are two elements
     */
    template <class OBJECT, class PREDICATE1, class PREDICATE2>
    inline bool found_2( OBJECT            first ,
                         OBJECT            last  ,
                         const PREDICATE1& cut1  ,
                         const PREDICATE2& cut2  )
    {
      return _found_2 ( first , last , cut1 , cut2 ) ;
    }
    // ========================================================================
    template <class OBJECT, class PREDICATE1, class PREDICATE2, class PREDICATE3>
    inline bool _found_3( OBJECT            first                ,
                          OBJECT            last                 ,
                          const PREDICATE1& cut1                 ,
                          const PREDICATE2& cut2                 ,
                          const PREDICATE3& cut3                 ,
                          const INDICES&    except  = INDICES()  )
    {
      //
      if  ( last - first  < 3 ) { return false ; }                      // RETURN
      //
      for ( auto f1 = first ; f1 != last ; ++f1 )
      {
        const std::ptrdiff_t index = f1 - first ;
        if  ( except.end() != except.find ( index ) ) { continue ; }
        INDICES updated ( except ) ;
        updated.insert ( index ) ;
        if       ( cut1 ( *f1 ) &&
                   _found_2 ( first , last , cut2 , cut3 , updated ) ) { return true ; }
        else if  ( cut2 ( *f1 ) &&
                   _found_2 ( first , last , cut1 , cut3 , updated ) ) { return true ; }
        else if  ( cut3 ( *f1 ) &&
                   _found_2 ( first , last , cut1 , cut2 , updated ) ) { return true ; }
      }
      return false ;
    }
    // ========================================================================
    /** simple algorithm to check the presence of at least
     *  three *different* objects in the sequence which satisfy
     *  three  predicates
     *  @param first 'begin'-iterator for the sequence
     *  @param last  'end'-iterator for the sequence
     *  @param cut1 the first predicate
     *  @param cut2 the second predicate
     *  @param cut3 the third predicate
     *  @return true if there are two elements
     */
    template <class OBJECT, class PREDICATE1, class PREDICATE2, class PREDICATE3>
    inline bool found_3( OBJECT            first ,
                         OBJECT            last  ,
                         const PREDICATE1& cut1  ,
                         const PREDICATE2& cut2  ,
                         const PREDICATE3& cut3  )
    {
      return _found_3 ( first , last , cut1 , cut2 , cut3 ) ;
    }
    // ========================================================================
    template <class OBJECT,
              class PREDICATE1, class PREDICATE2,
              class PREDICATE3, class PREDICATE4>
    inline bool _found_4( OBJECT            first  ,
                          OBJECT            last   ,
                          const PREDICATE1& cut1   ,
                          const PREDICATE2& cut2   ,
                          const PREDICATE3& cut3   ,
                          const PREDICATE4& cut4   ,
                          const INDICES&    except  = INDICES()  )
    {
      //
      if  ( last - first  < 4 ) { return false ; }                      // RETURN
      //
      for ( auto f1 = first ; f1 != last ; ++f1 )
      {
        //
        const std::ptrdiff_t index = f1 - first ;
        if  ( except.end() != except.find ( index ) ) { continue ; }
        INDICES updated ( except ) ;
        updated.insert ( index ) ;
        //
        if       ( cut1 ( *f1 ) &&
                   _found_3 ( first , last , cut2 , cut3 , cut4 , updated ) ) { return true ; }
        else if  ( cut2 ( *f1 ) &&
                   _found_3 ( first , last , cut1 , cut3 , cut4 , updated ) ) { return true ; }
        else if  ( cut3 ( *f1 ) &&
                   _found_3 ( first , last , cut1 , cut2 , cut4 , updated ) ) { return true ; }
        else if  ( cut4 ( *f1 ) &&
                   _found_3 ( first , last , cut1 , cut2 , cut3 , updated ) ) { return true ; }
      }
      return false ;
    }
    // ========================================================================
    /** simple algorithm to check the presence of at least
     *  four *different* objects in the sequence which satisfy
     *  four  predicates
     *  @param first 'begin'-iterator for the sequence
     *  @param last  'end'-iterator for the sequence
     *  @param cut1 the first predicate
     *  @param cut2 the second predicate
     *  @param cut3 the third  predicate
     *  @param cut4 the fourth predicate
     *  @return true if there are two elements
     */
    template <class OBJECT,
              class PREDICATE1, class PREDICATE2,
              class PREDICATE3, class PREDICATE4>
    inline bool found_4 ( OBJECT            first ,
                          OBJECT            last  ,
                          const PREDICATE1& cut1  ,
                          const PREDICATE2& cut2  ,
                          const PREDICATE3& cut3  ,
                          const PREDICATE4& cut4  )
    {
      return _found_4 ( first , last , cut1 , cut2 , cut3 , cut4 ) ;
    }
    // ========================================================================
    template <class OBJECT,
              class PREDICATE1, class PREDICATE2,
              class PREDICATE3, class PREDICATE4, class PREDICATE5>
    inline bool _found_5
    ( OBJECT            first  ,
      OBJECT            last   ,
      const PREDICATE1& cut1   ,
      const PREDICATE2& cut2   ,
      const PREDICATE3& cut3   ,
      const PREDICATE4& cut4   ,
      const PREDICATE5& cut5   ,
      const INDICES&    except  = INDICES()  )
    {
      //
      if  ( last - first  < 4 ) { return false ; }                      // RETURN
      //
      for ( OBJECT f1 = first ; f1 != last ; ++f1 )
      {
        //
        const std::ptrdiff_t index = f1 - first ;
        if  ( except.end() != except.find ( index ) ) { continue ; }
        INDICES updated ( except ) ;
        updated.insert ( index ) ;
        //
        if       ( cut1 ( *f1 ) &&
                   _found_4 ( first , last , cut2 , cut3 , cut4 , cut5 , updated ) ) { return true ; }
        else if  ( cut2 ( *f1 ) &&
                   _found_4 ( first , last , cut1 , cut3 , cut4 , cut5 , updated ) ) { return true ; }
        else if  ( cut3 ( *f1 ) &&
                   _found_4 ( first , last , cut1 , cut2 , cut4 , cut5 , updated ) ) { return true ; }
        else if  ( cut4 ( *f1 ) &&
                   _found_4 ( first , last , cut1 , cut2 , cut3 , cut5 , updated ) ) { return true ; }
        else if  ( cut5 ( *f1 ) &&
                   _found_4 ( first , last , cut1 , cut2 , cut3 , cut4 , updated ) ) { return true ; }
      }
      return false ;
    }
    // ========================================================================
    /** simple algorithm to check the presence of at least
     *  five *different* objects in the sequence which satisfy
     *  five  predicates
     *  @param first 'begin'-iterator for the sequence
     *  @param last  'end'-iterator for the sequence
     *  @param cut1 the first predicate
     *  @param cut2 the second predicate
     *  @param cut3 the third  predicate
     *  @param cut4 the fourth predicate
     *  @param cut5 the fith predicate
     *  @return true if there are two elements
     */
    template <class OBJECT,
              class PREDICATE1, class PREDICATE2,
              class PREDICATE3, class PREDICATE4, class PREDICATE5>
    inline bool found_5
    ( OBJECT            first  ,
      OBJECT            last   ,
      const PREDICATE1& cut1   ,
      const PREDICATE2& cut2   ,
      const PREDICATE3& cut3   ,
      const PREDICATE4& cut4   ,
      const PREDICATE5& cut5   )
    {
      return _found_5
        ( first , last , cut1 , cut2 , cut3 , cut4 , cut5 ) ;
    }
    // ========================================================================
    template <class OBJECT, class PREDICATE>
    inline bool _found_N_
    ( OBJECT first                                 ,
      OBJECT last                                  ,
      const std::vector<const PREDICATE*>& cuts    ,
      const INDICES&                       except  )
    {
      //
      const std::size_t size = last - first ;
      if ( size  < cuts.size() + except.size() ) { return false ; }
      //
      for ( auto f = first ; last != f ; ++f )
      {
        const size_t index = f - first ;
        if ( except.end() != except.find ( index ) ) { continue ; }
        //
        auto icut = std::find_if( cuts.begin(), cuts.end(),
                                  [&](const PREDICATE* cut) { return (*cut)(*f); } );
        //
        if ( cuts.end() == icut ) continue;
        if ( 1 == cuts.size()  ) { return true ; }              // TRUE
        //
        INDICES updated { except } ;
        updated.insert ( index )   ;
        std::vector<const PREDICATE*> _cuts { cuts } ;
        _cuts.erase ( _cuts.begin() + ( icut - cuts.begin() ) ) ;
        //
        return _found_N_ ( first , last , _cuts , updated ) ;
      }
      return false ;
    }
    // =======================================================================
    template <class OBJECT, class PREDICATE>
    inline bool _found_N
    ( OBJECT first                          ,
      OBJECT last                           ,
      const std::vector<PREDICATE>& cuts    ,
      const INDICES&    except = INDICES()  )
    {
      const std::size_t size = last - first ;
      if ( size  < cuts.size() + except.size() ) { return false ; }
      //
      std::vector<const PREDICATE*> _cuts; _cuts.reserve ( cuts.size() ) ;
      for ( typename std::vector<PREDICATE>::const_iterator i = cuts.begin() ; cuts.end() != i ; ++i )
      { _cuts.push_back ( &(*i ) ) ; }
      return _found_N_ ( first , last , _cuts , except ) ;
    }
    // ========================================================================
    /** find "N" different elements in the sequence which satisfy to N
     *  various predicates
     *  @param first 'begin'-iterator of the sequence
     *  @param last  'end'-iterator for the sequence
     *  @param cust  the list of N-predicates
     *  @return true if there are N different good elements in the sequence
     */
    template <class OBJECT, class PREDICATE>
    inline bool found_N( OBJECT first                          ,
                         OBJECT last                           ,
                         const std::vector<PREDICATE>& cuts    )
    {
      if ( static_cast<std::size_t>(std::distance( first, last )) < cuts.size() )  return false;

      switch (cuts.size())
      {
      case 0 : return false;
      case 1 : return last != std::find_if( first , last , cuts.front() );
      case 2 : return found_2( first , last , cuts.front() , cuts.back() );
      case 3 : return found_3( first , last ,
                               *( cuts.begin ()     ) ,
                               *( cuts.begin () + 1 ) ,
                               *( cuts.begin () + 2 ) ) ;
      // case 4 : return found_4( first , last ,
      //                          *( cuts.begin ()     ) ,
      //                          *( cuts.begin () + 1 ) ,
      //                          *( cuts.begin () + 2 ) ,
      //                          *( cuts.begin () + 3 ) ) ;
      // case 5 : return found_5( first , last ,
      //                          *( cuts.begin ()     ) ,
      //                          *( cuts.begin () + 1 ) ,
      //                          *( cuts.begin () + 2 ) ,
      //                          *( cuts.begin () + 3 ) ,
      //                          *( cuts.begin () + 4 ) ) ;
      default : return _found_N ( first , last , cuts ) ;
      }
      return false ;
    }
    // ========================================================================
  } //                                              end of namespace LoKi::Algs
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_ALGS_H
