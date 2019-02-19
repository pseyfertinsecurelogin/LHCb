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
/** @file RichPDIdentifier.h
 *
 *  Header file for Rich::DAQ::HPDIdentifier
 *
 *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
 *  @date   27/04/2007
 */
//-----------------------------------------------------------------------------

#pragma once

// Kernel
#include "Kernel/RichSmartID.h"

namespace Rich::DAQ {

  /** @class PDIdentifier RichPDIdentifier.h RichUtils/RichPDIdentifier.h
   *
   *  Simple class to convert a human readable HPD number into and
   *  from a RichSmartID
   *
   *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
   *  @date   27/04/2007
   */
  class PDIdentifier final {

  public:
    /** Constructor from human readable number
     *  @param data The HPD information in a human readable form.
     *              The format is ABCCDD where A is the Rich number (1,2),
     *              B is the panel number (0,1), CC is the column number
     *              and DD is the number in column.
     *              For the PMT case RPMMMNN is Rich, Panel, Module and Number
     */
    explicit PDIdentifier( const int data = -1 ) : m_data( data ) {}

    /// Constructor from a RichSmartID
    explicit PDIdentifier( const LHCb::RichSmartID id ) {
      if ( LHCb::RichSmartID::HPDID == id.idType() ) {
        m_data = ( id.isValid() ? (int)( 100000 * ( 1 + (int)id.rich() ) + 10000 * ( (int)id.panel() ) +
                                         100 * id.pdCol() + id.pdNumInCol() )
                                : -1 );
      } else // MaPMT
      {
        m_data = ( id.isValid() ? (int)( 1000000 * ( 1 + (int)id.rich() ) + 100000 * ( (int)id.panel() ) +
                                         100 * id.pdCol() + id.pdNumInCol() )
                                : -1 );
      }
    }

  public:
    /// Return a RichSmartID
    inline LHCb::RichSmartID smartID() const {
      if ( m_data > 999999 ) // MaPMT
      {
        return LHCb::RichSmartID( ( Rich::DetectorType )( ( m_data / 1000000 ) - 1 ),
                                  ( Rich::Side )( ( m_data / 100000 ) % 10 ), m_data % 100, ( m_data / 100 ) % 1000,
                                  LHCb::RichSmartID::MaPMTID );
      } else {
        return ( m_data == -1 ? LHCb::RichSmartID()
                              : LHCb::RichSmartID( ( Rich::DetectorType )( ( m_data / 100000 ) - 1 ),
                                                   ( Rich::Side )( ( m_data / 10000 ) % 10 ), m_data % 100,
                                                   ( m_data / 100 ) % 100 ) );
      }
    }

    /// Implicit conversion into a RichSmartID
    inline operator LHCb::RichSmartID() const { return this->smartID(); }

    /// Access the human readable HPD number
    inline int number() const noexcept { return m_data; }

  private:
    int m_data{-1}; ///< The internal data
  };

} // namespace Rich::DAQ
