// ============================================================================
#ifndef LOKI_GENSOURCES_H
#define LOKI_GENSOURCES_H 1
// ============================================================================
// Include files
// ============================================================================
// GaudiKErnel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
// ============================================================================
// GenEvent
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/Interface.h"
#include "LoKi/GenTypes.h"
#include "LoKi/Sources.h"
// ============================================================================
/** @file LoKi/GenSources.h
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 */
// ============================================================================
namespace LoKi
{
  // ==========================================================================
  namespace Functors 
  {
    // ========================================================================
    template <>
    inline 
    LoKi::GenTypes::GenContainer
    LoKi::Functors::Source<HepMC::GenParticle,
                           LoKi::GenTypes::GenContainer,
                           LHCb::HepMCEvent::Container>::operator() ( ) const
    { return  LoKi::GenTypes::GenContainer() ; }
    // ========================================================================
  }
  // ==========================================================================
  namespace GenParticles
  {
    // ========================================================================
    /** @class SourceTES
     *  @see LoKi::Cuts::GSOURCE
     *  simple "source for the generator particles"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceTES
      : public LoKi::Functors::Source<HepMC::GenParticle           ,
                                      LoKi::GenTypes::GenContainer ,
                                      LHCb::HepMCEvent::Container  >
    {
      // =====================================================================
      typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Source _Source ;
      typedef LoKi::Functors::Source<HepMC::GenParticle           ,
                                     LoKi::GenTypes::GenContainer ,
                                     LHCb::HepMCEvent::Container  >  _Base ;
      // ======================================================================
      static_assert(std::is_base_of<_Source,_Base>::value, "Invalid base") ;
      // ======================================================================
    public:
      // =====================================================================
      /// constructor from the service, TES location and cuts
      SourceTES
      ( const IDataProviderSvc*      svc                                      ,
        const std::string&           path = LHCb::HepMCEventLocation::Default ) ;
      /// constructor from the service, TES location and cuts
      SourceTES
      ( const IDataProviderSvc*      svc                                      ,
        const LoKi::GenTypes::GCuts& cuts                                     ,
        const std::string&           path = LHCb::HepMCEventLocation::Default ) ;
      // ======================================================================
      /// constructor from the service, TES location and cuts
      SourceTES
      ( const GaudiAlgorithm*        alg                                      ,
        const std::string&           path = LHCb::HepMCEventLocation::Default ,
        const bool                   useRootInTES  = true                     ) ;
      /// constructor from the service, TES location and cuts
      SourceTES
      ( const GaudiAlgorithm*        alg                                      ,
        const LoKi::GenTypes::GCuts& cuts                                     ,
        const std::string&           path = LHCb::HepMCEventLocation::Default ,               
        const bool                   useRootInTES  = true                     ) ;
      /// MANDATORY: clone method ("virtual constructor")
      SourceTES* clone() const override { return new SourceTES(*this) ; }
      /// MANDATORY: the only essential method:
      std::vector<const HepMC::GenParticle*> operator() () const  override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream ( std::ostream& o ) const  override;
      // ======================================================================
    private:
      // ======================================================================
      /// 'on-flight' filter
      LoKi::GenTypes::GCut m_cut ;                       // 'on-flight' filter
      // ======================================================================
    } ;
    // ========================================================================
  } //                                  The end of namespace LoKi::GenParticles
  // ==========================================================================
  namespace Cuts
  {
    // ========================================================================
    /** @typedef GSOURCE
     *  The simlpe ``source'' of HepMC-particles
     *  @author Vanya BELAYEV Ivan.BElyaev@cern.ch
     *  @date 2006-12-07
     */
    typedef LoKi::GenParticles::SourceTES GSOURCE ;
    // ========================================================================
  } //                                              end of namespace LoKi::Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENSOURCES_H
// ============================================================================
