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
//#define YYDEBUG 1
#ifdef YYDEBUG
#  include <cstdio>
#  include <cstdlib>
#endif
#include "Event/MCParticle.h"
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/compose.h"
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "MCInterfaces/IMCDecayFinder.h"
#include <algorithm>
#include <functional>
#include <list>
#include <string>
#include <variant>

//-----------------------------------------------------------------------------
// Implementation file for class : MCDecayFinder
//
// 20/04/2002 : Olivier Dormond
//-----------------------------------------------------------------------------

struct yy_buffer_state;

/** @class MCDecayFinder MCDecayFinder.h
 *
 *  This tool allows you to search a vector of MCParticle for a specific decay.
 *  The decay is given thru the property 'Decay' as a string which describe
 *  what you are looking for. The syntax of this expression has been designed
 *  to be as powerful as possible. It can express inclusive decays as well as
 *  (almost) any set of decay.
 *
 *  The description is made of 'particle' and 'decay'.
 *
 *  A 'particle' is one of:
 *
 *  - a particle name as known by the ParticlePropertySvc,
 *
 *  - a set of 'particle's expressed like this: <c>"{A, B, C}"</c>,
 *
 *  - a set of 'particle's or their charge conjugate, expressed as <c>"[A,B,C]cc"</c>,
 *
 *  - a 'particle' with it's oscillation flag set (<c>"[A]os"</c>),
 *
 *  - a 'wildcard', any particle with some quarks inside is expressed as <c>"<Xq>"</c>
 *    with up to tree quarks in place of <c>"q"</c>. Quarks are u,d,c,s,t,b,u~,d~,...
 *    A simple '?' is also a wildcard matching any single particle,
 *
 *  a 'decay' is one of:
 *
 *  - an optional <c>"("</c> and a 'particle' and the matching <c>")"</c> if needed,
 *
 *  - a <c>"("</c>, a 'particle', one of <c>"->"</c> or <c>"=>"</c>, a blank separated list of decay,
 *    an optional <c>"..."</c> and a <c>")"</c>, e.g. <c>"(B0 -> pi+ pi-)"</c>,
 *
 *  - a <c>"pp"</c> one of <c>"->"</c> or <c>"=>"</c> and a blank separated list of decay.
 *
 *  Note: the <c>"()"</c> are not needed around the first decay so the previous exemple
 *        could simply be written <c>"B0 -> pi+ pi-"</c> but they are mandatory for
 *        subdecay like <c>"B0 -> (J/psi(1S) -> mu+ mu-) KS0"</c>.
 *
 *  Note: If you want to find a stable particle you must explicily request that
 *        it decay to nothing but the arrow has to be dropped. So this looks
 *        like <c>"B0 -> D_s- (K+)"</c>, here the D_s- can decay to whatever but
 *        the K+ must not decay. NB: Right now secondary interaction are seen
 *        as decay!
 *
 *  Note: <c>"->"</c> means <c>"decay to"</c> and
 *        <c>"=>"</c> means "decay to .... with some resonnances in the middle".
 *        So if you want to catch B0 to D_s- K+ with any D*_s in beetween you
 *        simply write <c>"B0 => D_s- K+ ..."</c>. NB: The <c>"..."</c> is here to catch any
 *        number of particles of any kind so you get the products of the D*_s-.
 *
 *  Note: <c>"pp"</c> stands for <c>"proton - proton collision"</c>. The decay products will
 *        be matched against the particles with no mother. An implicit <c>"..."</c> is
 *        appended. Only the boolean information for that kind of find can be
 *        used, the returned particle is set (MCParticle *)1 in case of match
 *        NULL otherwise.
 *
 *  Note: alternate decays can be specified by puting them in a comma seperated
 *        list surrounded by braces (e.g. <c>"B0 -> pi+ pi- , B0 -> K+ pi-"</c>).
 *
 *  Note: the charge conjugate operator can also be applied to a set of decay
 *        (e.g. <c>"[B0 -> K+ pi-]cc"</c> == <c>"B0 -> K+ pi- , B0~ -> K- pi+"</c> (Please
 *        note the B0 turned into a B0~)).
 *
 *  Note: alternate set of daughters can also be specified by putting the sets
 *        in a comma seperated list surrounded by braces (e.g.
 *        <c>"B0 -> {pi+ pi-, K+ pi-, K- pi+, K+ K-}"</c>). If one of the alternate
 *        set has only one daughter, DO NOT put it first in the list has it
 *        will not be able to parse the decay correctly (it will fail indeed).
 *
 *  Note: you can also use the charge conjugate operator on the daughters set.
 *
 *  Note: you can even mix the []cc and the {}.
 *
 * Extracting particles from the decay:
 *
 * - It is possible to extract all particle in the decay tree which match a
 *   given id by preceeding the description with the list of particle you want
 *   to be extracted (cc, os, nos operator applies here too) terminated by a
 *   colon (e.g. <c>"pi+ : B0 -> pi+ pi-"</c>).
 *
 * - Precise particle inside the decay tree can also be flagged for extraction
 *   by preceeding them with a '^' (like <c>"B0 -> (^J/psi(1S) -> mu+ mu-) KS0"</c>).
 *
 * You can then retrieve these particles with the MCDecayFinder::members
 * method.
 *
 * NOTE: You realy don't want to know how it works!
 *
 *  @author Olivier Dormond
 *  @date   12/03/2002
 *
 * Edited by R Lambert 2009-03-04
 * Check the decay string for syntax errors before parsing
 * this avoids most segfaults
 *
 */
class MCDecayFinder : public extends<GaudiTool, IMCDecayFinder> {
public:
  /// Standard Constructor
  MCDecayFinder( const std::string& type, const std::string& name, const IInterface* parent );

  /// Destructor
  ~MCDecayFinder() override; ///< Destructor

  StatusCode initialize() override;

  /// Get/Set the decay string to find
  std::string decay() const override { return m_source; }
  StatusCode  setDecay( std::string decay ) override;

  std::string revert() const override;

  /// Does the described decay exists in the event?
  bool hasDecay( const LHCb::MCParticle::ConstVector& event ) const override;
  bool hasDecay( const LHCb::MCParticle::Container& event ) const override;
  bool hasDecay() const override;

  /** Try to find the (next) match of the decay in the event.
   *
   *  @return true if found, false otherwise.
   *  @param event the vector of particles in which to search for the decay.
   *  @param previous_result the address of the previously matched particle
   *         or NULL if you want to start from the begining. This is an In/Out
   *         parameter, a reference to a const LHCb::MCParticle *.
   *  The particle pointed to by previous_result must be contained in event.
   */
  bool findDecay( const LHCb::MCParticle::ConstVector& event, const LHCb::MCParticle*& previous_result ) const override;
  bool findDecay( const LHCb::MCParticle::Container& event, const LHCb::MCParticle*& previous_result ) const override;
  bool findDecay( const LHCb::MCParticle*& previous_result ) const override;

  /** Return the tree pointed at by head as a flat list.
   *
   *  @param head   the particle at the top of the decay tree.
   *  @param result the list to fill with the particle in the 'head' tree.
   *         The list is not cleared before use.
   *  @param leaf   a bool indicating whether to include all particles or only
   *         the one at the ends of the branches. (Default: all)
   */
  void descendants( const LHCb::MCParticle* head, LHCb::MCParticle::ConstVector& result,
                    bool leaf = false ) const override;

  /** Get a list of all the requested members that are present in a decay.
   *
   *  @param head    the head of a decay tree in which to find the members.
   *  @param members the resulting list.
   *  The members are requested from the decay descriptor either as a list of
   *  particles seperated from the decay by a ':' and/or by putting a '^' before
   *  any particle in the decay.
   */
  void decayMembers( const LHCb::MCParticle* head, LHCb::MCParticle::ConstVector& members ) const override;

  /// Get a vector of pairs <mother, products> for all sub-trees.
  void decaySubTrees(
      const LHCb::MCParticle*                                                         head,
      std::vector<std::pair<const LHCb::MCParticle*, LHCb::MCParticle::ConstVector>>& subtrees ) const override;

private:
  DataObjectReadHandle<LHCb::MCParticles> m_mcParts{this, "MCParticles", LHCb::MCParticleLocation::Default};
  /// Enumaration types used internally.
  enum Quarks {
    empty,
    up,
    down,
    charm,
    strange,
    top,
    bottom,
    antiup,
    antidown,
    anticharm,
    antistrange,
    antitop,
    antibottom
  };
  enum Quantums {
    Q_quantum = 1,
    B_quantum,
    l_quantum,
    I_quantum,
    J_quantum,
    S_quantum,
    L_quantum,
    G_quantum,
    C_quantum,
    P_quantum,
    CP_quantum,
    c_quantum,
    s_quantum,
    t_quantum,
    b_quantum
  };
  enum Relations { eq_rel = 1, lesseq_rel, greatereq_rel, less_rel, greater_rel, noteq_rel };
  /// The opaque representation of a particle matcher
  class ParticleMatcher;

  /// The opaque representation of the decay chain.
  class Descriptor;

  class DescriptorError {
  public:
    DescriptorError( std::string s ) : msg( s ) {}
    std::string& cause() { return msg; }

  private:
    std::string msg;
  };

  LHCb::IParticlePropertySvc*    m_ppSvc   = nullptr;
  std::string                    m_source  = "B0 -> pi+ pi-";
  Descriptor*                    m_decay   = nullptr;
  std::vector<ParticleMatcher*>* m_members = nullptr;
  Gaudi::Property<double>        m_resThreshold{this, "ResonanceThreshold", 1e-15 * Gaudi::Units::second};

  bool compile( std::string& decay );

#include "mcparser.h"

  int  yparse();
  void yerror( std::string msg ) { throw DescriptorError( msg ); }

  int ylex();

  int yygrowstack();

  static const short int ylhs[];
  static const short int ylen[];
  static const short int ydefred[];
  static const short int ydgoto[];
  static const short int ysindex[];
  static const short int yrindex[];
  static const short int ygindex[];
  static const short int ytable[];
  static const short int ycheck[];
  static int             ydebug;
  static int             ynerrs;
  static int             yerrflag;
  static int             ychar;
  static int             ystacksize;
  static short int*      yssp;
  static YYSTYPE*        yvsp;
  static YYSTYPE         yval;
  static short int*      yss;
  static short int*      ysslim;
  static YYSTYPE*        yvs;
#ifdef YYDEBUG
  static const char* const yname[];
  static const char* const yrule[];
#endif

  /// extra sanity checks to avoid segfaults, R. Lambert 04-03-2009
  bool sanityCheck( const std::string& decay );

public:
  static YYSTYPE ylval;
};

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

  auto dispatch_variant = []( auto&& variant, auto&&... lambdas ) -> decltype( auto ) {
    return std::visit( Gaudi::overload( std::forward<decltype( lambdas )>( lambdas )... ),
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
  bool        getExact() { return !qmark && !inverse && !conjugate && parms.index() == 1; }
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
  std::variant<std::monostate, stdhep_t, quark_t, quantum_t> parms;

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
  const LHCb::MCParticles* mcparts = m_mcParts.get();
  if ( !mcparts ) {
    fatal() << "Unable to find MC particles at '" << m_mcParts.objKey() << "'" << endmsg;
    return false;
  }
  return hasDecay( *mcparts );
}

bool MCDecayFinder::findDecay( const LHCb::MCParticle*& previous_result ) const {
  const LHCb::MCParticles* mcparts = m_mcParts.get();
  if ( !mcparts ) {
    fatal() << "Unable to find MC particles at '" << m_mcParts.objKey() << "'" << endmsg;
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
    : parms{std::monostate{}}, m_ppSvc( ppSvc ) {}

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
  dispatch_variant(
      parms, [&]( const std::monostate& ) { result += "## MUST NOT COMPILE ##"; },
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
      parms, [&]( const std::monostate& ) { return true; },
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
  dispatch_variant(
      parms, []( const std::monostate& ) {}, [&]( stdhep_t& stdhep ) { stdhep.id = this->conjugatedID( stdhep.id ); },
      []( ... ) {
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
