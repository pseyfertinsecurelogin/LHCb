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
#ifndef CONDDBREPLAYALG_H_
#define CONDDBREPLAYALG_H_

#include "DetCond/ICondDBReader.h"

#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/Time.h"

#include <list>
#include <string>

class ICondDBReader;

/** @class CondDBReplayAlg CondDBReplayAlg.h
 *
 *  Simple algorithm that reads a file in the format produced by CondDBLogger
 *  and re-play the request to the database with the same timing written in the
 *  log file.
 *
 *  @author Marco Clemencic <marco.clemencic@cern.ch>
 *  @date   2008-01-25
 */
class CondDBReplayAlg : public GaudiAlgorithm {
public:
  /// Standard constructor
  CondDBReplayAlg( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~CondDBReplayAlg( ); ///< Destructor

  StatusCode initialize() override;    ///< Algorithm initialization
  StatusCode execute   () override;    ///< Algorithm execution
  StatusCode finalize  () override;    ///< Algorithm finalization

protected:

private:

  /// Path to the file containing the log.
  std::string m_logFileName;

  /// Name of the reader to use to replay the requests.
  std::string m_readerName;

  /// Pointer to the ICondDBReader service.
  ICondDBReader *m_reader;

  struct operation_t {
    Gaudi::Time time;
    std::string node;
    Gaudi::Time evttime;
    bool use_numeric_channel;
    cool::ChannelId channel;
    std::string chn_name;
  };
  typedef std::list<operation_t> list_t;

  /// List of operations to perform
  list_t m_operations;

};

#endif /*CONDDBREPLAYALG_H_*/
