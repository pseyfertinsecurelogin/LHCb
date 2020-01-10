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
/** @file MCRichDigitHistoryCode.cpp
 *
 *  Implementation file for class : MCRichDigitHistoryCode
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    01/09/2006
 */
//-----------------------------------------------------------------------------

#include "Event/MCRichDigitHistoryCode.h"

std::ostream& LHCb::MCRichDigitHistoryCode::fillStream( std::ostream& s ) const {
  s << "[";

  // event history
  if ( signalEvent() ) s << " SignalEvent";
  if ( prevEvent() ) s << " PrevEvent";
  if ( prevPrevEvent() ) s << " PrevPrevEvent";
  if ( nextEvent() ) s << " NextEvent";
  if ( nextNextEvent() ) s << " NextNextEvent";

  // signal radiators
  if ( aerogelHit() ) s << " AeroCK";
  if ( c4f10Hit() ) s << " R1GasCK";
  if ( cf4Hit() ) s << " R2GasCK";

  // background radiators
  if ( gasQuartzCK() ) s << " GasQuartzCK";
  if ( hpdQuartzCK() ) s << " HPDQuartzCK";
  if ( nitrogenCK() ) s << " N2CK";
  if ( aeroFilterCK() ) s << " AeroFilterCK";

  // background history
  if ( scatteredHit() ) s << " RayleighScattered";
  if ( chargedTrack() ) s << " TrackHitHPD";
  if ( darkHit() ) s << " DarkHit";
  if ( chargeShareHit() ) s << " ChargeShare";
  if ( hpdSiBackscatter() ) s << " hpdSiBackscatter";
  if ( hpdReflQWPC() ) s << " hpdReflQWPC";
  if ( hpdReflChr() ) s << " hpdReflChr";
  if ( hpdReflAirQW() ) s << " hpdReflAirQW";
  if ( hpdReflAirPC() ) s << " hpdReflAirPC";
  if ( hpdReflSi() ) s << " hpdReflSi";
  if ( hpdReflKovar() ) s << " hpdReflKovar";
  if ( hpdReflKapton() ) s << " hpdReflKapton";
  if ( hpdReflPCQW() ) s << " hpdReflPCQW";

  return s << " ]";
}
