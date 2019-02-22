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
// Include files
#include <algorithm>
#include <functional>
#include <list>

#include "boost/variant.hpp"

// from Gaudi
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/SystemOfUnits.h"

// from PartProp
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"

// local
#include "MCDecayFinder.h"

//-----------------------------------------------------------------------------
// Implementation file for class : MCDecayFinder
//
// 20/04/2002 : Olivier Dormond
//-----------------------------------------------------------------------------
namespace {
  static const std::string s_brackets{"{}()[]"}; // sanity check the brackets in the decay descriptor

  template <typename C>
  void delete_( C* c ) {
    if ( !c ) return;
    for ( auto& m : *c ) delete m;
    delete c;
  }

  static inline int DIGIT( int n, int id ) {
    int base = 1;
    for ( ; n; n-- ) base *= 10;
    return ( ( std::abs( id ) % ( 10 * base ) ) / base );
  }

  static inline int quarkOne( int id ) { return DIGIT( 3, id ); }

  static inline int quarkTwo( int id ) { return DIGIT( 2, id ); }

  static inline int quarkThree( int id ) { return DIGIT( 1, id ); }

  static inline bool isIon( int id ) { return ( 1 == DIGIT( 10, id ) ); }

  static inline bool isHadron( int id ) { return abs( id ) >= 110; }

  static inline bool isMeson( int id ) { return ( isHadron( id ) && !isIon( id ) && 0 == quarkOne( id ) ); }

  static const int QuarksCharge[] = {0, -1, 1, -1, 1, -1, 1};

  static int firstQuark( int id ) {
    int q;

    if ( !isHadron( id ) || isIon( id ) ) return 0;

    if ( isMeson( id ) ) {
      q = quarkTwo( id );
      q *= QuarksCharge[q];
    } else
      q = quarkOne( id );

    return q * ( id > 0 ? 1 : -1 );
  }

  static int secondQuark( int id ) {

    if ( !isHadron( id ) || isIon( id ) ) return 0;

    int q;
    if ( isMeson( id ) ) {
      q     = quarkThree( id );
      int r = quarkTwo( id );
      if ( QuarksCharge[q] == QuarksCharge[r] )
        q *= -QuarksCharge[q];
      else
        q *= QuarksCharge[q];
    } else
      q = quarkTwo( id );

    return q * ( id > 0 ? 1 : -1 );
  }

  static int thirdQuark( int id ) {
    if ( !isHadron( id ) || isIon( id ) ) return 0;
    int q = ( isMeson( id ) ? 0 : quarkThree( id ) );
    return q * ( id > 0 ? 1 : -1 );
  }

  // FIXME: with version of Gaudi 30r1 or higher, just
  //  #include "GaudiKernel/compose.h"
  namespace Compose {
    template <typename... lambda_ts>
    struct composer_t;

    template <typename lambda_t>
    struct composer_t<lambda_t> : lambda_t {
      composer_t( const lambda_t& lambda ) : lambda_t{lambda} {}
      composer_t( lambda_t&& lambda ) : lambda_t{std::move( lambda )} {}

      using lambda_t::operator();
    };

    template <typename lambda_t, typename... more_lambda_ts>
    struct composer_t<lambda_t, more_lambda_ts...> : lambda_t, composer_t<more_lambda_ts...> {
      using super_t = composer_t<more_lambda_ts...>;

      template <typename... lambda_ts>
      composer_t( const lambda_t& lambda, lambda_ts&&... more_lambdas )
          : lambda_t{lambda}, super_t{std::forward<lambda_ts>( more_lambdas )...} {}
      template <typename... lambda_ts>
      composer_t( lambda_t&& lambda, lambda_ts&&... more_lambdas )
          : lambda_t{std::move( lambda )}, super_t{std::forward<lambda_ts>( more_lambdas )...} {}

      using lambda_t::operator();
      using super_t:: operator();
    };

    template <typename... lambda_ts>
    composer_t<std::decay_t<lambda_ts>...> compose( lambda_ts&&... lambdas ) {
      return {std::forward<lambda_ts>( lambdas )...};
    }
  } // namespace Compose

  auto dispatch_variant = []( auto&& variant, auto&&... lambdas ) -> decltype( auto ) {
    return boost::apply_visitor( Compose::compose( std::forward<decltype( lambdas )>( lambdas )... ),
                                 std::forward<decltype( variant )>( variant ) );
  };

} // namespace

class MCDecayFinder::ParticleMatcher final {
public:
  ParticleMatcher( LHCb::IParticlePropertySvc* ppSvc );
  ParticleMatcher( std::string* name, LHCb::IParticlePropertySvc* ppSvc );
  ParticleMatcher( Quarks q1, Quarks q2, Quarks q3, LHCb::IParticlePropertySvc* ppSvc );
  ParticleMatcher( Quantums quantum, Relations relation, double value, LHCb::IParticlePropertySvc* ppSvc );
  bool        test( const LHCb::MCParticle* part, LHCb::MCParticle::ConstVector* collect = nullptr );
  void        setLift() { lift = true; }
  bool        getLift() { return lift; }
  void        setEmpty() { empty_f = true; }
  bool        getEmpty() { return empty_f; }
  void        setQmark() { qmark = true; }
  bool        getQmark() { return qmark; }
  void        setConjugate() { conjugate = true; }
  bool        getConjugate() { return conjugate; }
  void        setOscillate() { oscillate = true; }
  void        setNotOscillate() { noscillate = true; }
  bool        getOscillate() { return oscillate; }
  bool        getNotOscillate() { return noscillate; }
  void        setInverse() { inverse = true; }
  bool        getInverse() { return inverse; }
  void        setStable() { stable = true; }
  bool        getStable() { return stable; }
  bool        getExact() { return !qmark && !inverse && !conjugate && parms.which() == 1; }
  void        conjugateID();
  std::string describe();

private:
  int conjugatedID( int id );

  struct quark_t {
    Quarks q1, q2, q3;
  };
  struct quantum_t {
    Quantums  q;
    Relations r;
    double    d;
  };
  struct stdhep_t {
    int id;
  };
  boost::variant<boost::blank, stdhep_t, quark_t, quantum_t> parms;

  bool                              lift       = false;
  bool                              empty_f    = false;
  bool                              qmark      = false;
  bool                              conjugate  = false;
  bool                              oscillate  = false;
  bool                              noscillate = false;
  bool                              inverse    = false;
  bool                              stable     = false;
  const LHCb::IParticlePropertySvc* m_ppSvc    = nullptr;
};

class MCDecayFinder::Descriptor final {
public:
  Descriptor( LHCb::IParticlePropertySvc* ppSvc, double resonnanceThreshold );
  Descriptor( Descriptor& copy );
  Descriptor( ParticleMatcher* mother, LHCb::IParticlePropertySvc* ppSvc, double resonnanceThreshold );

  ~Descriptor();

  template <class iter>
  bool test( const iter first, const iter last, const LHCb::MCParticle*& previous_result ) {
    iter start;
    if ( previous_result && ( ( start = std::find( first, last, previous_result ) ) == last ) ) {
      previous_result = nullptr;
      return false; // Bad previous_result
    }
    if ( previous_result ) start++;

    if ( !mother ) { // No mother == pp collision
      std::list<const LHCb::MCParticle*> prims;
      for ( auto i = ( previous_result ? start : first ); i != last; i++ ) {
        const LHCb::MCVertex* origin = ( *i )->originVertex();
        if ( origin && origin->mother() ) continue;
        prims.push_back( *i );
      }
      if ( skipResonance ) filterResonances( prims );
      if ( testDaughters( prims ) ) {
        previous_result = (const LHCb::MCParticle*)1;
        return true;
      }
      if ( getAlternate() ) getAlternate()->test( first, last, previous_result );
      return false;
    }

    auto part_i = ( previous_result ? start : first );
    while ( ( part_i != last ) && ( test( *part_i ) == false ) ) part_i++;

    if ( part_i != last ) {
      previous_result = *part_i;
      return true;
    }
    return false;
  }
  bool test( const LHCb::MCParticle* mother, LHCb::MCParticle::ConstVector* collect = nullptr,
             std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>* subTree = nullptr );

  void        setAlternate( Descriptor* a ) { alternate.reset( a ); }
  Descriptor* getAlternate() { return alternate.get(); }

  void setMother( ParticleMatcher* m ) { mother.reset( m ); }

  void                      addDaughter( Descriptor* daughter );
  std::vector<Descriptor*>& getDaughters() { return daughters; }

  void        setElipsis() { elipsis = true; }
  bool        getElipsis() { return elipsis; }
  void        setResonance() { skipResonance = true; }
  void        conjugate();
  std::string describe();

private:
  bool
       testDaughters( std::list<const LHCb::MCParticle*>& parts, LHCb::MCParticle::ConstVector* collect = nullptr,
                      std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>* subTree = nullptr );
  void addNonResonantDaughters( std::list<const LHCb::MCParticle*>& parts, const LHCb::MCParticle* part );
  void filterResonances( std::list<const LHCb::MCParticle*>& parts );

  std::unique_ptr<ParticleMatcher> mother;
  std::vector<Descriptor*>         daughters; // wildcard mother at the end!
  bool                             skipResonance  = false;
  bool                             elipsis        = false;
  double                           m_resThreshold = 0;
  LHCb::IParticlePropertySvc*      m_ppSvc        = nullptr;

  std::unique_ptr<Descriptor> alternate;
};

DECLARE_COMPONENT( MCDecayFinder )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
MCDecayFinder::MCDecayFinder( const std::string& type, const std::string& name, const IInterface* parent )
    : base_class( type, name, parent ) {
  declareProperty( "Decay", m_source );
}

//=============================================================================
// Standard destructor
//=============================================================================

MCDecayFinder::~MCDecayFinder() {
  delete m_decay;
  delete_( m_members );
}

//=============================================================================

StatusCode MCDecayFinder::initialize() {
  StatusCode sc = base_class::initialize();
  if ( !sc ) return sc;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Initializing" << endmsg;

  m_ppSvc = svc<LHCb::IParticlePropertySvc>( "LHCb::ParticlePropertySvc", true );

  if ( m_source.empty() ) { return Warning( "No decay specified!", StatusCode::SUCCESS ); }
  if ( compile( m_source ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "The compilation of the decay was successful" << endmsg;
    return StatusCode::SUCCESS;
  }
  return Error( "Could not compile the decay description" );
}

StatusCode MCDecayFinder::setDecay( std::string decay ) {

  Descriptor*                    old_decay   = m_decay;
  std::vector<ParticleMatcher*>* old_members = m_members;
  // Be sure we don't get a old piece if the feature is not used.
  m_decay   = nullptr;
  m_members = nullptr;

  if ( msgLevel( MSG::DEBUG ) ) debug() << "Setting decay to " << decay << endmsg;
  if ( sanityCheck( decay ) && compile( decay ) ) {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "The compilation of the decay was successfull" << endmsg;
    m_source = decay;
    delete old_decay;
    delete_( old_members );
    return StatusCode::SUCCESS;
  }

  // Restore previous decay if compilation failed.
  if ( m_decay != old_decay ) {
    delete m_decay;
    m_decay = old_decay;
  }
  if ( m_members != old_members ) {
    delete_( m_members );
    m_members = old_members;
  }
  err() << "Could not compile the decay description: " << decay << endmsg;

  return StatusCode::FAILURE;
}

std::string MCDecayFinder::revert() const {
  std::string result;
  if ( !m_decay ) return result;
  bool alt = ( m_decay->getAlternate() != nullptr );
  if ( alt ) result += "{ ";
  Descriptor* a = m_decay;
  while ( a ) {
    result += a->describe();
    if ( a->getAlternate() ) result += ", ";
    a = a->getAlternate();
  }
  if ( alt ) result += " }";
  return result;
}

#include "mclexer.icpp"
#include "mcparser.icpp"

bool MCDecayFinder::compile( std::string& source ) {
  yy_buffer_state* bs = yy_scan_string( source.c_str() );
  try {
    if ( yparse() ) throw( "Syntax Error" );
  } catch ( DescriptorError e ) {
    err() << "Invalid decay description '" << source << "'" << endmsg;
    err() << e.cause() << endmsg;
    yy_delete_buffer( bs );
    return false;
  }
  yy_delete_buffer( bs );
  if ( msgLevel( MSG::DEBUG ) ) debug() << "Result of the compilation:\n" << revert() << endmsg;
  return true;
}

bool MCDecayFinder::hasDecay( const LHCb::MCParticle::ConstVector& event ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "About to test the event" << endmsg;
  if ( !m_decay ) Exception( "Trying to find an unspecified decay!" );
  const LHCb::MCParticle* drop_me = nullptr;
  return m_decay->test( event.begin(), event.end(), drop_me );
}

bool MCDecayFinder::findDecay( const LHCb::MCParticle::ConstVector& event,
                               const LHCb::MCParticle*&             previous_result ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "About to test the event" << endmsg;
  if ( !m_decay ) Exception( "Trying to find an unspecified decay!" );
  bool r = m_decay->test( event.begin(), event.end(), previous_result );
  if ( !r && UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Could not find decay" << endmsg;
  return r;
}

bool MCDecayFinder::hasDecay( const LHCb::MCParticles& event ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "About to test the event" << endmsg;
  if ( !m_decay ) Exception( "Trying to find an unspecified decay!" );
  const LHCb::MCParticle* drop_me = nullptr;
  bool                    r       = m_decay->test( event.begin(), event.end(), drop_me );
  if ( !r && UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Could not find decay" << endmsg;
  return r;
}

bool MCDecayFinder::findDecay( const LHCb::MCParticles& event, const LHCb::MCParticle*& previous_result ) const {
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "About to test the event" << endmsg;
  if ( !m_decay ) Exception( "Trying to find an unspecified decay!" );
  bool r = m_decay->test( event.begin(), event.end(), previous_result );
  if ( !r && UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Could not find decay" << endmsg;
  return r;
}

bool MCDecayFinder::hasDecay() const {
  LHCb::MCParticles* mcparts = get<LHCb::MCParticles>( LHCb::MCParticleLocation::Default );
  if ( !mcparts ) {
    fatal() << "Unable to find MC particles at '" << LHCb::MCParticleLocation::Default << "'" << endmsg;
    return false;
  }
  return hasDecay( *mcparts );
}

bool MCDecayFinder::findDecay( const LHCb::MCParticle*& previous_result ) const {
  LHCb::MCParticles* mcparts = get<LHCb::MCParticles>( LHCb::MCParticleLocation::Default );
  if ( !mcparts ) {
    fatal() << "Unable to find MC particles at '" << LHCb::MCParticleLocation::Default << "'" << endmsg;
    return false;
  }
  return findDecay( *mcparts, previous_result );
}

void MCDecayFinder::descendants( const LHCb::MCParticle* head, LHCb::MCParticle::ConstVector& result,
                                 bool leaf ) const {
  bool term = true;
  for ( const auto& v : head->endVertices() ) {
    for ( const auto& p : v->products() ) {
      term = false;
      descendants( p, result, leaf );
    }
  }

  if ( !leaf || term ) result.push_back( const_cast<LHCb::MCParticle*>( head ) );
}

void MCDecayFinder::decayMembers( const LHCb::MCParticle* head, LHCb::MCParticle::ConstVector& members ) const {
  if ( m_members ) {
    LHCb::MCParticle::ConstVector flat;
    descendants( head, flat, false );
    std::copy_if( flat.begin(), flat.end(), std::back_inserter( members ), [&]( const LHCb::MCParticle* p ) {
      return std::any_of( m_members->begin(), m_members->end(), [&]( const auto& m ) { return m->test( p ); } );
    } );
  }
  m_decay->test( head, &members );
}

void MCDecayFinder::decaySubTrees(
    const LHCb::MCParticle*                                                         head,
    std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>& subtrees ) const {
  m_decay->test( head, nullptr, &subtrees );
}

MCDecayFinder::Descriptor::Descriptor( LHCb::IParticlePropertySvc* ppSvc, double rThre )
    : m_resThreshold( rThre ), m_ppSvc( ppSvc ) {}

MCDecayFinder::Descriptor::Descriptor( Descriptor& copy ) {
  if ( copy.mother ) mother = std::make_unique<ParticleMatcher>( *copy.mother );
  for ( const auto& d : copy.daughters ) { daughters.push_back( new Descriptor( *d ) ); }
  skipResonance  = copy.skipResonance;
  elipsis        = copy.elipsis;
  m_resThreshold = copy.m_resThreshold;
  m_ppSvc        = copy.m_ppSvc;
}

MCDecayFinder::Descriptor::Descriptor( ParticleMatcher* m, LHCb::IParticlePropertySvc* ppSvc, double rThre )
    : mother( m ), m_resThreshold( rThre ), m_ppSvc( ppSvc ) {}

MCDecayFinder::Descriptor::~Descriptor() {
  for ( auto& d : daughters ) delete d;
}

std::string MCDecayFinder::Descriptor::describe() {
  std::string result;
  if ( !daughters.empty() ) result += '(';
  result += ( mother ? mother->describe() : "pp" );
  if ( !daughters.empty() ) {
    result += ( skipResonance ? " => " : " -> " );
    for ( auto di = daughters.begin(); di != daughters.end(); di++ ) {
      result += ( *di )->describe();
      if ( *di != daughters.back() ) result += ' ';
    }
  }
  if ( elipsis ) result += " ...";
  if ( !daughters.empty() ) result += ')';
  return result;
}

bool MCDecayFinder::Descriptor::test(
    const LHCb::MCParticle* part, LHCb::MCParticle::ConstVector* collect,
    std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>* subtrees ) {
  LHCb::MCParticle::ConstVector local_collect( 0 );
  auto*                         local  = ( ( collect || subtrees ) ? &local_collect : nullptr );
  bool                          result = false;
  if ( mother && mother->test( part, local ) ) {
    if ( daughters.empty() ) {
      result = true; // Nothing to test for the daughters.
    } else {
      std::list<const LHCb::MCParticle*> parts;
      for ( const auto& vi : part->endVertices() ) {
        if ( !vi ) continue;
        std::copy_if( vi->products().begin(), vi->products().end(), std::back_inserter( parts ),
                      []( const LHCb::MCParticle* p ) { return p != nullptr; } );
      }
      if ( skipResonance ) filterResonances( parts );

      if ( subtrees ) {
        std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>> local_subtrees;
        result = testDaughters( parts, local, &local_subtrees );
        if ( result ) subtrees->insert( subtrees->end(), local_subtrees.begin(), local_subtrees.end() );
      } else {
        result = testDaughters( parts, local );
      }
    }
  }
  if ( result ) {
    if ( collect ) collect->insert( collect->end(), local_collect.begin(), local_collect.end() );
    if ( subtrees && !daughters.empty() ) subtrees->emplace_back( part, local_collect );
    return true;
  }
  return alternate && alternate->test( part, collect, subtrees );
}

bool MCDecayFinder::Descriptor::testDaughters(
    std::list<const LHCb::MCParticle*>& parts, LHCb::MCParticle::ConstVector* collect,
    std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>* subtrees ) {
  auto di = daughters.begin();
  for ( ; ( di != daughters.end() ) && !parts.empty(); di++ ) {
    if ( !*di ) continue;
    auto p = std::find_if( parts.begin(), parts.end(),
                           [&]( const LHCb::MCParticle* q ) { return ( *di )->test( q, collect, subtrees ); } );
    if ( p == parts.end() ) return false; // None of the parts has matched the test
    parts.erase( p );
  }
  // enough particles,  just the right number of particles, or too many particles but we don't care
  return di == daughters.end() && ( parts.empty() || elipsis );
}

void MCDecayFinder::Descriptor::addDaughter( Descriptor* daughter ) {
  if ( !daughter->mother ) return; // empty daughter of a {}

  if ( daughter->mother->getQmark() ) {
    daughters.push_back( daughter );
    return;
  }
  if ( daughter->mother->getExact() ) {
    daughters.insert( daughters.begin(), daughter );
    return;
  }
  auto d =
      std::find_if_not( daughters.begin(), daughters.end(), []( const auto& _ ) { return _->mother->getExact(); } );
  daughters.insert( d, daughter );
}

void MCDecayFinder::Descriptor::addNonResonantDaughters( std::list<const LHCb::MCParticle*>& parts,
                                                         const LHCb::MCParticle*             part ) {
  if ( !m_ppSvc ) return;
  for ( const auto& vi : part->endVertices() ) {
    if ( !vi ) break;
    for ( const auto& idau : vi->products() ) {
      if ( !idau ) break;

      const LHCb::ParticleProperty* pp = m_ppSvc->find( idau->particleID() );
      //      std::cout << "addNonResonantDaughters " << idau->particleID() << " " << pp << std::endl ;

      if ( !pp ) {
        // throw DescriptorError(std::string("Unknown particle '")+"'");
        std::cout << "MCDecayFinder::Descriptor::addNonResonantDaughters WARNING Particle property not obtainable for "
                  << idau->particleID() << std::endl;
        break;
      }

      if ( pp->lifetime() >= m_resThreshold )
        parts.push_front( idau );
      else
        addNonResonantDaughters( parts, idau );
    }
  }
}

void MCDecayFinder::Descriptor::filterResonances( std::list<const LHCb::MCParticle*>& parts ) {
  if ( !m_ppSvc ) return;
  auto pi = parts.begin();
  while ( pi != parts.end() ) {
    const LHCb::ParticleProperty* pp = m_ppSvc->find( ( *pi )->particleID() );
    if ( !pp ) {
      //      throw DescriptorError(std::string("Unknown particle '")+"'");
      std::cout << "MCDecayFinder::Descriptor::filterResonances WARNING Particle property not obtainable for "
                << ( *pi )->particleID() << std::endl;
      break;
    }

    if ( pp->lifetime() < m_resThreshold ) {
      const LHCb::MCParticle* part = *pi;
      pi                           = parts.erase( pi );
      addNonResonantDaughters( parts, part ); // Daughters are prepended.
    } else
      ++pi;
  }
}

void MCDecayFinder::Descriptor::conjugate( void ) {
  if ( mother ) mother->conjugateID();
  for ( auto& d : daughters ) d->conjugate();
}

MCDecayFinder::ParticleMatcher::ParticleMatcher( LHCb::IParticlePropertySvc* ppSvc )
    : parms{boost::blank{}}, m_ppSvc( ppSvc ) {}

MCDecayFinder::ParticleMatcher::ParticleMatcher( std::string* name, LHCb::IParticlePropertySvc* ppSvc )
    : m_ppSvc( ppSvc ) {
  const LHCb::ParticleProperty* pp = m_ppSvc->find( *name );
  if ( !pp ) throw DescriptorError( std::string( "Unknown particle '" ) + *name + "'" );
  parms = stdhep_t{pp->pid().pid()};
}

MCDecayFinder::ParticleMatcher::ParticleMatcher( Quarks q1, Quarks q2, Quarks q3, LHCb::IParticlePropertySvc* ppSvc )
    : parms{quark_t{q1, q2, q3}}, m_ppSvc( ppSvc ) {}

MCDecayFinder::ParticleMatcher::ParticleMatcher( Quantums q, Relations r, double d, LHCb::IParticlePropertySvc* ppSvc )
    : parms{quantum_t{q, r, d}}, m_ppSvc( ppSvc ) {}

std::string MCDecayFinder::ParticleMatcher::describe() {
  std::string result;
  if ( stable ) result += '(';
  if ( lift ) result += '^';
  if ( qmark ) result += '?';
  if ( conjugate ) result += '[';
  if ( oscillate ) result += '[';
  if ( noscillate ) result += '[';
  if ( inverse ) result += '!';
  dispatch_variant( parms, [&]( const boost::blank& ) { result += "## MUST NOT COMPILE ##"; },
                    [&]( const stdhep_t& stdhep ) {
                      if ( !m_ppSvc ) return;
                      if ( !m_ppSvc->find( LHCb::ParticleID( stdhep.id ) ) )
                        throw DescriptorError( std::string( "Unknown particle" ) );
                      ;
                      result += m_ppSvc->find( LHCb::ParticleID( stdhep.id ) )->particle();
                    },
                    [&]( const quark_t& quarks ) {
                      auto append = []( std::string& s, Quarks q ) {
                        switch ( q ) {
                        case up:
                          s += 'u';
                          break;
                        case down:
                          s += 'd';
                          break;
                        case charm:
                          s += 'c';
                          break;
                        case strange:
                          s += 's';
                          break;
                        case top:
                          s += 't';
                          break;
                        case bottom:
                          s += 'b';
                          break;
                        case antiup:
                          s += "u~";
                          break;
                        case antidown:
                          s += "d~";
                          break;
                        case anticharm:
                          s += "c~";
                          break;
                        case antistrange:
                          s += "s~";
                          break;
                        case antitop:
                          s += "t~";
                          break;
                        case antibottom:
                          s += "b~";
                          break;
                        case empty:
                          break;
                        };
                      };
                      result += "<X";
                      append( result, quarks.q1 );
                      append( result, quarks.q2 );
                      append( result, quarks.q3 );
                      result += '>';
                    },
                    [&]( const quantum_t& ) { result += "## NOT IMPLEMENTED ##"; } );

  if ( noscillate ) result += "]nos";
  if ( oscillate ) result += "]os";
  if ( conjugate ) result += "]cc";
  if ( stable ) result += ')';
  return result;
}

bool MCDecayFinder::ParticleMatcher::test( const LHCb::MCParticle* part, LHCb::MCParticle::ConstVector* collect ) {
  bool result = dispatch_variant(
      parms, [&]( const boost::blank& ) { return true; },
      [&]( const stdhep_t& stdhep ) {
        bool result = ( stdhep.id == part->particleID().pid() );
        if ( conjugate ) {
          int cc_id = conjugatedID( stdhep.id );
          result    = result || ( cc_id == part->particleID().pid() );
        }
        if ( oscillate ) result = result && part->hasOscillated();
        if ( noscillate ) result = result && ( !part->hasOscillated() );
        if ( inverse ) result = !result;
        if ( stable ) {
          result = result && std::any_of( part->endVertices().begin(), part->endVertices().end(),
                                          []( const auto& v ) { return !v->products().empty(); } );
        }
        return result;
      },
      [&]( const quark_t& quarks ) {
        static const Quarks Q[]  = {empty, down, up, strange, charm, bottom, top};
        static const Quarks AQ[] = {empty, antidown, antiup, antistrange, anticharm, antibottom, antitop};
        int                 q    = firstQuark( part->particleID().pid() );
        Quarks              q1   = ( q < 0 ? AQ[-q] : Q[q] );
        Quarks              cq1  = ( q < 0 ? Q[-q] : AQ[q] ); // cc hypothesis
        q                        = secondQuark( part->particleID().pid() );
        Quarks q2                = ( q < 0 ? AQ[-q] : Q[q] );
        Quarks cq2               = ( q < 0 ? Q[-q] : AQ[q] ); // cc hypothesis
        q                        = thirdQuark( part->particleID().pid() );
        Quarks q3                = ( q < 0 ? AQ[-q] : Q[q] );
        Quarks cq3               = ( q < 0 ? Q[-q] : AQ[q] ); // cc hypothesis

        // We don't care of the ordering so we check all permutations.
        auto cmp3 = []( quark_t qqq1, quark_t qqq2 ) {
          auto cmp = []( quark_t qqq1, Quarks q_1, Quarks q_2, Quarks q_3 ) {
            return ( qqq1.q1 == q_1 || qqq1.q1 == empty ) && ( qqq1.q2 == q_2 || qqq1.q2 == empty ) &&
                   ( qqq1.q3 == q_3 || qqq1.q3 == empty );
          };
          return cmp( qqq1, qqq2.q1, qqq2.q2, qqq2.q3 ) || cmp( qqq1, qqq2.q1, qqq2.q3, qqq2.q2 ) ||
                 cmp( qqq1, qqq2.q2, qqq2.q1, qqq2.q3 ) || cmp( qqq1, qqq2.q2, qqq2.q3, qqq2.q1 ) ||
                 cmp( qqq1, qqq2.q3, qqq2.q1, qqq2.q2 ) || cmp( qqq1, qqq2.q3, qqq2.q2, qqq2.q1 );
        };

        //                                         Should we check for the charge conjugated particle ?
        return cmp3( quarks, quark_t{q1, q2, q3} ) || ( conjugate && cmp3( quarks, quark_t{cq1, cq2, cq3} ) );
      },
      [&]( const quantum_t& ) {
        // ******* NOT IMPLEMENTED YET *******
        return false;
      } );
  if ( result && lift && collect ) collect->push_back( const_cast<LHCb::MCParticle*>( part ) );
  return result;
}

void MCDecayFinder::ParticleMatcher::conjugateID() {
  dispatch_variant( parms, []( boost::blank& ) {},
                    [&]( stdhep_t& stdhep ) { stdhep.id = this->conjugatedID( stdhep.id ); },
                    []( const auto& ) {
                      throw DescriptorError( "Charge conjugate only allowed"
                                             " on explicit particle or '?'" );
                    } );
}

int MCDecayFinder::ParticleMatcher::conjugatedID( int id ) {
  if ( m_ppSvc ) {
    const LHCb::ParticleProperty* pp = m_ppSvc->find( LHCb::ParticleID( id ) );
    if ( !pp ) { throw DescriptorError( "A particle was not known in the prop service" ); }
    const LHCb::ParticleProperty* anti = pp->antiParticle();
    return anti ? anti->pid().pid() : id; // if !anti, assume it has no antiparticle
  }
  // else warning() << "Particle property service not defined, guessing conjugate ID" << endmsg;

  int cc_id = -id;
  switch ( cc_id ) { // Particles which are their own anti-particle
  case -12:          // nu_e
  case -22:          // gamma
  case -23:          // Z0
  case -25:          // H_10
  case -35:          // H_20
  case -36:          // H_30
  case -111:         // pi0
  case -113:         // rho(770)
  case -115:         // a_2(1320)0
  case -130:         // KS0
  case -221:         // eta
  case -223:         // omega(782)
  case -310:         // K0L
  case -331:         // eta'
  case -333:         // phi(1020)
  case -441:         // eta_c(1S)
  case -443:         // J/psi(1S)
  case -445:         // chi_c2(1P)
  case -10441:       // chi_c0(1P)
  case -20113:       // a_1(1260)0
  case -20443:       // chi_c1(1P)
  case -100443:      // psi(2S)
  case -9000111:     // a_0(980)0
    cc_id *= -1;
    break;
    //  Interesting case of CPT violation ;-)
    //  case -310:    // KS0~ <-> KL0
    //    cc_id = 130;
    //    break;
    //  case -130:    // KL0~ <-> KS0
    //    cc_id = 310;
    //    break;
  default:
    break;
  }
  return cc_id;
}
bool MCDecayFinder::sanityCheck( const std::string& decay ) {
  // check for more than one lot of '...'
  auto apos = decay.find( "..." );
  if ( apos != std::string::npos ) {
    apos++;
    apos = decay.find( "...", apos );
    if ( apos != std::string::npos ) {
      warning() << "The decay descriptor " << decay
                << " has two lots of |...|. This would cause a segfault if I hadn't caught it here." << endmsg;
      return false;
    }
  }
  // check for mismatched brackets, first make the string with only brackets
  std::string bstring;
  std::copy_if( decay.cbegin(), decay.cend(), std::back_inserter( bstring ), []( const char& c ) {
    return std::any_of( s_brackets.begin(), s_brackets.end(), [&]( const char& b ) { return b == c; } );
  } );

  // check the number of brackets matches up
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Checking brackets " << bstring << endmsg;
  if ( bstring.empty() ) return true;
  if ( bstring.size() % 2 != 0 ) {
    warning() << "There is an uneven number of brackets in: " << bstring << endmsg;
    return false;
  }

  // loop until there is no string
  while ( bstring.size() > 1 ) {
    const unsigned int start = bstring.size();

    // loop over possible bracket combinations
    // iterate over n and n+1, to get opening and closing bracket
    for ( unsigned int itb = 0; bstring.size() > 1 && itb < s_brackets.size() - 1; itb += 2 ) {
      // loop over the string
      for ( unsigned int itd = 0; bstring.size() > 1 && itd < bstring.size() - 1; itd += 1 ) {
        // remove matching brackets
        if ( bstring[itd] == s_brackets[itb] && bstring[itd + 1] == s_brackets[itb + 1] ) {
          if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Removing : " << bstring[itd] << bstring[itd + 1] << endmsg;
          bstring.erase( itd, 2 );
          // reduced loop counter by two, to move to previous location
          if ( itd >= 2 )
            itd -= 2;
          else
            itd = 0;
          itb = 0;
          // unless I am at the beginning, then just go to back to zero
        }
      }
    }

    // break if the string is the same size at beginning and at end
    if ( start == bstring.size() ) break;
  }
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Removed matching brackets now: " << bstring << endmsg;
  if ( !bstring.empty() ) {
    warning() << "There are mismatched brackets in this decay descriptor. The non-matching brackets are: " << bstring
              << endmsg;
    return false;
  }
  // now check for at least one comma between all curly brackets

  bstring = decay;

  while ( !bstring.empty() ) {
    const unsigned int start = bstring.size();

    // find the first closing bracket }

    for ( unsigned int iclose = 0; bstring.size() != 0 && iclose < bstring.size(); iclose++ ) {
      if ( bstring[iclose] == s_brackets[1] ) {
        // find the opening bracket for this
        for ( unsigned int iopen = iclose - 1; true; iopen-- ) {
          if ( bstring[iopen] == s_brackets[0] ) {
            // check there is at least one comma
            unsigned int comma  = iopen + 1;
            char         acomma = ',';
            while ( comma < iclose ) {
              if ( bstring[comma] == acomma )
                break;
              else
                comma++;
            }
            if ( bstring[comma] != acomma ) {
              warning() << "every {} must contain at least one alternative. " << bstring << " does not" << endmsg;
              return false;
            } else {
              // OK so far, erase this bracket
              bstring.erase( iopen, iclose - iopen + 1 );
              // start from the beginning of this bracket
              iclose = iopen - 1;
              // stop looping over iopen, go on to the next closing bracket
              if ( msgLevel( MSG::VERBOSE ) )
                verbose() << "OK so far, moving onto the next curly bracket in " << bstring << endmsg;
              break;
            }
          } else if ( iopen <= 0 || iopen >= iclose ) {
            warning() << "there is something odd with the descriptor: " << bstring << endmsg;
            return false;
          }
        }
      }
    }

    if ( start == bstring.size() ) break;
  }
  // find the opening bracket for this
  // check that there is at least one comma
  // replace all with a #
  if ( msgLevel( MSG::VERBOSE ) ) verbose() << "Sanity check passed for decay: " << decay << endmsg;

  return true;
}
