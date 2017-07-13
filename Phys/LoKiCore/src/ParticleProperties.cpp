// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <algorithm>
#include <shared_mutex>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/SystemOfUnits.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// Kernel
// ============================================================================
#include "Kernel/ParticleID.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/ParticleProperties.h"
#include "LoKi/Report.h"
#include "LoKi/Services.h"
#include "LoKi/Constants.h"
// Boost
// ============================================================================
#include "boost/lexical_cast.hpp"
#include "boost/optional.hpp"
#include "boost/container/flat_map.hpp"
// ============================================================================

// ============================================================================
/** @file
 *
 * Implementation file for functions form the file ParticleProperties.h
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
 *  @date 2001-01-23
 */
// ============================================================================
namespace
{
  // ==========================================================================
  /** @var s_InvalidPIDName
   *  representation of "invalid" particle ID
   */
  static const std::string s_InvalidPIDName = "Unknown" ;
  // ==========================================================================

  template <typename T, typename Mutex = std::shared_timed_mutex> // C++17: replace with shared_mutex...
  class Synced {
      T m_obj;
      mutable Mutex m_mtx;
  public:
      template <typename... Args>
      Synced(Args&&... args) : m_obj{ std::forward<Args>(args)... } {}

      template <typename F> auto with_wlock(F&& f) -> decltype(auto)
      { auto lock=std::unique_lock<Mutex>{m_mtx}; return f(m_obj); }
      template <typename F> auto with_rlock(F&& f) const -> decltype(auto)
      { auto lock=std::shared_lock<Mutex>{m_mtx}; return f(m_obj); }
  };

}
// ============================================================================
/*  retrieve particle ID from Particle name
 *  @param name particle name
 *  @return particle ID
 */
// ============================================================================
LHCb::ParticleID LoKi::Particles::pidFromName( const std::string& name )
{
  typedef boost::container::flat_map<std::string,LHCb::ParticleID> Map ;
  /// ATTENTION
  static Synced<Map> s_map{ std::make_pair( "gamma" , LHCb::ParticleID (    22 ) ),
                            std::make_pair( "e+"    , LHCb::ParticleID (   -11 ) ),
                            std::make_pair( "e-"    , LHCb::ParticleID (    11 ) ),
                            std::make_pair( "mu+"   , LHCb::ParticleID (   -13 ) ),
                            std::make_pair( "mu-"   , LHCb::ParticleID (    13 ) ),
                            std::make_pair( "pi+"   , LHCb::ParticleID (   211 ) ),
                            std::make_pair( "pi0"   , LHCb::ParticleID (   111 ) ),
                            std::make_pair( "pi-"   , LHCb::ParticleID (  -211 ) ),
                            std::make_pair( "K+"    , LHCb::ParticleID (   321 ) ),
                            std::make_pair( "K-"    , LHCb::ParticleID (  -321 ) ),
                            std::make_pair( "p+"    , LHCb::ParticleID (  2212 ) ),
                            std::make_pair( "p~-"   , LHCb::ParticleID ( -2212 ) ) };

  auto res = s_map.with_rlock( [&]( const Map& m )
                               -> boost::optional<LHCb::ParticleID> {
      auto i = m.find( name );
      if ( UNLIKELY( i == m.end() ) ) return {};
      return i->second ;
  });
  if (res) return *res;
  //
  const LHCb::ParticleProperty* pp = ppFromName( name ) ;
  if( UNLIKELY(!pp) )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::pidFromName:") +
        "LHCb::ParticleProperty* points to NULL for '"      +
        name + "' return ParticleID() " ) ;
    return LHCb::ParticleID();
  }
  // update the map:
  s_map.with_wlock( [&](Map& m) { m.try_emplace( name, pp->particleID() ); } );
  return pp -> particleID() ;
}
// ============================================================================
/* retrieve particle ID from Particle name
 *  @param names particle names
 *  @return particle IDs
 */
// ============================================================================
std::vector<LHCb::ParticleID>
LoKi::Particles::pidsFromNames ( const std::vector<std::string>& names )
{
  std::vector<LHCb::ParticleID> res ( names.size() ) ;
  std::transform ( names.begin() ,
                   names.end  () ,
                   res.begin  () , &pidFromName ) ;
  return res ;
}
// ============================================================================
/*  retrieve particle ID from Particle name
 *  @param name particle name
 *  @return particle ID
 */
// ============================================================================
const LHCb::ParticleProperty* LoKi::Particles::_ppFromName
( const std::string& name )
{
  const LoKi::Services& services = LoKi::Services::instance () ;
  LHCb::IParticlePropertySvc* ppSvc = services.ppSvc();
  if( !ppSvc )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::pidFromName:") +
        "LHCb::IParticlePropertySvc* points to NULL!"       +
        " return NULL " ) ;
    return nullptr ;
  }
  //
  return ppSvc -> find ( name ) ;
}
// ============================================================================
/*  retrieve particle ID from Particle name
 *  @param name particle name
 *  @return particle ID
 */
// ============================================================================
const LHCb::ParticleProperty* LoKi::Particles::ppFromName
( const std::string& name )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::_ppFromName ( name ) ;
  if ( !pp )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::ppFromName:")  +
        "ParticleProperty* points to NULL for '"      +
        name + "' return NULL " ) ;
    return nullptr ;
  }
  return pp ;
}
// ============================================================================
/*  retrieve ParticleProperty from ParticleID
 *  @param pid particle ID
 *  @param particle property
 */
// ============================================================================
const LHCb::ParticleProperty*
LoKi::Particles::_ppFromPID ( const LHCb::ParticleID& pid )
{
  const LoKi::Services& services = LoKi::Services::instance () ;
  LHCb::IParticlePropertySvc* ppSvc = services.ppSvc();
  if( !ppSvc )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::_ppFromPID(")   +
        boost::lexical_cast<std::string>( pid )  +
        "): LHCb::IParticlePropertySvc* points to NULL!"     +
        " return NULL " ) ;
    return nullptr ;
  }
  const LHCb::ParticleProperty* pp = ppSvc -> find ( pid ) ;
  //
  return pp ;
}
// ============================================================================
/*  retrieve ParticleProperty from ParticleID
 *  @param pid particle ID
 *  @param particle property
 */
// ============================================================================
const LHCb::ParticleProperty*
LoKi::Particles::__ppFromPID ( const LHCb::ParticleID& pid )
{
  const LoKi::Services& services = LoKi::Services::instance () ;
  LHCb::IParticlePropertySvc* ppSvc = services.ppSvc();
  return ppSvc ? ppSvc -> find ( pid ) : nullptr;
}
// ============================================================================
/*  retrieve ParticleProperty from ParticleID
 *  @param pid particle ID
 *  @param particle property
 */
// ============================================================================
const LHCb::ParticleProperty*
LoKi::Particles::ppFromPID ( const LHCb::ParticleID& pid )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::_ppFromPID( pid ) ;
  if( !pp )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::ppFromPID(")    +
        boost::lexical_cast<std::string>( pid )  +
        "): LHCb::ParticleProperty* points to NULL,"         +
        " return NULL " ) ;
    return nullptr ;
  }
  return pp ;
}
// ============================================================================
/*  retrieve ParticleProperty from ParticleID
 *  @param pid particle ID
 *  @param particle mass
 */
// ============================================================================
double LoKi::Particles::massFromPID  ( const LHCb::ParticleID& pid   )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::ppFromPID( pid ) ;
  if ( !pp )
  {
    LoKi::Report::Error
      ( " LoKi::Particles::massFromPID(" +
        boost::lexical_cast<std::string>( pid ) +
        ") : LHCb::ParticleProperty* points to NULL, return -1 * TeV " ) ;
    return -1.0 * Gaudi::Units::TeV  ;
  }
  return pp->mass() ;
}
// ============================================================================
/*  retrieve ParticleProperty from ParticleID
 *  @param pid particle ID
 *  @param particle property
 */
// ============================================================================
double LoKi::Particles::massFromName ( const std::string&  name )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::ppFromName( name ) ;
  if ( !pp )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::massFromName('") +
        name + "')" +
        "LHCb::ParticleProperty* points to NULL, return -1 * TeV " ) ;
    return -1.0 * Gaudi::Units::TeV  ;
  }
  return pp->mass() ;
}
// ============================================================================
/*  get name of 'antiparticle'
 *  @param name particlre name
 *  @return anme of antiParticle
 */
// ============================================================================
std::string LoKi::Particles::antiParticle( const std::string&      name )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::ppFromName( name ) ;
  if ( !pp )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::antiParticle('") +
        name + "')" +
        "ParticleProperty* points to NULL, return '"     +
        s_InvalidPIDName+"'" ) ;
    return s_InvalidPIDName ;
  }
  const LHCb::ParticleProperty* antiPP = LoKi::Particles::antiParticle( pp ) ;
  if ( !antiPP )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::antiParticle:") +
        "antiParticle('" + (pp->particle()) +
        "')points to NULL" +" return '"     +
        s_InvalidPIDName + "'" ) ;
    return s_InvalidPIDName ;
  }
  return antiPP->particle() ;
}
// ============================================================================
/*  get name of 'antiparticle'
 *  @param name particlre name
 *  @return anme of antiParticle
 */
// ============================================================================
const LHCb::ParticleProperty*
LoKi::Particles::antiParticle ( const LHCb::ParticleProperty* pp )
{
  if ( !pp )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::antiParticle") +
        "ParticleProperty* points to NULL, return NULL" ) ;
    return nullptr ;
  }
  const LHCb::ParticleProperty* antiPP = pp->antiParticle() ;
  if ( !antiPP )
  {
    LoKi::Report::Error
      ( std::string(" LoKi::Particles::antiParticle():") +
        "antiParticle('" + (pp->particle())+"') points to NULL"+
        " return NULL" ) ;
    return  nullptr ;
  }
  return antiPP ;
}
// ============================================================================
/*  get Particle ID for antiparticle
 *  @param  pid particle
 *  @return pid fro antiparticle
 */
// ============================================================================
LHCb::ParticleID LoKi::Particles::antiParticle( const LHCb::ParticleID& pid  )
{
  // check the validity of own pid
  const LHCb::ParticleProperty* p1 = LoKi::Particles::_ppFromPID( pid ) ;
  //
  if ( !p1 )
  {
    LoKi::Report::Error
      ( " LoKi::Particles::antiParticle("              +
        boost::lexical_cast<std::string> ( pid )  +
        "): LHCb::ParticleProperty* points to NULL "         +
        " return LHCb::ParticleID()  "                 ) ;
    return LHCb::ParticleID()  ;
  }
  // get the anti particle
  const LHCb::ParticleProperty* antiPP = p1->antiParticle() ;
  if ( !antiPP )
  {
    LoKi::Report::Error
      ( " LoKi::Particles::antiParticle("              +
        boost::lexical_cast<std::string> ( pid )  +
        "): LHCb::ParticleProperty* points to NULL "         +
        " return LHCb::ParticleID()  "                 ) ;
    return LHCb::ParticleID()  ;
  }
  //
  return LHCb::ParticleID( antiPP -> pdgID () ) ;
}
// ============================================================================
/*  retrieve particle name for given PID
 *  @param pid particle PID
 *  @return particle name
 */
// ============================================================================
std::string  LoKi::Particles::nameIdFromPID ( const LHCb::ParticleID& pid )
{
  const LHCb::ParticleProperty* pp = __ppFromPID ( pid ) ;
  if ( pp ) { return pp->particle() ; }
  return "PID(" + boost::lexical_cast<std::string>( pid.pid() ) + ")" ;
}
// ============================================================================
/*  retrieve particle name for given PID
 *  @param pid particle PID
 *  @return particle name
 */
// ============================================================================
std::string  LoKi::Particles::nameFromPID ( const LHCb::ParticleID& pid )
{
  typedef boost::container::flat_map<LHCb::ParticleID,std::string> Map ;
  // ATTENTION
  static Synced<Map> s_map{ std::make_pair( LHCb::ParticleID (    22 ) , "gamma" ),
                            std::make_pair( LHCb::ParticleID (   -11 ) , "e+"    ),
                            std::make_pair( LHCb::ParticleID (    11 ) , "e-"    ),
                            std::make_pair( LHCb::ParticleID (   -13 ) , "mu+"   ),
                            std::make_pair( LHCb::ParticleID (    13 ) , "mu-"   ),
                            std::make_pair( LHCb::ParticleID (   211 ) , "pi+"   ),
                            std::make_pair( LHCb::ParticleID (   111 ) , "pi0"   ),
                            std::make_pair( LHCb::ParticleID (  -211 ) , "pi-"   ),
                            std::make_pair( LHCb::ParticleID (   321 ) , "K+"    ),
                            std::make_pair( LHCb::ParticleID (  -321 ) , "K-"    ),
                            std::make_pair( LHCb::ParticleID (  2212 ) , "p+"    ),
                            std::make_pair( LHCb::ParticleID ( -2212 ) , "p~-"   ) };

  auto res = s_map.with_rlock( [&]( const Map& m )
                               -> boost::optional<std::string> {
      auto i = m.find( pid );
      if ( UNLIKELY( i == m.end() ) ) return {};
      return i->second ;
  });
  if (res) return *res;
  //
  const LHCb::ParticleProperty* pp = LoKi::Particles::_ppFromPID( pid ) ;
  if ( !pp && 0 == pid.abspid() )
  {
    LoKi::Report::Error
      ( " LoKi::Particles::nameFromPID("               +
        boost::lexical_cast<std::string>( pid )  +
        "): LHCb::ParticleProperty* points to NULL "         +
        " return '" + s_InvalidPIDName + "'"           ) ;
    return s_InvalidPIDName ;
  }
  else if ( !pp )
  {
    std::string invalid_name = Gaudi::Utils::toString ( pid.pid() ) ;
    LoKi::Report::Error
      ( " LoKi::Particles::nameFromPID("             +
        boost::lexical_cast<std::string>( pid )      +
        "): LHCb::ParticleProperty* points to NULL " +
        " return 'Unknown(" + invalid_name + ")'"  ) ;
    return "Unknown(" + invalid_name + ")";
  }
  // update the map
  {
  s_map.with_wlock( [&](Map& m) { m.try_emplace( pid, pp->particle() ); } );
  return pp->particle() ;
  }
}
// ============================================================================
/*  retrieve the lifetime for the particle form the name
 *  @param name particle name
 */
// ============================================================================
double LoKi::Particles::ctau
( const std::string&      name )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::_ppFromName ( name ) ;
  if ( UNLIKELY(!pp) )
  {
    LoKi::Report::Error
      ( "LoKi::Particles::lifeTime('" +
        name +
        "'): ParticleProperty is NULL" );
    return LoKi::Constants::InvalidTime ;
  }
  //
  return pp -> ctau () ;
}
// ============================================================================
/*  retrieve the lifetime for the particle form the name
 *  @param name particle name
 */
// ============================================================================
double LoKi::Particles::ctau
( const LHCb::ParticleID& pid )
{
  const LHCb::ParticleProperty* pp = LoKi::Particles::_ppFromPID ( pid ) ;
  if ( UNLIKELY(!pp) )
  {
    LoKi::Report::Error
      ( "LoKi::Particles::lifeTime('" +
        boost::lexical_cast<std::string>( pid )  +
        "'): LHCb::ParticleProperty* is NULL");
    return LoKi::Constants::InvalidTime ;
  }
  //
  return pp -> ctau () ;
}
// ============================================================================
std::vector<LHCb::ParticleID>
std::abs ( const std::vector<LHCb::ParticleID>& pids )
{
  std::vector<LHCb::ParticleID> res; res.reserve ( pids.size() ) ;
  std::transform ( pids.begin(), pids.end(), std::back_inserter(res),
                   []( const LHCb::ParticleID& i ) -> LHCb::ParticleID
                   { return std::abs( i ) ; } );
  return res ;
}


// ============================================================================
// The END
// ============================================================================
