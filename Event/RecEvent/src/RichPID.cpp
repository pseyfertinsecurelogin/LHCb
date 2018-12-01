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
/** @file RichPID.cpp
 *
 *  Implementation file for class : LHCb::RichPID
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2002-06-10
 */
//-----------------------------------------------------------------------------

// STL
#include <sstream>
#include <algorithm>
#include <string>

// RichEvent includes
#include "Event/RichPID.h"

// Boost
#include "boost/format.hpp"

std::string LHCb::RichPID::pidType() const
{
  std::string hist;
  int cnt = 0;
  if ( this->offlineGlobal() ) {                       hist+="OfflineGlobal"; ++cnt; }
  if ( this->offlineLocal()  ) { hist+=(cnt>0?"+":""); hist+="OfflineLocal";  ++cnt; }
  if ( this->ringRefit()     ) { hist+=(cnt>0?"+":""); hist+="RingRefit";     ++cnt; }
  if ( this->hltGlobal()     ) { hist+=(cnt>0?"+":""); hist+="HltGlobal";     ++cnt; }
  if ( this->hltLocal()      ) { hist+=(cnt>0?"+":""); hist+="HltLocal";      ++cnt; }
  return hist;
}

bool LHCb::RichPID::isAboveThreshold( const Rich::ParticleIDType type ) const
{
  return ( Rich::Electron == type ? this->electronHypoAboveThres() :
           Rich::Muon     == type ? this->muonHypoAboveThres()     :
           Rich::Pion     == type ? this->pionHypoAboveThres()     :
           Rich::Kaon     == type ? this->kaonHypoAboveThres()     :
           Rich::Proton   == type ? this->protonHypoAboveThres()   :
           Rich::Deuteron == type ? this->deuteronHypoAboveThres() :
           false );
}

void LHCb::RichPID::setAboveThreshold( const Rich::ParticleIDType type,
                                       const bool flag )
{
  if      ( Rich::Electron == type ) { this->setElectronHypoAboveThres(flag); }
  else if ( Rich::Muon     == type ) { this->setMuonHypoAboveThres(flag);     }
  else if ( Rich::Pion     == type ) { this->setPionHypoAboveThres(flag);     }
  else if ( Rich::Kaon     == type ) { this->setKaonHypoAboveThres(flag);     }
  else if ( Rich::Proton   == type ) { this->setProtonHypoAboveThres(flag);   }
  else if ( Rich::Deuteron == type ) { this->setDeuteronHypoAboveThres(flag); }
}

bool LHCb::RichPID::traversedRadiator(const Rich::RadiatorType radiator) const
{
  return ( Rich::Aerogel  == radiator ? this->usedAerogel()  :
           Rich::Rich1Gas == radiator ? this->usedRich1Gas() :
           Rich::Rich2Gas == radiator ? this->usedRich2Gas() :
           false );
}

std::ostream& LHCb::RichPID::fillStream( std::ostream& s ) const
{
  s << "[ ";

  // Formatting strings
  const std::string fF = "%6.2f"; // floats
  const std::string iF = "%5i";   // ints

  // PID type
  s << "Key" << boost::format(iF) % key() << " " << pidType();

  // Track info
  if ( track() )
  {
    // Track type
    std::ostringstream tType;
    tType << track()->type();
    const unsigned int minTkTypeSize = 10;
    auto S = tType.str();
    if ( S.size() < minTkTypeSize ) { S.resize(minTkTypeSize,' '); }
    // print
    s << " | Tk " << boost::format(iF) % track()->key() 
      << " " << S
      << " " << boost::format(fF) % (track()->p()/Gaudi::Units::GeV)
      << " GeV";
  }
  else
  {
    s << " | NO ASSOCIATED TRACK";
  }

  // Active radiators
  std::ostringstream rads;
  if ( usedAerogel()  ) { rads << " " << Rich::text(Rich::Aerogel);  }
  if ( usedRich1Gas() ) { rads << " " << Rich::text(Rich::Rich1Gas); }
  if ( usedRich2Gas() ) { rads << " " << Rich::text(Rich::Rich2Gas); }
  const unsigned int minRadsSize = 18; // nice for RICH1 and RICH2 gas
  auto S = rads.str();
  if ( S.size() < minRadsSize ) { S.resize(minRadsSize,' '); }
  s << " |" << S;

  // Mass thresholds
  s << " | Thres ";
  for ( const auto pid : Rich::particles() )
  {
    s << ( isAboveThreshold(pid) ? "T" : "F" );
  }

  // DLL values
  s << " | DLLs ";
  for ( const auto pid : Rich::particles() )
  {
    s << " " << boost::format(fF) % particleDeltaLL(pid);
  }

  // Best ID
  s << " | " << bestParticleID();

  // return message stream
  return s << " ]";
}
