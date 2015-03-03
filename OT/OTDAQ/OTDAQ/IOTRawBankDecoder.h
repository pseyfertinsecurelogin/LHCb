// $Id: IOTRawBankDecoder.h,v 1.2 2007-11-14 16:05:39 wouter Exp $
#ifndef OTDAQ_IOTRAWBANKDECODER_H
#define OTDAQ_IOTRAWBANKDECODER_H 1

// Include files
// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Event/OTLiteTime.h"

// Forward declarations
class DeOTModule ;

/// Interface ID
static const InterfaceID IID_OTRawBankDecoder( "IOTRawBankDecoder", 1, 0 );

/** @class IOTRawBankDecoder IOTRawBankDecoder.h OTDAQ/IOTRawBankDecoder.h
 *
 *  Decodes the OT data for single modules. The output is a list of
 *  OTLiteTimes, which are essentially just wrappers of an OTChannelId.
 *
 *  Based on old OTTimeCreator by Jacopo Nardulli
 *
 *  @author S. Hansmann-Menzemer, W. Hulsbergen, C. Jones, K. Rinnert
 *  @author Jacopo Nardulli
 *
 *  @date   2007-05-30
 */

class IOTRawBankDecoder : virtual public IAlgTool
{
  
public: 
  /// Retrieve interface ID
  static const InterfaceID& interfaceID() { return IID_OTRawBankDecoder ; }
  
  /// Decode data for a single module
  virtual LHCb::OTLiteTimeRange decodeModule( const LHCb::OTChannelID& moduleId ) const = 0 ;
  
  /// Decode all gol headers
  virtual StatusCode decodeGolHeaders() const = 0;
  
  /// Decode all modules
  virtual StatusCode decode( LHCb::OTLiteTimeContainer& ottimes ) const = 0;

  /// Conversion factor from tdc to ns
  virtual double nsPerTdcCount() const = 0 ;

  /// Get a lite time from an OTChannelId (and its module mother)
  virtual LHCb::OTLiteTime time( LHCb::OTChannelID id, const DeOTModule& module) const = 0 ;
};

#endif // OTDAQ_IOTRAWBANKDECODER_H
