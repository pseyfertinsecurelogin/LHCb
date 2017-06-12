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
    inline
    StatEntity stat
    ( OBJECT           first     ,
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
      /// the base
      typedef LoKi::Functor<std::vector<TYPE>,double>  uBase    ; // the base
      /// pointer to member function
      typedef double (StatEntity::*PMF)() const ;
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the function
      Stat ( PMF                               pmf ,
             const LoKi::Functor<TYPE,double>& fun ,
             const std::string&                nam )
        : LoKi::Functors::Sum <TYPE> ( fun )
        , m_pmf ( pmf )
        , m_nam ( nam )
      {}
      /// constructor from the function and  predicate
      Stat ( PMF                               pmf ,
             const LoKi::Functor<TYPE,double>& fun ,
             const LoKi::Functor<TYPE,bool>&   cut ,
             const std::string&                nam )
        : LoKi::Functors::Sum <TYPE> ( fun , cut )
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
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  mean ( const LoKi::Functor<TYPE,double>& fun )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::mean ,
                                       fun               ,
                                       "mean"            ) ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::mean
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  mean ( const LoKi::Functor<TYPE,double>& fun ,
         const LoKi::Functor<TYPE,bool>&   cut )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::mean ,
                                       fun               ,
                                       cut               ,
                                       "mean"            ) ; }
  // ==========================================================================
  /** get the rms value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity:rms
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  rms ( const LoKi::Functor<TYPE,double>& fun )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::rms  ,
                                       fun               ,
                                       "rms"             ) ; }
  // ==========================================================================
  /** get the rms value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::rms
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  rms ( const LoKi::Functor<TYPE,double>& fun ,
        const LoKi::Functor<TYPE,bool>&   cut )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::rms  ,
                                       fun               ,
                                       cut               ,
                                       "rms"             ) ; }
  // ==========================================================================
  /** get the meanErr value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::meanErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  meanErr ( const LoKi::Functor<TYPE,double>& fun )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::meanErr ,
                                       fun                  ,
                                       "meanErr"            ) ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::meanErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  meanErr ( const LoKi::Functor<TYPE,double>& fun ,
            const LoKi::Functor<TYPE,bool>&   cut )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::meanErr ,
                                       fun                  ,
                                       cut                  ,
                                       "meanErr"            ) ; }
  // ==========================================================================
  /** get the 'efficiency' value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::eff
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  eff ( const LoKi::Functor<TYPE,bool>& fun )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::eff ,
                                       LoKi::SimpleSwitch<TYPE> ( fun , 1 , 0 ) ,
                                       "eff"            ) ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::eff
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  eff ( const LoKi::Functor<TYPE,bool>& fun ,
        const LoKi::Functor<TYPE,bool>& cut )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::eff     ,
                                       LoKi::SimpleSwitch<TYPE> ( fun , 1 , 0 ) ,
                                       cut                  ,
                                       "eff"                ) ; }
  // ==========================================================================
  /** get the 'efficiency' value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::effErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  effErr ( const LoKi::Functor<TYPE,bool>& fun )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::effErr ,
                                       LoKi::SimpleSwitch<TYPE> ( fun , 1 , 0 ) ,
                                       "effErr"            ) ; }
  // ==========================================================================
  /** get the mean value for some functor
   *  @see LoKi::Functors::Stat
   *  @see StatEntity
   *  @see StatEntity::effErr
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-03-04
   */
  template <class TYPE>
  inline LoKi::Functors::Stat<TYPE>
  effErr( const LoKi::Functor<TYPE,bool>& fun ,
          const LoKi::Functor<TYPE,bool>& cut )
  { return LoKi::Functors::Stat<TYPE>( &StatEntity::effErr ,
                                       LoKi::SimpleSwitch<TYPE> ( fun , 1 , 0 ) ,
                                       cut                  ,
                                       "effErr"             ) ; }
  // ==========================================================================
} //                                                      end of namesapce LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_STAT_H
// ============================================================================
