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
#include "Event/PackedTrack.h"
#include "Event/Track.h"
#include "GaudiAlg/Consumer.h"

//-----------------------------------------------------------------------------
// Implementation file for class : CompareTrack
//
// 2008-11-14 : Olivier Callot
//-----------------------------------------------------------------------------

/** @class CompareTrack CompareTrack.h
 *  Compare two containers of Tracks
 *
 *  @author Olivier Callot
 *  @date   2008-11-14
 */
struct CompareTrack : Gaudi::Functional::Consumer<void( LHCb::Tracks const&, LHCb::Tracks const& )> {

  CompareTrack( const std::string& name, ISvcLocator* pSvcLocator )
      : Consumer{name,
                 pSvcLocator,
                 {KeyValue{"InputName", LHCb::TrackLocation::Default},
                  KeyValue{"TestName", LHCb::TrackLocation::Default + "Test"}}} {}

  void operator()( LHCb::Tracks const& old, LHCb::Tracks const& test ) const override {
    const LHCb::TrackPacker packer( this );
    packer.check( old, test ).orThrow( "Comparison failed", "CompareTrack" );
  }
};

// Declaration of the Algorithm Factory
DECLARE_COMPONENT( CompareTrack )
