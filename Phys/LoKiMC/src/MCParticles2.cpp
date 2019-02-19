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
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/SmartIF.h"
#include "GaudiKernel/ToStream.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/IMCDecay.h"
#include "LoKi/Kinematics.h"
#include "LoKi/MCChild.h"
#include "LoKi/MCParticles2.h"
#include "LoKi/Trees.h"
// ============================================================================
/** @file
 *  Implementation file for functirons from file  LoKi/MCParticles2.h
 *  @author Vaya BELYAEV Ivan.Belyaev@nikhef.nl
 *  @date 2010-05-31
 */
namespace LoKi {
  namespace MCParticles {
    // ============================================================================
    // constructor from child-selectors
    // ============================================================================
    PolarizationAngle::PolarizationAngle( const LoKi::MCChild::Selector& daughter,
                                          const LoKi::MCChild::Selector& parent, const bool mother )
        : LoKi::AuxFunBase( std::tie( daughter, parent, mother ) )
        , m_first( daughter )
        , m_second( parent )
        , m_mother( mother ) {}
    // ============================================================================
    // constructor from two trees
    // ============================================================================
    PolarizationAngle::PolarizationAngle( const Decays::IMCDecay::iTree& daughter,
                                          const Decays::IMCDecay::iTree& parent, const bool mother )
        : LoKi::AuxFunBase( std::tie( daughter, parent, mother ) )
        , m_first( daughter )
        , m_second( parent )
        , m_mother( mother ) {}
    // ============================================================================
    // constructor from two nodes
    // ============================================================================
    PolarizationAngle::PolarizationAngle( const Decays::iNode& daughter, const Decays::iNode& parent, const bool mother,
                                          const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( daughter, parent, mother, decayOnly ) )
        , m_first( daughter, decayOnly )
        , m_second( parent, decayOnly )
        , m_mother( mother ) {}
    // ============================================================================
    // constructor from two cuts
    // ============================================================================
    PolarizationAngle::PolarizationAngle( const LoKi::MCTypes::MCCuts& daughter, const LoKi::MCTypes::MCCuts& parent,
                                          const bool mother, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( daughter, parent, mother, decayOnly ) )
        , m_first( daughter, decayOnly )
        , m_second( parent, decayOnly )
        , m_mother( mother ) {}
    // ============================================================================
    // constructor from two trees
    // ============================================================================
    PolarizationAngle::PolarizationAngle( const std::string& daughter, const std::string& parent, const bool mother,
                                          const std::string& factory )
        : LoKi::AuxFunBase( std::tie( daughter, parent, mother, factory ) )
        , m_first( daughter, factory )
        , m_second( parent, factory )
        , m_mother( mother ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    PolarizationAngle* PolarizationAngle::clone() const { return new PolarizationAngle( *this ); }
    // ============================================================================
    // get the proper decay components
    // ==========================================================================
    StatusCode PolarizationAngle::getComponents12( const LHCb::MCParticle*              p,
                                                   Decays::IMCDecay::iTree::Collection& vct ) const {
      //
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL" );
        return StatusCode::FAILURE; //                                 RETURN
      }
      //
      if ( !m_first.valid() ) { m_first.validate(); }
      if ( !m_second.valid() ) { m_second.validate(); }
      //
      Assert( m_first.valid(), "The first  tree is invalid!" );
      Assert( m_second.valid(), "The second tree is invalid!" );
      //
      const LHCb::MCParticle* c1 = m_first.child( p );
      if ( !c1 ) {
        Error( "Invalid first  child : '" + m_first.printOut() + "'" );
        return StatusCode::FAILURE;
      }
      vct.push_back( c1 );
      //
      const LHCb::MCParticle* c2 = m_second.child( p );
      if ( !c2 ) {
        Error( "Invalid second child : '" + m_second.printOut() + "'" );
        return StatusCode::FAILURE;
      }
      vct.push_back( c2 );
      //
      return StatusCode::SUCCESS;
    }
    // ==========================================================================
    // MANDATORY: the only one essential method
    // ==========================================================================
    double PolarizationAngle::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection vct;
      vct.reserve( 2 );
      StatusCode sc = getComponents12( p, vct );

      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'", sc );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 2 != vct.size() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'", sc );
        return LoKi::Constants::InvalidAngle;
      }
      //
      return m_mother ? LoKi::Kinematics::decayAngle( vct[0]->momentum(), vct[1]->momentum(), p->momentum() )
                      : LoKi::Kinematics::decayAngle( vct[0]->momentum(), vct[0]->momentum() + vct[1]->momentum(),
                                                      p->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& PolarizationAngle::fillStream( std::ostream& s ) const {
      s << " MCCOSPOL( " << m_first << " , " << m_second;
      //
      if ( !m_mother ) {
        s << " , ";
        Gaudi::Utils::toStream( m_mother, s );
      }
      //
      return s << " ) ";
    }
    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    SinChi::SinChi( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                    const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , PolarizationAngle( particle1, particle2, true )
        , m_tree3( particle3 )
        , m_tree4( particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    SinChi::SinChi( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                    const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , PolarizationAngle( particle1, particle2, true )
        , m_tree3( particle3 )
        , m_tree4( particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    SinChi::SinChi( const Decays::iNode& particle1, const Decays::iNode& particle2, const Decays::iNode& particle3,
                    const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , PolarizationAngle( particle1, particle2, true, decayOnly )
        , m_tree3( particle3, decayOnly )
        , m_tree4( particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the cuts
    // ============================================================================
    SinChi::SinChi( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                    const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                    const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , PolarizationAngle( particle1, particle2, true, decayOnly )
        , m_tree3( particle3, decayOnly )
        , m_tree4( particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    SinChi::SinChi( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                    const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , PolarizationAngle( particle1, particle2, true, factory )
        , m_tree3( particle3, factory )
        , m_tree4( particle4, factory ) {}
    // ============================================================================
    // get the proper decay components
    // ==========================================================================
    StatusCode SinChi::getComponents34( const LHCb::MCParticle* p, Decays::IMCDecay::iTree::Collection& vct ) const {
      //
      if ( !p ) {
        Error( "LHCb::GenParticle* points to NULL" );
        return StatusCode::FAILURE; //                                 RETURN
      }
      //
      if ( !m_tree3.valid() ) { m_tree3.validate(); }
      if ( !m_tree4.valid() ) { m_tree4.validate(); }
      //
      Assert( m_tree3.valid(), "The third  tree is invalid!" );
      Assert( m_tree4.valid(), "The fourth tree is invalid!" );
      //
      const LHCb::MCParticle* c1 = m_tree3.child( p );
      if ( !c1 ) {
        Error( "Invalid third  child : '" + m_tree3.printOut() + "'" );
        return StatusCode::FAILURE;
      }
      vct.push_back( c1 );
      //
      const LHCb::MCParticle* c2 = m_tree4.child( p );
      if ( !c2 ) {
        Error( "Invalid fourth child : '" + m_tree4.printOut() + "'" );
        return StatusCode::FAILURE;
      }
      vct.push_back( c2 );
      //
      return StatusCode::SUCCESS;
    }
    // ============================================================================
    // get the proper decay components
    // ==========================================================================
    StatusCode SinChi::getComponents( const LHCb::MCParticle* p, Decays::IMCDecay::iTree::Collection& vct ) const {
      vct.clear();
      StatusCode sc1 = getComponents12( p, vct );
      if ( sc1.isFailure() ) {
        Error( "Unable to pickup 12-components", sc1 );
        return sc1;
      }
      //
      StatusCode sc2 = getComponents34( p, vct );
      if ( sc2.isFailure() ) {
        Error( "Unable to pickup 34-components", sc2 );
        return sc2;
      }
      return StatusCode::SUCCESS;
    }
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    SinChi* SinChi::clone() const { return new SinChi( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double SinChi::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the angle chi:
      return LoKi::Kinematics::sinDecayAngleChi( daughters[0]->momentum(), daughters[1]->momentum(),
                                                 daughters[2]->momentum(), daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& SinChi::fillStream( std::ostream& s ) const {
      s << " MCSINCHI( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from selectors
    // ============================================================================
    CosChi::CosChi( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                    const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , SinChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor form the trees
    // ============================================================================
    CosChi::CosChi( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                    const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , SinChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    CosChi::CosChi( const Decays::iNode& particle1, const Decays::iNode& particle2, const Decays::iNode& particle3,
                    const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , SinChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    CosChi::CosChi( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                    const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                    const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , SinChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    CosChi::CosChi( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                    const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , SinChi( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    CosChi* CosChi::clone() const { return new CosChi( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double CosChi::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the angle chi:
      return LoKi::Kinematics::cosDecayAngleChi( daughters[0]->momentum(), daughters[1]->momentum(),
                                                 daughters[2]->momentum(), daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& CosChi::fillStream( std::ostream& s ) const {
      s << " MCCOSCHI( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    AngleChi::AngleChi( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                        const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    AngleChi::AngleChi( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                        const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    AngleChi::AngleChi( const Decays::iNode& particle1, const Decays::iNode& particle2, const Decays::iNode& particle3,
                        const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    AngleChi::AngleChi( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                        const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                        const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    AngleChi::AngleChi( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                        const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , CosChi( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    AngleChi* AngleChi::clone() const { return new AngleChi( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double AngleChi::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the angle chi:
      return LoKi::Kinematics::decayAngleChi( daughters[0]->momentum(), daughters[1]->momentum(),
                                              daughters[2]->momentum(), daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& AngleChi::fillStream( std::ostream& s ) const {
      s << " MCANGLECHI( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    CosThetaTr::CosThetaTr( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                            const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , AngleChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    CosThetaTr::CosThetaTr( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                            const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , AngleChi( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    CosThetaTr::CosThetaTr( const Decays::iNode& particle1, const Decays::iNode& particle2,
                            const Decays::iNode& particle3, const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , AngleChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    CosThetaTr::CosThetaTr( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                            const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                            const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , AngleChi( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    CosThetaTr::CosThetaTr( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                            const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , AngleChi( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    CosThetaTr* CosThetaTr::clone() const { return new CosThetaTr( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double CosThetaTr::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the angle chi:
      return LoKi::Kinematics::cosThetaTr( daughters[0]->momentum(), daughters[1]->momentum(), daughters[2]->momentum(),
                                           daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& CosThetaTr::fillStream( std::ostream& s ) const {
      s << " MCCOSTHETATR( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    SinPhiTr::SinPhiTr( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                        const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosThetaTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    SinPhiTr::SinPhiTr( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                        const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosThetaTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    SinPhiTr::SinPhiTr( const Decays::iNode& particle1, const Decays::iNode& particle2, const Decays::iNode& particle3,
                        const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosThetaTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    SinPhiTr::SinPhiTr( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                        const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                        const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosThetaTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    SinPhiTr::SinPhiTr( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                        const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , CosThetaTr( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    SinPhiTr* SinPhiTr::clone() const { return new SinPhiTr( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double SinPhiTr::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the angle chi:
      return LoKi::Kinematics::sinPhiTr( daughters[0]->momentum(), daughters[1]->momentum(), daughters[2]->momentum(),
                                         daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& SinPhiTr::fillStream( std::ostream& s ) const {
      s << " MCSINPHITR( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    CosPhiTr::CosPhiTr( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                        const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , SinPhiTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    CosPhiTr::CosPhiTr( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                        const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , SinPhiTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    CosPhiTr::CosPhiTr( const Decays::iNode& particle1, const Decays::iNode& particle2, const Decays::iNode& particle3,
                        const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , SinPhiTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    CosPhiTr::CosPhiTr( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                        const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                        const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , SinPhiTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    CosPhiTr::CosPhiTr( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                        const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , SinPhiTr( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    CosPhiTr* CosPhiTr::clone() const { return new CosPhiTr( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double CosPhiTr::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the cos(phi_tr)
      return LoKi::Kinematics::cosPhiTr( daughters[0]->momentum(), daughters[1]->momentum(), daughters[2]->momentum(),
                                         daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& CosPhiTr::fillStream( std::ostream& s ) const {
      s << " MCCOSPHITR( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // constructor from child-selector
    // ============================================================================
    AnglePhiTr::AnglePhiTr( const LoKi::MCChild::Selector& particle1, const LoKi::MCChild::Selector& particle2,
                            const LoKi::MCChild::Selector& particle3, const LoKi::MCChild::Selector& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosPhiTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the trees
    // ============================================================================
    AnglePhiTr::AnglePhiTr( const Decays::IMCDecay::iTree& particle1, const Decays::IMCDecay::iTree& particle2,
                            const Decays::IMCDecay::iTree& particle3, const Decays::IMCDecay::iTree& particle4 )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4 ) )
        , CosPhiTr( particle1, particle2, particle3, particle4 ) {}
    // ============================================================================
    // constructor from the nodes
    // ============================================================================
    AnglePhiTr::AnglePhiTr( const Decays::iNode& particle1, const Decays::iNode& particle2,
                            const Decays::iNode& particle3, const Decays::iNode& particle4, const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosPhiTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor form the cuts
    // ============================================================================
    AnglePhiTr::AnglePhiTr( const LoKi::MCTypes::MCCuts& particle1, const LoKi::MCTypes::MCCuts& particle2,
                            const LoKi::MCTypes::MCCuts& particle3, const LoKi::MCTypes::MCCuts& particle4,
                            const bool decayOnly )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, decayOnly ) )
        , CosPhiTr( particle1, particle2, particle3, particle4, decayOnly ) {}
    // ============================================================================
    // constructor from the decay descriptors
    // ============================================================================
    AnglePhiTr::AnglePhiTr( const std::string& particle1, const std::string& particle2, const std::string& particle3,
                            const std::string& particle4, const std::string& factory )
        : LoKi::AuxFunBase( std::tie( particle1, particle2, particle3, particle4, factory ) )
        , CosPhiTr( particle1, particle2, particle3, particle4, factory ) {}
    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    AnglePhiTr* AnglePhiTr::clone() const { return new AnglePhiTr( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double AnglePhiTr::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "LHCb::MCParticle* points to NULL, return 'Invaild Angle'" );
        return LoKi::Constants::InvalidAngle;
      }
      //
      Decays::IMCDecay::iTree::Collection daughters;
      daughters.reserve( 4 );
      //
      StatusCode sc = getComponents( p, daughters );
      //
      if ( sc.isFailure() ) {
        Error( "Unable to get proper decay components (1), return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      if ( 4 != daughters.size() ) {
        Error( "Unable to get proper decay components (2) , return 'InvalidAngle'" );
        return LoKi::Constants::InvalidAngle;
      }
      // finally evaluate the phi_tr
      return LoKi::Kinematics::anglePhiTr( daughters[0]->momentum(), daughters[1]->momentum(), daughters[2]->momentum(),
                                           daughters[3]->momentum() );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& AnglePhiTr::fillStream( std::ostream& s ) const {
      s << " MCANGLEPHITR( "
        << "'" << child1() << "','" << child2() << "','" << child3() << "','" << child4() << "'";
      //
      return s << " ) ";
    }
    // ============================================================================

  } // namespace MCParticles
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
