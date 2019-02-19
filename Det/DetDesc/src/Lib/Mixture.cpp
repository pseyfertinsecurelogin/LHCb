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
// DetDesc
#include "DetDesc/Mixture.h"
#include "DetDesc/MaterialException.h"
#include <cmath>
#include <numeric>
// Gaudi Math (for floating point comparison)
#include "LHCbMath/Lomont.h"
//
Mixture::Mixture( const std::string& name, const double a, const double z, const double i, const double density,
                  const double rl, const double al, const double temp, const double press, const eState s )
    : Material( name, density, rl, al, temp, press, s )
    , m_A( a )
    , m_Z( z )
    , m_I( i )
    , m_a( 0. )
    , m_m( 0. )
    , m_C( 0 )
    , m_X0( 0. )
    , m_X1( 0. ) {}

void Mixture::reset() {
  m_elements.clear();
  m_atoms.clear();
  m_own.reset();
  m_A  = 0;
  m_Z  = 0;
  m_I  = 0;
  m_a  = 0;
  m_m  = 0;
  m_C  = 0;
  m_X0 = 0;
  m_X1 = 0;
  Material::reset();
}

const SmartRef<Element>& Mixture::element( const unsigned int i ) const {
  if ( i >= m_elements.size() ) { throw MaterialException( "Mixture::element:: wrong index! ", this ); }
  return m_elements[i].second;
}
//
SmartRef<Element>& Mixture::element( const unsigned int i ) {
  if ( i >= m_elements.size() ) { throw MaterialException( "Mixture::element:: wrong index! ", this ); }
  return m_elements[i].second;
}
//
double Mixture::elementFraction( const unsigned int i ) const {
  if ( i >= m_elements.size() ) { throw MaterialException( "Mixture::element:: wrong index! ", this ); }
  return m_elements[i].first;
}
//
void Mixture::addElement( const SmartRef<Element>& e, const int nOfAtoms, const bool comp ) {
  if ( m_own ) { throw MaterialException( "Mixture::could not add element!", this ); }
  if ( !e ) { throw MaterialException( "Mixture::could not add element", this ); }
  if ( m_elements.size() != m_atoms.size() ) { throw MaterialException( "Mixture::mismatch container sizes! ", this ); }
  //
  m_elements.emplace_back( 0, e );
  m_atoms.push_back( nOfAtoms );
  //
  if ( comp ) { computeByAtoms(); }
  //
}
//
void Mixture::addElement( const SmartRef<Element>& e, const double fraction, const bool comp ) {
  if ( m_own ) { throw MaterialException( "Mixture::could not add element!", this ); }
  if ( !e ) { throw MaterialException( "Mixture::non valid pointer!" + name() ); }
  if ( !m_atoms.empty() ) { throw MaterialException( "Mixture:could not add element!", this ); }
  //
  auto it = std::find_if( m_elements.begin(), m_elements.end(),
                          [&]( const Entry& i ) { return e->name() == i.second->name(); } );
  if ( it == m_elements.end() ) {
    m_elements.emplace_back( fraction, e );
  } else {
    it->first += fraction;
  }
  //
  if ( comp ) { compute(); }
}
//
void Mixture::addMixture( const SmartRef<Mixture>& mx, const double fraction, const bool comp ) {
  if ( !mx ) { throw MaterialException( "Mixture::non valid pointer!" ); }
  //
  for ( const auto& e : mx->elements() ) { addElement( e.second, fraction * e.first ); }
  //
  if ( comp ) { compute(); }
}
//
StatusCode Mixture::compute() {
  if ( m_atoms.empty() ) {
    return computeByFraction();
  } else if ( m_atoms.size() == m_elements.size() ) {
    return computeByAtoms();
  } else {
    throw MaterialException( "Mixture::compute: could not compute " + name() );
  }
}
//
StatusCode Mixture::computeByAtoms() {
  //
  if ( m_elements.empty() ) { addMyself(); }
  //
  if ( m_elements.size() != m_atoms.size() ) {
    throw MaterialException( std::string( "Mixture::computeByAtoms::" ) + "mismatch in container sizes!", this );
  }
  //
  // Compute molecular weight
  double sum = 0.0;
  for ( unsigned int i1 = 0; i1 < m_elements.size(); ++i1 ) {
    Element* elem = m_elements[i1].second;
    sum += m_atoms[i1] * elem->A();
  }
  //
  // Compute proprotion by weight for each element
  for ( unsigned int i2 = 0; i2 < m_elements.size(); ++i2 ) {
    Element* elem        = m_elements[i2].second;
    m_elements[i2].first = m_atoms[i2] * elem->A() / sum;
  }
  //
  m_atoms.clear();
  //
  return computeByFraction();
}
//
StatusCode Mixture::computeByFraction() {
  //
  if ( !m_atoms.empty() ) {
    throw MaterialException( std::string( "Mixture::computeByFraciton!" ) + "Atoms nust be empty!", this );
  }
  //
  if ( m_elements.empty() ) {
    addMyself();
    m_atoms.clear();
  }
  //
  // recompute fractions
  double frsum = std::accumulate( m_elements.begin(), m_elements.end(), 0.0,
                                  []( double s, std::pair<double, SmartRef<Element>>& i ) { return s + i.first; } );
  if ( frsum <= 0 ) {
    throw MaterialException( std::string( "Mixture::computeByFractions::" ) + "not positive fraction sum!", this );
  }
  // rescale  fractions
  for ( auto& e : m_elements ) e.first /= frsum;
  //
  m_A = 0;
  m_Z = 0;
  m_I = 0;
  //
  double radleninv = 0;
  double lambdainv = 0;
  //
  for ( const auto& e : m_elements ) {
    const double   frac = e.first;
    const Element* elem = e.second;
    //
    m_A += frac * elem->A();
    m_Z += frac * elem->Z();
    m_I += frac * elem->Z() * std::log( elem->I() );
    //
    // Use the aproximate formula for radiation lengh of mixtures 1/x0 = sum(wi/Xi)
    if ( elem->radiationLength() > 0.0 && elem->density() > 0.0 ) {
      radleninv += frac / ( elem->radiationLength() * elem->density() );
    } else {
      radleninv += frac * Gaudi::Units::Avogadro * elem->tsaiFactor() / elem->A();
    }
    //
    if ( elem->absorptionLength() > 0.0 ) { lambdainv += frac / ( elem->absorptionLength() * elem->density() ); }
  }

  if ( Z() <= 0 ) return StatusCode::FAILURE;
  m_I = std::exp( m_I / m_Z );

  const double A1 = 1.0 * Gaudi::Units::g / Gaudi::Units::mole;
  const double D1 = 1.0 * Gaudi::Units::g / Gaudi::Units::cm3;

  if ( density() <= 0 || A() <= 0 ) return StatusCode::FAILURE;
  const double plasma = 28.816 * std::sqrt( density() / D1 * Z() * A1 / A() ) * Gaudi::Units::eV;

  m_C        = 1 + 2 * std::log( m_I / plasma );
  double x_a = m_C / 4.606;

  if ( m_I < 100 * Gaudi::Units::eV ) {
    if ( m_C > 3.681 ) {
      m_X0 = 0.326 * m_C - 1.0;
      m_X1 = 2;
    } else {
      m_X0 = 0.2;
      m_X1 = 2;
    }
  } else {
    if ( m_C > 5.215 ) {
      m_X0 = 0.326 * m_C - 1.5;
      m_X1 = 3;
    } else {
      m_X0 = 0.2;
      m_X1 = 3;
    }
  }

  m_a = 4.606 * ( x_a - m_X0 ) / std::pow( m_X1 - m_X0, 3 );
  m_m = 3.0;

  // only update the radiation and interaction length if the attribute is
  // is not provided in the input file

  if ( LHCb::Math::lomont_compare_double( radiationLength(), 0.0, 1000 ) ) {
    if ( radleninv > 0 ) { setRadiationLength( 1.0 / radleninv / density() ); }
  }
  if ( LHCb::Math::lomont_compare_double( absorptionLength(), 0.0, 1000 ) ) {
    if ( lambdainv > 0 ) { setAbsorptionLength( 1.0 / lambdainv / density() ); }
  }
  //
  return I() <= 0 ? StatusCode::FAILURE
                  : radleninv <= 0 ? StatusCode::FAILURE : lambdainv <= 0 ? StatusCode::FAILURE : StatusCode::SUCCESS;
}
//
StatusCode Mixture::addMyself() {
  if ( m_own ) {
    throw MaterialException( std::string( "Mixture::addMyself: " ) + "could not add myself twice! ", this );
  }
  if ( !m_elements.empty() ) {
    throw MaterialException( std::string( "Mixture::addMyself: " ) + "could not add myself to " + "existing elements! ",
                             this );
  }
  if ( !m_atoms.empty() ) {
    throw MaterialException( std::string( "Mixture::addMyself: could " ) + "not add myself to existing atoms! ", this );
  }
  //
  m_own = std::make_unique<Element>( name(), "XX", A(), Z(), I(), density(), radiationLength(), absorptionLength(),
                                     temperature(), pressure(), state() );
  //
  m_own->compute();
  m_own->setName( m_own->name() + "[ownElementForMixture]" );
  //
  m_elements.emplace_back( 1, m_own.get() );
  m_atoms.push_back( 1 );
  //
  return StatusCode::SUCCESS;
}
//
MsgStream& Mixture::fillStream( MsgStream& s ) const {
  Material::fillStream( s );
  if ( !m_elements.empty() ) {
    s << "\t #MassComponents=" << std::setw( 2 ) << m_elements.size() << endmsg;
    for ( auto it = m_elements.begin(); m_elements.end() != it; ++it ) {
      s << "\t\tcomponent#" << std::setw( 2 ) << it - m_elements.begin() << " massfraction=" << std::setw( 9 )
        << it->first << endmsg << "\t" << it->second;
    }
  }
  //
  if ( !m_atoms.empty() ) {
    s << "\t #AtomComponents=" << std::setw( 2 ) << m_atoms.size() << endmsg;
    for ( auto it = m_atoms.begin(); m_atoms.end() != it; ++it ) {
      s << "\t\tcomponent#" << std::setw( 2 ) << it - m_atoms.begin() << " atom=" << std::setw( 2 ) << *it << endmsg;
    }
  }
  return s;
}
//
std::ostream& Mixture::fillStream( std::ostream& s ) const {
  Material::fillStream( s );
  if ( !m_elements.empty() ) {
    s << "\t #MassComponents=" << std::setw( 2 ) << m_elements.size() << '\n';
    for ( auto it = m_elements.begin(); m_elements.end() != it; ++it ) {
      s << " \t\tcomponent#" << std::setw( 2 ) << it - m_elements.begin() << " massfraction=" << std::setw( 9 )
        << it->first << "\n\t" << it->second;
    }
  }
  //
  if ( !m_atoms.empty() ) {
    s << "\t #AtomComponents=" << std::setw( 2 ) << m_atoms.size() << '\n';
    for ( auto it = m_atoms.begin(); m_atoms.end() != it; ++it ) {
      s << " \t\tcomponent#" << std::setw( 2 ) << it - m_atoms.begin() << " atom=" << std::setw( 2 ) << *it << '\n';
    }
  }
  return s;
}
