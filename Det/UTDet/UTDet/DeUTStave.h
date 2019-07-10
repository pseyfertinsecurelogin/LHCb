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
#ifndef _DEUTSTAVE_H_
#define _DEUTSTAVE_H_

#include <string>
#include <vector>

#include "Kernel/UTChannelID.h"
#include "UTDet/DeUTBaseElement.h"

class DeUTSensor;
class DeUTLayer;

/** @class DeUTStave DeUTStave.h UTDet/DeUTStave.h
 *
 *  Class representing a UT Stave (13 or 7 sensors)
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

static const CLID CLID_DeUTStave = 9310;

class DeUTStave : public DeUTBaseElement {

public:
  /** parent type */
  typedef UTDetTraits<DeUTStave>::parent parent_type;

  /** child type */
  typedef UTDetTraits<DeUTStave>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeUTStave( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeUTStave; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /** region where Stave is located
   * @return m_region
   */
  unsigned int detRegion() const;

  /// Workaround to prevent hidden base class function
  inline const std::type_info& type( const std::string& name ) const override {
    return ParamValidDataObject::type( name );
  }
  /** indicate the Stave type (A/B/C/D)
   * @return m_type
   */
  std::string type() const;

  /** indicate the Stave is rotated around Z or not
   * @return m_staveRotZ
   */
  std::string staveRotZ() const;

  /** first readout sector on Stave
   * @return m_firstSector
   */
  unsigned int firstSector() const;

  /** number of readout sectors expected
   * @return m_numSectors
   */
  unsigned int numSectorsExpected() const;

  /** last readout sector on Stave
   * @return m_firstSector
   */
  unsigned int lastSector() const;

  /** test whether contains channel
   * @param  aChannel test channel
   * @return bool
   */
  bool contains( const LHCb::UTChannelID aChannel ) const override;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to stream */
  MsgStream& printOut( MsgStream& os ) const override;

  /**  locate sector based on a channel id
  @return  sector */
  DeUTSector* findSector( const LHCb::UTChannelID aChannel );

  /** locate sector based on a point
  @return sector */
  DeUTSector* findSector( const Gaudi::XYZPoint& point );

  /** children */
  const DeUTStave::Children& sectors() const;

  /** column number */
  unsigned int column() const;

  /** production id */
  unsigned int prodID() const;

  /**
   * fraction active channels
   * @return bool fraction active
   */
  double fractionActive() const;

  /** version */
  std::string versionString() const;

protected:
  StatusCode updateProdIDCondition();

private:
  unsigned int m_detRegion   = 0;
  unsigned int m_firstSector = 0;
  unsigned int m_column      = 0;
  std::string  m_type;
  std::string  m_staveRotZ;
  unsigned int m_numSectors = 0;
  parent_type* m_parent     = nullptr;
  Children     m_sectors;
  unsigned int m_prodID        = 0;
  std::string  m_versionString = "DC11";
  std::string  m_prodIDString  = "ProdID";
};

#include "UTDet/DeUTLayer.h"
#include "UTDet/DeUTSector.h"

inline const DeUTStave::Children& DeUTStave::sectors() const { return m_sectors; }

inline unsigned int DeUTStave::detRegion() const { return m_detRegion; }

inline std::string DeUTStave::type() const { return m_type; }

inline std::string DeUTStave::staveRotZ() const { return m_staveRotZ; }

inline unsigned int DeUTStave::lastSector() const { return firstSector() + m_sectors.size() - 1u; }

inline unsigned int DeUTStave::firstSector() const { return m_firstSector; }

inline unsigned int DeUTStave::numSectorsExpected() const { return m_numSectors; }

inline unsigned int DeUTStave::column() const { return m_column; }

inline bool DeUTStave::contains( const LHCb::UTChannelID aChannel ) const {
  return ( ( aChannel.detRegion() == m_detRegion && aChannel.sector() >= m_firstSector &&
             aChannel.sector() < m_firstSector + m_sectors.size() ) &&
           m_parent->contains( aChannel ) );
}

inline unsigned int DeUTStave::prodID() const { return m_prodID; }

inline std::string DeUTStave::versionString() const { return m_versionString; }

/** output operator for class DeUTStave
 *  @see DeUTStave
 *  @see MsgStream
 *  @param os      reference to STL output stream
 *  @param aStave reference to DeUTStave object
 */
inline std::ostream& operator<<( std::ostream& os, const DeUTStave* aStave ) { return aStave->printOut( os ); }

/** output operator for class DeUTStave
 *  @see DeUTStave
 *  @see MsgStream
 *  @param os      reference to MsgStream output stream
 *  @param aStave reference to DeUTStave object
 */
inline MsgStream& operator<<( MsgStream& os, const DeUTStave* aStave ) { return aStave->printOut( os ); }

#endif // _DEUTSTAVE_H
