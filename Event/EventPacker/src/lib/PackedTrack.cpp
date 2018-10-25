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
#include "Event/PackedTrack.h"

// Checks
#include "Event/PackedEventChecks.h"

// Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

using namespace LHCb;

//-----------------------------------------------------------------------------

void TrackPacker::pack( const Data & track,
                        PackedData & ptrack,
                        PackedDataVector & ptracks ) const
{
  // check version
  const auto ver = ptracks.version();
  if ( isSupportedVer(ver) )
  {

    ptrack.chi2PerDoF = m_pack.fltPacked( track.chi2PerDoF() );
    ptrack.nDoF       = track.nDoF();
    ptrack.flags      = track.flags();
    ptrack.likelihood = m_pack.fltPacked( track.likelihood() );
    ptrack.ghostProba = m_pack.fltPacked( track.ghostProbability() );

    //== Store the LHCbID as int
    ptrack.firstId = ptracks.ids().size();
    for ( const auto& id : track.lhcbIDs() ) { ptracks.ids().push_back( id.lhcbID() ); }
    ptrack.lastId  = ptracks.ids().size();
    if ( UNLIKELY( parent().msgLevel(MSG::DEBUG) ) )
    {
      parent().debug() << "Stored LHCbIDs from "
                       << ptrack.firstId << " to " << ptrack.lastId << endmsg;
    }

    //== Handle the states in the track
    ptrack.firstState = ptracks.states().size();
    for ( const auto* S : track.states() ) { convertState( *S, ptracks ); }
    ptrack.lastState = ptracks.states().size();
    if ( UNLIKELY( parent().msgLevel(MSG::DEBUG) ) )
    {
      parent().debug() << "Stored states from " << ptrack.firstState
                       << " to " << ptrack.lastState << endmsg;
    }

    //== Handles the ExtraInfo
    ptrack.firstExtra = ptracks.extras().size();
    for ( const auto& E : track.extraInfo() )
    {
      ptracks.extras().emplace_back( std::make_pair( E.first,
                                                     m_pack.fltPacked(E.second) ) );
    }
    ptrack.lastExtra = ptracks.extras().size();

  }

}

void TrackPacker::pack( const DataVector & tracks,
                        PackedDataVector & ptracks ) const
{
  // check version
  const auto ver = ptracks.version();
  if ( isSupportedVer(ver) )
  {
    ptracks.tracks().reserve(tracks.size());
    for ( const auto * track : tracks )
    {
      if ( !track ) continue;
      ptracks.tracks().emplace_back( LHCb::PackedTrack() );
      auto & ptrack = ptracks.tracks().back();
      ptrack.key = track->key();
      pack( *track, ptrack, ptracks );
    }
  }
}

void TrackPacker::convertState( const LHCb::State& state,
                                PackedDataVector & ptracks ) const
{
  ptracks.states().emplace_back( LHCb::PackedState() );
  auto & newState = ptracks.states().back();

  newState.flags = state.flags();

  newState.x    = m_pack.position( state.x()  );
  newState.y    = m_pack.position( state.y()  );
  newState.z    = m_pack.position( state.z()  );
  newState.tx   = m_pack.slope   ( state.tx() );
  newState.ty   = m_pack.slope   ( state.ty() );
  double p = 0.;
  if ( 0 != state.qOverP() ) p = 1./ state.qOverP();
  newState.p    = m_pack.energy  ( p );
  //== Get the coded value in case of saturation, to code properly the error later
  p = m_pack.energy( newState.p );

  // convariance Matrix
  std::vector<double> err;
  err.push_back( safe_sqrt( state.errX2() ) );
  err.push_back( safe_sqrt( state.errY2() ) );
  err.push_back( safe_sqrt( state.errTx2() ) );
  err.push_back( safe_sqrt( state.errTy2() ) );
  err.push_back( safe_sqrt( state.errQOverP2() ) );

  newState.cov_00 = m_pack.position( err[0] );
  newState.cov_11 = m_pack.position( err[1] );
  newState.cov_22 = m_pack.slope   ( err[2] );
  newState.cov_33 = m_pack.slope   ( err[3] );
  newState.cov_44 = m_pack.energy  ( 1.e5 * fabs(p) * err[4] ); //== 1.e5 * dp/p (*1.e2)

  newState.cov_10 = m_pack.fraction( state.covariance()(1,0), err[1]*err[0] );
  newState.cov_20 = m_pack.fraction( state.covariance()(2,0), err[2]*err[0] );
  newState.cov_21 = m_pack.fraction( state.covariance()(2,1), err[2]*err[1] );
  newState.cov_30 = m_pack.fraction( state.covariance()(3,0), err[3]*err[0] );
  newState.cov_31 = m_pack.fraction( state.covariance()(3,1), err[3]*err[1] );
  newState.cov_32 = m_pack.fraction( state.covariance()(3,2), err[3]*err[2] );
  newState.cov_40 = m_pack.fraction( state.covariance()(4,0), err[4]*err[0] );
  newState.cov_41 = m_pack.fraction( state.covariance()(4,1), err[4]*err[1] );
  newState.cov_42 = m_pack.fraction( state.covariance()(4,2), err[4]*err[2] );
  newState.cov_43 = m_pack.fraction( state.covariance()(4,3), err[4]*err[3] );
}

void TrackPacker::unpack( const PackedData       & ptrack,
                          Data                   & track,
                          const PackedDataVector & ptracks,
                          DataVector             & /* tracks */ ) const
{

  // Check version
  const auto ver = ptracks.version();
  if ( isSupportedVer(ver) )
  {

    // Try and detect changes that require the cached wrap IDs to be reset.
    // trigger a reset if a new packed container is detected, or if the
    // packed track object is the first in the packed container.
    // Not garanteed to be 100% perfect...
    if ( &ptracks != m_lastPackedDataV ||
         ( !ptracks.tracks().empty() && &ptracks.tracks().front() == &ptrack ) )
    {
      m_lastPackedDataV = &ptracks;
      resetWrappingCounts();
    }

    track.setChi2PerDoF( m_pack.fltPacked( ptrack.chi2PerDoF ) );
    track.setNDoF ( ptrack.nDoF  );
    track.setFlags( ptrack.flags );
    if ( ver > 2 )
    {
      track.setLikelihood(       m_pack.fltPacked( ptrack.likelihood ) );
      track.setGhostProbability( m_pack.fltPacked( ptrack.ghostProba ) );
    }

    // extract the first and last ID for the LHCbIDs
    int firstId = ptrack.firstId;
    int lastId  = ptrack.lastId;

    // Apply protection for short int wrapping
    if ( UNLIKELY( ver < 5 &&
                   ptracks.ids().size() > std::numeric_limits<uint8_t>::max() ) )
    {
      firstId = m_firstIdHigh + ptrack.firstId;
      lastId  = m_lastIdHigh  + ptrack.lastId;
      if ( lastId < firstId )
      { // we wrapped in the track
        parent().warning() << "** ID index wrapped, first/last Id = "
                           << firstId << " " << lastId << endmsg;
        m_lastIdHigh  += 0x10000;
        m_firstIdHigh += 0x10000;
        lastId = m_lastIdHigh + ptrack.lastId;
      }
    }

    // Check firstId and lastID are sane
    if ( UNLIKELY( lastId > static_cast<int>(ptracks.ids().size()) ||
                   firstId > lastId ) )
    {
      parent().Warning( "Attempted out-of-range access to packed LHCbIDs. "
                        "This is bad, do not ignore !" ).ignore();
      lastId = firstId = 0;
    }

    // Fill the LHCbIDs for this track
    std::vector<LHCb::LHCbID> lhcbids;
    lhcbids.reserve( lastId - firstId );
    std::transform( std::next( ptracks.ids().begin(), firstId ),
                    std::next( ptracks.ids().begin(), lastId  ),
                    std::back_inserter(lhcbids),
                    []( const int i ) { return LHCb::LHCbID(i); } );
    // schema change: sorting no longer needed when we write DSTs with sorted lhcbids
    if ( ver <= 1 )
    { std::sort( lhcbids.begin(), lhcbids.end() ); }
    track.addSortedToLhcbIDs( lhcbids );

    // extract the first and last indices for the states for this track
    int firstState = ptrack.firstState;
    int lastState  = ptrack.lastState;

    // protection for short int wrapping
    if ( UNLIKELY( ver < 5 &&
                   ptracks.states().size() > std::numeric_limits<uint8_t>::max() ) )
    {
      firstState = m_firstStateHigh + ptrack.firstState;
      lastState  = m_lastStateHigh  + ptrack.lastState;
      if ( lastState < firstState )
      {  // we wrapped in the track
        parent().warning() << "** State index wrapped, first/last = "
                           << firstState << " " << lastState << endmsg;
        m_lastStateHigh  += 0x10000;
        m_firstStateHigh += 0x10000;
        lastState = m_lastStateHigh + ptrack.lastState;
      }
    }

    // check the indices are sane
    if ( UNLIKELY( lastState > static_cast<int>(ptracks.states().size()) ||
                   firstState > lastState ) )
    {
      parent().Warning( "Attempted out-of-range access to packed States. "
                        "This is bad, do not ignore !" ).ignore();
      lastState = firstState = 0;
    }

    // convert the states
    std::for_each( std::next( ptracks.states().begin(), firstState ),
                   std::next( ptracks.states().begin(),  lastState ),
                   [&]( const LHCb::PackedState& s ) // could be auto& with C++14
                   { this->convertState( s, track ); }
                   );

    // extract the first and last extra info indices
    int firstExtra = ptrack.firstExtra;
    int lastExtra  = ptrack.lastExtra;

    // protect against short int wrapping
    if ( UNLIKELY( ver < 5 &&
                   ptracks.extras().size() > std::numeric_limits<uint8_t>::max() ) )
    {
      firstExtra = m_firstExtraHigh + ptrack.firstExtra;
      lastExtra  = m_lastExtraHigh  + ptrack.lastExtra;
      if ( lastExtra < firstExtra )
      { // we wrapped in the track
        parent().warning() << "** Extra index wrapped, first/last = "
                           << firstExtra << " " << lastExtra << endmsg;
        m_lastExtraHigh  += 0x10000;
        m_firstExtraHigh += 0x10000;
        lastExtra = m_lastExtraHigh + ptrack.lastExtra;
      }
    }

    // sanity checks on the indices
    if ( UNLIKELY( lastExtra > static_cast<int>(ptracks.extras().size()) ||
                   firstExtra > lastExtra ) )
    {
      parent().Warning( "Attempted out-of-range access to packed ExtraInfo. "
                        "This is bad, do not ignore !" ).ignore();
      lastExtra = firstExtra = 0;
    }

    // fill the extras
    std::for_each( std::next( ptracks.extras().begin(), firstExtra ),
                   std::next( ptracks.extras().begin(), lastExtra  ),
                   [&]( const std::pair<int,int>& info ) // could be auto& with C++14
                   { track.addInfo( info.first, m_pack.fltPacked(info.second) ); }
                   );

    //== Cleanup extraInfo and set likelihood/ghostProbability for old data
    if ( UNLIKELY( ver <= 2 ) )
    {
      track.eraseInfo(LHCb::Track::AdditionalInfo::PatQuality);
      track.eraseInfo(LHCb::Track::AdditionalInfo::Cand1stQPat);
      track.eraseInfo(LHCb::Track::AdditionalInfo::Cand2ndQPat);
      track.eraseInfo(LHCb::Track::AdditionalInfo::NCandCommonHits);
      track.eraseInfo(LHCb::Track::AdditionalInfo::Cand1stChi2Mat);
      track.eraseInfo(LHCb::Track::AdditionalInfo::Cand2ndChi2Mat);
      track.eraseInfo(LHCb::Track::AdditionalInfo::MatchChi2);
      track.eraseInfo(LHCb::Track::AdditionalInfo::TsaLikelihood);
      track.eraseInfo(LHCb::Track::AdditionalInfo::nPRVeloRZExpect);
      track.eraseInfo(LHCb::Track::AdditionalInfo::nPRVelo3DExpect);
      track.setLikelihood      ( track.info(   1, 999 ) ); // was the key of likelihood...
      track.setGhostProbability( track.info( 102, 999 ) ); // was the key of ghost probability
      track.eraseInfo(   1 );
      track.eraseInfo( 102 );
    }

  }

}

void TrackPacker::unpack( const PackedDataVector & ptracks,
                          DataVector             & tracks ) const
{
  // Check version
  const auto ver = ptracks.version();
  if ( isSupportedVer(ver) )
  {

    // reserve the required size
    tracks.reserve( ptracks.tracks().size() );

    // reset the cached variables to handle the wrapping bug ...
    resetWrappingCounts();

    // unpack
    for ( const auto& ptrack : ptracks.tracks() )
    {
      LHCb::Track* track = new LHCb::Track( );
      //parent().debug() << "Unpacked Track key=" << ptrack.key << endmsg;
      tracks.insert( track, ptrack.key );
      unpack( ptrack, *track, ptracks, tracks );
    }

  }
}

void TrackPacker::convertState ( const LHCb::PackedState& pSta,
                                 LHCb::Track & tra ) const
{
  LHCb::StateVector stateVector;
  stateVector.setX( m_pack.position( pSta.x ) );
  stateVector.setY( m_pack.position( pSta.y ) );
  stateVector.setZ( m_pack.position( pSta.z ) );
  stateVector.setTx( m_pack.slope( pSta.tx ) );
  stateVector.setTy( m_pack.slope( pSta.ty ) );
  const double pInv = ( 0 != pSta.p ? 1.0/m_pack.energy(pSta.p) : 0.0 );
  stateVector.setQOverP( pInv );

  LHCb::State state( stateVector );

  //== Fill covariance matrix
  const double err0 = m_pack.position( pSta.cov_00 );
  const double err1 = m_pack.position( pSta.cov_11 );
  const double err2 = m_pack.slope   ( pSta.cov_22 );
  const double err3 = m_pack.slope   ( pSta.cov_33 );
  const double err4 = m_pack.energy  ( pSta.cov_44 ) * fabs(pInv) * 1.e-5;
  auto & stateCov = *(const_cast<Gaudi::TrackSymMatrix*>(&state.covariance()));
  stateCov(0,0) = err0 * err0;
  stateCov(1,1) = err1 * err1;
  stateCov(2,2) = err2 * err2;
  stateCov(3,3) = err3 * err3;
  stateCov(4,4) = err4 * err4;
  stateCov(1,0) = err1 * err0 * m_pack.fraction( pSta.cov_10 );
  stateCov(2,0) = err2 * err0 * m_pack.fraction( pSta.cov_20 );
  stateCov(2,1) = err2 * err1 * m_pack.fraction( pSta.cov_21 );
  stateCov(3,0) = err3 * err0 * m_pack.fraction( pSta.cov_30 );
  stateCov(3,1) = err3 * err1 * m_pack.fraction( pSta.cov_31 );
  stateCov(3,2) = err3 * err2 * m_pack.fraction( pSta.cov_32 );
  stateCov(4,0) = err4 * err0 * m_pack.fraction( pSta.cov_40 );
  stateCov(4,1) = err4 * err1 * m_pack.fraction( pSta.cov_41 );
  stateCov(4,2) = err4 * err2 * m_pack.fraction( pSta.cov_42 );
  stateCov(4,3) = err4 * err3 * m_pack.fraction( pSta.cov_43 );

  state.setFlags( pSta.flags );

  tra.addToStates( state );
}

StatusCode TrackPacker::check( const Data & dataA,
                               const Data & dataB ) const
{

  if ( dataA.key() != dataB.key() )
  {
    parent().warning() << "Wrong key : old " <<  dataA.key() << " test " << dataB.key() << endmsg;
  }
  bool isOK = true;
  if ( 0. == dataB.chi2PerDoF() )
  {
    if( 0. != dataA.chi2PerDoF() ) isOK = false;
  }
  else if ( 1.e-7 < fabs( (dataA.chi2PerDoF()-dataB.chi2PerDoF())/dataB.chi2PerDoF() ) ) isOK = false;
  if ( 0   < abs( dataA.nDoF() - dataB.nDoF() ) ) isOK = false;
  if ( dataA.flags() != dataB.flags() )              isOK = false;
  if ( dataA.lhcbIDs().size() != dataB.lhcbIDs().size() ) isOK = false;
  if ( 0. == dataB.likelihood() )
  {
    if( 0. != dataA.likelihood() ) isOK = false;
  }
  else if ( 1.e-7 < fabs( (dataA.likelihood() - dataB.likelihood() )/ dataB.likelihood() ) ) isOK = false;
  if ( 0. == dataB.ghostProbability() )
  {
    if( 0. != dataA.ghostProbability() ) isOK = false;
  }
  else if ( 1.e-7 < fabs( (dataA.ghostProbability()-dataB.ghostProbability())/
                          dataB.ghostProbability()) ) isOK = false;
  unsigned int kk;
  for ( kk = 0 ; dataA.lhcbIDs().size() != kk ; ++kk )
  {
    if ( dataA.lhcbIDs()[kk].lhcbID() != dataB.lhcbIDs()[kk].lhcbID() )     isOK = false;
  }
  const LHCb::Track::ExtraInfo& oExtra = dataA.extraInfo();
  const LHCb::Track::ExtraInfo& tExtra = dataB.extraInfo();
  if ( oExtra.size() != tExtra.size() ) isOK = false;
  LHCb::Track::ExtraInfo::const_iterator oIt = oExtra.begin();
  LHCb::Track::ExtraInfo::const_iterator tIt = tExtra.begin();
  for ( kk = 0; tExtra.size() > kk; ++kk, ++oIt, ++tIt )
  {
    if ( (*oIt).first != (*tIt).first ) isOK = false;
    if ( 0. == (*tIt).second )
    {
      if( 0. != (*oIt).second ) isOK = false;
    }
    else if ( 1.e-7 < fabs( ((*oIt).second - (*tIt).second ) / (*tIt).second ) ) isOK = false;
  }

  if ( dataA.nStates() != dataB.nStates() ) isOK = false;

  if ( !isOK || MSG::DEBUG >= parent().msgLevel() )
  {
    parent().info() << "===== Track key " << dataA.key() << endmsg;
    parent().info() << format( "Old   chi2 %10.4f  nDoF %6i flags %8x nLhcbID %4d nExtra %4d  nStates %4d",
                               dataA.chi2PerDoF(), dataA.nDoF(), dataA.flags(),
                               dataA.lhcbIDs().size(), oExtra.size(), dataA.nStates() );
    parent().info() << format( " Likelihood %10.6f ghostProba %10.8f",
                               dataA.likelihood(), dataA.ghostProbability() )
                    << endmsg;
    parent().info() << format( "Test  chi2 %10.4f  nDoF %6i flags %8x nLhcbID %4d nExtra %4d  nStates %4d",
                               dataB.chi2PerDoF(), dataB.nDoF(), dataB.flags(),
                               dataB.lhcbIDs().size(), tExtra.size(), dataB.nStates() );
    parent().info() << format( " Likelihood %10.6f ghostProba %10.8f",
                               dataB.likelihood(), dataB.ghostProbability() )
                    << endmsg;
    for ( kk = 0 ; dataA.lhcbIDs().size() != kk ; ++kk ) {
      parent().info() << format( "   old ID %8x   new %8x",
                                 dataA.lhcbIDs()[kk].lhcbID(),
                                 dataB.lhcbIDs()[kk].lhcbID() ) << endmsg;
    }
    oIt = oExtra.begin();
    tIt = tExtra.begin();
    for ( kk = 0 ; oExtra.size() != kk ; ++kk, ++oIt, ++tIt ) {
      parent().info() << format( "   old Extra %5d %12.4f     new %5d %12.4f",
                                 (*oIt).first, (*oIt).second, (*tIt).first, (*tIt).second )
                      << endmsg;
    }
  }

  //== Compare the states. The track number won't be reported...

  for ( kk = 0; (dataA.nStates() > kk) && (dataB.nStates() > kk); ++kk )
  {
    compareStates( *dataA.states()[kk], *dataB.states()[kk] );
  }

  return ( isOK ? StatusCode::SUCCESS : StatusCode::FAILURE );
}

StatusCode TrackPacker::check( const DataVector & dataA,
                               const DataVector & dataB ) const
{
  StatusCode sc = StatusCode::SUCCESS;

  if ( dataA.version() != dataB.version() )
  {
    return parent().Error( "Version number mis-match" );
  }

  if ( dataA.size() != dataB.size() )
  {
    parent().err() << "Old Track size " << dataA.size()
                   << " differs form Test " << dataB.size()
                   << endmsg;
    return StatusCode::FAILURE;
  }

  LHCb::Tracks::const_iterator itOld  = dataA.begin();
  LHCb::Tracks::const_iterator itTest = dataB.begin();
  while ( dataA.end() != itOld )
  {
    LHCb::Track* oTrack = (*itOld++);
    LHCb::Track* tTrack = (*itTest++);
    if ( sc ) sc = check( *oTrack, *tTrack );
  }

  return sc;
}

void TrackPacker::compareStates ( const LHCb::State& oSta,
                                  const LHCb::State& tSta ) const
{
  bool isOK = true;

  if ( 5.e-5 < fabs( oSta.z() - tSta.z() ) ) isOK = false;
  if ( 5.e-5 < fabs( oSta.x() - tSta.x() ) ) isOK = false;
  if ( 5.e-5 < fabs( oSta.y() - tSta.y() ) ) isOK = false;
  if ( 5.e-8 < fabs( oSta.tx() - tSta.tx() ) ) isOK = false;
  if ( 5.e-8 < fabs( oSta.ty() - tSta.ty() ) ) isOK = false;
  const auto oP = safe_divide( 1.0, oSta.qOverP() );
  const auto tP = safe_divide( 1.0, tSta.qOverP() );
  if ( !( fabs(oSta.qOverP()) > 0 && fabs(tSta.qOverP()) > 0 ) ) 
  {
    parent().info() << "=== State has q/p == 0 ! "  << endmsg;
    parent().info() << "     old "
                    << format( " %12.5f %12.5f %12.5f %12.9f %12.9f %12.3f",
                               oSta.z(), oSta.x(), oSta.y(),
                               oSta.tx(), oSta.ty(), oP )
                    << endmsg;
    parent().info() << "    test "
                    << format( " %12.5f %12.5f %12.5f %12.9f %12.9f %12.3f",
                               tSta.z(), tSta.x(), tSta.y(),
                               tSta.tx(), tSta.ty(), tP )
                    << endmsg;
  }
  if ( 5.e-3 < fabs( oP - tP ) ) isOK = false;

  const auto oDiag = std::array {
      safe_sqrt(oSta.errX2()),
      safe_sqrt(oSta.errY2()),
      safe_sqrt(oSta.errTx2()),
      safe_sqrt(oSta.errTy2()),
      safe_sqrt(oSta.errQOverP2()) };

  const auto tDiag = std::array {
      safe_sqrt(tSta.errX2()),
      safe_sqrt(tSta.errY2()),
      safe_sqrt(tSta.errTx2()),
      safe_sqrt(tSta.errTy2()),    
      safe_sqrt(tSta.errQOverP2()) };

  if ( 5.e-5 < fabs( oDiag[0] - tDiag[0] ) ) isOK = false;
  if ( 5.e-5 < fabs( oDiag[1] - tDiag[1] ) ) isOK = false;
  if ( 5.e-8 < fabs( oDiag[2] - tDiag[2] ) ) isOK = false;
  if ( 5.e-8 < fabs( oDiag[3] - tDiag[3] ) ) isOK = false;
  //== Don't report problem if the term saturated: 2.e9 times energy scale 1.e-2
  if ( 5.    < fabs( oDiag[4]*oP*1.e5 - tDiag[4]*tP*1.e5 ) &&
       fabs( tDiag[4]*tP*1.e5 ) < 1.999e7                   ) isOK = false;

  const auto oFrac = std::array {
      safe_divide( oSta.covariance()(1,0) , oDiag[1] * oDiag[0] ),
      safe_divide( oSta.covariance()(2,0) , oDiag[2] * oDiag[0] ),
      safe_divide( oSta.covariance()(2,1) , oDiag[2] * oDiag[1] ),
      safe_divide( oSta.covariance()(3,0) , oDiag[3] * oDiag[0] ),
      safe_divide( oSta.covariance()(3,1) , oDiag[3] * oDiag[1] ),
      safe_divide( oSta.covariance()(3,2) , oDiag[3] * oDiag[2] ),
      safe_divide( oSta.covariance()(4,0) , oDiag[4] * oDiag[0] ),
      safe_divide( oSta.covariance()(4,1) , oDiag[4] * oDiag[1] ),
      safe_divide( oSta.covariance()(4,2) , oDiag[4] * oDiag[2] ),
      safe_divide( oSta.covariance()(4,3) , oDiag[4] * oDiag[3] ) };

  const auto tFrac = std::array {
      safe_divide( tSta.covariance()(1,0) , tDiag[1] * tDiag[0] ),
      safe_divide( tSta.covariance()(2,0) , tDiag[2] * tDiag[0] ),
      safe_divide( tSta.covariance()(2,1) , tDiag[2] * tDiag[1] ),
      safe_divide( tSta.covariance()(3,0) , tDiag[3] * tDiag[0] ),
      safe_divide( tSta.covariance()(3,1) , tDiag[3] * tDiag[1] ),
      safe_divide( tSta.covariance()(3,2) , tDiag[3] * tDiag[2] ),
      safe_divide( tSta.covariance()(4,0) , tDiag[4] * tDiag[0] ),
      safe_divide( tSta.covariance()(4,1) , tDiag[4] * tDiag[1] ),
      safe_divide( tSta.covariance()(4,2) , tDiag[4] * tDiag[2] ),
      safe_divide( tSta.covariance()(4,3) , tDiag[4] * tDiag[3] ) };

  unsigned int kk;
  for ( kk = 0 ; oFrac.size() > kk ; ++kk )
  {
    if ( 2.e-5 < fabs( oFrac[kk] - tFrac[kk] ) ) isOK = false;
  }

  if ( MSG::VERBOSE >= parent().msgLevel() ) isOK = false; //== force printing
  if ( !isOK )
  {
    parent().info() << "=== State differ: " << endmsg;
    parent().info() << "     old "
                    << format( " %12.5f %12.5f %12.5f %12.9f %12.9f %12.3f",
                               oSta.z(), oSta.x(), oSta.y(),
                               oSta.tx(), oSta.ty(), oP )
                    << endmsg;
    parent().info() << "    test "
                    << format( " %12.5f %12.5f %12.5f %12.9f %12.9f %12.3f",
                               tSta.z(), tSta.x(), tSta.y(),
                               tSta.tx(), tSta.ty(), tP )
                    << endmsg;
    parent().info() << format( " old Diag %10.5f %10.5f %12.9f %12.9f %12.3f",
                               oDiag[0], oDiag[1], oDiag[2], oDiag[3], oDiag[4]*oP*1.e5 )
                    << endmsg;
    parent().info() << format( "test Diag %10.5f %10.5f %12.9f %12.9f %12.3f",
                               tDiag[0], tDiag[1], tDiag[2], tDiag[3], tDiag[4]*tP*1.e5 )
                    << endmsg;
    parent().info() << " old Frac ";
    for ( kk = 0 ; oFrac.size() > kk ; ++kk )
    {
      parent().info() << format( " %8.5f", oFrac[kk] );
    }
    parent().info() << endmsg << "test Frac ";
    for ( kk = 0 ; tFrac.size() > kk ; ++kk )
    {
      parent().info() << format( " %8.5f", tFrac[kk] );
    }
    parent().info() << endmsg;
  }

}
