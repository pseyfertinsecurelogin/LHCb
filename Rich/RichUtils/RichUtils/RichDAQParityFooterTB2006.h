
//=============================================================================================
/** @file RichDAQParityFooterTB2006.h
 *
 *  Header file for RICH DAQ utility class : Rich::DAQ::ParityFooterTB2006
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/09/2006
 */
//=============================================================================================

#pragma once

#include "RichUtils/RichDAQFooterPDBase.h"

namespace Rich::DAQ
{

  //=============================================================================================
  /** @class ParityFooterTB2006 RichDAQParityFooterTB2006.h
   *
   *  Implements a single word parity footer, as used in the 2006 RICH testbeam
   *
   *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *  @date   05/09/2006
   */
  //=============================================================================================

  class ParityFooterTB2006 final : public FooterPDBase
  {

  public:

    /// Standard constructor
    ParityFooterTB2006() : FooterPDBase( 1 ) {}

    /// Does this foot have a parity word ?
    inline bool hasParityWord() const noexcept { return true; }

    /// Access the parity word
    inline WordType parityWord() const { return footerWords()[ 0 ]; }

    /// Set the parity word
    inline void setParityWord( const WordType word ) { this->setWord( 0, word ); }

    /// Test the parity word
    inline bool testParityWord( const WordType refWord ) const
    {
      return ( refWord == this->parityWord() );
    }
  };

} // namespace Rich::DAQ
