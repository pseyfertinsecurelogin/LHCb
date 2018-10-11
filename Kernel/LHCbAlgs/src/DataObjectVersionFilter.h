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
// $Id: DataObjectVersionFilter.h,v 1.3 2009-11-10 09:27:51 jonrob Exp $
#ifndef DATAOBJECTVERSIONFILTER_H
#define DATAOBJECTVERSIONFILTER_H 1

// Include files
// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"


/** @class DataObjectVersionFilter DataObjectVersionFilter.h
 *
 *  Checks the version of a given DataObject in the TES, and sets FilterPassed according
 *  to a min and max version number. Useful to allow reprocessing of some data depending
 *  on the version number
 *
 *  @author Chris Jones
 *  @date   2009-11-06
 */
class DataObjectVersionFilter final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  DataObjectVersionFilter( const std::string& name, ISvcLocator* pSvcLocator );

  virtual ~DataObjectVersionFilter( ) = default; ///< Destructor

  StatusCode execute() override;    ///< Algorithm execution

private:

  unsigned int m_minV;          ///< Min version number
  unsigned int m_maxV;          ///< Max version number
  std::string m_loc;            ///< DataObject location in TES

};

#endif // DATAOBJECTVERSIONFILTER_H
