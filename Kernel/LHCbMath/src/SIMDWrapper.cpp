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
  InstructionSet type_map<InstructionSet::Scalar>::stackInstructionSet() { return scalar::instructionSet(); }
  InstructionSet type_map<InstructionSet::SSE>::stackInstructionSet() { return sse::instructionSet(); }
  InstructionSet type_map<InstructionSet::AVX2>::stackInstructionSet() { return avx2::instructionSet(); }
  InstructionSet type_map<InstructionSet::AVX256>::stackInstructionSet() { return avx256::instructionSet(); }
  InstructionSet type_map<InstructionSet::AVX512>::stackInstructionSet() { return avx512::instructionSet(); }
  InstructionSet type_map<InstructionSet::Best>::stackInstructionSet() { return best::instructionSet(); }
} // namespace SIMDWrapper
