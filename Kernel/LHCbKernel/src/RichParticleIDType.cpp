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

//-----------------------------------------------------------------------------
/** @file RichParticleIDType.cpp
 *
 *  Implementation file for enumeration : Rich::ParticleIDType
 *
 *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
 *  @date   2002-06-19
 */
//-----------------------------------------------------------------------------

// local
#include "Kernel/RichParticleIDType.h"

// Text conversion for ParticleIDType enumeration
std::string
Rich::text( const Rich::ParticleIDType particle )
{
  switch ( particle )
  {
    case Rich::Electron: return "electron";
    case Rich::Muon: return "muon";
    case Rich::Pion: return "pion";
    case Rich::Kaon: return "kaon";
    case Rich::Proton: return "proton";
    case Rich::BelowThreshold: return "below_threshold";
    case Rich::Deuteron: return "deuteron";
    case Rich::Unknown: return "unknown";
    default: return "?"; // should never happen
  }
}

const Rich::Particles &
Rich::particles() noexcept
{
  static Rich::Particles pids = { Rich::Electron, Rich::Muon,     Rich::Pion,          Rich::Kaon,
                                  Rich::Proton,   Rich::Deuteron, Rich::BelowThreshold };
  return pids;
}
