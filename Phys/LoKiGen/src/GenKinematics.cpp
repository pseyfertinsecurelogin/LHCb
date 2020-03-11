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
// LoKiCore
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Report.h"
// ============================================================================
// LoKiGen
// ============================================================================
#include "LoKi/GenKinematics.h"
// ============================================================================
/** @file
 *
 * Implementation file for class : GenKinematics
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
 *  @date 2001-01-23
 */
// ============================================================================
/*  trivial function to evaluate the mass HepMC::GenParticle
 *  @param  p particle
 *  @return invariant mass
 *  @see HepMC::GenParticle
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::GenKinematics::mass( const HepMC::GenParticle* p ) {
  if ( !p ) {
    LoKi::Report::Error( "mass(HepMC::GenParticle*),invaild argument,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  return LoKi::Kinematics::mass( LoKi::LorentzVector( p->momentum() ) );
}
// ============================================================================
/*  trivial function to evaluate the mass HepMC::GenParticle
 *  @param  p1 the first particle
 *  @param  p2 the second particle
 *  @return invariant mass
 *  @see HepMC::GenParticle
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::GenKinematics::mass( const HepMC::GenParticle* p1, const HepMC::GenParticle* p2 ) {
  if ( !p1 ) {
    LoKi::Report::Error( "mass(2*HepMC::GenParticle*),invaild argument 1,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p2 ) {
    LoKi::Report::Error( "mass(2*HepMC::GenParticle*),invaild argument 2,return'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  //
  return LoKi::Kinematics::mass( LoKi::LorentzVector( p1->momentum() ), LoKi::LorentzVector( p2->momentum() ) );
}
// ============================================================================
/*  trivial function to evaluate the mass HepMC::GenParticle
 *  @param  p1 the first particle
 *  @param  p2 the third particle
 *  @param  p3 the second particle
 *  @return invariant mass
 *  @see HepMC::GenParticle
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::GenKinematics::mass( const HepMC::GenParticle* p1, const HepMC::GenParticle* p2,
                                  const HepMC::GenParticle* p3 ) {
  if ( !p1 ) {
    LoKi::Report::Error( "mass(3*HepMC::GenParticle*),invaild argument 1,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p2 ) {
    LoKi::Report::Error( "mass(3*HepMC::GenParticle*),invaild argument 2,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p3 ) {
    LoKi::Report::Error( "mass(3*HepMC::GenParticle*),invaild argument 3,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  //
  return LoKi::Kinematics::mass( LoKi::LorentzVector( p1->momentum() ), LoKi::LorentzVector( p2->momentum() ),
                                 LoKi::LorentzVector( p3->momentum() ) );
}
// ============================================================================
/*  trivial function to evaluate the mass HepMC::GenParticle
 *  @param  p1 the first particle
 *  @param  p2 the second particle
 *  @param  p3 the third  particle
 *  @param  p3 the fourth  particle
 *  @return invariant mass
 *  @see HepMC::GenParticle
 *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
 *  @date 2006-01-17
 */
// ============================================================================
double LoKi::GenKinematics::mass( const HepMC::GenParticle* p1, const HepMC::GenParticle* p2,
                                  const HepMC::GenParticle* p3, const HepMC::GenParticle* p4 ) {
  if ( !p1 ) {
    LoKi::Report::Error( "mass(4*HepMC::GenParticle*),invaild argument 1,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p2 ) {
    LoKi::Report::Error( "mass(4*HepMC::GenParticle*),invaild argument 2,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p3 ) {
    LoKi::Report::Error( "mass(4*HepMC::GenParticle*),invaild argument 3,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  if ( !p4 ) {
    LoKi::Report::Error( "mass(4*HepMC::GenParticle*),invaild argument 4,return 'InvalidMass'" )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    return LoKi::Constants::InvalidMass;
  }
  //
  return LoKi::Kinematics::mass( LoKi::LorentzVector( p1->momentum() ), LoKi::LorentzVector( p2->momentum() ),
                                 LoKi::LorentzVector( p3->momentum() ), LoKi::LorentzVector( p4->momentum() ) );
}
// ============================================================================
// The END
// ============================================================================
