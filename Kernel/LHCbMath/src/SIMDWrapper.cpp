/*****************************************************************************\
* (c) Copyright 2019 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#include "LHCbMath/SIMDWrapper.h"
namespace SIMDWrapper {
  namespace avx2 {
    InstructionSet stackInstructionSet() { return avx2::instructionSet(); }
  } // namespace avx2
  namespace avx256 {
    InstructionSet stackInstructionSet() { return avx256::instructionSet(); }
  } // namespace avx256
  namespace avx512 {
    InstructionSet stackInstructionSet() { return avx512::instructionSet(); }
  } // namespace avx512
  namespace best {
    InstructionSet stackInstructionSet() { return best::instructionSet(); }
  } // namespace best
} // namespace SIMDWrapper
