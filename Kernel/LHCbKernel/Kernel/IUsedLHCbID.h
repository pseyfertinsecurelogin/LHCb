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
#ifndef _IUsedLHCbID_H
#define _IUsedLHCbID_H

#include "GaudiKernel/IAlgTool.h"
#include "Kernel/LHCbID.h"

/** @class IUsedLHCbID IUsedLHCbID.h Kernel/IUsedLHCbID.h
 *
 *  interface for testing if a given id is used.
 *
 *  @author M.Needham
 *  @date   02/08/2006
 */

struct IUsedLHCbID : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IUsedLHCbID, 1, 0 );

  /** Test if the LHCbID is used
   * @param id to be test tested
   *  @return true if used
   */
  [[nodiscard]] virtual bool used( const LHCb::LHCbID id ) const = 0;
};

#endif
