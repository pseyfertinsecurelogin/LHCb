/*****************************************************************************\
* (c) Copyright 2018 CERN for the benefit of the LHCb Collaboration           *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
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
 *  @author A Beiter (based on code by M Needham)
 *  @date   2018-09-04
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
