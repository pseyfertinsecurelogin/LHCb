// $Id: HybridParticleArrayFilter.cpp,v 1.2 2007-11-28 14:55:54 ibelyaev Exp $
// ============================================================================
// Include files 
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/ToolFactory.h" 
// ============================================================================
// GaudiAlg
// ============================================================================
#include "GaudiAlg/GaudiTool.h" 
// ============================================================================
// DaVinciKernel
// ============================================================================
#include "Kernel/IParticleArrayFilter.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/IHybridFactory.h"
#include "LoKi/select.h"
#include "LoKi/Primitives.h"
#include "LoKi/Operators.h"
// ============================================================================
namespace LoKi
{
  namespace Hybrid 
  {
    /** @class ParticleArrayFilter HybridParticleArrayFilter.cpp
     *  Simple "hybrid-based" implementation of the interface 
     *  IParticelArrayFilter 
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-07-26
     */
    class ParticleArrayFilter
      : public GaudiTool 
      , public virtual IParticleArrayFilter 
    {
      /// the friend factory for istantiation
      friend class ToolFactory<LoKi::Hybrid::ParticleArrayFilter> ;
    public:
      // ==================================================================
      /** Filter and put the results into new array
       *  @see IParticleArrayFilter
       */
      virtual StatusCode filter
      ( const LHCb::Particle::ConstVector& input    , 
        LHCb::Particle::ConstVector&       filtered ) const 
      {
        filtered.clear() ;
        // copy the particles,whcih satisfy the cut into output conterner
        LoKi::select                      ///< missing "std::copy_if" 
          ( input.begin ()              , ///< begin of the input sequence 
            input.end   ()              , ///< end of the input sequence      
            std::back_inserter ( filtered ) ,   ///< destination 
            m_cut                           ) ; ///< criteria  
        //
        return StatusCode::SUCCESS ;
      }
      // ==================================================================
      /** Filter and remove elements that do not pass filter from array
       *  @see IParticleArrayFilter
       */
      virtual StatusCode filter
      ( LHCb::Particle::ConstVector& particles ) const 
      {
        // remove all particles which do *NOT* satisfy the criteria:
        particles.erase 
          ( std::remove_if ( particles.begin () , 
                             particles.end   () , 
                             !m_cut             ) , particles.end () ) ;
        //
        return StatusCode::SUCCESS ;
      }
    public:
      // ==================================================================
      /// intialize the tool 
      virtual StatusCode initialize () ;      
      // ==================================================================
    protected:
      // ==================================================================
      /// Standard constructor
      ParticleArrayFilter  
      ( const std::string& type, 
        const std::string& name,
        const IInterface* parent)
        : GaudiTool ( type , name , parent )
        , m_cut     ( LoKi::Constant<const LHCb::Particle*,bool>( false ) ) 
        , m_code    ( "NONE" )
        , m_factory ( "LoKi::Hybrid::Tool/HybridFactory:PUBLIC") 
      {
        declareInterface<IParticleArrayFilter>(this);
        declareProperty 
          ( "Code"    , m_code    , "Python pseudocode for the filter criteria"  ) ;
        declareProperty 
          ( "Factory" , m_factory , "Type/Name for C++/Python Hybrid Factory" ) ;
      } ;
      // ==================================================================      
      /// destructor : virtual and protected
      virtual ~ParticleArrayFilter( ){}
      // ==================================================================
    private:
      // selection criteria itself 
      LoKi::Types::Cut  m_cut ; ///< selection criteria itself
      // python pseudo-code
      std::string       m_code    ; ///< python pseudo-code
      // factory type/name
      std::string       m_factory ; ///< factory type/name
    } ;
    // ==================================================================
  } // end of namespace LoKi::Hybrid
} // end of namespace LoKi 
// ============================================================================
/// Declaration of the Tool Factory
DECLARE_NAMESPACE_TOOL_FACTORY(LoKi::Hybrid,ParticleArrayFilter);
// ============================================================================
// the intializationfo the tool
// ============================================================================
StatusCode LoKi::Hybrid::ParticleArrayFilter::initialize () 
{
  // (1) initialize the base 
  StatusCode  sc = GaudiTool::initialize() ;
  if ( sc.isFailure() ) { return sc ; }                          // RETURN 
  // (2) get the factory:
  IHybridFactory* factory = tool<IHybridFactory> ( m_factory , this ) ;  
  if ( 0 == factory ) 
  { return Error ( "Could not locate IHybridFactory" ) ; }      // RETURN 
  // (3) use the factory to get the cuts 
  sc = factory->get (  m_code , m_cut ) ;
  if ( sc.isFailure() ) 
  { return Error ( "Error from IHybridFactory", sc   ) ; }     // RETURN 
  // 
  info() << "CUT: '" << m_cut << "' "<< endreq ;
  //
  return StatusCode::SUCCESS ;  
}
// ============================================================================
// The END 
// ============================================================================


