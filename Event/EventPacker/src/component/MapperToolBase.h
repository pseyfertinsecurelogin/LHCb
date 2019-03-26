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
#ifndef MAPPERTOOLBASE_H
#define MAPPERTOOLBASE_H 1

// from Gaudi
#include "GaudiAlg/GaudiTool.h"
#include "GaudiKernel/IJobOptionsSvc.h"

// interfaces
#include "GaudiKernel/IDODAlgMapper.h"
#include "GaudiKernel/IDODNodeMapper.h"

// Helpers for printing
#define ON_VERBOSE if ( UNLIKELY( msgLevel( MSG::VERBOSE ) ) )
#define ON_DEBUG if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) )
#define LOG_VERBOSE ON_VERBOSE verbose()
#define LOG_DEBUG ON_DEBUG debug()

/** @class MapperToolBase MapperToolBase.h
 *
 *  Base class for mapper tools
 *
 *  @author Chris Jones
 *  @date   2012-03-26
 */
class MapperToolBase : public extends<GaudiTool, IDODAlgMapper, IDODNodeMapper> {

public:
  /// Standard constructor
  using base_class::base_class;

  /// Initialize the tool instance.
  StatusCode finalize() override;

protected:
  /// Access the Job Option Service
  SmartIF<IJobOptionsSvc>& joSvc() const;

  /// Get the Stream name from a data path
  std::string streamName( const std::string& path ) const;

  /// Get the stream root from a data path
  inline std::string streamRoot( const std::string& path ) const { return "/Event/" + streamName( path ); }

  /// Make sure a path starts with /Event/
  inline std::string fixPath( const std::string& path ) const {
    return path.compare( 0, 7, "/Event/" ) != 0 ? "/Event/" + path : path;
  }

private:
  /// Reference to the JobOptionsSvc.
  /// It is needed to configure the Algorithm instances.
  mutable SmartIF<IJobOptionsSvc> m_jos;
};

#endif // MAPPERTOOLBASE_H
