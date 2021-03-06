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
#ifndef _DeTTLayer_H_
#define _DeTTLayer_H_

#include <string>
#include <vector>

#include "STDet/DeSTLayer.h"

class DeTTHalfModule;
class DeTTStation;

/** @class DeTTLayer DeTTLayer.h "STDet/DeTTLayer.h"
 *
 *  TT Layer detector element
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on
 STDet</b></a> \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *
 */

static const CLID CLID_DeTTLayer = 9103;

class DeTTLayer : public DeSTLayer {

public:
  /** parent type */
  typedef STDetTraits<DeTTLayer>::parent parent_type;

  /** child type */
  typedef STDetTraits<DeTTLayer>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeTTLayer( const std::string& name = "" );

  /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
  static const CLID& classID() { return CLID_DeTTLayer; }

  /**
   * another reference to class identifier
   * @return the class identifier for this class
   */
  const CLID& clID() const override;

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;

  /**  locate half module based on a channel id
  @return  module */
  DeTTHalfModule* findHalfModule( const LHCb::STChannelID aChannel );

  /** locate half module  based on a point
  @return module */
  DeTTHalfModule* findHalfModule( const Gaudi::XYZPoint& point );

  /** check whether contains
   *  @param  aChannel channel
   *  @return bool
   */
  bool contains( const LHCb::STChannelID aChannel ) const override;

  /** vector of children */
  const Children& halfModules() const;

  /**
   * fraction active channels
   * @return bool fraction active
   */
  double fractionActive() const;

private:
  /** make flat list of lowest descendents  and also layers*/
  void flatten();

  Children     m_modules;
  parent_type* m_parent;
};

#include "STDet/DeTTHalfModule.h"

inline bool DeTTLayer::contains( const LHCb::STChannelID aChannel ) const {
  return ( elementID().station() == aChannel.station() && ( elementID().layer() == aChannel.layer() ) );
}

inline const DeTTLayer::Children& DeTTLayer::halfModules() const { return m_modules; }

#endif // _DeTTLayer_H
