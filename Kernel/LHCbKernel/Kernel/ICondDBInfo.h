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
#ifndef DETCOND_ICONDDBINFO_H
#define DETCOND_ICONDDBINFO_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IInterface.h"
#include "Kernel/CondDBNameTagPair.h"

/** @class ICondDBInfo ICondDBInfo.h DetCond/ICondDBInfo.h
 *
 *  Interface class to obtain information about CondDB
 *
 *  @author Marco Clemencic
 *  @author Marco Cattaneo
 *  @date   2007-05-10
 */
struct ICondDBInfo : extend_interfaces<IInterface> {

  /// InterfaceID
  DeclareInterfaceID(ICondDBInfo, 2, 0);

  /** Get the current default database tags
   *  @param  tags vector of DB name, tag pairs. Empty if DB not available
   */
  virtual void defaultTags( std::vector<LHCb::CondDBNameTagPair>& tags) const = 0;

};
#endif // DETCOND_ICONDDBINFO_H
