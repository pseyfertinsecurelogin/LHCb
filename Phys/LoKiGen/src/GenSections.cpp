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
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Combiner.h"
#include "LoKi/GenChild.h"
#include "LoKi/GenSections.h"
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
size_t Decays::sections( const HepMC::GenParticle* particle, Decays::GenSections& output ) {
  // trivial case 1
  if ( 0 == particle ) { return output.size(); } // RETURN
  // add the particle itself as one of the section
  output.push_back( GenSection( 1, particle ) );               // the primitive section
  if ( 0 == particle->end_vertex() ) { return output.size(); } // RETURN
  // get all children particles:
  std::vector<const HepMC::GenParticle*> children;
  LoKi::GenChild::daughters( particle, children );
  if ( children.empty() ) { return output.size(); } // RETURN
  // get all sections from daughter particles:
  std::vector<GenSections> all( children.size() );
  for ( size_t index = 0; index < children.size(); ++index ) { sections( children[index], all[index] ); } // RECURSION
  //
  typedef LoKi::Combiner_<GenSections> Loop;
  typedef Loop::Range                  Range;
  typedef Loop::Select                 Select;
  // create the proper combiner
  Loop loop;
  // fill the combiner with the information
  for ( const auto& i : all ) { loop.add( Range( i ) ); } // Fill the LOOP!
  ///
  // make the real looping over all combinations of sections
  for ( ; loop.valid(); ++loop ) {
    // get the "current" multi-iterator
    const Select& sel = loop.current();
    // construct the section
    GenSection section;
    for ( const auto& s : sel ) { section.insert( section.end(), s->begin(), s->end() ); }
    // add it into the collection of sections
    output.push_back( section );
  }
  //
  return output.size(); // RETURN
}
// ============================================================================
// The END
// ============================================================================
