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
// $Id: LHCbRecordStream.h,v 1.1 2008/12/10 18:37:36 marcocle Exp $
#ifndef LHCB_RECORDSTREAM_H
#define LHCB_RECORDSTREAM_H

// Required for inheritance
#include "LHCbOutputStream.h"

/** @class LHCbRecordStream LHCbRecordStream.h
  * Extension of OutputStream to write run records after last event
  *
  * @author  M.Frank
  * @version 1.0
  */
class LHCbRecordStream : public LHCbOutputStream     {
public:
  /// Standard algorithm Constructor
  LHCbRecordStream(const std::string&, ISvcLocator*);
  /// Standard Destructor
  virtual ~LHCbRecordStream() {}
  /// Algorithm overload: finalization
  StatusCode finalize() override;
  /// Runrecords do not get written for each event: Event processing hence dummy....
  StatusCode execute() override {  return StatusCode::SUCCESS; }
};

#endif // LHCB_RECORDSTREAM_H
