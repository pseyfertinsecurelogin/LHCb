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
#ifndef EVENT_CALOFUTUREMCTOOLS1_H
#define EVENT_CALOFUTUREMCTOOLS1_H 1
// include
#include <numeric>
// from CaloFutureEvent
#include "Event/CaloCluster.h"
#include "Event/CaloDigit.h"
#include "Event/MCCaloDigit.h"
#include "Event/MCCaloHit.h"
#include "Event/MCParticle.h"
#include "Event/MCTruth.h"
#include "GaudiKernel/Hash.h"
#include "GaudiKernel/HashMap.h"

/** @file CaloFutureMCTools1.h
 *  few more utilities for extraction of MC information from
 *  CalorimeterFuture Event Object
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 */

namespace GaudiUtils {
  /** hashing function for LHCb::MCParticle object
   *  it uses <tt>key</tt> method
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   */
  template <>
  struct Hash<const LHCb::MCParticle*> {
    // Needed to behave like VC++'s hash_compare
    enum {             // parameters for hash table
      bucket_size = 4, // 0 < bucket_size
      min_buckets = 8
    }; // min_buckets = 2 ^^ N, 0 < N

    size_t operator()( const LHCb::MCParticle* p ) const { return !p ? size_t( 0 ) : size_t( p->key() ); }

    bool operator()( const LHCb::MCParticle* p1, const LHCb::MCParticle* p2 ) const { return m_less( p1, p2 ); }

  private:
    std::less<const LHCb::MCParticle*> m_less;
  };
} // namespace GaudiUtils

namespace CaloFutureMCTools {

  /** @class AllEnergyFromMCParticle CaloFutureMCTools1.h Event/CaloFutureMCTools1.h
   *
   * generic class/functor  to extract
   * the ALL (active) energy deposition from given LHCb::MCParticle
   *
   * For "general class" it returns  negative energy of 1 TeV
   * only the "specializations" produce
   * reasonable results
   *
   * The specializations exist for
   *
   *   - class LHCb::MCCaloFutureHit
   *   - class LHCb::MCCaloDigit
   *   - class MCCaloFutureSensPlaneHit
   *   - class   CaloDigit
   *   - class   LHCb::CaloFutureCluster
   *
   *  @see LHCb::MCParticle
   *  @see LHCb::MCCaloFutureHit
   *  @see LHCb::MCCaloDigit
   *  @see MCCaloFutureSensPlaneHit
   *  @see   CaloDigit
   *  @see   LHCb::CaloFutureCluster
   *  @see   CaloFutureHypo
   *
   *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
   *  @date   26/06/2002
   */
  struct AllEnergyFromMCParticle {
    ///  the only one essential (and absolutely useless! :-)) ) method
    template <class TYPE>
    double operator()( const TYPE* /* object   */, const LHCb::MCParticle* /* particle */ ) const {
      return -1. * Gaudi::Units::TeV;
    }
    // ========================================================================
    double operator()( const LHCb::MCCaloHit* hit, const LHCb::MCParticle* particle ) const {
      // invalid arguments
      if ( !hit || !particle ) { return 0; }
      // True particle!
      if ( particle == hit->particle() ) { return hit->activeE(); }
      const auto& vertices = particle->endVertices();
      // loop over all daughter particles
      return std::accumulate( vertices.begin(), vertices.end(), 0., [&]( double energy, const LHCb::MCVertex* mcv ) {
        if ( mcv ) {
          const auto& products = mcv->products();
          energy +=
              std::accumulate( products.begin(), products.end(), energy, [&]( double ep, const LHCb::MCParticle* mcp ) {
                if ( mcp ) ep += ( *this )( hit, mcp );
                return ep;
              } );
        }
        return energy;
      } );
    }
    // ========================================================================

    double operator()( const LHCb::MCCaloDigit* digit, const LHCb::MCParticle* particle ) const {
      // invalid arguments
      if ( !digit || !particle ) { return 0; }
      const auto& hits = digit->hits();
      return std::accumulate( hits.begin(), hits.end(), 0., [&]( double energy, const LHCb::MCCaloHit* hit ) {
        if ( hit ) energy += ( *this )( hit, particle );
        return energy;
      } );
    }
    // ==========================================================================

    double operator()( const LHCb::CaloDigit* digit, const LHCb::MCParticle* particle ) const {
      // invalid arguments
      if ( !digit || !particle ) { return 0; }
      // get MCTruth information
      // temporary plug
      if ( !digit->parent() ) { return 0; }
      // get MC truth information
      const LHCb::MCCaloDigit* mcdigit = mcTruth<LHCb::MCCaloDigit>( digit );
      // truth is available?
      if ( !mcdigit ) { return 0; }
      return ( *this )( mcdigit, particle );
    }
    // ==========================================================================

    double operator()( const LHCb::CaloCluster* cluster, const LHCb::MCParticle* particle ) const {
      // invalid arguments
      if ( !cluster || !particle ) { return 0; }
      // empty clusters
      if ( cluster->entries().empty() ) { return 0; }
      const LHCb::CaloCluster::Entries& entries = cluster->entries();
      return std::accumulate( entries.begin(), entries.end(), 0.,
                              [&]( double energy, const LHCb::CaloClusterEntry& entry ) {
                                const LHCb::CaloDigit* digit = entry.digit();
                                if ( digit ) energy += ( *this )( digit, particle );
                                return energy;
                              } );
    }
    // ==========================================================================
  };

  /** the auxilalry type to keep all MC history for calo objects
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  typedef GaudiUtils::HashMap<const LHCb::MCParticle*, double> CaloFutureMCMap;

  /** The most efficient generic structure for extraction
   *  of MC truth information for CalorimeterFuture Event objects
   *
   * For "general class" it returns  error code
   * Only the "template specializations" produce
   * reasonable results
   *
   * The specializations exist for
   *
   *   - class LHCb::MCCaloFutureHit
   *   - class LHCb::MCCaloDigit
   *   - class   CaloDigit
   *   - class   LHCb::CaloFutureCluster
   *
   *  @see MCParticle
   *  @see LHCb::MCCaloFutureHit
   *  @see LHCb::MCCaloDigit
   *  @see   CaloDigit
   *  @see   LHCb::CaloFutureCluster
   *
   *  @see   CaloFutureMCTools::CaloFutureMCMap
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  template <class TYPE>
  class MCCaloFutureHistory {
  public:
    /** constructor from pointer to CaloFutureMCMap object
     *  @param mcmap pointer to existing CaloFutureMC Map object
     */
    MCCaloFutureHistory( CaloFutureMCMap* mcmap ) : m_mcmap( mcmap ){};

    /** the oly one essential method
     *  indeed it is absolutely useless!
     *  @return status code 900 (always!)
     */
    StatusCode operator()( const TYPE* /* object */ ) const { return StatusCode( 900 ); };

    /** Accessor to history map
     *  @return pointer to history map
     */
    [[nodiscard]] CaloFutureMCMap* mcmap() const { return m_mcmap; }

  private:
    CaloFutureMCMap* m_mcmap;
  };

  /** The specialization of the most efficient generic
   *  structure for extraction
   *  of MC truth information for LHCb::MCCaloFutureHit object
   *
   *  Error codes:
   *  <ul>
   *    <li>
   *         901   invalid CaloFutureMCTool::CaloFutureMCMap object
   *    </li>
   *    <li>
   *         902   invalid LHCb::MCCaloFutureHit  object
   *    </li>
   *    <li>
   *         903   invalid link to LHCb::MCParticle object
   *    </li>
   *   </ul>
   *
   *  @param  hit pointer to LHCb::MCCaloFutureHit object
   *  @return status code
   *
   *  @see LHCb::MCCaloFutureHit
   *  @see CaloFutureMCTools::CaloFutureMCMap
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  template <>
  inline StatusCode MCCaloFutureHistory<LHCb::MCCaloHit>::operator()( const LHCb::MCCaloHit* hit ) const {
    if ( !mcmap() ) { return StatusCode( 901 ); } // RETURN
    if ( !hit ) { return StatusCode( 902 ); }     // RETURN

    // get MC particle from the hit
    const LHCb::MCParticle* particle = hit->particle();
    if ( !particle ) { return StatusCode( 903 ); } // RETURN

    // add the energy to the MC history Map
    ( *mcmap() )[particle] += hit->activeE();

    return StatusCode::SUCCESS; // RETURN
  }

  /** The specialization of the most efficient generic
   *  structure for extraction
   *  of MC truth information for LHCb::MCCaloDigit object
   *
   *  Error codes:
   *  <ul>
   *    <li>
   *         901   invalid CaloFutureMCTool::CaloFutureMCMap object
   *    </li>
   *    <li>
   *         904   invalid LHCb::MCCaloDigit  object
   *    </li>
   *   </ul>
   *
   *  @param  digit pointer to LHCb::MCCaloDigit object
   *  @return status code
   *
   *  @see LHCb::MCCaloDigit
   *  @see CaloFutureMCTools::CaloFutureMCMap
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  template <>
  inline StatusCode MCCaloFutureHistory<LHCb::MCCaloDigit>::operator()( const LHCb::MCCaloDigit* digit ) const {
    if ( !mcmap() ) { return StatusCode( 901 ); } // RETURN
    if ( !digit ) { return StatusCode( 904 ); }   // RETURN

    // get all hits
    const LHCb::MCCaloDigit::Hits& hits = digit->hits();
    if ( hits.empty() ) { return StatusCode::SUCCESS; } // RETURN

    // create the evaluator
    MCCaloFutureHistory<LHCb::MCCaloHit> evaluator( mcmap() );

    // use it!
    //    std::for_each( hits.begin() , hits.end() , evaluator ) ;
    for ( const auto& hit : hits ) { evaluator( hit ).ignore(); }

    return StatusCode::SUCCESS; // RETURN
  }

  /** The specialization of the most efficient generic
   *  structure for extraction
   *  of MC truth information for CaloDigit object
   *
   *  Error codes:
   *  <ul>
   *    <li>
   *         901   invalid CaloFutureMCTool::CaloFutureMCMap object
   *    </li>
   *    <li>
   *         905   invalid CaloDigit  object
   *    </li>
   *    <li>
   *         906   invalid parent object
   *    </li>
   *    <li>
   *         907   invalid LHCb::MCCaloDigit object
   *    </li>
   *   </ul>
   *
   *  @param  digit pointer to CaloDigit object
   *  @return status code
   *
   *  @see CaloDigit
   *  @see CaloFutureMCTools::CaloFutureMCMap
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  template <>
  inline StatusCode MCCaloFutureHistory<LHCb::CaloDigit>::operator()( const LHCb::CaloDigit* digit ) const {
    if ( !mcmap() ) { return StatusCode( 901 ); } // RETURN
    if ( !digit ) { return StatusCode( 905 ); }   // RETURN

    // is it needed ?
    if ( !digit->parent() ) { return StatusCode( 906 ); } // RETURN

    // get MC truth information
    const LHCb::MCCaloDigit* mcdigit = mcTruth<LHCb::MCCaloDigit>( digit );

    // truth is available?
    if ( !mcdigit ) { return StatusCode( 907 ); } // RETURN

    // create the evaluator
    MCCaloFutureHistory<LHCb::MCCaloDigit> evaluator( mcmap() );

    // use it!
    return evaluator( mcdigit );
  }

  /** The specialization of the most efficient generic
   *  structure for extraction
   *  of MC truth information for LHCb::CaloFutureCluster object
   *
   *  Error codes:
   *  <ul>
   *    <li>
   *         901   invalid CaloFutureMCTool::CaloFutureMCMap object
   *    </li>
   *    <li>
   *         908   invalid LHCb::CaloFutureCluster  object
   *    </li>
   *   </ul>
   *
   *  @param  cluster pointer to LHCb::CaloFutureCluster object
   *  @return status code
   *
   *  @see LHCb::CaloFutureCluster
   *  @see CaloFutureMCTools::CaloFutureMCMap
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  template <>
  inline StatusCode MCCaloFutureHistory<LHCb::CaloCluster>::operator()( const LHCb::CaloCluster* cluster ) const {
    if ( !mcmap() ) { return StatusCode( 901 ); } // RETURN
    if ( !cluster ) { return StatusCode( 908 ); } // RETURN

    // create the evaluator
    MCCaloFutureHistory<LHCb::CaloDigit> evaluator( mcmap() );
    // and use it!
    for ( const auto& entry : cluster->entries() ) {
      const LHCb::CaloDigit* digit = entry.digit();
      if ( digit ) { evaluator( digit ); }
    }

    return StatusCode::SUCCESS;
  }

  /** Auxillary function to update the CaloFutureMCTools::CaloFutureMCMap object
   *  It adds the energy deposition from the given particle to
   *  ALL parents
   *  @param mcp    pointer to LHCb::MCParticle
   *  @param energy nergy need to be added
   *  @param mcmap  map to be updated
   *
   *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-04-01
   */
  inline StatusCode updateCaloFutureMCMap( const LHCb::MCParticle* mcp, const double energy, CaloFutureMCMap& mcmap ) {
    if ( !mcp ) { return StatusCode::FAILURE; }

    const LHCb::MCParticle* particle = mcp;
    while ( particle ) {
      const LHCb::MCVertex* vertex = particle->originVertex();
      if ( !vertex ) { return StatusCode::SUCCESS; }
      particle = vertex->mother();
      if ( !particle ) { return StatusCode::SUCCESS; }

      mcmap[particle] += energy;
    }

    return StatusCode::SUCCESS;
  }

} // namespace CaloFutureMCTools

// ============================================================================
#endif // EVENT_CALOFUTUREMCTOOLS1_H
