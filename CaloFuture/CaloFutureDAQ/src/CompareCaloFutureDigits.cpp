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
// Include files

// STL
#include <math.h> // for fabs() on Windows

// from Gaudi
#include "GaudiKernel/IRegistry.h"
#include "GaudiKernel/SystemOfUnits.h"

// local
#include "CompareCaloFutureDigits.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareCaloFutureDigits
//
// 2003-11-18 : Olivier Callot
//-----------------------------------------------------------------------------

DECLARE_COMPONENT( CompareCaloFutureDigits )

//=============================================================================
// Standard constructor, initializes variables
//=============================================================================
CompareCaloFutureDigits::CompareCaloFutureDigits( const std::string& name, ISvcLocator* pSvcLocator )
    : GaudiAlgorithm( name, pSvcLocator ) {
  declareProperty( "Extension", m_extension = "Test" );
  declareProperty( "PackedRawBuffer", m_packedRawBuffer = false );
}

//=============================================================================
// Main execution
//=============================================================================
StatusCode CompareCaloFutureDigits::execute() {

  if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

  std::string ecalName = LHCb::CaloDigitLocation::Ecal;
  std::string hcalName = LHCb::CaloDigitLocation::Hcal;
  if ( m_packedRawBuffer ) {
    ecalName = LHCb::CaloDigitLocation::FullEcal;
    hcalName = LHCb::CaloDigitLocation::FullHcal;
  }

  //== SPD. Digits are yes/no -> tolerance = .5
  LHCb::CaloDigits* spd1 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Spd );
  LHCb::CaloDigits* spd2 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Spd + m_extension );
  compareContainers( *spd1, *spd2, 0.5 );

  //== PreShower.
  LHCb::CaloDigits* prs1 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Prs );
  LHCb::CaloDigits* prs2 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Prs + m_extension );
  compareContainers( *prs1, *prs2, 0.1 );

  //== Ecal.
  LHCb::CaloDigits* ecal1 = get<LHCb::CaloDigits>( ecalName );
  LHCb::CaloDigits* ecal2 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Ecal + m_extension );
  compareContainers( *ecal1, *ecal2, 1. );

  //== Hcal.
  LHCb::CaloDigits* hcal1 = get<LHCb::CaloDigits>( hcalName );
  LHCb::CaloDigits* hcal2 = get<LHCb::CaloDigits>( LHCb::CaloDigitLocation::Hcal + m_extension );
  compareContainers( *hcal1, *hcal2, 1. );

  return StatusCode::SUCCESS;
}

//=========================================================================
//  Compare two CaloDigits containers
//=========================================================================
void CompareCaloFutureDigits::compareContainers( const LHCb::CaloDigits& dig1, const LHCb::CaloDigits& dig2,
                                                 double tol ) const {
  if ( dig1.size() != dig2.size() ) {
    error() << "Incoherent size : " << dig1.registry()->identifier() << " : " << dig1.size() << " and "
            << dig2.registry()->identifier() << " : " << dig2.size() << endmsg;
  } else {
    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
      debug() << "Comparing " << dig1.name() << " and " << dig2.name() << " both sizes = " << dig2.size() << endmsg;
  }

  auto it1 = dig1.begin();
  auto it2 = dig2.begin();
  while ( dig1.end() != it1 && dig2.end() != it2 ) {
    if ( !( ( *it1 )->cellID() == ( *it2 )->cellID() ) || fabs( ( *it1 )->e() - ( *it2 )->e() ) > tol ) {
      info() << "Error in sequence/energy (old-new): " << ( *it1 )->cellID() << " " << ( *it2 )->cellID()
             << format( "  E %7.1f %7.1f %7.2f ", ( *it1 )->e() / Gaudi::Units::MeV, ( *it2 )->e() / Gaudi::Units::MeV,
                        ( *it1 )->e() / Gaudi::Units::MeV - ( *it2 )->e() / Gaudi::Units::MeV )
             << endmsg;
    } else {
      if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
        verbose() << ( *it1 )->cellID() << " " << ( *it2 )->cellID() << "  E " << ( *it1 )->e() / Gaudi::Units::MeV
                  << " " << ( *it2 )->e() / Gaudi::Units::MeV << endmsg;
    }
    if ( ( *it1 )->cellID() == ( *it2 )->cellID() ) {
      it1++;
      it2++;
    } else if ( ( *it1 )->cellID() < ( *it2 )->cellID() ) {
      it1++;
    } else {
      it2++;
    }
  }
}

//=============================================================================
