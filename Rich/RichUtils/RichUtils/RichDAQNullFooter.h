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

//=============================================================================================
/** @file RichDAQNullFooter.h
 *
 *  Header file for RICH DAQ utility class : Rich::DAQ::NullFooter
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   05/09/2006
 */
//=============================================================================================

#pragma once

// Kernel
#include "RichUtils/RichDAQFooterPDBase.h"

namespace Rich::DAQ {

  //=============================================================================================
  /** @class NullFooter RichDAQNullFooter.h
   *
   *  Implements a null footer for the LHCB HPD data blocks
   *
   *  @author Chris Jones    Christopher.Rob.Jones@cern.ch
   *  @date   05/09/2006
   */
  //=============================================================================================

  class NullFooter final : public FooterPDBase, public LHCb::MemPoolAlloc<NullFooter> {

  public:
    /// Does this foot have a parity word ?
    inline bool hasParityWord() const noexcept { return false; }

    /// Access the parity word
    inline WordType parityWord() const noexcept { return WordType( 0 ); }

    /// Set the parity word
    inline void setParityWord( const WordType /* word */ ) { ; }

    /// Test the parity word
    inline bool testParityWord( const WordType ) const noexcept { return true; }
  };

} // namespace Rich::DAQ
