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
#ifndef FILEIDBANKWRITER_H
#define FILEIDBANKWRITER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

#include "Event/FileId.h"
#include "Event/RawEvent.h"

/** @class FileIdBankWriter FileIdBankWriter.h
 *
 *
 *  @author Jaap Panman
 *  @date   2009-10-01
 */
class FileIdBankWriter : public GaudiAlgorithm {
public:
  /// Standard constructor
  FileIdBankWriter( const std::string& name, ISvcLocator* pSvcLocator );

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  std::string m_rawEventLocation; // Location where we get the RawEvent
  std::string m_current_fname;    // current file ID string
  int         m_count_files;      // number of files read

  std::vector<unsigned int> m_bank;

  LHCb::FileId m_fileId;
};
#endif // FILEIDBANKWRITER_H
