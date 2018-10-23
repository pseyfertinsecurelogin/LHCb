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
// local
#include "Event/PackedRichPID.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

void RichPIDPacker::pack( const Data & pid,
                          PackedData & ppid,
                          PackedDataVector & ppids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    ppid.pidResultCode = (int)pid.pidResultCode();
    ppid.dllEl = m_pack.deltaLL( pid.particleDeltaLL(Rich::Electron) );
    ppid.dllMu = m_pack.deltaLL( pid.particleDeltaLL(Rich::Muon)     );
    ppid.dllPi = m_pack.deltaLL( pid.particleDeltaLL(Rich::Pion)     );
    ppid.dllKa = m_pack.deltaLL( pid.particleDeltaLL(Rich::Kaon)     );
    ppid.dllPr = m_pack.deltaLL( pid.particleDeltaLL(Rich::Proton)   );
    if ( ver > 0 ) ppid.dllBt = m_pack.deltaLL( pid.particleDeltaLL(Rich::BelowThreshold) );
    if ( ver > 3 ) ppid.dllDe = m_pack.deltaLL( pid.particleDeltaLL(Rich::Deuteron) );
    if ( nullptr != pid.track() )
    {
      ppid.track = ( UNLIKELY( ver < 3 ) ?
                     m_pack.reference32( &ppids,
                                         pid.track()->parent(),
                                         pid.track()->key() ) :
                     m_pack.reference64( &ppids,
                                         pid.track()->parent(),
                                         pid.track()->key() ) );
    }
  }
}

void RichPIDPacker::pack( const DataVector & pids,
                          PackedDataVector & ppids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    ppids.data().reserve( pids.size() );
    for ( const auto * pid : pids )
    {
      // create new packed PID object in container and get reference to it.
      ppids.data().emplace_back( PackedData() );
      auto & ppid = ppids.data().back();
      // fill the key
      ppid.key = pid->key();
      // fill the rest of the data
      pack( *pid, ppid, ppids );
    }
  }
}

void RichPIDPacker::unpack( const PackedData       & ppid,
                            Data                   & pid,
                            const PackedDataVector & ppids,
                            DataVector             & pids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    pid.setPidResultCode( ppid.pidResultCode );
    pid.setParticleDeltaLL( Rich::Electron,  (float)m_pack.deltaLL(ppid.dllEl) );
    pid.setParticleDeltaLL( Rich::Muon,      (float)m_pack.deltaLL(ppid.dllMu) );
    pid.setParticleDeltaLL( Rich::Pion,      (float)m_pack.deltaLL(ppid.dllPi) );
    pid.setParticleDeltaLL( Rich::Kaon,      (float)m_pack.deltaLL(ppid.dllKa) );
    pid.setParticleDeltaLL( Rich::Proton,    (float)m_pack.deltaLL(ppid.dllPr) );
    // Below threshold information only available in packed version 1 onwards
    if ( ver > 0 ) pid.setParticleDeltaLL( Rich::BelowThreshold,
                                           (float)m_pack.deltaLL(ppid.dllBt) );
    // Deuteron information only available in packed version 4 onwards
    if ( ver > 3 ) pid.setParticleDeltaLL( Rich::Deuteron,
                                           (float)m_pack.deltaLL(ppid.dllDe) );
    if ( -1 != ppid.track )
    {
      int hintID(0), key(0);
      // Smart ref packing changed to 64 bit in packing version 3 onwards
      if ( ( ver >= 3 && m_pack.hintAndKey64(ppid.track,&ppids,&pids,hintID,key) ) ||
           ( ver <  3 && m_pack.hintAndKey32(ppid.track,&ppids,&pids,hintID,key) ) )
      {
        SmartRef<LHCb::Track> ref(&pids,hintID,key);
        pid.setTrack( ref );
      }
      else { parent().Error( "Corrupt RichPID Track SmartRef detected." ).ignore(); }
    }
    // If the packing version is pre-Deuteron, check the BestPID field and correct for
    // the fact the numerical value of the Below Threshold enum changed when Deuteron
    // was added.
    if ( ver < 4 && Rich::Deuteron == pid.bestParticleID() )
    {
      pid.setBestParticleID( Rich::BelowThreshold );
    }
  }
}

void RichPIDPacker::unpack( const PackedDataVector & ppids,
                            DataVector             & pids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    pids.reserve( ppids.data().size() );
    for ( const auto & ppid : ppids.data() )
    {
      // make and save new pid in container
      auto * pid  = new Data();
      // key information only available in version 2 onwards
      if ( ver > 1 ) { pids.insert( pid, ppid.key ); }
      else           { pids.add( pid ); }
      // Fill data from packed object
      unpack( ppid, *pid, ppids, pids );
    }
    // Check for 'RichFuture' bug where original RichPID version was not 
    // properly propagated to the packed data objects
    // Easy to detect if packing version > 3 and data version = 0 
    if ( ver > 3 && 0 == pids.version() )
    {
      // only warn if the packed location is not empty. This is because its
      // possible for the HLT to create a default container with V0 but packing V4.
      if ( !ppids.data().empty() )
      {
        parent().Warning( "Incorrect data version 0 for packing version > 3."
                          " Correcting data to version 2.", StatusCode::SUCCESS ).ignore();
      }
      // reset to V2
      pids.setVersion(2);
    }
  }
}

StatusCode RichPIDPacker::check( const Data & dataA,
                                 const Data & dataB ) const
{
  // checker
  const DataPacking::DataChecks ch(parent());

  // assume OK from the start
  bool ok = true;
  // key
  ok &= ch.compareInts( "Key", dataA.key(), dataB.key() );
  // History code
  ok &= ch.compareInts( "PIDCode", dataA.pidResultCode(), dataB.pidResultCode() );
  // Track reference
  ok &= ch.comparePointers( "Track", dataA.track(), dataB.track() );
  // DLLs
  ok &= dataA.particleLLValues().size() == dataB.particleLLValues().size();
  auto iLLA(dataA.particleLLValues().begin());
  auto iLLB(dataB.particleLLValues().begin());
  for ( ; iLLA != dataA.particleLLValues().end() && iLLB != dataB.particleLLValues().end();
        ++iLLA, ++iLLB )
  {
    ok &= ch.compareDoubles( "Delta(LL)", *iLLA, *iLLB );
  }

  // force printout for tests
  //ok = false;
  // If comparison not OK, print full information
  if ( !ok )
  {
    const std::string loc = ( dataA.parent() && dataA.parent()->registry() ?
                              dataA.parent()->registry()->identifier() : "Not in TES" );
    parent().warning() << "Problem with RichPID data packing :-" << endmsg
                       << "  Original PID key=" << dataA.key()
                       << " in '" << loc << "'" << endmsg
                       << dataA << endmsg
                       << "  Unpacked PID" << endmsg
                       << dataB << endmsg;
  }

  return ( ok ? StatusCode::SUCCESS : StatusCode::FAILURE );
}

StatusCode RichPIDPacker::check( const DataVector & dataA,
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
