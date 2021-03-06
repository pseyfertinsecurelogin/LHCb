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
#include <vector>
// ============================================================================
// Event
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Combiner.h"
#include "LoKi/MCChild.h"
#include "LoKi/MCSections.h"
// ============================================================================
/** @file
 *  Implementation file for function LoKi::Decay::sections
 *  @author Vanya BELYAEV Ivan.Belayev@nikhef.nl
 *  @date 2008-04-29
 */
// ============================================================================
/*  simple function to extract all section from the graph/tree
 *  @param particle the head of th egraph/tree
 *  @param output the sections of the graph/tree
 *  @param decay the boolean flag to consider <b>only</b> decay products
 *  @return size of the section container
 */
// ============================================================================
size_t Decays::sections( const LHCb::MCParticle* particle, Decays::MCSections& output, const bool decay ) {
  // trivial case 1
  if ( 0 == particle ) { return output.size(); } // RETURN
  // add the particle itself as one of the section
  output.push_back( MCSection( 1, particle ) );                    // the primitive section
  if ( particle->endVertices().empty() ) { return output.size(); } // RETURN
  // get all daughters particles
  LHCb::MCParticle::ConstVector children;
  LoKi::MCChild::daughters( particle, children, decay );
  if ( children.empty() ) { return output.size(); } // RETURN
  // get all sections fomr daughter particles
  typedef std::vector<MCSections> SECTIONS;
  SECTIONS                        all( children.size() );
  for ( size_t index = 0; index < children.size(); ++index ) { sections( children[index], all[index] ); } // RECURSION
  //
  typedef LoKi::Combiner_<MCSections> Loop;
  typedef Loop::Range                 Range;
  typedef Loop::Select                Select;
  // create the proper combiner
  Loop loop;
  // fill the combiner with the information
  for ( SECTIONS::const_iterator it = all.begin(); all.end() != it; ++it ) {
    loop.add( Range( *it ) );
  } // Fill the LOOP!
  ///
  // make the real looping over all combinations of sections
  for ( ; loop.valid(); ++loop ) {
    // get the "current" multi-iterator
    const Select& sel = loop.current();
    // construct the section
    MCSection section;
    for ( Select::const_iterator it = sel.begin(); sel.end() != it; ++it ) {
      section.insert( section.end(), ( *it )->begin(), ( *it )->end() );
    }
    // add it into the collection of sections
    output.push_back( section );
  }
  //
  return output.size(); // RETURN
}

// ============================================================================
// The END
// ============================================================================
