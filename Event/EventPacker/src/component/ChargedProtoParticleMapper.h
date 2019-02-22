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
#ifndef SRC_ChargedProtoParticleMapper_H
#define SRC_ChargedProtoParticleMapper_H 1

#include <map>
#include <string>

// base class
#include "MapperToolBase.h"

// Event model
#include "Event/PackedProtoParticle.h"

class IJobOptionsSvc;

/** @class ChargedProtoParticleMapper ChargedProtoParticleMapper.h
 *
 *  Tool for mappings for charged ProtoParticles
 *
 * @author Chris Jones
 * @date 16/04/2013
 */
class ChargedProtoParticleMapper : public MapperToolBase {

public:
  /// Standard constructor
  ChargedProtoParticleMapper( const std::string& type, const std::string& name, const IInterface* parent );

public:
  /** Returns the correctly configured and name instance of the
   *  Clusters unpacker, for the given path
   */
  Gaudi::Utils::TypeNameString algorithmForPath( const std::string& path ) override;

public:
  /** Instruct the DataOnDemandSvc to create DataObjects for the
   *  intermediate levels of a path we can handle.
   */
  std::string nodeTypeForPath( const std::string& path ) override;

private:
  /// Add a path to the node type mappings
  void addPath( const std::string& path );

  /// Check if a given path is in the list of data locations created
  inline bool pathIsHandled( const std::string& path ) const {
    // See if we have an entry for this path
    NodeTypeMap::const_iterator it = m_nodeTypeMap.find( fixPath( path ) );
    return ( it != m_nodeTypeMap.end() );
  }

  /// Check the node mappings are updated for the given path stream
  void updateNodeTypeMap( const std::string& path );

  /// Get the location of the packed ProtoParticles for the given stream
  std::string packedProtoLocation( const std::string& stream ) const {
    return stream + "/" + LHCb::PackedProtoParticleLocation::Charged;
  }

  /// Get the location of the packed ProtoParticles for the given stream
  std::string protoLocation( const std::string& stream ) const {
    return stream + "/" + LHCb::ProtoParticleLocation::Charged;
  }

private:
  /// Map to say which stream roots have been configured
  std::map<std::string, bool> m_streamsDone;

  /// Mapping between TES path and node type
  typedef std::map<std::string, std::string> NodeTypeMap;
  NodeTypeMap                                m_nodeTypeMap;

  /// List of track types for PID recalibration
  std::vector<std::string> m_tkTypes;

  /// List of hypo types for PID recalibration
  std::vector<std::string> m_pidTypes;

  /// ANN PID recalibration tuning
  std::string m_pidTune;

  /// String match regex for the Brunel application version for rerunning the ANNPID
  std::string m_regex;

  /// Outputlevel for unpackers created
  int m_unpackersOutputLevel;
};

#endif // SRC_ChargedProtoParticleMapper_H
