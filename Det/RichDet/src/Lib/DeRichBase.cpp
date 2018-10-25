/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// Gaudi
#include "GaudiKernel/SmartDataPtr.h"

// local
#include "RichDet/DeRichBase.h"
#include "RichDet/DeRichSystem.h"

// Access DeRichSystem on demand
DeRichSystem *
DeRichBase::deRichSys()
{
  if ( UNLIKELY( !m_deRichS ) )
  {
    // find the RichSystem
    SmartDataPtr< DetectorElement > afterMag( dataSvc(), "/dd/Structure/LHCb/AfterMagnetRegion" );
    if ( !afterMag )
    {
      throw GaudiException(
        "Could not load AfterMagnetRegion ", "DeRichBase::deRichSys()", StatusCode::FAILURE );
    }

    std::string deRichSysLoc;
    if ( afterMag->exists( "RichDetectorLocations" ) )
    {
      const auto firstRichLoc = afterMag->paramVect< std::string >( "RichDetectorLocations" )[0];
      SmartDataPtr< DetectorElement > deRich( dataSvc(), firstRichLoc );
      if ( !deRich )
      {
        throw GaudiException( "Could not load first RICH at " + firstRichLoc,
                              "DeRichBase::deRichSys()",
                              StatusCode::FAILURE );
      }

      if ( deRich->exists( "RichSystemDetElemLocation" ) )
      { deRichSysLoc = deRich->param< std::string >( "RichSystemDetElemLocation" ); }
      else
      {
        throw GaudiException( "First RICH does not have RichSystem location",
                              "DeRichBase::deRichSys()",
                              StatusCode::FAILURE );
      }
    }
    else
    {
      deRichSysLoc = DeRichLocations::RichSystem;
    }

    // get the rish system from the location found
    SmartDataPtr< DeRichSystem > deRichS( dataSvc(), deRichSysLoc );
    if ( !deRichS )
    {
      throw GaudiException( "Could not load DeRichSystem at " + deRichSysLoc,
                            "DeRichBase::deRichSys()",
                            StatusCode::FAILURE );
    }

    m_deRichS = deRichS;
  } // close if for first time access

  return m_deRichS;
}
