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
#ifndef DUMPFSR_H
#define DUMPFSR_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "GaudiKernel/IDataManagerSvc.h"
#include "GaudiKernel/IRegistry.h"

// for incidents listener
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"

// event model
#include "Event/EventCountFSR.h"
#include "Event/LumiFSR.h"
#include "Event/TimeSpanFSR.h"

// local
#include "FSRNavigator.h"

/** @class DumpFSR DumpFSR.h
 *
 *
 *  @author Jaap Panman
 *  @date   2009-02-27
 */
class DumpFSR : public extends<GaudiAlgorithm, IIncidentListener> {
public:
  /// Standard constructor
  DumpFSR( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

  // ==========================================================================
  // IIncindentListener interface
  // ==========================================================================
  void handle( const Incident& ) override;
  // ==========================================================================

private:
  void        dump_file( std::string txt = "" ); ///< print the FSRs of one input file
  void        write_file();                      ///< write the FSRs as ascii to a file
  std::string fileID();                          ///< get the fileID

  /// Reference to run records data service
  SmartIF<IDataProviderSvc> m_fileRecordSvc;

  std::string m_rawEventLocation;   ///< Location where we get the RawEvent
  std::string m_FileRecordName;     ///< location of FileRecords
  std::string m_FSRName;            ///< specific tag of summary data in FSR
  std::string m_LowFSRName;         ///< specific tag of low lumi summary data in FSR
  std::string m_EventCountFSRName;  ///< specific tag of event summary data in FSR
  std::string m_TimeSpanFSRName;    ///< specific tag of event summary data in FSR
  std::string m_current_fname;      ///< current file ID string
  std::string m_ascii_fname;        ///< name of ascii file to write FSR data to
  std::string m_dumprequests;       ///< job: E:event F:fini, files: B:begin C:close
  int         m_count_files    = 0; ///< number of files read
  int         m_count_events   = 0; ///< number of events read
  int         m_events_in_file = 0; ///< events after OpenFileIncident

private:
  IFSRNavigator*                m_navigatorTool = nullptr; ///< tool to navigate FSR
  mutable SmartIF<IIncidentSvc> m_incSvc;                  ///< the incident service
};
#endif // DUMPFSR_H
