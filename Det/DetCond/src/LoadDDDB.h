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
#ifndef LOADDDDB_H
#define LOADDDDB_H 1

#include "GaudiAlg/GaudiAlgorithm.h"

/** @class LoadDDDB LoadDDDB.h
 *
 *  Load entries in the detector transient store using IDataSvc::preLoad().
 *  The node to be loaded is set with the option LoadDDDB.Node.
 *
 *  @author Marco Clemencic
 *  @date   2005-10-14
 */
class LoadDDDB : public GaudiAlgorithm {
public:
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode initialize() override; ///< Algorithm initialization
  StatusCode execute() override;    ///< Algorithm execution
  StatusCode finalize() override;   ///< Algorithm finalization

private:
  Gaudi::Property<std::string> m_treeToLoad{this, "Node", "/dd*"};
};
#endif // LOADDDDB_H
