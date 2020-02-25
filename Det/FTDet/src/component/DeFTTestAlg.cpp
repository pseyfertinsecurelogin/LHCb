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
// Local
#include "DeFTTestAlg.h"

/** @file DeFTTestAlg.cpp
 *
 *  Implementation of class : DeFTTestAlg
 *
 *  @author Plamen Hopchev
 *  @date   2012-04-25
 */

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( DeFTTestAlg )

//=============================================================================
// Initialization
//=============================================================================
StatusCode DeFTTestAlg::initialize() {

  // Mandatory initialization of GaudiAlgorithm
  StatusCode sc = GaudiTupleAlg::initialize();
  if ( sc.isFailure() ) { return sc; }

  // Retrieve and initialize DeFT
  m_deFT = getDet<DeFTDetector>( DeFTDetectorLocation::Default );
  if ( m_deFT == nullptr ) return Error( "Could not initialize DeFTDetector.", StatusCode::FAILURE );
  if ( m_deFT->version() < 61 ) return Error( "This version requires FTDet v6.1 or higher.", StatusCode::FAILURE );

  return StatusCode::SUCCESS;
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode DeFTTestAlg::execute() {

  if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

  // book tuple
  Tuples::Tuple tuple = GaudiTupleAlg::nTuple( "TestFTTree", "Events" );

  /// Points of the real MCHits
  const LHCb::MCHits* hitsCont = get<LHCb::MCHits>( m_mcHitsLocation );
  /// Iterate over the first few hits and test the calculateHits method
  std::cout << "MC hits: " << hitsCont->size() << std::endl;
  for ( const auto& aHit : *hitsCont ) {

    Gaudi::XYZPoint pMid = aHit->midPoint();
    Gaudi::XYZPoint pIn  = aHit->entry();
    Gaudi::XYZPoint pOut = aHit->exit();

    tuple->column( "Hit_X", pMid.X() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "Hit_Y", pMid.Y() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "Hit_Z", pMid.Z() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitIn_X", pIn.X() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitIn_Y", pIn.Y() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitIn_Z", pIn.Z() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitOut_X", pOut.X() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitOut_Y", pOut.Y() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );
    tuple->column( "HitOut_Z", pOut.Z() ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    // Make DeFT checks
    if ( msgLevel( MSG::DEBUG ) ) {
      debug() << "\n\n\n**************************\nMC Hit " << aHit->index() << "\n"
              << "**************************" << endmsg;
      debug() << *aHit << endmsg;
    }
    std::string lVolName;

    if ( msgLevel( MSG::DEBUG ) )
      debug() << "Test of det. element geometry() methods, using "
              << "the midPoint() of the MC hit: " << pMid << endmsg;

    /// check isInside FT
    bool isInsideFT = m_deFT->isInside( pMid );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Global Point " << pMid << "; isInside =  " << isInsideFT << endmsg;
    tuple->column( "InFT", isInsideFT ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    /// test findStation method
    const DeFTStation* pStation = m_deFT->findStation( pMid );
    lVolName                    = ( pStation ? pStation->geometry()->lvolumeName() : "" );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Found Station: " << lVolName << endmsg;
    tuple->column( "StationID", ( pStation ? pStation->stationID() : -1 ) )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    /// test findLayer method
    const DeFTLayer* pLayer = m_deFT->findLayer( pMid );
    lVolName                = ( pLayer ? pLayer->geometry()->lvolumeName() : "" );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Found Layer  : " << lVolName << endmsg;
    tuple->column( "LayerID", ( pLayer ? pLayer->layerID() : -1 ) )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    /// test findModule method
    const DeFTModule* pModule = m_deFT->findModule( pMid );
    lVolName                  = ( pModule ? pModule->geometry()->lvolumeName() : "" );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Found Module  : " << lVolName << endmsg;
    tuple->column( "ModuleID", ( pModule ? pModule->moduleID() : -1 ) )
        .ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    /// test findMat method
    const DeFTMat* pMat = m_deFT->findMat( pMid );
    lVolName            = ( pMat ? pMat->geometry()->lvolumeName() : "" );
    if ( msgLevel( MSG::DEBUG ) ) debug() << "Found Mat  : " << lVolName << endmsg;
    tuple->column( "MatID", ( pMat ? pMat->matID() : -1 ) ).ignore( /* AUTOMATICALLY ADDED FOR gaudi/Gaudi!763 */ );

    // ok, write tuple row
    StatusCode status = tuple->write();
    if ( status.isFailure() ) return Error( "Cannot fill ntuple" );

  } // end loop over hits

  return StatusCode::SUCCESS;
}
