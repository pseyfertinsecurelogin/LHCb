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
#ifndef SORTDIGITINODE_H 
#define SORTDIGITINODE_H 1

#include "MuonDAQ/MuonHLTDigitFormat.h"
#include "MuonHLTBase.h"

struct SortDigitInODE {
  bool operator()(  const unsigned int first, 
                    const unsigned int second ) const {
     return (( first  & MuonHLTBaseDC06::MaskAddress ) >> MuonHLTBaseDC06::ShiftAddress) 
          < (( second & MuonHLTBaseDC06::MaskAddress ) >> MuonHLTBaseDC06::ShiftAddress);

  }
};


#endif // SORTDIGITINODE_H
