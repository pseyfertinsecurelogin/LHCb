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

#ifndef ApplicationVersionFilter_H
#define ApplicationVersionFilter_H 1

// from Gaudi
#include "GaudiAlg/GaudiAlgorithm.h"

// Event
#include "Event/ProcessHeader.h"

// boost
#include <boost/regex.hpp>

/** @class ApplicationVersionFilter ApplicationVersionFilter.h
 *
 *  Checks the application version from the given ProcessHeader,
 *  comparing it to a given regex pattern to pass/reject the event.
 *
 *  @author Chris Jones
 *  @date   18-04-2013
 */
class ApplicationVersionFilter final : public GaudiAlgorithm
{

public:

  /// Standard constructor
  ApplicationVersionFilter( const std::string& name,
                            ISvcLocator* pSvcLocator );

  virtual ~ApplicationVersionFilter( ) = default; ///< Destructor

  StatusCode execute() override;    ///< Algorithm execution

private:

  std::string m_regex;    ///< String match regex
  std::string m_loc;      ///< DataObject location in TES

};

#endif // DATAOBJECTVERSIONFILTER_H
