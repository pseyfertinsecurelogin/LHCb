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
#ifndef DATAOBJECTVERSIONFILTER_H
#define DATAOBJECTVERSIONFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"
#include "GaudiKernel/DataObjectHandle.h"

/** @class DataObjectVersionFilter DataObjectVersionFilter.h
 *
 *  Checks the version of a given DataObject in the TES, and sets FilterPassed according
 *  to a min and max version number. Useful to allow reprocessing of some data depending
 *  on the version number
 *
 *  @author Chris Jones
 *  @date   2009-11-06
 */
class DataObjectVersionFilter final : public GaudiAlgorithm {
public:
  /// Standard constructor
  using GaudiAlgorithm::GaudiAlgorithm;

  StatusCode execute() override; ///< Algorithm execution

private:
  Gaudi::Property<unsigned int>    m_minV{this, "MinVersion", 0, "Min version number"};
  Gaudi::Property<unsigned int>    m_maxV{this, "MaxVersion", 9999999, "Max version number"};
  DataObjectReadHandle<DataObject> m_loc = {this, "DataObjectLocation", "", "DataObject location in TES"};
};

#endif // DATAOBJECTVERSIONFILTER_H
