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
#include "Event/MCVertex.h"
#include "GaudiAlg/Consumer.h"
#include "boost/numeric/conversion/bounds.hpp"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareMCVertex
//
// 2005-03-18 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareMCVertex CompareMCVertex.h
 *  Compare two containers of MCVertices
 *
 *  @author Olivier Callot
 *  @date   2005-03-18
 */
struct CompareMCVertex : Gaudi::Functional::Consumer<void( const LHCb::MCVertices&, const LHCb::MCVertices& )> {

  CompareMCVertex( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::MCVertexLocation::Default},
                  KeyValue{"TestName", LHCb::MCVertexLocation::Default + "Test"}}} {}

  void operator()( const LHCb::MCVertices& old, const LHCb::MCVertices& test ) const override {

    if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "==> Execute" << endmsg;

    if ( old.size() != test.size() ) {
      err() << "Old MCVertex size " << old.size() << " differs form Test " << test.size() << endmsg;
      throw GaudiException{"Sizes differ", "CompareMCVertex", StatusCode::FAILURE};
    }
    auto itOld  = old.begin();
    auto itTest = test.begin();

    static const double smallest = boost::numeric::bounds<float>::smallest();
    static const double tiny     = boost::numeric::bounds<double>::smallest();

    while ( old.end() != itOld ) {
      LHCb::MCVertex* oVert = ( *itOld++ );
      LHCb::MCVertex* tVert = ( *itTest++ );

      if ( oVert->key() != tVert->key() ) {
        warning() << "Wrong key : old " << oVert->key() << " test " << tVert->key() << endmsg;
      }
      bool isOK = true;
      if ( 5.e-5 < fabs( oVert->position().x() - tVert->position().x() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVert->position().y() - tVert->position().y() ) ) isOK = false;
      if ( 5.e-5 < fabs( oVert->position().z() - tVert->position().z() ) ) isOK = false;
      // Protect crazy vertex times in DC06 data. Test times should be OK after unpacking
      double oldTime;
      if ( oVert->time() > 0. && oVert->time() < smallest ) {
        if ( msgLevel( MSG::DEBUG ) ) {
          if ( oVert->time() < tiny )
            debug() << "time smaller than " << tiny;
          else
            debug() << "time " << oVert->time();

          debug() << " set to zero for vertex " << oVert->key() << " of type " << oVert->type() << endmsg;
        }
        oldTime = 0;
      } else {
        oldTime = oVert->time();
      }
      if ( 1.e-3 < fabs( oldTime - tVert->time() ) ) isOK = false;

      if ( oVert->type() != tVert->type() ) isOK = false;

      if ( oVert->mother() != tVert->mother() ) isOK = false;
      if ( isOK && msgLevel( MSG::VERBOSE ) ) verbose() << "diff mother OK" << endmsg;
      const auto& oProd = oVert->products();
      const auto& tProd = tVert->products();
      if ( !std::equal( oProd.begin(), oProd.end(), tProd.begin(), tProd.end(),
                        []( const LHCb::MCParticle* dum, const LHCb::MCParticle* dum1 ) { // convert smartref to
                                                                                          // pointers
                          return dum == dum1;
                        } ) )
        isOK = false;
      if ( isOK && msgLevel( MSG::VERBOSE ) ) verbose() << "diff products OK" << endmsg;
      if ( !isOK || msgLevel( MSG::VERBOSE ) ) {
        if ( !isOK ) {
          Warning( "Packed MCVertex info truncated. Set DEBUG OutputLevel for details", StatusCode::SUCCESS, 0 )
              .ignore();
          debug() << "Packed MCVertex info truncated" << endmsg;
        }
        debug() << "=== MCVertex key " << oVert->key() << endmsg;
        debug() << format( "  old point %12.5f %12.5f %12.5f %12.4f %2d", oVert->position().x(), oVert->position().y(),
                           oVert->position().z(), oVert->time(), oVert->type() )
                << " mother " << oVert->mother() << endmsg;
        debug() << format( " test point %12.5f %12.5f %12.5f %12.4f %2d", tVert->position().x(), tVert->position().y(),
                           tVert->position().z(), tVert->time(), tVert->type() )
                << " mother " << tVert->mother() << endmsg << "  old products ";
        for ( auto& kk : oVert->products() ) { debug() << " " << kk; }
        debug() << endmsg << " test products ";
        for ( auto& kk : tVert->products() ) { debug() << " " << kk; }
        debug() << endmsg;
      }
    }
  }
};

DECLARE_COMPONENT( CompareMCVertex )
//=============================================================================
