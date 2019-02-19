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
#ifndef LOKI_MCPARTICLES_H
#define LOKI_MCPARTICLES_H 1
// ============================================================================
// Include files
// ============================================================================
// STL & STD
// ============================================================================
#include <math.h>
#include <string>
// ============================================================================
// Event
// ============================================================================
#include "Event/MCParticle.h"
// ============================================================================
// MCInterfaces
// ============================================================================
#include "MCInterfaces/IMCDecayFinder.h"
#include "MCInterfaces/IMCParticleSelector.h"
#include "MCInterfaces/IMCReconstructible.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Functions.h"
#include "LoKi/Interface.h"
#include "LoKi/Kinematics.h"
#include "LoKi/MCChildSelector.h"
#include "LoKi/MCPIDOperators.h"
#include "LoKi/MCTypes.h"
#include "LoKi/Operators.h"
#include "LoKi/PidFunctions.h"
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
 *  @date 2001-01-23
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  /** @namespace LoKi::MCParticles MCParticles.h LoKi/MCParticles.h
   *
   *  @author Vanya  BELYAEV Ivan.Belyaev@itep.ru
   *  @date   2003-08-09
   */
  namespace MCParticles {
    // ========================================================================
    /** @class Momentum
     *  evaluator of the momentum of the particle
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API Momentum : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Momentum* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Energy
     *  evaluator of the energy of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API Energy : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Energy* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class TransverseMomentum
     *  evaluator of the transverse momentum of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API TransverseMomentum : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      TransverseMomentum* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumX
     *  evaluator of the x component of the particle's momentum
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API MomentumX : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      MomentumX* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumY
     *  evaluator of the y component of the particle's momentum
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API MomentumY : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      MomentumY* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumZ
     *  evaluator of the z component of the particle's momentum
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API MomentumZ : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      MomentumZ* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Theta
     *  evaluator of the theta angle of the particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date   2002-07-15
     */
    class GAUDI_API Theta : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Theta* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class PseudoRapidity
     *  evaluator of the pseudorapidity of the particle
     *  @see LoKi::Cuts::MCETA
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API PseudoRapidity : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      PseudoRapidity* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
      /// get eta
      double eta( const LHCb::MCParticle* p ) const { return eta( p->momentum() ); }
      /// get eta
      double eta( const Gaudi::LorentzVector& v ) const { return v.Eta(); }
      /// get rapidity
      double y( const LHCb::MCParticle* p ) const { return y( p->momentum() ); }
      /// get rapidity
      double y( const Gaudi::LorentzVector& v ) const;
      /// get rapidity0
      double y0( const LHCb::MCParticle* p ) const { return y( p->momentum() ); }
      /// get rapidity0
      double y0( const Gaudi::LorentzVector& v ) const;
      // ======================================================================
    };
    // ========================================================================
    /** @class Rapidity
     *  evaluator of the rapidity of the particle
     *
     *  \f$ y = \frac{1}{2}\log \frac{ E - p_z }{ E + p_z } \f$
     *
     *  @see LoKi::Cuts::MCY
     *  @see LoKi::Cuts::MCY0
     *  @see LoKi::Cuts::MCETA
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2010-02-10
     */
    class GAUDI_API Rapidity : public PseudoRapidity {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Rapidity* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Rapidity0
     *  evaluator of the rapidity_0 of the particle
     *
     *  \f$ y_0 = \frac{1}{2}\log \frac{ E - p }{ E + p } \f$
     *
     *  @see LoKi::Cuts::MCY0
     *  @see LoKi::Cuts::MCY
     *  @see LoKi::Cuts::MCETA
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2010-02-10
     */
    class GAUDI_API Rapidity0 : public Rapidity {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Rapidity0* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Phi
     *  evaluator of the 'Phi' of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API Phi : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Phi* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      /// get phi
      double phi( const LHCb::MCParticle* p ) const { return p->momentum().Phi(); }
      // ======================================================================
      /// adjust phi into the range of [-180:180]degrees
      double adjust( double angle ) const {
        static const double s_180 = 180 * Gaudi::Units::degree;
        static const double s_360 = 360 * Gaudi::Units::degree;
        //
        while ( angle > s_180 ) { angle -= s_360; }
        while ( angle < -1 * s_180 ) { angle += s_360; }
        //
        return angle;
      }
      // ======================================================================
    };
    // ========================================================================
    /** @class Mass
     *  evaluator of the mass of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API Mass : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Mass* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Identifier
     *  evaluator of the id of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API Identifier : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Identifier* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsID
     *  new verison of PID-comparison
     */
    // ========================================================================
    class GAUDI_API IsID : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate, public LoKi::Pids::GetPids {
    public:
      // ======================================================================
      IsID( const long id );
      IsID( const unsigned long id );
      IsID( const LHCb::ParticleID& id );
      IsID( const std::string& id );
      IsID( const std::vector<int>& ids );
      IsID( const std::vector<long>& ids );
      IsID( const std::vector<unsigned int>& ids );
      IsID( const std::vector<unsigned long>& ids );
      IsID( const std::vector<LHCb::ParticleID>& ids );
      IsID( const std::vector<std::string>& ids );
      IsID( const LoKi::Pids::GetPids& ids );
      // ======================================================================
      IsID* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsNotID
     *  new verison of PID-comparison
     */
    // ========================================================================
    class GAUDI_API IsNotID : public LoKi::MCParticles::IsID {
    public:
      // ======================================================================
      IsNotID( const long id );
      IsNotID( const unsigned long id );
      IsNotID( const LHCb::ParticleID& id );
      IsNotID( const std::string& id );
      IsNotID( const std::vector<int>& ids );
      IsNotID( const std::vector<long>& ids );
      IsNotID( const std::vector<unsigned int>& ids );
      IsNotID( const std::vector<unsigned long>& ids );
      IsNotID( const std::vector<LHCb::ParticleID>& ids );
      IsNotID( const std::vector<std::string>& ids );
      IsNotID( const LoKi::Pids::GetPids& ids );
      // ======================================================================
      IsNotID* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    class GAUDI_API IsAbsID : public LoKi::MCParticles::IsID {
    public:
      // ======================================================================
      IsAbsID( const long id );
      IsAbsID( const unsigned long id );
      IsAbsID( const LHCb::ParticleID& id );
      IsAbsID( const std::string& id );
      IsAbsID( const std::vector<int>& ids );
      IsAbsID( const std::vector<long>& ids );
      IsAbsID( const std::vector<unsigned int>& ids );
      IsAbsID( const std::vector<unsigned long>& ids );
      IsAbsID( const std::vector<LHCb::ParticleID>& ids );
      IsAbsID( const std::vector<std::string>& ids );
      IsAbsID( const LoKi::Pids::GetPids& ids );
      // ======================================================================
      IsAbsID* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    class GAUDI_API IsNotAbsID : public LoKi::MCParticles::IsAbsID {
    public:
      // ======================================================================
      IsNotAbsID( const long id );
      IsNotAbsID( const unsigned long id );
      IsNotAbsID( const LHCb::ParticleID& id );
      IsNotAbsID( const std::string& id );
      IsNotAbsID( const std::vector<int>& ids );
      IsNotAbsID( const std::vector<long>& ids );
      IsNotAbsID( const std::vector<unsigned int>& ids );
      IsNotAbsID( const std::vector<unsigned long>& ids );
      IsNotAbsID( const std::vector<LHCb::ParticleID>& ids );
      IsNotAbsID( const std::vector<std::string>& ids );
      IsNotAbsID( const LoKi::Pids::GetPids& ids );
      // ======================================================================
      IsNotAbsID* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class AbsIdentifier
     *  evaluator of the id of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API AbsIdentifier : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      AbsIdentifier* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Oscillated
     *
     *  It evaluates to 'true' for oscillated particles
     *
     *  @author Vanya Belyaev ibelyaev@physics.syr.edu
     *  @date   2006-02-05
     */
    class GAUDI_API Oscillated : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      Oscillated* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ThreeCharge
     *  evaluator of the id of the particle
     *
     *  @author Vanya Belyaev Ivan.Belyaev@itep.ru
     *  @date   2002-07-15
     */
    class GAUDI_API ThreeCharge : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      ThreeCharge* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class ProperLifeTime
     *  evaluator of the proper lifetime of the particle
     *
     *  Properlife time formaly evaluated as the
     *  distance between the first 'endVertex'
     *  and 'originVertex' times m/p
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API ProperLifeTime : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      ProperLifeTime* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class HasQuark
     *  simple predicate , return true if particle has quark 'quark'
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API HasQuark : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      HasQuark( const LHCb::ParticleID::Quark quark );
      /// clone method (mandatory!)
      HasQuark* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      //
      LHCb::ParticleID::Quark quark() const { return m_quark; }
      // ======================================================================
    private:
      // ======================================================================
      LHCb::ParticleID::Quark m_quark;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsCharged
     *  simple predicate , return true if the particle is charged
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsCharged : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsCharged* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsNeutral
     *  simple predicate , return true if the particle is neutral
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsNeutral : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsNeutral* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsLepton
     *  simple predicate , return true if the particle is a lepton
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsLepton : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsLepton* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsMeson
     *  simple predicate , return true if the particle is a meson
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsMeson : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsMeson* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsBaryon
     *  simple predicate , return true if the particle is a baryon
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsBaryon : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsBaryon* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsHadron
     *  simple predicate , return true of particle is hadron
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsHadron : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsHadron* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsNucleus
     *  simple predicate , return true of particle is nucleus
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-01-25
     */
    class GAUDI_API IsNucleus : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// clone method (mandatory!)
      IsNucleus* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class FromMCDecayTree
     *  simple predicate whoch evaluates if the MC particle belons to
     *  MC decay tree of other MC particle
     *  @author Vanya BELYAEV  Ivan.Belyaev@itep.ru
     *  @date   2004-07-07
     *  @warning for this class <c>toCpp</c> method is not properly defined
     */
    class GAUDI_API FromMCDecayTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    protected:
      // ======================================================================
      typedef std::vector<const LHCb::MCParticle*> MCCont;
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from 1 MC particle
       *  @param mcp pointer to MC particle
       */
      FromMCDecayTree( const LHCb::MCParticle* mcp );
      /** constructor from range of MC particles
       *  @param cnt range of MC particles
       */
      FromMCDecayTree( const LoKi::MCTypes::MCRange& cnt );
      /** constructor from container of MC particles
       *  @param cnt container of MC particles
       */
      FromMCDecayTree( const LoKi::MCTypes::MCContainer& cnt );
      /** constructor from container of MC particles
       *  @param cnt container of MC particles
       */
      FromMCDecayTree( const ::LHCb::MCParticles* cnt );
      /** templated constructor from any sequence of MC particles
       *  @param first begin of the sequence
       *  @param last  end   of the sequence
       */
      template <class MCPARTICLE>
      FromMCDecayTree( MCPARTICLE first, MCPARTICLE last )
          : LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate(), m_cont( first, last ) {}
      /// clone method (mandatory!)
      FromMCDecayTree* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /** the add MCparticle into the list of tested particles
       *  @param p pointe to the particle
       *  @return status code
       */
      StatusCode add( const LHCb::MCParticle* p );
      /** add particles into the lst of particles
       *  @param first begin of sequence
       *  @param last  end of the sequence
       *  @return status code
       */
      template <class MCPARTICLE>
      StatusCode add( MCPARTICLE first, MCPARTICLE last ) {
        m_cont.reserve( m_cont.size() + last - first );
        StatusCode sc = StatusCode::SUCCESS;
        for ( ; first != last; ++first ) {
          StatusCode code = add( *first );
          if ( code.isFailure() ) { sc = code; }
        }
        return sc;
      }
      /** remove LHCb::MCParticle from the list of tested LHCb::MCParticles
       *  @param p pointe to the LHCb::MCParticle
       *  @return status code
       */
      StatusCode remove( const LHCb::MCParticle* p );
      /** remove LHCb::MCParticles from the list of tested MCarticles
       *  @param first begin of sequence
       *  @param last  end of the sequence
       */
      template <class MCPARTICLE>
      StatusCode remove( MCPARTICLE first, MCPARTICLE last ) {
        StatusCode sc = StatusCode::SUCCESS;
        for ( ; first != last; ++first ) {
          StatusCode code = remove( *first );
          if ( code.isFailure() ) { sc = code; }
        }
        return sc;
      }
      /** set new LHCb::MCParticles
       *  @param first begin of sequence
       *  @param last  end of the sequence
       *  @return status code
       */
      template <class MCPARTICLE>
      StatusCode setParticles( MCPARTICLE first, MCPARTICLE last ) {
        m_cont.clear();
        return add( first, last );
      }
      // ======================================================================
    private:
      // ======================================================================
      MCCont m_cont;
      // ======================================================================
    };
    // ========================================================================
    /** @class InAncestors
     *  Simple predicate to check certain cut for ancestors (recursive)
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @see LoKi::Cuts::MCINANCESTORS
     *  @date 2010-02-16
     */
    class GAUDI_API InAncestors : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// constructor from the predicate
      InAncestors( const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      /// MANDATORY: clone method ("virtual constructor")
      InAncestors* clone() const override;
      /// MANDATORY: the only one important method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// the actual evaluator
      bool inAncestors( const LHCb::MCParticle* p ) const;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual predicate
      LoKi::MCTypes::MCCut m_cut; // the actual predicate
      bool                 m_decayOnly;
      // ======================================================================
    };
    // ========================================================================
    /** @class NinAncestors
     *  Simple functor for count the certain cut for ancestors (recursive)
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @see LoKi::Cuts::MCNINANCESTORS
     *  @date 2010-02-16
     */
    class GAUDI_API NinAncestors : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// constructor from the predicate
      NinAncestors( const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      /// MANDATORY: clone method ("virtual constructor")
      NinAncestors* clone() const override;
      /// MANDATORY: the only one important method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// the actual evaluator
      unsigned int nInAncestors( const LHCb::MCParticle* p ) const;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual predicate
      LoKi::MCTypes::MCCut m_cut; // the actual predicate
      bool                 m_decayOnly;
      // ======================================================================
    };
    // ========================================================================
    /** @class NinMCdownTree
     *  Simple function to count the appearence
     *  of predicate over the decay tree (down!)
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-11-17
     */
    class GAUDI_API NinMCdownTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** constructor
       *  @param cut cut to be inspected
       */
      NinMCdownTree( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate& cut );
      /// clone method/"virtual constructor" (mandatory!)
      NinMCdownTree* clone() const override;
      /// the main method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCCut m_cut;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCMotherFunction
     *  simple adapter function which
     *  delegates the evaluation of
     *  function to LHCb::MCParticle::mother()
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2004-11-17
     */
    class GAUDI_API MCMotherFunction : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** constructor
       *  @param fun function to be evaluated
       *  @param val value to be returned for 'invalid' mother
       */
      MCMotherFunction( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Function& fun, const double val );
      /// clone method (virtual constructor) (mandatory!)
      MCMotherFunction* clone() const override;
      /// the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      double               m_val;
      LoKi::MCTypes::MCFun m_fun;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCMotherPredicate
     *  simple adapter function which
     *  delegates the evaluation of
     *  function to LHCb::MCParticle::mother()
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date   2005-02-02
     */
    class GAUDI_API MCMotherPredicate : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /** constructor
       *  @param cut predicate to be evaluated
       *  @param val value to be returned for 'invalid' mother
       */
      MCMotherPredicate( const LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate& cut, const bool val );
      /// clone method (virtual constructor) (mandatory!)
      MCMotherPredicate* clone() const override;
      /// the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      bool                 m_val;
      LoKi::MCTypes::MCCut m_cut;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsParticle
     *  Trivial predicate to check
     *  if particle is identical
     *  to some other Particle
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-02
     *  @warning for this class <c>toCpp</c> method is not defined properly
     */
    class GAUDI_API IsParticle : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /** constructor from 1 particle
       *  @param object object to be compared
       */
      IsParticle( const LHCb::MCParticle* object );
      /** constructor from range of particles
       *  @param obj range of object to be compared
       */
      IsParticle( const LoKi::MCTypes::MCRange& obj );
      /** templated constructor
       *  @param begin 'begin'-iterator for sequence of objects
       *  @param end   'end'-iterator for sequence of objects
       */
      template <class OBJECT>
      IsParticle( OBJECT begin, OBJECT end ) : m_objects( begin, end ) {}
      /// MANDATORY: clone method ('virtual constructor')
      IsParticle* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LHCb::MCParticle::ConstVector m_objects;
      // ======================================================================
    };
    // ========================================================================
    /** @class IsContainedObject
     *  Trivial predicate to check
     *  if particle is identical
     *  to some ContainedObject
     *  @warning for this class <c>toCpp</c> method is not defined properly
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-02-02
     */
    class GAUDI_API IsContainedObject : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      typedef std::vector<const ContainedObject*> Objects;
      // ======================================================================
    public:
      // ======================================================================
      /** constructor from contained object
       *  @param object object to be compared
       */
      IsContainedObject( const ContainedObject* object );
      /** templated constructor
       *  @param begin 'begin'-iterator for sequence of objects
       *  @param end   'end'-iterator for sequence of objects
       */
      template <class OBJECT>
      IsContainedObject( OBJECT begin, OBJECT end )
          : LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate(), m_objects( begin, end ) {}
      /// MANDATORY: clone method ('virtual constructor')
      IsContainedObject* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      Objects m_objects;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumDistance
     *  Trivial evaluator of EUCLIDIAN distance
     *  of 4-momentums; it could be useful e.g.
     *  for trivial "kinematical matching":
     *  One could find the particle with "closest"
     *  4-momentum to the given one:
     *
     *  @code
     *
     *  SEQUENCE            particles = ... ;
     *  LoKi::LorentzVector vct       = ... ;
     *
     *  // find "best match"
     *  SEQUENCE::const_iterator imin =
     *    LoKi::select_min( particles.begin () ,
     *                      particles.end   () ,
     *                      MCMOMDIST ( vct )  ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCMOMDIST
     *  @see LoKi::LorentzVector
     *
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    class GAUDI_API MomentumDistance : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// constructor from four components
      MomentumDistance( const double px, const double py, const double pz, const double e );
      /// constructor from the given 4-momentum
      MomentumDistance( const LoKi::LorentzVector& vct );
      /// MANDATORY: clone function ("virtual constructor")
      MomentumDistance* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;

    private:
      // ======================================================================
      LoKi::LorentzVector m_vct;
      // ======================================================================
    };
    // ========================================================================
    /** @class TransverseMomentumRel
     *  Trivial evaluator of the transverse momenrum,
     *  relatve to the given direction
     *
     *  @code
     *
     *  const LHCb::MCParticle*  particle = ... ;
     *  LoKi::ThreeVector  vct  = ... ;
     *
     *  MCFun pt = MCPTDIR( vct ) ;
     *  const double pt = pt ( particle ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCPTDIR
     *  @author Vanya BELYAEV belyaev@lapp.in2p3.fr
     *  @date 2005-03-23
     */
    class GAUDI_API TransverseMomentumRel : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// constructor from theta & phi
      TransverseMomentumRel( const double theta, const double phi );
      /** constructor
       *  @param vct direction vertor
       *  @see LoKi::ThreeVector
       */
      TransverseMomentumRel( const LoKi::ThreeVector& vct );
      /// MANDATORY: clone function ("virtual constructor")
      TransverseMomentumRel* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // =====================================================================
    private:
      // ======================================================================
      LoKi::ThreeVector m_vct;
      // ======================================================================
    };
    // ========================================================================
    /** @class DeltaPhi
     *  Simple evaluator of "DeltaPhi" for the particle
     *
     *  @see LoKi::Cuts::MCDPHI
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaPhi : public Phi {
    public:
      // ======================================================================
      /// constructor from the angle
      DeltaPhi( const double phi );
      /// constructor from the vector
      DeltaPhi( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaPhi( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaPhi( const LHCb::MCParticle* p );
      // /// templated constructor from vector
      //       template <class VECTOR>
      //       DeltaPhi ( const VECTOR& v )
      //         : LoKi::MCParticles::Phi ()
      //         , m_phi  ( v.Phi() )
      //       {
      //         m_phi = adjust ( m_phi ) ;
      //       } ;
      // /// templated constructor from particle
      //       template <class PARTICLE>
      //       DeltaPhi ( const PARTICLE* p )
      //         : LoKi::MCParticles::Phi ()
      //         , m_phi  (  )
      //       {
      //         if ( 0 == p ) { Exception("Invalid PARTICLE*") ;}
      //         m_phi = p->momentum().Phi() ;
      //         m_phi = adjust ( m_phi ) ;
      //       } ;
      /// MANDATORY: clone method ("virtual constructor")
      DeltaPhi* clone() const override { return new DeltaPhi( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// accessor to phi0
      double phi0() const { return m_phi; }
      /// get delta phi
      double dphi( const LHCb::MCParticle* p ) const { return adjust( phi( p ) - phi0() ); }
      // ======================================================================
    private:
      // ======================================================================
      /// the angle itself
      double m_phi; // the angle itself
      // ======================================================================
    };
    // ========================================================================
    /** @class DeltaEta
     *  Simple evaluator of "DeltaEta" for the particle
     *
     *  @see LoKi::Cuts::DETA
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaEta : public PseudoRapidity {
    public:
      // ======================================================================
      /// constructor from the eta
      DeltaEta( const double eta );
      /// constructor from the vector
      DeltaEta( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaEta( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaEta( const LHCb::MCParticle* p );
      //       /// templated constructor from vector
      //       template <class VECTOR>
      //       DeltaEta ( const VECTOR& v )
      //         : PseudoRapidity ()
      //         , m_eta  ( v.Eta() )
      //       {} ;
      //       /// templated constructor from particle
      //       template <class PARTICLE>
      //       DeltaEta ( const PARTICLE* p )
      //         : PseudoRapidity ()
      //         , m_eta  (  )
      //       {
      //         if ( 0 == p ) { Exception("Invalid PARTICLE*") ;}
      //         m_eta = p->momentum().Eta() ;
      //       } ;
      /// MANDATORY: clone method ("virtual constructor")
      DeltaEta* clone() const override { return new DeltaEta( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      // accessor to eta0
      double eta0() const { return m_eta; }
      double deta( const LHCb::MCParticle* p ) const { return eta( p ) - eta0(); }
      // ======================================================================
    private:
      // ======================================================================
      /// the angle itself
      double m_eta; // the angle itself
      // ======================================================================
    };
    // ========================================================================
    /** @class DeltaR2
     *  Simple evaluator of "DeltaPhi**2+ DeltaEta**2"
     *
     *  @see LoKi::Cuts::MCDR2
     *  @see LoKi::Cuts::MCDELTAR2
     *  @see LoKi::Cuts::MCDPHI
     *  @see LoKi::Cuts::MCDETA
     *  @see LoKi::MCParticles::DeltaPhi
     *  @see LoKi::MCParticles::DeltaEta
     *  @see LoKi::MCParticles::Phi
     *  @see LoKi::MCParticles::PseudoRapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaR2 : public DeltaPhi {
    public:
      // ======================================================================
      /// constructor from eta and phi
      DeltaR2( const double eta, const double phi );
      /// constructor from the vector
      DeltaR2( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaR2( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaR2( const LHCb::MCParticle* p );
      //       /// templated constructor from vector
      //       template <class VECTOR>
      //       DeltaR2 ( const VECTOR& v )
      //         : DeltaPhi ()
      //         , m_deta ( v )
      //       {} ;
      //       /// templated constructor from particle
      //       template <class PARTICLE>
      //       DeltaR2 ( const PARTICLE* p )
      //         : DeltaPhi ()
      //         , m_deta ( p )
      //       {} ;
      /// MANDATORY: clone method ("virtual constructor")
      DeltaR2* clone() const override { return new DeltaR2( *this ); }
      /// MANDATORY: the only essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// accessor to eta0
      double eta0() const { return m_deta.eta0(); }
      /// evaluate delta eta
      double deta( const LHCb::MCParticle* p ) const { return eta( p ) - eta0(); }
      /// get the eta
      double eta( const LHCb::MCParticle* p ) const { return m_deta.eta( p ); }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual evaluator of delta eta
      LoKi::MCParticles::DeltaEta m_deta; // the actual evaluator of delta eta
      // ======================================================================
    };
    // ========================================================================
    /** @class ValidOrigin
     *  Trivial predicat eto test teh valdity of "origin" vertex
     *  for Monte Carlo particle
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    class GAUDI_API ValidOrigin : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// MANDATORY : clone method ("virtual constructor")
      ValidOrigin* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCVertexFunAdapter
     *  simple function adapter which forward the
     *  real evaluation to "originVertex"
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    class GAUDI_API MCVertexFunAdapter : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** constructor from vertex funtion
       *  @param vfunc vertex function
       *  @param err  value to be returned for invalid argument
       */
      MCVertexFunAdapter( const LoKi::MCTypes::MCVFunc& vfunc, const double err = -1000000 );
      /// MANDATORY : clone method ("virtual constructor")
      MCVertexFunAdapter* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCVFun m_fun;
      double                m_err;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCDecayPattern
     *  Simple predicate to test certain decay pattern, using
     *  nice tool IMCDecayFinder, developed by Olivier Dormond
     *
     *  The idea of this predicate belongs to Luis Fernandez
     *
     *  @code
     *
     *   // create the predicate which evaluated to true
     *   // for B0 -> + pi- and all its ancestors
     *   MCCut b2pp = MCDECAY ( "B0 -> pi+ pi-"   ,
     *                          mcDecayFinder () ) ;
     *
     *   // use this predicate :
     *  MCRange B0 = mcselect ( "B0" , b2pp ) ;
     *
     *  @endcode
     *
     *  @attention The "decay" string of IMCDecayFinder tool is redefined!
     *
     *  @see IMCDecayFinder
     *  @see LoKi::Cuts::MCDECAY
     *
     *  @author Vanya BELYAEV Ivan.Belyaev@lapp.in2p3.fr
     *  @date 2005-05-16
     */
    class GAUDI_API MCDecayPattern : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /** constructor
       *  @param decay  decay descriptor
       *  @param finder decay finder tool
       *  @see IMCDecayFinder
       */
      MCDecayPattern( const std::string& decay, IMCDecayFinder* finder );
      /** constructor
       *  @param decay  decay descriptor
       *  @param finder decay finder tool
       *  @see IMCDecayFinder
       */
      MCDecayPattern( const std::string& decay, const LoKi::Interface<IMCDecayFinder>& finder );
      /// MANDATORY: clone method ("virtual constructor")
      MCDecayPattern* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::Interface<IMCDecayFinder> m_finder;
      std::string                     m_decay;
      // ======================================================================
    };
    // ========================================================================
    /** @class MCFilter
     *  The simple class-adapter to IMCParticleSelector tool
     *  @see IMCParticleSelector
     *  @see LoKi::Cuts::MCFILTER
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-03-20
     */
    class GAUDI_API MCFilter : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// constructor from selector
      MCFilter( const IMCParticleSelector* selector );
      /// constructor from selector
      MCFilter( const LoKi::Interface<IMCParticleSelector>& selector );
      /// MANDATORY: clone method ("virtual constructor")
      MCFilter* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// "SHORT" representation, @see LoKi::AuxFunBase
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      // the selector itself
      LoKi::Interface<IMCParticleSelector> m_selector; ///< the selector itself
      // ======================================================================
    };
    // ========================================================================
    /** @class MCReconstructuble
     *  Simple function which evaluates the "MCRecontructible" category
     *  @see IMCRecontructible
     *  @see LoKi::Cuts::MCREC
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-01
     */
    class GAUDI_API MCReconstructible : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /// constructor from the tool
      MCReconstructible( const IMCReconstructible* tool );
      /// constructor from the holder
      MCReconstructible( const LoKi::Interface<IMCReconstructible>& tool );
      /// MANDATORY: clone method ("virtual constructor")
      MCReconstructible* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "short representation"
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// cast operator to the underlying tool
      operator const LoKi::Interface<IMCReconstructible>&() const { return m_eval; }
      // ======================================================================
    private:
      // ======================================================================
      // the underlying tool
      LoKi::Interface<IMCReconstructible> m_eval; ///< the underlying tool
      // ======================================================================
    };
    // ========================================================================
    /** @class MCReconstructubleAs
     *  Simple function which checks
     *     the "MCRecontructible" category
     *  @see IMCRecontructible
     *  @see LoKi::Cuts::MCRECAS
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-01
     */
    class GAUDI_API MCReconstructibleAs : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// constructor from the tool and category
      MCReconstructibleAs( const IMCReconstructible* tool, const IMCReconstructible::RecCategory cat );
      /// constructor from the tool and category
      MCReconstructibleAs( const IMCReconstructible::RecCategory cat, const IMCReconstructible* tool );
      /// constructor from the holder and category
      MCReconstructibleAs( const LoKi::Interface<IMCReconstructible>& tool, const IMCReconstructible::RecCategory cat );
      /// constructor from the holder and category
      MCReconstructibleAs( const IMCReconstructible::RecCategory cat, const LoKi::Interface<IMCReconstructible>& tool );
      /// MAND ATORY: clone method ("virtual constructor")
      MCReconstructibleAs* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: "short representation"
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// cast operator to the underlying tool
      operator const LoKi::Interface<IMCReconstructible>&() const { return m_eval; }
      // ======================================================================
    private:
      // ======================================================================
      /// the underlying tool
      LoKi::Interface<IMCReconstructible> m_eval; // the underlying tool
      /// the recontruction category
      IMCReconstructible::RecCategory m_cat; // the recontruction category
      // ======================================================================
    };
    // ========================================================================
    /** @class ChildFunction
     *  Simple adapter function which
     *  apply the function to a daughter MC-particle
     *
     *  @code
     *
     *  const LHCb::MCParticle* B = ... ;
     *
     *  // the transverse momentum of the first daughter particle:
     *  MCFun fun = MCCHILD( MCPT , 1 ) ;
     *  const double pt1 = fun( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCCHILD
     *  @see LoKi::Child::child
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    class GAUDI_API ChildFunction : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** constructor from the function and child selector
       *  @param fun    the function to be used
       *  @param child  the selector of the proper   child
       *  @param bad    the value to be returned for invalid particle
       */
      ChildFunction( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCChild::Selector& child,
                     const double bad = LoKi::Constants::NegativeInfinity );
      /** constructor from the function and child selector
       *  @param fun    the function to be used
       *  @param child  the selector of the proper   child
       *  @param bad    the value to be returned for invalid particle
       */
      ChildFunction( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& child,
                     const double bad = LoKi::Constants::NegativeInfinity );
      /** constructor from the function and child selector
       *  @param fun    the function to be used
       *  @param child  the selector of the proper   child
       *  @param bad    the value to be returned for invalid particle
       */
      ChildFunction( const LoKi::MCTypes::MCFunc& fun, const std::string& child,
                     const double bad = LoKi::Constants::NegativeInfinity );
      /** constructor from the function and child selector
       *  @param fun    the function to be used
       *  @param child  the selector of the proper   child
       *  @param bad    the value to be returned for invalid particle
       */
      ChildFunction( const LoKi::MCTypes::MCFunc& fun, const Decays::IMCDecay::iTree& child,
                     const double bad = LoKi::Constants::NegativeInfinity );
      /** constructor from the function and child selector
       *  @param fun    the function to be used
       *  @param child  the selector of the proper   child
       *  @param bad    the value to be returned for invalid particle
       */
      ChildFunction( const LoKi::MCTypes::MCFunc& fun, const Decays::iNode& child,
                     const double bad = LoKi::Constants::NegativeInfinity );
      /// MANDATORY: clone method ("virtual constructor")
      ChildFunction* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL:  specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the function itself
      LoKi::MCTypes::MCFun m_fun; // the function itself
      /// the  child selector
      LoKi::MCChild::Selector m_child;
      ///
      double m_bad; // return value for invalid particle
      // ======================================================================
    };
    // ========================================================================
    /** @class ChildPredicate
     *  Simple adapter predicate  which
     *  apply the predicate to a daughter  particle
     *
     *  @code
     *
     *  const LHCb::MCParticle* B = ... ;
     *
     *  // ask if the first daughter is pi+
     *  MCCut cut = MCCHILDCUT( "pi+" == MCID  , 1 ) ;
     *  const bool pion = cut ( B ) ;
     *
     *  @endcode
     *
     *  @see LoKi::Cuts::MCCHILDCUT
     *  @see LoKi::Child::child
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-06-02
     */
    class GAUDI_API ChildPredicate : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /** constructor from the function and child selector
       *  @param cut    the function to be used
       *  @param child  the child selector
       *  @param bad    the return value for invalid particle
       */
      ChildPredicate( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCChild::Selector& child, const bool bad = false );
      /** constructor from the function and child selector
       *  @param cut    the function to be used
       *  @param child  the child selector
       *  @param bad    the return value for invalid particle
       */
      ChildPredicate( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCTypes::MCCuts& child, const bool bad = false );
      /** constructor from the function and child selector
       *  @param cut    the function to be used
       *  @param child  the child selector
       *  @param bad    the return value for invalid particle
       */
      ChildPredicate( const LoKi::MCTypes::MCCuts& cut, const std::string& child, const bool bad = false );
      /** constructor from the function and child selector
       *  @param cut    the function to be used
       *  @param child  the child selector
       *  @param bad    the return value for invalid particle
       */
      ChildPredicate( const LoKi::MCTypes::MCCuts& cut, const Decays::IMCDecay::iTree& child, const bool bad = false );
      /** constructor from the function and child selector
       *  @param cut    the function to be used
       *  @param child  the child selector
       *  @param bad    the return value for invalid particle
       */
      ChildPredicate( const LoKi::MCTypes::MCCuts& cut, const Decays::iNode& child, const bool bad = false );
      /// MANDATORY: clone method ("virtual constructor")
      ChildPredicate* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL:  specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      /// the function itself
      LoKi::MCTypes::MCCut m_cut; // the function itself
      /// the  child selector
      LoKi::MCChild::Selector m_child;
      ///
      bool m_bad; // return value for invalid particle
      // ======================================================================
    };
    // ========================================================================
    /** @class InTree
     *  The trivial predicate which evaluates to true
     *  if there is at least one particle in the decay
     *  tree of the given particle which satisfies the
     *  certain criteria
     *
     *  The function uses the algorithm LoKi::MCAlgs::found
     *
     *  @see LoKi::MCAlgs::found
     *  @see LoKi::Cuts::MCINTREE
     *  @see LHCb::MCParticle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API InTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /** standard constructor
       *  @param cut cut to be checked
       *  @param decayOnly flag to indicate the search through decay products only
       */
      InTree( const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      /// MANDATORY: clone function ("virtual constructor")
      InTree* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      // ======================================================================
    };
    // ========================================================================
    /** @class NinTree
     *  The simple funtion which evaluates the number
     *  of particle in decay tree which satisfies the certan criteria
     *
     *  The function uses the algorithm LoKi::MCAlgs::count_if
     *
     *  @see LoKi::MCAlgs::count_if
     *  @see LoKi::Cuts::MCNINTREE
     *  @see LHCb::MCParticle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API NinTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** standard constructor
       *  @param cut cut to be applied
       *  @param decayOnly search only within decay products
       */
      NinTree( const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false );
      /// MANDATORY: clone function (virtual destructor)
      NinTree* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      // ======================================================================
    };
    // ========================================================================
    /** @class SumTree
     *
     *  The simple function which accumulated the
     *  value of certain function over the decay
     *  tree of the particle for the articles which
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::MCAlgs::accumulate
     *
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cuts::MCSUMTREE
     *  @see LHCb::MCParticle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API SumTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      SumTree( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
               const double res = 0.0 );
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      SumTree( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCTypes::MCFunc& fun, const bool decayOnly = false,
               const double res = 0.0 );
      /// MANDATORY: clone function (virtual destructor)
      SumTree* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCFun m_fun;
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      double               m_res;
      // ======================================================================
    };
    // ========================================================================
    /** @class MultTree
     *
     *  The simple function which accumulated the
     *  value of certain function over the decay
     *  tree of the particle for the articles which
     *  satisfy the certain selection criteria
     *  Accumulation is done throught summation
     *
     *  The function uses the algorithm LoKi::MCAlgs::accumulate
     *
     *  @see LoKi::MCAlgs::accumulate
     *  @see LoKi::Cuts::MCMULTTREE
     *  @see LHCb::MCParticle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MultTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MultTree( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
                const double res = 1.0 );
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MultTree( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCTypes::MCFunc& fun, const bool decayOnly = false,
                const double res = 1.0 );
      /// MANDATORY: clone function (virtual destructor)
      MultTree* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCFun m_fun;
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      double               m_res;
      // ======================================================================
    };
    // ========================================================================
    /** @class MinTree
     *
     *  The trivial algorithm which scans the decay
     *  tree of the particle and searches for the the
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria
     *
     *  The function uses the algorithm LoKi::MCAlgs::min_value
     *
     *  @see LoKi::MCAlgs::min_value
     *  @see LoKi::Cuts::MCMINTREE
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MinTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MinTree( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
               const double res = LoKi::Constants::PositiveInfinity );
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MinTree( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCTypes::MCFunc& fun, const bool decayOnly = false,
               const double res = LoKi::Constants::PositiveInfinity );
      /// MANDATORY: clone function (virtual destructor)
      MinTree* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCFun m_fun;
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      double               m_res;
      // ======================================================================
    };
    // ========================================================================
    /** @class MaxTree
     *
     *  The trivial algorithm which scans the decay
     *  tree of the particle and searches for the the
     *  minimal value for some functions for
     *  particles which satisfy the certain criteria
     *
     *  The function uses the algorithm LoKi::MCAlgs::max_value
     *
     *  @see LoKi::MCAlgs::max_value
     *  @see LoKi::Cuts::MCMAXTREE
     *  @see LHCb::Particle
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2004-05-05
     */
    class GAUDI_API MaxTree : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Function {
    public:
      // ======================================================================
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MaxTree( const LoKi::MCTypes::MCFunc& fun, const LoKi::MCTypes::MCCuts& cut, const bool decayOnly = false,
               const double res = LoKi::Constants::NegativeInfinity );
      /** standard constructor
       *  @param fun the function to be evaluated
       *  @param cut cut to be applied
       *  @param decayOnly flag to traverse only decay tree
       *  @param res initial value
       */
      MaxTree( const LoKi::MCTypes::MCCuts& cut, const LoKi::MCTypes::MCFunc& fun, const bool decayOnly = false,
               const double res = LoKi::Constants::NegativeInfinity );
      /// MANDATORY: clone function (virtual destructor)
      MaxTree* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the specific printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      LoKi::MCTypes::MCFun m_fun;
      LoKi::MCTypes::MCCut m_cut;
      bool                 m_decayOnly;
      double               m_res;
      // ======================================================================
    };
    // ========================================================================
    /** @classFromDecays
     *  simple predicate to check if the particle comes only form 'decays'
     *  using LHCb::MCVertex::MCVertexType
     *  @see LHCb::MCVertex
     *  @see LHCb::MCVertex::MCVertexType
     *  The functor return true, if all valid "mother" vertices
     *  are of type:
     *
     *     - LHCb::MCVertex::DecayVertex and
     *     - LHCb::MCVertex::OscillatedAndDecay
     *     - LHCb::MCVertex::ppCollision
     *
     *  @see LoKi::Cuts::MCFROMDECAYS
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    class GAUDI_API FromDecays : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      FromDecays* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Signal
     *  Is it a SIGNAL particle?
     *  @see LHCb::MCParticle::fromSignal
     *  @see LoKi::Cuts::MCSIGNAL
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    class GAUDI_API Signal : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      Signal* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      inline bool signal( const LHCb::MCParticle* p ) const { return nullptr != p && p->fromSignal(); }
      // ======================================================================
    };
    // ========================================================================
    /** @class FromSignal
     *  Is it from a SIGNAL particle?
     *  @see LHCb::MCParticle::
     *  @see LoKi::Cuts::MCFROMSIGNAL
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    class GAUDI_API FromSignal : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      FromSignal* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    private:
      // ======================================================================
      inline bool signal( const LHCb::MCParticle* p ) const { return nullptr != p && p->fromSignal(); }
      // ======================================================================
    };
    // ========================================================================
    /** @class FromInteractions
     *  simple predicate to check if the particle comes from 'interactions'
     *  using LHCb::MCVertex::MCVertexType
     *
     *  @see LHCb::MCVertex
     *  @see LHCb::MCVertex::MCVertexType
     *
     *  The functor return true, if at least one 'mother' vertex
     *  is not of the type:
     *
     *     - LHCb::MCVertex::DecayVertex and
     *     - LHCb::MCVertex::OscillatedAndDecay
     *     - LHCb::MCVertex::ppCollision
     *
     *  @see LoKi::Cuts::MCFROMXS
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2010-02-18
     */
    class GAUDI_API FromInteractions : public LoKi::BasicFunctors<const LHCb::MCParticle*>::Predicate {
    public:
      // ======================================================================
      /// MANDATORY: clone method ("virtual constructor")
      FromInteractions* clone() const override;
      /// MANDATORY: the only one essential method
      bool operator()( const LHCb::MCParticle* p ) const override;
      /// OPTIONAL: the nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /// soem unique hex-id
    GAUDI_API std::string hex_id( const LHCb::MCParticle* particle );
    /// soem unique hash
    GAUDI_API std::size_t hash( const LHCb::MCParticle* particle );
    /// soem unique hash
    GAUDI_API std::size_t hash( const LHCb::MCVertex* vertex );
    // ========================================================================
  } // namespace MCParticles
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_MCPARTICLES_H
