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
#include "Event/RecVertex.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareRecVertex
//
// 2008-11-17 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareRecVertex CompareRecVertex.h
 *  Compare two containers of RecVertices
 *
 *  @author Olivier Callot
 *  @date   2008-11-17
 */
struct CompareRecVertex : Gaudi::Functional::Consumer<void( LHCb::RecVertices const&, LHCb::RecVertices const& )> {

  CompareRecVertex( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::RecVertexLocation::Primary},
                  KeyValue{"TestName", LHCb::RecVertexLocation::Primary + "Test"}}} {}

  void operator()( LHCb::RecVertices const& old, LHCb::RecVertices const& test ) const override {
    if ( msgLevel( MSG::DEBUG ) ) debug() << "==> Execute" << endmsg;

    if ( old.size() != test.size() ) {
      err() << "Old RecVertex size " << old.size() << " differs form Test " << test.size() << endmsg;
      throw GaudiException{"Sizes differ", "CompareRecVertex", StatusCode::FAILURE};
    }
    auto itOld  = old.begin();
    auto itTest = test.begin();

    while ( old.end() != itOld ) {
      LHCb::RecVertex* oVtx = ( *itOld++ );
      LHCb::RecVertex* tVtx = ( *itTest++ );
      if ( oVtx->key() != tVtx->key() ) {
        warning() << "Wrong key : old " << oVtx->key() << " test " << tVtx->key() << endmsg;
      }
      bool isOK = true;
      if ( 1.e-7 < fabs( ( oVtx->chi2() - tVtx->chi2() ) / oVtx->chi2() ) ) isOK = false;
      if ( oVtx->nDoF() != tVtx->nDoF() ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().x() - tVtx->position().x() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().y() - tVtx->position().y() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVtx->position().z() - tVtx->position().z() ) ) isOK = false;

      auto oDiag = std::array{sqrt( oVtx->covMatrix()( 0, 0 ) ), sqrt( oVtx->covMatrix()( 1, 1 ) ),
                              sqrt( oVtx->covMatrix()( 2, 2 ) )};

      auto tDiag = std::array{sqrt( tVtx->covMatrix()( 0, 0 ) ), sqrt( tVtx->covMatrix()( 1, 1 ) ),
                              sqrt( tVtx->covMatrix()( 2, 2 ) )};

      if ( 5.e-5 < fabs( oDiag[0] - tDiag[0] ) ) isOK = false;
      if ( 5.e-5 < fabs( oDiag[1] - tDiag[1] ) ) isOK = false;
      if ( 5.e-5 < fabs( oDiag[2] - tDiag[2] ) ) isOK = false;

      auto oFrac =
          std::array{oVtx->covMatrix()( 1, 0 ) / oDiag[1] / oDiag[0], oVtx->covMatrix()( 2, 0 ) / oDiag[2] / oDiag[0],
                     oVtx->covMatrix()( 2, 1 ) / oDiag[2] / oDiag[1]};

      auto tFrac =
          std::array{tVtx->covMatrix()( 1, 0 ) / tDiag[1] / tDiag[0], tVtx->covMatrix()( 2, 0 ) / tDiag[2] / tDiag[0],
                     tVtx->covMatrix()( 2, 1 ) / tDiag[2] / tDiag[1]};

      if ( !std::equal( oFrac.begin(), oFrac.end(), tFrac.begin(), tFrac.end(),
                        []( double o, double t ) { return std::abs( t - o ) < 2.e-5; } ) )
        isOK = false;

      const auto& oExtra = oVtx->extraInfo();
      const auto& tExtra = tVtx->extraInfo();
      if ( !std::equal( oExtra.begin(), oExtra.end(), tExtra.begin(), tExtra.end(), []( const auto& o, const auto& t ) {
             return o.first == t.first && std::abs( ( o.second - t.second ) / o.second ) < 1.e-7;
           } ) )
        isOK = false;

      const auto& oTrks = oVtx->tracks();
      const auto& tTrks = tVtx->tracks();
      if ( !std::equal( oTrks.begin(), oTrks.end(), tTrks.begin(), tTrks.end(),
                        []( const LHCb::Track* dum, const LHCb::Track* dum1 ) { // convert smartref
                                                                                // to pointers
                          return dum == dum1;
                        } ) )
        isOK = false;

      const auto& oWeights = oVtx->weights();
      const auto& tWeights = tVtx->weights();
      if ( !std::equal( oWeights.begin(), oWeights.end(), tWeights.begin(), tWeights.end(),
                        []( double o, double t ) { return std::abs( o - t ) < 2.e-5; } ) )
        isOK = false;

      if ( !isOK || MSG::DEBUG >= msgLevel() ) {
        info() << "===== RecVertex key " << oVtx->key() << endmsg;
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
        if ( oVtx->weights().size() == tVtx->weights().size() ) {
          info() << "Old  weights ";
          for ( auto& kk : oVtx->weights() ) info() << " " << kk;
          info() << endmsg << "Test weights ";
          for ( auto& kk : tVtx->weights() ) info() << " " << kk;
          info() << endmsg;
        }
        // position
        info() << format( "position Old  x%10.5f y%10.5f z%10.5f", oVtx->position().x(), oVtx->position().y(),
                          oVtx->position().z() )
               << endmsg;
        info() << format( "position Test x%10.5f y%10.5f z%10.5f", tVtx->position().x(), tVtx->position().y(),
                          tVtx->position().z() )
               << endmsg;

        info() << format( " old Diag %10.5f %10.5f %10.5f", oDiag[0], oDiag[1], oDiag[2] ) << endmsg;
        info() << format( "test Diag %10.5f %10.5f %10.5f", tDiag[0], tDiag[1], tDiag[2] ) << endmsg;
        info() << " old Frac ";
        for ( auto& kk : oFrac ) { info() << format( " %8.5f", kk ); }
        info() << endmsg << "test Frac ";
        for ( auto& kk : tFrac ) { info() << format( " %8.5f", kk ); }
        info() << endmsg;
        //== extraInfo
        auto oIt = oExtra.begin();
        auto tIt = tExtra.begin();
        for ( unsigned kk = 0; oExtra.size() != kk; ++kk, ++oIt, ++tIt ) {
          info() << format( "   old Extra %5d %12.4f     new %5d %12.4f", ( *oIt ).first, ( *oIt ).second,
                            ( *tIt ).first, ( *tIt ).second )
                 << endmsg;
        }
      }
    }
  }
};
// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareRecVertex )

//=============================================================================
