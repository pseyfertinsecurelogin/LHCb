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
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <algorithm>
// ============================================================================
// Event
// ============================================================================
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCChild.h"
#include "LoKi/MCDecayVertex.h"
// ============================================================================
/** @file
 *  Implementation file for functions form namespace LoKi::MCChild
 *  @date 2007-06-02
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 */
// ============================================================================
// get the number of children
// ============================================================================
unsigned int LoKi::MCChild::nChildren( const LHCb::MCParticle* mother ) {
  if ( 0 == mother ) { return 0; } // RETURN
  typedef SmartRefVector<LHCb::MCVertex> EVs;
  const EVs&                             evs = mother->endVertices();
  // find "LHCb::MCParticle::isDecay" vertex:
  EVs::const_iterator iev = std::find_if( evs.begin(), evs.end(), LoKi::MCVertices::IsDecay );
  if ( evs.end() == iev ) { return 0; } // RETURN
  const LHCb::MCVertex* ev = *iev;
  return ev->products().size(); // RETURN
}
// ============================================================================
/* Trivial accessor to the dautgher particles for the given MC-particle.
 *
 *  @attention index starts with 1 , null index corresponds
 *             to the original particle
 *
 *  @param  particle (const) pointer to mother particle
 *  @param  index    index   index of the child particle
 *  @return daughter particle with given index
 */
// ============================================================================
const LHCb::MCParticle* LoKi::MCChild::child( const LHCb::MCParticle* mother, const unsigned int index ) {
  if ( 0 == mother ) { return 0; }     // RETURN
  if ( 0 == index ) { return mother; } // RETURN
  typedef SmartRefVector<LHCb::MCVertex> EVs;
  const EVs&                             evs = mother->endVertices();
  // find "LHCb::MCParticle::isDecay" vertex:
  EVs::const_iterator iev = std::find_if( evs.begin(), evs.end(), LoKi::MCVertices::IsDecay );
  if ( evs.end() == iev ) { return 0; } // RETURN
  const LHCb::MCVertex* ev = *iev;
  if ( index > ev->products().size() ) { return 0; } // RETURN
  // get the proper particle:
  return ev->products()[index - 1]; // RETURN
}
// ========================================================================
/*  get all daughters for the given MCparticle
 *  @param particle  MC-particle
 *  @param output the vector of daughetr particles
 *  @param decayOnly flag to consider only particles from the decay
 *  @return the size of daughter vector
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ========================================================================
unsigned int LoKi::MCChild::daughters( const LHCb::MCParticle* particle, LHCb::MCParticle::ConstVector& output,
                                       const bool decayOnly ) {
  if ( !output.empty() ) { output.clear(); }
  if ( 0 == particle ) { return output.size(); } // RETURN
  typedef SmartRefVector<LHCb::MCVertex> EVs;
  const EVs&                             evs = particle->endVertices();
  for ( EVs::const_iterator iev = evs.begin(); evs.end() != iev; ++iev ) {
    const LHCb::MCVertex* ev = *iev;
    if ( 0 == ev ) { continue; }                     // CONTINUE
    if ( decayOnly && !ev->isDecay() ) { continue; } // CONTINUE
    const SmartRefVector<LHCb::MCParticle>& products = ev->products();
    output.insert( output.end(), products.begin(), products.end() );
  }
  return output.size();
}
// ============================================================================
/** get all descendants for the given MCparticle
 *  @param particle  MC-particle
 *  @param decayOnly flag to consider only particles from the decay
 *  @return vector of descendants particles
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCChild::descendants( const LHCb::MCParticle* particle, const bool decayOnly ) {
  LHCb::MCParticle::ConstVector result;
  if ( 0 == particle ) { return result; } // RETURN
  typedef SmartRefVector<LHCb::MCVertex> EVs;
  const EVs&                             evs = particle->endVertices();
  for ( EVs::const_iterator iev = evs.begin(); evs.end() != iev; ++iev ) {
    const LHCb::MCVertex* ev = *iev;
    if ( 0 == ev ) { continue; }                     // CONTINUE
    if ( decayOnly && !ev->isDecay() ) { continue; } // CONTINUE
    typedef SmartRefVector<LHCb::MCParticle> Ps;
    const Ps&                                ps = ev->products();
    for ( Ps::const_iterator ip = ps.begin(); ps.end() != ip; ++ip ) {
      if ( 0 == *ip ) { continue; }
      // insert the daughter:
      result.push_back( *ip );
      const LHCb::MCParticle::ConstVector& v = descendants( *ip, decayOnly );
      // insert all its descendants:
      result.insert( result.end(), v.begin(), v.end() );
    } // loop over daughter particles
  }   // loop over end-vertices
  return result;
}
// ============================================================================
/** get all ancestors for the given MCparticle
 *  @param particle  MC-particle
 *  @param decayOnly flag to consider only particles from the decay
 *  @return vector of ancestors particles
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCChild::ancestors( const LHCb::MCParticle* particle ) {
  LHCb::MCParticle::ConstVector result;
  if ( 0 == particle ) { return result; } // RETURN
  const LHCb::MCParticle* mother = particle->mother();
  while ( 0 != mother ) {
    result.push_back( mother );
    mother = mother->mother();
  }
  return result;
}
// ============================================================================
/*  get all (0 or 1) parents for the given particle
 *  @param particle  MC-particle
 *  @return vector of parent particles (1or 2)
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCChild::parents( const LHCb::MCParticle* particle ) {
  LHCb::MCParticle::ConstVector result;
  if ( 0 == particle ) { return result; } // RETURN
  const LHCb::MCParticle* mother = particle->mother();
  if ( 0 == mother ) { return result; } // RETURN
  result.push_back( mother );
  return result; // RETURN
}
// ============================================================================
/*  get the mother particle  (just for completeness of interface)
 *  @param particle  MC-particle
 *  @return mother particle
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
const LHCb::MCParticle* LoKi::MCChild::mother( const LHCb::MCParticle* particle ) {
  return 0 == particle ? particle : particle->mother();
}
// ============================================================================
/*  get all daughters for the given MCparticle
 *  @param particle  MC-particle
 *  @param decayOnly flag to consider only particles from the decay
 *  @return vector of daughetr particles
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
LHCb::MCParticle::ConstVector LoKi::MCChild::children( const LHCb::MCParticle* particle, const bool decayOnly ) {
  LHCb::MCParticle::ConstVector result;
  daughters( particle, result, decayOnly );
  return result;
}
// ============================================================================
/*  Trivial accessor to the daughter particles for the given MC-particle.
 *
 *  @attention index starts with 1 , null index corresponds
 *             to the original particle
 *  @attention Only the particles from isDecay" end-vertex are considered
 *
 *  @see LHCb::MCParticle
 *  @see LHCb::MCVertex::isDecay
 *  @param  particle (const) pointer to mother particle
 *  @param  index1   index   index of the child particle
 *  @param  index2   index   index of the child particle
 *  @param  index3   index   index of the child particle
 *  @param  index4   index   index of the child particle
 *  @return daughter particle with given index
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
const LHCb::MCParticle* LoKi::MCChild::child( const LHCb::MCParticle* particle, const unsigned int index1,
                                              const unsigned int index2, const unsigned int index3,
                                              const unsigned int index4 ) {
  return child( child( particle, index1 ), index2, index3, index4 );
}
// ============================================================================
/*  Trivial accessor to the daughter "decay" particles for the
 *  given MC-particle.
 *
 *  @attention index starts with 1 , null index corresponds
 *             to the original particle
 *  @attention Only the particles from isDecay" end-vertex are considered
 *
 *  @see LHCb::MCParticle
 *  @see LHCb::MCVertex::isDecay
 *  @param  particle (const) pointer to mother particle
 *  @param  index1   index   index of the child particle
 *  @param  index2   index   index of the child particle
 *  @param  index3   index   index of the child particle
 *  @return daughter particle with given index
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
const LHCb::MCParticle* LoKi::MCChild::child( const LHCb::MCParticle* particle, const unsigned int index1,
                                              const unsigned int index2, const unsigned int index3 ) {
  return child( child( particle, index1 ), index2, index3 );
}
// ============================================================================
/*  Trivial accessor to the daughter "decay" particles for the given
 *  MC-particle.
 *
 *  @attention index starts with 1 , null index corresponds
 *             to the original particle
 *  @attention Only the particles from isDecay" end-vertex are considered
 *
 *  @see LHCb::MCParticle
 *  @see LHCb::MCVertex::isDecay
 *  @param  particle (const) pointer to mother particle
 *  @param  index1   index   index of the child particle
 *  @param  index2   index   index of the child particle
 *  @return daughter particle with given index
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ============================================================================
const LHCb::MCParticle* LoKi::MCChild::child( const LHCb::MCParticle* particle, const unsigned int index1,
                                              const unsigned int index2 ) {
  return child( child( particle, index1 ), index2 );
}
// ========================================================================
namespace {
  // ==========================================================================
  inline const LHCb::MCParticle* _child( const LHCb::MCParticle* particle, const std::vector<unsigned int>& indices,
                                         unsigned int last ) {
    if ( 0 == particle || indices.empty() || 0 == last || indices.size() < last ) { return 0; }
    //
    switch ( last ) {
    case 1:
      return LoKi::MCChild::child( particle, indices[0] );
    case 2:
      return LoKi::MCChild::child( particle, indices[0], indices[1] );
    case 3:
      return LoKi::MCChild::child( particle, indices[0], indices[1], indices[2] );
    case 4:
      return LoKi::MCChild::child( particle, indices[0], indices[1], indices[2], indices[3] );
    default:
      break;
    }
    //
    const unsigned int next = --last;
    //
    return LoKi::MCChild::child( _child( particle, indices, next ), indices[next] );
    //
  }
  // ==========================================================================
} // namespace
// ============================================================================
/* Trivial accessor to the daughter "decay" particles for the
 *  given MC-particle
 *
 *  @attention index starts with 1 , null index corresponds
 *             to the original particle
 *  @attention Only the particles from isDecay" end-vertex are considered
 *
 *  @see LHCb::MCParticle
 *  @see LHCb::MCVertex::isDecay
 *  @param  mother  (const) pointer to mother particle
 *  @param  indices vector of indices of the child particles
 *  @return daughter particle with given index
 *  @author Vanya BELYAEV ibelyaev@phsycis.syr.edu
 *  @date 2007-06-02
 */
// ========================================================================
const LHCb::MCParticle* LoKi::MCChild::child( const LHCb::MCParticle*          mother,
                                              const std::vector<unsigned int>& indices ) {
  return _child( mother, indices, indices.size() );
}
// ========================================================================

// ============================================================================
// The END
// ============================================================================
