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
#ifndef _DeTTSector_H_
#define _DeTTSector_H_

#include <string>

#include "Kernel/STChannelID.h"
#include "STDet/DeSTSector.h"

/** @class DeTTSector DeTTSector.h "STDet/DeTTSector.h"
 *
 *  Class representing a TT Half module (7 sensors ganged together)
 *
 *  @author Matthew Needham Matthew.Needham@cern.ch
 *
 * <b> Additional information: </b>
 * \li <a href="http://doc.cern.ch//archive/electronic/cern/others/LHB/internal/lhcb-2006-034.pdf"><b>LHCb note on STDet</b></a>
   \li  <a href="http://ckm.physik.unizh.ch/software"><b>ST Software page</b></a><p>
 *
 */

class DeTTHalfModule;

static const CLID CLID_DeTTSector = 9120;

class DeTTSector : public DeSTSector  {

public:

  /** parent type */
  typedef STDetTraits<DeTTSector>::parent parent_type;

  /** Constructor */
  DeTTSector ( const std::string& name = "" ) ;

  /**
  * Retrieves reference to class identifier
  * @return the class identifier for this class
  */
  static const CLID& classID(){return CLID_DeTTSector;}

  /**
  * another reference to class identifier
  * @return the class identifier for this class
  */
  const CLID& clID () const override;

  /**
   * Retrives the hybrid type
   * @return the hybrid type
   */
  std::string hybridType() const;

  /** initialization method
  * @return Status of initialisation
  */
  StatusCode initialize() override;

  /** column number */
  unsigned int column() const;

  /** row Number.... */
  unsigned int row() const;

  /** above or below the beam-pipe **/
  std::string position() const;

  /** production ID --> in fact parent ID */
  unsigned int prodID() const override;

  std::string conditionsPathName() const override;

  /** half module type */
  std::string halfModuleType() const;

private:

  parent_type* m_parent;
  unsigned int m_row;
  std::string m_hybridType;
  std::string m_conditionPathName;

  std::string moduleNumber(const unsigned int& chan,
                           const unsigned int& reg) const;
};

#include "STDet/DeTTHalfModule.h"

inline unsigned int DeTTSector::column() const {
  return m_parent->column();
}

inline std::string DeTTSector::position() const {
  return m_parent->position();
}

inline unsigned int DeTTSector::row() const {
  return m_row;
}

inline std::string DeTTSector::halfModuleType() const {
  return m_parent->type();
}


#endif // _DeTTSector_H








