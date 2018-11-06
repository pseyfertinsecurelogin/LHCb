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
#ifndef LOKI_ALGFUNCTORS_H
#define LOKI_ALGFUNCTORS_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <vector>
// ============================================================================
// Gaudi Kernel
// ============================================================================
#include "GaudiKernel/Kernel.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/ThreadLocalContext.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
// ============================================================================
/** @file
 *
 *  Collection of various functors which test the status of "algorithm".
 *  The idea comes from Gerhard Raven
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @see IAlgorithm
 *  @see  Algorithm
 *  @see IAlgManager
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-010-14
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::Algortithms
   *
   *  Collection of various functors which test the status of "algorithm".
   *  @see LoKi::Cuts::ALG_PASSED
   *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
   *  @date 2008-010-14
   */
  namespace Algorithms {
      namespace Predicates {
          // ==========================================================================
          // filter passed ?
          inline constexpr auto filterPassed = [](const IAlgorithm* ia) {
            return ia && ia->execState( Gaudi::Hive::currentContext() ).filterPassed();
          };
          // ========================================================================
          // is enabled ?
          inline constexpr auto isEnabled = [](const IAlgorithm* ia) {
            return ia && ia->isEnabled () ;
          };
          // ==========================================================================
          // is executed ?
          inline constexpr auto isExecuted = []( const IAlgorithm* ia ) {
            return ia && ia->execState( Gaudi::Hive::currentContext() ).state() == AlgExecState::State::Done;
          };
          // ==========================================================================
      }
    // =========================================================================
    /** @class Passed
     *  simple check of the certain algorithm to pass the filter
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Passed : public LoKi::BasicFunctors<void>::Predicate
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Passed ( const std::string& name ) ;
      ~Passed() override;
      /// MANDATORY: clone method ("virtual constructor")
      Passed* clone() const override { return new Passed ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() () const override ;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override ;
      // ======================================================================
    protected:
      // ======================================================================
      /// set the algorithm
      const LoKi::Interface<IAlgorithm>& setAlgorithm
      ( const IAlgorithm* a ) const ;                      // set the algorithm
      /// get the algorithm
      const LoKi::Interface<IAlgorithm>& algorithm () const
      { return m_algorithm ; }                             // get the algorithm
      /// get the algorithm name
      const std::string& algName () const { return m_name ; }
      /// get the algorithm
      const LoKi::Interface<IAlgorithm>&
      getAlgorithm ( const std::string& name ) const ; // get the algorithm
      // ======================================================================
    private:
      // ======================================================================
      /// the algorithm name
      std::string                         m_name      ;
      /// the algorithm itself
      mutable LoKi::Interface<IAlgorithm> m_algorithm ;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Enabled
     *  simple check oif the algorithm is enabled
     *  @see LoKi::Cuts::ALG_ENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Enabled final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Enabled ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      Enabled* clone() const override { return new Enabled ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Executed
     *  simple check if the algorithm is enabled
     *  @see LoKi::Cuts::ALG_EXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Executed final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Executed ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      Executed* clone() const override { return new Executed ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    } ;
    // =========================================================================
    /** @class Run
     *  A bit complicated action:
     *   - algorithm must be enabled, SUCCESSFULLY executed and passed
     *   - if not executed, execuye it
     *  @see LoKi::Cuts::ALG_RUN
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    class GAUDI_API Run final : public Passed
    {
    public:
      // ======================================================================
      /// constructor from the algorithm name
      explicit Run ( const std::string& name ) ;
      /// MANDATORY: clone method ("virtual constructor")
      Run* clone() const override { return new Run ( *this ) ; }
      /// MANDATORY: the only one essential method
      bool operator() () const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream ( std::ostream& s ) const override;
      // ======================================================================
    } ;
    namespace details {
        // ========================================================================
        /** @class AlgsFunctorBase
         *  Base class for Algorithm predicates
         */
        // =========================================================================

        // TODO: AlgsFunctorBase doesn't actually depend on Ret -- so should
        //       try to move the code to a non-template implemenation to reduce
        //       code size...
        template <typename ReturnType>
        class GAUDI_API AlgsFunctorBase : public LoKi::Functor<void,ReturnType>
        {
        public:
          // ======================================================================
          /// constructor from algorithm names
          AlgsFunctorBase ( std::vector<std::string> name ) ;
          AlgsFunctorBase ( AlgsFunctorBase&& ) = default;
          AlgsFunctorBase ( const AlgsFunctorBase& ) = default;
          ~AlgsFunctorBase() override;
          // ======================================================================
        protected:
          // ======================================================================
          /// the actual type of vector of algorithms
          bool empty() const { return m_algorithms.empty () ; }
          /// get the algorithm name
          const std::string& algName ( const size_t i ) const { return m_names[i] ; }
          /// get the algorithm
          LoKi::Interface<IAlgorithm>
          getAlgorithm ( const std::string& name ) const ; // get the algorithm
        public:
          // ======================================================================
          std::ostream& print( const std::string& name, std::ostream& s) const ;
          // ======================================================================
          // ======================================================================
          auto begin() const { return m_algorithms.begin() ; }
          auto end()   const { return m_algorithms.end()   ; }
          /// get the algorithms
          const auto& algorithms () const { return m_algorithms ; }
          /// get the algorithm name
          const std::vector<std::string>& algNames () const { return m_names ; }
          /// get the algorithms
          void getAlgorithms () const ; // get the algorithms
          // ======================================================================
        private:
          // ======================================================================
          /// the algorithm names
          std::vector<std::string> m_names ;               // the algorithm names
          /// the algorithms themself
          mutable std::vector< LoKi::Interface<IAlgorithm> > m_algorithms ;
          // ======================================================================
        } ;

        template <typename Compose>
        using result_t = decltype(Compose::execute(std::declval<IAlgorithm**>(),
                                                   std::declval<IAlgorithm**>()));

        template <typename Compose>
        struct GAUDI_API AlgsFunctor final : AlgsFunctorBase<result_t<Compose>> {
          /// constructor from the algorithm name
          AlgsFunctor ( std::string name1 ,
                        std::string name2 )
              : AlgsFunctor( std::vector<std::string>{ std::move(name1), std::move(name2) } ) {}
          AlgsFunctor ( std::string name1 ,
                        std::string name2 ,
                        std::string name3 )
              : AlgsFunctor( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3) } ) {}
          AlgsFunctor( std::string name1 ,
                       std::string name2 ,
                       std::string name3 ,
                       std::string name4 )
              : AlgsFunctor( std::vector<std::string>{ std::move(name1), std::move(name2), std::move(name3), std::move(name4) } ) {}
          AlgsFunctor( std::vector<std::string> names )
              : AuxFunBase{ std::tie(names) }
              , AlgsFunctorBase<result_t<Compose>>( std::move(names) ) {};
          /// MANDATORY: clone method ("virtual constructor")
          AlgsFunctor* clone() const override { return new AlgsFunctor ( *this ) ; }
          /// MANDATORY: the only one essential method
          result_t<Compose> operator() () const override {
            if ( this->algNames().size() != this->algorithms().size() ) this->getAlgorithms() ;
            return Compose::execute( this->begin(), this->end() );
          }
          /// OPTIONAL: nice printout
          std::ostream& fillStream ( std::ostream& s ) const override
          { return this->print ( Compose::name() , s ) ; }

        };
    }
    // ========================================================================
    /** @class AnyPassed
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct AnyPassed {
            static const char* name() { return "ALG_ANYPASSED" ; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end) {
                return std::any_of( begin, end, Predicates::filterPassed );
            }
        };
    }
    using AnyPassed = details::AlgsFunctor<details::AnyPassed>;
    // ======================================================================
    // ========================================================================
    /** @class AllPassed
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */

    namespace details {
        struct AllPassed {
            static const char* name() { return "ALG_ANYPASSED" ; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end) {
                return begin!=end && std::all_of( begin, end, Predicates::filterPassed );
            }
        };
    }
    using AllPassed = details::AlgsFunctor<details::AllPassed>;

    // ========================================================================
    /** @class AnyEnabled
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct AnyEnabled {
            static const char* name() { return "ALG_ANYENABLED"; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end)
            { return std::any_of( begin , end, Predicates::isEnabled ) ; }
        };
    }
    using AnyEnabled = details::AlgsFunctor<details::AnyEnabled>;

    // ========================================================================
    /** @class AllEnabled
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct AllEnabled {
            static const char* name() { return "ALG_ALLENABLED"; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end)
            { return begin!=end  && std::all_of( begin, end, Predicates::isEnabled ) ; }
        };
    }
    using AllEnabled = details::AlgsFunctor<details::AllEnabled>;

    // ========================================================================
    /** @class AnyExecuted
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ANYEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct AnyExecuted {
            static const char* name() { return "ALG_ANYEXECUTED"; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end)
            { return std::any_of( begin, end, Predicates::isExecuted ) ;}
        };
    }
    using AnyExecuted = details::AlgsFunctor<details::AnyExecuted>;

    // ========================================================================
    /** @class AllExecuted
     *  simple check of the certain algorithm to pass the filter
     *  @see LoKi::Cuts::ALG_ALLEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct AllExecuted {
            static const char* name() { return "ALG_ALLEXECUTED"; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end)
            { return begin!=end  && std::all_of ( begin, end, Predicates::isExecuted ) ; }
        };
    }
    using AllExecuted = details::AlgsFunctor<details::AllExecuted>;
    // ========================================================================
    /** @class RunAll
     *  @see LoKi::Cuts::ALG_RUNALL
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-14
     */
    // =========================================================================
    namespace details {
        struct RunAll {
            static const char* name() { return "ALG_RUNALL" ; }
            template <typename Iterator>
            static bool execute(Iterator begin, Iterator end) {
                return std::all_of( begin, end, [](IAlgorithm* alg) {
                    if ( UNLIKELY(!alg) ) {
                      throw GaudiException( "Invalid algorithm!", "RunAll", StatusCode::FAILURE );
                    }
                    if ( UNLIKELY(!Predicates::isEnabled( alg )) ) {
                      throw GaudiException( "Algorithm '" + alg->name() + "' is disabled", "RunAll", StatusCode::SUCCESS );
                    }
                    if ( !Predicates::isExecuted ( alg ) ) {
                      StatusCode sc = alg->sysExecute(Gaudi::Hive::currentContext()) ;  // EXECUTE IT!!!
                      if ( sc.isFailure() ) {
                        throw GaudiException("Error from algorithm '" + alg->name() + "' sysExecute", "RunAll", sc );
                      }
                    }
                    //
                    return Predicates::filterPassed( alg );
                } );
            }
        };
    }
    using RunAll = details::AlgsFunctor<details::RunAll>;
    // ======================================================================



    // =========================================================================
    /** @class NumPassed
     *  Simple functor to count number of passed algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMPASSED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    namespace details {
        struct NumPassed {
            static const char* name() { return "ALG_NUMPASSED" ; }
            template <typename Iterator>
            static double execute(Iterator begin, Iterator end) {
                return std::count_if( begin, end, Predicates::filterPassed ) ;
            }
        };
    }
    using NumPassed = details::AlgsFunctor<details::NumPassed>;
    // =========================================================================
    /** @class NumEnabled
     *  Simple functor to count number of enabled algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMENABLED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    namespace details {
        struct NumEnabled {
            static const char* name() { return "ALG_NUMENABLED" ; }
            template <typename Iterator>
            static double execute(Iterator begin, Iterator end) {
                return std::count_if( begin, end, Predicates::isEnabled ) ;
            }
        };
    }
    using NumEnabled = details::AlgsFunctor<details::NumEnabled>;
    // ========================================================================
    /** @class NumExecuted
     *  Simple functor to count number of executed algorithms from the list
     *  @see LoKi::Cuts::ALG_NUMEXECUTED
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-10-19
     */
    namespace details {
        struct NumExecuted {
            static const char* name() { return "ALG_NUMEXECUTED" ; }
            template <typename Iterator>
            static double execute(Iterator begin, Iterator end) {
                return std::count_if( begin, end, Predicates::isExecuted ) ;
            }
        };
    }
    using NumExecuted = details::AlgsFunctor<details::NumExecuted>;
    // ========================================================================
  } //                                        end of namespace LoKi::Algorithms
  // ==========================================================================
} //                                                     end of namaespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_ALGFUNCTORS_H
// ============================================================================
