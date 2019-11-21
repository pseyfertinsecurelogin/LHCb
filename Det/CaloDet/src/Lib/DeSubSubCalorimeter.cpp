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
#include "CaloDet/DeSubSubCalorimeter.h"
#include "CaloKernel/CaloException.h"

#include <algorithm> // for std::find

/** @file DeSubSubCalorimeter.cpp
 *
 *  Implementation of class :  DeSubSubCalorimeter
 *  @author Olivier Callot Olivier.Callot@cern.ch
 *  @author Vanya Belyaev  Ivan.Belyaev@itep.ru
 */

// ============================================================================
/// object identification
// ============================================================================
const CLID& DeSubSubCalorimeter::clID() const { return DeSubSubCalorimeter::classID(); }

// ============================================================================
// standard initialization
// ============================================================================
StatusCode DeSubSubCalorimeter::initialize() {
  /// initialize the base class
  StatusCode sc = DetectorElement::initialize();
  if ( sc.isFailure() ) return sc;

  const auto& pars = paramNames();
  /// cell size
  auto it = std::find( pars.begin(), pars.end(), "CellSize" );
  if ( pars.end() == it ) return StatusCode::FAILURE;
  setCellSize( param<double>( *it ) );

  /// subSubCalo X size
  auto itx = std::find( pars.begin(), pars.end(), "XSize" );
  if ( pars.end() == itx ) return StatusCode::FAILURE;
  setXSize( param<double>( *itx ) );

  /// subSubCalo Y size
  auto ity = std::find( pars.begin(), pars.end(), "YSize" );
  if ( pars.end() == ity ) return StatusCode::FAILURE;
  setYSize( param<double>( *ity ) );

  /// area Id
  auto itt = std::find( pars.begin(), pars.end(), "Area" );
  if ( pars.end() == itt ) return StatusCode::FAILURE;
  setArea( param<int>( *itt ) );

  Assert( nullptr != geometry(), "DeSubSubcalorimeter: Invalid GeometryInfo" );
  return StatusCode::SUCCESS;
}

// ============================================================================
// standard printout to STL standard stream
// ============================================================================
std::ostream& DeSubSubCalorimeter::printOut( std::ostream& os ) const {
  os << "\tDeSubSubCalorimeter"
     << " fullname ='" << name() << std::endl;
  os << "\t\t\tCellSize=" << std::setw( 12 ) << m_cellSize << " Area-ID = " << m_area
     << " X-center offset : " << xSize() / 2. << std::endl;
  return os;
}

// ============================================================================
// standard printout to Gaudi standard stream
// ============================================================================
MsgStream& DeSubSubCalorimeter::printOut( MsgStream& os ) const {
  os << "\tDeSubSubCalorimeter"
     << " fullname ='" << name() << endmsg;
  os << "\t\t\tCellSize=" << std::setw( 12 ) << m_cellSize << " Area-ID = " << m_area
     << " X-center offset : " << xSize() / 2 << endmsg;
  return os;
}
