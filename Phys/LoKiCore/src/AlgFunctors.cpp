// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include  "GaudiKernel/Bootstrap.h"
#include  "GaudiKernel/ISvcLocator.h"
#include  "GaudiKernel/IAlgorithm.h"
#include  "GaudiKernel/IAlgManager.h"
#include  "GaudiKernel/SmartIF.h"
#include  "GaudiKernel/ToStream.h"
#include  "GaudiKernel/TypeNameString.h"
#include  "GaudiKernel/IJobOptionsSvc.h"
// ============================================================================
// GaudiALg
// ============================================================================
#include  "GaudiAlg/GaudiAlgorithm.h"
#include  "GaudiAlg/GetAlgs.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ILoKiSvc.h"
#include "LoKi/AlgFunctors.h"
#include "LoKi/Report.h"
#include "LoKi/Services.h"
#include "LoKi/Assert.h"
// ============================================================================
#ifdef __INTEL_COMPILER       // Disable ICC remark
#pragma warning(disable:2259) // non-pointer conversion may lose significant bits
#pragma warning(disable:1572) // floating-point equality and inequality comparisons are unreliable
#endif
#ifdef GAUDI_SYSEXECUTE_WITHCONTEXT
/// \fixme backward compatibility with Gaudi <= v28r1
#include "GaudiKernel/ThreadLocalContext.h"
#define SYSEX_ARGUMENT Gaudi::Hive::currentContext()
#else
#define SYSEX_ARGUMENT
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
namespace
{
  // ==========================================================================
  //  const IAlgManager* const s_ALGMANAGER = 0 ;
  const IAlgorithm*  const s_ALGORITHM  = nullptr ;
  // ==========================================================================
  /// get the algorithm manager
  inline SmartIF<IAlgManager> getAlgManager ( LoKi::ILoKiSvc* loki )
  {
    if ( !loki ) { loki = LoKi::Services::instance().lokiSvc () ; }
    SmartIF<IAlgManager> alg ( loki ) ;
    if ( !alg )
    { LoKi::Report::Error
        ( "AlgFunctors::getAlgManager: Unable to locate IAlgManager" ) ; }
    return alg ;
  }
  // =========================================================================
  // get the algorithm
  // =========================================================================
  inline LoKi::Interface<IAlgorithm> getAlgorithm
  ( const std::string&    name ,
    SmartIF<IAlgManager>  iam  )
  {

    if ( !iam )
    {
      LoKi::Report::Error
        ( "AlgFunctors::getAlgorithm: IAlgManager* points to NULL" ) ;
      return LoKi::Interface<IAlgorithm>()  ;
    }

    {
      // if the algorithm is already exist and running  - just get it!
      IAlgorithm* iialg = iam->algorithm ( name , false ) ;
      if ( iialg
           && Gaudi::StateMachine::RUNNING == iialg -> FSMState()
           && LoKi::Services::instance().lokiSvc()
           && Gaudi::StateMachine::RUNNING == LoKi::Services::instance().lokiSvc()->FSMState() )
      { return LoKi::Interface<IAlgorithm>( iialg ) ; }
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
    LoKi::ILoKiSvc *ls = LoKi::Services::instance().lokiSvc ();
    SmartIF<IAlgContextSvc> cntx ( ls ) ;
    LoKi::Assert ( !(!cntx) , "IAlgContextSvc* points to NULL!" ) ;
    GaudiAlgorithm *parent = Gaudi::Utils::getGaudiAlg ( cntx ) ;
    LoKi::Assert ( parent!=nullptr , "parent not a GaudiAlg!" ) ;
    IJobOptionsSvc* jos = parent->svc<IJobOptionsSvc>( "JobOptionsSvc" );

    /////////////////////////////
    /////////////////////////////
    const Gaudi::Utils::TypeNameString typeName(name);
    const std::string &theName = typeName.name();
    const std::string &theType = typeName.type();

    bool addedContext = false;
    bool addedRootInTES = false;
    // do not create it now
    SmartIF<IAlgorithm> myIAlg = iam->algorithm( typeName , false);

    if ( !myIAlg.isValid() ) {
      //== Set the Context if not in the jobOptions list
      if ( ""  != parent->context() ||
           ""  != parent->rootInTES() ) {
        bool foundContext = false;
        bool foundRootInTES = false;
        const std::vector<const Property*>* properties =
          jos->getProperties( theName );
        if ( properties ) {
          // Iterate over the list to set the options
          for (const auto & property : *properties)   {
            const StringProperty* sp =
              dynamic_cast<const StringProperty*>(property);
            if ( sp )    {
              if ( "Context" == property->name() ) foundContext = true;
              if ( "RootInTES" == property->name() ) foundRootInTES = true;
            }
          }
        }
        if ( !foundContext && "" != parent->context() ) {
          StringProperty contextProperty( "Context", parent->context() );
          jos->addPropertyToCatalogue( theName, contextProperty ).ignore();
          addedContext = true;
        }
        if ( !foundRootInTES && "" != parent->rootInTES() ) {
          StringProperty rootInTESProperty( "RootInTES", parent->rootInTES() );
          jos->addPropertyToCatalogue( theName, rootInTESProperty ).ignore();
          addedRootInTES = true;
        }
      }

      Algorithm *myAlg = nullptr;
      StatusCode result = parent->createSubAlgorithm( theType, theName, myAlg );
      // (MCl) this should prevent bug #35199... even if I didn't manage to
      // reproduce it with a simple test.
      if (result.isSuccess()) myIAlg = myAlg;
    } else {
      Algorithm *myAlg = dynamic_cast<Algorithm*>(myIAlg.get());
      if (myAlg) {
        parent->subAlgorithms()->push_back(myAlg);
        // when the algorithm is not created,
        // the ref count is short by one, so we have to fix it.
        myAlg->addRef();
      }
    }


    //== Remove the property, in case this is not a GaudiAlgorithm...
    if ( addedContext ) {
      jos->removePropertyFromCatalogue( theName, "Context" ).ignore();
      addedContext = false;
    }
    if ( addedRootInTES ) {
      jos->removePropertyFromCatalogue( theName, "RootInTES" ).ignore();
      addedRootInTES = false;
    }
    ///////// end of code copied from GaudiSequencer...

    IAlgorithm* _a =  myIAlg;
    if ( Gaudi::StateMachine::INITIALIZED    > _a -> FSMState() )
    {
      const Gaudi::StateMachine::State loki_state =
        LoKi::Services::instance().lokiSvc()->FSMState() ;
      if ( Gaudi::StateMachine::INITIALIZED <= loki_state )
      {
        StatusCode sc = _a->sysInitialize() ;
        if ( sc.isFailure() )
        {
          LoKi::Report::Error
            ( "AlgFunctors::getAlgorithm: Failure to initialize '" + name + "'" , sc ) ;
          return LoKi::Interface<IAlgorithm>()  ;
        }
      }
    }
    //
    if ( Gaudi::StateMachine::RUNNING != _a -> FSMState() )
    {
      const Gaudi::StateMachine::State loki_state =
        LoKi::Services::instance().lokiSvc()->FSMState() ;
      if ( Gaudi::StateMachine::RUNNING == loki_state )
      {
        // start it!
        StatusCode sc = _a->sysStart() ;
        if ( sc.isFailure() )
        {
          LoKi::Report::Error
            ( "AlgFunctors::getAlgorithm: Failure to start '" + name + "'" , sc ) ;
          return LoKi::Interface<IAlgorithm>()  ;
        }
      }
    }
    //
    return LoKi::Interface<IAlgorithm>( _a )  ;
  }
  // ===========================================================================
  // get the algorithm
  inline LoKi::Interface<IAlgorithm> getAlgorithm
  ( const std::string&    name , LoKi::ILoKiSvc* loki )
  {
    SmartIF<IAlgManager> iam = getAlgManager  ( loki ) ;
    return iam ? getAlgorithm ( name , iam ) : nullptr;
  }
  // ==========================================================================
  // filter passed ?
  constexpr struct FilterPassed_t
  {
    inline bool operator() ( const IAlgorithm* ia ) const
    { return ia && ia -> filterPassed () ; }
  } filterPassed{};
  // ========================================================================
  // is enabled ?
  constexpr struct IsEnabled_t
  {
    inline bool operator() ( const IAlgorithm* ia ) const
    { return ia && ia -> isEnabled () ; }
  } isEnabled{};
  // ==========================================================================
  // is executed ?
  constexpr struct IsExecuted_t
  {
    inline bool operator() ( const IAlgorithm* ia ) const
    { return ia && ia -> isExecuted () ; }
  } isExecuted{};
  // ==========================================================================
}
// ============================================================================
// get the algorithm
// ============================================================================
const LoKi::Interface<IAlgorithm>&
LoKi::Algorithms::Passed::getAlgorithm
( const std::string& name ) const  // get the algorithm
{
  const IAlgorithm* a = ::getAlgorithm ( name , lokiSvc () ) ;
  Assert ( 0 != a , "Unable to locate algorithm '" + name + "'" ) ;
  m_algorithm = a ;
  return algorithm () ;
}
// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Passed::Passed
( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , m_name       ( name        )
  , m_algorithm  ( s_ALGORITHM )
{
  if ( gaudi() ) { this->getAlgorithm ( algName() ) ; }
}
// ============================================================================
LoKi::Algorithms::Passed::~Passed ()
{
  if ( m_algorithm && !gaudi() )
  {
    // Warning("IAlgorithm: manual reset!") ;
    m_algorithm.reset() ;
  }
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Passed::operator() () const
{
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm ( algName() ) ; }
  //
  return filterPassed ( algorithm() ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Passed::fillStream( std::ostream& s ) const
{ return s << "ALG_PASSED(" << Gaudi::Utils::toString( algName() ) << ")" ; }
// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Enabled::Enabled ( const std::string& name ) : Passed ( name ){}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Enabled::operator() () const
{
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm ( algName() ) ; }
  //
  return isEnabled ( algorithm() ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Enabled::fillStream( std::ostream& s ) const
{ return s << "ALG_ENABLED(" << Gaudi::Utils::toString( algName() ) << ")" ; }

// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Executed::Executed
( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , Passed ( name ){}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Executed::operator() () const
{
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm ( algName() ) ; }
  //
  return isExecuted ( algorithm() ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Executed::fillStream( std::ostream& s ) const
{ return s << "ALG_EXECUTED(" << Gaudi::Utils::toString( algName() ) << ")" ; }


// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::Run::Run ( const std::string& name )
  : LoKi::AuxFunBase ( std::tie ( name ) )
  , Passed ( name ){}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::Run::operator() () const
{
  //
  if ( !algorithm().validPointer() ) { this->getAlgorithm ( algName() ) ; }
  //
  if ( !isEnabled ( algorithm() ) )
  {
    Warning("Algorithm '" + algName() + "' is disabled, return false " );
    return false ;                                                  // RETURN
  }
  //
  if ( !isExecuted ( algorithm() ) )
  {
    StatusCode sc = algorithm()->sysExecute(SYSEX_ARGUMENT) ;  // EXECUTE IT!!!
    if ( sc.isFailure() )
    {
      Error("Error from algorithm '" + algName() + "' sysExecute", sc );
      return false ;                                                // RETURN
    }
  }
  //
  // finally:
  return filterPassed ( algorithm() ) ;
  //
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::Run::fillStream( std::ostream& s ) const
{ return s << "ALG_RUN(" << Gaudi::Utils::toString( algName() ) << ")" ; }


// ============================================================================
// get the algorithm
// ============================================================================
LoKi::Interface<IAlgorithm>
LoKi::Algorithms::AnyPassed::getAlgorithm
( const std::string& name ) const  // get the algorithm
{
  LoKi::Interface<IAlgorithm> a = ::getAlgorithm ( name , lokiSvc () ) ;
  Assert ( a.validPointer() , "Unable to locate algorithm '" + name + "'" ) ;
  return a ;
}
// ============================================================================
// get the algorithms
// ============================================================================
void LoKi::Algorithms::AnyPassed::getAlgorithms () const  // get the algorithm
{
  // unsigned int i = 0 ;
  std::transform( m_names.begin(), m_names.end(),
                  std::back_inserter(m_algorithms),
                  [&](const std::string& name) {
    // Warning( " AQUICRE alg: " + a->name()
    // + Gaudi::Utils::toCpp ( ++i ) + "/"
    // + Gaudi::Utils::toCpp ( m_names.size() ) ) ;
                      return this->getAlgorithm(name);
                  });
}
// ============================================================================
// constructor from the algorithm name
// ============================================================================
LoKi::Algorithms::AnyPassed::AnyPassed
( std::vector<std::string> names )
  : LoKi::AuxFunBase ( std::tie ( names ) )
  , m_names     ( std::move(names) )
{
  if ( gaudi() ) { getAlgorithms() ; }
}
// ============================================================================
// MANDATORY: virtual destructor
// ============================================================================
LoKi::Algorithms::AnyPassed::~AnyPassed()
{
  for ( auto& ia : m_algorithms) {
    if ( ia && !gaudi() ) {
      // Warning("IAlgorithm: manual reset!") ;
      ia.reset() ;
    }
  }
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AnyPassed::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::any_of ( begin () , end   () , filterPassed ) ;
}
// ============================================================================
// OPTIONAL: nice printout
// ============================================================================
std::ostream& LoKi::Algorithms::AnyPassed::print
( const std::string& name , std::ostream& s ) const
{
  s << name << "(";
  if      ( 4 == algNames().size() ) {
    s << Gaudi::Utils::toString ( algName( 0 ) ) << ","
      << Gaudi::Utils::toString ( algName( 1 ) ) << ","
      << Gaudi::Utils::toString ( algName( 2 ) ) << ","
      << Gaudi::Utils::toString ( algName( 3 ) ) ;
  } else if ( 3 == algNames().size() ) {
    s << Gaudi::Utils::toString ( algName( 0 ) ) << ","
      << Gaudi::Utils::toString ( algName( 1 ) ) << ","
      << Gaudi::Utils::toString ( algName( 2 ) ) ;
  } else if ( 2 == algNames().size() ) {
    s << Gaudi::Utils::toString ( algName( 0 ) ) << ","
      << Gaudi::Utils::toString ( algName( 1 ) ) ;
  } else {
    s << Gaudi::Utils::toString( algNames() ) ;
  }
  return s << ")" ;
}


// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AllPassed::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return !empty()  && std::all_of( begin () , end () ,  filterPassed ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AnyEnabled::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::any_of( begin () , end () , isEnabled ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AllEnabled::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return !empty()  && std::all_of( begin () , end () , isEnabled ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AnyExecuted::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::any_of( begin () , end () , isExecuted ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::AllExecuted::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return !empty()  && std::all_of ( begin () , end () , isExecuted ) ;
}
// ============================================================================

// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Algorithms::NumPassed::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::count_if( begin () , end() , filterPassed ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Algorithms::NumEnabled::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::count_if ( begin () , end() , isEnabled ) ;
}
// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
double LoKi::Algorithms::NumExecuted::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  return std::count_if ( begin () , end() , isExecuted ) ;
}

// ============================================================================
// MANDATORY: the only one essential method
// ============================================================================
bool LoKi::Algorithms::RunAll::operator() () const
{
  //
  if ( algNames().size() != algorithms().size() ) { getAlgorithms() ; }
  //
  //
  for ( IAlgorithm* alg : algorithms() ){
    if ( !alg ) {
      Warning("Invalid algorithm!, return false " );
      return false ;                                                  // RETURN
    }
    //
    if ( !isEnabled( alg ) ) {
      Warning("Algorithm '" + alg->name() + "' is disabled, return false " );
      return false ;                                                  // RETURN
    }
    //
    if ( !isExecuted ( alg ) ) {
      StatusCode sc = alg->sysExecute(SYSEX_ARGUMENT) ;  // EXECUTE IT!!!
      if ( sc.isFailure() ) {
        Error("Error from algorithm '" + alg->name() + "' sysExecute", sc );
        return false ;                                                // RETURN
      }
    }
    //
    if ( !filterPassed ( alg ) ) { return false ; }                           // RETURN
  }
  //
  return true ;
}
// ============================================================================
// The END
// ============================================================================
