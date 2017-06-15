#ifndef LOKI_TIMERS_H
#define LOKI_TIMERS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <string>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IChronoSvc.h"
#include "GaudiKernel/ChronoEntity.h"
#include "GaudiKernel/Chrono.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Functor.h"
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *  ``C++ ToolKit  for Smart and Friendly Physics Analysis''
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
 *  @date 2011-02-02
 */
namespace LoKi
{
  // ==========================================================================
  /** @class Timer
   *  Helepr class for implementation of timers
   *  @see LoKi::Functors::Timer_
   *  @see LoKi::timer
   *  @see Chrono
   *  @see ChronoEntity
   *  @see ChronoSvc
   */
  class GAUDI_API Timer
  {
  public:
    // ========================================================================
    /// constructor from the timer name
    Timer ( const std::string& name ) ;
    /// destructor
    virtual ~Timer () ; // destructor
    // ========================================================================
  public:
    // ========================================================================
    /// get the timer name
    const std::string& name() const { return m_name ; }   // get the timer name
    // ========================================================================
  private:
    // ========================================================================
    /// the default constructor is disabled
    Timer () ;                           // the default constructor is disabled
    // ========================================================================
  private:
    // ========================================================================
    /// the actual name of timer
    std::string m_name ;                            // the actual name of timer
    // ========================================================================
  };
  // ==========================================================================
  namespace Functors
  {
    // ========================================================================
    /** @class Timer_
     *  Helper class for monitoring/timing of LoKi functors
     *  @see ChronoEntity
     *  @see Chrono
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-02-02
     */
    template <class TYPE1,class TYPE2>
    class Timer_ : public LoKi::Functor<TYPE1,TYPE2>
    {
    public :
      // ======================================================================
      /// constructor from functor&timer
      template <typename F, typename = details::require_signature<F,TYPE1,TYPE2>>
      Timer_ ( F&&           fun    ,
               ChronoEntity* timer  )
        : m_fun   ( std::forward<F>(fun)   )
        , m_timer ( timer )
        , m_first ( false )
      {}
      /// constructor from functor&service&timer name
      template <typename F, typename = details::require_signature<F,TYPE1,TYPE2>>
      Timer_ ( F&&                fun    ,
               IChronoSvc*        svc    ,
               const std::string& timer  )
        : m_fun   ( std::forward<F>(fun)   )
        , m_svc   ( svc   )
        , m_tname ( timer )
      {
        if ( !m_svc && this->gaudi() )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
      }
      /// constructor from functor&timer name
      template <typename F, typename = details::require_signature<F,TYPE1,TYPE2>>
      Timer_ ( F&&                fun    ,
               const std::string& timer  )
        : LoKi::AuxFunBase ( std::tie ( fun , timer ) )
        , m_fun   ( std::forward<F>(fun)   )
        , m_tname ( timer )
      {
        if ( this->gaudi() )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
      }
      /// destructor
      virtual ~Timer_ ()
      { if ( this->m_svc && !this->gaudi() ) { this->m_svc.reset() ; } }
      /// MANDATORY: clone method ("virtual constructor")
      Timer_ * clone() const override { return new Timer_ ( *this ) ; }
      /// MANDATORY: the only one essenital method
      TYPE2 operator()
      ( typename LoKi::Functor<TYPE1,TYPE2>::argument a ) const override
      {
        //
        if ( !m_timer && m_first && !m_svc )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
        //
        if ( !m_timer && m_first && !(!m_svc ) )
        { m_timer = m_svc->chronoStart ( m_tname ) ; }
        //
        Chrono _timer ( m_timer ) ;
        m_first = false ;
        //
        return m_fun.fun ( a ) ;
      }
      // ======================================================================
      /// optional: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      {
        //
        if ( !m_timer && m_tname.empty() && !m_svc ) { return s << m_fun ; }
        //
        return s << " timer(" << m_fun << ",'" << m_tname << "')" ;
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled
      Timer_ () ;                        // the default constructor is disabled
      // ======================================================================
    public:
      // ======================================================================
      /// get the timer
      const ChronoEntity* timer() const { return m_timer ; }   // get the timer
      // ======================================================================
    private:
      // ======================================================================
      /// the functor
      LoKi::FunctorFromFunctor<TYPE1,TYPE2> m_fun   ;   // the functor
      /// the service
      mutable LoKi::Interface<IChronoSvc>   m_svc   ;   // the service
      /// the timer name
      std::string                           m_tname ;   // the timer name
      /// the actual timer
      mutable ChronoEntity*                 m_timer = nullptr ;   // the actual timer
      /// the first time?
      mutable bool                          m_first = true;   // the first time?
      // ======================================================================
    };
    // ========================================================================
    /// specialization for "sources"
    template <class TYPE2>
    class Timer_ <void,TYPE2> : public LoKi::Functor<void,TYPE2>
    {
    public :
      // ======================================================================
      /// constructor from functor&timer
      template <typename F, typename = details::require_signature<F,void,TYPE2>>
      Timer_( F&&           fun    ,
              ChronoEntity* timer  )
        : m_fun   ( std::forward<F>(fun)   )
        , m_timer ( timer )
        , m_first ( false )
      {}
      /// constructor from functor&service&timer name
      template <typename F, typename = details::require_signature<F,void,TYPE2>>
      Timer_ ( F&&                fun    ,
               IChronoSvc*        svc    ,
               const std::string& timer  )
        : m_fun   ( std::forward<F>(fun)   )
        , m_svc   ( svc   )
        , m_tname ( timer )
      {
        if ( !m_svc && this->gaudi() )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
      }
      /// constructor from functor&timer name
      template <typename F, typename = details::require_signature<F,void,TYPE2>>
      Timer_ ( F&&                 fun    ,
               const std::string&  timer  )
        : LoKi::AuxFunBase ( std::tie ( fun , timer ) )
        , m_fun   ( std::forward<F>(fun)   )
        , m_tname ( timer )
      {
        if ( this->gaudi() )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
      }
      /// MANDATORY: virtual destructor
      virtual ~Timer_ ()
      { if ( this->m_svc && !this->gaudi() ) { this->m_svc.reset() ; } }
      /// MANDATORY: clone method ("virtual constructor")
      Timer_ * clone() const override { return new Timer_ ( *this ) ; }
      // ======================================================================
      /// MANDATORY: the only one essenital method
      TYPE2 operator() () const override
      {
        //
        if ( !m_timer && m_first && !m_svc )
        {
          SmartIF<IChronoSvc> cs ( this->lokiSvc().getObject() ) ;
          m_svc = cs.get() ;
        }
        //
        if ( !m_timer && m_first && !(!m_svc ) )
        { m_timer = m_svc->chronoStart ( m_tname ) ; }
        //
        //
        Chrono _timer ( m_timer ) ;
        m_first = false ;
        //
        return m_fun.fun () ;
      }
      // ======================================================================
    public:
      // ======================================================================
      /// get the timer
      const ChronoEntity* timer() const { return m_timer ; }   // get the timer
      // ======================================================================
    private:
      // ======================================================================
      /// the functor
      LoKi::FunctorFromFunctor<void,TYPE2>  m_fun   ;   // the functor
      /// the service
      mutable LoKi::Interface<IChronoSvc>   m_svc   ;   // the service
      /// the timer name
      std::string                           m_tname ;   // the timer name
      /// the actual timer
      mutable ChronoEntity*                 m_timer = nullptr;   // the actual timer
      /// the first time?
      mutable bool                          m_first = true;   // the first time?
      // ======================================================================
    };
    // ========================================================================
  } // end of namespace LoKi::Functors
  // ========================================================================
  /** get the timer functor
   *  @see LoKi::Timer
   *  @see ChronoEntity
   *  @see Chrono
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-02-02
   */
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Timer_<TYPE1,TYPE2> timer( F&& fun, ChronoEntity*  timer )
  { return { std::forward<F>(fun) , timer } ; }
  // ==========================================================================
  /** get the timer functor
   *  @see LoKi::Timer
   *  @see ChronoEntity
   *  @see Chrono
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-02-02
   */
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Timer_<TYPE1,TYPE2> timer( F&& fun, IChronoSvc* svc  ,
                                             const std::string& timer  )
  { return { fun , svc , timer } ; }
  // ==========================================================================
  /** get the timer functor
   *  @see LoKi::Timer
   *  @see ChronoEntity
   *  @see Chrono
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-02-02
   */
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Timer_<TYPE1,TYPE2> timer( F&& fun, const std::string& timer )
  { return { std::forward<F>(fun) , timer } ; }
  // ==========================================================================
  /** get the timer functor
   *  @see LoKi::Timer
   *  @see ChronoEntity
   *  @see Chrono
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-02-02
   */
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Timer_<TYPE1,TYPE2> timer( F&& fun, const LoKi::Timer& timer )
  { return { std::forward<F>(fun) , timer.name() } ; }
  // ==========================================================================
  /// operator form of timers
  template <typename F,
            typename TYPE1 = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::FunctorFromFunctor<TYPE1,TYPE2>
  operator %( const LoKi::Timer&                timer  ,
              F&& fun    )
  {
    if (timer.name().empty()) return  std::forward<F>(fun);
    return LoKi::Functors::Timer_<TYPE1,TYPE2>( std::forward<F>(fun) , timer.name() ) ;
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
/// operator form of timers
template <typename F,
          typename TYPE1 = LoKi::details::type1_t<F>,
          typename TYPE2 = LoKi::details::type2_t<F>>
LoKi::FunctorFromFunctor<TYPE1,TYPE2> operator %( ChronoEntity* timer, F&& fun)
{
  if (!timer) return std::forward<F>(fun);
  return LoKi::Functors::Timer_<TYPE1,TYPE2>( std::forward<F>(fun), timer );
}
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_TIMERS_H
// ============================================================================
