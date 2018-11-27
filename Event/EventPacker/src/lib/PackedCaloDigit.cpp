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

// local
#include "Event/PackedCaloDigit.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

void CaloDigitPacker::pack( const DataVector & digs,
                            PackedDataVector & pdigs ) const
{
  if ( isSupportedVer(pdigs.packingVersion()) )
  {
    pdigs.data().reserve( digs.size() );
    for ( const auto * dig : digs )
    {
      // make a new packed object
      pdigs.data().emplace_back( );
      auto & pdig = pdigs.data().back();
      // general
      pdig.key  = (dig->key()).all();
      pdig.e    = m_pack.energy( dig->e() );
    }
  }
}

void CaloDigitPacker::unpack( const PackedDataVector & pdigs,
                              DataVector             & digs ) const
{
  if ( isSupportedVer(pdigs.packingVersion()) )
  {
    digs.reserve( pdigs.data().size() );
    for ( const auto & pdig : pdigs.data() )
    {
      // make and save new digit container, with original key
      auto * dig  = new Data();
      digs.insert( dig, LHCb::CaloCellID(pdig.key) );
      // general
      dig->setE( m_pack.energy( pdig.e ) );
    }
  }
}

StatusCode CaloDigitPacker::check( const DataVector & dataA,
                                   const DataVector & dataB ) const
{
  StatusCode sc = StatusCode::SUCCESS;

  // Loop over data containers together and compare
  auto iA(dataA.begin()), iB(dataB.begin());
  for ( ; iA != dataA.end() && iB != dataB.end(); ++iA, ++iB )
  {
    if ( sc ) sc = check( **iA, **iB );
  }
  // Return final status
  return sc;
}

StatusCode CaloDigitPacker::check( const Data & dataA, const Data & dataB ) const{

  // assume OK from the start
  bool ok = true;

  // checker
  const DataPacking::DataChecks ch(parent());

  // checks here

  // key
  ok &= ch.compareInts( "key", dataA.key(), dataB.key() );
  // energy
  ok &= ch.compareDoubles( "type", dataA.e(), dataB.e() );


  // force printout for tests
  //ok = false;
  // If comparison not OK, print full information
  if ( !ok ){ const std::string loc = ( dataA.parent() && dataA.parent()->registry() ?
                              dataA.parent()->registry()->identifier() : "Not in TES" );
    parent().warning() << "Problem with CaloDigit data packing :-" << endmsg
                       << "  Original Digit key=" << dataA.key()
                       << " in '" << loc << "'" << endmsg
                       << dataA << endmsg
                       << "  Unpacked Digit" << endmsg
                       << dataB << endmsg;
  }
  return ( ok ? StatusCode::SUCCESS : StatusCode::FAILURE );
}
