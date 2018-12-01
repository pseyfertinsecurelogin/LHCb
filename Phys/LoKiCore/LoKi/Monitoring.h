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
#ifndef LOKI_MONITORING_H
#define LOKI_MONITORING_H 1
// ============================================================================
// Include files
// ============================================================================
// AIDA
// ============================================================================
#include "AIDA/IHistogram1D.h"
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/StatEntity.h"
#include "GaudiKernel/HistoDef.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// Local
// ============================================================================
#include "LoKi/Functions.h"
#include "LoKi/HistoBook.h"
#include "LoKi/Monitor.h"
// ============================================================================
/// forward declarations
// ============================================================================
class IStatSvc       ;
class IAlgContextSvc ;
class GaudiAlgorithm ;
class GaudiTool      ;
namespace GaudiAlg { class ID ; }
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  /** @namespace LoKi::Monitoring Monitoring.h LoKi/Monitoring.h
   *  Collection of  useful smart "self-monitoring" meta-functors
   *
   *  Many thanks to Hugo RUIZ PEREZ for his nice and brilliant ideas!
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date   2007-06-14
   */
  namespace Monitoring
  {
    // ========================================================================
    /** @class Counter
     *  Simple functor which decorates another functor
     *  with some "monitoring" features : it uses the generic
     *  counter to monitor the performace.
     *
     *
     *  @code
     *
     *  // some predicate:
     *  Cut cut = ...
     *
     *  // in the monitoring context, substitute it the monitored predicate:
     *  if ( monitoring )
     *   {
     *     cut = Counter<const LHCb::Particle*,bool> ( cut , &counter("efficiency") ) ;
     *   }
     *
     *  // and use it as a regular predicate
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-06-14
     *  @warning This class has no proper <c>toCpp</c> method
     */
    template <class TYPE, class TYPE2>
    class Counter : public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      /// constructor from the predicate and the generic counter
      Counter ( LoKi::FunctorFromFunctor<TYPE,TYPE2> cut  ,
                StatEntity* stat )
        : m_cut    ( std::move(cut) )
        , m_stat   ( stat )
      {}
      /// constructor from the predicate and the generic counter
      Counter ( LoKi::FunctorFromFunctor<TYPE,TYPE2> cut  ,
                const LoKi::CounterDef& cnt )
        : LoKi::AuxFunBase ( std::tie ( cut , cnt ) )
        , m_cut    ( std::move(cut)  )
        , m_cntdef ( cnt  )
      {
        if ( this->gaudi() && m_cntdef.valid() )
        { m_stat = LoKi::Monitoring::getCounter ( m_cntdef ) ; }
      }
      /// MANDATORY: clone method ("virtual constructor")
      Counter* clone() const override { return new Counter(*this); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()
        ( typename LoKi::Functor<TYPE,TYPE2>::argument a ) const override
      {
        const auto result = m_cut.fun ( a ) ;
        /// get counter if needed
        if ( !m_stat &&  m_cntdef.valid() )
        { m_stat = LoKi::Monitoring::getCounter ( m_cntdef ) ; }
        ///  perform monitoring
        if ( m_stat ){ (*m_stat) += result ; }       // perform monitoring
        return result ;                                               // RETURN
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return m_cut.fillStream ( s ) ; }
      /// OPTIONAL: delegate ID:
      std::size_t id() const override { return m_cut.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" predicate:
      LoKi::FunctorFromFunctor<TYPE,TYPE2> m_cut ;      // the "main" predicate
      /// generic counter used for monitoring:
      mutable StatEntity* m_stat = nullptr ; // generic counter used for monitoring
      /// counter description
      LoKi::CounterDef    m_cntdef ;     // counter description
      // ======================================================================
    };
    // ========================================================================
    template <class TYPE2>
    class Counter<void,TYPE2> : public LoKi::Functor<void,TYPE2>
    {
    public:
      // ======================================================================
      /// constructor from the predicate and the generic counter
      Counter ( LoKi::FunctorFromFunctor<void,TYPE2> cut  ,
                StatEntity* stat )
        : m_cut     ( std::move(cut) )
        , m_stat    ( stat )
      {}
      Counter ( LoKi::FunctorFromFunctor<void,TYPE2> cut  ,
                const LoKi::CounterDef&  cnt  )
        : LoKi::AuxFunBase ( std::tie ( cut , cnt ) )
        , m_cut     ( std::move(cut) )
        , m_cntdef  ( cnt  )
      {
        if ( this->gaudi() && m_cntdef.valid() )
        { m_stat = LoKi::Monitoring::getCounter ( m_cntdef ) ; }
      }
      /// MANDATORY: clone method ("virtual constructor")
      Counter* clone() const override { return new Counter(*this); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()(  ) const override
      {
        const auto result = m_cut.fun ( /* a */ ) ;
        /// get counter if needed
        if ( !m_stat &&  m_cntdef.valid() )
        { m_stat = LoKi::Monitoring::getCounter ( m_cntdef ) ; }
        /// perform monitoring
        if ( m_stat ){ (*m_stat) += result ; } // perform monitoring
        return result ;                                           // RETURN
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return m_cut.fillStream ( s ) ; }
      /// OPTIONAL: delegate ID:
      std::size_t id() const override { return m_cut.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" predicate:
      LoKi::FunctorFromFunctor<void,TYPE2> m_cut ;      // the "main" predicate
      /// generic counter used for monitoring:
      mutable StatEntity* m_stat = nullptr; // generic counter for monitoring
      /// counter description
      LoKi::CounterDef    m_cntdef ;    // counter description
      // ======================================================================
    };
    // ========================================================================
    /** @class Plot
     *  Simple functor which decorates another functor
     *  with some "monitoring" features : it fills the monitoring histogram
     *
     *  @code
     *
     *  // some function
     *  Fun fun = PT ;
     *
     *  // in the monitoring context, substitute it with the monitored predicate:
     *  if ( monitoring )
     *   {
     *     IHistogram1D* histo = book (... ) ;
     *     fun = Plot<const LHCb::Particle> ( fun , histo ) ;
     *   }
     *
     *  // and use it as a regular function:
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2007-06-14
     *  @warning This class has no proper <c>toCpp</c> method
     */
    template <class TYPE, class TYPE2=double>
    class Plot: public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      /// constructor from the function and the histogram
      Plot ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun   ,
             AIDA::IHistogram1D* histo )
        : m_fun     ( std::move(fun)   )
        , m_histo   ( histo )
      {}
      /// constructor from the function and the histogram  description
      Plot ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun   ,
             const LoKi::Histo& hdef  )
        : LoKi::AuxFunBase ( std::tie ( fun , hdef ) )
        , m_fun     ( std::move(fun)   )
        , m_hdef    ( hdef  )
      {
        // try to load the histo
        if ( this->gaudi() && m_hdef.valid() )
        { m_histo = LoKi::HistoBook::book ( m_hdef ) ; }
      }
      /// constructor from the function and the histogram  description
      Plot ( const LoKi::Histo& hdef  ,
             LoKi::FunctorFromFunctor<TYPE,TYPE2> fun   )
        : LoKi::AuxFunBase ( std::tie ( hdef , fun ) )
        , m_fun     ( std::move(fun)   )
        , m_hdef    ( hdef  )
      {
        // try to load the histo
        if ( this->gaudi() && m_hdef.valid() )
        { m_histo = LoKi::HistoBook::book ( m_hdef ) ; }
      }
      /// constructor from the function and the histogram
      Plot ( AIDA::IHistogram1D* histo ,
             LoKi::FunctorFromFunctor<TYPE,TYPE2> fun   )
        : m_fun     ( std::move(fun) )
        , m_histo   ( histo )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      Plot* clone() const override { return new Plot(*this); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()
        ( typename LoKi::Functor<TYPE,TYPE2>::argument a ) const override
      {
        // evaluate the functor
        const auto result = m_fun.fun ( a ) ;
        // try to load the histo
        if ( !m_histo && m_hdef.valid() )
        { m_histo = LoKi::HistoBook::book ( m_hdef ) ; }
        //
        // perform monitoring
        if ( m_histo ) { m_histo -> fill ( result ) ;  } ///< perform monitoring
        return result ;
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return m_fun.fillStream ( s ) ; }
      /// delegate ID:
      std::size_t id() const override { return m_fun.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" function:
      LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ; // the "main" function
      /// the histogram for monitoring
      mutable AIDA::IHistogram1D*      m_histo = nullptr ; // the histogram for monitoring
      /// histogram description for lazy instantiation
      LoKi::Histo                      m_hdef  ; // histogram description
      // ======================================================================
    };
    // ========================================================================
    template <class TYPE2>
    class Plot<void,TYPE2>: public LoKi::Functor<void,TYPE2>
    {
    public:
      // ======================================================================
      /// constructor from the function and the histogram
      Plot ( LoKi::FunctorFromFunctor<void,TYPE2> fun,
             AIDA::IHistogram1D* histo )
        : m_fun     ( std::move(fun) )
        , m_histo   ( histo )
      {}
      /// constructor from the function and the histogram
      Plot ( LoKi::FunctorFromFunctor<void,TYPE2> fun,
             const LoKi::Histo&  hdef  )
        : LoKi::AuxFunBase ( std::tie ( fun , hdef ) )
        , m_fun     ( std::move(fun) )
        , m_hdef    ( hdef  )
      {
        // try to load the histo
        if ( this->gaudi() && m_hdef.valid() )
        { m_histo = LoKi::HistoBook::book ( m_hdef ) ; }
      }
      /// constructor from the function and the histogram
      Plot ( AIDA::IHistogram1D* histo ,
             LoKi::FunctorFromFunctor<void,TYPE2> fun   )
        : m_fun     ( std::move(fun) )
        , m_histo   ( histo )
      {}
      /// MANDATORY: clone method ("virtual constructor")
      Plot* clone() const override { return new Plot(*this); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator() (  ) const override
      {
        const auto result = m_fun.fun ( ) ;
        // try to load the histo
        if ( !m_histo && m_hdef.valid() )
        { m_histo = LoKi::HistoBook::book ( m_hdef ) ; }
        // perform monitoring
        if ( m_histo ) { m_histo -> fill ( result ) ;  } ///< perform monitoring
        return result ;
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override 
      { return m_fun.fillStream ( s ) ; }
      /// delegate ID:
      std::size_t id() const override { return m_fun.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" function:
      LoKi::FunctorFromFunctor<void,TYPE2> m_fun ; // the "main" function
      /// the histogram for monitoring
      mutable AIDA::IHistogram1D*      m_histo = nullptr ; // the histogram for monitoring
      /// histogram description for lazy instantiation
      LoKi::Histo                      m_hdef  ; // histogram description
      // ======================================================================
    };
    // ========================================================================
    /** @class Printer
     *  Simple functor, which decorates the another functor
     *  with simple printout abilities.
     *  It is very useful for debugging purposes
     *
     *  @code
     *
     *  // some functor
     *  const LoKi::Functor<TYPE1,TYPE2>& fun = ... ;
     *
     *  // good "monitored" functor:
     *  const LoKi::Monitoring::Printer<TYPE1,TYPE2> prnt =
     *     print ( fun , std::cout ) ;
     *
     *  @endcode
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2008-06-14
     */
    template <class TYPE, class TYPE2>
    class Printer : public LoKi::Functor<TYPE,TYPE2>
    {
    public:
      // ======================================================================
      /** constructor from the functor, stream, suffix and prefix
       *  @param fun the functor
       *  @param stream the stream
       *  @param suffix the suffix
       *  @param prefix the prefix
       */
      Printer ( LoKi::FunctorFromFunctor<TYPE,TYPE2> fun,
                std::ostream&        stream = std::cout ,
                const std::string&   suffix = "\n"      ,
                const std::string&   prefix = ""        )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun    ( std::move(fun) )
        , m_stream ( stream )
        , m_suffix ( suffix )
        , m_prefix ( prefix ) {}
      // ======================================================================
      /** constructor from the functor, stream, suffix and prefix
       *  @param fun the functor
       *  @param stream the stream
       *  @param suffix the suffix
       *  @param prefix the prefix
       */
      Printer ( std::ostream&      stream        ,
                LoKi::FunctorFromFunctor<TYPE,TYPE2> fun,
                const std::string& suffix = "\n" ,
                const std::string& prefix = ""   )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun    ( std::move(fun) )
        , m_stream ( stream )
        , m_suffix ( suffix )
        , m_prefix ( prefix ) {}
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Printer* clone() const override { return new Printer ( *this ) ; }
      /// MANDATORY: the only one essential method
      TYPE2 operator()
        ( typename LoKi::Functor<TYPE,TYPE2>::argument a ) const override
      {
        // evaluat ethe underlying functor:
        const auto result = m_fun.fun ( a )        ;
        // perform the actual printout
        m_stream << m_prefix << m_fun << "=" ;
        Gaudi::Utils::toStream ( result , m_stream ) ; // print the result
        m_stream << m_suffix       ;
        return result ;
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return m_fun.fillStream ( s ) ; }
      /// OPTIONAL: delegate ID:
      std::size_t id() const override { return m_fun.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" functor
      LoKi::FunctorFromFunctor<TYPE,TYPE2> m_fun ; // the "main" functor
      /// the reference for the stream
      std::ostream& m_stream ; // the reference for the stream
      /// the suffix
      std::string   m_suffix ; // the suffix
      /// the prefix
      std::string   m_prefix ; // the prefix
      // ======================================================================
    } ;
    // ========================================================================
    template <class TYPE2>
    class Printer<void,TYPE2> : public LoKi::Functor<void,TYPE2>
    {
    public:
      // ======================================================================
      /** constructor from the functor, stream, suffix and prefix
       *  @param fun the functor
       *  @param stream the stream
       *  @param suffix the suffix
       *  @param prefix the prefix
       */
      Printer ( LoKi::FunctorFromFunctor<void,TYPE2> fun,
                std::ostream&      stream = std::cout ,
                const std::string& suffix = "\n"      ,
                const std::string& prefix = ""        )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun    ( std::move(fun) )
        , m_stream ( stream )
        , m_suffix ( suffix )
        , m_prefix ( prefix ) {}
      // ======================================================================
      /** constructor from the functor, stream, suffix and prefix
       *  @param fun the functor
       *  @param stream the stream
       *  @param suffix the suffix
       *  @param prefix the prefix
       */
      Printer ( std::ostream&      stream        ,
                LoKi::FunctorFromFunctor<void,TYPE2> fun,
                const std::string& suffix = "\n" ,
                const std::string& prefix = ""   )
        : LoKi::AuxFunBase ( std::tie ( fun ) )
        , m_fun    ( std::move(fun) )
        , m_stream ( stream )
        , m_suffix ( suffix )
        , m_prefix ( prefix ) {}
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Printer* clone() const override { return new Printer ( *this ) ; }
      /// MANDATORY: the only one essential method
      TYPE2 operator() ( ) const override
      {
        // evaluate the underlying functor:
        const auto result = m_fun.fun ( /* a */ )        ;
        // perform the actual printout
        m_stream << m_prefix << m_fun << "=" ;
        Gaudi::Utils::toStream ( result , m_stream ) ; // print the result
        m_stream << m_suffix       ;
        return result ;
      }
      /// OPTIONAL: just a nice printout
      std::ostream& fillStream ( std::ostream& s ) const override
      { return m_fun.fillStream ( s ) ; }
      /// OPTIONAL: delegate ID:
      std::size_t id() const override { return m_fun.id() ; }
      // ======================================================================
    private:
      // ======================================================================
      /// the "main" functor
      LoKi::FunctorFromFunctor<void,TYPE2> m_fun ; // the "main" functor
      /// the reference for the stream
      std::ostream& m_stream ; // the reference for the stream
      /// the suffix
      std::string   m_suffix ; // the suffix
      /// the prefix
      std::string   m_prefix ; // the prefix
      // ======================================================================
    } ;
    // ========================================================================
 } // end of namespace LoKi::Monitoring
  // ==========================================================================
  /** helper function for creation of monitored predicate
   *
   *  @code
   *
   *  // some predicate:
   *  Cut cut = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring ) { cut = monitor ( cut , &counter("some counter") ) ; }
   *
   *
   *  @endcode
   *
   *  @see LoKi::Predicate
   *  @see LoKi::Monitoring::Counter
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Counter<TYPE,TYPE2> monitor( F&& cut , StatEntity* entity )
  { return { std::forward<F>(cut) , entity } ; }
  // ==========================================================================
  /** helper function for creation of monitored predicate
   *
   *  @code
   *
   *  // some predicate:
   *  Cut cut = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring ) { cut = monitor ( cut , counter("some counter") ) ; }
   *
   *
   *  @endcode
   *
   *  @see LoKi::Predicate
   *  @see LoKi::Monitoring::Counter
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Counter<TYPE,TYPE2> monitor( F&& cut, StatEntity& entity )
  { return { std::forward<F>(cut) , &entity } ; }
  // ==========================================================================
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Counter<TYPE,TYPE2>
  monitor ( F&& cut, const LoKi::CounterDef& cnt )
  { return { std::forward<F>(cut) , cnt } ; }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     AIDA::IHistogram1D* histo = ... ;
   *     fun = monitor ( fun , histo ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> monitor(F&& cut, AIDA::IHistogram1D* histo)
  { return { std::forward<F>(cut) , histo } ; }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     AIDA::IHistogram1D* histo = ... ;
   *     fun = monitor ( fun , histo ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> monitor(F&& cut, const LoKi::Histo& histo)
  { return { std::forward<F>(cut) , histo } ; }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     AIDA::IHistogram1D* histo = ... ;
   *     fun = plot ( fun , histo ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> plot ( F&& cut, AIDA::IHistogram1D* histo)
  { return { std::forward<F>(cut) , histo } ; }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     const std::string&       path = ... ;
   *     const Gaudi::Histo1DDef& desc = ... ;
   *     fun = plot ( fun , path , desc , histoSvc() ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @param cut the function to be monitored
   *  @param path the historgam path in HDS
   *  @param histo the histogram desctriptor
   *  @param hsvc the histogram service
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> plot ( F&&                      cut   ,
                                            const std::string&       path  ,
                                            const Gaudi::Histo1DDef& histo ,
                                            IHistogramSvc*           hsvc  )
  {
    return plot( std::forward<F>(cut), LoKi::HistoBook::book( path, histo, hsvc ) ) ;
  }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     const std::string&       dir = ... ;
   *     const std::string&       id  = ... ;
   *     const Gaudi::Histo1DDef& desc = ... ;
   *     fun = plot ( fun , path , desc ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @param cut the function to be monitored
   *  @param dir the historgam directory in HDS
   *  @param id  the histogram  ID
   *  @param histo the histogram descriptor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> plot ( F&&                      cut   ,
                                            const std::string&       dir   ,
                                            const GaudiAlg::ID&      id    ,
                                            const Gaudi::Histo1DDef& histo ,
                                            IHistogramSvc*           hsvc  = nullptr )
  {
    return plot( std::forward<F>(cut) ,
                 LoKi::HistoBook::book ( dir , id , histo , hsvc ) ) ;
  }
  // ==========================================================================
  /** helper function for creation of monitored function
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  // for monitoring mode, decorate it with self-monitoring abilities:
   *  if ( monitoring )
   *   {
   *     const std::string&       dir = ... ;
   *     const std::string&       id  = ... ;
   *     const Gaudi::Histo1DDef& desc = ... ;
   *     fun = plot ( fun , path , desc ) ; ;
   *   }
   *
   *
   *  @endcode
   *
   *  @param cut the function to be monitored
   *  @param dir the historgam directory in HDS
   *  @param id  the histogram  ID
   *  @param histo the histogram descriptor
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-06-14
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> plot ( F&&                      cut   ,
                                            const Gaudi::Histo1DDef& histo ,
                                            const GaudiAlg::ID&      id    ,
                                            const IAlgContextSvc*    csvc  = nullptr )
  {
    return plot ( std::forward<F>(cut),
                  LoKi::HistoBook::book ( histo , id , csvc ) ) ;
  }
  // ==========================================================================
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Plot<TYPE,TYPE2> plot ( F&& cut , const LoKi::Histo& hid )
  { return { std::forward<F>(cut) , hid } ; }
  // ==========================================================================
  /** helper function to instantiate the monitored functor
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  if  ( monitoring )
   *   {
   *     fun = print ( fun , std:cout , "\n" ) ;
   *   }
   *  @endcode
   *
   *  @see LoKi::Monitoring::Printer
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2008-02-29
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Printer<TYPE,TYPE2> print( F&&                fun                ,
                                               std::ostream&      stream = std::cout ,
                                               const std::string& suffix = "\n"      ,
                                               const std::string& prefix = ""        )
  {
    return { std::forward<F>(fun) , stream , suffix , prefix } ;
  }
  // ==========================================================================
  /** helper function to instantiate the monitored functor
   *
   *  @code
   *
   *  // some function
   *  Fun fun = ... ;
   *
   *  if  ( monitoring )
   *   {
   *     fun = monitor ( fun , std:cout , "\n" ) ;
   *   }
   *  @endcode
   *
   *  @see LoKi::Monitoring::Printer
   *  @see LoKi::print
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date 2008-02-29
   */
  template <typename F,
            typename TYPE  = details::type1_t<F>,
            typename TYPE2 = details::type2_t<F>>
  LoKi::Monitoring::Printer<TYPE,TYPE2> monitor( F&&                fun                ,
                                                 std::ostream&      stream = std::cout ,
                                                 const std::string& suffix = "\n"      ,
                                                 const std::string& prefix = ""        )
  {
    return print ( std::forward<F>(fun) , stream , suffix , prefix ) ;
  }
  // ==========================================================================
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_MONITORING_H
// ============================================================================
