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
// $Id: $
#ifndef L0MUONKERNEL_L0MPTLUT_H 
#define L0MUONKERNEL_L0MPTLUT_H 1

// Include files
#include <string>
#include <map>
#include "Kernel/MuonTileID.h"

/** @class L0MPtLUT L0MPtLUT.h L0MuonKernel/L0MPtLUT.h
 *  
 *
 *  @author Julien Cogan
 *  @date   2012-04-24
 */


class L0MPtLUT {

public: 

  /// Key used in the pt Look Up Table
  typedef std::map<LHCb::MuonTileID,int> m1ptMap;
  /// Typdef for l0muon-pt Look Up Table
  typedef std::map< LHCb::MuonTileID, m1ptMap > m2m1Lut;


public: 
  /// Standard constructor
  L0MPtLUT(); 

  virtual ~L0MPtLUT( ); ///< Destructor

  bool read(std::string filename);
  int getEncodedPt( LHCb::MuonTileID m2, LHCb::MuonTileID m1);
  int getEncodedPt( LHCb::MuonTileID m3, LHCb::MuonTileID m2, LHCb::MuonTileID m1);
  int getAbsEncodedPt( LHCb::MuonTileID m2, LHCb::MuonTileID m1){
    int encodedpt = getEncodedPt(m2,m1);
    return (encodedpt&0x7F);
  };
  double getPtInMeV( LHCb::MuonTileID m2, LHCb::MuonTileID m1){
    int encodedpt = getEncodedPt(m2,m1);
    return (encodedpt&0x7F)*40.;
  };
  

protected:

private:

  m2m1Lut m_m2m1lut;

};
#endif // L0MUONKERNEL_L0MPTLUT_H
