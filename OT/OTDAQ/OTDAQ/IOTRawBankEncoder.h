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
#ifndef OTDAQ_IOTRAWBANKENCODER_H
#define OTDAQ_IOTRAWBANKENCODER_H 1

// Include files
// from std
#include <vector>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Event
#include "Kernel/OTChannelID.h"

/// Interface ID

/** @class IOTRawBankEncoder IOTRawBankEncoder.h OTDAQ/IOTRawBankEncoder.h
 *
 *  Interface for tools that encode the channel ids and
 *  put them in the raw bank. 
 *
 *  @author Jan Mennis Amoraal
 *
 *  @date   2008-05-22
 */

struct IOTRawBankEncoder : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IOTRawBankEncoder, 2, 0 );

  virtual StatusCode encodeChannels( const std::vector<LHCb::OTChannelID>& channels ) const = 0;
  
};

#endif // OTDAQ_IOTRAWBANKENCODER_H
