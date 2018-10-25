/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

//=============================================================================================
/** @file RichDAQL1IngressHeader.cpp
 *
 *  Implementation file for RICH DAQ utility class : Rich::DAQ::L1IngressHeader
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   619/01/2007
 */
//=============================================================================================

#include "RichUtils/RichDAQL1IngressHeader.h"

using namespace Rich::DAQ;

void
L1IngressHeader::setHPDsActive( const L1IngressInputs &inputs )
{
  for ( const auto &input : inputs ) { setHPDActive( input ); }
}

void
L1IngressHeader::activeHPDInputs( L1IngressInputs &inputs ) const
{
  inputs.clear();
  inputs.reserve( numActiveHPDs() );
  const auto hpdBits = activeHPDbits();
  for ( auto i = 0u; i < HPD::NumL1InputsPerIngress; ++i )
  {
    if ( isBitOn( hpdBits, i ) ) inputs.emplace_back( i );
  }
}

void
L1IngressHeader::inactiveHPDInputs( L1IngressInputs &inputs ) const
{
  inputs.clear();
  const auto hpdBits = activeHPDbits();
  for ( auto i = 0u; i < HPD::NumL1InputsPerIngress; ++i )
  {
    if ( !isBitOn( hpdBits, i ) ) inputs.emplace_back( i );
  }
}

std::ostream &
L1IngressHeader::fillStream( std::ostream &os ) const
{
  os << "[ ID=" << ingressID() << " EvtID=" << eventID() << " BXID=" << bxID() << " ActiveHPDs=";
  L1InputWithinIngress in( 0 );
  for ( ; in < L1InputWithinIngress( HPD::NumL1InputsPerIngress ); ++in )
  { os << isHPDActive( in ); }
  os << " Truncated=" << hpdsSuppressed();
  return os << " ]";
}
