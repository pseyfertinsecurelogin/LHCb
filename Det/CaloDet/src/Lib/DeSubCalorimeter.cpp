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

// CaloDet
#include "CaloDet/DeSubCalorimeter.h"
#include "CaloDet/DeSubSubCalorimeter.h"
#include "CaloKernel/CaloException.h"

/** @file DeSubCalorimeter.cpp
 *
 *  Implementation of class :  DeSubCalorimeter
 *  @author Olivier Deschamos odescham@in2p3.fr
 */

// ============================================================================
// Standard Constructors
// ============================================================================
DeSubCalorimeter::DeSubCalorimeter( const std::string& name ) : DetectorElement( name ) {}

// ============================================================================
/// object identification
// ============================================================================
const CLID& DeSubCalorimeter::clID() const { return DeSubCalorimeter::classID(); }

// ============================================================================
// standard initialization
// ============================================================================
StatusCode DeSubCalorimeter::initialize() {
  /// initialize the base class
  StatusCode sc = DetectorElement::initialize();
  if ( sc.isFailure() ) { return sc; }

  auto        index  = CaloCellCode::CaloNumFromName( name() );
  std::string myName = CaloCellCode::caloName( index ) + "SubDet";
  MsgStream   msg( msgSvc(), myName );

  auto pars = paramNames();
  /// calo half ID
  auto it = std::find( pars.begin(), pars.end(), "CaloSide" );
  if ( pars.end() == it ) return StatusCode::FAILURE;
  const int value = param<int>( *it );
  setSide( value == 1 ? DeSubCalorimeter::Side::A : DeSubCalorimeter::Side::C );
  pars.erase( it );

  {
    // collect the sub-sub-calorimeters (calo areas)
    IDetectorElement::IDEContainer& subdets = childIDetectorElements();
    if ( UNLIKELY( msg.level() <= MSG::DEBUG ) )
      msg << MSG::DEBUG << name() << " :  number of subSubCalorimeter " << subdets.size() << endmsg;
    for ( auto child : subdets ) {

      if ( !child ) { continue; }
      auto* subSub = dynamic_cast<DeSubSubCalorimeter*>( child );
      Assert( subSub, "no DeSubSubCalorimeter!" );
      m_subSubCalos.push_back( subSub );
    }
    Assert( !m_subSubCalos.empty(), "Empty subcalorimeters!" );
  }

  //
  Assert( geometry(), "DeSubcalorimeter: Invalid GeometryInfo" );
  ///
  return StatusCode::SUCCESS;
  ///
}

// ============================================================================
// standard printout to STL standard stream
// ============================================================================
std::ostream& DeSubCalorimeter::printOut( std::ostream& os ) const {
  os << "\tDeSubCalorimeter"
     << " fullname ='" << name() << std::endl;
  os << "\t\t\tSide=" << sideName() << " #areas " << subSubCalos().size() << std::endl;
  return os;
}

// ============================================================================
// standard printout to Gaudi standard stream
// ============================================================================
MsgStream& DeSubCalorimeter::printOut( MsgStream& os ) const {
  os << "\tDeSubCalorimeter"
     << " fullname ='" << name() << endmsg;
  os << "\t\t\tSide=" << sideName() << " #areas " << subSubCalos().size() << endmsg;
  return os;
}

// ============================================================================
