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
#include "Event/TwoProngVertex.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareTwoProngVertex
//
// 2009-01-21 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareTwoProngVertex CompareTwoProngVertex.h
 *
 *
 *  @author Olivier Callot
 *  @date   2009-01-21
 */
struct CompareTwoProngVertex
    : Gaudi::Functional::Consumer<void( LHCb::TwoProngVertices const&, LHCb::TwoProngVertices const& )> {

  CompareTwoProngVertex( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::TwoProngVertexLocation::Default},
                  KeyValue{"TestName", LHCb::TwoProngVertexLocation::Default + "Test"}}} {}

  void operator()( LHCb::TwoProngVertices const& old, LHCb::TwoProngVertices const& test ) const override {

    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

    if ( old.size() != test.size() ) {
      err() << "Old TwoProngVertex size " << old.size() << " differs form Test " << test.size() << endmsg;
      throw GaudiException{"Sizes differ", "CompareTwoProngVertex", StatusCode::FAILURE};
    }
    auto itOld  = old.begin();
    auto itTest = test.begin();

    while ( itOld != old.end() ) {
      LHCb::TwoProngVertex* oVtx = ( *itOld++ );
      LHCb::TwoProngVertex* tVtx = ( *itTest++ );
      if ( oVtx->key() != tVtx->key() ) {
        warning() << "Wrong key : old " << oVtx->key() << " test " << tVtx->key() << endmsg;
      }
      bool isOK = true;
      if ( 1.e-7 < fabs( ( oVtx->chi2() - tVtx->chi2() ) / oVtx->chi2() ) ) isOK = false;
      if ( oVtx->nDoF() != tVtx->nDoF() ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().x() - tVtx->position().x() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().y() - tVtx->position().y() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().z() - tVtx->position().z() ) ) isOK = false;

      if ( 5.e-8 < fabs( oVtx->momA()[0] - tVtx->momA()[0] ) ) isOK = false;
      if ( 5.e-8 < fabs( oVtx->momA()[1] - tVtx->momA()[1] ) ) isOK = false;
      double oPA = 1. / oVtx->momA()[2];
      double tPA = 1. / tVtx->momA()[2];
      if ( 5.e-3 < fabs( oPA - tPA ) ) isOK = false;

      if ( 5.e-8 < fabs( oVtx->momB()[0] - tVtx->momB()[0] ) ) isOK = false;
      if ( 5.e-8 < fabs( oVtx->momB()[1] - tVtx->momB()[1] ) ) isOK = false;
      double oPB = 1. / oVtx->momB()[2];
      double tPB = 1. / tVtx->momB()[2];
      if ( 5.e-3 < fabs( oPB - tPB ) ) isOK = false;

      auto oDiag = std::array{
          sqrt( oVtx->covMatrix()( 0, 0 ) ), sqrt( oVtx->covMatrix()( 1, 1 ) ), sqrt( oVtx->covMatrix()( 2, 2 ) ),
          sqrt( oVtx->momcovA()( 0, 0 ) ),   sqrt( oVtx->momcovA()( 1, 1 ) ),   sqrt( oVtx->momcovA()( 2, 2 ) ),
          sqrt( oVtx->momcovB()( 0, 0 ) ),   sqrt( oVtx->momcovB()( 1, 1 ) ),   sqrt( oVtx->momcovB()( 2, 2 ) )};

      auto tDiag = std::array{
          sqrt( tVtx->covMatrix()( 0, 0 ) ), sqrt( tVtx->covMatrix()( 1, 1 ) ), sqrt( tVtx->covMatrix()( 2, 2 ) ),
          sqrt( tVtx->momcovA()( 0, 0 ) ),   sqrt( tVtx->momcovA()( 1, 1 ) ),   sqrt( tVtx->momcovA()( 2, 2 ) ),
          sqrt( tVtx->momcovB()( 0, 0 ) ),   sqrt( tVtx->momcovB()( 1, 1 ) ),   sqrt( tVtx->momcovB()( 2, 2 ) )};

      if ( 5.e-5 < fabs( oDiag[0] - tDiag[0] ) ) isOK = false;
      if ( 5.e-5 < fabs( oDiag[1] - tDiag[1] ) ) isOK = false;
      if ( 5.e-5 < fabs( oDiag[2] - tDiag[2] ) ) isOK = false;
      if ( 5.e-8 < fabs( oDiag[3] - tDiag[3] ) ) isOK = false;
      if ( 5.e-8 < fabs( oDiag[4] - tDiag[4] ) ) isOK = false;
      if ( 5. < fabs( oDiag[5] * oPA * 1.e4 - tDiag[5] * tPA * 1.e4 ) ) isOK = false;
      if ( 5.e-8 < fabs( oDiag[6] - tDiag[6] ) ) isOK = false;
      if ( 5.e-8 < fabs( oDiag[7] - tDiag[7] ) ) isOK = false;
      if ( 5. < fabs( oDiag[8] * oPB * 1.e4 - tDiag[8] * tPB * 1.e4 ) ) isOK = false;

      auto oFrac =
          std::array{oVtx->covMatrix()( 1, 0 ) / oDiag[1] / oDiag[0],  oVtx->covMatrix()( 2, 0 ) / oDiag[2] / oDiag[0],
                     oVtx->covMatrix()( 2, 1 ) / oDiag[2] / oDiag[1],  oVtx->momposcovA()( 0, 0 ) / oDiag[3] / oDiag[0],
                     oVtx->momposcovA()( 0, 1 ) / oDiag[3] / oDiag[1], oVtx->momposcovA()( 0, 2 ) / oDiag[3] / oDiag[2],
                     oVtx->momposcovA()( 1, 0 ) / oDiag[4] / oDiag[0], oVtx->momposcovA()( 1, 1 ) / oDiag[4] / oDiag[1],
                     oVtx->momposcovA()( 1, 2 ) / oDiag[4] / oDiag[2], oVtx->momcovA()( 1, 0 ) / oDiag[4] / oDiag[3],
                     oVtx->momposcovA()( 2, 0 ) / oDiag[5] / oDiag[0], oVtx->momposcovA()( 2, 1 ) / oDiag[5] / oDiag[1],
                     oVtx->momposcovA()( 2, 2 ) / oDiag[5] / oDiag[2], oVtx->momcovA()( 2, 0 ) / oDiag[5] / oDiag[3],
                     oVtx->momcovA()( 2, 1 ) / oDiag[5] / oDiag[4],    oVtx->momposcovB()( 0, 0 ) / oDiag[6] / oDiag[0],
                     oVtx->momposcovB()( 0, 1 ) / oDiag[6] / oDiag[1], oVtx->momposcovB()( 0, 2 ) / oDiag[6] / oDiag[2],
                     oVtx->mommomcov()( 0, 0 ) / oDiag[6] / oDiag[3],  oVtx->mommomcov()( 0, 1 ) / oDiag[6] / oDiag[4],
                     oVtx->mommomcov()( 0, 2 ) / oDiag[6] / oDiag[5],  oVtx->momposcovB()( 1, 0 ) / oDiag[7] / oDiag[0],
                     oVtx->momposcovB()( 1, 1 ) / oDiag[7] / oDiag[1], oVtx->momposcovB()( 1, 2 ) / oDiag[7] / oDiag[2],
                     oVtx->mommomcov()( 1, 0 ) / oDiag[7] / oDiag[3],  oVtx->mommomcov()( 1, 1 ) / oDiag[7] / oDiag[4],
                     oVtx->mommomcov()( 1, 2 ) / oDiag[7] / oDiag[5],  oVtx->momcovB()( 1, 0 ) / oDiag[7] / oDiag[6],
                     oVtx->momposcovB()( 2, 0 ) / oDiag[8] / oDiag[0], oVtx->momposcovB()( 2, 1 ) / oDiag[8] / oDiag[1],
                     oVtx->momposcovB()( 2, 2 ) / oDiag[8] / oDiag[2], oVtx->mommomcov()( 2, 0 ) / oDiag[8] / oDiag[3],
                     oVtx->mommomcov()( 2, 1 ) / oDiag[8] / oDiag[4],  oVtx->mommomcov()( 2, 2 ) / oDiag[8] / oDiag[5],
                     oVtx->momcovB()( 2, 0 ) / oDiag[8] / oDiag[6],    oVtx->momcovB()( 2, 1 ) / oDiag[8] / oDiag[7]};

      auto tFrac =
          std::array{tVtx->covMatrix()( 1, 0 ) / tDiag[1] / tDiag[0],  tVtx->covMatrix()( 2, 0 ) / tDiag[2] / tDiag[0],
                     tVtx->covMatrix()( 2, 1 ) / tDiag[2] / tDiag[1],  tVtx->momposcovA()( 0, 0 ) / tDiag[3] / tDiag[0],
                     tVtx->momposcovA()( 0, 1 ) / tDiag[3] / tDiag[1], tVtx->momposcovA()( 0, 2 ) / tDiag[3] / tDiag[2],
                     tVtx->momposcovA()( 1, 0 ) / tDiag[4] / tDiag[0], tVtx->momposcovA()( 1, 1 ) / tDiag[4] / tDiag[1],
                     tVtx->momposcovA()( 1, 2 ) / tDiag[4] / tDiag[2], tVtx->momcovA()( 1, 0 ) / tDiag[4] / tDiag[3],
                     tVtx->momposcovA()( 2, 0 ) / tDiag[5] / tDiag[0], tVtx->momposcovA()( 2, 1 ) / tDiag[5] / tDiag[1],
                     tVtx->momposcovA()( 2, 2 ) / tDiag[5] / tDiag[2], tVtx->momcovA()( 2, 0 ) / tDiag[5] / tDiag[3],
                     tVtx->momcovA()( 2, 1 ) / tDiag[5] / tDiag[4],    tVtx->momposcovB()( 0, 0 ) / tDiag[6] / tDiag[0],
                     tVtx->momposcovB()( 0, 1 ) / tDiag[6] / tDiag[1], tVtx->momposcovB()( 0, 2 ) / tDiag[6] / tDiag[2],
                     tVtx->mommomcov()( 0, 0 ) / tDiag[6] / tDiag[3],  tVtx->mommomcov()( 0, 1 ) / tDiag[6] / tDiag[4],
                     tVtx->mommomcov()( 0, 2 ) / tDiag[6] / tDiag[5],  tVtx->momposcovB()( 1, 0 ) / tDiag[7] / tDiag[0],
                     tVtx->momposcovB()( 1, 1 ) / tDiag[7] / tDiag[1], tVtx->momposcovB()( 1, 2 ) / tDiag[7] / tDiag[2],
                     tVtx->mommomcov()( 1, 0 ) / tDiag[7] / tDiag[3],  tVtx->mommomcov()( 1, 1 ) / tDiag[7] / tDiag[4],
                     tVtx->mommomcov()( 1, 2 ) / tDiag[7] / tDiag[5],  tVtx->momcovB()( 1, 0 ) / tDiag[7] / tDiag[6],
                     tVtx->momposcovB()( 2, 0 ) / tDiag[8] / tDiag[0], tVtx->momposcovB()( 2, 1 ) / tDiag[8] / tDiag[1],
                     tVtx->momposcovB()( 2, 2 ) / tDiag[8] / tDiag[2], tVtx->mommomcov()( 2, 0 ) / tDiag[8] / tDiag[3],
                     tVtx->mommomcov()( 2, 1 ) / tDiag[8] / tDiag[4],  tVtx->mommomcov()( 2, 2 ) / tDiag[8] / tDiag[5],
                     tVtx->momcovB()( 2, 0 ) / tDiag[8] / tDiag[6],    tVtx->momcovB()( 2, 1 ) / tDiag[8] / tDiag[7]};

      if ( !std::equal( oFrac.begin(), oFrac.end(), tFrac.begin(), tFrac.end(),
                        []( double o, double t ) { return std::abs( o - t ) < 2.e-5; } ) )
        isOK = false;
      //== Compare extra info
      LHCb::TwoProngVertex::ExtraInfo oExtra = oVtx->extraInfo();
      LHCb::TwoProngVertex::ExtraInfo tExtra = tVtx->extraInfo();
      if ( !std::equal( oExtra.begin(), oExtra.end(), tExtra.begin(), tExtra.end(), []( const auto& o, const auto& t ) {
             return o.first == t.first && std::abs( ( o.second - t.second ) / o.second ) < 1.e-7;
           } ) )
        isOK = false;

      //== Compare tracks
      const auto& oTracks = oVtx->tracks();
      const auto& tTracks = tVtx->tracks();
      if ( !std::equal( oTracks.begin(), oTracks.end(), tTracks.begin(), tTracks.end(),
                        []( const LHCb::Track* oTrk, const LHCb::Track* tTrk ) { // convert smartref to pointers
                          return oTrk == tTrk;
                        } ) )
        isOK = false;

      //== compare ParticleID
      const std::vector<LHCb::ParticleID>& oPid = oVtx->compatiblePIDs();
      const std::vector<LHCb::ParticleID>& tPid = tVtx->compatiblePIDs();
      if ( !std::equal( oPid.begin(), oPid.end(), tPid.begin(), tPid.end() ) ) isOK = false;

      if ( !isOK || MSG::DEBUG >= msgLevel() ) {
        info() << "===== TwoProngVertex key " << oVtx->key() << endmsg;
        info() << format( "Old   Technique %6d  chi2 %12.6f  nDoF%4d nTrack%4d  nExtra%4d", oVtx->technique(),
                          oVtx->chi2(), oVtx->nDoF(), oVtx->tracks().size(), oVtx->extraInfo().size() )
               << endmsg;
        info() << format( "Test  Technique %6d  chi2 %12.6f  nDoF%4d nTrack%4d  nExtra%4d", tVtx->technique(),
                          tVtx->chi2(), tVtx->nDoF(), tVtx->tracks().size(), tVtx->extraInfo().size() )
               << endmsg;
        if ( oVtx->tracks().size() == tVtx->tracks().size() ) {
          info() << "Old  tracks ";
          for ( auto& kk : oVtx->tracks() ) info() << " " << kk;
          info() << endmsg << "Test tracks ";
          for ( auto& kk : tVtx->tracks() ) info() << " " << kk;
          info() << endmsg;
        }
        // position
        info()
            << format(
                   "position Old  x%10.5f y%10.5f z%10.5f  txA%10.7f tyA%10.7f p%10.3f  txB%10.7f tyB%10.7f pB%10.3f",
                   oVtx->position().x(), oVtx->position().y(), oVtx->position().z(), oVtx->momA()[0], oVtx->momA()[1],
                   oPA, oVtx->momB()[0], oVtx->momB()[1], oPB )
            << endmsg;
        info()
            << format(
                   "position Test x%10.5f y%10.5f z%10.5f  txA%10.7f tyA%10.7f p%10.3f  txB%10.7f tyB%10.7f pB%10.3f",
                   tVtx->position().x(), tVtx->position().y(), tVtx->position().z(), tVtx->momA()[0], tVtx->momA()[1],
                   tPA, tVtx->momB()[0], tVtx->momB()[1], tPB )
            << endmsg;
        int indx = 0;
        for ( int krow = 0; 9 > krow; ++krow ) {
          info() << format( " Old row%2d : ", krow );
          int kcol;
          for ( kcol = 0; krow > kcol; ++kcol ) { info() << format( "%8.5f ", oFrac[indx + kcol] ); }
          info() << format( "  %10.5g", oDiag[krow] ) << endmsg;
          info() << format( "Test row%2d : ", krow );
          for ( kcol = 0; krow > kcol; ++kcol ) { info() << format( "%8.5f ", tFrac[indx + kcol] ); }
          indx += kcol;
          info() << format( "  %10.5g", tDiag[krow] ) << endmsg;
        }
        //== extraInfo
        auto oIt = oExtra.begin();
        auto tIt = tExtra.begin();
        for ( unsigned kk = 0; oExtra.size() != kk; ++kk, ++oIt, ++tIt ) {
          info() << format( "   old Extra %5d %12.4f     new %5d %12.4f", ( *oIt ).first, ( *oIt ).second,
                            ( *tIt ).first, ( *tIt ).second )
                 << endmsg;
        }
        //== Compatible PID
        info() << " old compatiblePID : ";
        for ( auto& kk : oPid ) { info() << format( " %12d", kk.pid() ); }
        info() << endmsg << "Test compatiblePID : ";
        for ( auto& kk : tPid ) { info() << format( " %12d", kk.pid() ); }
        info() << endmsg;
      }
    }
  }
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareTwoProngVertex )
//=============================================================================
