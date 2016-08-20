
//-----------------------------------------------------------------------------
/** @file RichSmartID32.cpp
 *
 *  Implementation file for RICH Channel ID class : RichSmartID32
 *
 *  @author  Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date    2005-01-06
 */
//-----------------------------------------------------------------------------

// STL
#include <sstream>
#include <iostream>

// local
#include "Kernel/RichSmartID.h"
#include "Kernel/RichSmartID32.h"

// Gaudi
#include "GaudiKernel/GaudiException.h"

LHCb::RichSmartID32::RichSmartID32( const RichSmartID& id ) : m_key(0) 
{
  if ( id.idType() == RichSmartID::HPDID ) 
  {
    setIDType(HPDID);  
    if ( id.pixelSubRowIsSet() ) { setPixelSubRow( id.pixelSubRow() ); }
  }
  else if ( id.idType() == RichSmartID::MaPMTID ) { setIDType(MaPMTID); }
  else
  {
    throw GaudiException ( "Only HPDs or MaPMTs supported by 32bit version",
                           "*RichSmartID32*", StatusCode::FAILURE );
  }
  if ( id.richIsSet()     ) { setRich ( id.rich()  ); }
  if ( id.panelIsSet()    ) { setPanel( id.panel() ); }
  if ( id.pdIsSet()       ) { setPD( id.pdCol(), id.pdNumInCol() ); }
  if ( id.pixelRowIsSet() ) { setPixelRow( id.pixelRow() ); }
  if ( id.pixelColIsSet() ) { setPixelCol( id.pixelCol() ); }
}

std::ostream& LHCb::RichSmartID32::dumpBits(std::ostream& s) const
{
  for ( auto iCol = 0u; iCol < NBits; ++iCol ) { s << isBitOn( iCol ); }
  return s;
}

std::ostream& LHCb::RichSmartID32::fillStream( std::ostream& s,
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
    s << " WARNING Invalid RichSmartID32";
  }

  // end
  s << " }";

  return s;
}

void LHCb::RichSmartID32::rangeError(const DataType value,
                                     const DataType maxValue,
                                     const std::string& message) const
{
  std::ostringstream mess;
  mess << message << " value " << value << " exceeds field maximum " << maxValue;
  throw GaudiException ( mess.str(), "*RichSmartID32*", StatusCode::FAILURE );
}

std::string LHCb::RichSmartID32::toString() const
{
  std::ostringstream text;
  text << *this;
  return text.str();
}

void LHCb::RichSmartID32::setPixelSubRow( const DataType pixelSubRow )
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
                           "*RichSmartID32*", StatusCode::FAILURE );
  }
}

