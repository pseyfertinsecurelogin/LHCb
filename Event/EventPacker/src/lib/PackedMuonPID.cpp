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
#include "Event/PackedMuonPID.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

void MuonPIDPacker::pack( const Data & pid,
                          PackedData & ppid,
                          PackedDataVector & ppids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    ppid.MuonLLMu = m_pack.deltaLL(pid.MuonLLMu());
    ppid.MuonLLBg = m_pack.deltaLL(pid.MuonLLBg());
    if(ver > 2){
      ppid.chi2Corr = m_pack.fltPacked(pid.chi2Corr());
      ppid.muonMVA1 = m_pack.mva(pid.muonMVA1());
      ppid.muonMVA2 = m_pack.mva(pid.muonMVA2());
      ppid.muonMVA3 = m_pack.mva(pid.muonMVA3());
      ppid.muonMVA4 = m_pack.mva(pid.muonMVA4());
    }
    ppid.nShared  = (int)pid.nShared();
    ppid.status   = (int)pid.Status();
    if ( pid.idTrack() )
    {
      ppid.idtrack = ( UNLIKELY( 1 >= ver ) ?
                       m_pack.reference32( &ppids,
                                           pid.idTrack()->parent(),
                                           pid.idTrack()->key() ) :
                       m_pack.reference64( &ppids,
                                           pid.idTrack()->parent(),
                                           pid.idTrack()->key() ) );
    }
    if ( pid.muonTrack() )
    {
      ppid.mutrack = ( UNLIKELY( 1 >= ver ) ?
                       m_pack.reference32( &ppids,
                                           pid.muonTrack()->parent(),
                                           pid.muonTrack()->key() ) :
                       m_pack.reference64( &ppids,
                                           pid.muonTrack()->parent(),
                                           pid.muonTrack()->key() ) );
    }
  }
}

void MuonPIDPacker::pack( const DataVector & pids,
                          PackedDataVector & ppids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    ppids.data().reserve( pids.size() );
    for ( const auto * pid : pids )
    {
      ppids.data().emplace_back( );
      auto & ppid = ppids.data().back();
      // save the key
      ppid.key = pid->key();
      // fill the rest of the data
      pack( *pid, ppid, ppids );
    }
  }
}

void MuonPIDPacker::unpack( const PackedData       & ppid,
                            Data                   & pid,
                            const PackedDataVector & ppids,
                            DataVector             & pids ) const
{
  const auto ver = ppids.packingVersion();
  if ( isSupportedVer(ver) )
  {
    pid.setMuonLLMu( m_pack.deltaLL(ppid.MuonLLMu) );
    pid.setMuonLLBg( m_pack.deltaLL(ppid.MuonLLBg) );
    if(ver > 2){
      pid.setChi2Corr( m_pack.fltPacked(ppid.chi2Corr) );
      pid.setMuonMVA1( m_pack.mva(ppid.muonMVA1) );
      pid.setMuonMVA2( m_pack.mva(ppid.muonMVA2) );
      pid.setMuonMVA3( m_pack.mva(ppid.muonMVA3) );
      pid.setMuonMVA4( m_pack.mva(ppid.muonMVA4) );
    }
    pid.setNShared( ppid.nShared );
    pid.setStatus( ppid.status );
    if ( -1 != ppid.idtrack )
    {
      int hintID(0), key(0);
      if ( ( 1 <  ver && m_pack.hintAndKey64(ppid.idtrack,&ppids,&pids,hintID,key) ) ||
           ( 1 >= ver && m_pack.hintAndKey32(ppid.idtrack,&ppids,&pids,hintID,key) ) )
      {
        SmartRef<LHCb::Track> ref(&pids,hintID,key);
        pid.setIDTrack( ref );
      }
      else { parent().Error( "Corrupt MuonPID Track SmartRef detected." ).ignore(); }
    }
    if ( -1 != ppid.mutrack )
    {
      int hintID(0), key(0);
      if ( ( 1 <  ver && m_pack.hintAndKey64(ppid.mutrack,&ppids,&pids,hintID,key) ) ||
           ( 1 >= ver && m_pack.hintAndKey32(ppid.mutrack,&ppids,&pids,hintID,key) ) )
      {
        SmartRef<LHCb::Track> ref(&pids,hintID,key);
        pid.setMuonTrack( ref );
      }
      else { parent().Error( "Corrupt MuonPID MuTrack SmartRef detected." ).ignore(); }
    }
  }
}

void MuonPIDPacker::unpack( const PackedDataVector & ppids,
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
      if ( 0 == ver ) { pids.add( pid ); }
      else            { pids.insert( pid, ppid.key ); }
      // Fill data from packed object
      unpack( ppid, *pid, ppids, pids );
    }
  }
}

StatusCode MuonPIDPacker::check( const Data & dataA,
                                 const Data & dataB ) const
{
  // checker
  const DataPacking::DataChecks ch(parent());

  // assume OK from the start
  bool ok = true;

  // key
  ok &= ch.compareInts( "Key", dataA.key(), dataB.key() );
  // History code
  ok &= ch.compareInts( "Status", dataA.Status(), dataB.Status() );
  // Track references
  ok &= ch.comparePointers( "Track", dataA.idTrack(), dataB.idTrack() );
  ok &= ch.comparePointers( "MuonTrack", dataA.muonTrack(), dataB.muonTrack() );
  // DLLs
  ok &= ch.compareDoubles( "MuonLLMu", dataA.MuonLLMu(), dataB.MuonLLMu() );
  ok &= ch.compareDoubles( "MuonLLBg", dataA.MuonLLBg(), dataB.MuonLLBg() );
  ok &= ch.compareDoubles( "chi2Corr", dataA.chi2Corr(), dataB.chi2Corr() );
  ok &= ch.compareDoubles( "muonMVA1", dataA.muonMVA1(), dataB.muonMVA1() );
  ok &= ch.compareDoubles( "muonMVA2", dataA.muonMVA2(), dataB.muonMVA2() );
  ok &= ch.compareDoubles( "muonMVA3", dataA.muonMVA3(), dataB.muonMVA3() );
  ok &= ch.compareDoubles( "muonMVA4", dataA.muonMVA4(), dataB.muonMVA4() );

  // force printout for tests
  //ok = false;
  // If comparison not OK, print full information
  if ( !ok )
  {
    const std::string loc = ( dataA.parent() && dataA.parent()->registry() ?
                              dataA.parent()->registry()->identifier() : "Not in TES" );
    parent().warning() << "Problem with MuonPID data packing :-" << endmsg
                       << "  Original PID key=" << dataA.key()
                       << " in '" << loc << "'" << endmsg
                       << dataA << endmsg
                       << "  Unpacked PID" << endmsg
                       << dataB << endmsg;
  }

  return ( ok ? StatusCode::SUCCESS : StatusCode::FAILURE );
}

StatusCode MuonPIDPacker::check( const DataVector & dataA,
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
