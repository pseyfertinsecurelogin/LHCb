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

//-----------------------------------------------------------------------------
/** @file RichGeomPhoton.cpp
 *
 *  Implementation file for class : RichGeomPhoton
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2017-02-06
 */
//-----------------------------------------------------------------------------

// local
#include "RichFutureUtils/RichGeomPhoton.h"

std::ostream &
Rich::Future::RecoPhoton::fillStream( std::ostream &s ) const
{
  return s << "{"
           << " ckTheta:" << CherenkovTheta() << " ckPhi:" << CherenkovPhi()
           << " smartID:" << smartID() << " activeFraction:" << activeSegmentFraction() << " }";
}

std::ostream &
Rich::Future::GeomPhoton::fillStream( std::ostream &s ) const
{
  RecoPhoton::fillStream( s );
  return s << "{"
           << " emissionPoint:" << emissionPoint() << " emissionDir:" << emissionDir()
           << " detectionPoint:" << detectionPoint()
           << " sphMirReflectionPoint:" << sphMirReflectionPoint()
           << " flatMirReflectionPoint:" << flatMirReflectionPoint()
           << " primaryMirror:" << (void *)primaryMirror()
           << " secondaryMirror:" << (void *)secondaryMirror() << " }";
}
