/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

// Include files
#include "Event/MuonCoord.h"
#include "GaudiKernel/KeyedContainer.h"
#include "GaudiKernel/KeyedObject.h"
#include "GaudiKernel/SmartRefVector.h"
#include "Kernel/MuonTileID.h"
#include <ostream>
#include <vector>

// Forward declarations

namespace LHCb {

  // Forward declarations

  // Class ID definition
  static const CLID CLID_MuonCluster = 11041;

  // Namespace for locations in TDS
  namespace MuonClusterLocation {
    inline const std::string MuonClusters = "Raw/Muon/Clusters";
  }

  /** @class MuonCluster MuonCluster.h
   *
   * Reconstructed muon hit clusters
   *
   * @author Alessia Satta
   *
   */

  class MuonCluster final : public KeyedObject<int> {
  public:
    /// typedef for KeyedContainer of MuonCluster
    typedef KeyedContainer<MuonCluster, Containers::HashMap> Container;

    /// create a cluster from single coords
    MuonCluster( LHCb::MuonCoord* coord, float x, float dx, float y, float dy, float z, float dz );

    /// Copy constructor
    MuonCluster( const MuonCluster& clu );

    /// Default Constructor
    MuonCluster()
        : m_x( 0.0 )
        , m_y( 0.0 )
        , m_z( 0.0 )
        , m_xmin( 0.0 )
        , m_ymin( 0.0 )
        , m_zmin( 0.0 )
        , m_xmax( 0.0 )
        , m_ymax( 0.0 )
        , m_zmax( 0.0 ) {}

    // Retrieve pointer to class definition structure
    const CLID&        clID() const override;
    static const CLID& classID();

    /// Fill the ASCII output stream
    std::ostream& fillStream( std::ostream& s ) const override;

    /// Add a pad to the cluster!
    void addPad( LHCb::MuonCoord* coord, float x, float dx, float y, float dy, float z, float dz );

    /// Add a pad to the cluster!
    void addPad( LHCb::MuonCoord* coord );

    /// check if  a pad is already in  the cluster!
    bool checkPad( const LHCb::MuonCoord* coord );

    /// merge two clusters
    void mergeCluster( LHCb::MuonCluster* cluster );

    /// Retrieve const  average x position of the cluster
    float x() const;

    /// Update  average x position of the cluster
    void setX( float value );

    /// Retrieve const  average y position of the cluster
    float y() const;

    /// Update  average y position of the cluster
    void setY( float value );

    /// Retrieve const  average z position of the cluster
    float z() const;

    /// Update  average z position of the cluster
    void setZ( float value );

    /// Retrieve const  left x position of the cluster
    float xmin() const;

    /// Update  left x position of the cluster
    void setXmin( float value );

    /// Retrieve const  left y position of the cluster
    float ymin() const;

    /// Update  left y position of the cluster
    void setYmin( float value );

    /// Retrieve const  left z position of the cluster
    float zmin() const;

    /// Update  left z position of the cluster
    void setZmin( float value );

    /// Retrieve const  right x position of the cluster
    float xmax() const;

    /// Update  right x position of the cluster
    void setXmax( float value );

    /// Retrieve const  right y position of the cluster
    float ymax() const;

    /// Update  right y position of the cluster
    void setYmax( float value );

    /// Retrieve const  right z position of the cluster
    float zmax() const;

    /// Update  right z position of the cluster
    void setZmax( float value );

    /// Retrieve (const)  References to the MuonCoords
    const SmartRefVector<LHCb::MuonCoord>& coords() const;

    /// Update  References to the MuonCoords
    void setCoords( const SmartRefVector<LHCb::MuonCoord>& value );

    /// Add to  References to the MuonCoords
    void addToCoords( const SmartRef<LHCb::MuonCoord>& value );

    /// Att to (pointer)  References to the MuonCoords
    void addToCoords( const LHCb::MuonCoord* value );

    /// Remove from  References to the MuonCoords
    void removeFromCoords( const SmartRef<LHCb::MuonCoord>& value );

    /// Clear  References to the MuonCoords
    void clearCoords();

    friend std::ostream& operator<<( std::ostream& str, const MuonCluster& obj ) { return obj.fillStream( str ); }

  protected:
  private:
    float                           m_x;      ///< average x position of the cluster
    float                           m_y;      ///< average y position of the cluster
    float                           m_z;      ///< average z position of the cluster
    float                           m_xmin;   ///< left x position of the cluster
    float                           m_ymin;   ///< left y position of the cluster
    float                           m_zmin;   ///< left z position of the cluster
    float                           m_xmax;   ///< right x position of the cluster
    float                           m_ymax;   ///< right y position of the cluster
    float                           m_zmax;   ///< right z position of the cluster
    SmartRefVector<LHCb::MuonCoord> m_coords; ///< References to the MuonCoords

  }; // class MuonCluster

  /// Definition of Keyed Container for MuonCluster
  typedef KeyedContainer<MuonCluster, Containers::HashMap> MuonClusters;

} // namespace LHCb

// -----------------------------------------------------------------------------
// end of class
// -----------------------------------------------------------------------------

// Including forward declarations

inline LHCb::MuonCluster::MuonCluster( LHCb::MuonCoord* coord, float x, float dx, float y, float dy, float z, float dz )
    : m_x( x ), m_y( y ), m_z( z ) {

  addToCoords( coord );
  m_xmin = x - dx;
  m_ymin = y - dy;
  m_zmin = z - dz;
  m_xmax = x + dx;
  m_ymax = y + dy;
  m_zmax = z + dz;
}

inline LHCb::MuonCluster::MuonCluster( const MuonCluster& clu ) : KeyedObject<int>() {

  setX( clu.x() );
  setY( clu.y() );
  setZ( clu.z() );
  setXmin( clu.xmin() );
  setYmin( clu.ymin() );
  setZmin( clu.zmin() );
  setXmax( clu.xmax() );
  setYmax( clu.ymax() );
  setZmax( clu.zmax() );
  setCoords( clu.coords() );
}

inline const CLID& LHCb::MuonCluster::clID() const { return LHCb::MuonCluster::classID(); }

inline const CLID& LHCb::MuonCluster::classID() { return CLID_MuonCluster; }

inline std::ostream& LHCb::MuonCluster::fillStream( std::ostream& s ) const {
  s << "{ "
    << "x :	" << m_x << std::endl
    << "y :	" << m_y << std::endl
    << "z :	" << m_z << std::endl
    << "xmin :	" << m_xmin << std::endl
    << "ymin :	" << m_ymin << std::endl
    << "zmin :	" << m_zmin << std::endl
    << "xmax :	" << m_xmax << std::endl
    << "ymax :	" << m_ymax << std::endl
    << "zmax :	" << m_zmax << std::endl
    << " }";
  return s;
}

inline float LHCb::MuonCluster::x() const { return m_x; }

inline void LHCb::MuonCluster::setX( float value ) { m_x = value; }

inline float LHCb::MuonCluster::y() const { return m_y; }

inline void LHCb::MuonCluster::setY( float value ) { m_y = value; }

inline float LHCb::MuonCluster::z() const { return m_z; }

inline void LHCb::MuonCluster::setZ( float value ) { m_z = value; }

inline float LHCb::MuonCluster::xmin() const { return m_xmin; }

inline void LHCb::MuonCluster::setXmin( float value ) { m_xmin = value; }

inline float LHCb::MuonCluster::ymin() const { return m_ymin; }

inline void LHCb::MuonCluster::setYmin( float value ) { m_ymin = value; }

inline float LHCb::MuonCluster::zmin() const { return m_zmin; }

inline void LHCb::MuonCluster::setZmin( float value ) { m_zmin = value; }

inline float LHCb::MuonCluster::xmax() const { return m_xmax; }

inline void LHCb::MuonCluster::setXmax( float value ) { m_xmax = value; }

inline float LHCb::MuonCluster::ymax() const { return m_ymax; }

inline void LHCb::MuonCluster::setYmax( float value ) { m_ymax = value; }

inline float LHCb::MuonCluster::zmax() const { return m_zmax; }

inline void LHCb::MuonCluster::setZmax( float value ) { m_zmax = value; }

inline const SmartRefVector<LHCb::MuonCoord>& LHCb::MuonCluster::coords() const { return m_coords; }

inline void LHCb::MuonCluster::setCoords( const SmartRefVector<LHCb::MuonCoord>& value ) { m_coords = value; }

inline void LHCb::MuonCluster::addToCoords( const SmartRef<LHCb::MuonCoord>& value ) { m_coords.push_back( value ); }

inline void LHCb::MuonCluster::addToCoords( const LHCb::MuonCoord* value ) { m_coords.push_back( value ); }

inline void LHCb::MuonCluster::removeFromCoords( const SmartRef<LHCb::MuonCoord>& value ) {
  auto i = std::remove( m_coords.begin(), m_coords.end(), value );
  m_coords.erase( i, m_coords.end() );
}

inline void LHCb::MuonCluster::clearCoords() { m_coords.clear(); }

inline void LHCb::MuonCluster::addPad( LHCb::MuonCoord* coord, float x, float dx, float y, float dy, float z,
                                       float dz ) {

  if ( checkPad( coord ) ) return;
  addToCoords( coord );
  if ( m_xmin > x - dx ) m_xmin = x - dx;
  if ( m_ymin > y - dy ) m_ymin = y - dy;
  if ( m_zmin > z - dz ) m_zmin = z - dz;
  if ( m_xmax > x + dx ) m_xmax = x + dx;
  if ( m_ymax > y + dy ) m_ymax = y + dy;
  if ( m_zmax > z + dz ) m_zmax = z + dz;
}

inline void LHCb::MuonCluster::addPad( LHCb::MuonCoord* coord ) {

  if ( checkPad( coord ) ) return;
  addToCoords( coord );
}

inline bool LHCb::MuonCluster::checkPad( const LHCb::MuonCoord* coord ) {

  SmartRefVector<LHCb::MuonCoord>::iterator it;
  for ( it = m_coords.begin(); it != m_coords.end(); it++ ) {
    LHCb::MuonCoord* mycoord = *it;
    if ( mycoord->key() == coord->key() ) return true;
  }
  return false;
}

inline void LHCb::MuonCluster::mergeCluster( LHCb::MuonCluster* cluster ) {

  if ( cluster->xmin() < m_xmin ) m_xmin = cluster->xmin();
  if ( cluster->ymin() < m_ymin ) m_ymin = cluster->ymin();
  if ( cluster->zmin() < m_zmin ) m_zmin = cluster->zmin();
  if ( cluster->xmax() > m_xmax ) m_xmax = cluster->xmax();
  if ( cluster->ymax() > m_ymax ) m_ymax = cluster->ymax();
  if ( cluster->zmax() > m_zmax ) m_zmax = cluster->zmax();
  SmartRefVector<LHCb::MuonCoord>::const_iterator it;

  for ( it = cluster->coords().begin(); it != cluster->coords().end(); it++ ) {

    const LHCb::MuonCoord* mycoord = *it;
    if ( checkPad( mycoord ) ) continue;
    addToCoords( mycoord );
  }
}
