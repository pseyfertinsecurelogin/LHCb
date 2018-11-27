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

//===============================================================================
/** @file RichTraceMode.cpp
 *
 *  Implementation file for utility class : RichTraceMode
 *
 *  @author Chris Jones Christopher.Rob.Jones@cern.ch
 *  @date   05/08/2006
 */
//===============================================================================

// Include files

// local
#include "Kernel/RichTraceMode.h"

std::ostream &
LHCb::RichTraceMode::fillStream( std::ostream &os ) const
{
  os << "RichTraceMode[ DetectionPrecision=" << detPrecision()
     << " DetPlaneBoundaries=" << detPlaneBound();
  if ( forcedSide() ) { os << " [Will force given RICH side]"; }
  if ( outMirrorBoundary() ) { os << " [Will Respect Outside Mirror Boundaries]"; }
  if ( mirrorSegBoundary() ) { os << " [Will Respect Mirror Seg Boundaries]"; }
  if ( beamPipeIntersects() ) { os << " [Will Test for intersections with beampipe]"; }
  if ( hpdKaptonShadowing() ) { os << " [Will check for intersections with HPD kapton]"; }
  if ( aeroRefraction() ) { os << " [Will correct for refraction at Aerogel/Rich1Gas boundary]"; }
  return os << " ]";
}

//-----------------------------------------------------------------------------
