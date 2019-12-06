/*****************************************************************************\
* (c) Copyright 2000-2020 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/

// local
#include "RichFutureUtils/RichMirrorFinder.h"

// RichDet
#include "RichDet/DeRich1.h"
#include "RichDet/DeRich2.h"

using namespace Rich::Utils;

namespace {

  // Returns the side for a given mirror and Rich Detector
  inline Rich::Side side( const DeRichSphMirror*   mirror, //
                          const Rich::DetectorType rich ) noexcept {
    return ( Rich::Rich1 == rich ? mirror->mirrorCentre().y() > 0.0 ? Rich::top : Rich::bottom
                                 : mirror->mirrorCentre().x() > 0.0 ? Rich::left : Rich::right );
  }

} // namespace

// Constructor from dependent detector elements
MirrorFinder::MirrorFinder( const DeRich1& rich1, //
                            const DeRich2& rich2 ) {

  // load all the mirrors
  for ( auto deRich : DetectorArray<const DeRich*>{&rich1, &rich2} ) {
    // primary mirrors
    for ( const auto m : deRich->primaryMirrors() ) {
      m_sphMirrFinder.addMirror( deRich->rich(), side( m, deRich->rich() ), m );
    }
    // secondary mirrors
    for ( const auto m : deRich->secondaryMirrors() ) {
      m_secMirrFinder.addMirror( deRich->rich(), side( m, deRich->rich() ), m );
    }
  }

  // initialise the finders
  m_sphMirrFinder.init();
  m_secMirrFinder.init();
}
