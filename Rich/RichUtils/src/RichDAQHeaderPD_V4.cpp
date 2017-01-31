//=============================================================================================
/** @file RichDAQHeaderPD_V4.cpp
 *
 *  Implementation file for RICH DAQ class : RichDAQHeaderV4::RichDAQHeaderPD
 *
 *  @author Chris Jones       Christopher.Rob.Jones@cern.ch
 *  @date   23/04/2007
 */
//=============================================================================================

// local
#include "RichUtils/RichDAQHeaderPD_V4.h"

// Gaudi
#include "GaudiKernel/GaudiException.h"

namespace Rich
{
  namespace DAQ
  {
    namespace RichDAQHeaderV4
    {

      // Read correct number of data words from given stream
      // Note, after this call data pointer is incremented to the next word after the header
      void RichDAQHeaderPD::readFromDataStream( const LongType *& data )
      {
        if ( data )
        {
          // Read the first word (which gives us enough info to know the format)
          setPrimaryHeaderWord( WordType(*data) );
          // If extended mode, read the two L0 words (requires last line)
          if ( UNLIKELY( extendedFormat() ) )
          {
            // Reset header to have 1+2 words
            makeExtended();
            // set the primary word
            setPrimaryHeaderWord( WordType(*(data++)) );
            // set the extended words
            extendedHeaderWords()[0] = WordType(*(data++));
            extendedHeaderWords()[1] = WordType(*(data++));
          }
          else
          {
            // Just increment the pointer
            ++data;
          }
        }
        else
        {
          throw GaudiException( "Attempt to read from NULL data stream",
                                "*RichDAQHeaderPD*", StatusCode::FAILURE );
        }
      }

      void RichDAQHeaderPD::makeExtended()
      {
        if ( nHeaderWords() != nHeaderWordsExtended )
        {
          extendedHeaderWords().clear();
          extendedHeaderWords().resize( nHeaderWordsExtended-1, WordType(0) );
        }
      }

    }
  }
}
