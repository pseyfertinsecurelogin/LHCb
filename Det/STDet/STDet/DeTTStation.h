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
#ifndef _DeTTStation_H_
#define _DeTTStation_H_

#include <string>
#include <vector>

#include "STDet/DeSTStation.h"

class DeTTLayer;
class DeTTDetector;

/** @class DeTTStation DeTTStation.h "STDet/DeTTStation.h"
 *
 *  TT Station detector element
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
  * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on STDet</b></a>
   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *
 */

static const CLID CLID_DeTTStation = 9102;

class DeTTStation : public DeSTStation  {

public:

  /** parent type */
  typedef STDetTraits<DeTTStation>::parent parent_type;

  /** child type */
  typedef STDetTraits<DeTTStation>::child child_type;

  /** children */
  typedef std::vector<child_type*> Children;

  /** Constructor */
  DeTTStation ( const std::string& name = "" ) ;

   /**
   * Retrieves reference to class identifier
   * @return the class identifier for this class
   */
   static const CLID& classID(){return CLID_DeTTStation;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /**  locate the layer based on a channel id
  @return  layer */
  DeTTLayer* findLayer(const LHCb::STChannelID aChannel);

  /** locate layer based on a point
  @return layer */
  DeTTLayer* findLayer(const Gaudi::XYZPoint& point) ;

  /** vector of children */
  const Children& layers() const;

  /**
  * fraction active channels
  * @return bool fraction active
  */
  double fractionActive() const;

private:


  parent_type* m_parent;
  Children m_layers;

};


inline const DeTTStation::Children& DeTTStation::layers() const{
  return m_layers;
}

#endif // _DeTTStation_H



