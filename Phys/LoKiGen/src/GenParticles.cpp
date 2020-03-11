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
#include <cmath>
// ============================================================================
// GaudiKernel
// ============================================================================
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/SmartIF.h"
// ============================================================================
// PartProp
// ============================================================================
#include "Kernel/IParticlePropertySvc.h"
#include "Kernel/Nodes.h"
// ============================================================================
// LoKi
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/ParticleProperties.h"
#include "LoKi/Services.h"
#include "LoKi/Trees.h"
#include "LoKi/valid.h"
// this must be included after the others
#include "LoKi/MoreFunctions.h"
// ============================================================================
// LoKiGen
// ============================================================================
#include "LoKi/GenAlgs.h"
#include "LoKi/GenExtract.h"
#include "LoKi/GenOscillated.h"
#include "LoKi/GenParticles.h"
#include "LoKi/IGenDecay.h"
// ============================================================================
#include "LoKi/CmpBarCode.h"
// ============================================================================
// Boost
// ============================================================================
#include "boost/format.hpp"
// ============================================================================
/** @file
 *
 *  Implementation of the functions form namespace LoKi::GenParticles
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
namespace LoKi {
  namespace GenParticles {
    // ============================================================================
    //  clone method (mandatory!)
    // ============================================================================
    BarCode* BarCode::clone() const { return new LoKi::GenParticles::BarCode( *this ); }
    // ============================================================================
    //  the only one essential method
    // ============================================================================
    double BarCode::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle, return 0 " ).ignore();
        return 0; // RETURN
      }
      return p->barcode(); // RETURN
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& BarCode::fillStream( std::ostream& s ) const { return s << "GBAR"; }
    // ============================================================================
    //  MANDATORY: clone method ("virtual" constructor")
    // ============================================================================
    Identifier* Identifier::clone() const { return new Identifier( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double Identifier::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "HepMC::GenParticle* points to NULL, return InvalidID" ).ignore();
        return LoKi::Constants::InvalidID;
      }
      return p->pdg_id();
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& Identifier::fillStream( std::ostream& s ) const { return s << "GID"; }
    // ============================================================================
    //  MANDATORY: clone method ("virtual" constructor")
    // ============================================================================
    Status* Status::clone() const { return new Status( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double Status::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "HepMC::GenParticle* points to NULL, return -1000 " ).ignore();
        return -1000;
      }
      return p->status();
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& Status::fillStream( std::ostream& s ) const { return s << "GSTATUS"; }
    // ============================================================================

    // ============================================================================
    //  MANDATORY: clone method ("virtual" constructor")
    // ============================================================================
    AbsIdentifier* AbsIdentifier::clone() const { return new LoKi::GenParticles::AbsIdentifier( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double AbsIdentifier::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "HepMC::GenParticle* points to NULL, return InvalidID" ).ignore();
        return LoKi::Constants::InvalidID;
      }
      return std::abs( p->pdg_id() );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& AbsIdentifier::fillStream( std::ostream& s ) const { return s << "GABSID"; }
    // ============================================================================

    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    ValidEndVertex* ValidEndVertex::clone() const { return new LoKi::GenParticles::ValidEndVertex( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    bool ValidEndVertex::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "HepMC::GenParticle* points to NULL, return 'false'" ).ignore();
        return false;
      }
      return LoKi::valid( p->end_vertex() );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& ValidEndVertex::fillStream( std::ostream& s ) const { return s << "GVEV"; }
    // ============================================================================
    //  constructor
    // ============================================================================
    MomentumDistance::MomentumDistance( const double px, const double py, const double pz, const double e )
        : LoKi::AuxFunBase( std::tie( px, py, pz, e ) ) {
      m_vct.SetXYZT( px, py, pz, e );
    }
    // ============================================================================
    /*  constructor
     *  @param vct the reference 4-momentum
     */
    // ============================================================================
    MomentumDistance::MomentumDistance( const LoKi::LorentzVector& vct )
        : LoKi::AuxFunBase( std::tie( vct ) ), m_vct( vct ) {}
    // ============================================================================
    //  MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    MomentumDistance* MomentumDistance::clone() const { return new LoKi::GenParticles::MomentumDistance( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double MomentumDistance::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle*, return (InvalidMomentum)^2" ).ignore();
        return LoKi::Constants::InvalidMomentum * LoKi::Constants::InvalidMomentum;
      }
      const double de = ( p->momentum().e() - m_vct.e() );
      const double dx = ( p->momentum().x() - m_vct.x() );
      const double dy = ( p->momentum().y() - m_vct.y() );
      const double dz = ( p->momentum().z() - m_vct.z() );
      return de * de + dx * dx + dy * dy + dz * dz;
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& MomentumDistance::fillStream( std::ostream& s ) const {
      return s << "GMOMDIST(" << m_vct.Px() << "," << m_vct.Py() << "," << m_vct.Pz() << "," << m_vct.E() << ")";
    }
    // ============================================================================
    /*  constructor from theta and phi
     *  @param theta theta angle for the direction
     *  @param phi phi angle for the direction
     */
    // ============================================================================
    TransverseMomentumRel::TransverseMomentumRel( const double theta, const double phi )
        : LoKi::AuxFunBase( std::tie( theta, phi ) ) {
      m_vct.SetXYZ( ::sin( theta ) * ::cos( phi ), ::sin( theta ) * ::sin( phi ), ::cos( theta ) );
    }
    // ============================================================================
    /*  constructor from x,y,z
     *  @param x x-component of the direction vector
     *  @param y y-component of the direction vector
     *  @param z z-component of the direction vector
     */
    // ============================================================================
    TransverseMomentumRel::TransverseMomentumRel( const double x, const double y, const double z )
        : LoKi::AuxFunBase( std::tie( x, y, z ) ) {
      m_vct.SetXYZ( x, y, z );
    }
    // ============================================================================
    /*  constructor
     *  @param vct direction vector
     *  @see LoKi::ThreeVector
     */
    // ============================================================================
    TransverseMomentumRel::TransverseMomentumRel( const LoKi::ThreeVector& vct )
        : LoKi::AuxFunBase( std::tie( vct ) ), m_vct( vct ) {}
    // ============================================================================
    /*  constructor
     *  @param vct direction vector
     *  @see LoKi::LorentzVector
     */
    // ============================================================================
    TransverseMomentumRel::TransverseMomentumRel( const LoKi::LorentzVector& vct )
        : LoKi::AuxFunBase( std::tie( vct ) ), m_vct( vct.Vect() ) {}
    // ============================================================================
    //  MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    TransverseMomentumRel* TransverseMomentumRel::clone() const {
      return new LoKi::GenParticles::TransverseMomentumRel( *this );
    }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double TransverseMomentumRel::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle*, return 'InvalidMomentum'" ).ignore();
        return LoKi::Constants::InvalidMomentum;
      }
      const double            b2 = m_vct.mag2();
      const LoKi::ThreeVector va( p->momentum().x(), p->momentum().y(), p->momentum().z() );
      if ( 0 == b2 ) { return ::sqrt( va.mag2() ); }
      const double            ab = va.Dot( m_vct ) / b2;
      const LoKi::ThreeVector vd = va - m_vct * ab;
      return ::sqrt( vd.mag2() );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& TransverseMomentumRel::fillStream( std::ostream& s ) const {
      return s << "GPTDIR(" << m_vct.Theta() << "," << m_vct.Phi() << ")";
    }
    // ============================================================================
    /*  constructor from particle ("head")
     *  @param p pointer to HepMC::GenParticle
     */
    // ============================================================================
    FromHepMCTree::FromHepMCTree( const HepMC::GenParticle* p ) { _add( p ); }
    // ============================================================================
    //  constructor from range of particles
    // ============================================================================
    FromHepMCTree::FromHepMCTree( const LoKi::Types::GRange& r ) { _add( r.begin(), r.end() ); }
    // ============================================================================
    //  constructor from range of particles
    // ============================================================================
    FromHepMCTree::FromHepMCTree( const LoKi::GenTypes::GenContainer& r ) { _add( r.begin(), r.end() ); }
    // ============================================================================
    /*  constructor from vertex  ("head")
     *  @param v pointer to HepMC::GenParticle
     */
    // ============================================================================
    FromHepMCTree::FromHepMCTree( const HepMC::GenVertex* v ) { _add( v ); }
    // ============================================================================
    void FromHepMCTree::_add( const HepMC::GenParticle* p ) {
      if ( 0 != p ) { _add( p->end_vertex() ); }
    }
    void FromHepMCTree::_add( const HepMC::GenVertex* v ) {
      if ( v ) { m_vertices.push_back( const_cast<HepMC::GenVertex*>( v ) ); }
    }
    // ============================================================================
    FromHepMCTree& FromHepMCTree::remove( const HepMC::GenVertex* vertex ) {
      auto ifind = std::remove( m_vertices.begin(), m_vertices.end(), vertex );
      m_vertices.erase( ifind, m_vertices.end() );
      return *this;
    }
    // ============================================================================
    FromHepMCTree& FromHepMCTree::remove( const HepMC::GenParticle* particle ) {
      if ( 0 == particle ) { return *this; }
      return remove( particle->end_vertex() );
    }
    // ============================================================================
    //  MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    FromHepMCTree* FromHepMCTree::clone() const { return new LoKi::GenParticles::FromHepMCTree( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    bool FromHepMCTree::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( " HepMC::GenParticle* points to NULL, return 'false' " ).ignore();
        return false; // RETURN
      }
      if ( m_vertices.empty() ) {
        // Error ( " empty list of vertices , return 'false' " ) ;
        return false; // RETURN
      }
      // loop over vertices
      for ( auto* vertex : m_vertices ) {
        if ( 0 == vertex ) {
          Warning( "NULL vertex is detected, skip" ).ignore();
          continue;
        }
        // explict loop over all descendants
        auto begin = vertex->particles_begin( HepMC::descendants );
        auto end   = vertex->particles_end( HepMC::descendants );
        if ( std::find( begin, end, p ) != end ) { return true; } // RETURN
        //
      }
      return false; // RETURN
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& FromHepMCTree::fillStream( std::ostream& s ) const { return s << "GFROMTREE"; }
    // ============================================================================
    /*  constructor from particle ("head")
     *  @param p pointer to HepMC::GenParticle
     */
    // ============================================================================
    IsAnAncestor::IsAnAncestor( const HepMC::GenParticle* p ) : m_particle( p ) {}
    // ============================================================================
    //  MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    IsAnAncestor* IsAnAncestor::clone() const { return new LoKi::GenParticles::IsAnAncestor( *this ); }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    bool IsAnAncestor::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p || 0 == m_particle ) {
        Error( " HepMC::GenParticle* points to NULL, return 'false' " ).ignore();
        return false; // RETURN
      }
      HepMC::GenVertex* vertex = p->end_vertex();
      if ( 0 == vertex ) { return false; } // RETURN
      // explict loop over all descendants
      for ( auto ip = vertex->particles_begin( HepMC::descendants ); vertex->particles_end( HepMC::descendants ) != ip;
            ++ip ) {
        if ( m_particle == ( *ip ) ) { return true; }
      } // RETURN

      return false;
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& IsAnAncestor::fillStream( std::ostream& s ) const { return s << "GANCESTOR"; }
    // ============================================================================
    //  constructor from Quark
    // ============================================================================
    HasQuark::HasQuark( const LHCb::ParticleID::Quark quark )
        : LoKi::AuxFunBase( std::tie( quark ) ), m_quark( quark ) {}
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool HasQuark::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).hasQuark( quark() );
    }
    // ============================================================================
    HasQuark* HasQuark::clone() const { return new LoKi::GenParticles::HasQuark( *this ); }
    // ============================================================================
    std::ostream& HasQuark::fillStream( std::ostream& s ) const {
      switch ( quark() ) {
      case LHCb::ParticleID::top:
        return s << "GTOP";
        break;
      case LHCb::ParticleID::bottom:
        return s << "GBEAUTY";
        break;
      case LHCb::ParticleID::charm:
        return s << "GCHARM";
        break;
      case LHCb::ParticleID::strange:
        return s << "GSTRANGE";
        break;
      default:
        return s << "GQUARK(" << quark() << ")";
        break;
      }
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsCharged::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return 0 != LHCb::ParticleID( p->pdg_id() ).threeCharge();
    }
    // ============================================================================
    IsCharged* IsCharged::clone() const { return new LoKi::GenParticles::IsCharged( *this ); }
    // ============================================================================
    std::ostream& IsCharged::fillStream( std::ostream& s ) const { return s << "GCHARGED"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsNeutral::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return 0 == LHCb::ParticleID( p->pdg_id() ).threeCharge();
    }
    // ============================================================================
    IsNeutral* IsNeutral::clone() const { return new LoKi::GenParticles::IsNeutral( *this ); }
    // ============================================================================
    std::ostream& IsNeutral::fillStream( std::ostream& s ) const { return s << "GNEUTRAL"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsLepton::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).isLepton();
    }
    // ============================================================================
    IsLepton* IsLepton::clone() const { return new LoKi::GenParticles::IsLepton( *this ); }
    // ============================================================================
    std::ostream& IsLepton::fillStream( std::ostream& s ) const { return s << "GLEPTON"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsMeson::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).isMeson();
    }
    // ============================================================================
    IsMeson* IsMeson::clone() const { return new LoKi::GenParticles::IsMeson( *this ); }
    // ============================================================================
    std::ostream& IsMeson::fillStream( std::ostream& s ) const { return s << "GMESON"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsBaryon::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).isBaryon();
    }
    // ============================================================================
    IsBaryon* IsBaryon::clone() const { return new LoKi::GenParticles::IsBaryon( *this ); }
    // ============================================================================
    std::ostream& IsBaryon::fillStream( std::ostream& s ) const { return s << "GBARYON"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsHadron::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).isHadron();
    }
    // ============================================================================
    IsHadron* IsHadron::clone() const { return new LoKi::GenParticles::IsHadron( *this ); }
    // ============================================================================
    std::ostream& IsHadron::fillStream( std::ostream& s ) const { return s << "GHADRON"; }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsNucleus::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, returs 'false'" ).ignore();
        return false;
      }
      return LHCb::ParticleID( p->pdg_id() ).isNucleus();
    }
    // ============================================================================
    IsNucleus* IsNucleus::clone() const { return new LoKi::GenParticles::IsNucleus( *this ); }
    // ============================================================================
    std::ostream& IsNucleus::fillStream( std::ostream& s ) const { return s << "GNUCLEUS"; }
    // ============================================================================
    ProperLifeTime::ProperLifeTime( const double bad ) : LoKi::AuxFunBase( std::tie( bad ) ), m_bad( bad ) {}
    // ============================================================================
    ProperLifeTime* ProperLifeTime::clone() const { return new LoKi::GenParticles::ProperLifeTime( *this ); }
    // ============================================================================
    std::ostream& ProperLifeTime::fillStream( std::ostream& s ) const { return s << "GTIME"; }
    // ============================================================================
    double ProperLifeTime::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( " Invalid HepMC::GenParticle, return 'InvalidTime'" ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      // end-vertex
      const HepMC::GenVertex* end = p->end_vertex();
      if ( 0 == end ) { return m_bad; }
      // production vertex
      const HepMC::GenVertex* origin = p->production_vertex();
      if ( 0 == origin ) {
        Error( " Invalid production_vertex, return 'InvalidTime'" ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      const double dist = ( LoKi::ThreeVector( end->position() ) - LoKi::ThreeVector( origin->position() ) ).R();
      const double mass = LoKi::LorentzVector( p->momentum() ).M();
      const double mom  = LoKi::LorentzVector( p->momentum() ).P();
      if ( 0 >= mass ) {
        Error( " Mass is invalid, return 'InvalidTime' " ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      if ( 0 >= mom ) {
        Error( " |Momentum| is invalid, return 'InvalidTime' " ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      const double betagamma = mom / mass;
      //
      if ( 0 >= betagamma ) {
        Error( " betagamma is invalid, return 'InvalidTime' " ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      //
      return dist / betagamma;
    }
    // ============================================================================
    NominalLifeTime* NominalLifeTime::clone() const { return new LoKi::GenParticles::NominalLifeTime( *this ); }
    // ============================================================================
    double NominalLifeTime::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( " Invalid HepMC::GenParticle, return 'InvalidTime'" ).ignore();
        return LoKi::Constants::InvalidTime; // RETURN
      }
      return LoKi::Particles::lifeTime( LHCb::ParticleID( p->pdg_id() ) );
    }
    // ============================================================================
    std::ostream& NominalLifeTime::fillStream( std::ostream& s ) const { return s << "GNLT"; }
    // ============================================================================
    /*  constructor from vertex function and "bad" value
     *  @param fun verted function to be used
     *  @param bad the value to be returnedd for invalid vertex
     */
    // ============================================================================
    AdapterToProductionVertex::AdapterToProductionVertex( const LoKi::Types::GVFunc& fun, const double bad )
        : LoKi::AuxFunBase( std::tie( fun, bad ) ), m_fun( fun ), m_bad( bad ) {}
    // ============================================================================
    //  clone method (mandatory!)
    // ============================================================================
    AdapterToProductionVertex* AdapterToProductionVertex::clone() const {
      return new AdapterToProductionVertex( *this );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& AdapterToProductionVertex::fillStream( std::ostream& stream ) const {
      return stream << "GFAPVX(" << m_fun << ")";
    }
    // ============================================================================
    //  the only one essential method
    // ============================================================================
    double AdapterToProductionVertex::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) { return m_bad; }
      HepMC::GenVertex* v = p->production_vertex();
      if ( 0 == v ) { return m_bad; }
      return m_fun( v );
    }
    // ============================================================================
    /*  constructor from vertex function and "bad" value
     *  @param fun verted function to be used
     *  @param bad the value to be returnedd for invalid vertex
     */
    // ============================================================================
    AdapterToEndVertex::AdapterToEndVertex( const LoKi::Types::GVFunc& fun, const double bad )
        : LoKi::AuxFunBase( std::tie( fun, bad ) ), m_fun( fun ), m_bad( bad ) {}
    // ============================================================================
    //  clone method (mandatory!)
    // ============================================================================
    AdapterToEndVertex* AdapterToEndVertex::clone() const { return new AdapterToEndVertex( *this ); }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& AdapterToEndVertex::fillStream( std::ostream& stream ) const {
      return stream << "GFAEVX(" << m_fun << ")";
    }
    // ============================================================================
    //  the only one essential method
    // ============================================================================
    double AdapterToEndVertex::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) { return m_bad; }
      HepMC::GenVertex* v = p->end_vertex();
      if ( 0 == v ) { return m_bad; }
      return m_fun( v );
    }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double ThreeCharge::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( " Invalid Particle, return 'InvalidCharge'" ).ignore();
        return LoKi::Constants::InvalidCharge; // RETURN
      }
      return LHCb::ParticleID( p->pdg_id() ).threeCharge();
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& ThreeCharge::fillStream( std::ostream& stream ) const { return stream << "G3Q"; }

    // ============================================================================
    /*  constructor
     *  @param cut    predicate to be used for counting
     *  @param range  "iterator range", @see HepMC::IteratorRange
     *  @see HepMC::IteratorRange
     */
    // ============================================================================
    NInTree::NInTree( const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range )
        : LoKi::AuxFunBase( std::tie( cut, range ) ), m_cut( cut ), m_range( range ) {}
    // ============================================================================
    //  MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    NInTree* NInTree::clone() const { return new LoKi::GenParticles::NInTree( *this ); }
    // ============================================================================
    /*  count the particles in the tree according
     *  the predicat eand iterator range
     *  @see HepMC::GenVertex
     *  @param vertex  root of the tree
     *  @return number of particles
     */
    // ============================================================================
    size_t NInTree::count( HepMC::GenVertex* vertex ) const {
      if ( 0 == vertex ) { return 0; }
      return std::count_if( vertex->particles_begin( m_range ), vertex->particles_end( m_range ), m_cut );
    }
    // ============================================================================
    //  MANDATORY: the only one essential method
    // ============================================================================
    double NInTree::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* points to NULL, return 0" ).ignore();
        return 0; // RETURN
      };
      if ( HepMC::parents == m_range || HepMC::ancestors == m_range ) {
        return count( p->production_vertex() );
      } else if ( HepMC::children == m_range || HepMC::descendants == m_range ) {
        return count( p->end_vertex() );
      }
      //
      Error( "Unimplemented case: HepMC::IteratorRange, return 0 ;" ).ignore();
      //
      return 0;
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& NInTree::fillStream( std::ostream& s ) const {
      return s << "GNINTREE(" << m_cut << "," << m_range << ")";
    }
    // ============================================================================

    // ============================================================================
    /*  constructor
     *  @param fun    function to be evaluated
     *  @param cut    predicate to be used for counting
     *  @param range  "iterator range", see HepMC::IteratorRange
     *  @see HepMC::IteratorRange
     */
    // ============================================================================
    MaxTree::MaxTree( const LoKi::GenTypes::GFunc& fun, const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range )
        : LoKi::AuxFunBase( std::tie( fun, cut, range ) ), NInTree( cut, range ), m_fun( fun ) {}
    // ============================================================================
    // MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    MaxTree* MaxTree::clone() const { return new MaxTree( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double MaxTree::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      HepMC::GenVertex* vertex =
          ( HepMC::parents == m_range || HepMC::ancestors == m_range )
              ? p->production_vertex()
              : ( HepMC::children == m_range || HepMC::descendants == m_range ) ? p->end_vertex() : nullptr;
      //
      if ( UNLIKELY( !vertex ) ) {
        Error( "Invalid HepMC::GenVertex/range, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      double result = LoKi::Constants::NegativeInfinity;
      //
      return LoKi::Algs::max_value( vertex->particles_begin( m_range ), vertex->particles_end( m_range ), m_fun, m_cut,
                                    result );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& MaxTree::fillStream( std::ostream& s ) const {
      return s << "GMAXTREE(" << m_fun << "," << m_cut << "," << m_range << ")";
    }
    // ============================================================================

    // ============================================================================
    /*  constructor
     *  @param fun    function to be evaluated
     *  @param cut    predicate to be used for counting
     *  @param range  "iterator range", see HepMC::IteratorRange
     *  @see HepMC::IteratorRange
     */
    // ============================================================================
    MinTree::MinTree( const LoKi::GenTypes::GFunc& fun, const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range )
        : LoKi::AuxFunBase( std::tie( fun, cut, range ) ), MaxTree( fun, cut, range ) {}
    // ============================================================================
    // MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    MinTree* MinTree::clone() const { return new MinTree( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double MinTree::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle, return +inf" ).ignore();
        return LoKi::Constants::PositiveInfinity; // RETURN
      }
      //
      HepMC::GenVertex* vertex =
          ( HepMC::parents == m_range || HepMC::ancestors == m_range )
              ? p->production_vertex()
              : ( HepMC::children == m_range || HepMC::descendants == m_range ) ? p->end_vertex() : nullptr;
      //
      if ( UNLIKELY( !vertex ) ) {
        Error( "Invalid HepMC::GenVertex/range, return +inf" ).ignore();
        return LoKi::Constants::PositiveInfinity; // RETURN
      }
      //
      double result = LoKi::Constants::PositiveInfinity;
      //
      return LoKi::Algs::min_value( vertex->particles_begin( m_range ), vertex->particles_end( m_range ), m_fun, m_cut,
                                    result );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& MinTree::fillStream( std::ostream& s ) const {
      return s << "GMINTREE(" << m_fun << "," << m_cut << "," << m_range << ")";
    }
    // ============================================================================

    // ============================================================================
    /*  constructor
     *  @param fun    function to be evaluated
     *  @param cut    predicate to be used for counting
     *  @param range  "iterator range", see HepMC::IteratorRange
     *  @see HepMC::IteratorRange
     */
    // ============================================================================
    SumTree::SumTree( const LoKi::GenTypes::GFunc& fun, const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range )
        : LoKi::AuxFunBase( std::tie( fun, cut, range ) ), MinTree( fun, cut, range ) {}
    // ============================================================================
    // MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    SumTree* SumTree::clone() const { return new SumTree( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double SumTree::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      HepMC::GenVertex* vertex =
          ( HepMC::parents == m_range || HepMC::ancestors == m_range )
              ? p->production_vertex()
              : ( HepMC::children == m_range || HepMC::descendants == m_range ) ? p->end_vertex() : nullptr;
      //
      if ( UNLIKELY( !vertex ) ) {
        Error( "Invalid HepMC::GenVertex/range, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      double result = 0;
      //
      return LoKi::Algs::accumulate( vertex->particles_begin( m_range ), vertex->particles_end( m_range ), m_fun, m_cut,
                                     result, std::plus<double>() );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& SumTree::fillStream( std::ostream& s ) const {
      return s << "GSUMTREE(" << m_fun << "," << m_cut << "," << m_range << ")";
    }
    // ============================================================================

    // ============================================================================
    /*  constructor
     *  @param fun    function to be evaluated
     *  @param cut    predicate to be used for counting
     *  @param range  "iterator range", see HepMC::IteratorRange
     *  @see HepMC::IteratorRange
     */
    // ============================================================================
    MultTree::MultTree( const LoKi::GenTypes::GFunc& fun, const LoKi::GenTypes::GCuts& cut, HepMC::IteratorRange range )
        : LoKi::AuxFunBase( std::tie( fun, cut, range ) ), SumTree( fun, cut, range ) {}
    // ============================================================================
    // MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    MultTree* MultTree::clone() const { return new MultTree( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double MultTree::operator()( const HepMC::GenParticle* p ) const {
      if ( UNLIKELY( !p ) ) {
        Error( "Invalid HepMC::GenParticle, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      HepMC::GenVertex* vertex =
          ( HepMC::parents == m_range || HepMC::ancestors == m_range )
              ? p->production_vertex()
              : ( HepMC::children == m_range || HepMC::descendants == m_range ) ? p->end_vertex() : nullptr;
      //
      if ( UNLIKELY( !vertex ) ) {
        Error( "Invalid HepMC::GenVertex/range, return -inf" ).ignore();
        return LoKi::Constants::NegativeInfinity; // RETURN
      }
      //
      double result = 1.0;
      //
      return LoKi::Algs::accumulate( vertex->particles_begin( m_range ), vertex->particles_end( m_range ), m_fun, m_cut,
                                     result, std::multiplies<double>() );
    }
    // ============================================================================
    //  "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& MultTree::fillStream( std::ostream& s ) const {
      return s << "GMULTTREE(" << m_fun << "," << m_cut << "," << m_range << ")";
    }
    // ============================================================================

    // ============================================================================
    /*  standard constructor
     *  @param cut cut to be checked
     */
    // ============================================================================
    InTree::InTree( const LoKi::GenTypes::GCuts& cut )
        : LoKi::AuxFunBase( std::tie( cut ) ), LoKi::GenTypes::GCuts(), m_cut( cut ) {}
    // ============================================================================
    // MANDATORY: clone function ("virtual constructor")
    // ============================================================================
    InTree* InTree::clone() const { return new LoKi::GenParticles::InTree( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool InTree::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Warning( "HepMC::GenParticle* points to NULL" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      return LoKi::GenAlgs::found( p, m_cut );
    }
    // ============================================================================
    //  OPTIONAL: "SHORT" representation, @see LoKi::AuxFunBase
    // ============================================================================
    std::ostream& InTree::fillStream( std::ostream& s ) const { return s << "GINTREE(" << m_cut << ")"; }
    // ============================================================================

    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    Oscillated* Oscillated::clone() const { return new LoKi::GenParticles::Oscillated( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool Oscillated::operator()( const HepMC::GenParticle* p ) const {
      return 0 != LoKi::GenParticles::oscillated1( p ) || 0 != LoKi::GenParticles::oscillated2( p );
    }
    // ============================================================================
    // OPTIONAL: the specific printout
    // ============================================================================
    std::ostream& Oscillated::fillStream( std::ostream& o ) const { return o << "GOSCILLATED"; }
    // ============================================================================

    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    Oscillated1* Oscillated1::clone() const { return new LoKi::GenParticles::Oscillated1( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool Oscillated1::operator()( const HepMC::GenParticle* p ) const {
      return 0 != LoKi::GenParticles::oscillated1( p );
    }
    // ============================================================================
    // OPTIONAL: the specific printout
    // ============================================================================
    std::ostream& Oscillated1::fillStream( std::ostream& o ) const { return o << "GOSCILLATED1"; }
    // ============================================================================

    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    Oscillated2* Oscillated2::clone() const { return new Oscillated2( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool Oscillated2::operator()( const HepMC::GenParticle* p ) const {
      return 0 != LoKi::GenParticles::oscillated2( p );
    }
    // ============================================================================
    // OPTIONAL: the specific printout
    // ============================================================================
    std::ostream& Oscillated2::fillStream( std::ostream& o ) const { return o << "GOSCILLATED2"; }
    // ============================================================================

    // ============================================================================
    // constructor from the actual node
    // ============================================================================
    DecNode::DecNode( const Decays::iNode& node )
        : LoKi::AuxFunBase( std::tie( node ) )
        , LoKi::BasicFunctors<const HepMC::GenParticle*>::Predicate()
        , m_node( node ) {}
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool DecNode::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, return false" ).ignore();
        return false;
      }
      //
      if ( !m_node.valid() ) {
        LoKi::ILoKiSvc*                     ls = lokiSvc();
        SmartIF<LHCb::IParticlePropertySvc> ppSvc( ls );
        StatusCode                          sc = m_node.validate( ppSvc );
        Assert( sc.isSuccess(), "Unable to validate the node!" );
      }
      // use the node for evaluation
      return m_node.node( LHCb::ParticleID( p->pdg_id() ) );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& DecNode::fillStream( std::ostream& s ) const {
      if ( !valid() ) { return s << "GDECNODE(invalid)"; }
      return s << "GDECNODE( " << m_node << ")";
    }
    // ============================================================================

    // ============================================================================
    // MANDATORY: clone method ("virtual constructor")
    // ============================================================================
    LongLived* LongLived::clone() const { return new LongLived( *this ); }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& LongLived::fillStream( std::ostream& s ) const { return s << "GLONGLIVED"; }
    // ============================================================================

    namespace {
      // ==========================================================================
      /// invalid Node
      static const Decays::Nodes::Invalid s_NODE{};
      /// invalid decay
      static const Decays::Trees::Types_<const HepMC::GenParticle*>::Invalid s_TREE{};
      /// "Factory"
      static const std::string s_FACTORY = "LoKi::GenDecay";
      // ==========================================================================
    } // namespace
    // ============================================================================
    // constructor from the actual tree
    // ============================================================================
    DecTree::DecTree( const iTree& tree, const bool autovalidate ) : m_tree( tree ), m_autovalidate( autovalidate ) {}
    // ============================================================================
    // constructor from the decay descriptor
    // ============================================================================
    DecTree::DecTree( const std::string& descriptor )
        : LoKi::AuxFunBase( std::tie( descriptor ) ), m_tree( s_TREE ), m_autovalidate( true ) {
      LoKi::ILoKiSvc*   ls = lokiSvc();
      SmartIF<IToolSvc> toolSvc( ls );
      Assert( !( !toolSvc ), "Unable to aquire IToolSvc tool" );

      Decays::IGenDecay* tool = 0;
      StatusCode         sc   = toolSvc->retrieveTool( s_FACTORY, tool );
      Assert( sc.isSuccess(), "Unable to retrieve '" + s_FACTORY + "'", sc );
      Assert( 0 != tool, "Decays::IGenDecay* points to NULL" );
      //
      m_tree = tool->tree( descriptor );
      toolSvc->releaseTool( tool ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ ); // do not need the tool
                                                                                            // anymore
      //
      Assert( !( !m_tree ), "The tree is invalid : '" + descriptor + "'" );
      Assert( !m_tree.marked(), "The tree is marked  : '" + descriptor + "'" );
      //
    }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool DecTree::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* point to NULL, return false" ).ignore();
        return false;
      }
      //
      if ( !valid() && m_autovalidate ) {
        const LoKi::Services&             svcs  = LoKi::Services::instance();
        const LHCb::IParticlePropertySvc* ppsvc = svcs.ppSvc();
        Assert( 0 != ppsvc, "LHCb::ParticlePropertySvc* poinst to NULL!" );
        StatusCode sc = validate( ppsvc );
        Assert( sc.isSuccess(), "Unable to validate Decays::Tree", sc );
      }
      if ( !valid() ) {
        Error( "LoKi::GenParticles::DecTree::Tree  is invalid, return false" ).ignore();
        return false;
      }
      // use the node for evaluation
      return m_tree.tree( p );
    }
    // ============================================================================
    // OPTIONAL: the nice printout
    // ============================================================================
    std::ostream& DecTree::fillStream( std::ostream& s ) const {
      if ( !valid() ) { return s << "GDECTREE(invalid)"; }
      return s << "GDECTREE( " << m_tree << ")";
    }
    // ============================================================================

    // ============================================================================
    /* constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildFun::ChildFun( const LoKi::GenTypes::GFunc& fun, const LoKi::GenChild::Selector& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /* constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildFun::ChildFun( const LoKi::GenTypes::GFunc& fun, const std::string& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildFun::ChildFun( const LoKi::GenTypes::GFunc& fun, const Decays::IGenDecay::iTree& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildFun::ChildFun( const LoKi::GenTypes::GFunc& fun, const Decays::iNode& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildFun::ChildFun( const LoKi::GenTypes::GFunc& fun, const LoKi::GenTypes::GCuts& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid:!" );
    }
    // ============================================================================
    // clone method  ("virtual destructor" )
    // ============================================================================
    ChildFun* ChildFun::clone() const { return new LoKi::GenParticles::ChildFun( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    double ChildFun::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* points to NULL, return NegativeInfinity" ).ignore();
        return LoKi::Constants::NegativeInfinity;
      }
      const HepMC::GenParticle* c = m_child.child( p );
      if ( 0 == c ) {
        Error( " child HepMC::GenParticle* points to NULL, return NegativeInfinity" ).ignore();
        return LoKi::Constants::NegativeInfinity;
      }
      //
      return m_fun( c );
    }
    // ============================================================================
    //  OPTIONAL:  specific printout
    // ============================================================================
    std::ostream& ChildFun::fillStream( std::ostream& s ) const {
      return s << " GCHILD(" << m_fun << "," << m_child << " ) ";
    }
    // ============================================================================

    // ============================================================================
    /* constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildCut::ChildCut( const LoKi::GenTypes::GCuts& fun, const LoKi::GenChild::Selector& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /* constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildCut::ChildCut( const LoKi::GenTypes::GCuts& fun, const std::string& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildCut::ChildCut( const LoKi::GenTypes::GCuts& fun, const Decays::IGenDecay::iTree& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildCut::ChildCut( const LoKi::GenTypes::GCuts& fun, const Decays::iNode& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid!" );
    }
    // ============================================================================
    /*  constructor from the function and child selector
     *  @param fun      the function to be used
     *  @param selector the child selector
     */
    // ============================================================================
    ChildCut::ChildCut( const LoKi::GenTypes::GCuts& fun, const LoKi::GenTypes::GCuts& selector )
        : LoKi::AuxFunBase( std::tie( fun, selector ) ), m_fun( fun ), m_child( selector ) {
      Assert( m_child.valid(), "Child selector is invalid:!" );
    }
    // ============================================================================
    // clone method  ("virtual destructor" )
    // ============================================================================
    ChildCut* ChildCut::clone() const { return new LoKi::GenParticles::ChildCut( *this ); }
    // ============================================================================
    // MANDATORY: the only one essential method
    // ============================================================================
    bool ChildCut::operator()( const HepMC::GenParticle* p ) const {
      if ( 0 == p ) {
        Error( "HepMC::GenParticle* points to NULL, return false" ).ignore();
        return false;
      }
      const HepMC::GenParticle* c = m_child.child( p );
      if ( 0 == c ) {
        Error( " child HepMC::GenParticle* points to NULL, return false" ).ignore();
        return false;
      }
      //
      return m_fun( c );
    }
    // ============================================================================
    //  OPTIONAL:  specific printout
    // ============================================================================
    std::ostream& ChildCut::fillStream( std::ostream& s ) const {
      return s << " GCHILDCUT(" << m_fun << "," << m_child << " ) ";
    }
    // ============================================================================

    // ============================================================================
    // IsID
    // ============================================================================
    IsID::IsID( const long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const unsigned long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const LHCb::ParticleID& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::string& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<int>& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<unsigned int>& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<long>& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<unsigned long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<LHCb::ParticleID>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const std::vector<std::string>& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID::IsID( const LoKi::Pids::GetPids& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::Pids::GetPids( id ) {}
    // ============================================================================
    IsID* IsID::clone() const { return new LoKi::GenParticles::IsID( *this ); }
    // ============================================================================
    std::ostream& IsID::fillStream( std::ostream& s ) const {
      //
      s << "(GID== ";
      //
      if ( 1 == m_names.size() ) {
        Gaudi::Utils::toStream( m_names[0], s );
      } else if ( !m_names.empty() ) {
        Gaudi::Utils::toStream( m_names, s );
      } else if ( 1 == m_ints.size() ) {
        Gaudi::Utils::toStream( m_ints[0], s );
      } else {
        Gaudi::Utils::toStream( m_ints, s );
      }
      //
      return s << ")";
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsID::operator()( const HepMC::GenParticle* p ) const {
      //
      if ( 0 == p ) {
        Error( "Invalid Particle, return 'False'" ).ignore();
        return false; // RETURN
      }
      //
      return in_list( p->pdg_id() );
    }

    // ============================================================================
    // IsNotID
    // ============================================================================
    IsNotID::IsNotID( const long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const unsigned long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const LHCb::ParticleID& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::string& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<unsigned int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<unsigned long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<LHCb::ParticleID>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID::IsNotID( const std::vector<std::string>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsNotID* IsNotID::clone() const { return new IsNotID( *this ); }
    // ============================================================================
    std::ostream& IsNotID::fillStream( std::ostream& s ) const {
      //
      s << "(GID!= ";
      //
      if ( 1 == m_names.size() ) {
        Gaudi::Utils::toStream( m_names[0], s );
      } else if ( !m_names.empty() ) {
        Gaudi::Utils::toStream( m_names, s );
      } else if ( 1 == m_ints.size() ) {
        Gaudi::Utils::toStream( m_ints[0], s );
      } else {
        Gaudi::Utils::toStream( m_ints, s );
      }
      //
      return s << ")";
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsNotID::operator()( const HepMC::GenParticle* p ) const {
      //
      if ( 0 == p ) {
        Error( "Invalid Particle, return 'False'" ).ignore();
        return false; // RETURN
      }
      return not_in_list( p->pdg_id() );
    }

    // ============================================================================
    // IsAbsID
    // ============================================================================
    IsAbsID::IsAbsID( const long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const unsigned long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const LHCb::ParticleID& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::string& id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<unsigned int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<unsigned long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<LHCb::ParticleID>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const std::vector<std::string>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID::IsAbsID( const LoKi::Pids::GetPids& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsID( id ) {}
    // ============================================================================
    IsAbsID* LoKi::GenParticles::IsAbsID::clone() const { return new LoKi::GenParticles::IsAbsID( *this ); }
    // ============================================================================
    std::ostream& IsAbsID::fillStream( std::ostream& s ) const {
      //
      s << "(GABSID== ";
      //
      if ( 1 == m_names.size() ) {
        Gaudi::Utils::toStream( m_names[0], s );
      } else if ( !m_names.empty() ) {
        Gaudi::Utils::toStream( m_names, s );
      } else if ( 1 == m_ints.size() ) {
        Gaudi::Utils::toStream( m_ints[0], s );
      } else {
        Gaudi::Utils::toStream( m_ints, s );
      }
      //
      return s << ")";
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsAbsID::operator()( const HepMC::GenParticle* p ) const {
      //
      if ( 0 == p ) {
        Error( "Invalid Particle, return 'False'" ).ignore();
        return false; // RETURN
      }
      //
      return in_abs_list( p->pdg_id() );
    }

    // ============================================================================
    // IsNotAbsID
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const long id ) : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const unsigned long id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const LHCb::ParticleID& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::string& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<unsigned int>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<unsigned long>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<LHCb::ParticleID>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const std::vector<std::string>& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID::IsNotAbsID( const LoKi::Pids::GetPids& id )
        : LoKi::AuxFunBase( std::tie( id ) ), LoKi::GenParticles::IsAbsID( id ) {}
    // ============================================================================
    IsNotAbsID* IsNotAbsID::clone() const { return new IsNotAbsID( *this ); }
    // ============================================================================

    std::ostream& IsNotAbsID::fillStream( std::ostream& s ) const {
      //
      s << "(GABSID!= ";
      //
      if ( 1 == m_names.size() ) {
        Gaudi::Utils::toStream( m_names[0], s );
      } else if ( !m_names.empty() ) {
        Gaudi::Utils::toStream( m_names, s );
      } else if ( 1 == m_ints.size() ) {
        Gaudi::Utils::toStream( m_ints[0], s );
      } else {
        Gaudi::Utils::toStream( m_ints, s );
      }
      //
      return s << ")";
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    bool IsNotAbsID::operator()( const HepMC::GenParticle* p ) const {
      //
      if ( 0 == p ) {
        Error( "Invalid Particle, return 'False'" ).ignore();
        return false; // RETURN
      }
      //
      return not_in_abs_list( p->pdg_id() );
    }
    // ============================================================================

    // ============================================================================
    // get unique string for HepMC::Particle
    // ============================================================================
    std::string hex_id( const HepMC::GenParticle* particle ) {
      if ( 0 == particle ) { return "NULL"; }
      boost::format fmt( "%p" );
      const void*   p = particle;
      fmt % p;
      return fmt.str();
    }

    // ============================================================================
    namespace {
      // hashing function
      const std::hash<const void*> s_hash{};
    } // namespace
    // ============================================================================
    // get hash function for HepMC::GenParticle
    // ============================================================================
    std::size_t hash( const HepMC::GenParticle* particle ) { return 0 == particle ? 0 : s_hash( particle ); }
    // ============================================================================
    // get hash function for HepMC::GenVertex
    // ============================================================================
    std::size_t hash( const HepMC::GenVertex* vertex ) { return 0 == vertex ? 0 : s_hash( vertex ); }

  } // namespace GenParticles
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
