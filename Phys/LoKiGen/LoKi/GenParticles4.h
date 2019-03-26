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
#ifndef LOKI_GENPARTICLES4_H
#define LOKI_GENPARTICLES4_H 1
// ============================================================================
// Include files
// ============================================================================
// Event
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/GenChildSelector.h"
#include "LoKi/GenTypes.h"
#include "LoKi/Kinematics.h"
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
 *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
 *  @date 2012-01-28
 *
 */
// ============================================================================
namespace LoKi {
  // ==========================================================================
  namespace GenParticles {
    // ========================================================================
    /** @class FourMomentum
     *  base class for implementation of many function dealing
     *  with the various subcombinations of particles
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date 2012-06-17
     */
    class GAUDI_API FourMomentum : public LoKi::GenTypes::GFunc {
    public:
      // ======================================================================
      /// constructor from one
      FourMomentum( const LoKi::GenChild::Selector& c1 );
      /// constructor from two selectors
      FourMomentum( const LoKi::GenChild::Selector& c1, const LoKi::GenChild::Selector& c2 );
      /// constructor from three selectors
      FourMomentum( const LoKi::GenChild::Selector& c1, const LoKi::GenChild::Selector& c2,
                    const LoKi::GenChild::Selector& c3 );
      /// constructor from four selectors
      FourMomentum( const LoKi::GenChild::Selector& c1, const LoKi::GenChild::Selector& c2,
                    const LoKi::GenChild::Selector& c3, const LoKi::GenChild::Selector& c4 );
      /// contructor from list of selectors
      FourMomentum( const LoKi::GenChild::Selector::Vector& cs );
      // ======================================================================
      /// constructor from one selector
      FourMomentum( const LoKi::GenTypes::GCuts& c1 );
      /// constructor from two selectors
      FourMomentum( const LoKi::GenTypes::GCuts& c1, const LoKi::GenTypes::GCuts& c2 );
      /// constructor from three selectors
      FourMomentum( const LoKi::GenTypes::GCuts& c1, const LoKi::GenTypes::GCuts& c2, const LoKi::GenTypes::GCuts& c3 );
      /// constructor from four selectors
      FourMomentum( const LoKi::GenTypes::GCuts& c1, const LoKi::GenTypes::GCuts& c2, const LoKi::GenTypes::GCuts& c3,
                    const LoKi::GenTypes::GCuts& c4 );
      /// default constructor
      FourMomentum() = default;
      // ======================================================================
    public:
      // ======================================================================
      /// evaluate the full four momentum of combination
      LoKi::LorentzVector momentum( const HepMC::GenParticle* p ) const;
      /**  get the rapidity y
       *  \f$ y = \frac{1}{2}\log \frac{ E + p_z }{ E - p_z } \f$
       */
      double y( const LoKi::LorentzVector& lv ) const;
      /**  get the rapidity y0
       *  \f$ y_0 = \frac{1}{2}\log \frac{ E + p }{ E - p } \f$
       */
      double y0( const LoKi::LorentzVector& lv ) const;
      // get the angle phi
      double phi( const LoKi::LorentzVector& lv ) const;
      // get the pseudorapidity
      double eta( const LoKi::LorentzVector& lv ) const;
      // get beta-factor
      double beta( const LoKi::LorentzVector& lv ) const;
      // get gamma-factor
      double gamma( const LoKi::LorentzVector& lv ) const;
      // get beta*gamma-factor
      double bgamma( const LoKi::LorentzVector& lv ) const;
      // ======================================================================
    protected:
      // ======================================================================
      /// base class printput
      std::ostream& print_( std::ostream& s, const std::string& n1, const std::string& n2 ) const;
      // ======================================================================
    private:
      // ======================================================================
      /// the actual list of selectors
      LoKi::GenChild::Selector::Vector m_children;
      // ======================================================================
    };
    // ========================================================================
    /** @class Mass
     *  evaluator of the 'Mass for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GM
     *  @see LoKi::Cuts::GMASS
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Mass : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Mass* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Energy
     *  evaluator of the 'Energy for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GE
     *  @see LoKi::Cuts::GENERGY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Energy : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructor
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual constructor")
      Energy* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Momentum
     *  evaluator of the 3-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GP
     *  @see LoKi::Cuts::GMOMENTUM
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Momentum : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base class constructor
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Momentum* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumX
     *  evaluator of the x-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPX
     *  @see LoKi::Cuts::GMOMX
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API MomentumX : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      MomentumX* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumY
     *  evaluator of the x-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPY
     *  @see LoKi::Cuts::GMOMY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API MomentumY : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// constructor from one selectors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      MomentumY* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class MomentumZ
     *  evaluator of the x-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPZ
     *  @see LoKi::Cuts::GMOMZ
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API MomentumZ : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      MomentumZ* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class TransverseMomentum
     *  evaluator of the x-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPT
     *  @see LoKi::Cuts::GMOMT
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API TransverseMomentum : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual constructor")
      TransverseMomentum* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class PseudoRapidity
     *  evaluator of the x-momentum for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GETA
     *  @see LoKi::Cuts::GPSEUDORAPIDITY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API PseudoRapidity : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual constructor")
      PseudoRapidity* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Rapidity
     *  evaluator of the rapidity for particle and/or combination of daughters
     *
     *  \f$ y = \frac{1}{2}\log \frac{ E + p_z }{ E - p_z } \f$
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GY
     *  @see LoKi::Cuts::GRAPIDITY
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Rapidity : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Rapidity* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Rapidity0
     *  evaluator of the rapidity0 for particle and/or combination of daughters
     *
     *  \f$ y_0 = \frac{1}{2}\log \frac{ E + p }{ E - p } \f$
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GY0
     *  @see LoKi::Cuts::GRAPIDITY0
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Rapidity0 : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Rapidity0* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Phi
     *  evaluator of the \f$\phi\f$ for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPHI
     *  @see LoKi::Cuts::GASYMUTH
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Phi : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Phi* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Theta
     *  evaluator of the \f$\phi\f$ for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GPHI
     *  @see LoKi::Cuts::GASYMUTH
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2006-01-17
     */
    struct GAUDI_API Theta : LoKi::GenParticles::FourMomentum {
      // ======================================================================
      /// forward to base-class constructors
      using LoKi::GenParticles::FourMomentum::FourMomentum;
      /// MANDATORY: clone method ("virtual destructor")
      Theta* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class Beta
     *  evaluator of the \f$\beta\f$ for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GBETA
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2013-04-14
     */
    struct GAUDI_API Beta : LoKi::GenTypes::GFunc {
      /// MANDATORY: clone method ("virtual destructor")
      Beta* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
      // get beta-factor
      double beta( const HepMC::GenParticle* p ) const;
      // get gamma-factor
      double gamma( const HepMC::GenParticle* p ) const;
      // get beta*gamma-factor
      double betagamma( const HepMC::GenParticle* p ) const;
      // ======================================================================
    };
    // ========================================================================
    /** @class Gamma
     *  evaluator of the \f$\gamma\f$ for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GGAMMA
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2013-04-14
     */
    struct GAUDI_API Gamma : LoKi::GenParticles::Beta {
      // ======================================================================
      /// MANDATORY: clone method ("virtual destructor")
      Gamma* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class BetaGamma
     *  evaluator of the \f$\beta\gamma\f$ for particle and/or combination of daughters
     *
     *  @see HepMC::GenParticle
     *  @see LoKi::Cuts::GBETAGAMMA
     *  @see LoKi::Cuts::GBGAMMA
     *  @see LoKi::Cuts::GBG
     *  @author Vanya BELYAEV Ivan.Belyaev@itep.ru
     *  @date   2013-04-14
     */
    struct GAUDI_API BetaGamma : LoKi::GenParticles::Beta {
      // ======================================================================
      /// MANDATORY: clone method ("virtual destructor")
      BetaGamma* clone() const override;
      /// MANDATORY: the only one essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: nice printout
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    };
    // ========================================================================
    /** @class DeltaPhi
     *  Simple evaluator of "DeltaPhi" for the particle
     *
     *  @see LoKi::Cuts::GDPHI
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaPhi : public LoKi::GenParticles::Phi {
    public:
      // ======================================================================
      /// constructor from the angle
      DeltaPhi( const double phi );
      /// constructor from the vector
      DeltaPhi( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaPhi( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaPhi( const HepMC::GenParticle* p );
      /// MANDATORY: clone method ("virtual constructor")
      DeltaPhi* clone() const override;
      /// MANDATORY: the only essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the phi
      double phi0() const { return m_phi; } // get the phi
      double dphi( const HepMC::GenParticle* p ) const;
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
     *  @see LoKi::Cuts::GDETA
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaEta : public LoKi::GenParticles::PseudoRapidity {
    public:
      // ======================================================================
      /// constructor from the eta
      DeltaEta( const double eta );
      /// constructor from the vector
      DeltaEta( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaEta( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaEta( const HepMC::GenParticle* p );
      /// MANDATORY: clone method ("virtual constructor")
      DeltaEta* clone() const override;
      /// MANDATORY: the only essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
      /// get the eta
      double eta0() const { return m_eta; }
      double deta( const HepMC::GenParticle* p ) const;
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
     *  @see LoKi::Cuts::GDR2
     *  @see LoKi::Cuts::GDELTAR2
     *  @see LoKi::Cuts::GDPHI
     *  @see LoKi::Cuts::GDETA
     *  @see LoKi::GenParticles::DeltaPhi
     *  @see LoKi::GenParticles::DeltaEta
     *  @see LoKi::GenParticles::Phi
     *  @see LoKi::GenParticles::PseudoRapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2007-03-03
     */
    class GAUDI_API DeltaR2 : public LoKi::GenParticles::DeltaPhi {
    public:
      // ======================================================================
      /// constructor from eta & phi
      DeltaR2( const double eta, const double phi );
      /// constructor from the vector
      DeltaR2( const LoKi::ThreeVector& v );
      /// constructor from the vector
      DeltaR2( const LoKi::LorentzVector& v );
      /// constructor from the particle
      DeltaR2( const HepMC::GenParticle* p );
      /// MANDATORY: clone method ("virtual constructor")
      DeltaR2* clone() const override;
      /// MANDATORY: the only essential method
      double operator()( const HepMC::GenParticle* p ) const override;
      /// OPTIONAL: "SHORT" representation
      std::ostream& fillStream( std::ostream& s ) const override;
      // ======================================================================
    public:
      // ======================================================================
      /// get the eta
      double eta0() const { return m_deta.eta0(); }
      double deta( const HepMC::GenParticle* p ) const { return m_deta.deta( p ); }
      // ======================================================================
    private:
      // ======================================================================
      /// the actual evaluator of delta eta
      LoKi::GenParticles::DeltaEta m_deta; // the actual evaluator of delta eta
      // ======================================================================
    };
    // ========================================================================
  } // namespace GenParticles
  // ==========================================================================
  namespace Cuts {
    // ========================================================================
    /** @var GM
     *  evaluator of particle mass
     *
     *  @see LoKi::Cuts::GMASS
     *  @see LoKi::GenParticles::Mass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GM = LoKi::GenParticles::Mass{};
    // ========================================================================
    /** @typedef GMASS
     *  evaluator of particle mass
     *
     *  @see LoKi::Cuts::GM
     *  @see LoKi::GenParticles::Mass
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMASS = LoKi::GenParticles::Mass;
    // ========================================================================
    /** @var GE
     *  evaluator of particle energy
     *
     *  @see LoKi::Cuts::GENERGY
     *  @see LoKi::GenParticles::Energy
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GE = LoKi::GenParticles::Energy{};
    // ========================================================================
    /** @typedef GENERGY
     *  evaluator of particle energy
     *
     *  @see LoKi::Cuts::GE
     *  @see LoKi::GenParticles::Energy
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GENERGY = LoKi::GenParticles::Energy;
    // ========================================================================
    /** @var GP
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GENERGY
     *  @see LoKi::GenParticles::Momentum
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GP = LoKi::GenParticles::Momentum{};
    // ========================================================================
    /** @typedef GMOMENTUM
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GP
     *  @see LoKi::Cuts::GMOMENTUM
     *  @see LoKi::GenParticles::Momentum
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMOMENTUM = LoKi::GenParticles::Momentum;
    // ========================================================================
    /** @var GPX
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GMOMX
     *  @see LoKi::GenParticles::MomentumX
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GPX = LoKi::GenParticles::MomentumX{};
    // ========================================================================
    /** @typedef GMOMX
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GPX
     *  @see LoKi::GenParticles::MomentumX
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMOMX = LoKi::GenParticles::MomentumX;
    // ========================================================================
    /** @var GPY
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GMOMY
     *  @see LoKi::GenParticles::MomentumY
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GPY = LoKi::GenParticles::MomentumY{};
    // ========================================================================
    /** @typedef GMOMY
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GPY
     *  @see LoKi::GenParticles::MomentumY
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMOMY = LoKi::GenParticles::MomentumY;
    // ========================================================================
    /** @var GPZ
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GMOMZ
     *  @see LoKi::GenParticles::MomentumZ
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GPZ = LoKi::GenParticles::MomentumZ{};
    // ========================================================================
    /** @typedef GMOMZ
     *  evaluator of particle momentum
     *
     *  @see LoKi::Cuts::GPZ
     *  @see LoKi::GenParticles::MomentumZ
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMOMZ = LoKi::GenParticles::MomentumZ;
    // ========================================================================
    /** @var GPT
     *  evaluator of particle transverse momentum
     *
     *  @see LoKi::Cuts::GMOMT
     *  @see LoKi::GenParticles::TransverseMomentum
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GPT = LoKi::GenParticles::TransverseMomentum{};
    // ========================================================================
    /** @typedef GMOMT
     *  evaluator of particle transverse momentum
     *
     *  @see LoKi::Cuts::GPZ
     *  @see LoKi::GenParticles::TransverseMomentum
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GMOMT = LoKi::GenParticles::TransverseMomentum;
    // ========================================================================
    /** @var GETA
     *  evaluator of particle pseudorapidity
     *
     *  @see LoKi::Cuts::GPSEUDORAPIDITY
     *  @see LoKi::GenParticles::PseudoRapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GETA = LoKi::GenParticles::PseudoRapidity{};
    // ========================================================================
    /** @typedef GPSEUDORAPIDITY
     *  evaluator of particle transverse momentum
     *
     *  @see LoKi::Cuts::GETA
     *  @see LoKi::GenParticles::PseudoRapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GPSEUDORAPIDITY = LoKi::GenParticles::PseudoRapidity;
    // ========================================================================
    /** @var GY
     *  evaluator of particle rapidity
     *
     *  \f$ y = \frac{1}{2}\log \frac{ E + p_z }{ E - p_z } \f$
     *
     *  @see LoKi::Cuts::GRAPIDITY
     *  @see LoKi::GenParticles::Rapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GY = LoKi::GenParticles::Rapidity{};
    // ========================================================================
    /** @typedef GRAPIDITY
     *  evaluator of particle rapidity
     *
     *  \f$ y = \frac{1}{2}\log \frac{ E + p_z }{ E - p_z } \f$
     *
     *  @see LoKi::Cuts::GY
     *  @see LoKi::GenParticles::Rapidity
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GRAPIDITY = LoKi::GenParticles::Rapidity;
    // ========================================================================
    /** @var GY0
     *  evaluator of particle rapidity0
     *
     *  \f$ y_0 = \frac{1}{2}\log \frac{ E + p }{ E - p } \f$
     *
     *  @see LoKi::Cuts::GRAPIDITY0
     *  @see LoKi::GenParticles::Rapidity0
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GY0 = LoKi::GenParticles::Rapidity0{};
    // ========================================================================
    /** @typedef GRAPIDITY0
     *  evaluator of particle rapidity0
     *
     *  \f$ y_0 = \frac{1}{2}\log \frac{ E + p }{ E - p } \f$
     *
     *  @see LoKi::Cuts::GY0
     *  @see LoKi::GenParticles::Rapidity0
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GRAPIDITY0 = LoKi::GenParticles::Rapidity0;
    // ========================================================================
    /** @var GPHI
     *  evaluator of particle \f$\phi\f$
     *
     *  @see LoKi::Cuts::GASYMUTH
     *  @see LoKi::GenParticles::Phi
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GPHI = LoKi::GenParticles::Phi{};
    // ========================================================================
    /** @typedef GASYMUTH
     *  evaluator of particle \f$\phi\f$
     *
     *  @see LoKi::Cuts::GPHI
     *  @see LoKi::GenParticles::Phi
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    using GASYMUTH = LoKi::GenParticles::Phi;
    // ========================================================================
    /** @var GTHETA
     *  evaluator of particle \f$\theta\f$
     *
     *  @see LoKi::Cuts::GPOLAR
     *  @see LoKi::GenParticles::Theta
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    inline const auto GTHETA = LoKi::GenParticles::Theta{};
    // ========================================================================
    /** @typedef GPOLAR
     *  evaluator of particle \f$\theta\f$
     *
     *  @see LoKi::Cuts::GTHETA
     *  @see LoKi::GenParticles::Theta
     *
     *  @author Vanya BELYAEV ibelyaev@physics.syr.edu
     *  @date 2006-01-18
     */
    typedef LoKi::GenParticles::Theta GPOLAR;
    // ========================================================================
    /** @typedef GDPHI
     *  evaluator of \f$\Delta\phi\f$
     *  @see LoKi::GenParticles::DeltaPhi
     */
    typedef LoKi::GenParticles::DeltaPhi GDPHI;
    // ========================================================================
    /** @typedef GDETA
     *  evaluator of \f$\Delta\eta\f$
     *  @see LoKi::GenParticles::DeltaEta
     */
    typedef LoKi::GenParticles::DeltaEta GDETA;
    // ========================================================================
    /** @typedef GDR2
     *  evaluator of \f$\Delta\eta^2+\Delta\phi^2\f$
     *  @see LoKi::GenParticles::DeltaR2
     */
    typedef LoKi::GenParticles::DeltaR2 GDR2;
    // ========================================================================
    /** @var GBETA
     *  get \f$\beta\f$ for the particle
     *  @see LoKi::GenParticles::Beta
     */
    inline const auto GBETA = LoKi::GenParticles::Beta{};
    // ========================================================================
    /** @var GGAMMA
     *  get \f$\gamma\f$ for the particle
     *  @see LoKi::GenParticles::Gamma
     */
    inline const auto GGAMMA = LoKi::GenParticles::Gamma{};
    // ========================================================================
    /** @var GBETAGAMMA
     *  get \f$\beta\gamma\f$ for the particle
     *  @see LoKi::GenParticles::BetaGamma
     */
    inline const auto GBETAGAMMA = LoKi::GenParticles::BetaGamma{};
    // ========================================================================
    /** @var GBGAMMA
     *  get \f$\beta\gamma\f$ for the particle
     *  @see LoKi::GenParticles::BetaGamma
     */
    inline const auto GBGAMMA = LoKi::GenParticles::BetaGamma{};
    // ========================================================================
    /** @var GBG
     *  get \f$\beta\gamma\f$ for the particle
     *  @see LoKi::GenParticles::BetaGamma
     */
    inline const auto GBG = LoKi::GenParticles::BetaGamma{};
    // ========================================================================
  } // namespace Cuts
  // ==========================================================================
} //                                                      end of namespace LoKi
// ============================================================================
//                                                                      The END
// ============================================================================
#endif // LOKI_GENPARTICLES4_H
