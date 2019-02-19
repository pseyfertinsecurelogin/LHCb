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
/** @file DeRichRadiator.cpp
 *
 *  Implementation file for detector description class : DeRichRadiator
 *
 *  @author Antonis Papanestis a.papanestis@rl.ac.uk
 *  @date   2004-06-18
 */
//----------------------------------------------------------------------------

#define DERICHRADIATOR_CPP

// Include files
#include "RichDet/DeRichRadiator.h"

// Gaudi
#include "GaudiKernel/Bootstrap.h"
#include "GaudiKernel/IAlgTool.h"
#include "GaudiKernel/ISvcLocator.h"
#include "GaudiKernel/IToolSvc.h"
#include "GaudiKernel/MsgStream.h"

//----------------------------------------------------------------------------

//=========================================================================
//  default constructor
//=========================================================================
DeRichRadiator::DeRichRadiator( const std::string& name ) : DeRichBase( name ) {}

StatusCode DeRichRadiator::initialize() {
  // store the name of the radiator
  const auto pos = name().find( "Rich" );
  setMyName( std::string::npos != pos ? name().substr( pos ) : "DeRichRadiator_NO_NAME" );

  const auto sc = setRadiatorID();

  _ri_debug << "Initializing Radiator : " << rich() << " " << radiatorID() << endmsg;

  return sc;
}

//=========================================================================
// initTabPropInterpolators
//=========================================================================
StatusCode DeRichRadiator::initTabPropInterpolators() {
  _ri_debug << "Initialising interpolators" << endmsg;

  if ( m_refIndexTabProp ) {
    if ( !m_refIndex ) {
      m_refIndex.reset( new Rich::TabulatedProperty1D( m_refIndexTabProp ) );
    } else {
      m_refIndex->initInterpolator( m_refIndexTabProp );
    }
    if ( !m_refIndex->valid() ) {
      error() << "Invalid RINDEX Rich::TabulatedProperty1D for " << m_refIndexTabProp->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  if ( m_rayleighTabProp ) {
    if ( !m_rayleigh ) {
      m_rayleigh.reset( new Rich::TabulatedProperty1D( m_rayleighTabProp ) );
    } else {
      m_rayleigh->initInterpolator( m_rayleighTabProp );
    }
    if ( !m_rayleigh->valid() ) {
      error() << "Invalid RAYLEIGH Rich::TabulatedProperty1D for " << m_rayleighTabProp->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  if ( m_absorptionTabProp ) {
    if ( !m_absorption ) {
      m_absorption.reset( new Rich::TabulatedProperty1D( m_absorptionTabProp ) );
    } else {
      m_absorption->initInterpolator( m_absorptionTabProp );
    }
    if ( !m_absorption->valid() ) {
      error() << "Invalid ABSORPTION Rich::TabulatedProperty1D for " << m_absorptionTabProp->name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

//=========================================================================
// generateHltRefIndex
//=========================================================================
StatusCode DeRichRadiator::generateHltRefIndex() {
  // use normal refractive index
  m_hltRefIndex = m_refIndex;
  // return OK
  return StatusCode::SUCCESS;
}

//=========================================================================
// Set RichDetector and name
//=========================================================================
StatusCode DeRichRadiator::setRadiatorID() {
  if ( exists( "IDVector" ) ) {
    const auto radID = paramVect<int>( "IDVector" );
    m_rich           = Rich::DetectorType( radID[0] );
    m_radiatorID     = Rich::RadiatorType( radID[1] );
    return StatusCode::SUCCESS;
  } else {

    if ( std::string::npos != name().find( "Rich2" ) ) {
      m_radiatorID = Rich::Rich2Gas;
      m_rich       = Rich::Rich2;
    } else if ( std::string::npos != name().find( "Aerogel" ) ) {
      m_radiatorID = Rich::Aerogel;
      m_rich       = Rich::Rich1;
    } else if ( std::string::npos != name().find( "Rich1Gas" ) ) {
      m_radiatorID = Rich::Rich1Gas;
      m_rich       = Rich::Rich1;
    } else {
      error() << "Cannot find radiator type for " << name() << endmsg;
      return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

//=============================================================================
