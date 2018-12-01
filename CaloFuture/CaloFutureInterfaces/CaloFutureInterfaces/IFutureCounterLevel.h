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
#ifndef IFUTURECOUNTERLEVEL_H 
#define IFUTURECOUNTERLEVEL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"


/** @class IFutureCounterLevel IFutureCounterLevel.h
 *  
 *
 *  @author Olivier Deschamps
 *  @date   2016-08-13
 */
struct IFutureCounterLevel : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( IFutureCounterLevel, 1, 0 );

  virtual bool isQuiet() const = 0;
  virtual bool isVerbose() const = 0;
  virtual bool isLevel(int l) const=0;
  virtual int  level() const=0;

};
#endif // IFUTURECOUNTERLEVEL_H
