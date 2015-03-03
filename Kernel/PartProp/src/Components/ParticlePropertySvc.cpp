// $Id: ParticlePropertySvc.cpp,v 1.6 2009-05-06 15:45:52 ibelyaev Exp $
// ============================================================================
//Include files
// ============================================================================
// STD&STL 
// ============================================================================
#include <set>
#include <cstdlib>
#include <fstream>
#include <functional>
#include <algorithm>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/SvcFactory.h"
#include "GaudiKernel/Service.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/ParticleProperty.h"
#include "GaudiKernel/PhysicalConstants.h"
#include "GaudiKernel/IFileAccess.h"
#include "GaudiKernel/VectorMap.h"
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/ParticleProperty.h"
#include "Kernel/ParticleID.h"
#include "Kernel/CC.h"
// ============================================================================
// Boost 
// ============================================================================
#include "boost/lambda/lambda.hpp"
#include "boost/lambda/bind.hpp"
#include "boost/algorithm/string.hpp"
// ============================================================================
// local 
// ============================================================================
/** @file
 *  Simple implementation of class LHCb::ParticlePropertySvc 
 *  @author      : I. Last
 *  - Modified by  G.Corti 09/11/1999 to read file from LHCBDBASE and
 *                 introduce find/eraseByStdHepID methods
 *  - Modifies by Vanya BELYAEV to allow readig of additional files to 
 *                add/replace/modify the existing particle properties 
 */
// ============================================================================
namespace LHCb
{
  // ==========================================================================
  /** @class ParticlePropertySvc ParticlePropertySvc.cpp
   *
   *  This service provides access to particle properties.
   *  The settable property of this service is the 
   *  ParticlePropertiesFile, an ASCII file containing the 
   *  list of properties for the particles.
   *  The default file is: 
   *      "$PARAMFILESROOT/data/ParticleTable.txt"
   *      or if $PARAMFILESROOT is not defined 
   *      "ParticleTable.txt"
   * 
   *  @author Iain Last
   *  @author Gloria Corti 
   *  @author Vanya BELYAEV
   *  @date 2006-09-22 
   *
   *  1) Modified to add possibility to redefine properties of existing 
   *  particles and to read addiitonal files.
   *  New propeety "OtherFiles" (default is empty vector) is introduces.
   *  Service parsed additional files after the main one.
   * 
   *    @code 
   *
   *    // read the additional files with particle properties 
   *    ParticlePropertySvc.OtherFiles += {
   *       "$SOMELOCATION1/file1.txt" , 
   *       "$SOMELOCATION2/file2.txt"  
   *    } ;
   *
   *    @endcode 
   *
   *  2) Add possibility to modify only certain partiles through the 
   *  new property "Particles"  (default is enpty list), 
   *  Each line is interpreted as a line in particle data table, e.g.
   *
   *   @code
   *    
   *   // redefine the properties of H_20 and H_30 particles:
   *   ParticlePropertySvc.Particles = {
   *       "H_20 88 35 0.0 120.0 9.4e-26 Higgs'0 35 0.0e+00" ,
   *       "H_30 89 36 0.0  40.0 1.0e-12      A0 36 0.0e+00"
   *     } ;
   *
   *   @endcode 
   *
   *  The replaces/modified particles are reported.
   */
  class ParticlePropertySvc 
    :         public Service 
    , virtual public LHCb::IParticlePropertySvc 
  {
    // ========================================================================
    /// the friend factory for instantiations 
    friend class SvcFactory<LHCb::ParticlePropertySvc> ;
    // ========================================================================
  public: // LHCb::IParticlePropertySvc
    // =========================================================================
    /** get the begin-iterator for the container of particle properties
     *  It is assumed that the container is properly ordered
     *  @return begin-iterator for the container of particle properties 
     */  
    virtual iterator begin () const { return m_vector.begin () ; }
    /** get the end-iterator for the container of particle properties
     *  It is assumed that the container is properly ordered
     *  @return end-iterator for the container of particle properties 
     */  
    virtual iterator end   () const { return m_vector.end   () ; }
    /// get the container size.
    virtual size_t   size  () const { return m_vector.size  () ; }
    /** Retrieve an object by name:
     *
     *  @code 
     *  
     *   LHCb::IParticlePropertySvc* svc = ... ;
     *
     *   const std::string& name = ... ;
     * 
     *   const LHCb::ParticleProperty*  pp = svc -> find ( name ) ;
     *
     *  @endcode 
     *  @param name the particle name
     *  @return pointer to particle property object
     */
    virtual const LHCb::ParticleProperty* find 
    ( const std::string&      name ) const { return m_nameMap ( name ) ; }  
    /** Retrieve an object by PID:
     *
     *  @code 
     *  
     *   LHCb::IParticlePropertySvc* svc = ... ;
     *
     *   const LHCb::ParticleID& pid = ... ;
     *
     *   const LHCb::ParticleProperty*  pp = svc -> find ( pid ) ;
     *
     *  @endcode 
     *  @param name the particle name
     *  @return pointer to particle property object
     */
    virtual const LHCb::ParticleProperty* find 
    ( const LHCb::ParticleID& pid  ) const { return m_pidMap ( pid ) ; }  
    // =========================================================================
  public:  // CC-ing
    // =========================================================================
    /** make the charge conjugation for the string/decay descriptor 
     * 
     *  @code 
     * 
     *   std::string decay = "B0 -> pi+ pi-" ;
     *  
     *   LHCb::IParticleProeprtySvc* svc = ... ;
     *  
     *   std::string cc = svc -> cc ( decay ) ;
     *
     *  @endcode 
     *
     *  @param decay the decay descriptor
     *  @return the charge conjugation for the decay descriptor
     */  
    virtual std::string cc ( const std::string& decay ) const ;
     // =========================================================================
  public:  // (I)Service
    // =========================================================================
    /// Initialize the service.
    virtual StatusCode initialize () ;  
    /// Finalise the service.
    virtual StatusCode finalize   () ;
    /// Query the interfaces.
    virtual StatusCode queryInterface 
    ( const InterfaceID& riid, void** ppvInterface );
    /// Service type.
    virtual const InterfaceID& type() const 
    { return LHCb::IParticlePropertySvc::interfaceID() ; }
    // ========================================================================
  private :  // technical methods 
    // ========================================================================
    /** Standard Constructor.
     *  @param  name   service instance name 
     *  @param  pSvc   pointer to service locator `
     */
    ParticlePropertySvc
    ( const std::string& name ,   // the service instance name 
      ISvcLocator*       pSvc )   // the Service Locator 
      : Service ( name , pSvc ) 
        /// the base storage of all particle properties 
      , m_set()                    // the base storage of all particle properties 
        /// the ordered conatiner of particle proeprties 
      , m_vector  ()               // the ordered container of particle properties 
        /// Map { "name" : "property"} 
      , m_nameMap ()                                 // Map { "name" : "proeprty"} 
        /// Map { "pid" : "property"} 
      , m_pidMap  ()                                 // Map { "pid"  : "proeprty"}
        /// dump the properties ?
      , m_dump ( false )                             // dump the properties ?
        /// the 'main' file with aprticle data 
      , m_filename ( "./ParticleTable.txt" ) // the 'main' file with aprticle data 
        /// additional files with particle data 
      , m_other ()                          // additional files with particle data 
        /// thelist of special particle properties 
      , m_particles ()                 /// the list of special particle properties 
        //
      //
      , m_ccMap        ()
      , m_ccmap_       ()
      //
    {
      // Redefine the default name: 
      if( NULL != getenv("PARAMFILESROOT") ) 
      {
        m_filename  = getenv( "PARAMFILESROOT" ) ;
        m_filename += "/data/ParticleTable.txt"  ;
      }
      //
      declareProperty 
        ( "ParticlePropertiesFile" , 
          m_filename               , 
          "The name of 'main' particle properties file" ) 
        -> declareUpdateHandler (&LHCb::ParticlePropertySvc::updateHandler , this ) ;
      declareProperty 
        ( "OtherFiles"  , 
          m_other       , 
          "The (optional) list of additional files with the particle data" ) 
        -> declareUpdateHandler (&LHCb::ParticlePropertySvc::updateHandler , this ) ;
      declareProperty 
        ( "Particles"  , 
          m_particles  , 
          "The (optional) list of special particle properties" ) 
        -> declareUpdateHandler (&LHCb::ParticlePropertySvc::updateHandler , this ) ;
      declareProperty 
        ( "Dump" , m_dump , 
          "Dump all properties in a table format" ) 
        -> declareUpdateHandler (&LHCb::ParticlePropertySvc::updateDump   , this ) ;
      //
      // CC-related part 
      //
      m_ccmap_ [ "X+"         ] = "X-"         ; // charged, positive
      m_ccmap_ [ "X-"         ] = "X+"         ; // charged, negative 
      m_ccmap_ [ "l+"         ] = "l-"         ; // charged lepton, positive 
      m_ccmap_ [ "l-"         ] = "l+"         ; // charged lepton, negative
      // protect some other symbols from inproper conversions
      m_ccmap_ [ "Meson"      ] = "Meson"      ; // the generic meson 
      m_ccmap_ [ "Hadron"     ] = "Hadron"     ; // the generic hadron
      m_ccmap_ [ "Baryon"     ] = "Baryon"     ; // the generic baryon
      m_ccmap_ [ "Lepton"     ] = "Lepton"     ; // the generic lepton
      m_ccmap_ [ "Nu"         ] = "Nu"         ; // the generic neutrino
      m_ccmap_ [ "Nucleus"    ] = "Nucleus"    ; // the generic nucleus 
      m_ccmap_ [ "Scalar"     ] = "Scalar"     ; // the generic scalar 
      m_ccmap_ [ "Vector"     ] = "Vector"     ; // the generic vector
      m_ccmap_ [ "Tensor"     ] = "Tensor"     ; // the generic tensor 
      m_ccmap_ [ "OneHalf"    ] = "OneHalf"    ; // the generic spinor 1/2 
      m_ccmap_ [ "ThreeHalf"  ] = "ThreeHalf"  ; // the generic spinor 3/2 
      m_ccmap_ [ "FiveHalf"   ] = "FiveHalf"   ; // the generic spinor 5/2 
      m_ccmap_ [ "Spinor"     ] = "Spinor"     ; // the generic spinor 1/2
      m_ccmap_ [ "Nucleus"    ] = "Nucleus"    ; // the nucleus 
      // protection:
      m_ccmap_ [ "HasQuark"   ] = "HasQuark"   ; // for protection 
      m_ccmap_ [ "JSpin"      ] = "JSpin"      ; // for protection
      m_ccmap_ [ "SSpin"      ] = "SSpin"      ; // for protection
      m_ccmap_ [ "LSpin"      ] = "LSpin"      ; // for protection
      m_ccmap_ [ "cc"         ] = "cc"         ; // for protection
      m_ccmap_ [ "CC"         ] = "CC"         ; // for protection
      m_ccmap_ [ "os"         ] = "os"         ; // for protection
      m_ccmap_ [ "Xd"         ] = "Xd"         ; // for protection
      m_ccmap_ [ "Xu"         ] = "Xu"         ; // for protection
      m_ccmap_ [ "Xs"         ] = "Xs"         ; // for protection
      m_ccmap_ [ "Xc"         ] = "Xc"         ; // for protection
      m_ccmap_ [ "Xb"         ] = "Xb"         ; // for protection
      m_ccmap_ [ "Xt"         ] = "Xt"         ; // for protection
      m_ccmap_ [ "Up"         ] = "Up"         ; // for protection
      m_ccmap_ [ "Down"       ] = "Down"       ; // for protection
      m_ccmap_ [ "Strange"    ] = "Strange"    ; // for protection
      m_ccmap_ [ "Charm"      ] = "Charm"      ; // for protection
      m_ccmap_ [ "Bottom"     ] = "Bottom"     ; // for protection
      m_ccmap_ [ "Beauty"     ] = "Beauty"     ; // for protection
      m_ccmap_ [ "Top"        ] = "Top"        ; // for protection
      //
      declareProperty 
        ( "ChargeConjugations" , m_ccmap_ , 
          "The map of charge-conjugation symbols" ) 
        -> declareUpdateHandler (&LHCb::ParticlePropertySvc::updateCC   , this ) ;
    }
    /// virtual & protected destructor 
    virtual ~ParticlePropertySvc() // virtual & protected destructor 
    {
      // clear all storages:
      m_vector  . clear () ;
      m_nameMap . clear () ;
      m_pidMap  . clear () ;
      // delete all properties 
      for ( Set::iterator i = m_set.begin() ; m_set.end()  != i ; ++i ) 
      { LHCb::ParticleProperty* pp = *i ; delete pp ; }               // DELETE 
      m_set.clear() ;
    }
    // ========================================================================
  private: // update handler 
    // ========================================================================  
    /** the action  in the case of interactive manipulation with properties:
     *   - no action if the internal data is not yet build
     *   - else rebuild the internal data 
     *  Such action will allow more flexible interactive configuration 
     *  of the service 
     *  @param p the updated property
     */
    void updateHandler ( Property& p ) ;
    // ========================================================================  
    /** the action  in the case of interactive manipulation with properties:
     *  of the service 
     *  @param p the updated property
     */
    void updateDump    ( Property& p ) ;
    /** the action  in the case of interactive manipulation with properties:
     *  of the service 
     *  @param p the updated property
     */
    void updateCC      ( Property& p ) ;
    /// dump the table of particle properties
    void dump () ;
    // ========================================================================
    /** rebuild the Particle Property Data
     *   - clear existing conitainers 
     *   - parse main file 
     *   - parse additional files 
     *   - parse the specific options
     *   - set particle<->antiparticle links 
     *   @return status code 
     */
    StatusCode rebuild () ;
    // ========================================================================
    /** parse the file 
     *  @param file the fiel name to be parsed 
     *  @return status code
     */
    StatusCode parse ( const std::string& file ) ;
    /** parse the line 
     *  @param line the line to be parsed 
     *  @return status code 
     */
    StatusCode parseLine ( const std::string& line ) ;
    // ========================================================================
    /** add new particle (or rdefine the existing one)
     *  @param pname  the particle  name 
     *  @param pid    the particle ID
     *  @param charge the particle charge
     *  @param mass   the particle mass 
     *  @param tlife  the particle tlife 
     *  @param maxWidth the particle max-width
     *  @param evtgen the name for EvtGen 
     *  @param pythia the ID for pythia generator 
     */
    StatusCode addParticle
    ( const std::string&      pname    , 
      const LHCb::ParticleID& pid      , 
      const double            charge   ,
      const double            mass     , 
      const double            tlife    ,
      const double            maxWidth ,
      const std::string&      evtgen   ,
      const int               pythia   ) ;
    // ========================================================================
    /** set properly particle<-->antiparticle relations 
     *  @return status code 
     */
    StatusCode setAntiParticles () ;
    // ========================================================================
    /// check the difference of two properties and fill corresponding sets 
    bool diff ( const LHCb::ParticleProperty& n , 
                const LHCb::ParticleProperty& o ) ;
    // ========================================================================
  private: // disabled creators
    // ========================================================================
    /// the default constructor is disabled 
    ParticlePropertySvc () ;             // the default constructor is disabled 
    /// the copy constructor is disabled 
    ParticlePropertySvc ( const ParticlePropertySvc& ) ; // no copy 
    /// the assignement operator is disabled 
    ParticlePropertySvc& operator=( const ParticlePropertySvc& ) ; // no = 
    // ========================================================================
  private: // "vizible" particle data 
    // ========================================================================
    /// the actual storage of all properties 
    typedef std::set<LHCb::ParticleProperty*>                             Set ;
    /// the actual type of map: { "name" : "property" }  
    typedef GaudiUtils::VectorMap<std::string,     const LHCb::ParticleProperty*> NameMap ;
    /// the actual type of map: { "pid"  : "property" } 
    typedef GaudiUtils::VectorMap<LHCb::ParticleID,const LHCb::ParticleProperty*> PidMap  ;
    typedef LHCb::IParticlePropertySvc::ParticleProperties Vector ; 
    /// the actual storage of all particle properties
    Set m_set ;                // the actual storage of all particle properties
    /// "vizible" data (the ordered container)
    Vector m_vector ;                  // "vizible" data (the ordered container)
    /// Map:   { "name" : "property" } 
    NameMap m_nameMap ;                         // Map:   { "name" : "property"} 
    /// Map:   { "pid"  : "property" } 
    PidMap  m_pidMap  ;                         // Map:   { "pid"  : "property"}  
    /// dump the table? 
    bool m_dump ;                               // dump the table? 
    // ========================================================================
  private: // service data 
    // ========================================================================
    /// the actual type for the list of files 
    typedef std::vector<std::string> Files ;                   // list of files 
    /// the actual type for the list of particle properties (strings)
    typedef std::vector<std::string> Particles ;
    /// the main file with particle properties
    std::string m_filename;           // the main file with particle properties
    /// additional files 
    Files m_other ;                                    // additional file names 
    /// properties to be redefined  explicitely 
    Particles m_particles ;          // properties to be redefined  explicitely 
    // ========================================================================
  private: // CC-related stuff 
    // ========================================================================
    /// the CC-map 
    mutable Decays::CC::MapCC         m_ccMap  ;            //       the CC-map 
    /// CC-map for properties 
    std::map<std::string,std::string> m_ccmap_ ;            //           CC-map
    // ========================================================================
  private: // various statistics of modifications 
    // ========================================================================
    typedef std::set<std::string>       NameSet ;
    typedef std::set<LHCb::ParticleID>  PidSet  ;    
    // ========================================================================
    NameSet m_by_charge ;
    NameSet m_by_mass   ;
    NameSet m_by_tlife  ;
    NameSet m_by_width  ;
    NameSet m_by_evtgen ;
    NameSet m_by_pythia ;
    // ========================================================================
    NameSet m_replaced_names ;
    PidSet  m_replaced_pids  ;
    NameSet m_no_anti        ;
    // ========================================================================
  };
  // ==========================================================================
} // end of namespace LHCb
// ============================================================================
// initialize the service 
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::initialize () 
{ 
  // 1) initialize the base 
  StatusCode sc = Service::initialize();
  if ( sc.isFailure() ) { return sc ; }
  // 2) create the log 
  MsgStream log( msgSvc() , name() ) ;
  // 3) rebuild everything 
  sc = rebuild () ;
  if ( sc.isFailure () ) 
  {
    log << MSG::ERROR << " Unable to initialize the internal structures " << endmsg ;
    return sc ;
  }
  //
  m_by_charge      . clear () ;
  m_by_mass        . clear () ;
  m_by_tlife       . clear () ;
  m_by_width       . clear () ;
  m_by_evtgen      . clear () ;
  m_by_pythia      . clear () ;
  m_replaced_names . clear () ;
  m_replaced_pids  . clear () ;
  m_no_anti        . clear () ;
  //
  m_ccMap.clear () ;
  //
  if ( m_dump || MSG::DEBUG >= outputLevel () ) { dump () ; }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
/* rebuild the Particle Property Data
 *   - clear existing conitainers 
 *   - parse main file 
 *   - parse additional files 
 *   - parse the specific options
 *   - set particle<->antiparticle links 
 *   @return status code 
 */
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::rebuild ()
{
  // clear all existing containers 
  m_nameMap   . clear () ;
  m_pidMap    . clear () ;
  m_vector    . clear () ;
  m_ccMap     . clear () ;
  // ==========================================================================
  // parse the main file 
  StatusCode sc = parse ( m_filename ) ;
  if ( sc.isFailure() ) { return sc ; }                              // RETURN
  // parse the additional files 
  for ( Files::const_iterator ifile = m_other.begin () ; 
        m_other.end() != ifile ; ++ifile )
  {
    sc = parse ( *ifile ) ;
    if ( sc.isFailure() ) { return sc ; }                            // RETURN 
  }
  // parse the options/lines
  for ( Particles::const_iterator iline = m_particles.begin() ; 
        m_particles.end() != iline ; ++iline )
  {
    sc = parseLine ( *iline ) ;
    if ( sc.isFailure() ) { return sc ; }                            // RETURN 
  }
  // sort the vector 
  std::stable_sort 
    ( m_vector.begin() , m_vector.end  () , 
      LHCb::ParticleProperty::Compare () ) ;
  // set particle<-->antiparticle links 
  sc = setAntiParticles () ;
  if ( sc.isFailure() ) { return sc ; }                             // RETURN 
  /// some debug printout
  MsgStream log ( msgSvc() , name() ) ;
  log << MSG::DEBUG 
      << " All:   "   << m_vector   .size () 
      << " By Name: " << m_nameMap  .size () 
      << " By PID: "  << m_pidMap   .size () 
      << " Total: "   << m_set      .size () 
      << endmsg ;
  //
  if ( !m_by_charge.empty()  ) 
  {
    log << MSG::INFO
        << " Charge   has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_charge ) << endmsg ;
  }
  if ( !m_by_mass.empty()  ) 
  {
    log << MSG::INFO
        << " Mass     has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_mass ) << endmsg ;
  }
  if ( !m_by_tlife.empty()  ) 
  {
    log << MSG::INFO
        << " Lifetime has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_tlife ) << endmsg ;
  }
  if ( !m_by_width.empty()  ) 
  {
    log << MSG::INFO
        << " MaxWidth has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_width ) << endmsg ;
  }
  if ( !m_by_evtgen.empty()  ) 
  {
    log << MSG::INFO
        << " EvtGenID has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_evtgen ) << endmsg ;
  }
  if ( !m_by_pythia.empty()  ) 
  {
    log << MSG::INFO
        << " PythiaID has beed redefined for " 
        << Gaudi::Utils::toString ( m_by_pythia ) << endmsg ;
  }    
  if ( !m_replaced_names.empty()  ) 
  {
    log << MSG::INFO
        << " Replaced names : " 
        << Gaudi::Utils::toString ( m_replaced_names ) << endmsg ;
  }
  if ( !m_replaced_pids.empty()  ) 
  {
    log << MSG::INFO
        << " Replaced PIDs  : " 
        << Gaudi::Utils::toString ( m_replaced_pids ) << endmsg ;
  }
  if ( !m_no_anti.empty()  ) 
  {
    log << MSG::INFO
        << " No anti particle : " 
        << Gaudi::Utils::toString ( m_no_anti ) << endmsg ;
  }
  //
  return StatusCode::SUCCESS ;
}
// =============================================================================
// finalize
// =============================================================================
StatusCode LHCb::ParticlePropertySvc::finalize() 
{ 
  /// finalize the base class 
  return Service::finalize () ; 
}
// ============================================================================
/*  the action  in the case of interactive manipulation with properties:
 *   - no action if the internal data is not yet build
 *   - else rebuild the internal data 
 *  Such action will allow more flexible interactive configuration 
 *  of the service 
 */
// ============================================================================
void LHCb::ParticlePropertySvc::updateHandler ( Property& p  ) 
{
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ) { return ; }      // RETURN 
  //
  MsgStream log ( msgSvc() , name() ) ;
  log << MSG::INFO 
      << "Property triggers the update of internal Particle Property Data : "
      << p << endmsg ;
  // rebuild the internal data 
  StatusCode sc = rebuild () ;
  if ( sc.isFailure() )
  { throw GaudiException 
      ( "Can't rebuild Particle Properties Data" , 
        "*ParticlePropertySvc*" , sc ) ; } 
  // clear CC-map 
  m_ccMap.clear () ;  
}
// ============================================================================
//  the action  in the case of redefiniiton of "ChargeConjugates"
// ============================================================================
void LHCb::ParticlePropertySvc::updateCC ( Property& /* p */ ) 
{ m_ccMap.clear () ; }
// ============================================================================
/* the action  in the case of interactive manipulation with properties:
 *  of the service 
 *  @param p the updated property
 */
// ============================================================================
void LHCb::ParticlePropertySvc::updateDump ( Property& /* p */ ) 
{
  if ( FSMState() < Gaudi::StateMachine::INITIALIZED ) { return ; }      // RETURN 
  // dump the table 
  dump () ;
}
// =============================================================================
// queryInterface.
// =============================================================================
StatusCode LHCb::ParticlePropertySvc::queryInterface
( const InterfaceID& riid, 
  void** ppvInterface ) 
{
  // check the validity of the placeholder for result 
  if ( !ppvInterface ) { return StatusCode::FAILURE ; }              // RETURN
  *ppvInterface = 0;
  if ( IParticlePropertySvc::interfaceID() == riid  ) 
  {
    *ppvInterface = static_cast<IParticlePropertySvc*>(this);
    addRef();                    // IMPORTANT: increment the reference counter 
    return StatusCode::SUCCESS ;                                     // RETURN
  }
  return  Service::queryInterface( riid, ppvInterface ) ;            // RETURN 
} 
// ============================================================================
// parse the file 
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::parse( const std::string& file ) 
{  
  MsgStream log ( msgSvc() , name() ) ;
  //
  IFileAccess* fileAccess = 0 ;
  StatusCode sc = service ( "VFSSvc" , fileAccess ) ;
  if ( sc.isFailure() )
  {
    log << MSG::ERROR << "Unable to locate IFileAccess('VFSSvc') service" << endmsg ;
    return sc ;                                                     // RETURN 
  }
  // check once more: 
  if ( 0 == fileAccess ) 
  {
    log << MSG::ERROR << "IFileAccess* points to NULL" << endmsg ;
    return StatusCode::FAILURE ;                                      // RETURN 
  }
  // "open" the file
  std::auto_ptr<std::istream> infile = fileAccess->open ( file ) ;
  if (  0 == infile.get()  )
  {
    log << MSG::ERROR << "Unable to open file '" << file << "'" << endmsg ;
    return StatusCode::FAILURE ;                                      // RETURN 
  }
  bool active = false ;
  // read the file line-by-line 
  while ( *infile ) 
  {
    std::string line ;
    std::getline ( *infile , line ) ;
    // comment lines start with '#'
    if ( line[0] == '#' ) { continue ; }                            // CONTINUE
    // skip empty lines:
    if ( line.empty ()  ) { continue ; }                            // CONTINUE
    //
    if ( !active ) 
    {
      if ( "PARTICLE" == boost::to_upper_copy ( boost::trim_copy ( line ) ) )
      {
        active = true ; // ACTIVATE!
        continue      ;
      } 
    }
    else
    {
      if ( "END PARTICLE" == boost::to_upper_copy ( boost::trim_copy ( line ) ) )
      { 
        active = false ;  // DEACTIVATE!
        continue ;
      } 
    }
    //
    if ( !active ) { continue ; } // skip the lines if not active
    // parse the line
    StatusCode sc = parseLine ( line ) ;
    if ( sc.isFailure() ) 
    {
      log << MSG::ERROR << "Unable to parse the file '" << file << "'" << endmsg ;
      return sc ;                                                     // RETURN 
    } 
  }
  // release the service
  fileAccess -> release () ; 
  //
  return StatusCode::SUCCESS ;                                        // RETURN 
}
// ============================================================================
/*  parse the line 
 *  the format of the line is defined by old SICB CDF 
 *  @param line the line to be parsed 
 *  @return status code 
 */
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::parseLine ( const std::string& line ) 
{
  MsgStream log ( msgSvc() , name() ) ;
  // get the input stream  from the line :
  std::istringstream input ( line ) ;
  // get the name
  std::string p_name   ;
  int         p_geant  ; // obsolete, to be ignored  
  int         p_pdg    ;  
  double      p_charge ;
  double      p_mass   ;
  double      p_ltime  ;
  std::string p_evtgen ;
  int         p_pythia ;
  double      p_maxwid ;
  // parse the line 
  if ( input 
       >> p_name   >> p_geant  >> p_pdg 
       >> p_charge >> p_mass   >> p_ltime 
       >> p_evtgen >> p_pythia >> p_maxwid )
  {
    StatusCode sc = addParticle
      ( p_name                        , 
        LHCb::ParticleID ( p_pdg )    ,
        p_charge                      , 
        p_mass    * Gaudi::Units::GeV ,   // rescale from CDF units
        p_ltime   * Gaudi::Units::s   ,   // rescale from CDF units
        p_maxwid  * Gaudi::Units::GeV ,   // rescale from CDF units 
        p_evtgen                      ,
        p_pythia                      ) ;
    if ( sc.isFailure() ) { return sc ; }                        // RETURN 
  }
  else
  {
    MsgStream log ( msgSvc() , name() ) ;
    log << MSG::ERROR
        << " could not parse the line: '" << line << "'" << endmsg ;
    return StatusCode::FAILURE ;                                 // RETURN 
  }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// Add the particle 
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::addParticle
( const std::string&      pname    , 
  const LHCb::ParticleID& pid      , 
  const double            charge   ,
  const double            mass     , 
  const double            tlife    ,
  const double            maxWidth ,
  const std::string&      evtgen   ,
  const int               pythia   ) 
{
  // create the local object 
  const LHCb::ParticleProperty pp 
    ( pname , pid , charge , mass , tlife , maxWidth , evtgen , pythia   ) ;
  //
  // 1) find the object with smae name & pid in set:
  Set::iterator it = std::find_if 
    ( m_set.begin() , m_set.end() , 
      ( boost::lambda::bind ( &LHCb::ParticleProperty::name , boost::lambda::_1 ) == pp.name () ) 
      &&
      ( boost::lambda::bind ( &LHCb::ParticleProperty::pid  , boost::lambda::_1 ) == pp.pid  () )
      );
  // 2) object is found, redefine it!
  LHCb::ParticleProperty* newp = 0 ;
  if ( m_set.end() != it ) { newp = *it ; }
  // new property ? 
  if ( 0 == newp ) 
  {
    newp = new LHCb::ParticleProperty( pp ) ;
    it = m_set.insert ( newp ).first  ;
  }
  else if ( diff ( *newp , pp ) ) { *newp = pp ; } // NB: redefine the properties
  //
  // insert into name map
  {
    NameMap::const_iterator i1 = m_nameMap.find ( newp -> name () ) ;
    if ( m_nameMap.end() != i1 && i1->second != newp ) 
    { m_replaced_names.insert ( newp->name() ) ; } 
    m_nameMap.update ( newp -> name () , newp ) ; 
  }
  // insert into PID map
  {
    PidMap::const_iterator i2 = m_pidMap.find ( newp -> pid () ) ;
    if ( m_pidMap.end() != i2 && i2->second != newp ) 
    { m_replaced_pids.insert ( newp -> pid() ) ; }
    m_pidMap.update ( newp -> pid () , newp ) ; 
  }
  // insert into vector
  if ( m_vector.end() == std::find ( m_vector.begin() , m_vector.end() , newp ) )
  { m_vector.push_back ( newp ) ; }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
// Set the antiparticle 
// ============================================================================
StatusCode LHCb::ParticlePropertySvc::setAntiParticles () 
{
  MsgStream log ( msgSvc() , name() ) ;
  //
  for ( Vector::iterator i = m_vector.begin() ; m_vector.end() != i ; ++i ) 
  {
    const LHCb::ParticleProperty* _pp = *i ;
    LHCb::ParticleProperty* pp = const_cast<LHCb::ParticleProperty*> ( _pp );
    pp -> setAntiParticle ( 0 ) ;
    // get the ID for antiParticle
    LHCb::ParticleID panti ( -pp->particleID().pid() ) ;
    // 
    const LHCb::ParticleProperty* anti = m_pidMap ( panti ) ;
    //
    if ( 0 == anti 
         && 0 < pp->particleID().pid() 
         && !pp->particleID().isNucleus() ) { anti = _pp ; }
    //
    pp -> setAntiParticle ( anti ) ;
    if ( 0 != pp && 0 != pp->antiParticle() ) 
    {
      log << MSG::VERBOSE
          << "Antiparticle for \n" <<  (* pp)
          << " is set to be    \n" <<  (*(pp->antiParticle())) << endmsg ;
    }
    if ( (0 == pp->antiParticle()) && (0 == pp->pid().extraBits()) ) { m_no_anti.insert ( pp->name() ) ; }
  }
  return StatusCode::SUCCESS ;
}
// ============================================================================
bool LHCb::ParticlePropertySvc::diff 
( const LHCb::ParticleProperty& n , 
  const LHCb::ParticleProperty& o ) 
{
  bool d = false ;
  if ( n.charge   () != o.charge   () ) { m_by_charge.insert ( n.name () ) ; d = true ; }
  if ( n.mass     () != o.mass     () ) { m_by_mass.insert   ( n.name () ) ; d = true ; }
  if ( n.lifetime () != o.lifetime () ) { m_by_tlife.insert  ( n.name () ) ; d = true ; }
  if ( n.maxWidth () != o.maxWidth () ) { m_by_width.insert  ( n.name () ) ; d = true ; }
  if ( n.evtGen   () != o.evtGen   () ) { m_by_evtgen.insert ( n.name () ) ; d = true ; }
  if ( n.pythia   () != o.pythia   () ) { m_by_pythia.insert ( n.name () ) ; d = true ; }
  //
  if ( d ) 
  {
    MsgStream log ( msgSvc () , name () ) ;
    log << MSG::DEBUG 
        << " Change the properties of '" << n.name() << "'/" << n.pid().pid() 
        << std::endl << " New: " << n
        << std::endl << " Old: " << o 
        << endmsg ;
  }
  return d ;
}
// ============================================================================
// dump the table of particle properties
// ============================================================================ 
void LHCb::ParticlePropertySvc::dump () 
{
  MsgStream log ( msgSvc() , name() ) ;
  //
  log << MSG::ALWAYS << " The Table of Particle Properties " << std::endl ;
  LHCb::ParticleProperties::printAsTable ( m_vector , log , this ) ;
  log << endmsg ;
}
// ============================================================================
/* make the charge conjugation for the string/decay descriptor 
 * 
 *  @code 
 * 
 *   std::string decay = "B0 -> pi+ pi-" ;
 *  
 *   LHCb::IParticleProeprtySvc* svc = ... ;
 *  
 *   std::string cc = svc -> cc ( decay ) ;
 *
 *  @endcode 
 *
 *  @param decay the decay descriptor
 *  @return the charge conjugation for the decay descriptor
 */  
// ============================================================================
std::string LHCb::ParticlePropertySvc::cc ( const std::string& decay ) const 
{
  // build the map if not done yet 
  if ( m_ccMap.empty() )  
  {
    // get the particles from the service 
    for ( iterator it = m_vector.begin() ; m_vector.end() != it ; ++it ) 
    {
      const LHCb::ParticleProperty* pp = *it ;
      if ( 0 == pp   ) { continue ; }
      const LHCb::ParticleProperty* anti = pp->antiParticle() ;
      if ( 0 == anti ) { continue ; }
      m_ccMap [ pp   -> particle() ] = anti->particle() ;
    }
    // get the particles from the options 
    for ( std::map<std::string,std::string>::const_iterator ic = 
            m_ccmap_.begin() ; m_ccmap_.end() != ic ; ++ic ) 
    {
      m_ccMap [ ic -> first  ] = ic -> second ;
      m_ccMap [ ic -> second ] = ic -> first  ;
    }
    MsgStream log ( msgSvc() , name() ) ;
    log  << MSG::DEBUG ;
    if ( log.isActive() ) 
    {
      log << " CC-map is " << std::endl ;
      Gaudi::Utils::toStream ( m_ccMap , log.stream() ) ;
      log << endmsg ;
    }
  }
  // use the map 
  return Decays::CC::cc ( decay , m_ccMap ) ;
}
// ============================================================================
// Instantiation of a static factory class used by clients to create
DECLARE_NAMESPACE_SERVICE_FACTORY(LHCb,ParticlePropertySvc)
// ============================================================================
// The END 
// ============================================================================


