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
#include "UTDet/DeUTStave.h"
#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTSector.h"

#include "DetDesc/IGeometryInfo.h"

#include <algorithm>

#include <numeric>

using namespace LHCb;

/** @file DeUTStave.cpp
 *
 *  Implementation of class :  DeUTStave
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

DeUTStave::DeUTStave( const std::string& name ) : DeUTBaseElement( name ) {}

const CLID& DeUTStave::clID() const { return DeUTStave::classID(); }

StatusCode DeUTStave::initialize() {
  // initialize method
  StatusCode sc = DeUTBaseElement::initialize();

  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    m_detRegion   = param<int>( "detRegion" );
    m_firstSector = param<int>( "firstReadoutSector" );
    m_column      = param<int>( "column" );
    m_numSectors  = param<int>( "numSectors" );

    if ( exists( "staveType" ) ) {
      m_staveRotZ = param<std::string>( "staveRotZ" );
      m_type      = param<std::string>( "staveType" );
    } else if ( exists( "moduleType" ) ) {
      m_staveRotZ = param<std::string>( "moduleRotZ" );
      m_type      = param<std::string>( "moduleType" );
    }

    m_parent                   = getParent<DeUTStave>();
    const UTChannelID parentID = m_parent->elementID();
    UTChannelID chan( UTChannelID::detType::typeUT, parentID.station(), parentID.layer(), m_detRegion, m_firstSector,
                      0 );
    setElementID( chan );
    m_sectors = getChildren<DeUTStave>();
  }

  if ( exists( "version" ) ) m_versionString = param<std::string>( "version" );

  sc = registerCondition( this, m_prodIDString, &DeUTStave::updateProdIDCondition, true );
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to register prodID conditions" << endmsg;
    return StatusCode::FAILURE;
  }

  return StatusCode::SUCCESS;
}

StatusCode DeUTStave::updateProdIDCondition() {
  const Condition* aCon = condition( m_prodIDString );
  if ( !aCon ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to find condition" << endmsg;
    return StatusCode::FAILURE;
  }
  m_prodID = aCon->param<int>( "ProdID" );
  return StatusCode::SUCCESS;
}

std::ostream& DeUTStave::printOut( std::ostream& os ) const {
  // stream to cout
  os << " Stave : " << name() << " type " << m_type << " Det region " << m_detRegion << " Column " << m_column
     << std::endl;

  return os;
}

MsgStream& DeUTStave::printOut( MsgStream& os ) const {

  // stream to Msg service
  os << " Stave : " << name() << " type " << m_type << " Det region " << m_detRegion << " Column " << m_column
     << std::endl;

  return os;
}

DeUTSector* DeUTStave::findSector( const UTChannelID aChannel ) {
  auto iter = std::find_if( m_sectors.begin(), m_sectors.end(),
                            [&]( const DeUTSector* s ) { return s->contains( aChannel ); } );

  return ( iter != m_sectors.end() ? *iter : nullptr );
}

DeUTSector* DeUTStave::findSector( const Gaudi::XYZPoint& point ) {
  auto iter =
      std::find_if( m_sectors.begin(), m_sectors.end(), [&]( const DeUTSector* s ) { return s->isInside( point ); } );
  return ( iter != m_sectors.end() ? *iter : nullptr );
}

double DeUTStave::fractionActive() const {
  return std::accumulate( m_sectors.begin(), m_sectors.end(), 0.0,
                          []( double f, const DeUTSector* s ) { return f + s->fractionActive(); } ) /
         double( m_sectors.size() );
}
