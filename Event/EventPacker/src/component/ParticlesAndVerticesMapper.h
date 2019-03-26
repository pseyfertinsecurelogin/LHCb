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
#ifndef SRC_ParticlesAndVerticesMapper_H
#define SRC_ParticlesAndVerticesMapper_H 1

#include <map>
#include <string>

// base class
#include "MapperToolBase.h"

// Implemented interfaces
#include "GaudiKernel/IIncidentListener.h"
#include "GaudiKernel/IIncidentSvc.h"

// Event model
#include "Event/PackedFlavourTag.h"
#include "Event/PackedMuonPID.h"
#include "Event/PackedPartToRelatedInfoRelation.h"
#include "Event/PackedParticle.h"
#include "Event/PackedProtoParticle.h"
#include "Event/PackedRecVertex.h"
#include "Event/PackedRelations.h"
#include "Event/PackedRichPID.h"
#include "Event/PackedTrack.h"
#include "Event/PackedVertex.h"

#include "Event/StandardPacker.h"

class IJobOptionsSvc;

/** @class ParticlesAndVerticesMapper ParticlesAndVerticesMapper.h
 *
 *  Tool for automatic conversions in the transient store.
 *
 *  Bespoke mapping tool for the UnpackParticlesAndVertices unpacker
 *
 * @author Chris Jones
 * @date 06/02/2012
 */
class ParticlesAndVerticesMapper : public extends1<MapperToolBase, IIncidentListener> {

public:
  /// Standard constructor
  ParticlesAndVerticesMapper( const std::string& type, const std::string& name, const IInterface* parent );

  /// Initialize the tool instance.
  StatusCode initialize() override;

public:
  /** Implement the handle method for the Incident service.
   *  This is used to inform the tool of software incidents.
   *
   *  @param incident The incident identifier
   */
  void handle( const Incident& incident ) override;

public:
  /** Returns the correctly configured and name instance of the
   *  Particles and Vertices unpacker, for the given path
   */
  Gaudi::Utils::TypeNameString algorithmForPath( const std::string& path ) override;

public:
  /** Instruct the DataOnDemandSvc to create DataObjects for the
   *  intermediate levels of a path we can handle.
   */
  std::string nodeTypeForPath( const std::string& path ) override;

private:
  /// Load the packed data and update the mappings of paths to Node Type
  void updateNodeTypeMap( const std::string& path );

  /// Add a path to the node type mappings
  void addPath( const std::string& path );

  /// Check if a given path is in the list of data locations created
  inline bool pathIsHandled( const std::string& path ) const {
    // See if we have an entry for this path
    NodeTypeMap::const_iterator it = m_nodeTypeMap.find( fixPath( path ) );
    return ( it != m_nodeTypeMap.end() );
  }

private:
  /// Mapping between TES path and node type
  typedef std::map<std::string, std::string> NodeTypeMap;
  NodeTypeMap                                m_nodeTypeMap;

  /// Map to say which stream roots have been processed each event
  std::map<std::string, bool> m_streamsDone;

  StandardPacker m_pack; ///< Standard Packer

  /// Unpacker class type
  std::string m_unpackerType;

  /// Outputlevel for unpackers created
  int m_unpackersOutputLevel;

  // List of streams we should ignore paths under, e.g. `['/Event/Something']`
  std::vector<std::string> m_vetoStreams;
};

#endif // SRC_ParticlesAndVerticesMapper_H
