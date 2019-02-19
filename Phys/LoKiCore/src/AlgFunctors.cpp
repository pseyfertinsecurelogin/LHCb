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
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
#include <initializer_list>
#include <memory>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAlgManager.h"
#include "GaudiKernel/IAlgorithm.h"
#include "GaudiKernel/IJobOptionsSvc.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
#include "GaudiKernel/TypeNameString.h"
// ============================================================================
// GaudiALg
// ============================================================================
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiAlg/GetAlgs.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/AlgFunctors.h"
#include "LoKi/Assert.h"
#include "LoKi/ILoKiSvc.h"
#include "LoKi/Report.h"
#include "LoKi/Services.h"
// ============================================================================
#ifdef __INTEL_COMPILER             // Disable ICC remark
#  pragma warning( disable : 2259 ) // non-pointer conversion may lose significant bits
#  pragma warning( disable : 1572 ) // floating-point equality and inequality comparisons are unreliable
#endif
// ============================================================================
/** @file
 *  Implementation file for functions from namespace LoKi::Algorithms
 *  @see LoKi::Algorithms
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2008-010-14
 */
// ============================================================================
namespace {
  bool isDefault( const std::string& s ) { return s.empty(); }
  // utility class to populate some properties in the job options service
  // for a given instance name in case those options are not explicitly
  // set a-priori (effectively inheriting their values from the GaudiSequencer)
  class populate_JobOptionsSvc_t {
    std::vector<std::unique_ptr<Gaudi::Details::PropertyBase>> m_props;
    IJobOptionsSvc*                                            m_jos;
    std::string                                                m_name;

    template <typename T>
    void push_back( T&& t ) {
      static_assert( std::tuple_size<T>::value == 2, "Expecting an std::tuple key-value pair" );
      using type   = typename std::decay<typename std::tuple_element<1, T>::type>::type;
      using prop_t = Gaudi::Property<type>;
      if ( !isDefault( std::get<1>( t ) ) )
        m_props.push_back( std::make_unique<prop_t>( std::get<0>( t ), std::get<1>( t ) ) );
    }
    void check_veto() { // avoid changing properties expliclty present in the JOS...
      const auto*props = m_jos->getProperties( m_name );
      if ( !props ) return;
      for ( const auto& i : *props ) {
        auto j = std::find_if( std::begin( m_props ), std::end( m_props ),
                               [&i]( const auto& prop ) { return prop->name() == i->name(); } );
        if ( j == std::end( m_props ) ) continue;
        m_props.erase( j );
        if ( m_props.empty() ) break; // done!
      }
    }

  public:
    template <typename... Args>
    populate_JobOptionsSvc_t( std::string name, IJobOptionsSvc* jos, Args&&... args )
        : m_jos{jos}, m_name{std::move( name )} {
      (void)std::initializer_list<int>{( push_back( std::forward<Args>( args ) ), 0 )...};
      if ( !m_props.empty() ) check_veto();
      std::for_each( std::begin( m_props ), std::end( m_props ),
                     [&]( const auto& i ) { m_jos->addPropertyToCatalogue( m_name, *i ).ignore(); } );
    }
    ~populate_JobOptionsSvc_t() {
      std::for_each( std::begin( m_props ), std::end( m_props ),
                     [&]( const auto& i ) { m_jos->removePropertyFromCatalogue( m_name, i->name() ).ignore(); } );
    }
  };
  // ==========================================================================
  //  const IAlgManager* const s_ALGMANAGER = 0 ;
  const IAlgorithm* const s_ALGORITHM = nullptr;
  // ==========================================================================
  /// get the algorithm manager
  inline SmartIF<IAlgManager> getAlgManager( LoKi::ILoKiSvc* loki ) {
    if ( !loki ) { loki = LoKi::Services::instance().lokiSvc(); }
    SmartIF<IAlgManager> alg( loki );
    if ( !alg ) { LoKi::Report::Error( "AlgFunctors::getAlgManager: Unable to locate IAlgManager" ).ignore(); }
    return alg;
  }
  // =========================================================================
  // get the algorithm
  // =========================================================================
  inline LoKi::Interface<IAlgorithm> getAlgorithm( const std::string& name, SmartIF<IAlgManager> iam ) {

    if ( !iam ) {
      LoKi::Report::Error( "AlgFunctors::getAlgorithm: IAlgManager* points to NULL" ).ignore();
      return LoKi::Interface<IAlgorithm>();
    }

    {
      // if the algorithm is already exist and running  - just get it!
      IAlgorithm* iialg = iam->algorithm( name, false );
      if ( iialg && Gaudi::StateMachine::RUNNING == iialg->FSMState() && LoKi::Services::instance().lokiSvc() &&
           Gaudi::StateMachine::RUNNING == LoKi::Services::instance().lokiSvc()->FSMState() ) {
        return LoKi::Interface<IAlgorithm>( iialg );
      }
    }

    ///////// start of code copied from GaudiSequencer...

    /** @todo       AlgFunctors.cpp : This stupid code MUST BE REMOVED ASAP!
     *              we definitely can;t live with it!
     *
     *  @warning    AlgFunctors.cpp : This stupid code MUST BE REMOVED ASAP!
     *              we definitely can;t live with it!
     *
     *  @attention  AlgFunctors.cpp : This stupid code MUST BE REMOVED ASAP!
     *              we definitely can;t live with it!
     */

    // get job options svc -- todo: move this one level up, not done
    // on this branch to retain binary compatiblity...
    LoKi::ILoKiSvc*         ls = LoKi::Services::instance().lokiSvc();
    SmartIF<IAlgContextSvc> cntx( ls );
    LoKi::Assert( !( !cntx ), "IAlgContextSvc* points to NULL!" );
    GaudiAlgorithm* parent = Gaudi::Utils::getGaudiAlg( cntx );
    LoKi::Assert( parent != nullptr, "parent not a GaudiAlg!" );
    IJobOptionsSvc* jos = parent->svc<IJobOptionsSvc>( "JobOptionsSvc" );

    /////////////////////////////
    /////////////////////////////
    const Gaudi::Utils::TypeNameString typeName( name );

    // do not create it now
    SmartIF<IAlgorithm> myIAlg = iam->algorithm( typeName, false );

    if ( !myIAlg.isValid() ) {
      // ensure some magic properties are set while we create the subalgorithm so
      // that it effectively inherites 'our' settings -- if they have non-default
      // values... and are not set explicitly already.
      populate_JobOptionsSvc_t populate_guard{typeName.name(), jos,
                                              std::forward_as_tuple( "Context", parent->context() ),
                                              std::forward_as_tuple( "RootInTES", parent->rootInTES() )};
      myIAlg = iam->algorithm( typeName, true );
    }

    ///////// end of code copied from GaudiSequencer...
    return LoKi::Interface<IAlgorithm>( myIAlg );
  }
  // ===========================================================================
  // get the algorithm
  inline LoKi::Interface<IAlgorithm> getAlgorithm( const std::string& name, LoKi::ILoKiSvc* loki ) {
    SmartIF<IAlgManager> iam = getAlgManager( loki );
    return iam ? getAlgorithm( name, iam ) : nullptr;
  }
} // namespace
// ============================================================================
// get the algorithm
// ============================================================================
const LoKi::Interface<IAlgorithm>& LoKi::Algorithms::Passed::getAlgorithm( const std::string& name ) const {
  const IAlgorithm* a = ::getAlgorithm( name, lokiSvc() );
  Assert( nullptr != a, "Unable to locate algorithm '" + name + "'" );
  m_algorithm = a;
  return algorithm();
}
// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Passed::Passed( const std::string& name )
    : LoKi::AuxFunBase( std::tie( name ) ), m_name( name ), m_algorithm( s_ALGORITHM ) {
  if ( gaudi() ) { this->getAlgorithm( algName() ); }
}
// ============================================================================
LoKi::Algorithms::Passed::~Passed() {
  if ( m_algorithm && !gaudi() ) {
    // Warning("IAlgorithm: manual reset!") ;
    m_algorithm.reset();
  }
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Passed::operator()() const {
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm( algName() ); }
  //
  return Predicates::filterPassed( algorithm() );
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Passed::fillStream( std::ostream& s ) const {
  return s << "ALG_PASSED(" << Gaudi::Utils::toString( algName() ) << ")";
}
// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Enabled::Enabled( const std::string& name ) : Passed( name ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Enabled::operator()() const {
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm( algName() ); }
  //
  return Predicates::isEnabled( algorithm() );
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Enabled::fillStream( std::ostream& s ) const {
  return s << "ALG_ENABLED(" << Gaudi::Utils::toString( algName() ) << ")";
}

// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Executed::Executed( const std::string& name )
    : LoKi::AuxFunBase( std::tie( name ) ), Passed( name ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Executed::operator()() const {
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm( algName() ); }
  //
  return Predicates::isExecuted( algorithm() );
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Executed::fillStream( std::ostream& s ) const {
  return s << "ALG_EXECUTED(" << Gaudi::Utils::toString( algName() ) << ")";
}

// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Run::Run( const std::string& name ) : LoKi::AuxFunBase( std::tie( name ) ), Passed( name ) {}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Run::operator()() const {
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm( algName() ); }
  //
  if ( !Predicates::isEnabled( algorithm() ) ) {
    Warning( "Algorithm '" + algName() + "' is disabled, return false " ).ignore();
    return false; // RETURN
  }
  //
  if ( !Predicates::isExecuted( algorithm() ) ) {
    StatusCode sc = algorithm()->sysExecute( Gaudi::Hive::currentContext() ); // EXECUTE IT!!!
    if ( sc.isFailure() ) {
      Error( "Error from algorithm '" + algName() + "' sysExecute", sc ).ignore();
      return false; // RETURN
    }
  }
  //
  // finally:
  return Predicates::filterPassed( algorithm() );
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Run::fillStream( std::ostream& s ) const {
  return s << "ALG_RUN(" << Gaudi::Utils::toString( algName() ) << ")";
}

// ============================================================================
// get the algorithm
// ============================================================================
template <typename ReturnType>
LoKi::Interface<IAlgorithm>
LoKi::Algorithms::details::AlgsFunctorBase<ReturnType>::getAlgorithm( const std::string& name ) const // get the
                                                                                                      // algorithm
{
  LoKi::Interface<IAlgorithm> a = ::getAlgorithm( name, this->lokiSvc() );
  Assert( a.validPointer(), "Unable to locate algorithm '" + name + "'" );
  return a;
}
// ============================================================================
// get the algorithms
// ============================================================================
template <typename ReturnType>
void LoKi::Algorithms::details::AlgsFunctorBase<ReturnType>::getAlgorithms() const // get the algorithm
{
  // unsigned int i = 0 ;
  std::transform( m_names.begin(), m_names.end(), std::back_inserter( m_algorithms ), [&]( const std::string& name ) {
    // Warning( " AQUICRE alg: " + a->name()
    // + Gaudi::Utils::toCpp ( ++i ) + "/"
    // + Gaudi::Utils::toCpp ( m_names.size() ) ) ;
    return this->getAlgorithm( name );
  } );
}
// ============================================================================
// constructor from the algorithm name
// ============================================================================
template <typename ReturnType>
LoKi::Algorithms::details::AlgsFunctorBase<ReturnType>::AlgsFunctorBase( std::vector<std::string> names )
    : m_names( std::move( names ) ) {
  if ( this->gaudi() ) { this->getAlgorithms(); }
}
// ============================================================================
// MANDATORY: virtual destructor
// ============================================================================
template <typename ReturnType>
LoKi::Algorithms::details::AlgsFunctorBase<ReturnType>::~AlgsFunctorBase() {
  for ( auto& ia : m_algorithms ) {
    if ( ia && !this->gaudi() ) {
      // Warning("IAlgorithm: manual reset!") ;
      ia.reset();
    }
  }
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
template <typename ReturnType>
std::ostream& LoKi::Algorithms::details::AlgsFunctorBase<ReturnType>::print( const std::string& name,
                                                                             std::ostream&      s ) const {
  s << name << "(";
  if ( 4 == algNames().size() ) {
    s << Gaudi::Utils::toString( algName( 0 ) ) << "," << Gaudi::Utils::toString( algName( 1 ) ) << ","
      << Gaudi::Utils::toString( algName( 2 ) ) << "," << Gaudi::Utils::toString( algName( 3 ) );
  } else if ( 3 == algNames().size() ) {
    s << Gaudi::Utils::toString( algName( 0 ) ) << "," << Gaudi::Utils::toString( algName( 1 ) ) << ","
      << Gaudi::Utils::toString( algName( 2 ) );
  } else if ( 2 == algNames().size() ) {
    s << Gaudi::Utils::toString( algName( 0 ) ) << "," << Gaudi::Utils::toString( algName( 1 ) );
  } else {
    s << Gaudi::Utils::toString( algNames() );
  }
  return s << ")";
}

template class LoKi::Algorithms::details::AlgsFunctorBase<bool>;
template class LoKi::Algorithms::details::AlgsFunctorBase<double>;

// ============================================================================
// The END
// ============================================================================
