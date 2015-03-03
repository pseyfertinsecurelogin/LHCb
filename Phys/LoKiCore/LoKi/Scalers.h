// $Id: Scalers.h,v 1.2 2010-02-13 16:31:06 ibelyaev Exp $
// ============================================================================
#ifndef LOKI_SCALERS_H 
#define LOKI_SCALERS_H 1
// ============================================================================
// Include files
// ============================================================================
// LHCbKernel
// ============================================================================
#include "Kernel/IReferenceRate.h"
// ============================================================================
// LoKi 
// ============================================================================
#include "LoKi/Random.h"
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  /** @namespace LoKi::Scalers LoKi/Scalers.h
   *  Helper namespace with varios "scalers"   
   *
   *  @author Vanya Belyaev Ivan.Belyaev@nikhef.nl
   *  @date   2009-12-04
   */  
  namespace Scalers 
  {
    // ========================================================================
    // void -> bool 
    // ========================================================================
    /** @class RandomScaleV 
     *  simple scaler for randomly prescale
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class RandomScaleV : public LoKi::Functor<void,bool> 
    {
    public:
      // ======================================================================
      /** constructor from probability and random service 
       *  @param prop accep probability 
       *  @param service the random number servce 
       */
      RandomScaleV ( const double prob         , 
                     IRndmGenSvc* service = 0  ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~RandomScaleV () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  RandomScaleV*  clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( /* argument v  */ ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( /* argument v = 0 */ ) const ;         //      get the result
      /// get the probability 
      double prob() const { return m_prob ; }            // get the probability 
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      RandomScaleV () ;                     // Default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// random numbers source  
      LoKi::Random::Uniform m_uniform ;               //         random numbers 
      /// the accept ptobability 
      double                m_prob    ;               // the accept probability
      // ======================================================================
    };
    // ========================================================================
    /** @class SkipperV
     *  simple scaler to skip events 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class SkipperV : public LoKi::Functor<void,bool> 
    {
    public:
      // ======================================================================
      /** constructor from skip rate 
       *  @param skip the skip rate  
       */
      SkipperV ( const size_t skip ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~SkipperV () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  SkipperV* clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( /* argument v */ ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( /* argument v */ ) const ;                  // get the result
      /// get skip rate 
      size_t skip() const { return m_skip ; }
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      SkipperV () ;                           // Default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// skip
      size_t          m_skip ;                                          // skip 
      mutable size_t  m_curr ;                                       // current 
      // ======================================================================
    } ;
    // ========================================================================
    /** @class RateLimitV 
     *  simple scaler that scales the predicate according to event rate 
     *  @see IReferenceRate
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class RateLimitV : public LoKi::Functor<void,bool> 
    {
    public:
      // ======================================================================
      /** constructor from rate and "random" flag 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimitV ( const double maxRate        , 
                   const bool   random  = true ) ;
      // ======================================================================
      /** constructor from the service , rate and "random" flag 
       *  @param svc     the rate service 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimitV ( const IReferenceRate* service        ,  
                   const double          maxRate        , 
                   const bool            random  = true ) ;
      // ======================================================================
      /** constructor from the service name , rate and "random" flag 
       *  @param svc     the rate service name 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimitV ( const std::string&    service        ,  
                   const double          maxRate        , 
                   const bool            random  = true ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~RateLimitV () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  RateLimitV*  clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( /* argument v */ ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( /* argument v = 0 */ ) const ;              // get the result 
      /// get the rate 
      double rate() const { return m_rate ; }                   // get the rate 
      /// get the random 
      bool   random() const { return m_random ; }             // get the random 
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      RateLimitV () ;                        // Default constructor is disabled 
      // ======================================================================
    protected:
      // ======================================================================
      /// get the service
      StatusCode getService ( const std::string& service ) const ;
      // ======================================================================
    private:
      // ======================================================================
      /// rate service 
      mutable LoKi::Interface<IReferenceRate> m_rateSvc ; //       rate service
      /// random numbers source  
      LoKi::Random::Uniform                   m_uniform ; //     random numbers 
      /// The rate limit 
      double                                  m_rate    ; //     the rate limit 
      // the random flag 
      bool                                    m_random  ; //        random flag 
      // ======================================================================
    private:
      // ======================================================================
      /// Tick interval 
      long double         m_interval ;                         // Tick interval 
      /// the next tick 
      mutable long double m_next     ;                         // the next tick 
      // ======================================================================
    };
    // ========================================================================
    // double -> bool 
    // ========================================================================
    /** @class RandomScale 
     *  simple scaler for randomly prescale
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class RandomScale : public LoKi::Functor<double,bool> 
    {
    public:
      // ======================================================================
      /** constructor from probability and random service 
       *  @param prop accep probability 
       *  @param service the random number servce 
       */
      RandomScale ( const double prob         , 
                    IRndmGenSvc* service = 0  ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~RandomScale () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  RandomScale*  clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( argument v  ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( argument v = 0 ) const ;                  // get the result ;
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      RandomScale () ;                       // Default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// Random scale 
      LoKi::Scalers::RandomScaleV m_scaler ;                      // the scaler 
      // ======================================================================
    };
    // ========================================================================
    /** @class Skipper
     *  simple scaler to skip events 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class Skipper : public LoKi::Functor<double,bool> 
    {
    public:
      // ======================================================================
      /** constructor from skip rate 
       *  @param skip the skip rate  
       */
      Skipper ( const size_t skip ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~Skipper () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  Skipper* clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( argument v ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( argument v = 0 ) const ;                  // get the result ;
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      Skipper () ;                           // Default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// the skipper 
      LoKi::Scalers::SkipperV m_skipper ;                        // the skipper 
      // ======================================================================
    } ;
    // ========================================================================
    /** @class RateLimit 
     *  simple scaler that scales the predicate according to event rate 
     *  @see IReferenceRate
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    class RateLimit : public LoKi::Functor<double,bool> 
    {
    public:
      // ======================================================================
      /** constructor from rate and "random" flag 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimit ( const double maxRate        , 
                  const bool   random  = true ) ;
      // ======================================================================
      /** constructor from the service , rate and "random" flag 
       *  @param svc     the rate service 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimit ( const IReferenceRate* service        ,  
                  const double          maxRate        , 
                  const bool            random  = true ) ;
      /** constructor from the service name , rate and "random" flag 
       *  @param svc     the rate service name 
       *  @param maxRate the maximal rate 
       *  @param random the random flag 
       */
      RateLimit ( const std::string&    service        ,  
                  const double          maxRate        , 
                  const bool            random  = true ) ;
      // ======================================================================
      /// MANDATORY: virtual destructor 
      virtual ~RateLimit () ;
      /// MANDATORY: clone method ("virtual constructor")
      virtual  RateLimit*  clone() const ;
      /// MANDATORY: the only one essential method 
      virtual result_type operator () ( argument v ) const ; 
      /// OPTIONAL: nice printout 
      virtual std::ostream& fillStream ( std::ostream& s ) const ;
      // ======================================================================
    public:
      // ======================================================================
      /// get the result ;
      bool eval ( argument v = 0 ) const ;                  // get the result ;
      // ======================================================================
    private:
      // ======================================================================
      /// Default constructor is disabled 
      RateLimit() ;                         // Default constructor is disabled 
      // ======================================================================
    private:
      // ======================================================================
      /// rate limit
      LoKi::Scalers::RateLimitV m_rateLimit ;                     // rate limit
      // ======================================================================
    };
    // ========================================================================

  } //                                           end of namespace LoKi::Scalers 
  // ==========================================================================
} //                                                      end of namespace LoKi 
// ============================================================================
namespace LoKi 
{
  // ==========================================================================
  namespace Cuts 
  {
    // ========================================================================
    /** @typedef RATE 
     *  Trivial predicate to determine the accept rate limit 
     * 
     *  The actual code and concept belongs to Gerhard "The Great" Raven and 
     *  Wouter Hulsbergen
     *
     *  @code
     *
     *    RATE scaler ( 5 * Gaudi::Units::Hz ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler() ; 
     *
     *  @endcode 
     *
     *  @see IReferenceRate
     *  @see LoKi::Scalers::RateLimitV
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::RateLimitV                                    RATE ;
    // ========================================================================
    /** @typedef XRATE 
     *  Trivial predicate to determine the accept rate limit 
     * 
     *  The actual code and concept belongs to Gerhard "The Great" Raven and 
     *  Wouter Hulsbergen
     *
     *  @code
     *
     *    XRATE scaler ( 5 * Gaudi::Units::Hz ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler( 0 ) ; 
     *
     *  @endcode 
     *
     *  @see IReferenceRate
     *  @see LoKi::Scalers::RateLimit
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::RateLimit                                    XRATE ;
    // ========================================================================
    /** @typedef SCALE
     *  Trivial predicate to prescale with given accept probability
     * 
     *  @code
     *
     *    SCALE scaler ( 0.05 ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler() ; 
     *
     *  @endcode 
     *
     *  @see LoKi::Scalers::RandomScaleV
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::RandomScaleV                                 SCALE ;
    // ========================================================================
    /** @typedef XSCALE
     *  Trivial predicate to prescale with given accept probability
     * 
     *  @code
     *
     *    XSCALE scaler ( 0.05 ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler( 0 ) ; 
     *
     *  @endcode 
     *
     *  @see LoKi::Scalers::RandomScale
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::RandomScale                                 XSCALE ;
    // ========================================================================
    /** @typedef SKIP
     *  Trivial predicate to accept only Nth event
     * 
     *  @code
     *
     *    SKIP scaler ( 10 ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler() ; 
     *
     *  @endcode 
     *
     *  @see LoKi::Scalers::SkipperV
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::SkipperV                                      SKIP ;
    // ========================================================================
    /** @typedef XSKIP
     *  Trivial predicate to accept only Nth event
     * 
     *  @code
     *
     *    XSKIP scaler ( 10 ) ;
     *
     *    PREDICATE predicate = ... ;
     *    OBJECT    object    = ... ;
     * 
     *    const bool accept =  predicate ( object ) && scaler( 0 ) ; 
     *
     *  @endcode 
     *
     *  @see LoKi::Scalers::Skipper
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2009-12-04
     */
    typedef LoKi::Scalers::Skipper                                      XSKIP ;
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts  
  // ==========================================================================
} //                                                      end of namespace LoKi 
// ============================================================================
// The END 
// ============================================================================
#endif // LOKI_SCALERS_H
// ============================================================================

