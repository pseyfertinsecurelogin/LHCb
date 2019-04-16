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
#ifndef _UTDAQDefinitions_H
#define _UTDAQDefinitions_H

#include <utility>

class UTTell1ID;

/** @namespace UTDAQDefinitions UTDAQDefinitions.h  Kernel/UTDAQDefinitions.h
 *
 *  Namespace for some useful type definitions and const
 *
 *  @author A. Beiter (based on code by M.Needham)
 *  @date   2018-09-04
 */

namespace UTDAQ {

  typedef std::pair<UTTell1ID, unsigned int> chanPair;

  typedef unsigned int rawInt;

  /// number of bits for adc
  inline constexpr unsigned int numBitsADC = 8u;

  /// and a mask
  inline constexpr unsigned int adcMask = 0xff;

  /// version of the decoding software
  enum version { inValidVersion = -1, v1 = 0, v2 = 1, v3 = 2, v4 = 3 };

  /// n strips per ppx
  inline constexpr unsigned int nStripPerPPx = 768u;

  /// n strips per Tell1 boar
  inline constexpr unsigned int nStripsPerBoard = 3072u;

  /// invalid pcn
  inline constexpr unsigned int inValidPcn = 200u;

  /// TELL1 links
  inline constexpr unsigned int noptlinks      = 24u;
  inline constexpr unsigned int nheaders       = 3u;
  inline constexpr unsigned int nbeetles       = 3u;
  inline constexpr unsigned int nports         = 4u;
  inline constexpr unsigned int nstrips        = 32u;
  inline constexpr unsigned int nBeetlesPerPPx = 6u;
  inline constexpr unsigned int mask1          = 0x000000ff;
  inline constexpr unsigned int mask2          = 0x0000ff00;
  inline constexpr unsigned int mask3          = 0x00ff0000;
  inline constexpr unsigned int mask4          = 0xff000000;

  /// Store the number of strips in each sector
  enum StripsInSector { nStripsInUTSector = 512u };

  // Non-Zero suppressed specific
  inline constexpr unsigned int nwordsFull   = 896u;
  inline constexpr unsigned int sizebankFull = nwordsFull * 4;

  // Pedestal specific
  inline constexpr unsigned int nwordsPedestal   = 768u;
  inline constexpr unsigned int sizebankPedestal = nwordsPedestal * 4;

  /// Event Info
  inline constexpr unsigned int npp   = 4u;
  inline constexpr unsigned int ninfo = 32u;

  //// min size of error bank
  inline constexpr unsigned int minErrorBankWords = 28u;

  /// max sixr of error bank
  inline constexpr unsigned int maxErrorBankWords = 56u;

} // namespace UTDAQ

#endif // _UTDAQDefinitions_H
