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
#ifndef EVENT_CALOMCTOOLS_H
#define EVENT_CALOMCTOOLS_H 1
// Include files
#include <numeric>
// Event
#include "Event/MCParticle.h"
// Kernel
#include "Event/MCTruth.h"
// GaudiKernel
#include "GaudiKernel/Map.h"
// CaloEvent/Event
#include "Event/MCCaloHit.h"
#include "Event/MCCaloDigit.h"
#include "Event/CaloDigit.h"
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"
#include "CaloMCTools1.h"

/** @namespace CaloMCTools CaloMCTools.h Event/CaloMCTools.h
 *
 *  Collection of useful methods/utilities/functors
 *  for fast(inline) extraction of MC truth information
 *  for Calo-object
 *
 *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
 *  @date   31/10/2001
 */

namespace CaloMCTools
{
  // ==========================================================================
  /** check if the first particle is a "parent" of the second one
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   *
   *  @see MCParticle
   *  @param first   pointer to MCParticle object
   *  @param second  pointer to MCParticle object
   *  @return true if first particle is equivalent to the second one
   *               or it is a "parent" of the second one
   */
  inline bool isParent( const LHCb::MCParticle* first  ,
                        const LHCb::MCParticle* second )
  {
    // check arguments
    if      ( !first || !second ) { return false ; }
    else if (  first ==  second ) { return true  ; }
    // extract the vertex
    const LHCb::MCVertex*   vertex = second->originVertex() ;
    // start the recursion
    return vertex && isParent( first , vertex->mother() );
  }
  // ==========================================================================

  // ==========================================================================
  /** @class EnergyFromMCParticle CaloMCTools.h Event/CaloMCTools.h
   *
   * generic class/functor  to extract
   * the (active) energy deposition from given MCParticle
   *
   * For "general class" it returns  negative energy of 1 TeV
   * only the "template specializations" produce
   * reasonable results
   *
   * The specializations exist for
   *
   *   - class MCCaloHit
   *   - class LHCb::MCCaloDigit
   *   - class MCCaloSensPlaneHit
   *   - class   CaloDigit
   *   - class   LHCb::CaloCluster
   *   - class   LHCb::CaloHypo
   *
   *  @see MCParticle
   *  @see MCCaloHit
   *  @see LHCb::MCCaloDigit
   *  @see MCCaloSensPlaneHit
   *  @see   CaloDigit
   *  @see   LHCb::CaloCluster
   *  @see   LHCb::CaloHypo
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   */
  struct EnergyFromMCParticle
  {
    ///  the only one essential (and quite useless! :-)) ) method
    template<class TYPE>
    inline double operator() ( const TYPE*       /* object   */  ,
                        const LHCb::MCParticle* /* particle */  ) const
    { return   -1. * Gaudi::Units::TeV ; }
    // ==========================================================================
    inline double operator() ( const LHCb::MCCaloHit*  hit      ,
                               const LHCb::MCParticle* particle ) const
    {
      return ( hit && particle && isParent( particle , hit->particle() ) ) ?
               hit->activeE() : 0.0 ;
    }
    // ==========================================================================
    inline double operator() ( const LHCb::MCCaloDigit*  digit    ,
                               const LHCb::MCParticle*   particle ) const
    {
      // trivial checks
      if( !digit || !particle ) { return 0. ; }
      // accumulate the energy from the hits
      const auto& hits = digit->hits();
      return std::accumulate( hits.begin(), hits.end(), 0.,
                              [&](double e, const LHCb::MCCaloHit* hit) {
                                  return e + (*this)( hit, particle );
                              });
    }
    // ==========================================================================
    inline double operator()( const LHCb::CaloDigit*    digit    ,
                              const LHCb::MCParticle*   particle ) const
    {
      // trivial checks
      if( !digit || !particle ) { return 0 ; }
      // temporary plug
      if( !digit->parent()        ) { return 0 ; }
      // get MC truth information
      const LHCb::MCCaloDigit* mcdigit = mcTruth<LHCb::MCCaloDigit>( digit );
      // truth is available?
      if( !mcdigit ) { return 0 ; }
      // go to MC history!
      return (*this)( mcdigit , particle );
    }
    // ==========================================================================
    inline double operator() ( const LHCb::CaloCluster*  cluster  ,
                               const LHCb::MCParticle*   particle ) const
    {
      // trivial checks
      if( !cluster || !particle ) { return 0 ; }
      // accumulate the energy
      const LHCb::CaloCluster::Entries& entries = cluster->entries();
      return std::accumulate( entries.begin(), entries.end(), 0.,
                              [&](double energy, const LHCb::CaloCluster::Entry& entry)
                              { return energy + (*this)( entry.digit() , particle ); } );
    }
    // ==========================================================================
    inline double operator() ( const LHCb::CaloHypo*     hypo     ,
                               const LHCb::MCParticle*   particle ) const
    {
      // trivial checks
      if( !hypo || !particle ) { return 0. ; }
      const LHCb::CaloHypo::Clusters& clusters = hypo->clusters();
      // accumulate the energy
      return std::accumulate( clusters.begin(), clusters.end(), 0.,
                              [&](double energy, const LHCb::CaloCluster* cluster)
                              { return energy + (*this)( cluster , particle ); } );
    }
    // ==========================================================================
  };



  // ==========================================================================
  /**
   *  generic function  to extract
   *  the (active) energy deposition from given MCParticle
   *
   *  @see EnergyFromMCParticle
   *  @see MCParticle
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   *
   *  @param object   pointer to the object object
   *  @param particle pointer to MCParticle  object
   *  @return summed active energy of the digit from
   *          the given particle or its dauhter particles
   */
  constexpr static const auto energyFromMCParticle = EnergyFromMCParticle{};
  // ==========================================================================

  // ==========================================================================
  /** @class FromMCParticle CaloMCTools.h Event/CaloMCTools.h
   *
   *  simple predicate/functor to select useful objects,
   *  which have the connection with given MCParticle
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   */
  class FromMCParticle
  {
  public:
    /** constructor
     *  @param particle pointer to MCParticle
     *  @param threshold on active energy deposition
     */
    FromMCParticle( const LHCb::MCParticle* particle     ,
                    const double      threshold = 0 )
      : m_particle  ( particle  )
      , m_threshold ( threshold )   {} ;
    /** the only essential method
     *  @param object pointer to the object
     *  @return true if energy deposition from given
     *           MCparticle to this object exceeds
     *           the given threshold
     */
    template <class TYPE>
    inline bool operator() ( const TYPE* object ) const
    {
      if( !m_particle || !object ) { return false ; }
      return m_threshold <  energyfromMCParticle( object , m_particle );
    }
    ///
    /** the only essential method
     *  @param digit pointer to the LHCb::MCCaloDigit
     *  @return true if energy deposition from given
     *           MCParticle to this digit exceeds
     *           the given threshold
     */
    inline bool operator() ( const LHCb::MCCaloDigit* digit ) const
    {
      // check arguments!
      if( !m_particle || !digit  ) { return false ; }
      // dramatical speed up!!!
      if( digit->activeE() < m_threshold       ) { return false ; }
      // use general technique
      return m_threshold <  energyFromMCParticle( digit , m_particle );
    };
    ///
  private:
    ///
    const LHCb::MCParticle*    m_particle  ;
    double                     m_threshold ;
    ///
  };
  // ==========================================================================

  // ==========================================================================
  /**
   *  simple function to select useful objects,
   *  which have the connection with given MCParticle
   *
   *  @see FromMCParticle
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   *
   *  @param object pointer to the object
   *  @param particle pointer to MCParticle
   *  @param threshold on active energy deposition
   *  @return true if energy deposition from given
   *           MCparticle to this object exceeds
   *           the given threshold
   */
  template <class TYPE>
  inline bool
  fromMCParticle ( const TYPE*       object         ,
                   const LHCb::MCParticle* particle       ,
                   const double      threshold  = 0 )
  {
    return FromMCParticle{ particle , threshold }( object );
  }
  // ==========================================================================

  // ==========================================================================
  /** useful type definition to return the particle found.
   *  The first element of the pair is the active energy deposition
   *  the second one is the pointer to MCParticle object
   */
  typedef std::pair<double,const LHCb::MCParticle*>  ParticlePair;
  // ==========================================================================

  // ==========================================================================
  /** @class LargestDeposition CaloMCTools.h Event/CaloMCTools.h
   *
   *  Determine the largest energy deposition to the calorimeter object.
   *
   *  It returs empty pair (deposition,MCParticle) for generic classes
   *  The non-trivial specializations exist for
   *
   *    - class LHCb::MCCaloHit
   *    - class LHCb::MCCaloDigit
   *    - class   CaloDigit
   *    - class   LHCb::CaloCluster
   *    - class MCCaloSensPlaneHit
   *
   *    @see MCParticle
   *    @see LHCb::MCCaloHit
   *    @see LHCb::MCCaloDigit
   *    @see   CaloDigit
   *    @see   LHCb::CaloCluster
   *    @see MCCaloSensPlaneHit
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   30/10/2001
   */
  struct  LargestDeposition
  {
    /** the only one essential method
     *  it is "trivial" for general class
     */
    template <class TYPE>
    ParticlePair operator() ( const TYPE* /* obj */ ) const
    { return ParticlePair( -1 * Gaudi::Units::TeV, nullptr ); }
    ///

    ParticlePair operator()( const LHCb::MCCaloHit* hit ) const
    {
      return hit ? ParticlePair( hit->activeE() , hit->particle() ) : ParticlePair();
    }
    // ==========================================================================

    ParticlePair operator() ( const LHCb::MCCaloDigit* digit ) const
    {
      // skip NULLs and trivial digits
      if( !digit || digit->hits().empty() ) { return ParticlePair(); }
      const LHCb::MCCaloDigit::Hits& hits = digit->hits();
      return std::accumulate( hits.begin(), hits.end(), ParticlePair{0,nullptr},
                              [&](ParticlePair output, const LHCb::MCCaloHit* hit)
                              {
                                auto aux =  (*this)(hit);
                                return aux.first >= output.first ? aux : output ;
                              } );
    }

    ParticlePair operator() ( const LHCb::CaloDigit* digit ) const
    {
      // skip nulls
      if( !digit           ) { return ParticlePair() ; }
      // temporary plug
      if( !digit->parent() ) { return ParticlePair() ; }
      // extract MC truth information
      const LHCb::MCCaloDigit* mcdigit = mcTruth<LHCb::MCCaloDigit>( digit );
      if( !mcdigit ) { return ParticlePair() ; }
      return (*this)( mcdigit );
    }
    // ==========================================================================

    ParticlePair operator() ( const LHCb::CaloCluster* cluster ) const
    {
      // skip NULLs and trivial clusters
      if( !cluster || cluster->entries().empty() ) { return ParticlePair(); }
      // local map
      GaudiUtils::Map<const LHCb::MCParticle*,double> auxMap;
      // fill the local map with information
      for( const auto& entry : cluster->entries() ) {
          const LHCb::CaloDigit*     digit    = entry.digit  () ;
          if( !digit               ) { continue ; }   //   CONTINUE !
          // temporary plug to avoid crash in mcTruth<> !
          if( !digit->parent()     ) { continue ; }   //   CONTINUE !
          const LHCb::MCCaloDigit* mcdigit    = mcTruth<LHCb::MCCaloDigit>( digit );
          if( !mcdigit )               { continue ; }   //   CONTINUE !
          // loop over all MC hits
          for( const auto& hit : mcdigit->hits () ) {
              const ParticlePair auxPair( (*this)( hit ) );
              auxMap[ auxPair.second ] += auxPair.first ;
          } // end of loop over hits in LHCb::MCCaloDigit
      } // end of loop over Cluster entries
      // find maximal deposition
      return std::accumulate( auxMap.begin(), auxMap.end(), ParticlePair{},
                              // select the maximal contribution
                              [](ParticlePair output, const std::pair<const LHCb::MCParticle*,double>& map)
                              { return ( map.first && map.second>output.first ) ?
                                        std::pair{map.second,map.first} : output ;
                              } );

    }
  // ==========================================================================
  };

  constexpr static const auto largestDeposition = LargestDeposition{};

} // The end of namespace CaloMCTools

// ============================================================================
#endif // EVENT_CALOMCTOOLS_H
// ============================================================================
