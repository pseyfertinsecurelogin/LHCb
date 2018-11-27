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
/** @file RecSummary.cpp
 *
 *  Implementation file for class : LHCb::RecSummary
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    19/02/2011
 */
//-----------------------------------------------------------------------------

#include "Event/RecSummary.h"

//-----------------------------------------------------------------------------

std::ostream& LHCb::RecSummary::fillStream(std::ostream& s) const
{
  s << "{ Summary Info :";

  s << " nPVs = " << info( LHCb::RecSummary::DataTypes::nPVs, 0 );

  s << " nTracks = " << info( LHCb::RecSummary::DataTypes::nTracks, 0 );
  s << " nLongTracks = " << info( LHCb::RecSummary::DataTypes::nLongTracks, 0 );
  s << " nDownstreamTracks = " << info( LHCb::RecSummary::DataTypes::nDownstreamTracks, 0 );
  s << " nUpstreamTracks = " << info( LHCb::RecSummary::DataTypes::nUpstreamTracks, 0 );
  s << " nVeloTracks = " << info( LHCb::RecSummary::DataTypes::nVeloTracks, 0 );
  s << " nBackTracks = " << info( LHCb::RecSummary::DataTypes::nBackTracks, 0 );
  s << " nTTracks = " << info( LHCb::RecSummary::DataTypes::nTTracks, 0 );
  s << " nGhosts = " << info( LHCb::RecSummary::DataTypes::nGhosts, 0 );

  s << " nRich1Hits = " << info( LHCb::RecSummary::DataTypes::nRich1Hits, 0 );
  s << " nRich2Hits = " << info( LHCb::RecSummary::DataTypes::nRich2Hits, 0 );

  s << " nVeloClusters = " << info( LHCb::RecSummary::DataTypes::nVeloClusters, 0 );

  s << " nTTClusters = " << info( LHCb::RecSummary::DataTypes::nTTClusters, 0 );

  s << " nUTClusters = " << info( LHCb::RecSummary::DataTypes::nUTClusters, 0 );

  s << " nFTClusters = " << info( LHCb::RecSummary::DataTypes::nFTClusters, 0 );

  s << " nITClusters = " << info( LHCb::RecSummary::DataTypes::nITClusters, 0 );

  s << " nOTClusters = " << info( LHCb::RecSummary::DataTypes::nOTClusters, 0 );

  s << " nSPDhits = " << info( LHCb::RecSummary::DataTypes::nSPDhits, 0 );

  s << " nMuonCoordsS0 = " << info( LHCb::RecSummary::DataTypes::nMuonCoordsS0, 0 );
  s << " nMuonCoordsS1 = " << info( LHCb::RecSummary::DataTypes::nMuonCoordsS1, 0 );
  s << " nMuonCoordsS2 = " << info( LHCb::RecSummary::DataTypes::nMuonCoordsS2, 0 );
  s << " nMuonCoordsS3 = " << info( LHCb::RecSummary::DataTypes::nMuonCoordsS3, 0 );
  s << " nMuonCoordsS4 = " << info( LHCb::RecSummary::DataTypes::nMuonCoordsS4, 0 );
  s << " nMuonTracks = "   << info( LHCb::RecSummary::DataTypes::nMuonTracks, 0 );

  return s << " }";
}

