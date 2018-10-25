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
/** @file RichDetectorType.cpp
 *
 *  Implementation file for enumeration : Rich::DetectorType
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2002-06-10
 */
//-----------------------------------------------------------------------------

// local
#include "Kernel/RichDetectorType.h"

// Text conversion for DetectorType enumeration
std::string
Rich::text( const Rich::DetectorType detector )
{
  switch ( detector )
  {
    case Rich::Rich1: return "Rich1";
    case Rich::Rich2:
      return "Rich2";
      // case Rich::Rich :              return "Single Rich";
    case Rich::InvalidDetector: return "Invalid Detector";
    default: return "SHOULD NEVER SEE THIS";
  }
}

const Rich::Detectors &
Rich::detectors() noexcept
{
  static Rich::Detectors dets = { Rich::Rich1, Rich::Rich2 };
  return dets;
}
