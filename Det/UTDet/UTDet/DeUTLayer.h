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
#ifndef _DEUTLAYER_H_
#define _DEUTLAYER_H_

#include <string>
#include <vector>

#include "GaudiKernel/Plane3DTypes.h"
#include "Kernel/UTChannelID.h"
#include "UTDet/DeUTBaseElement.h"

class DeUTSector;
class DeUTStave;
class DeUTStation;

/** @class DeUTLayer DeUTLayer.h UTDet/DeUTLayer.h
 *
 *  UT Layer detector element
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

static const CLID CLID_DeUTLayer = 9303;

class DeUTLayer : public DeUTBaseElement {

public:
  typedef std::vector<DeUTSector*> Sectors;

  /** parent type */
  typedef UTDetTraits<DeUTLayer>::parent parent_type;

  /** child type */
  typedef UTDetTraits<DeUTLayer>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeUTLayer( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeUTLayer; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /** layer identifier
   *  @return identifier
   */
  unsigned int id() const;

  /** stereo angle
   *  @return identifier
   */
  double angle() const;

  /** cosine stereo angle
   *  @return identifier
   */
  double cosAngle() const;

  /** sine stereo angle
   *  @return identifier
   */
  double sinAngle() const;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to stream */
  MsgStream& printOut( MsgStream& os ) const override;

  /**  locate stave based on a channel id
  @return  stave */
  DeUTStave* findStave( const LHCb::UTChannelID aChannel );

  /** locate stave based on a point
  @return stave */
  DeUTStave* findStave( const Gaudi::XYZPoint& point );

  /** check whether contains
   *  @param  aChannel channel
   *  @return bool
   */
  bool contains( const LHCb::UTChannelID aChannel ) const override;

  /** flat vector of sectors
   * @return vector of sectors
   */
  const Sectors& sectors() const;

  /** plane corresponding to the sector
   * @return the plane
   */
  Gaudi::Plane3D plane() const;

  /**
   * Nickname for the layer
   **/
  const std::string& nickname() const;

  /** vector of children */
  const Children& staves() const;

  /**
   * fraction active channels
   * @return bool fraction active
   */
  double fractionActive() const;

protected:
  Sectors     m_sectors;
  std::string m_nickname;

private:
  StatusCode cachePlane();

  Gaudi::Plane3D m_plane;

  unsigned int m_id       = 0u;
  double       m_angle    = 0.0;
  double       m_sinAngle = 0.0;
  double       m_cosAngle = 0.0;

  /** make flat list of lowest descendents  and also layers*/
  void flatten();

  Children     m_staves;
  parent_type* m_parent = nullptr;
};

inline unsigned int DeUTLayer::id() const { return m_id; }

inline double DeUTLayer::angle() const { return m_angle; }

inline double DeUTLayer::cosAngle() const { return m_cosAngle; }

inline double DeUTLayer::sinAngle() const { return m_sinAngle; }

inline const DeUTLayer::Sectors& DeUTLayer::sectors() const { return m_sectors; }

inline Gaudi::Plane3D DeUTLayer::plane() const { return m_plane; }

/** ouput operator for class DeUTLayer
 *  @see DeUTLayer
 *  @see MsgStream
 *  @param os     reference to STL output stream
 *  @param aLayer reference to DeUTLayer object
 */
inline std::ostream& operator<<( std::ostream& os, const DeUTLayer* aLayer ) { return aLayer->printOut( os ); }

/** ouput operator for class DeUTLayer
 *  @see DeUTLayer
 *  @see MsgStream
 *  @param os     reference to MsgStream output stream
 *  @param aLayer reference to DeUTLayer object
 */
inline MsgStream& operator<<( MsgStream& os, const DeUTLayer* aLayer ) { return aLayer->printOut( os ); }

inline const std::string& DeUTLayer::nickname() const { return m_nickname; }

#include "UTDet/DeUTStave.h"

inline bool DeUTLayer::contains( const LHCb::UTChannelID aChannel ) const {
  return ( elementID().station() == aChannel.station() && ( elementID().layer() == aChannel.layer() ) );
}

inline const DeUTLayer::Children& DeUTLayer::staves() const { return m_staves; }

#endif // _DEUTLAYER_H
