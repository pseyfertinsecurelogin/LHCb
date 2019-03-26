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
#ifndef LOKI_MCSOURCES_H
#define LOKI_MCSOURCES_H 1
// ============================================================================
// Include files
// ============================================================================
// STD&STL
// ============================================================================
#include <type_traits>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IDataProviderSvc.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/iNode.h"
// ============================================================================
// MCEvent
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/BasicFunctors.h"
#include "LoKi/IMCDecay.h"
#include "LoKi/Interface.h"
#include "LoKi/MCTypes.h"
#include "LoKi/Sources.h"
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace MCParticles {
    // ========================================================================
    /** @class SourceTES
     *  simple "source for the Monte Carlo particles"
     *  @see LoKi::Cuts::MCSOURCE
     *  @author Vanya BELYAEV ibelyav@physics.syr.edu
     *  @date 2006-12-07
     */
    class GAUDI_API SourceTES
        : public LoKi::Functors::Source<LHCb::MCParticle, LHCb::MCParticle::ConstVector, LHCb::MCParticle::Container> {
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::MCParticle*>::Source _Source;
      typedef LoKi::Functors::Source<LHCb::MCParticle, LHCb::MCParticle::ConstVector, LHCb::MCParticle::Container>
          _Base;
      // ======================================================================
      static_assert( std::is_base_of<_Source, _Base>::value, "Invalid base" );
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const std::string& path = LHCb::MCParticleLocation::Default );
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const LoKi::MCTypes::MCCuts& cuts,
                 const std::string& path = LHCb::MCParticleLocation::Default );
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const Decays::iNode& node,
                 const std::string& path = LHCb::MCParticleLocation::Default );
      /// constructor from the service, TES location and finder
      SourceTES( const IDataProviderSvc* svc, const Decays::IMCDecay::Finder& finder,
                 const std::string& path = LHCb::MCParticleLocation::Default );
      /// constructor from the service, TES location and tree
      SourceTES( const IDataProviderSvc* svc, const Decays::IMCDecay::iTree& finder,
                 const std::string& path = LHCb::MCParticleLocation::Default );
      /// constructor from the service, TES location
      SourceTES( const IDataProviderSvc* svc, const std::string& descriptor, const std::string& path );
      // ======================================================================
      /// constructor from the own algorithm , TES location and cuts
      SourceTES( const GaudiAlgorithm* algorithm, const std::string& path = LHCb::MCParticleLocation::Default,
                 const bool useRootInTES = true );
      /// constructor from the own algortihm , TES location and cuts
      SourceTES( const GaudiAlgorithm* algorithm, const LoKi::MCTypes::MCCuts& cuts,
                 const std::string& path = LHCb::MCParticleLocation::Default, const bool useRootInTES = true );
      /// constructor from the own algorithm , TES location and cuts
      SourceTES( const GaudiAlgorithm* algorithm, const Decays::iNode& node,
                 const std::string& path = LHCb::MCParticleLocation::Default, const bool useRootInTES = true );
      /// constructor from the own algorithm , TES location and finder
      SourceTES( const GaudiAlgorithm* algorithm, const Decays::IMCDecay::Finder& finder,
                 const std::string& path = LHCb::MCParticleLocation::Default, const bool useRootInTES = true );
      /// constructor from the own algorithm, TES location and tree
      SourceTES( const GaudiAlgorithm* algorithm, const Decays::IMCDecay::iTree& finder,
                 const std::string& path = LHCb::MCParticleLocation::Default, const bool useRootInTES = true );
      /// constructor from the algorithm , TES location
      SourceTES( const GaudiAlgorithm* algorithm, const std::string& descriptor, const std::string& path,
                 const bool useRootInTES = true );
      // =============================================================================
      /// MANDATORY: clone method ("virtual constructor")
      SourceTES* clone() const override;
      /// MANDATORY: the only essential method:
      LHCb::MCParticle::ConstVector operator()() const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& o ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the data from TES
      std::size_t get( const std::string& location, LHCb::MCParticle::ConstVector& output ) const;
      // ======================================================================
      /// count data in TES
      std::size_t count( const std::string& location ) const;
      // ======================================================================
    public:
      // ======================================================================
      const LoKi::MCTypes::MCCuts&    cut() const { return m_cut; }
      const Decays::IMCDecay::Finder& finder() const { return m_finder; }
      bool                            use_finder() const { return m_use_finder; }
      void                            buildFinder() const;
      // ======================================================================
    private:
      // ======================================================================
      /// 'on-flight' filter
      LoKi::MCTypes::MCCut m_cut; // 'on-flight' filter
      /// use decay finder ?
      bool m_use_finder; // use decay finder ?
      /// decay finder
      mutable Decays::IMCDecay::Finder m_finder; //       decay finder
      std::string                      m_decay;
      // ======================================================================
    };
    // ========================================================================
    /** @class TESData
     *  special source that relies on DataHandle to access data in TES
     *  @see LoKi::TES::DataHanble
     *  @see DataObjectReadHandle
     *  @see LoKi::Cuts::MCTESDATA
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date    2018-08-20
     */
    class TESData : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Source,
                    public LoKi::TES::DataHandle<LHCb::MCParticle::Container> {
    public:
      // ======================================================================
      /// constructor
      TESData( const GaudiAlgorithm* algorithm, const std::string& location = LHCb::MCParticleLocation::Default );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const std::string& location, const LoKi::MCTypes::MCCuts& cuts );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const LoKi::MCTypes::MCCuts& cuts,
               const std::string& location = LHCb::MCParticleLocation::Default );
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
      LoKi::MCTypes::MCCut m_cuts; // 'on-flight' filter
      // ======================================================================
    };
    // ========================================================================
    /** @class TESCounter
     *  simple functor to count number of 'good'-objects form TES
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     *  @see LoKi::Cuts::MCNUM
     *  @date   2010-10-24
     */
    class GAUDI_API TESCounter : public LoKi::Functor<void, double> {
    public:
      // ======================================================================
      /// constructor from the source
      explicit TESCounter( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Source& s );
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
      LoKi::Assignable_t<LoKi::BasicFunctors<const LHCb::MCParticle*>::Source> m_source; // the actual source
      // ======================================================================
    };
    // ========================================================================
  } // namespace MCParticles
  // ==========================================================================
  namespace MCVertices {
    // ========================================================================
    /** @class SourceTES
     *
     *  @see LoKi::Cuts::MCVSOURCE
     *  simple "source for the Monte Carlo vertices "
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     *  @date 2010-06-04
     */
    class GAUDI_API SourceTES
        : public LoKi::Functors::Source<LHCb::MCVertex, LHCb::MCVertex::ConstVector, LHCb::MCVertex::Container> {
      // ======================================================================
      typedef LoKi::BasicFunctors<const LHCb::MCVertex*>::Source                                             _Source;
      typedef LoKi::Functors::Source<LHCb::MCVertex, LHCb::MCVertex::ConstVector, LHCb::MCVertex::Container> _Base;
      // ======================================================================
      static_assert( std::is_base_of<_Source, _Base>::value, "Invalid base" );
      // ======================================================================
    public:
      // ======================================================================
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const std::string& path = LHCb::MCVertexLocation::Default );
      /// constructor from the service, TES location and cuts
      SourceTES( const IDataProviderSvc* svc, const LoKi::MCTypes::MCVCuts& cuts,
                 const std::string& path = LHCb::MCVertexLocation::Default );
      /// constructor from the service, TES location and cuts
      SourceTES( const GaudiAlgorithm* algorithm, const std::string& path = LHCb::MCVertexLocation::Default,
                 const bool useRootInTES = true );
      /// constructor from the service, TES location and cuts
      SourceTES( const GaudiAlgorithm* algorithm, const LoKi::MCTypes::MCVCuts& cuts,
                 const std::string& path = LHCb::MCVertexLocation::Default, const bool useRootInTES = true );
      /// MANDATORY: clone method ("virtual constructor")
      SourceTES* clone() const override;
      /// MANDATORY: the only essential method:
      LHCb::MCVertex::ConstVector operator()() const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& o ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// 'on-flight' filter
      LoKi::MCTypes::MCVCut m_cut; // 'on-flight' filter
      // ======================================================================
    };
    // ========================================================================
    /** @class TESData
     *  special source that relies on DataHandle to access data in TES
     *  @see LoKi::TES::DataHanble
     *  @see DataObjectReadHandle
     *  @see LoKi::Cuts::MCVTESDATA
     *  @author Vanya BELYAEV Ivan.BElyaev@itep.ru
     *  @date    2018-08-20
     */
    class TESData : public LoKi::BasicFunctors<const LHCb::MCVertex*>::Source,
                    public LoKi::TES::DataHandle<LHCb::MCVertex::Container> {
    public:
      // ======================================================================
      /// constructor
      TESData( const GaudiAlgorithm* algorithm, const std::string& location = LHCb::MCVertexLocation::Default );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const std::string& location, const LoKi::MCTypes::MCVCuts& cuts );
      /// constructor with cuts
      TESData( const GaudiAlgorithm* algorithm, const LoKi::MCTypes::MCVCuts& cuts,
               const std::string& location = LHCb::MCVertexLocation::Default );
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
      LoKi::MCTypes::MCVCut m_cuts; // 'on-flight' filter
      // ======================================================================
    };
    // ========================================================================
  } // namespace MCVertices
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @typedef MCTESDATA
     *  simple ``source'' for MC-particles
     *  @attention DataHandler is used!
     *  @see LoKi::MCParticles::TESData
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-08-22
     */
    typedef LoKi::MCParticles::TESData MCTESDATA;
    // ========================================================================
    /** @typedef MCVTESDATA
     *  simple ``source'' for MC-vertices
     *  @attention DataHandler is used!
     *  @see LoKi::MCVertices::TESData
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2018-08-22
     */
    typedef LoKi::MCVertices::TESData MCVTESDATA;
    // ========================================================================
    /** @typedef MCSOURCE
     *  simple ``source'' for MC-particles
     *
     *  @code
     *
     *   const MCSOURCE s1 = MCSOURCE ( "MC/Particles" ,   MCPT > 1 * GeV ) ;
     *   const MCSOURCE s2 = MCSOURCE ( "MC/Particles" , " D0 => K- pi+ " ) ;
     *
     *    const LHCb::MCParticle::ConstVector v1 = s1() ;
     *    const LHCb::MCParticle::ConstVector v2 = s2() ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticles::SourceTES
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     */
    typedef LoKi::MCParticles::SourceTES MCSOURCE;
    // ========================================================================
    /** @typedef MCSOURCE
     *  simple ``source'' for MC-particles
     *
     *  @code
     *
     *   const MCSOURCE s1 = MCSOURCE ( "MC/Particles" ,   MCPT > 1 * GeV ) ;
     *   const MCSOURCE s2 = MCSOURCE ( "MC/Particles" , " D0 => K- pi+ " ) ;
     *
     *    const LHCb::MCParticle::ConstVector v1 = s1() ;
     *    const LHCb::MCParticle::ConstVector v2 = s2() ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticles::SourceTES
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     */
    typedef LoKi::MCParticles::SourceTES MCSOURCE;
    // ========================================================================
    /** @typedef MCVSOURCE
     *  simple ``source'' for MC-vertices
     *
     *  @code
     *
     *   const MCVSOURCE s = MCSOURCE ( "MC/Vertices" ,   MCVZ < 1 * meter ) ;
     *
     *   const LHCb::MCVertex::ConstVector v2 = s2() ;
     *
     *  @endcode
     *
     *  @see LoKi::MCParticles::SourceTES
     *  @author Vanya BELYAEV Ivan.BElyaev@nikhef.nl
     */
    typedef LoKi::MCVertices::SourceTES MCVSOURCE;
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} // namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCSOURCES_H
