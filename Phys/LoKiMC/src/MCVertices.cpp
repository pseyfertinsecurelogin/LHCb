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
// Event
// ============================================================================
#include "Event/MCParticle.h"
#include "Event/MCVertex.h"
// ============================================================================
// local
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Kinematics.h"
#include "LoKi/MCTypes.h"
#include "LoKi/MCVertices.h"
// ============================================================================
/** @file
 *
 *  Implementation file for functions from namespace LoKi::MCVertices
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
  namespace MCVertices {
    // ============================================================================
    TypeOfMCVertex* TypeOfMCVertex::clone() const { return new TypeOfMCVertex(); }
    // ============================================================================
    double TypeOfMCVertex::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return -1000000." ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1000000.0;
      }
      return v->type();
    }
    // ============================================================================
    std::ostream& TypeOfMCVertex::fillStream( std::ostream& s ) const { return s << "MCVTYPE"; }
    // ============================================================================

    // ============================================================================
    TimeOfFlight* TimeOfFlight::clone() const { return new TimeOfFlight( *this ); }
    // ============================================================================
    double TimeOfFlight::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return -1000 * ms " )
            .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1000. * Gaudi::Units::ms;
      }
      return v->time();
    }
    // ============================================================================
    std::ostream& TimeOfFlight::fillStream( std::ostream& s ) const { return s << "MCVTOF"; }
    // ============================================================================

    // ============================================================================
    VertexPositionX* VertexPositionX::clone() const { return new VertexPositionX( *this ); }
    // ============================================================================
    double VertexPositionX::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'InvalidDistance'" )
            .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return LoKi::Constants::InvalidDistance;
      }
      return v->position().x();
    }
    // ============================================================================
    std::ostream& VertexPositionX::fillStream( std::ostream& s ) const { return s << "MCVX"; }
    // ============================================================================

    // ============================================================================
    VertexPositionY* VertexPositionY::clone() const { return new VertexPositionY( *this ); }
    // ============================================================================
    double VertexPositionY::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'InvalidDistance'" )
            .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return LoKi::Constants::InvalidDistance;
      }
      return v->position().y();
    }
    // ============================================================================
    std::ostream& VertexPositionY::fillStream( std::ostream& s ) const { return s << "MCVY"; }
    // ============================================================================

    // ============================================================================
    VertexPositionZ* VertexPositionZ::clone() const { return new VertexPositionZ( *this ); }
    // ============================================================================
    double VertexPositionZ::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'InvalidDistance'" )
            .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return LoKi::Constants::InvalidDistance;
      }
      return v->position().z();
    }
    // ============================================================================
    std::ostream& VertexPositionZ::fillStream( std::ostream& s ) const { return s << "MCVZ"; }
    // ============================================================================

    // ============================================================================
    VertexTime* VertexTime::clone() const { return new VertexTime( *this ); }
    // ============================================================================
    double VertexTime::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'InvalidTime'" )
            .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return LoKi::Constants::InvalidTime;
      }
      return v->time();
    }
    // ============================================================================
    std::ostream& VertexTime::fillStream( std::ostream& s ) const { return s << "MCVTIME"; }
    // ============================================================================

    // ============================================================================
    Primary* Primary::clone() const { return new Primary( *this ); }
    // ============================================================================
    bool Primary::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'false' " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return false;
      }
      return v->isPrimary();
    }
    // ============================================================================
    std::ostream& Primary::fillStream( std::ostream& s ) const { return s << "MCISPRIMARY"; }
    // ============================================================================

    // ============================================================================
    Decay* Decay::clone() const { return new Decay( *this ); }
    // ============================================================================
    bool Decay::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return 'false' " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return false;
      }
      return v->isDecay();
    }
    // ============================================================================
    std::ostream& Decay::fillStream( std::ostream& s ) const { return s << "MCISDECAY"; }
    // ============================================================================

    // ============================================================================
    MCVertexDistance::MCVertexDistance( const LoKi::Point3D& point )
        : LoKi::AuxFunBase( std::tie( point ) ), m_point( point ) {}
    // ============================================================================
    MCVertexDistance::MCVertexDistance( const LHCb::MCVertex* point ) {
      if ( !point ) {
        Error( "MCVertex* points to NULL!" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        m_point = LoKi::Point3D( -1.0 * Gaudi::Units::km, -1.0 * Gaudi::Units::km, -1.0 * Gaudi::Units::km );
      } else {
        m_point = point->position();
      }
    }
    // ============================================================================
    MCVertexDistance* MCVertexDistance::clone() const { return new MCVertexDistance( *this ); }
    // ============================================================================
    double MCVertexDistance::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( "MCVertex* points to NULL, return -1.0 * km " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1.0 * Gaudi::Units::km;
      }
      return ( v->position() - m_point ).R();
    }
    // ============================================================================
    std::ostream& MCVertexDistance::fillStream( std::ostream& s ) const { return s << "MCVDIST"; }
    // ============================================================================

    // ============================================================================
    MCVFunAsMCFun::MCVFunAsMCFun( const LoKi::MCTypes::MCVFunc& fun )
        : LoKi::AuxFunBase( std::tie( fun ) ), m_fun( fun ) {}
    // ============================================================================
    MCVFunAsMCFun* MCVFunAsMCFun::clone() const { return new MCVFunAsMCFun( *this ); }
    // ============================================================================
    double MCVFunAsMCFun::operator()( const LHCb::MCParticle* p ) const {
      if ( !p ) {
        Error( "MCParticle* points to NULL, return -1000000." ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1000000;
      };
      const LHCb::MCVertex* vertex = p->originVertex();
      if ( !vertex ) {
        Warning( "'origin' MCVertex* points to NULL" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      //
      return m_fun( vertex );
    }
    // ============================================================================
    std::ostream& MCVFunAsMCFun::fillStream( std::ostream& s ) const { return s << "MCVXFUN(" << m_fun << ")"; }
    // ============================================================================

    // ============================================================================
    Key* Key::clone() const { return new Key( *this ); }
    // ============================================================================
    double Key::operator()( const LHCb::MCVertex* v ) const {
      if ( !v ) {
        Error( " MCVertex* points to NULL, return -1000 " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1000;
      }
      return v->key();
    }
    // ============================================================================
    std::ostream& Key::fillStream( std::ostream& s ) const { return s << "MCVKEY"; }
    // ============================================================================

    // ============================================================================
    MCVPFunAsMCFun::MCVPFunAsMCFun( const LoKi::MCTypes::MCVFunc& fun )
        : LoKi::AuxFunBase( std::tie( fun ) ), LoKi::MCVertices::MCVFunAsMCFun( fun ) {}
    // ============================================================================
    MCVPFunAsMCFun* MCVPFunAsMCFun::clone() const { return new MCVPFunAsMCFun( *this ); }
    // ============================================================================
    double MCVPFunAsMCFun::operator()( const LHCb::MCParticle* p ) const {
      //
      if ( !p ) {
        Error( "MCParticle* points to NULL, return -1000000." ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1000000;
      }
      //
      const LHCb::MCVertex* vertex = p->primaryVertex();
      if ( !vertex ) {
        Warning( "'primary' MCVertex* points to NULL" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      //
      return func()( vertex );
    }
    // ============================================================================
    std::ostream& MCVPFunAsMCFun::fillStream( std::ostream& s ) const { return s << "MCVPXFUN(" << func() << ")"; }
    // ============================================================================

    // ============================================================================
    /* constructor from MC Vertex fuction
     *  @param func vertex function tobe applied
     *  @param index vertex index :
     *         -1   - the last vertex  in the list
     *         -2   - the origin  vertex
     *         -3   - the mother  vertex
     *         -4   - the primary vertex
     */
    // ============================================================================
    MCVFunction::MCVFunction( const LoKi::MCTypes::MCVFunc& func, const int index )
        : LoKi::AuxFunBase( std::tie( func, index ) )
        , LoKi::MCVertices::MCVFunAsMCFun( func )
        , m_case( true )
        , m_index( index )
        , m_cut( LoKi::Constant<const LHCb::MCVertex*, bool>( false ) ) {}
    // ============================================================================
    // constructor from MC Vertex fuction and vertex selection
    // ============================================================================
    MCVFunction::MCVFunction( const LoKi::MCTypes::MCVFunc& func, const LoKi::MCTypes::MCVCuts& cuts )
        : LoKi::AuxFunBase( std::tie( func, cuts ) )
        , LoKi::MCVertices::MCVFunAsMCFun( func )
        , m_case( false )
        , m_index( 0 )
        , m_cut( cuts ) {}
    // ============================================================================
    // constructor from MC Vertex fuction and vertex selection
    // ============================================================================
    MCVFunction::MCVFunction( const LoKi::MCTypes::MCVCuts& cuts, const LoKi::MCTypes::MCVFunc& func )
        : LoKi::AuxFunBase( std::tie( cuts, func ) )
        , LoKi::MCVertices::MCVFunAsMCFun( func )
        , m_case( false )
        , m_index( 0 )
        , m_cut( cuts ) {}
    // ============================================================================
    // clone method (mandatory!)
    // ============================================================================
    MCVFunction* MCVFunction::clone() const { return new MCVFunction( *this ); }
    // ============================================================================
    std::ostream& MCVFunction::fillStream( std::ostream& s ) const {
      if ( m_case ) { return s << "MCVFUN(" << func() << "," << m_index << ")"; }
      //
      return s << "MCVFUN(" << func() << "," << m_cut << ")";
    }
    // ============================================================================
    // the only one essential method
    // ============================================================================
    double MCVFunction::operator()( const LHCb::MCParticle* p ) const {
      //
      if ( !p ) {
        Warning( "MCParticle* points to NULL, return -1e+9 " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return -1.e+9;
      }
      //
      const LHCb::MCVertex* v = nullptr;
      //
      if ( m_case && Origin == m_index ) {
        //
        v = p->originVertex();
        if ( !v ) {
          Warning( " Origin     MCVertex* points to NULL" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        }
        //
        return func()( v );
      } else if ( m_case && Primary == m_index ) {
        //
        v = p->primaryVertex();
        if ( !v ) {
          Warning( " Primary    MCVertex* points to NULL" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        }
        //
        return func()( v );
      }
      //
      const auto& evs = p->endVertices();
      //
      if ( evs.empty() ) {
        Warning( " Empty EndVertices list " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      //
      if ( m_case && Last == m_index ) {
        if ( !evs.empty() ) { v = evs.back(); }
        return func()( v );
      } else if ( m_case && 0 <= m_index && m_index < (long)evs.size() ) {
        v = evs[m_index];
        return func()( v );
      } else if ( m_case ) {
        Error( " Invalid EndVertices index" ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
        return func()( v );
      }
      // now we deal with cuts:
      auto igood = std::find_if( evs.begin(), evs.end(), m_cut );
      //
      if ( evs.end() != igood ) {
        v = *igood;
      } else {
        Error( " No proper vertex is found " ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
      }
      //
      return func()( v );
      //
    }

  } // namespace MCVertices
} // namespace LoKi
// ============================================================================
// The END
// ============================================================================
