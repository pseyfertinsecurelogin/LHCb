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

//----------------------------------------------------------------------------
/** @file DeRichLocations.h
 *
 *  Implementation for for DeRichLocation methods
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   2006-03-02
 */
//----------------------------------------------------------------------------

// Local
#include "RichDet/DeRichLocations.h"

// RichUtils
#include "RichUtils/RichException.h"

const std::string DeRichLocations::location( const Rich::RadiatorType rad ) {
  if ( Rich::Aerogel == rad ) { return DeRichLocations::Aerogel; }
  if ( Rich::Rich1Gas == rad ) { return DeRichLocations::Rich1Gas; }
  if ( Rich::Rich2Gas == rad ) { return DeRichLocations::Rich2Gas; }
  throw Rich::Exception( "No Detector Element for radiator type " + Rich::text( rad ) );
  return "";
}

const std::string DeRichLocations::location( const Rich::DetectorType det ) {
  if ( Rich::Rich1 == det ) { return DeRichLocations::Rich1; }
  if ( Rich::Rich2 == det ) { return DeRichLocations::Rich2; }
  throw Rich::Exception( "No Detector Element for detector type " + Rich::text( det ) );
  return "";
}
