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

#ifndef L0MUONKERNEL_BANKUTILITIES_H
#define L0MUONKERNEL_BANKUTILITIES_H 1

#include "L0MuonKernel/CandRegisterHandler.h"
#include <iostream>

namespace L0Muon {

  /** @file  BankUtilities BankUtilities.h L0MuonKernel/BankUtilities.h

  Functions with behaviour that may change from a bank version to an other.

  @author Julien Cogan
  @date 2007 August the 27th
  */

  /// Used to read a candidate from a L0Muon bank and fill a register (CtrFinal::decodeBank* )
  void writeCandInRegister( CandRegisterHandler* handler, unsigned int data, int icand, int bankVersion );

  /// Used to read a candidate from a register and fill a L0Muon bank (CtrFinal::rawBank* )
  unsigned int readCandFromRegister( CandRegisterHandler* handler, int icand, int bankVersion );

} // namespace L0Muon

#endif // L0MUONKERNEL_BANKUTILITIESH_H
