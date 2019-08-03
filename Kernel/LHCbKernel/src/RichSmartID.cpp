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
/** @file RichSmartID.cpp
 *
 *  Implementation file for RICH Channel ID class : RichSmartID
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2005-01-06
 */
//-----------------------------------------------------------------------------

// STL
#include <iostream>
#include <sstream>

// local
#include "Kernel/RichSmartID.h"

// Gaudi
#include "GaudiKernel/GaudiException.h"

std::ostream& LHCb::RichSmartID::dumpBits( std::ostream& s ) const {
  for ( auto iCol = 0u; iCol < NBits; ++iCol ) { s << isBitOn( iCol ); }
  return s;
}

std::ostream& LHCb::RichSmartID::fillStream( std::ostream& s, const bool dumpSmartIDBits ) const {
  s << "{";

  // Dump the bits if requested
  if ( UNLIKELY( dumpSmartIDBits ) ) {
    s << " ";
    dumpBits( s );
  }

  // Type
  s << ( UNLIKELY( idType() == HPDID ) ? " HPD" : idType() == MaPMTID ? " PMT" : "UndefinedPD" );

  // if PMT add size (large or small)
  if ( pdIsSet() && idType() == MaPMTID ) { s << ( isLargePMT() ? ":l" : ":s" ); }

  // Is this smart ID valid
  if ( isValid() ) {

    // RICH detector
    if ( richIsSet() ) { s << " " << Rich::text( rich() ); }

    // Panel
    if ( panelIsSet() ) {
      s << ( rich() == Rich::Rich1 ? ( panel() == Rich::top ? " Top     " : " Bottom  " )
                                   : ( panel() == Rich::left ? " Left(A) " : " Right(C)" ) );
    }

    // PD
    if ( pdIsSet() ) {
      const std::string fS = ( idType() == MaPMTID ? "%4i" : "%3i" );
      s << " PDCol" << format( fS.c_str(), pdCol() );
      s << " PDNumInCol" << format( fS.c_str(), pdNumInCol() );
    }

    // Pixel
    if ( pixelColIsSet() ) s << " pixCol" << format( "%3i", pixelCol() );
    if ( pixelRowIsSet() ) s << " pixRow" << format( "%3i", pixelRow() );

    // Subpixel
    if ( UNLIKELY( pixelSubRowIsSet() ) ) s << " pixSubRow" << format( "%2i", pixelSubRow() );
  } else {
    // This SmartID has no valid bits set. This is bad ...
    s << " WARNING Invalid RichSmartID";
  }

  // end
  s << " }";

  return s;
}

void LHCb::RichSmartID::rangeError( const DataType value, const DataType maxValue, const std::string& message ) const {
  std::ostringstream mess;
  mess << message << " value " << value << " exceeds field maximum " << maxValue;
  throw GaudiException( mess.str(), "*RichSmartID*", StatusCode::FAILURE );
}

std::string LHCb::RichSmartID::toString() const {
  std::ostringstream text;
  text << *this;
  return text.str();
}

void LHCb::RichSmartID::setPixelSubRow( const DataType pixelSubRow ) {
  if ( HPDID == idType() ) {
#ifndef NDEBUG
    checkRange( pixelSubRow, HPD::MaxPixelSubRow, "PixelSubRow" );
#endif
    setData( pixelSubRow, HPD::ShiftPixelSubRow, HPD::MaskPixelSubRow, HPD::MaskPixelSubRowIsSet );
  } else {
    // MaPMTs do not have sub-pixel field...
    throw GaudiException( "MaPMTs cannot have their sub-pixel field set", "*RichSmartID*", StatusCode::FAILURE );
  }
}
