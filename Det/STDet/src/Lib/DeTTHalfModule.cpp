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

#include "STDet/DeTTHalfModule.h"
#include "STDet/DeTTLayer.h"
#include "STDet/DeTTSector.h"

#include "DetDesc/IGeometryInfo.h"

/** @file DeTTHalfModule.cpp
 *
 *  Implementation of class :  DeTTHalfModule
 *
 *    @author Matthew Needham
 */

// STL
#include <algorithm>

#include <numeric>

using namespace LHCb;

DeTTHalfModule::DeTTHalfModule( const std::string& name )
    : DeSTBaseElement( name )
    , m_detRegion( 0u )
    , m_firstSector( 0u )
    , m_column( 0u )
    , m_parent( NULL )
    , m_prodID( 0 )
    , m_versionString( "DC06" )
    , m_prodIDString( "ProdID" ) {
  // constructer
  m_sectors.clear();
}

const CLID& DeTTHalfModule::clID() const { return DeTTHalfModule::classID(); }

StatusCode DeTTHalfModule::initialize() {

  // initialize method

  StatusCode sc = DeSTBaseElement::initialize();

  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    m_detRegion   = param<int>( "detRegion" );
    m_firstSector = param<int>( "firstReadoutSector" );
    m_position    = param<std::string>( "top_bottom" );
    m_column      = param<int>( "column" );
    m_type        = param<std::string>( "moduleType" );

    m_parent                   = getParent<DeTTHalfModule>();
    const STChannelID parentID = m_parent->elementID();
    STChannelID chan( STChannelID::detType::typeTT, parentID.station(), parentID.layer(), m_detRegion, m_firstSector,
                      0 );
    setElementID( chan );
    m_sectors = getChildren<DeTTHalfModule>();
  }

  if ( exists( "version" ) ) m_versionString = param<std::string>( "version" );

  // Can't test the version string, it's unfortunalety not trustable
  // it exists a DC07 version (why?) that doesn't contain prodID
  if ( m_versionString != "DC06" && m_versionString != "DC07" ) {
    sc = registerCondition( this, m_prodIDString, &DeTTHalfModule::updateProdIDCondition, true );
    if ( sc.isFailure() ) {
      MsgStream msg( msgSvc(), name() );
      msg << MSG::ERROR << "Failed to register prodID conditions" << endmsg;
      return StatusCode::FAILURE;
    }
  }

  return StatusCode::SUCCESS;
}

StatusCode DeTTHalfModule::updateProdIDCondition() {
  const Condition* aCon = condition( m_prodIDString );
  if ( aCon == 0 ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to find condition" << endmsg;
    return StatusCode::FAILURE;
  }
  m_prodID = aCon->param<int>( "ProdID" );

  return StatusCode::SUCCESS;
}

std::ostream& DeTTHalfModule::printOut( std::ostream& os ) const {

  // stream to cout
  os << " Module : " << name() << " type " << m_type << "position " << m_position << " Det region " << m_detRegion
     << " Column " << m_column << std::endl;

  return os;
}

MsgStream& DeTTHalfModule::printOut( MsgStream& os ) const {

  // stream to Msg service
  os << " Module : " << name() << " type " << m_type << "position " << m_position << " Det region " << m_detRegion
     << " Column " << m_column << std::endl;

  return os;
}

DeTTSector* DeTTHalfModule::findSector( const STChannelID aChannel ) {

  // return pointer to the station from channel
  /*
  int sectorNum = -1;
  if ((m_detRegion == aChannel.detRegion()) && ((m_parent->contains(aChannel) == true))) {
    sectorNum = aChannel.sector() - m_firstSector;
  }
  return ((sectorNum >= 0 && (unsigned int) sectorNum < m_sectors.size()) ? m_sectors[sectorNum]: 0  );
  */

  auto iter = std::find_if( m_sectors.begin(), m_sectors.end(),
                            [&]( const DeSTSector* s ) { return s->contains( aChannel ); } );
  return iter != m_sectors.end() ? *iter : nullptr;
}

DeTTSector* DeTTHalfModule::findSector( const Gaudi::XYZPoint& point ) {

  // find the half module
  auto iter =
      std::find_if( m_sectors.begin(), m_sectors.end(), [&]( const DeSTSector* s ) { return s->isInside( point ); } );

  return iter != m_sectors.end() ? *iter : nullptr;
}

double DeTTHalfModule::fractionActive() const {

  return std::accumulate( m_sectors.begin(), m_sectors.end(), 0.0,
                          []( double f, const DeSTSector* s ) { return f + s->fractionActive(); } ) /
         double( m_sectors.size() );
}

bool DeTTHalfModule::contains( const LHCb::STChannelID aChannel ) const {
  return ( ( aChannel.detRegion() == m_detRegion && aChannel.sector() >= m_firstSector &&
             aChannel.sector() < m_firstSector + m_sectors.size() ) &&
           m_parent->contains( aChannel ) );
}
