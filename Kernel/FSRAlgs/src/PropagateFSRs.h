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
#ifndef PROPAGATEFSRS_H
#define PROPAGATEFSRS_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"
#include "GaudiUtils/IIODataManager.h"

// event model
#include "Event/HltLumiSummary.h"
#include "Event/LumiFSR.h"

/** @class PropagateFSRs PropagateFSRs.h
 *
 *
 *  @author Rob LAmbert
 *  @date   2010-03-26
 */
class PropagateFSRs : public GaudiAlgorithm, public virtual IIncidentListener {
public:
  /// Standard constructor
  PropagateFSRs( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

  // ==========================================================================
  // IIncindentListener interface
  // ==========================================================================
  void handle( const Incident& ) override;
  // ==========================================================================

protected:
  /// Reference to file records data service
  SmartIF<IDataProviderSvc> m_fileRecordSvc;
  std::string               m_rawEventLocation; // Location where we get the RawEvent

private:
  mutable SmartIF<IIncidentSvc> m_incSvc; /// the incident service

  // copied from XMLSummarySvc
  std::string m_filename; /// the current open filename

  Gaudi::IIODataManager* m_ioDataManager = nullptr; /// the pointer to the data manager service

  std::string file2GUID( const std::string& filename );
  std::string AFN2name( const std::string& filename ) const;
  std::string GUIDinEvent() const;
  void        AddName2GUID( const std::string& filename, const std::string& GUID );

  /// Map of FID to PFN
  typedef std::map<std::string, std::string> FidMap;
  /// Map of FID to PFN
  std::map<std::string, std::string> m_fidMap;
  /// Map of PFN to whether the GUID is known
  typedef std::map<std::string, bool> KnowGUIDMap;
  /// Map of  PFN to whether the GUID is known
  std::map<std::string, bool> m_knownFiles;
};
#endif // LUMIACCOUNTING_H
