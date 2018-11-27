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
#ifndef _DeITSector_H_
#define _DeITSector_H_

#include <string>

#include "Kernel/STChannelID.h"
#include "STDet/DeSTSector.h"

class DeITLayer;

/** @class DeITSector DeITSector.h "STDet/DeITSector.h"
 *
 *  Class representing a IT Ladder  --> sensor part
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on STDet</b></a>
   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *  @author Matthew Needham Matthew.Needham@cern.ch
 */

static const CLID CLID_DeITSector = 9206;

class DeITSector : public DeSTSector  {

public:

  /** parent type */
  typedef STDetTraits<DeITSector>::parent parent_type;

  /** Constructor */
  DeITSector ( const std::string& name = "" ) ;

  unsigned int prodID() const override;

  /**
  * Retrieves reference to class identifier
  * @return the class identifier for this class
  */
  static const CLID& classID(){return CLID_DeITSector;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  std::string conditionsPathName() const override;

private:

  StatusCode updateProdIDCondition();

  parent_type* m_parent;
  unsigned int m_prodID;
  std::string m_prodIDString;

};


#endif // _DeITSector_H








