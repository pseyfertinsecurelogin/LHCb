// $Id: IUTRawADCInfo.h,v 1.1 2009-08-20 07:58:46 mneedham Exp $
// ============================================================================
#ifndef UTKERNEL_IUTRAWADCINFO_H
#define UTKERNEL_IUTRAWADCINFO 1
// Include files
#include <string>
#include <vector>
// GaudiKernel
#include "GaudiKernel/IAlgTool.h"
// forward declaration
namespace LHCb{
  class  UTChannelID;
}

/** @class IUTRawADCInfo IUTRawADCInfo.h
 *
 *  An abstract interface for linking channels to RAW info
 *
 *
 *  @author M Needham
 *  @date   08/08/2009
 */

struct IUTRawADCInfo : extend_interfaces<IAlgTool> {

  /** static interface identification
   *  @see IInterface
   *  @see IID_IUTRawADCInfo
   *  @return the unique interface identifier
   */
  DeclareInterfaceID( IUTRawADCInfo , 2 , 0 );

  /** link to RAW adc info
  * @param LHCb::UTChannelID chan
  * @param adc value
  * @return StatusCode
  */
  virtual StatusCode link(const LHCb::UTChannelID& chan, unsigned int& value) const =0;

  /** link to RAW adc info for a list of channels
  * @param std::vector<LHCb::UTChannelID> chan
  * @param adc value
  * @return StatusCode
  */
  virtual StatusCode link(const std::vector<LHCb::UTChannelID>& chans,
                  std::vector<unsigned int>& values) const =0;

  /** link to RAW adc info for a list of channels in a window
  * @param std::vector<LHCb::UTChannelID> chan
  * @param adc value
  * @return StatusCode
  */
  virtual StatusCode link(const LHCb::UTChannelID& chan,
                  const unsigned int window,
                  std::vector<unsigned int>& values) const = 0;

};

// ============================================================================
#endif // UT_IUTRawADCInfo
// ============================================================================
