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
#ifndef IEVENTCOUNTER_H
#define IEVENTCOUNTER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class IEventCounter IEventCounter.h
 *
 *
 *  @author Patrick KOPPENBURG
 *  @date   2004-06-18
 *  @author Marco CLEMENCIC
 *  @date   2012-06-12
 */
struct IEventCounter : extend_interfaces<IAlgTool>
{

  /// InterfaceID
  DeclareInterfaceID(IEventCounter, 2, 0);

  virtual unsigned long long getEventCounter() const = 0;   ///< Get Event Counter

  virtual void incrementEventCounter() = 0;   ///< Increment Event Counter

  /// Set the event counter overriding the current value.
  virtual void setEventCounter(unsigned long long n) = 0;

};

#endif // IEVENTCOUNTER_H
