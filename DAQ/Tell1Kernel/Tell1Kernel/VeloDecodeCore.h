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
// $Id: VeloDecodeCore.h,v 1.1 2007-09-18 07:33:02 cattanem Exp $
#ifndef VELODECODECORE_H
#define VELODECODECORE_H 1
#include <vector>

// Include files

/** @namespace VeloDecodeCore VeloDecodeCore.h
 *
 *
 *  @author Tomasz Szumlak
 *  @date   2006-04-11
 */
namespace VeloTELL1
{
  typedef std::vector<unsigned int*> rawVec;
  typedef std::vector<unsigned int> dataVec;
  typedef std::vector<unsigned int>::const_iterator cdatIt;
  typedef std::vector<unsigned int>::iterator datIt;
  typedef std::vector<signed int> sdataVec;
  typedef std::vector<signed int>::const_iterator scdatIt;
  typedef std::vector<signed int>::iterator sdatIt;
  typedef std::vector<unsigned int> evtInfo;
  typedef std::vector<unsigned int> errorInfo;
  typedef std::vector<evtInfo> allEvt;
  typedef std::vector<errorInfo> allError;

  // define parameters for bitShifts (was ENUM)
  constexpr int bitShift8=8;
  constexpr int bitShift12=12;
  constexpr int bitShift16=16;
  constexpr int bitShift19=19;
  constexpr int bitShift21=21;
  constexpr int bitShift22=22;
  constexpr int bitShift24=24;
  constexpr int bitShift30=30;
  constexpr int bitShift32=32;

  // define parameters for bitMasks (was ENUM)
  constexpr int bitMask1=0x00000001;
  constexpr int bitMask2=0x00000003;
  constexpr int bitMask3=0x00000007;
  constexpr int bitMask4=0x0000000f;
  constexpr int bitMask8=0x000000ff;
  constexpr int bitMask12=0x00000fff;
  constexpr int bitMask16=0x0000ffff;
  constexpr int bitMask24=0x00ffffff;

  // define parameters for errorBankMarkers (was ENUM)
  constexpr int EBMarker0=0x00000000;
  constexpr int EBMarker1=0x00000001;
  constexpr int EBMarker2=0x00000002;
  constexpr int EBMarker3=0x00000003;
  constexpr int EBMarker4=0x00000004;
  constexpr int EBMarker=0x0000008e;
  //

}
#endif // VELODECODECORE_H
