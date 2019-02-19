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
#ifndef LOKI_FILTERS_H
#define LOKI_FILTERS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD &STL
// ============================================================================
#include <algorithm>
#include <climits>
#include <optional>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToStream.h"
// ============================================================================
// LHCb
// ============================================================================
#include "LHCbMath/LHCbMath.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Algs.h"
#include "LoKi/Dump.h"
#include "LoKi/FirstN.h"
#include "LoKi/Functions.h"
#include "LoKi/Functor.h"
#include "LoKi/Primitives.h"
#include "LoKi/Timers.h"
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
 *  @date 2007-10-30
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::Functors
   *  helper namespace for implementation of Gerhard's ideas
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
   *  @date 2007-10-30
   */
  namespace Functors {
    // ========================================================================
    /** @class Select
     *
     *  Simple functor for "filtering" or "selection".
     *
     *  @code
     *
     *   typedef Select<LHCb::Track*,LHCb::Track> SELECT ;
     *   // the selection criteria (predicate)  itself
     *   const LoKi::Functor<LHCb::Track,bool>& good = ... ;
     *   std::vector<LHCb::Track*> input = ... ;
     *   std::vector<LHCb::Track*> filtered = input >> SELECT ( good )  ;
     *
     *  @endcode
     *
     *  The concept belongs to Gerhard Raven.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-02
     */
    template <typename TYPE, typename TYPE2 = TYPE>
    class Select final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // ======================================================================
      /// constructor form the basic predicate:
      Select( LoKi::FunctorFromFunctor<TYPE2, bool> fun )
          : LoKi::AuxFunBase( std::tie( fun ) ), m_predicate( std::move( fun ) ) {}
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Select* clone() const override { return new Select( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        std::vector<TYPE> r;
        r.reserve( a.size() );
        std::copy_if( a.begin(), a.end(), std::back_inserter( r ),
                      [&]( const TYPE& arg ) { return LoKi::apply( m_predicate.func(), arg ); } );
        return r;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "select(" << m_predicate << ")"; }
      // ======================================================================
    private:
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<TYPE2, bool> m_predicate; // the functor itself
      // ======================================================================
    };
    // ========================================================================
    /** @class Yields
     *
     *  Simple functor for "mapping" of the containers.
     *
     *  @code
     *
     *   typedef Yields<LHCb::Track*,double,LHCb::Track> YIELD ;
     *   const LoKi::Functor<LHCb::Track,double>& pt = ... ;
     *   std::vector<LHCb::Track*> input = ... ;
     *   std::vector<double>       results = input >> YIELD( pt )  ;
     *
     *  @endcode
     *
     *  The concept belongs to Gerhard Raven and the
     *  name comes from Marcel Merk.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-02
     */
    template <typename TYPE, typename TYPE1, typename TYPE2 = TYPE>
    class Yields final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE1>> {
    public:
      // ======================================================================
      /// contructor from the basic functor
      Yields( LoKi::FunctorFromFunctor<TYPE2, TYPE1> fun )
          : LoKi::AuxFunBase( std::tie( fun ) ), m_functor( std::move( fun ) ) {}
      // ======================================================================
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Yields* clone() const override { return new Yields( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE1> operator()( const std::vector<TYPE>& a ) const override {
        std::vector<TYPE1> out;
        out.reserve( a.size() );
        LoKi::apply( a.begin(), a.end(), m_functor.func(), std::back_inserter( out ) );
        return out;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "yields(" << m_functor << ")"; };
      // ======================================================================
    private:
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<TYPE2, TYPE1> m_functor; // the functor itself
      // ======================================================================
    };
    // ========================================================================
    /** @class Process
     *
     *  Simple functor for "processing" of the containers.
     *
     *  @code
     *
     *    typedef Process<LHCb::Track*,double,LHCb::Track> PROCESS ;
     *    // get some functor for plotting the TrPT
     *    AIDA::IHistogram1D* histo = ...
     *    FunctorFromFunctor<LHCb::Track,double> plotter = monitor ( TrPT , histo )  ;
     *    std::vector<LHCb::Track*> input = ... ;
     *    input >> PROCESS( plotter ) ;
     *
     *  @endcode
     *
     *  The concept belongs to Gerhard Raven.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-02
     */
    template <typename TYPE, typename TYPE1, typename TYPE2 = TYPE>
    class Process final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // ======================================================================
      /// constructor form the basic predicate:
      Process( LoKi::FunctorFromFunctor<TYPE2, TYPE1> fun )
          : LoKi::AuxFunBase( std::tie( fun ) ), m_functor( std::move( fun ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Process* clone() const override { return new Process( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        LoKi::apply( a.begin(), a.end(), m_functor.func() );
        return a;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "process(" << m_functor << ")"; };
      // ======================================================================
    private:
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<TYPE2, TYPE1> m_functor; // the functor itself
      // ======================================================================
    };
    // ========================================================================
    /** @class Tee
     *  Simple functor for "tee" of the containers.
     *
     *  @code
     *
     *   // get some functor for plotting the TrPT
     *   AIDA::IHistogram1D* histo = ...
     *   FunctorFromFunctor<LHCb::Track,double> plotter = monitor ( TrPT , histo )  ;
     *   // get some functor for selection of track with maximal pt
     *   MaxElement<LHCb::Track*>  maxPt ( TrPT ) ;
     *   // container of input tracks:
     *   std::vector<LHCb::Track*> input = ... ;
     *   // "tee"
     *   input >> Tee<LHCb::Track*> ( compose ( maxPt , plotter ) ) ;
     *
     *  @endcode
     *
     *  The concept belongs to Gerhard Raven.
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-02
     */
    template <typename TYPE, typename TYPE2 = std::vector<TYPE>>
    class Tee final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // ======================================================================
      /// constructor form the basic predicate:
      Tee( LoKi::FunctorFromFunctor<std::vector<TYPE>, TYPE2> fun )
          : LoKi::AuxFunBase( std::tie( fun ) ), m_functor( std::move( fun ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Tee* clone() const override { return new Tee( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        std::vector<TYPE> out = a;
        LoKi::apply( m_functor.func(), a );
        return out;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "tee(" << m_functor << ")"; };
      // ======================================================================
    private:
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<std::vector<TYPE>, TYPE2> m_functor; //  functor
      // ======================================================================
    };
    // ========================================================================
    /** @class Extremum
     *  simple meta-functor to find an extreme value
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    template <typename TYPE, typename TYPE2, typename TYPE1, typename Traits_>
    class Extremum final : public LoKi::Functor<std::vector<TYPE>, TYPE1> {
      static_assert( std::numeric_limits<TYPE1>::is_specialized, "TYPE1 must be numeric" );
      // ======================================================================
    public:
      // ======================================================================
      ////TODO: in case TYPE2==TYPE1, add a c'tor where fun is not specified,
      ///       in which case it defaults to identity
      /// constructor
      Extremum( LoKi::FunctorFromFunctor<TYPE2, TYPE1> fun, const TYPE1 val = Traits_::initial_value() )
          : LoKi::AuxFunBase( std::tie( fun, val ) ), m_functor( std::move( fun ) ), m_value( val ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Extremum* clone() const override { return new Extremum( *this ); }
      /// MANDATORY: the only one essential method
      TYPE1 operator()( const std::vector<TYPE>& a ) const override {
        // return the value
        if ( a.empty() ) { return m_value; } // RETURN
        //
        auto res =
            LoKi::Algs::extremum( a.begin(), a.end(), LoKi::Apply( this->func() ), typename Traits_::BinaryOp{} );
        Assert( a.end() != res.first, "Empty input container!" );
        return res.second;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        s << " " << Traits_::name() << "(" << this->func();
        if ( Traits_::initial_value() != m_value ) { s << "," << m_value; }
        return s << ") ";
      }
      // ======================================================================
    private:
      // ======================================================================
      /// get the functor
      const auto& func() const { return m_functor.func(); }
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<TYPE2, TYPE1> m_functor; // the functor
      /// the value
      TYPE1 m_value; //   the value
      // ======================================================================
    };

    // ========================================================================
    /** @class Max
     *  simple instance of extremum meta-functor to find the maximum
     */

    namespace Traits {
      template <typename TYPE1>
      struct Max {
        using BinaryOp = std::greater<>;
        static constexpr TYPE1       intial_value() { return -std::numeric_limits<TYPE1>::max(); }
        static constexpr const char* name() { return "max_value"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using Max = Extremum<TYPE, TYPE2, TYPE1, Traits::Max<TYPE1>>;

    // ========================================================================
    /** @class AbsMax
     *  simple meta-functor to find the maximum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    namespace Traits {
      template <typename TYPE1>
      struct AbsMax {
        using BinaryOp = LHCb::Math::abs_greater<TYPE1>;
        static constexpr TYPE1       initial_value() { return -1; }
        static constexpr const char* name() { return "max_abs_value"; }
      };
    } // namespace Traits

    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using AbsMax = Extremum<TYPE, TYPE2, TYPE1, Traits::AbsMax<TYPE1>>;

    // ========================================================================
    /** @class Min
     *  simple meta-functor to find the minimum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */

    namespace Traits {
      template <typename TYPE1>
      struct Min {
        using BinaryOp = std::less<>;
        static constexpr TYPE1       initial_value() { return std::numeric_limits<TYPE1>::max(); }
        static constexpr const char* name() { return "min_value"; }
      };
    } // namespace Traits

    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using Min = Extremum<TYPE, TYPE2, TYPE1, Traits::Min<TYPE1>>;

    // ========================================================================
    /** @class AbsMin
     *  simple meta-functor to find the minimum
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    namespace Traits {
      template <typename TYPE1>
      struct AbsMin {
        using BinaryOp = LHCb::Math::abs_less<TYPE1>;
        static constexpr TYPE1       initial_value() { return std::numeric_limits<TYPE1>::max(); }
        static constexpr const char* name() { return "min_abs_value"; }
      };
    } // namespace Traits

    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using AbsMin = Extremum<TYPE, TYPE2, TYPE1, Traits::AbsMin<TYPE1>>;

    // ========================================================================
    /** @class ExtremeElement
     *  simple meta-functor to find the maximal element
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    template <typename TYPE, typename TYPE2, typename TYPE1, typename Traits_>
    class ExtremeElement final : public LoKi::BasicFunctors<TYPE>::Pipe {
    public:
      // ======================================================================
      /// the base
      typedef typename LoKi::BasicFunctors<TYPE>::Pipe uBase; //   the base
      // ======================================================================
    public:
      // ======================================================================
      /// constructor
      ExtremeElement( LoKi::FunctorFromFunctor<TYPE2, TYPE1> fun )
          : LoKi::AuxFunBase( std::tie( fun ) ), m_functor( std::move( fun ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      ExtremeElement* clone() const override { return new ExtremeElement( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        //
        if ( a.empty() ) { return a; }
        //
        auto res =
            LoKi::Algs::extremum( a.begin(), a.end(), LoKi::Apply( m_functor.func() ), typename Traits_::BinaryOp{} );
        Assert( res.first != a.end(), "Empty input container!" );
        return {*( res.first )};
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << Traits_::name() << "(" << m_functor << ")";
      };
      // ======================================================================
    private:
      // ======================================================================
      /// the basic functor itself
      LoKi::FunctorFromFunctor<TYPE2, TYPE1> m_functor; // the functor
      // ======================================================================
    };

    // ========================================================================
    /** @class MaxElement
     *  simple meta-functor to find the maximal element
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    namespace Traits {
      struct MaxElement {
        using BinaryOp = std::greater<>;
        static constexpr const char* name() { return "max_element"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using MaxElement = ExtremeElement<TYPE, TYPE2, TYPE1, Traits::MaxElement>;

    // ========================================================================
    /** @class MinElement
     *  simple meta-functor to find the minimal element
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    namespace Traits {
      struct MinElement {
        using BinaryOp = std::less<>;
        static constexpr const char* name() { return " min_element"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using MinElement = ExtremeElement<TYPE, TYPE2, TYPE1, Traits::MinElement>;

    // ========================================================================
    /** @class AbsMaxElement
     *  simple meta-functor to find the maximal element
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */
    namespace Traits {
      template <typename TYPE1>
      struct AbsMaxElement {
        using BinaryOp = LHCb::Math::abs_greater<TYPE1>;
        static constexpr const char* name() { return " max_abs_element"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using AbsMaxElement = ExtremeElement<TYPE, TYPE2, TYPE1, Traits::AbsMaxElement<TYPE1>>;

    // ========================================================================
    /** @class AbsMinElement
     *  simple meta-functor to find the minimal element
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-11-26
     */

    namespace Traits {
      template <typename TYPE1>
      struct AbsMinElement {
        using BinaryOp = LHCb::Math::abs_less<TYPE1>;
        static constexpr const char* name() { return "min_abs_element"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2 = TYPE, typename TYPE1 = double>
    using AbsMinElement = ExtremeElement<TYPE, TYPE2, TYPE1, Traits::AbsMinElement<TYPE1>>;

    // ========================================================================
    template <typename TYPE, typename TYPE1 = TYPE, typename TYPE2 = bool>
    class Count final : public LoKi::Functor<std::vector<TYPE>, double> {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the predicate
      Count( LoKi::FunctorFromFunctor<TYPE1, TYPE2> cut )
          : LoKi::AuxFunBase( std::tie( cut ) ), m_cut( std::move( cut ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Count* clone() const override { return new Count( *this ); }
      /// MANDATORY: the only one essential method:
      double operator()( const std::vector<TYPE>& a ) const override {
        return std::count_if( a.begin(), a.end(), LoKi::Apply( m_cut.func() ) );
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "count(" << m_cut << ")"; }
      // ======================================================================
    private:
      // ======================================================================
      /// the predicate
      LoKi::FunctorFromFunctor<TYPE1, TYPE2> m_cut; // the predicate
      // ======================================================================
    };

    namespace details {
      // ========================================================================
      /** @class Accumulate
       *  accumulate afunction over the stream
       *  @see LoKi::Algs::accumulate
       *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
       *  @date 2011-02-27
       */
      template <typename TYPE, typename Traits_>
      class Accumulate : public LoKi::Functor<std::vector<TYPE>, double> {
      public:
        // ======================================================================
        /** constructor from the function
         *  @param fun the function
         *  @param init the initial value for summation
         */
        Accumulate( LoKi::FunctorFromFunctor<TYPE, double> fun, const double init = Traits_::unit() )
            : LoKi::AuxFunBase( std::tie( fun, init ) ), m_fun( std::move( fun ) ), m_init( init ) {}
        /** constructor from the function
         *  @param fun the function
         *  @param init the initial value for summation
         */
        Accumulate( LoKi::FunctorFromFunctor<TYPE, double> fun, LoKi::FunctorFromFunctor<TYPE, bool> cut,
                    double init = Traits_::unit() )
            : LoKi::AuxFunBase( std::tie( fun, cut, init ) )
            , m_fun( std::move( fun ) )
            , m_cut( std::move( cut ) )
            , m_init( init ) {}
        /// MANDATORY: clone method ("virtual constructor")
        Accumulate* clone() const override { return new Accumulate( *this ); }
        /// MANDATORY: the only one essential method:
        double operator()( const std::vector<TYPE>& a ) const override {
          typename Traits_::BinaryOperation operation{};
          //
          return m_cut ? LoKi::Algs::accumulate( a.begin(), a.end(), LoKi::Apply( m_fun.func() ),
                                                 LoKi::Apply( m_cut->func() ), this->m_init, operation )
                       : LoKi::Algs::accumulate( a.begin(), a.end(), LoKi::Apply( m_fun.func() ), this->m_init,
                                                 operation );
        }
        /// OPTIONAL: the basic printout method
        std::ostream& fillStream( std::ostream& s ) const override {
          s << Traits_::name() << "(" << this->m_fun;
          if ( this->m_cut ) { s << "." << *( this->m_cut ); }
          if ( this->m_init != Traits_::unit() ) { s << "," << this->m_init; }
          return s << ")";
        }
        // ======================================================================
      protected:
        // ======================================================================
        /// OPTIONAL: the basic printout method
        std::ostream& _print_( std::ostream& s, const std::string& n, const double d ) const {
          s << n << "(" << this->m_fun;
          if ( this->m_cut ) { s << "." << *( this->m_cut ); }
          if ( d != this->m_init ) { s << "," << this->m_init; }
          return s << ")";
        }
        // ======================================================================
        /// the function
        LoKi::FunctorFromFunctor<TYPE, double>              m_fun;  // the function
        std::optional<LoKi::FunctorFromFunctor<TYPE, bool>> m_cut;  // the cut
        double                                              m_init; // init-value
        // ======================================================================
      };
    } // namespace details
    // ========================================================================
    /** @class Sum
     *  sum of the function over the stream
     *  @see LoKi::Algs::accumulate
     *  @author Vanya BELYAEV Ivan.BElyaev@cern.ch
     *  @date 2011-02-27
     */
    namespace Traits {
      struct Sum {
        using BinaryOperation = std::plus<double>;
        static constexpr double      unit() { return 0; }
        static constexpr const char* name() { return "sum"; }
      };
    } // namespace Traits
    template <typename TYPE>
    using Sum = details::Accumulate<TYPE, Traits::Sum>;

    // ========================================================================
    /** @class Product
     *  product of the function over the stream
     *  @see LoKi::Algs::accumulate
     *  @see LoKi::Functors::Sum
     *  @author Vanya BELYAEV Ivan.BElyaev@cern.ch
     *  @date 2011-02-27
     */

    namespace Traits {
      struct Product {
        using BinaryOperation = std::multiplies<>;
        static constexpr double      unit() { return 1; }
        static constexpr const char* name() { return "product"; }
      };
    } // namespace Traits
    template <typename TYPE>
    using Product = details::Accumulate<TYPE, Traits::Product>;

    // ========================================================================
    /** @class Fetch
     *  Fetch & evaluate the certain element of the stream
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2011-03-27
     */
    template <typename TYPE, typename TYPE2>
    class Fetch final : public LoKi::Functor<std::vector<TYPE>, TYPE2> {
    public:
      // ======================================================================
      /// constructor from functor, index and bad-value
      Fetch( LoKi::Assignable_t<LoKi::Functor<TYPE, TYPE2>> fun, const unsigned int index, const TYPE2 bad )
          : LoKi::AuxFunBase( std::tie( fun, index, bad ) )
          , m_fun( std::move( fun ) )
          , m_index( index )
          , m_bad( bad ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Fetch* clone() const override { return new Fetch( *this ); }
      /// MANDATORY: the only one essential method:
      TYPE2 operator()( const std::vector<TYPE>& a ) const override {
        if ( a.size() < m_index + 1 ) {
          this->Error( "Input vector is too short, return 'bad'" );
          return this->m_bad;
        }
        // evaluate the functor:
        return this->m_fun.fun( a[this->m_index] );
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the functor itself
      LoKi::Assignable_t<LoKi::Functor<TYPE, TYPE2>> m_fun;
      /// the index
      unsigned int m_index; // the index
      /// bad value
      TYPE2 m_bad; // the bad value
      // ======================================================================
    };
    // ========================================================================
    /** @class Has
     *  check the presence of good elemements in stream
     */
    template <typename TYPE, typename TYPE1 = TYPE, typename TYPE2 = bool>
    class Has final : public LoKi::Functor<std::vector<TYPE>, bool> {
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the predicate
      Has( LoKi::FunctorFromFunctor<TYPE1, TYPE2> cut )
          : LoKi::AuxFunBase( std::tie( cut ) ), m_cut( std::move( cut ) ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Has* clone() const override { return new Has( *this ); }
      /// MANDATORY: the only one essential method:
      bool operator()( const std::vector<TYPE>& a ) const override {
        return std::any_of( a.begin(), a.end(), LoKi::Apply( m_cut.func() ) );
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "has(" << m_cut << ")"; }
      // ======================================================================
    private:
      // ======================================================================
      /// the predicate
      LoKi::FunctorFromFunctor<TYPE1, TYPE2> m_cut; // the predicate
      // ======================================================================
    };
    // ========================================================================
    /** @class Empty
     *  Simple functor which checks the emptiness of the container
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-11-20
     */
    template <typename TYPE>
    struct Empty final : LoKi::Functor<std::vector<TYPE>, bool> {
      // ======================================================================
      Empty() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method ("virtual constructor")
      Empty* clone() const override { return new Empty( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( const std::vector<TYPE>& a ) const override { return a.empty(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Size
     *  Somple functor which checks the size of the container
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-11-20
     */
    template <typename TYPE>
    struct Size final : LoKi::Functor<std::vector<TYPE>, double> {
      // ======================================================================
      Size() : AuxFunBase( std::tie() ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Size* clone() const override { return new Size( *this ); }
      /// MANDATORY: the only one essential method
      double operator()( const std::vector<TYPE>& a ) const override { return a.size(); }
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };

    namespace details {
      // ========================================================================
      /** @class ComposeFunctions
       *  Facade to adapt the composition of the functions
       *  @author Gerhard Raven gerhard.raven@nikhef.nl
       *  @date 2016-08-13
       */

      template <typename TYPE, typename TYPE2, typename COMPOSE>
      class ComposeFunctions final : public LoKi::Functor<TYPE, std::vector<TYPE2>> {
      public:
        // ======================================================================
        /// constructor from two streamers
        ComposeFunctions( LoKi::FunctorFromFunctor<TYPE, std::vector<TYPE2>> fun1,
                          LoKi::FunctorFromFunctor<TYPE, std::vector<TYPE2>> fun2 )
            : LoKi::AuxFunBase( std::tie( fun1, fun2 ) ), m_two( std::move( fun1 ), std::move( fun2 ) ) {}
        /// MANDATORY: clone method ("virtual constructor")
        ComposeFunctions* clone() const override { return new ComposeFunctions( *this ); }
        /// MANDATORY: the only one essential method
        std::vector<TYPE2> operator()( const TYPE& a ) const override {
          return COMPOSE::binaryOp( m_two.func1(), m_two.func2(), a );
        }
        /// OPTIONAL: nice printout
        std::ostream& fillStream( std::ostream& s ) const override {
          return COMPOSE::fillStream( s, m_two.func1(), m_two.func2() );
        }
        // ======================================================================
      private:
        // ======================================================================
        /// the holder for two functors
        LoKi::TwoFunctors<TYPE, std::vector<TYPE2>> m_two; // two functors
        // ======================================================================
      };

      template <typename TYPE2, typename COMPOSE>
      class ComposeFunctions<void, TYPE2, COMPOSE> final : public LoKi::Functor<void, std::vector<TYPE2>> {
      public:
        // ======================================================================
        /// constructor from two streamers
        ComposeFunctions( LoKi::FunctorFromFunctor<void, std::vector<TYPE2>> fun1,
                          LoKi::FunctorFromFunctor<void, std::vector<TYPE2>> fun2 )
            : LoKi::AuxFunBase( std::tie( fun1, fun2 ) ), m_two( std::move( fun1 ), std::move( fun2 ) ) {}
        /// MANDATORY: clone method ("virtual constructor")
        ComposeFunctions* clone() const override { return new ComposeFunctions( *this ); }
        /// MANDATORY: the only one essential method
        std::vector<TYPE2> operator()() const override { return COMPOSE::binaryOp( m_two.func1(), m_two.func2() ); }
        /// OPTIONAL: nice printout
        std::ostream& fillStream( std::ostream& s ) const override {
          return s << " " << COMPOSE::name() << "(" << m_two.func1() << "," << m_two.func2() << ") ";
        }
        // ======================================================================
      protected:
        // ======================================================================
        /// the holder for two functors
        LoKi::TwoFunctors<void, std::vector<TYPE2>> m_two; // two functors
        // ======================================================================
      };

      template <typename Prefix>
      struct BinaryCompose {
        template <typename F1, typename F2>
        static std::ostream& fillStream( std::ostream& s, const F1& f1, const F2& f2 ) {
          return s << " " << Prefix::name() << "(" << f1 << "," << f2 << ") ";
        }
      };
    } // namespace details

    // ========================================================================
    /** @class Union
     *  simle functor to represent the "union" for two vector-functors
     *  @see LoKi::Operations::Union
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */
    namespace Traits {
      template <typename TYPE2>
      struct Union : details::BinaryCompose<Union<TYPE2>> {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, Args&&... args ) {
          LoKi::Operations::Union<TYPE2> _union;
          return _union( f1( args... ), f2( args... ) );
        }
        constexpr static const char* name() { return "union"; }
      };
    } // namespace Traits

    template <typename TYPE, typename TYPE2>
    using Union = details::ComposeFunctions<TYPE, TYPE2, Traits::Union<TYPE2>>;

    // ========================================================================
    /** @class NoEmptyUnion
     *  simple functor to represent the "union" for two NON-empty vector-functors
     *  Result is empty if one of inputs is empty
     *  @see LoKi::Operations::NoEmptyUnion
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */
    namespace Traits {
      template <typename TYPE2>
      struct NoEmptyUnion : details::BinaryCompose<NoEmptyUnion<TYPE2>> {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, Args&&... args ) {
          auto r1 = f1( args... );
          if ( r1.empty() ) { return r1; } // RETURN EMPTY
          auto r2 = f2( args... );
          if ( r2.empty() ) { return r2; } // RETURN EMPTY
          LoKi::Operations::NoEmptyUnion<TYPE2> _union;
          return _union( r1, r2 );
        }
        static constexpr const char* name() { return "no_empty_union"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2>
    using NoEmptyUnion = details::ComposeFunctions<TYPE, TYPE2, Traits::NoEmptyUnion<TYPE2>>;

    // ========================================================================
    /** @class Difference
     *  simle functor to represent the "difference" for two vector-functors
     *  @see LoKi::Operations::Difference
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */

    namespace Traits {
      template <typename TYPE2>
      struct Difference : details::BinaryCompose<Difference<TYPE2>> {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, Args&&... args ) {
          auto r1 = f1( args... );
          if ( r1.empty() ) { return r1; } // RETURN
          LoKi::Operations::Difference<TYPE2> _diff;
          return _diff( r1, f2( args... ) );
        }
        static constexpr const char* name() { return "difference"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2>
    using Difference = details::ComposeFunctions<TYPE, TYPE2, Traits::Difference<TYPE2>>;

    // ========================================================================
    /** @class Intersection
     *  simle functor to represent the "intersection" for two vector-functors
     *  @see LoKi::Operations::Intersection
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */

    namespace Traits {
      template <typename TYPE2>
      struct Intersection : details::BinaryCompose<Intersection<TYPE2>> {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, Args&&... args ) {
          LoKi::Operations::Intersection<TYPE2> _intersection;
          return _intersection( f1( args... ), f2( args... ) );
        }
        static constexpr const char* name() { return "intersection"; }
      };
    } // namespace Traits

    template <typename TYPE, typename TYPE2>
    using Intersection = details::ComposeFunctions<TYPE, TYPE2, Traits::Intersection<TYPE2>>;

    // ========================================================================
    /** @class SymDifference
     *  simle functor to represent the "symmetric-difference" for two vector-functors
     *  @see LoKi::Operations::SymDifference
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */
    namespace Traits {
      template <typename TYPE2>
      struct SymDifference : details::BinaryCompose<SymDifference<TYPE2>> {
        template <typename F1, typename F2, typename... Args>
        static auto binaryOp( const F1& f1, const F2& f2, Args&&... args ) {
          LoKi::Operations::SymDifference<TYPE2> _symdiff;
          return _symdiff( f1( args... ), f2( args... ) );
        }
        static constexpr const char* name() { return "sym_difference"; }
      };
    } // namespace Traits
    template <typename TYPE, typename TYPE2>
    using SymDifference = details::ComposeFunctions<TYPE, TYPE2, Traits::SymDifference<TYPE2>>;

    // ========================================================================
    /** @class Includes
     *  simle functor to represent the "includes" for two vector-functors
     *  @see LoKi::Operations::Includes
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-06
     */
    template <typename TYPE, typename TYPE2>
    class Includes final : public LoKi::Functor<TYPE, bool> {
    public:
      // ======================================================================
      /// constructor from two streamers
      Includes( LoKi::FunctorFromFunctor<TYPE, std::vector<TYPE2>> fun1,
                LoKi::FunctorFromFunctor<TYPE, std::vector<TYPE2>> fun2 )
          : LoKi::AuxFunBase( std::tie( fun1, fun2 ) ), m_two( std::move( fun1 ), std::move( fun2 ) ) {}
      /// MANDATORY: clone method ("virtual consturctor")
      Includes* clone() const override { return new Includes( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()( typename LoKi::Functor<TYPE, bool>::argument a ) const override {
        LoKi::Operations::Includes<TYPE2> _includes{};
        return _includes( m_two.fun1( a ), m_two.fun2( a ) );
      }
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " includes(" << m_two.func1() << "," << m_two.func2() << ") ";
      }
      // ======================================================================
    private:
      // ======================================================================
      /// storage of two functors
      LoKi::TwoFunctors<TYPE, std::vector<TYPE2>> m_two; // two functors
      // ======================================================================
    };
    // ========================================================================
    template <typename TYPE2>
    class Includes<void, TYPE2> final : public LoKi::Functor<void, bool> {
    public:
      // ======================================================================
      /// constructor from two streamers
      Includes( LoKi::FunctorFromFunctor<void, std::vector<TYPE2>> fun1,
                LoKi::FunctorFromFunctor<void, std::vector<TYPE2>> fun2 )
          : LoKi::AuxFunBase( std::tie( fun1, fun2 ) ), m_two( std::move( fun1 ), std::move( fun2 ) ) {}
      /// MANDATORY: clone method ("virtual consturctor")
      Includes* clone() const override { return new Includes( *this ); }
      /// MANDATORY: the only one essential method
      bool operator()() const override {
        LoKi::Operations::Includes<TYPE2> _includes;
        return _includes( m_two.fun1(), m_two.fun2() );
      }
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " includes(" << m_two.func1() << "," << m_two.func2() << ") ";
      }
      // ======================================================================
    private:
      // ======================================================================
      /// storage of two functors
      LoKi::TwoFunctors<void, std::vector<TYPE2>> m_two; // two functors
      // ======================================================================
    };
    // ========================================================================
    /** @class Unique
     *  simple function to remove duplicated elemens from the stream
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @date 2010-06-05
     */
    template <typename TYPE>
    struct Unique final : LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Unique* clone() const override { return new Unique( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        LoKi::Operations::Unique<TYPE> _unique;
        return _unique( a );
      }
      // ======================================================================
      /// OPTIONAL: nice printout
      virtual std::ostream& fillStream( std::ostream& s ) const;
      // ======================================================================
    };

    // ========================================================================
    /** @class Gate
     *  Helper class to control the flow of objects.
     *  Objects flows only if the gate is open.
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-11-17
     */
    template <typename TYPE>
    class Gate final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // =====================================================================
      /// the constructor from the stopper
      Gate( LoKi::FunctorFromFunctor<void, bool> gate )
          : LoKi::AuxFunBase( std::tie( gate ) ), m_gate( std::move( gate ) ) {}
      /// MANDATORY: clone method("virtual constructor")
      Gate* clone() const override { return new Gate( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        // run stopper only for non-empty containers
        // and stop the data flow if needed
        return ( a.empty() || m_gate() ) ? a : std::vector<TYPE>{};
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "gate(" << m_gate << ")"; }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual gate
      LoKi::FunctorFromFunctor<void, bool> m_gate; // the gate
      // ======================================================================
    };
    // ========================================================================
    /** @class Cause
     *  Helper class for ``conditional source''
     *  @thanks Roel AAIJ
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-11-18
     */
    template <typename TYPE>
    class Cause final : public LoKi::Functor<void, std::vector<TYPE>> {
    public:
      // =====================================================================
      /// the constructor from the start & source
      Cause( LoKi::FunctorFromFunctor<void, bool> start, LoKi::FunctorFromFunctor<void, std::vector<TYPE>> source )
          : LoKi::AuxFunBase( std::tie( source, start ) )
          , m_start( std::move( start ) )
          , m_source( std::move( source ) ) {}
      /// the constructor from the source & start
      Cause( LoKi::FunctorFromFunctor<void, std::vector<TYPE>> source, LoKi::FunctorFromFunctor<void, bool> start )
          : Cause( std::move( start ), std::move( source ) ) {}
      /// MANDATORY: clone method("virtual constructor")
      Cause* clone() const override { return new Cause( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()() const override {
        // invoke the source if enabled, else empty container
        return m_start() ? m_source() : std::vector<TYPE>{};
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        return s << " cause( " << m_start << "," << m_source << ") ";
      }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual starter
      LoKi::FunctorFromFunctor<void, bool> m_start; // the start
      /// the actual source
      LoKi::FunctorFromFunctor<void, std::vector<TYPE>> m_source; // the source
      // ======================================================================
    };
    // ========================================================================
    /** @class FirstN_
     *  get the first N-elements from the stream
     *  @thanks Roel AAIJ
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-11-18
     */
    template <typename TYPE>
    class FirstN_ final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // =====================================================================
      /// the constructor
      explicit FirstN_( const unsigned int N ) : LoKi::AuxFunBase( std::tie( N ) ), m_N( N ) {}
      /// the constructor
      explicit FirstN_( const LoKi::FirstN& N ) : LoKi::AuxFunBase( std::tie( N ) ), m_N( N ) {}
      /// MANDATORY: clone method("virtual constructor")
      FirstN_* clone() const override { return new FirstN_( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        auto              n = std::min( static_cast<std::size_t>( m_N.n() ), a.size() );
        std::vector<TYPE> r;
        r.reserve( n );
        std::copy_n( a.begin(), n, std::back_inserter( r ) );
        return r;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// number of elements to be selected :
      LoKi::FirstN m_N; // number of elements to be selected
      // ======================================================================
    };
    // ========================================================================
    /** @class Reverse_
     *  reverse order of elements in the stream
     *  @thanks Roel AAIJ
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-11-18
     */
    template <typename TYPE>
    struct Reverse_ final : LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
      // =========================================================================
      /// the constructor
      Reverse_() : AuxFunBase{std::tie()} {}
      /// MANDATORY: clone method("virtual constructor")
      Reverse_* clone() const override { return new Reverse_( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override { return {a.rbegin(), a.rend()}; }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Sort_
     *  get the first N-element of sorted input sequence
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2011-02-27
     */
    template <typename TYPE, typename TYPE2 = double, bool ASCENDING = true>
    class Sort_ final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // ======================================================================
      /** constructor from the function
       *  @param fun the function
       *  @param init the initial value for summation
       */
      Sort_( LoKi::FunctorFromFunctor<TYPE, TYPE2> fun, int N = -1 )
          : LoKi::AuxFunBase( std::tie( fun, N ) ), m_cmp( std::move( fun ) ), m_sort( N, ASCENDING ) {}
      /// MANDATORY: clone method ("virtual constructor")
      Sort_* clone() const override { return new Sort_( *this ); }
      /// MANDATORY: the only one essential method:
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override {
        // nothing to select => nothing to sort
        if ( 0 == this->N() ) { return {}; }

        auto r = a;
        if ( this->all() || (unsigned)this->N() >= a.size() ) {
          std::sort( r.begin(), r.end(), m_cmp );
        } else {
          std::partial_sort( r.begin(), std::next( r.begin(), N() ), r.end(), m_cmp );
          r.resize( N() );
        }
        return r;
      }
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override {
        s << " sort ( " << this->m_cmp.func1();
        //
        if ( ASCENDING && this->m_sort.all() ) {
          return s << ") ";
        } else if ( ASCENDING ) {
          return s << ", " << this->N() << ") ";
        }
        //
        return s << ", " << this->N() << ", False) ";
      }
      // ======================================================================
    public:
      // ======================================================================
      int  N() const { return this->m_sort.N(); }
      bool all() const { return this->m_sort.all(); }
      // ======================================================================
    protected:
      // ======================================================================
      /// the comparison criteria
      typename LoKi::Cmp<TYPE, TYPE2, ASCENDING>::Type m_cmp; // the comparison criteria
      /// sorting options
      LoKi::Sort m_sort;
      // ======================================================================
    };
    // ========================================================================
    /** @class Dump_
     *  Helper class to dump the content of the flow
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2010-11-17
     */
    template <typename TYPE>
    class Dump_ final : public LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>> {
    public:
      // =====================================================================
      /// the constructor
      Dump_( const std::string& open = "", const std::string& close = "\n", std::ostream& stream = std::cout )
          : LoKi::AuxFunBase( std::tie( open, close ) ), m_stream( stream ), m_dump( open, close ) {}
      /// the constructor
      Dump_( std::ostream& stream, const std::string& open = "", const std::string& close = "\n" )
          : LoKi::AuxFunBase( std::tie( open, close ) ), m_stream( stream ), m_dump( open, close ) {}
      /// the constructor from the stopper
      Dump_( std::ostream& stream, const LoKi::Dump& dump )
          : LoKi::AuxFunBase( std::tie( dump ) ), m_stream( stream ), m_dump( dump ) {}
      /// the constructor from the stopper
      Dump_( const LoKi::Dump& dump, std::ostream& stream = std::cout )
          : LoKi::AuxFunBase( std::tie( dump ) ), m_stream( stream ), m_dump( dump ) {}
      /// MANDATORY: clone method("virtual constructor")
      Dump_* clone() const override { return new Dump_( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()( const std::vector<TYPE>& a ) const override;
      /// OPTIONAL: the basic printout method
      std::ostream& fillStream( std::ostream& s ) const override { return s << "dump"; }
      // ======================================================================
    private:
      // ======================================================================
      /// the stream
      std::ostream& m_stream; // the stream
      /// open/close
      LoKi::Dump m_dump; // open/close
      // ======================================================================
    };
    // ========================================================================
    // MANDATORY: the only one essential method
    // ========================================================================
    template <typename TYPE>
    std::vector<TYPE> Dump_<TYPE>::operator()( const std::vector<TYPE>& a ) const {
      m_stream << m_dump.open();
      //
      if ( a.size() <= m_dump.nMax() ) {
        Gaudi::Utils::toStream( a.begin(), a.end(), m_stream, "[ ", " ]", " ,\n " );
      } else {
        Gaudi::Utils::toStream( a.begin(), a.begin() + m_dump.nMax(), m_stream, "[ ", " , ... ]", " ,\n " );
      }
      //
      m_stream << m_dump.close();
      return a;
    }
    // ========================================================================
    // MANDATORY: the only one essential method
    // ========================================================================
    template <>
    std::vector<double> Dump_<double>::operator()( const std::vector<double>& a ) const;
    // ========================================================================
    /** @class FakeSource
     *  simple "fake" source, useful for variuous debugging, fake streams, etc
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2015-03-12
     */
    template <typename TYPE>
    class FakeSource final : public LoKi::Functor<void, std::vector<TYPE>> {
    public:
      // =====================================================================
      /// the constructor
      explicit FakeSource( const unsigned short n = 0 ) : LoKi::AuxFunBase( std::tie( n ) ), m_n( n ) {}
      /// MANDATORY: clone method("virtual constructor")
      FakeSource* clone() const override { return new FakeSource( *this ); }
      /// MANDATORY: the only one essential method
      std::vector<TYPE> operator()() const override { return std::vector<TYPE>( m_n ); }
      // ======================================================================
    private:
      // ======================================================================
      /// data
      unsigned short m_n; // the length
      // ======================================================================
    };
    // ========================================================================
  } // namespace Functors
  // ==========================================================================
  /** simple "filter" function
   *
   *  @code
   *
   *   // some selection criteria (predicate)
   *   const LoKi::BasicFunctors<TYPE2>::Function& cut = ...;
   *   // input data
   *   std::vector<TYPE> in = ... ;
   *   // get the filtered results:
   *   std::vector<TYPE> out = in >> filter<TYPE> ( cut ) ;
   *
   *  @endcode
   *
   *  @see LoKi::Functors::Select
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>,
            typename = details::require_signature<F, TYPE2, bool>>
  LoKi::Functors::Select<TYPE, TYPE2> filter( F&& cut ) {
    return {std::forward<F>( cut )};
  }
  // ==========================================================================
  /** simple "select/filter" function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some cut:
   *   const LoKi::BasicFunctors<TYPE2>::Function& cut = ...;
   *   // get the filtered results:
   *   std::vector<TYPE> out = in >> select<TYPE> ( cun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>,
            typename = details::require_signature<F, TYPE2, bool>>
  LoKi::Functors::Select<TYPE, TYPE2> select( F&& cut ) {
    return {std::forward<F>( cut )};
  }
  // ==========================================================================
  /** simple "yield/map" function
   *
   *  @code
   *
   *   const LoKi::Functor<LHCb::Track,double>& pt = ... ;
   *   std::vector<LHCb::Track*> input = ... ;
   *   std::vector<double> results = input >> yields<LHCb::Track*>( pt )  ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven and the
   *  name comes from Marcel Merk.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-02
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Yields<TYPE, TYPE1, TYPE2> yields( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple "tee" function:
   *
   *  @code
   *
   *   // get some functor for plotting the TrPT
   *   AIDA::IHistogram1D* histo = ...
   *   FunctorFromFunctor<LHCb::Track,double> plotter = monitor ( TrPT , histo )  ;
   *   // get some functor for selection of track with maximal pt
   *   MaxElement<LHCb::Track*>  maxPt ( TrPT ) ;
   *   // container of input tracks:
   *   std::vector<LHCb::Track*> input = ... ;
   *   // "tee"
   *   input >> tee ( compose ( maxPt , plotter ) ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename F, typename TYPE = typename details::type1_t<F>::value_type, typename TYPE2 = details::type2_t<F>,
            typename = details::require_signature<F, std::vector<TYPE>, TYPE2>>
  LoKi::Functors::Tee<TYPE, TYPE2> tee( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple "min_value" function:
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the minimal value of the function
   *   double m = in >> min_value<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Min<TYPE, TYPE2, TYPE1> min_value( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Min<TYPE, TYPE2, TYPE1> min_value( F&& fun, const TYPE1& val ) {
    return {std::forward<F>( fun ), val};
  }
  // ==========================================================================
  /** simple "max_value" function:
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the maxima;l value of the function
   *   double m = in >> max_value<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Max<TYPE, TYPE2, TYPE1> max_value( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Max<TYPE, TYPE2, TYPE1> max_value( F&& fun, const TYPE1& val ) {
    return {std::forward<F>( fun ), val};
  }
  // ==========================================================================
  /** simple "min_abs_value" function:
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the minimal value of the function
   *   double m = in >> min_abs_value<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMin<TYPE, TYPE2, TYPE1> min_abs_value( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMin<TYPE, TYPE2, TYPE1> min_abs_value( F&& fun, const TYPE1& val ) {
    return {std::forward<F>( fun ), val};
  }
  // ==========================================================================
  /** simple "max_abs_value" function:
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the maximal value of the function
   *   double m = in >> max_abs_value<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMax<TYPE, TYPE2, TYPE1> max_abs_value( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMax<TYPE, TYPE2, TYPE1> max_abs_value( F&& fun, const TYPE1& val ) {
    return {std::forward<F>( fun ), val};
  }
  // ==========================================================================
  /** simple "process" function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some action
   *   const LoKi::Functor<TYPE2,TYPE3>& fun = ...;
   *   // get the processed results (indentical to the input)
   *   in >> process<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Process<TYPE, TYPE1, TYPE2> process( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple "for_each/process" function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some action
   *   const LoKi::Functor<TYPE2,TYPE3>& fun = ...;
   *   // get the processed results (indentical to the input)
   *   in >> for_each<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::Process<TYPE, TYPE1, TYPE2> for_each( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple 'max_element' function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the maximal element
   *   TYPE m = in >> max_element<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::MaxElement<TYPE, TYPE2, TYPE1> max_element( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple 'min_element' function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the minimal element
   *   TYPE m = in >> min_element<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::MinElement<TYPE, TYPE2, TYPE1> min_element( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple 'max_abs_element' function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the maximal element
   *   TYPE m = in >> max_abs_element<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMaxElement<TYPE, TYPE2, TYPE1> max_abs_element( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** simple 'min_abs_element' function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *
   *   // some function
   *   const LoKi::BasicFunctors<TYPE>::Function& fun = ...;
   *   // get the minimal element
   *   TYPE m = in >> min_abs_element<TYPE> ( fun ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
   *  @date 2007-11-28
   */
  template <typename TYPE, typename F, typename TYPE2 = details::type1_t<F>, typename TYPE1 = details::type2_t<F>>
  LoKi::Functors::AbsMinElement<TYPE, TYPE2, TYPE1> min_abs_element( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /**  simple "count" function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some predicate
   *   const LoKi::BasicFunctors<TYPE>::Predicate& cut = ...;
   *   // count the number of elements
   *   in >> count<TYPE>( cut ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-10-17
   */
  template <typename TYPE, typename F, typename TYPE1 = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Count<TYPE, TYPE1, TYPE2> count( F&& cut ) {
    return {std::forward<F>( cut )};
  }
  // ==========================================================================
  /**  simple "has" function
   *
   *  @code
   *
   *   std::vector<TYPE> in = ... ;
   *   // some predicate
   *   const LoKi::BasicFunctors<TYPE>::Predicate& cut = ...;
   *   // check the elements
   *   in >> has<TYPE>( cut ) ;
   *
   *  @endcode
   *
   *  The concept belongs to Gerhard Raven.
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-10-17
   */
  template <typename TYPE, typename F, typename TYPE1 = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Has<TYPE, TYPE1, TYPE2> has( F&& cut ) {
    return {std::forward<F>( cut )};
  }
  // ==========================================================================
  /** simple function to produce the "union" for two streamers
   *
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2010-06-05
   */
  template <typename F1, typename F2, typename TYPE = details::type1_t<F1, F2>,
            typename TYPE2 = typename details::type2_t<F1, F2>::value_type,
            typename       = details::require_signature<F1, TYPE, std::vector<TYPE2>>,
            typename       = details::require_signature<F2, TYPE, std::vector<TYPE2>>>
  LoKi::Functors::Union<TYPE, TYPE2> union_( F1&& fun1, F2&& fun2 ) {
    return {std::forward<F1>( fun1 ), std::forward<F2>( fun2 )};
  }

  // ==========================================================================
  template <typename F1, typename F2, typename TYPE = details::type1_t<F1, F2>,
            typename TYPE2 = typename details::type2_t<F1, F2>::value_type>
  LoKi::Functors::NoEmptyUnion<TYPE, TYPE2> no_empty_union( F1&& fun1, F2&& fun2 ) {
    return {std::forward<F1>( fun1 ), std::forward<F2>( fun2 )};
  }
  // ==========================================================================
  template <typename F1, typename F2, typename... Fs, typename TYPE = details::type1_t<F1, F2, Fs...>,
            typename TYPE2 = typename details::type2_t<F1, F2, Fs...>::value_type>
  LoKi::Functors::NoEmptyUnion<TYPE, TYPE2> no_empty_union( F1&& fun1, F2&& fun2, Fs&&... funs ) {
    return no_empty_union( no_empty_union( std::forward<F1>( fun1 ), std::forward<F2>( fun2 ) ),
                           std::forward<Fs>( funs )... );
  }

  // ==========================================================================
  template <typename TYPE>
  LoKi::Functors::Union<std::vector<TYPE>, TYPE>
  union_( const LoKi::Functor<TYPE, bool>& fun1, const LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>>& fun2 ) {
    return {LoKi::Functors::Select<TYPE, TYPE>( fun1 ), fun2};
  }
  // ==========================================================================
  template <typename TYPE>
  LoKi::Functors::Union<std::vector<TYPE>, TYPE>
  union_( const LoKi::Functor<std::vector<TYPE>, std::vector<TYPE>>& fun1, const LoKi::Functor<TYPE, bool>& fun2 ) {
    return {fun1, LoKi::Functors::Select<TYPE, TYPE>( fun2 )};
  }
  // ==========================================================================
  template <typename TYPE>
  LoKi::Functors::Union<std::vector<TYPE>, TYPE> union_( const LoKi::Functor<TYPE, bool>& fun1,
                                                         const LoKi::Functor<TYPE, bool>& fun2 ) {
    return {LoKi::Functors::Select<TYPE, TYPE>( fun1 ), LoKi::Functors::Select<TYPE, TYPE>( fun2 )};
  }
  // ==========================================================================
  /** simple "gate" function
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2010-11-17
   */
  template <typename TYPE, typename F, typename = details::require_signature<F, void, bool>>
  LoKi::Functors::Gate<TYPE> gate( F&& gate ) {
    return {std::forward<F>( gate )};
  }

  // ==========================================================================
  /** conditional source
   *  @param start the start condition
   *  @param source the actual source
   *  @thanks Roel AAIJ
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2010-11-18
   */
  template <typename TYPE>
  LoKi::Functors::Cause<TYPE> cause( const LoKi::Functor<void, bool>&              start,
                                     const LoKi::Functor<void, std::vector<TYPE>>& source ) {
    return {start, source};
  }
  // ==========================================================================
  /** conditional source
   *  @param source the actual source
   *  @param start the start condition
   *  @thanks Roel AAIJ
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2010-11-18
   */
  template <typename TYPE>
  LoKi::Functors::Cause<TYPE> cause( const LoKi::Functor<void, std::vector<TYPE>>& source,
                                     const LoKi::Functor<void, bool>&              start ) {
    return cause( start, source );
  }
  // ==========================================================================
  /** sum over the stream
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename = details::require_signature<F, TYPE, double>>
  LoKi::Functors::Sum<TYPE> sum( F&& fun, double init = 0 ) {
    return {std::forward<F>( fun ), init};
  }
  // ==========================================================================
  /** sum over good elements of the stream
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   */
  template <typename F1, typename F2, typename TYPE = details::type1_t<F1>,
            typename = details::require_signature<F1, TYPE, double>,
            typename = details::require_signature<F2, TYPE, bool>>
  LoKi::Functors::Sum<TYPE> sum( F1&& fun, F2&& cut, double init = 0 ) {
    return {std::forward<F1>( fun ), std::forward<F2>( cut ), init};
  }
  // ==========================================================================
  /** product over the stream
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename = details::require_signature<F, TYPE, double>>
  LoKi::Functors::Product<TYPE> product( F&& fun, double init = 1 ) {
    return {std::forward<F>( fun ), init};
  }
  // ==========================================================================
  /** product over good elements of the stream
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   */
  template <typename F1, typename F2, typename TYPE = details::type1_t<F1, F2>,
            typename = details::require_signature<F1, TYPE, double>,
            typename = details::require_signature<F2, TYPE, bool>>
  LoKi::Functors::Product<TYPE> product( F1&& fun, F2&& cut, double init = 1 ) {
    return {std::forward<F1>( fun ), std::forward<F2>( cut ), init};
  }
  // ==========================================================================
  /** fetch certain element from the vector and evaluate  the function
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Fetch<TYPE, TYPE2> fetch( F&& fun, unsigned int index, TYPE2 bad ) {
    return {std::forward<F>( fun ), index, std::move( bad )};
  }
  // ==========================================================================
  /** sort the input sequence
   *  @see LoKi::Functors::Sort_
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-30
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Sort_<TYPE, TYPE2> sort( F&& fun ) {
    return {std::forward<F>( fun )};
  }
  // ==========================================================================
  /** sort the sequence and get the first N-elements
   *  @see LoKi::Functors::Sort_
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-30
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Sort_<TYPE, TYPE2> sort( F&& fun, int N ) {
    return {std::forward<F>( fun ), N};
  }
  // ==========================================================================
  /** sort the sequence and get the first N-elements
   *  @see LoKi::Functors::Sort_
   *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
   *  @date 2011-10-30
   */
  template <typename F, typename TYPE = details::type1_t<F>, typename TYPE2 = details::type2_t<F>>
  LoKi::Functors::Sort_<TYPE, TYPE2> sort( int N, F&& fun ) {
    return sort( std::forward<F>( fun ), N );
  }
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
// the specific printout
// ============================================================================
template <typename TYPE>
std::ostream& LoKi::Functors::Empty<TYPE>::fillStream( std::ostream& s ) const {
  return s << "empty_";
}
// ============================================================================
// the specific printpout
// ============================================================================
template <typename TYPE>
std::ostream& LoKi::Functors::Size<TYPE>::fillStream( std::ostream& s ) const {
  return s << "size_";
}
// ============================================================================
// the specific printpout
// ============================================================================
template <typename TYPE>
std::ostream& LoKi::Functors::Unique<TYPE>::fillStream( std::ostream& s ) const {
  return s << "unique_";
}
// ============================================================================
// the specific printpout
// ============================================================================
template <typename TYPE>
std::ostream& LoKi::Functors::FirstN_<TYPE>::fillStream( std::ostream& s ) const {
  return s << m_N;
}
// ============================================================================
// the specific printpout
// ============================================================================
template <typename TYPE>
std::ostream& LoKi::Functors::Reverse_<TYPE>::fillStream( std::ostream& s ) const {
  return s << "reverse";
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
#endif // LOKI_FILTERS_H
