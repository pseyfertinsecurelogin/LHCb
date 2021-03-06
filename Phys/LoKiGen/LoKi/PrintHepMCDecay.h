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
#ifndef LOKI_PRINTHEPMCDECAY_H
#define LOKI_PRINTHEPMCDECAY_H 1
// ============================================================================
// Include files
// ===========================================================================
// STD & STL
// ===========================================================================
#include <iostream>
#include <limits>
#include <string>
// ===========================================================================
// HepMC
// ===========================================================================
#include "HepMC/GenParticle.h"
// ===========================================================================
// LoKi
// ===========================================================================
#include "LoKi/GenTypes.h"
// ===========================================================================
// forward declarations
// ===========================================================================
class MsgStream;
// ============================================================================
/** @file
 *
 *  This file is a part of LoKi project -
 *    "C++ ToolKit  for Smart and Friendly Physics Analysis"
 *
 *  The package has been designed with the kind help from
 *  Galina PAKHLOVA and Sergey BARSUK.  Many bright ideas,
 *  contributions and advices from G.Raven, J.van Tilburg,
 *  A.Golutvin, P.Koppenburg have been used in the design.
 *
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-05-26
 */
// ============================================================================
namespace LoKi {
  namespace PrintHepMC {
    // ========================================================================
    /// the maximal recursion level
    const int s_maxLevel1 = std::numeric_limits<int>::max();
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  printDecay( p , std::cout ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @param stream   stream to be used
     *  @param cut      condition
     *  @param blank    to be printed instead of cutted particles
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    std::ostream& printDecay( const HepMC::GenParticle* particle, std::ostream& stream,
                              const LoKi::GenTypes::GCuts& cut, const int level = s_maxLevel1,
                              const std::string& blank = "<cut>" );
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  printDecay( p , std::cout ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @param stream   stream to be used
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    std::ostream& printDecay( const HepMC::GenParticle* particle, std::ostream& stream );
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  printDecay( p , always() ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @param stream   stream to be used
     *  @param cut      condition
     *  @param blank    to be printed instead of cutted particles
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    MsgStream& printDecay( const HepMC::GenParticle* particle, MsgStream& stream, const LoKi::GenTypes::GCuts& cut,
                           const int level = s_maxLevel1, const std::string& blank = "<cut>" );
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  printDecay( p , always() ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @param stream   stream to be used
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    MsgStream& printDecay( const HepMC::GenParticle* particle, MsgStream& stream );
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  info() << printHepMCDecay( p ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @param cut      condition
     *  @param blank    to be printed instead of cutted particles
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    std::string printDecay( const HepMC::GenParticle* particle, const LoKi::GenTypes::GCuts& cut,
                            const int level = s_maxLevel1, const std::string& blank = "<cut>" );
    // ========================================================================
    /** Simple function to print HepMC decay in more or less "readable" format
     *
     *  @code
     *
     *  const HepMC::GenParticle* p = ... ;
     *
     *  info() << printDecay( p ) ;
     *
     *  @endcode
     *
     *  @param particle pointer to HepMC::GenParticle to be printed
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2006-01-18
     */
    std::string printDecay( const HepMC::GenParticle* particle );
    // ========================================================================
  } // namespace PrintHepMC
  // ==========================================================================
  namespace Print {
    // ========================================================================
    // import into namespace LoKi::Print
    // ========================================================================
    using namespace LoKi::PrintHepMC;
    // ========================================================================
  } // namespace Print
} // end of namespace LoKi
// ============================================================================
// The END
// ============================================================================
#endif // LOKI_PRINTHEPMCDECAY_H
