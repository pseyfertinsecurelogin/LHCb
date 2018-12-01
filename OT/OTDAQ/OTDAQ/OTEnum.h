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
#ifndef OTDAQ_OTENUM_H_
#define OTDAQ_OTENUM_H_

namespace OTDAQ
{
  enum { OffsetStations=1, OffsetLayers=0, OffsetQuadrants=0, OffsetModules=1, OffsetStraws=1 } ;
  enum { NumStations=3, NumLayers=4, NumQuadrants=4, NumModules=9, NumStraws=128 } ;
  enum { NumChanPerOtis=32 } ;
}

#endif
