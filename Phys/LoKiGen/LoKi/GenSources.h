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
#include "LoKi/GenTypes.h"
#include "LoKi/Interface.h"
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
namespace LoKi {
  // ==========================================================================
  namespace Functors {
    // ========================================================================
    template <>
    inline LoKi::GenTypes::GenContainer
    LoKi::Functors::Source<HepMC::GenParticle, LoKi::GenTypes::GenContainer, LHCb::HepMCEvent::Container>::
    operator()() const {
      return LoKi::GenTypes::GenContainer();
    }
    // ========================================================================
  } // namespace Functors
  // ==========================================================================
  namespace GenParticles {
    // ========================================================================
    /** @class SourceTES
     *  @see LoKi::Cuts::GSOURCE
     *  simple "source for the generator particles"
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class SourceTES
        : public LoKi::Functors::Source<HepMC::GenParticle, LoKi::GenTypes::GenContainer, LHCb::HepMCEvent::Container> {
      // =====================================================================
      typedef LoKi::BasicFunctors<const HepMC::GenParticle*>::Source _Source;
      typedef LoKi::Functors::Source<HepMC::GenParticle, LoKi::GenTypes::GenContainer, LHCb::HepMCEvent::Container>
          _Base;
      // ======================================================================
      static_assert( std::is_base_of<_Source, _Base>::value, "Invalid base" );
      // ======================================================================
    public:
      // =====================================================================
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const std::string& path = LHCb::HepMCEventLocation::Default );
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const LoKi::GenTypes::GCuts& cuts,
                 const std::string& path = LHCb::HepMCEventLocation::Default );
      // ======================================================================
      /// constructor from the service, TES location and cuts
      SourceTES( const GaudiAlgorithm* alg, const std::string& path = LHCb::HepMCEventLocation::Default,
                 const bool useRootInTES = true );
      /// constructor from the service, TES location and cuts
      SourceTES( const GaudiAlgorithm* alg, const LoKi::GenTypes::GCuts& cuts,
                 const std::string& path = LHCb::HepMCEventLocation::Default, const bool useRootInTES = true );
      /// MANDATORY: clone method ("virtual constructor")
      SourceTES* clone() const override { return new SourceTES( *this ); }
      /// MANDATORY: the only essential method:
      std::vector<const HepMC::GenParticle*> operator()() const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& o ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// 'on-flight' filter
      LoKi::GenTypes::GCut m_cut; // 'on-flight' filter
      // ======================================================================
    };
    // ========================================================================
    /** @class TESData
     *  special source that relies on DataHandle to access data in TES
     *  @see LoKi::TES::DataHanble
     *  @see DataObjectReadHandle
     *  @see LoKi::Cuts::GTESDATA
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date    2018-08-20
     */
    class TESData : public LoKi::BasicFunctors<const HepMC::GenParticle*>::Source,
                    public LoKi::TES::DataHandle<LHCb::HepMCEvent::Container> {
    public:
      // ======================================================================
      /// constructor
      TESData( const GaudiAlgorithm* algorithm, const std::string& location = LHCb::HepMCEventLocation::Default );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const std::string& location, const LoKi::GenTypes::GCuts& cuts );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const LoKi::GenTypes::GCuts& cuts,
               const std::string& location = LHCb::HepMCEventLocation::Default );
      /// MANDATORY: clone method ("virtual constructor")
      TESData* clone() const override;
      /// MANDATORY: the only essential method:
      result_type operator()() const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& o ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// 'on-flight' filter
      LoKi::GenTypes::GCut m_cuts; // 'on-flight' filter
      // ======================================================================
    };
    // ========================================================================
    /** @class TESCounter
     *  simple functor to count number of 'good'-objects form TES
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @see LoKi::Cuts::GNUM
     *  @date   2018-08-22
     */
    class GAUDI_API TESCounter : public LoKi::Functor<void, double> {
    public:
      // ======================================================================
      /// constructor from the source
      explicit TESCounter( const LoKi::BasicFunctors<const HepMC::GenParticle*>::Source& s );
      // =============================================================================
      /// MANDATORY: clone method ("virtual constructor")
      TESCounter* clone() const override;
      /// MANDATORY: the only essential method:
      double operator()() const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& o ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual source
      LoKi::Assignable_t<LoKi::BasicFunctors<const HepMC::GenParticle*>::Source> m_source; // the actual source
      // ======================================================================
    };
    // ========================================================================
  } // namespace GenParticles
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @typedef GSOURCE
     *  The simple ``source'' of HepMC-particles
     *  @author Vanya BELYAEV Ivan.BElyaev@cern.ch
     *  @date 2006-12-07
     */
    typedef LoKi::GenParticles::SourceTES GSOURCE;
    // ========================================================================
    /** @typedef GTESDATA
     *  The simple ``source'' of HepMC-particles
     *  @attention DataHandler is used
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-08-22
     */
    typedef LoKi::GenParticles::TESData GTESDATA;
    // ========================================================================
    /** @typedef GNUM
     *  Count particles from the source
     *  @author Vanya BELYAEV Ivan.Belyaev@cern.ch
     *  @date 2006-12-07
     */
    typedef LoKi::GenParticles::TESCounter GNUM;
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} // namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENSOURCES_H
