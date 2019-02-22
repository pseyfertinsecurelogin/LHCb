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
#ifndef MUONDET_IMUONFASTPOSTOOL_H
#define MUONDET_IMUONFASTPOSTOOL_H 1

// Include files
#include "Kernel/MuonTileID.h"

// GaudiKernel
#include "GaudiKernel/IAlgTool.h"

/** @class IMuonPosTool IMuonPosTool.h MuonTools/IMuonPosTool.h
 *
 *  Interface for the tools to convert MuonTileID to coordinates
 *
 *  @author David Hutchcroft
 *  @date   11/03/2002
 */

class IMuonFastPosTool : public extend_interfaces<IAlgTool> {
public:
  DeclareInterfaceID( IMuonFastPosTool, 5, 0 );

  /** Calculate the x,y,z and dx,dy,dz of a MuonTileID in mm
   *  this ignores gaps: these can never be read out independently
   */
  virtual StatusCode calcTilePos( const LHCb::MuonTileID& tile, double& x, double& deltax, double& y, double& deltay,
                                  double& z, double& deltaz ) const = 0;
};
#endif // MUONDET_IMUONFASTPOSTOOL_H
