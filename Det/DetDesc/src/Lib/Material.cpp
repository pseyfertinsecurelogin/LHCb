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
/// GaudiKernel
#include "GaudiKernel/System.h"
/// DetDesc
#include "DetDesc/Material.h"
#include "DetDesc/TabulatedProperty.h"

using namespace Gaudi::Units;

//////////////////////
Material::Material( const std::string& name, const double dens, const double rl, const double al, const double temp,
                    const double press, const eState s )
    : m_name( name )
    , m_density( dens )
    , m_radiationLength( rl )
    , m_absorptionLength( al )
    , m_temperature( temp )
    , m_pressure( press )
    , m_state( s ) {}

//////////////////////
void Material::reset() {
  setName( "" );
  setDensity( 0 );
  setRadiationLength( 0 );
  setAbsorptionLength( 0 );
  setTemperature( Gaudi::Units::STP_Temperature );
  setPressure( Gaudi::Units::STP_Pressure );
  setState( stateUndefined );
}

//////////////////////
MsgStream& Material::fillStream( MsgStream& s ) const {
  ///
  s << " Material/" << System::typeinfoName( typeid( *this ) ) << " \tname='" << name() << "'" << endmsg << "\t"
    << " Atomic[g/mole] =" << std::setw( 12 ) << A() / ( g / mole ) << " Z            =" << std::setw( 12 ) << Z()
    << " #Nucleons  =" << std::setw( 12 ) << (int)N() << endmsg << "\t"
    << " Density[g/cm3] =" << std::setw( 12 ) << m_density / ( g / cm3 ) << " X0 [cm]      =" << std::setw( 12 )
    << m_radiationLength / cm << " Lambda[cm] =" << std::setw( 12 ) << m_absorptionLength / cm << endmsg << "\t"
    << " Temperature [K]=" << std::setw( 12 ) << m_temperature / kelvin << " Pressure[atm]=" << std::setw( 12 )
    << m_pressure / atmosphere << " State=";
  ///
  switch ( m_state ) {
  case stateSolid:
    s << " Solid   ";
    break;
  case stateLiquid:
    s << " Liquid  ";
    break;
  case stateGas:
    s << " Gas     ";
    break;
  default:
    s << "undefined";
  }
  ///
  s << " #properties=" << std::setw( 2 ) << m_props.size() << endmsg;

  unsigned i = 0;
  for ( const auto& prop : m_props ) { s << "\t property#" << std::setw( 2 ) << i++ << "   " << prop; }
  return s;
}
//////////////////////
std::ostream& Material::fillStream( std::ostream& s ) const {
  ///
  s << " Material/" << System::typeinfoName( typeid( *this ) ) << " \tname='" << name() << "'" << std::endl
    << "\t"
    << " Atomic[g/mole] =" << std::setw( 12 ) << A() / ( g / mole ) << " Z            =" << std::setw( 12 ) << Z()
    << " #Nucleons  =" << std::setw( 12 ) << (int)N() << std::endl
    << "\t"
    << " Density[g/cm3] =" << std::setw( 12 ) << m_density / ( g / cm3 ) << " X0 [cm]      =" << std::setw( 12 )
    << m_radiationLength / cm << " Lambda[cm] =" << std::setw( 12 ) << m_absorptionLength / cm << std::endl
    << "\t"
    << " Temperature [K]=" << std::setw( 12 ) << m_temperature / kelvin << " Pressure[atm]=" << std::setw( 12 )
    << m_pressure / atmosphere << " State=";
  ///
  switch ( m_state ) {
  case stateSolid:
    s << " Solid   ";
    break;
  case stateLiquid:
    s << " Liquid  ";
    break;
  case stateGas:
    s << " Gas     ";
    break;
  default:
    s << "undefined";
    break;
  }
  ///
  s << " #properties=" << std::setw( 2 ) << m_props.size() << '\n';
  unsigned i = 0;
  for ( const auto& prop : m_props ) { s << "\t property#" << std::setw( 2 ) << i++ << "   " << prop; }
  return s;
}

// ============================================================================
// End
// ============================================================================
