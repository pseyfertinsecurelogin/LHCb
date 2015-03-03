// $Id: TupleTool.cpp,v 1.3 2008-05-28 16:15:33 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToolFactory.h"
#include "GaudiKernel/VectorMap.h"
// ============================================================================
// GaudiAlg 
// ============================================================================
#include "GaudiAlg/Tuple.h"
#include "GaudiAlg/Tuples.h"
#include "GaudiAlg/TupleObj.h"
#include "GaudiAlg/GaudiTool.h"
// ============================================================================
// DaVinciKernel
// ============================================================================
#include "Kernel/IParticleTupleTool.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/PhysTypes.h"
#include "LoKi/IHybridFactory.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
namespace LoKi 
{
  namespace Hybrid 
  {
    // ========================================================================
    /** @class TupleTool
     *  The simple "hybrid"-based implementation of 
     *  the abstract interface IParticleTupleTool
     *  @see IParticleTupleTool 
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2008-03-19
     */
    class TupleTool 
      : public virtual IParticleTupleTool 
      , public                  GaudiTool
    {
      /// the friend dactory for instantiation
      friend class ToolFactory<LoKi::Hybrid::TupleTool> ;
      // ======================================================================
    public:
      // ======================================================================
      /** helper class to keep the N-tuple items
       *  it is needed due to absence f the default constructor for 
       *  the class LoKi::PhysTypes::Fun 
       *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
       *  @date 2008-03-19
       */
      class Item
      {
      public:
        // ====================================================================
        // the default constructor 
        Item () 
          : m_name () 
          , m_fun  ( LoKi::BasicFunctors<const LHCb::Particle*>::Constant ( -1.e+10 ) )
        {}
        // ====================================================================
      public:
        /// the variable name 
        std::string           m_name ; // the variable name 
        /// the functor
        LoKi::PhysTypes::Fun  m_fun  ; /// the functor
      } ;
      // ======================================================================
    public:
      // ======================================================================
      /** Fill the tuple. 
       *  @see IParticleTupelTool 
       *  @param top      the top particle of the decay.
       *  @param particle the particle about which some info are filled.
       *  @param head     prefix for the tuple column name.
       *  @param tuple    the tuple to fill
       *  @return status code
       */
      virtual StatusCode fill
      ( const LHCb::Particle* top      , 
        const LHCb::Particle* particle , 
        const std::string&    head     , 
        Tuples::Tuple&        tuple    ) ;
      // ======================================================================
    public:
      // ======================================================================
      /// initialization of the tool 
      virtual StatusCode initialize () 
      {
        StatusCode sc = GaudiTool::initialize() ;
        if ( sc.isFailure() ) {  return sc ; }               // RETURN 
        svc<IService> ( "LoKiSvc" , true ) ;
        // get the factory
        IHybridFactory* factory = tool<IHybridFactory> ( m_factory , this ) ;
        //
        m_items.clear() ;
        m_items.reserve ( m_vars.size() ) ;
        for ( Map::const_iterator ivar = m_vars.begin() ; 
              m_vars.end() != ivar ; ++ivar ) 
        {
          Item item ;
          sc = factory->get ( ivar->second , item.m_fun ) ;
          if ( sc.isFailure() )
          { return Error
              ("Unable to decode " + ivar->first + " : " + ivar->second , sc ) ; }
          //
          item.m_name = ivar->first ;
          m_items.push_back  ( item ) ; 
          //
          debug() << "The decoded variable name is '"
                  << m_items.back().m_name << "'\t, the functor : '"
                  << m_items.back().m_fun  << "'" << endreq ;
        }  
        // 
        if ( m_items.empty() ) { Warning ( "No variables/items are defined" ) ; }
        //
        release ( factory ) ; // we do not need the factory anymore 
        //
        return StatusCode::SUCCESS ;
      }
      // ======================================================================
    protected: 
      // ======================================================================
      TupleTool 
      ( const std::string& type   , 
        const std::string& name   , 
        const IInterface*  parent ) 
        : GaudiTool ( type , name , parent ) 
        , m_factory  ( "LoKi::Hybrid::Tool/HybridFactory:PUBLIC")
        , m_vars  () 
        , m_items ()
      {
        declareInterface<IParticleTupleTool> ( this ) ;
        ///
        declareProperty 
          ( "Factory" , m_factory , 
            "Type/Name for C++/Python Hybrid Factory" ) ;
        //
        declareProperty 
          ( "Variables" , m_vars , 
            "The {'name':'functor'}-map of columns for N-tuple " ) ;
      }
      // ======================================================================
      /// virtual & protected destructor
      virtual ~TupleTool() {}
      // ======================================================================
    private:
      // ======================================================================
      /// the default constructor is disabled  
      TupleTool () ;                              // no default consructor 
      /// the copy  constructor is disabled  
      TupleTool ( const TupleTool& ) ;            // no copy constructor
      /// the assigenement operator is disabled  
      TupleTool& operator=( const TupleTool& ) ;  // no assignement
      // ======================================================================     
    private:
      // ======================================================================
      /// the actual type of {"name":"functor"} map 
      typedef std::map<std::string,std::string>  Map ;
      /// the actual type of containter of items 
      typedef std::vector<Item> Items ;
      // ======================================================================
    private:
      // ======================================================================
      /// the typename of the hybrid factory 
      std::string  m_factory ; // the typename of the hybrid factory
      /// { "name":"functor"} map 
      Map          m_vars  ; // { "name":"functor"} map 
      /// n-tuple columns 
      Items        m_items ; // N-tuple columns 
      // ======================================================================
    };
    // ========================================================================
  }// end of namespace LoKi::Hybrid 
  // ==========================================================================
} // end of namespace LoKi 
// ============================================================================
/*  Fill the tuple. 
 *  @see IParticleTupelTool 
 *  @param top      the top particle of the decay.
 *  @param particle the particle about which some info are filled.
 *  @param head     prefix for the tuple column name.
 *  @param tuple    the tuple to fill
 *  @return status code
 */
// ============================================================================
StatusCode LoKi::Hybrid::TupleTool::fill
( const LHCb::Particle* top      , 
  const LHCb::Particle* particle , 
  const std::string&    head     , 
  Tuples::Tuple&        tuple    ) 
{
  if ( 0 == top       ) { Warning ( "LHCb::Particle* 'Top' points to NULL" ) ; }
  if ( 0 == particle  ) { Warning ( "LHCb::Particle*       points to NULL" ) ; }
  //
  if ( !tuple.valid() ) { return Error ( "Invalid tuple " ) ; } 
  //
  std::string head_ = head + "_" ;
  //
  for ( Items::const_iterator item = m_items.begin() ; 
        m_items.end() != item ; ++item ) 
  {
    // fill N-tuple 
    tuple -> column ( head_ + item->m_name , item->m_fun ( particle ) ) ; 
  }
  //
  return StatusCode::SUCCESS ;
}
// ============================================================================



// ============================================================================
/// Declaration of the Tool Factory
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi::Hybrid,TupleTool);
// ============================================================================


// ============================================================================
// The END 
// ============================================================================
