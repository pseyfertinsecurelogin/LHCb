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
#ifndef _DEUTMODULE_H_
#define _DEUTMODULE_H_

#include <string>

#include "UTDet/DeUTStave.h"

/** @class DeUTModule DeUTModule.h UTDet/DeUTModule.h
 *
 *  Class representing a UT module (13 or 7 sensors)
 *
 *  @author Andy Beiter (based on code by Jianchun Wang, Matt Needham)
 *  @date   2018-09-04
 *
 */

class DeUTModule : public DeUTStave {

public:
  /** Constructor */
  DeUTModule( const std::string& name = "" );

  /** initialization method
   * @return Status of initialisation
   */
  StatusCode initialize() override;
};
#endif // _DEUTMODULE_H
