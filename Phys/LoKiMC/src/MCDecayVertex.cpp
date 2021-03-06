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
// STD& STL
// ============================================================================
#include <algorithm>
// ============================================================================
// Evevt
// ============================================================================
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/MCDecayVertex.h"
// ============================================================================
/*  simple function to get the decay vertex for the given
 *  Monte Carlo particle
 *
 *  @code
 *
 *  const LHCb::MCParticle* p = ... ;
 *
 *  // get the decay vertex:
 *  const LHCb::MCVertex* decay = decayVertex ( p ) ;
 *
 *  @endcode
 *
 *  @see LHCb::MCParticle
 *  @see LHCb::MCVertex
 *  @see LHCb::MCVertex::isDecay
 *  @see LoKi::MCVertices::IsDecay
 *  @see LoKi::MCVertices::MCDecayVertex
 *
 *  @param p Monte Carlo particle
 *  @return the decay vertex
 *
 *  @author Vanya BELYAEV  Ivan.Belyaev@nikhef.nl
 *  @date 2008-07-10
 */
// ============================================================================
const LHCb::MCVertex* LoKi::MCVertices::decayVertex( const LHCb::MCParticle* p ) {
  if ( 0 == p ) { return 0; } // RETURN
  typedef SmartRefVector<LHCb::MCVertex> MCVERTICES;
  const MCVERTICES&                      vertices = p->endVertices();
  MCVERTICES::const_iterator             idecay   = std::find_if( vertices.begin(), vertices.end(), IsDecay );
  if ( vertices.end() == idecay ) { return 0; } // RETURN
  return *idecay;
}
// ============================================================================
// The END
// ============================================================================
