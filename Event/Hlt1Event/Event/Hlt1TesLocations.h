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
    [[maybe_unused]] inline const char* PrPixelHits = "Rec/HLT1/PrPixelHits";
    [[maybe_unused]] inline const char* PrPixelBeamStates = "Rec/HLT1/PrPixelBeamStates";
    [[maybe_unused]] inline const char* PrPixelEndStates = "Rec/HLT1/PrPixelEndStates";
    [[maybe_unused]] inline const char* PrPixelVeloTracks = "Rec/HLT1/PrPixelVeloTracks";
    [[maybe_unused]] inline const char* PrPixelChiSquares = "Rec/HLT1/PrPixelChiSquares";

    [[maybe_unused]] inline const char* PrPixelHitsSelection = "Rec/HLT1/PrPixelHits_Selection";
    [[maybe_unused]] inline const char* PrPixelBeamStatesSelection = "Rec/HLT1/PrPixelBeamStates_Selection";
    [[maybe_unused]] inline const char* PrPixelEndStatesSelection = "Rec/HLT1/PrPixelEndStates_Selection";
    [[maybe_unused]] inline const char* PrPixelVeloTracksSelection = "Rec/HLT1/PrPixelVeloTracks_Selection";

    [[maybe_unused]] inline const char* PrVeloUtHits = "Rec/HLT1/PrVeloUtHits";
    [[maybe_unused]] inline const char* PrVeloUtVeloStates = "Rec/HLT1/PrVeloUtVeloStates";
    [[maybe_unused]] inline const char* PrVeloUtUtStates = "Rec/HLT1/PrVeloUtUtStates";
    [[maybe_unused]] inline const char* PrVeloUtTracks = "Rec/HLT1/PrVeloUtTracks";
} // namespace LHCb::HL1::TESLocations