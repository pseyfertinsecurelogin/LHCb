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
#ifndef LHCB_LHCBFSRSTREAM_H
#define LHCB_LHCBFSRSTREAM_H

// Required for inheritance
#include "LHCbRecordStream.h"
#include "IIOFSRSvc.h"
#include "Event/IOFSR.h"

/** @class LHCbFSRStream LHCbFSRStream.h
  * Extension of RecordStream to add IOFsr and automatically clean up the TES
  *
  *
  * @author  R.Lambert
  * @version 1.0
  */
class LHCbFSRStream : public LHCbRecordStream     {
public:
  /// Standard algorithm Constructor
  LHCbFSRStream(const std::string&, ISvcLocator*);
  /// Algorithm overload: initialization
  StatusCode initialize() override;
  /// Algorithm overload: finalization
  StatusCode finalize() override;
  /// Runrecords do not get written for each event: Event processing hence dummy....
  StatusCode execute() override {  return StatusCode::SUCCESS; }

protected:
  ///clean up empty nodes before writing
  void cleanNodes(DataObject * obj,
                  const std::string & location,
                  unsigned int nRecCount = 0 );
private:
  SmartIF<IIOFSRSvc> m_ioFsrSvc; ///<reference to IOFSR service
  bool m_doIOFsr = true; //set to false to skip all the IOFSR stuff
  bool m_cleanTree = true; //set to false to skip cleaning the FSR tree
  std::string m_cleanRoot; //clean from where in the FSR tree
};

#endif // LHCB_LHCBFSRSTREAM_H
