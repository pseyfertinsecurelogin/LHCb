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
#include "STDet/DeSTStation.h"

// DetDesc
#include "DetDesc/IGeometryInfo.h"

/** @file DeSTStation.cpp
 *
 *  Implementation of class :  DeSTStation
 *
 *    @author Matthew Needham
 */

using namespace LHCb;

DeSTStation::DeSTStation( const std::string& name ) : DeSTBaseElement( name ), m_id( 0u ) {
  // constructer
}

StatusCode DeSTStation::initialize() {

  // initialize

  StatusCode sc = DeSTBaseElement::initialize();
  if ( sc.isFailure() ) {
    MsgStream msg( msgSvc(), name() );
    msg << MSG::ERROR << "Failed to initialize detector element" << endmsg;
  } else {
    m_id = (unsigned int)param<int>( "stationID" );
  }

  return sc;
}

std::ostream& DeSTStation::printOut( std::ostream& os ) const {

  // stream to cout
  os << " Station: " << m_id << std::endl;
  os << " Nickname: " << m_nickname << std::endl;

  return os;
}

MsgStream& DeSTStation::printOut( MsgStream& os ) const {

  // stream to Msg service
  os << " Station : " << m_id << endmsg;
  os << " Nickname: " << m_nickname << endmsg;

  return os;
}
