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
// $Id: STDAQDefinitions.h,v 1.6 2009-02-05 13:45:55 jluisier Exp $
#ifndef _STDAQDefinitions_H
#define _STDAQDefinitions_H

#include <utility>

class STTell1ID;

/** @namespace STDAQDefinitions STDAQDefinitions.h  Kernel/STDAQDefinitions.h
 *
 *  Namespace for some useful type definitions and const
 *
 *  @author M.Needham
 *  @date   13/3/2002
*/


namespace STDAQ{

  typedef std::pair<STTell1ID,unsigned int> chanPair;

  typedef unsigned int rawInt;

  /// number of bits for adc
  static constexpr unsigned int numBitsADC = 8u;

  /// and a mask
  static constexpr unsigned int adcMask = 0xff;

  /// version of the decoding software
  enum version { inValidVersion = -1, v1 = 0, v2 = 1 , v3 = 2 , v4 = 3};

  /// n strips per ppx
  static constexpr unsigned int nStripPerPPx = 768u;

  /// n strips per Tell1 boar
  static constexpr unsigned int nStripsPerBoard = 3072u;

  /// invalid pcn
  static constexpr unsigned int inValidPcn = 200u;


  /// TELL1 links
  static constexpr unsigned int noptlinks = 24u;
  static constexpr unsigned int nheaders = 3u;
  static constexpr unsigned int nbeetles = 3u;
  static constexpr unsigned int nports = 4u;
  static constexpr unsigned int nstrips = 32u;
  static constexpr unsigned int nBeetlesPerPPx = 6u;
  static constexpr unsigned int mask1= 0x000000ff;
  static constexpr unsigned int mask2= 0x0000ff00;
  static constexpr unsigned int mask3= 0x00ff0000;
  static constexpr unsigned int mask4= 0xff000000;

  /// Store the number of strips in each sector
  enum StripsInSector{
    nStripsInITSector=384u,
    nStripsInTTSector=512u,
    nStripsInUTSector=512u
  };


  // Non-Zero suppressed specific
  static constexpr unsigned int nwordsFull = 896u;
  static constexpr unsigned int sizebankFull = nwordsFull*4;

  // Pedestal specific
  static constexpr unsigned int nwordsPedestal = 768u;
  static constexpr unsigned int sizebankPedestal = nwordsPedestal*4;

  /// Event Info
  static constexpr unsigned int npp = 4u;
  static constexpr unsigned int ninfo = 32u;

  //// min size of error bank
  static constexpr unsigned int minErrorBankWords = 28u;

  /// max sixr of error bank
  static constexpr unsigned int maxErrorBankWords = 56u;

}

#endif // _STDAQDefinitions_H
