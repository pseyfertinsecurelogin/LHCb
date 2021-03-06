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
#include "Kernel/UTDetectorPlot.h"
#include "Kernel/UTChannelID.h"
#include "UTDet/DeUTSector.h"

#include <string>

using namespace UT;

UTDetectorPlot::UTDetectorPlot( const std::string& name, const std::string& title, const unsigned int xBinsPerSector )
    : UT::Histo2DProperties( name, title, 19 * xBinsPerSector, 134, -9.5, 9.5, -0.5, 133.5 ) {
  // constructor
}

UTDetectorPlot::~UTDetectorPlot() {}

UTDetectorPlot::Bins UTDetectorPlot::toBins( const DeUTSector* theSector ) const {

  Bins theBins;

  const LHCb::UTChannelID chan    = theSector->elementID();
  const unsigned int      station = chan.station();
  const unsigned int      theCol  = theSector->column();
  station == 1 ? theBins.xBin = 8 - theCol : theBins.xBin = 9 - theCol;

  // now get the overall bin in y
  theBins.beginBinY = ( 34 * ( 2 * ( station - 1u ) + chan.layer() - 1u ) ) + 2;

  theBins.endBinY = theBins.beginBinY;

  unsigned int nSensors = theSector->sensors().size();
  if ( theBins.xBin == 0 ) theBins.beginBinY -= 1;
  switch ( nSensors ) {
  case 1:
    theBins.beginBinY += 12;
    theBins.endBinY = theBins.beginBinY + 2;
    break;
  case 2:
    theBins.beginBinY += 8;
    theBins.endBinY = theBins.beginBinY + 4;
    break;
  case 3:
    theBins.beginBinY += 8;
    theBins.endBinY = theBins.beginBinY + 6;
    break;
  default:
    theBins.endBinY = theBins.beginBinY + 8;
    break;
  } // switch sensors

  return theBins;
}
