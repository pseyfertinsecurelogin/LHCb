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
#ifndef DETDESC_ICONDITIONINFO_H
#define DETDESC_ICONDITIONINFO_H 1

#include "GaudiKernel/IInterface.h"
// Forward declarations
struct IDetectorElement;
struct Condition;

/** the unique interface identifier
 * ( unique interface identifier , major & minor versions)
 */
static const InterfaceID IID_IConditionInfo( "IConditionInfo" , 2 , 0 );

///---------------------------------------------------------------------------
/** @class IConditionInfo IConditionInfo.h Det/DetDesc/IConditionInfo.h

    An abstract interface to get the condition information of
    a detector element.

    @author Andrea Valassi
    @date December 2001
*///--------------------------------------------------------------------------

struct IConditionInfo : virtual IInterface
{

  /** retrieve the uniqie interface identifier
   *  @return the unique interface identifier
   */
  static const InterfaceID& interfaceID() { return IID_IConditionInfo; }

  /// Get a pointer to the detector element to which the ConditionInfo belongs
  virtual IDetectorElement* detElem() const = 0 ;

  /// Get the name of the associated condition
  virtual const std::string& conditionName() const = 0 ;

  /// Get a pointer to the associated condition
  virtual Condition* condition() = 0 ;

};

#endif // DETDESC_ICONDITIONINFO_H

