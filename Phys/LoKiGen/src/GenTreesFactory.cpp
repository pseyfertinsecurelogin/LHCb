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
// LoKi
// ============================================================================
#include "LoKi/iTree.h"
#include "LoKi/Trees.h"
#include "LoKi/GenDecays.h"
#include "LoKi/GenTreesFactory.h"
#include "LoKi/ParserFactory.h"
// ============================================================================
/** @file
 *  Implementation file for function from the file LoKi/GenTreeFactory.h
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-05-06
 */
// ============================================================================
/*  "Factory" to create the proper Tree from "short" descriptor
 *   @param tree       (OUTPUT) the constructed tree
 *   @param mother     (INPUT)  the mother particle
 *   @return status code
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *   @date   2009-05-06
 */
// ============================================================================
StatusCode Decays::Trees::factory
( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree& tree       ,
  const Decays::iNode&                                    mother     ,
  const Decays::Trees::Oscillation&                       oscillated )
{
  tree = Decays::Trees::GenExclusive ( oscillated , mother ) ;
  return StatusCode::SUCCESS ;
}
// ==========================================================================
/* "Factory" to create the proper Tree from the full description
 *  @param tree       (OUTPUT) the constructed tree
 *  @param mother     (INPUT)  the mother particle
 *  @param oscillated (INPUT)  the oscillation flag
 *  @param arrow      (INPUT)  the type of arrow
 *  @param daughters  (INPUT)  the list of daughter trees
 *  @param inclusive  (INPUT)  the flag for inclusive
 *  @param optional   (INPUT)  the list of "optional" particles
 *  @param stream     (OUTPUT) the stream to report errors
 *  @return StatusCode
 *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date   2009-05-06
 */
// ==========================================================================
StatusCode Decays::Trees::factory
( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree&           tree       ,
  const Decays::iNode&                                              mother     ,
  const Decays::Trees::Oscillation&                                 oscillated ,
  const Decays::Trees::Arrow&                                       arrow      ,
  const Decays::Trees::Types_<const HepMC::GenParticle*>::TreeList& daughters  ,
  const bool                                                        inclusive  ,
  const Decays::Trees::Types_<const HepMC::GenParticle*>::TreeList& optional   ,
  std::ostream&                                                     stream     )
{
  if ( !Decays::Trees::valid( arrow ) )
  {
    StatusCode code = StatusCode ( Decays::Trees::InvalidArrow ) ;
    stream << "ERROR: Invalid arrow : " << arrow << " " << code << '\n' ;
    return code ;
  }

  // decode the arrow
  const Decays::Trees::Alg alg       = Decays::Trees::algorithm ( arrow ) ;
  const bool               photos    = Decays::Trees::photos    ( arrow ) ;
  const bool               decayOnly = Decays::Trees::decayOnly ( arrow ) ;

  if ( inclusive && !optional.empty() )
  {
    StatusCode code = StatusCode ( Decays::Trees::InclusiveOptional ) ;
    stream << "ERROR: 'Inclusive' & 'optional' are mutually exclusive "
           <<  code << '\n' ;
    return code ;                                                     // RETURN
  }
  if ( inclusive && photos            )
  {
    StatusCode code = StatusCode ( Decays::Trees::InclusivePhotos   ) ;
    stream << "ERROR: 'Inclusive' & 'photos' are mutually exclusive   "
           << code << '\n' ;
    return code ;                                                      // RETURN
  }
  if ( !decayOnly )
  {
    StatusCode code = StatusCode ( Decays::Trees::InvalidDecayOnly ) ;
    stream << "ERROR: invalid 'Decay Only' flag                       "
           << code << '\n' ;
    return code ;                                                      // RETURN
  }

  // create the basic decay tree:
  Decays::Trees::GenExclusive decay ( mother     ,
                                      daughters  ,
                                      alg        ,
                                      oscillated ) ;
  //
  if ( inclusive && decay.marked() )
  {
    StatusCode code = StatusCode ( Decays::Trees::InclusiveMarked ) ;
    stream << "ERROR: 'Inclusive' & 'marked' can't be properly defined "
           <<  code << '\n' ;
    return code ;                                                     // RETURN
  }

  if ( optional.empty() )
  {
    if      ( !inclusive )                                   // exclusive decays
    {
      if  ( photos ) { tree = Decays::Trees::GenPhotos ( decay ) ; } // + photos
      else           { tree =                            decay   ; }
    }
    else { tree = Decays::Trees::GenInclusive ( decay ) ; }  // inclusive decays
  }
  else                                          // decays with optional elements
  {
    Decays::Trees::GenOptional opt ( decay , optional ) ;
    if  ( photos ) { tree = Decays::Trees::GenPhotosOptional ( opt) ; } // + photos
    else           { tree =                                    opt  ; }
    return StatusCode::SUCCESS ;
  }

  //
  return StatusCode::SUCCESS ;
}
// ============================================================================
/*  "Factory" to create the proper Tree from the parsed tree
 *   @param tree       (OUTPUT) the constructed tree
 *   @param parsed     (INPUT)  the parsed tree
 *   @param stream     (OUTPUT) the stream to report errors
 *   @return status code
 *   @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
 *   @date   2009-05-22
 */
// ============================================================================
StatusCode Decays::Trees::factory
( Decays::Trees::Types_<const HepMC::GenParticle*>::Tree& tree   ,
  const Decays::Parsers::Tree&                            parsed ,
  std::ostream&                                           stream )
{
  return Decays::Parsers::factory ( tree , parsed , stream ) ;        // RETURN
}
// ============================================================================


// ============================================================================
// The END
// ============================================================================
