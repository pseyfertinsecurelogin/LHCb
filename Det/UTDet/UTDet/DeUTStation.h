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
#ifndef _DEUTSTATION_H_
#define _DEUTSTATION_H_

#include <string>
#include <vector>

#include "UTDet/DeUTBaseElement.h"

#include "Kernel/UTChannelID.h"

/// GaudiKernel
#include "GaudiKernel/MsgStream.h"

class DeUTLayer;
class DeUTDetector;

/** @class DeUTStation DeUTStation.h UTDet/DeUTStation.h
 *
 *  UT Station detector element
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

static const CLID CLID_DeUTStation = 9302;

class DeUTStation : public DeUTBaseElement  {

public:

  /** parent type */
  typedef UTDetTraits<DeUTStation>::parent parent_type;

  /** child type */
  typedef UTDetTraits<DeUTStation>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeUTStation ( std::string name = {} ) ;

  /**
  * Retrieves reference to class identifier
  * @return the class identifier for this class
  */
  static const CLID& classID(){return CLID_DeUTStation;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /** station identifier
  *  @return identifier
  */
  unsigned int id() const;

  /** print to stream */
  std::ostream& printOut( std::ostream& os ) const override;

  /** print to stream */
  MsgStream& printOut( MsgStream& os) const override;

  /** check contains channel
  *  @param  aChannel channel
  *  @return bool
  */
  bool contains(const LHCb::UTChannelID aChannel) const override;

  /**
  * Nickname for the station
  **/
  const std::string& nickname() const ;

  /**  locate the layer based on a channel id
  @return  layer */
  DeUTLayer* findLayer(const LHCb::UTChannelID aChannel);

  /** locate layer based on a point
  @return layer */
  DeUTLayer* findLayer(const Gaudi::XYZPoint& point) ;

  /** vector of children */
  const Children& layers() const;

  /**
  * fraction active channels
  * @return bool fraction active
  */
  double fractionActive() const;

protected:

  std::string m_nickname;

private:

  unsigned int m_id;

  parent_type* m_parent = nullptr;
  Children m_layers;

};

inline unsigned int DeUTStation::id() const{
  return m_id;
}

inline bool DeUTStation::contains(const LHCb::UTChannelID aChannel) const{
  return elementID().station() == aChannel.station();
}

inline const std::string& DeUTStation::nickname() const{
  return m_nickname;
}

/** ouput operator for class DeUTStation
 *  @see DeUTStation
 *  @see MsgStream
 *  @param os       reference to STL output stream
 *  @param aStation reference to DeUTStation object
 */
inline std::ostream& operator<<( std::ostream& os , const DeUTStation* aStation )
{ return aStation->printOut( os ); }


/** ouput operator for class DeUTStation
 *  @see DeUTStation
 *  @see MsgStream
 *  @param os       reference to MsgStream output stream
 *  @param aStation reference to DeUTStation object
 */
inline MsgStream& operator<<( MsgStream& os , const DeUTStation* aStation )
{ return aStation->printOut( os ); }

inline const DeUTStation::Children& DeUTStation::layers() const{
  return m_layers;
}

#endif // _DEUTSTATION_H
