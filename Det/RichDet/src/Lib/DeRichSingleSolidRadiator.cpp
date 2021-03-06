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

//----------------------------------------------------------------------------
/** @file DeRichSingleSolidRadiator.cpp
 *
 *  Implementation file for detector description class : DeRichSingleSolidRadiator
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
//----------------------------------------------------------------------------

// Include files
#include "RichDet/DeRichSingleSolidRadiator.h"

// Gaudi
#include "GaudiKernel/SystemOfUnits.h"

// MathCore files
#include "GaudiKernel/Transform3DTypes.h"
#include "GaudiKernel/Vector3DTypes.h"

/// Detector description classes
#include "DetDesc/SolidBoolean.h"

//----------------------------------------------------------------------------

const CLID CLID_DeRichSingleSolidRadiator = 12040; // User defined

// Standard Constructor
DeRichSingleSolidRadiator::DeRichSingleSolidRadiator( const std::string& name ) : DeRichRadiator( name ) {}

// Retrieve Pointer to class defininition structure
const CLID& DeRichSingleSolidRadiator::classID() { return CLID_DeRichSingleSolidRadiator; }

StatusCode DeRichSingleSolidRadiator::initialize() {
  const StatusCode initSC = DeRichRadiator::initialize();
  if ( initSC.isFailure() ) return initSC;

  _ri_debug << "Starting initialisation for DeRichSingleSolidRadiator " << name() << endmsg;

  m_solid = geometry()->lvolume()->solid();
  if ( !m_solid ) {
    error() << "Failed to load solid" << endmsg;
    return StatusCode::FAILURE;
  }
  m_material = geometry()->lvolume()->material();
  if ( !m_material ) {
    error() << "Failed to load material" << endmsg;
    return StatusCode::FAILURE;
  }

  for ( auto mat : m_material->tabulatedProperties() ) {
    if ( mat ) {
      if ( mat->type() == "RINDEX" ) {
        m_refIndexTabProp = mat;
        _ri_debug << "Found TabProp " << m_refIndexTabProp->name() << " type " << m_refIndexTabProp->type() << endmsg;
      }
      if ( mat->type() == "RAYLEIGH" ) {
        m_rayleighTabProp = mat;
        _ri_debug << "Found TabProp " << m_rayleighTabProp->name() << " type " << m_rayleighTabProp->type() << endmsg;
      }
      if ( mat->type() == "CKVRNDX" ) {
        m_chkvRefIndexTabProp = mat;
        _ri_debug << "Found TabProp " << m_chkvRefIndexTabProp->name() << " type " << m_chkvRefIndexTabProp->type()
                  << endmsg;
      }
      if ( mat->type() == "ABSLENGTH" ) {
        m_absorptionTabProp = mat;
        _ri_debug << "Found TabProp " << m_absorptionTabProp->name() << " type " << m_absorptionTabProp->type()
                  << endmsg;
      }
    }
  }

  if ( !m_refIndexTabProp ) {
    error() << "Radiator " << name() << " without refractive index" << endmsg;
    return StatusCode::FAILURE;
  }

  _ri_debug << " Centre " << geometry()->toGlobal( Gaudi::XYZPoint( 0, 0, 0 ) ) << endmsg;

  return initSC;
}

//=========================================================================
// prepareMomentumVector
//=========================================================================
StatusCode DeRichSingleSolidRadiator::prepareMomentumVector( std::vector<double>& photonMomentumVect, //
                                                             const double         min,                //
                                                             const double         max,                //
                                                             const unsigned int   nbins               //
                                                             ) const {

  photonMomentumVect.clear();

  // check parameters are sane
  if ( UNLIKELY( max <= min ) ) {
    error() << "Inadmissible photon energy limits " << max << " " << min << endmsg;
    return StatusCode::FAILURE;
  }
  if ( UNLIKELY( nbins <= 0 ) ) {
    error() << "Inadmissible photon energy num bins " << nbins << endmsg;
    return StatusCode::FAILURE;
  }

  photonMomentumVect.reserve( nbins );

  // fill momentum vector
  const auto photonEnergyStep = ( max - min ) / (double)nbins;
  for ( unsigned int ibin = 0; ibin < nbins; ++ibin ) {
    const auto m = min + ( photonEnergyStep * (double)ibin );
    photonMomentumVect.push_back( m );
  }

  return StatusCode::SUCCESS;
}

//=========================================================================
//  nextIntersectionPoint
//=========================================================================
StatusCode DeRichSingleSolidRadiator::nextIntersectionPoint( const Gaudi::XYZPoint&  pGlobal,    //
                                                             const Gaudi::XYZVector& vGlobal,    //
                                                             Gaudi::XYZPoint&        returnPoint //
                                                             ) const {

  const auto pLocal = geometry()->toLocal( pGlobal );
  const auto vLocal = geometry()->toLocalMatrix() * vGlobal;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( pLocal, vLocal, ticks );

  if ( UNLIKELY( 0 == noTicks ) ) { return StatusCode::FAILURE; }

  returnPoint = geometry()->toGlobal( pLocal + ticks[0] * vLocal );
  return StatusCode::SUCCESS;
}

//=========================================================================
// intersectionPoints
//=========================================================================
StatusCode DeRichSingleSolidRadiator::intersectionPoints( const Gaudi::XYZPoint&  position,   //
                                                          const Gaudi::XYZVector& direction,  //
                                                          Gaudi::XYZPoint&        entryPoint, //
                                                          Gaudi::XYZPoint&        exitPoint   //
                                                          ) const {

  const auto pLocal = geometry()->toLocal( position );
  const auto vLocal = geometry()->toLocalMatrix() * direction;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( pLocal, vLocal, ticks );

  if ( UNLIKELY( 0 == noTicks ) ) { return StatusCode::FAILURE; }

  entryPoint = geometry()->toGlobal( pLocal + ticks[0] * vLocal );
  exitPoint  = geometry()->toGlobal( pLocal + ticks[noTicks - 1] * vLocal );

  return StatusCode::SUCCESS;
}

//=========================================================================
//
//=========================================================================
unsigned int DeRichSingleSolidRadiator::intersectionPoints( const Gaudi::XYZPoint&        pGlobal, //
                                                            const Gaudi::XYZVector&       vGlobal, //
                                                            std::vector<Gaudi::XYZPoint>& points   //
                                                            ) const {

  const auto pLocal = geometry()->toLocal( pGlobal );
  const auto vLocal = geometry()->toLocalMatrix() * vGlobal;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( pLocal, vLocal, ticks );
  if ( 0 != noTicks ) {
    points.reserve( ticks.size() );
    for ( const auto& tick : ticks ) { points.emplace_back( geometry()->toGlobal( pLocal + tick * vLocal ) ); }
  }

  return noTicks;
}

//=========================================================================
//  intersections
//=========================================================================
unsigned int DeRichSingleSolidRadiator::intersections( const Gaudi::XYZPoint&         pGlobal,      //
                                                       const Gaudi::XYZVector&        vGlobal,      //
                                                       Rich::RadIntersection::Vector& intersections //
                                                       ) const {

  const auto pLocal = geometry()->toLocal( pGlobal );
  const auto vLocal = geometry()->toLocalMatrix() * vGlobal;

  ISolid::Ticks ticks;
  const auto    noTicks = m_solid->intersectionTicks( pLocal, vLocal, ticks );
  if ( 0 != noTicks ) {
    assert( ( noTicks % 2 ) == 0 );
    intersections.reserve( noTicks / 2 );
    for ( unsigned int tick = 0; tick < noTicks; tick += 2 ) {
      intersections.emplace_back( geometry()->toGlobal( pLocal + ticks[tick] * vLocal ),     //
                                  vGlobal,                                                   //
                                  geometry()->toGlobal( pLocal + ticks[tick + 1] * vLocal ), //
                                  vGlobal,                                                   //
                                  this );
    }
  }
  return intersections.size();
}

//=========================================================================
// Refractive Index
//=========================================================================
double DeRichSingleSolidRadiator::refractiveIndex( const double energy, const bool hlt ) const {
  const auto ind = refIndex( hlt );
  if ( UNLIKELY( !ind ) ) {
    warning() << "Null refractive index pointer" << endmsg;
    return 0;
  }
  return ( *ind )[energy * Gaudi::Units::eV];
}
