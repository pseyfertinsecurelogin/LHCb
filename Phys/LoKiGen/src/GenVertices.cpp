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
#include <numeric>
// ============================================================================
// Event
// ============================================================================
#include "Event/HepMCEvent.h"
// ============================================================================
// LoKiCore
// ============================================================================
#include "LoKi/Constants.h"
#include "LoKi/Primitives.h"
// ============================================================================
// LoKiPhys
// ============================================================================
#include "LoKi/GenVertices.h"
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
 *  @date 2006-02-08
 */
// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::BarCode* LoKi::GenVertices::BarCode::clone() const {
  return new LoKi::GenVertices::BarCode( *this );
}
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::BarCode::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 0" );
    return 0;
  }
  return v->barcode();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::BarCode::fillStream( std::ostream& stream ) const { return stream << "GVBARCODE"; }
// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::PositionX* LoKi::GenVertices::PositionX::clone() const { return new PositionX( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::PositionX::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 'InvalidDistance'" );
    return LoKi::Constants::InvalidDistance;
  }
  return v->position().x();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::PositionX::fillStream( std::ostream& stream ) const { return stream << "GVX"; }
// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::PositionY* LoKi::GenVertices::PositionY::clone() const { return new PositionY( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::PositionY::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 'InvalidDistance'" );
    return LoKi::Constants::InvalidDistance;
  }
  return v->position().y();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::PositionY::fillStream( std::ostream& stream ) const { return stream << "GVY"; }
// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::PositionZ* LoKi::GenVertices::PositionZ::clone() const { return new PositionZ( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::PositionZ::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 'InvalidDistance'" );
    return LoKi::Constants::InvalidDistance;
  }
  return v->position().z();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::PositionZ::fillStream( std::ostream& stream ) const { return stream << "GVZ"; }
// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::PositionT* LoKi::GenVertices::PositionT::clone() const { return new PositionT( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::PositionT::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 'InvalidTime'" );
    return LoKi::Constants::InvalidTime;
  }
  return v->position().t();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::PositionT::fillStream( std::ostream& stream ) const { return stream << "GVT"; }

// ============================================================================
//  MANDATORY: clone method ("virtual" constructor")
// ============================================================================
LoKi::GenVertices::Rho* LoKi::GenVertices::Rho::clone() const { return new Rho( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::Rho::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* points to NULL; return 'InvalidDistance'" );
    return LoKi::Constants::InvalidDistance;
  }
  return v->position().perp();
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::Rho::fillStream( std::ostream& stream ) const { return stream << "GVRHO"; }

// ============================================================================
/*  constructor from the criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::CountIF::CountIF( const LoKi::Types::GCuts& cut, HepMC::IteratorRange range )
    : m_cut( cut ), m_range( range ) {}
// ============================================================================
/*  constructor from the criteria and "range"
 *  @param range search region
 *  @param cut the criteria
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::CountIF::CountIF( HepMC::IteratorRange range, const LoKi::Types::GCuts& cut )
    : m_cut( cut ), m_range( range ) {}
// ============================================================================
//  MANDATORY: clone method ("virtual contructor")
// ============================================================================
LoKi::GenVertices::CountIF* LoKi::GenVertices::CountIF::clone() const { return new CountIF( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::CountIF::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) { return 0; }
  HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*>( v );
  return std::count_if( _v->particles_begin( m_range ), _v->particles_end( m_range ), m_cut );
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::CountIF::fillStream( std::ostream& stream ) const {
  stream << "GVCOUNT(" << m_cut << ",";
  switch ( m_range ) {
  case HepMC::parents:
    stream << "HepMC.parents";
    break;
  case HepMC::children:
    stream << "HepMC.children";
    break;
  case HepMC::family:
    stream << "HepMC.family";
    break;
  case HepMC::ancestors:
    stream << "HepMC.ancestors";
    break;
  case HepMC::descendants:
    stream << "HepMC.descendants";
    break;
  case HepMC::relatives:
    stream << "HepMC.relatives";
    break;
  default:
    stream << m_range;
    break;
  };
  return stream << ")";
}
// ============================================================================
/*  constructor from the function and the "range"
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::SumIF::SumIF( const LoKi::Types::GFunc& fun, HepMC::IteratorRange range )
    : m_fun( fun )
    , m_cut( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant( true ) )
    , m_range( range ) {}
// ============================================================================
/*  constructor from the function and the "range"
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::SumIF::SumIF( HepMC::IteratorRange range, const LoKi::Types::GFunc& fun )
    : m_fun( fun )
    , m_cut( LoKi::BasicFunctors<const HepMC::GenParticle*>::BooleanConstant( true ) )
    , m_range( range ) {}
// ============================================================================
/*  constructor from the function, criteria and "range"
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::SumIF::SumIF( const LoKi::Types::GFunc& fun, HepMC::IteratorRange range,
                                 const LoKi::Types::GCuts& cut )
    : m_fun( fun ), m_cut( cut ), m_range( range ) {}
// ============================================================================
/*  constructor from the function, "range" and criteria
 *  @param fun function to be accuulated
 *  @param cut the criteria
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::SumIF::SumIF( const LoKi::Types::GFunc& fun, const LoKi::Types::GCuts& cut,
                                 HepMC::IteratorRange range )
    : m_fun( fun ), m_cut( cut ), m_range( range ) {}
// ============================================================================
/*  constructor from the function, "range" and criteria
 *  @param cut the criteria
 *  @param fun function to be accuulated
 *  @param range search region
 *  @see HepMC::IteratorRange
 */
// ============================================================================
LoKi::GenVertices::SumIF::SumIF( const LoKi::Types::GCuts& cut, const LoKi::Types::GFunc& fun,
                                 HepMC::IteratorRange range )
    : m_fun( fun ), m_cut( cut ), m_range( range ) {}
// ============================================================================
//  MANDATORY: clone method ("virtual contructor")
// ============================================================================
LoKi::GenVertices::SumIF* LoKi::GenVertices::SumIF::clone() const { return new SumIF( *this ); }
// ============================================================================
//  MANDATORY: the only one essential method
// ============================================================================
double LoKi::GenVertices::SumIF::operator()( const HepMC::GenVertex* v ) const {
  if ( UNLIKELY( !v ) ) {
    Error( "HepMC::GenVertex* poitns to NULL, return 0 " );
    return 0;
  }
  HepMC::GenVertex* _v = const_cast<HepMC::GenVertex*>( v );

  return std::accumulate( _v->particles_begin( m_range ), _v->particles_end( m_range ), 0.,
                          [&]( double r, const HepMC::GenParticle* p ) {
                            if ( UNLIKELY( !p ) ) {
                              Warning( "HepMC::GenParticle* points to 0; skip it" );
                            } else if ( m_cut( p ) ) {
                              r += m_fun( p );
                            }
                            return r;
                          } );
}
// ============================================================================
//  "SHORT" representation, @see LoKi::AuxFunBase
// ============================================================================
std::ostream& LoKi::GenVertices::SumIF::fillStream( std::ostream& stream ) const {
  stream << "GVSUM(" << m_fun << ",";
  switch ( m_range ) {
  case HepMC::parents:
    stream << "HepMC.parents";
    break;
  case HepMC::children:
    stream << "HepMC.children";
    break;
  case HepMC::family:
    stream << "HepMC.family";
    break;
  case HepMC::ancestors:
    stream << "HepMC.ancestors";
    break;
  case HepMC::descendants:
    stream << "HepMC.descendants";
    break;
  case HepMC::relatives:
    stream << "HepMC.relatives";
    break;
  default:
    stream << m_range;
    break;
  };
  return stream << "," << m_cut << ")";
}
// ============================================================================

// ============================================================================
// The END
// ============================================================================
