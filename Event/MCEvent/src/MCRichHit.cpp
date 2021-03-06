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
/** @file MCRichHit.cpp
 *
 *  Implementation file for class : MCRichHit
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    26/01/2008
 */
//-----------------------------------------------------------------------------

// local
#include "Event/MCRichHit.h"

LHCb::MCRichDigitHistoryCode LHCb::MCRichHit::mcRichDigitHistoryCode() const {
  // make a default history code object
  LHCb::MCRichDigitHistoryCode hist;

  // set signal flags
  hist.setAerogelHit( Rich::Aerogel == radiator() );
  hist.setC4f10Hit( Rich::C4F10 == radiator() );
  hist.setCf4Hit( Rich::CF4 == radiator() );
  hist.setHasSignal( isSignal() );

  // set background flags
  hist.setScatteredHit( scatteredPhoton() );
  hist.setChargedTrack( chargedTrack() );
  hist.setGasQuartzCK( gasQuartzCK() );
  hist.setHpdQuartzCK( hpdQuartzCK() );
  hist.setNitrogenCK( nitrogenCK() );
  hist.setAeroFilterCK( aeroFilterCK() );
  hist.setHpdSiBackscatter( hpdSiBackscatter() );
  hist.setHpdReflQWPC( hpdReflQWPC() );
  hist.setHpdReflChr( hpdReflChr() );
  hist.setHpdReflAirQW( hpdReflAirQW() );
  hist.setHpdReflAirPC( hpdReflAirPC() );
  hist.setHpdReflSi( hpdReflSi() );
  hist.setHpdReflKovar( hpdReflKovar() );
  hist.setHpdReflKapton( hpdReflKapton() );
  hist.setHpdReflPCQW( hpdReflPCQW() );
  hist.setRadScintillation( radScintillation() );

  // return final flag
  return hist;
}

std::ostream& LHCb::MCRichHit::fillStream( std::ostream& s ) const {
  s << "{ " << Rich::text( radiator() ) << " " << m_sensDetID;

  s << " entry=" << m_entry << " energy=" << m_energy << " tof=" << m_timeOfFlight << " mcPart=" << mcParticle();

  // background flags
  if ( scatteredPhoton() ) s << " RayleighScattered";
  if ( chargedTrack() ) s << " TrackHitHPD";
  if ( hpdSiBackscatter() ) s << " hpdSiBackscatter";
  if ( hpdReflQWPC() ) s << " hpdReflQWPC";
  if ( hpdReflChr() ) s << " hpdReflChr";
  if ( hpdReflAirQW() ) s << " hpdReflAirQW";
  if ( hpdReflAirPC() ) s << " hpdReflAirPC";
  if ( hpdReflSi() ) s << " hpdReflSi";
  if ( hpdReflKovar() ) s << " hpdReflKovar";
  if ( hpdReflKapton() ) s << " hpdReflKapton";
  if ( hpdReflPCQW() ) s << " hpdReflPCQW";

  return s << " }";
}
