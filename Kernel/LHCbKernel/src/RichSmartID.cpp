
//-----------------------------------------------------------------------------
/** @file RichSmartID.cpp
 *
 *  Implementation file for RICH Channel ID class : RichSmartID
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2005-01-06
 */
//-----------------------------------------------------------------------------

#include <iostream>
#include <string>

// Gaudi
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/MsgStream.h"
#include "GaudiKernel/StatusCode.h"
#include "Kernel/RichDetectorType.h"
#include "Kernel/RichSide.h"
// local
#include "Kernel/RichSmartID.h"

std::ostream& LHCb::RichSmartID::dumpBits(std::ostream& s) const
{
  for ( auto iCol = 0u; iCol < NBits; ++iCol ) { s << isBitOn( iCol ); }
  return s;
}

std::ostream& LHCb::RichSmartID::fillStream( std::ostream& s, 
                                             const bool dumpSmartIDBits ) const
{
  s << "{";

  // Dump the bits if requested
  if ( dumpSmartIDBits ) { s << " "; dumpBits(s); }

  // Type
  s << ( idType() == HPDID   ? " HPD"   : 
         idType() == MaPMTID ? " MaPMT" : 
         "UndefinedPD" );

  // Is this smart ID valid
  if ( isValid() )
  {

    // RICH detector
    if ( richIsSet() ) s << " " << Rich::text( rich() );

    // Panel
    if ( panelIsSet() )
    {
      const std::string PANEL
        = ( rich() == Rich::Rich1 ?
            ( panel() == Rich::top  ? "Top     " : "Bottom  " ) :
            ( panel() == Rich::left ? "Left(A) " : "Right(C)" ) );
      s << " "            << PANEL;
    }

    // PD
    if ( pdIsSet() )
    {
      const std::string fS = ( idType() == MaPMTID ? "%4i" : "%3i" );
      s << " PDCol"      << format(fS.c_str(),pdCol());
      s << " PDNumInCol" << format(fS.c_str(),pdNumInCol());
    }

    // Pixel
    if ( pixelColIsSet() ) s << " pixCol" << format("%3i",pixelCol());
    if ( pixelRowIsSet() ) s << " pixRow" << format("%3i",pixelRow());

    // Subpixel
    if ( pixelSubRowIsSet() ) s << " pixSubRow" << format("%2i",pixelSubRow());

  }
  else
  {
    // This SmartID has no valid bits set. This is bad ...
    s << " WARNING Invalid RichSmartID";
  }

  // end
  s << " }";

  return s;
}

void LHCb::RichSmartID::rangeError( const DataType value,
                                    const DataType maxValue,
                                    const std::string & message ) const
{
  std::ostringstream mess;
  mess << message << " value " << value << " exceeds field maximum " << maxValue;
  throw GaudiException ( mess.str(), "*RichSmartID*", StatusCode::FAILURE );
}

std::string LHCb::RichSmartID::toString() const
{
  std::ostringstream text;
  text << *this;
  return text.str();
}

void LHCb::RichSmartID::setPixelSubRow( const DataType pixelSubRow )
{
  if ( HPDID == idType() )
  {
#ifndef NDEBUG
    checkRange( pixelSubRow, HPD::MaxPixelSubRow, "PixelSubRow" );
#endif
    setData( pixelSubRow, HPD::ShiftPixelSubRow, HPD::MaskPixelSubRow, HPD::MaskPixelSubRowIsSet );
  }
  else
  {
    // MaPMTs do not have sub-pixel field...
    throw GaudiException ( "MaPMTs cannot have their sub-pixel field set",
                           "*RichSmartID*", StatusCode::FAILURE );
  }
}
