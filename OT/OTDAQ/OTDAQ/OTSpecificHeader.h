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
#ifndef OTDAQ_OTSPECIFICHEADER_H
#define OTDAQ_OTSPECIFICHEADER_H 1
#include <ostream>

// Forward declarations

namespace OTDAQ {

  class OTSpecificHeader final {
    unsigned int m_data;

  public:
    ///
    //    static size_t BitsNumberOfGOLs = 16 ;
    //     static size_t BitsBunchCounter =  8 ;
    //     static size_t BitsErrorFlag    =  1 ;
    //     static size_t BitsTriggerType  =  3 ;

    /// Bitfield definition
    enum Shifts {
      NumberOfGOLsShift = 0,
      BunchCounterShift = 16,
      ErrorShift        = 24,
      TriggerTypeShift  = 25,
      EventIDShift      = 28
    };

    enum Masks {
      NumberOfGOLsMask = 0x0000FFFFL,
      BunchCounterMask = 0x00FF0000L,
      ErrorMask        = 0x01000000L,
      TriggerTypeMask  = 0x0E000000L,
      EventIDMask      = 0xF0000000L
    };

    /// constructor with word, station, layer, quarter, module, otisErFlag and size
    OTSpecificHeader( unsigned int iError, unsigned int iTriggerType, unsigned int iBunchCounter,
                      unsigned int iNumberOfGOLs )
        : m_data( ( iError << NumberOfGOLsShift ) + ( iTriggerType << TriggerTypeShift ) +
                  ( iBunchCounter << BunchCounterShift ) + ( iNumberOfGOLs << NumberOfGOLsShift ) ){};

    /// constructor with golHeader
    OTSpecificHeader( unsigned int data ) : m_data( data ) {}

    unsigned int numberOfGOLs() const { return ( m_data & NumberOfGOLsMask ) >> NumberOfGOLsShift; }
    unsigned int bunchCounter() const { return ( m_data & BunchCounterMask ) >> BunchCounterShift; }
    unsigned int error() const { return ( m_data & ErrorMask ) >> ErrorShift; }
    unsigned int triggerType() const { return ( m_data & TriggerTypeMask ) >> TriggerTypeShift; }
    unsigned int eventID() const { return ( m_data & EventIDMask ) >> EventIDShift; }

    friend inline std::ostream& operator<<( std::ostream& os, const OTSpecificHeader& header ) {
      return os << "[ triggerType = " << header.triggerType() << ", error = " << header.error()
                << ", bunchCounter = " << header.bunchCounter() << ", numberOfGOLs = " << header.numberOfGOLs()
                << " ] ";
    }
  };
} // namespace OTDAQ

#endif
