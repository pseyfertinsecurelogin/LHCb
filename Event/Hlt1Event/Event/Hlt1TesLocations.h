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

#pragma once

#include <string>


namespace LHCb::HLT1::TESLocations {
// out of PrPixel
    [[maybe_unused]] inline const char* PrPixelHits = "Rec/HLT1/PrPixelHits";
    [[maybe_unused]] inline const char* PrPixelBeamStates = "Rec/HLT1/PrPixelBeamStates";
    [[maybe_unused]] inline const char* PrPixelEndStates = "Rec/HLT1/PrPixelEndStates";
    [[maybe_unused]] inline const char* PrPixelChiSquares = "Rec/HLT1/PrPixelChiSquares";

// out of IpFilter
    [[maybe_unused]] inline const char* PrPixelSelection = "Rec/HLT1/PrPixel_Selection";

// out of PrVeloUT
    [[maybe_unused]] inline const char* PrVeloUtUtHits = "Rec/HLT1/PrVeloUtHits";
    [[maybe_unused]] inline const char* PrVeloUtVeloHits = "Rec/HLT1/PrVeloUtVeloHits";
    [[maybe_unused]] inline const char* PrVeloUtStates = "Rec/HLT1/PrVeloUtStates";
} // namespace LHCb::HL1::TESLocations
