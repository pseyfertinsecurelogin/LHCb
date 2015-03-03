
//-----------------------------------------------------------------------------
/** @file IRichRawDataFormatTool.h
 *
 *  Header file for tool interface : Rich::DAQ::IRawDataFormatTool
 *
 *  CVS Log :-
 *  $Id: IRichRawDataFormatTool.h,v 1.11 2007-04-23 12:44:03 jonrob Exp $
 *
 *  @author Chris Jones   Christopher.Rob.Jones@cern.ch
 *  @date   2004-12-18
 */
//-----------------------------------------------------------------------------

#ifndef RICHKERNEL_IRICHHPDDATABANKTOOL_H
#define RICHKERNEL_IRICHHPDDATABANKTOOL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

// Kernel
#include "Kernel/RichSmartID.h"

// RichKernel
#include "RichKernel/RichDecodedData.h"

// forward declarations
namespace LHCb
{
  class RawBank;
}

/// Static Interface Identification
static const InterfaceID IID_IRichRawDataFormatTool ( "Rich::DAQ::IRawDataFormatTool", 1, 0 );

namespace Rich
{
  namespace DAQ
  {

    /** @class IRawDataFormatTool IRichRawDataFormatTool.h RichKernel/IRichRawDataFormatTool.h
     *
     *  Interface to tool to encode and decode the Raw Buffer for the RICH
     *
     *  @author Chris Jones  Christopher.Rob.Jones@cern.ch
     *  @date   2004-12-18
     */

    class IRawDataFormatTool : virtual public IAlgTool
    {

    public:

      /** static interface identification
       *  @return unique interface identifier
       */
      static const InterfaceID& interfaceID() { return IID_IRichRawDataFormatTool; }

      /** Fills the RICH RawEvent from a vector of RichSmartIDs
       *
       *  @param smartIDs  Vector of RichSmartIDs
       *  @param version The RICH DAQ data bank version
       */
      virtual void fillRawEvent( const LHCb::RichSmartID::Vector & smartIDs,
                                 const Rich::DAQ::BankVersion version = Rich::DAQ::LHCb2 ) const = 0;

      /** Decode all RICH RawBanks into the RICH decoded data structure
       *
       *  @param decodedData The RICH data structure to fill
       */
      virtual void decodeToSmartIDs( Rich::DAQ::L1Map & decodedData ) const = 0;

    };

  }
}

#endif // RICHKERNEL_IRICHHPDDATABANKTOOL_H
