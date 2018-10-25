/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
// ============================================================================
#ifndef LOKI_STAT_H
#define LOKI_STAT_H 1
// ============================================================================
// Include files
// ============================================================================
#include <numeric>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatEntity.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Filters.h"
#include "LoKi/Primitives.h"
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
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Algs
  {
    // ========================================================================
    /** The trivial algorithm which accumulate the statistics
     *  of the function over the sequence
     *
     *  @param first    'begin'-iterator for the sequence
     *  @param last      'end'-iterator for the sequence
     *  @param functor   function to be evaluated
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-03-04
     */
    template <class OBJECT,class FUNCTOR>
    StatEntity stat ( OBJECT         first     ,
                      OBJECT         last      ,
                      const FUNCTOR& functor   )
    {
      using arg_t = decltype(*first);
      return std::accumulate(first,last,StatEntity{},
                             [&](StatEntity se, arg_t arg) {
                                se += functor(arg);
                                return se;
                             } );
    }
    // ========================================================================
    /** The trivial algorithm which accumulate the statistics
     *  of the function over the sequence
     *
     *  @param first    'begin'-iterator for the sequence
     *  @param last      'end'-iterator for the sequence
     *  @param functor   function to be evaluated
     *  @param predicate predicate to be used
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date   2011-03-04
     */
    template <class OBJECT,class FUNCTOR,class PREDICATE>
    StatEntity stat ( OBJECT           first     ,
                      OBJECT           last      ,
                      const FUNCTOR&   functor   ,
                      const PREDICATE& predicate )
    {
      using arg_t = decltype(*first);
      return std::accumulate( first, last, StatEntity{},
                              [&](StatEntity se, arg_t arg) {
                                if ( predicate(arg) ) se += functor(arg);
                                return se;
                              });
    }
    // ========================================================================
  } //                                              end of namespace LoKi::Algs
  // ==========================================================================
  namespace Functors
  {
    // ========================================================================
    /** @class Stat
     *  get the statistical quantities
     *  @see LoKi::Algs::accumulate
     *  @see LoKi::Functors::Sum
     *  @see StatEntity
     *  @author Vanya BELYAEV Ivan.BElyaev@cern.ch
     *  @date 2011-02-27
     */
    template <class TYPE>
    class Stat : public LoKi::Functors::Sum<TYPE>
    {
    public:
      // ======================================================================
      /// pointer to member function
      typedef double (StatEntity::*PMF)() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the function
      Stat ( PMF pmf , LoKi::FunctorFromFunctor<TYPE,double> fun , const std::string& nam )
        : LoKi::Functors::Sum <TYPE> ( std::move(fun) )
        , m_pmf ( pmf )
        , m_nam ( nam )
      {}
      /// constructor from the function and  predicate
      Stat ( PMF  pmf ,
             LoKi::FunctorFromFunctor<TYPE,double> fun ,
             LoKi::FunctorFromFunctor<TYPE,bool> cut ,
             const std::string& nam )
        : LoKi::Functors::Sum <TYPE> ( std::move(fun) , std::move(cut) )
        , m_pmf ( pmf )
        , m_nam ( nam )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      Stat* clone() const override { return new Stat ( *this ) ; }
      /// MANDATORY: the only one essential method:
      double operator()( const std::vector<TYPE>& a ) const override
      {
        StatEntity _stat = ( this->m_cut ?
          LoKi::Algs::stat ( a.begin(), a.end(),
                             LoKi::Apply( this->m_fun.func() ),
                             LoKi::Apply( this->m_cut->func() ) ) :
          LoKi::Algs::stat ( a.begin(), a.end(),
                             LoKi::Apply( this->m_fun.func() ) ) ) ;
        return (_stat.*m_pmf)() ;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override
      { return this -> _print_      ( s , this->m_nam , 0 ) ; }
      /// print as C++
      std::string   toCpp () const override
      {
          std::string s = "LoKi::" + this->m_nam          + "( "
                                   + this->m_fun.toCpp () ;
          if (this->m_cut) s += ", " + this->m_cut->toCpp () ;
          return s += ") " ;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// member function
      PMF m_pmf ;                                            // member function
      /// the function name
      std::string m_nam ;                                  // the function name
      // ======================================================================
    };
    // ========================================================================
  } //                                          end of namespace LoKi::Functors
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::mean
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F,
            typename TYPE = LoKi::details::type1_t<F>,
            typename = LoKi::details::require_signature<F,TYPE,double>>
  LoKi::Functors::Stat<TYPE> mean ( F&& fun )
  { return { &StatEntity::mean , std::forward<F>(fun) , "mean" } ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::mean
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F1, typename F2,
            typename TYPE = LoKi::details::type1_t<F1,F2>,
            typename = LoKi::details::require_signature<F1,TYPE,double>,
            typename = LoKi::details::require_signature<F2,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> mean ( F1&& fun , F2&&   cut )
  { return { &StatEntity::mean , std::forward<F1>(fun), std::forward<F2>(cut),
             "mean"            } ; }
  // ==========================================================================
  /** get the rms value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity:rms
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F,
            typename TYPE = LoKi::details::type1_t<F>,
            typename = LoKi::details::require_signature<F,TYPE,double>>
  LoKi::Functors::Stat<TYPE> rms ( F&& fun )
  { return { &StatEntity::rms  , std::forward<F>(fun) , "rms" } ; }
  // ==========================================================================
  /** get the rms value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::rms
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F1, typename F2,
            typename TYPE = LoKi::details::type1_t<F1,F2>,
            typename = LoKi::details::require_signature<F1,TYPE,double>,
            typename = LoKi::details::require_signature<F2,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> rms ( F1&& fun , F2&& cut )
  { return { &StatEntity::rms, std::forward<F1>(fun), std::forward<F2>(cut),
             "rms" } ; }
  // ==========================================================================
  /** get the meanErr value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::meanErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F,
            typename TYPE = LoKi::details::type1_t<F>,
            typename = LoKi::details::require_signature<F,TYPE,double>>
  LoKi::Functors::Stat<TYPE> meanErr ( F&& fun )
  { return { &StatEntity::meanErr, std::forward<F>(fun), "meanErr" } ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::meanErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F1, typename F2,
            typename TYPE = LoKi::details::type1_t<F1,F2>,
            typename = LoKi::details::require_signature<F1,TYPE,double>,
            typename = LoKi::details::require_signature<F2,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> meanErr ( F1&& fun , F2&& cut )
  { return { &StatEntity::meanErr , std::forward<F1>(fun), std::forward<F2>(cut)                  ,
             "meanErr"            } ; }
  // ==========================================================================
  /** get the 'efficiency' value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::eff
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F,
            typename TYPE = LoKi::details::type1_t<F>,
            typename = LoKi::details::require_signature<F,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> eff ( F&& fun )
  { return { &StatEntity::eff ,
             LoKi::SimpleSwitch<TYPE> ( std::forward<F>(fun) , 1 , 0 ) ,
             "eff"            } ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::eff
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F1, typename F2,
            typename TYPE = LoKi::details::type1_t<F1,F2>,
            typename = LoKi::details::require_signature<F1,TYPE,bool>,
            typename = LoKi::details::require_signature<F2,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> eff ( F1&& fun , F2&& cut )
  { return { &StatEntity::eff     ,
             LoKi::SimpleSwitch<TYPE> ( std::forward<F1>(fun) , 1 , 0 ) ,
             std::forward<F2>(cut),
             "eff"                } ; }
  // ==========================================================================
  /** get the 'efficiency' value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::effErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F,
            typename TYPE = LoKi::details::type1_t<F>,
            typename = LoKi::details::require_signature<F,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> effErr ( F&& fun )
  { return { &StatEntity::effErr ,
             LoKi::SimpleSwitch<TYPE> ( std::forward<F>(fun) , 1 , 0 ) ,
             "effErr"            } ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::effErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <typename F1, typename F2,
            typename TYPE = LoKi::details::type1_t<F1,F2>,
            typename = LoKi::details::require_signature<F1,TYPE,bool>,
            typename = LoKi::details::require_signature<F2,TYPE,bool>>
  LoKi::Functors::Stat<TYPE> effErr( F1&& fun ,
                                     F2&& cut )
  { return { &StatEntity::effErr ,
             LoKi::SimpleSwitch<TYPE>( std::forward<F1>(fun) , 1 , 0 ) ,
             std::forward<F2>(cut),
             "effErr"             } ; }
  // ==========================================================================
} //                                                      end of namesapce LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_STAT_H
// ============================================================================
