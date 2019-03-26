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
#ifndef ICOUNTERLEVEL_H
#define ICOUNTERLEVEL_H 1

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

/** @class ICounterLevel ICounterLevel.h
 *
 *
 *  @author Olivier Deschamps
 *  @date   2016-08-13
 */
struct ICounterLevel : extend_interfaces<IAlgTool> {

  DeclareInterfaceID( ICounterLevel, 1, 0 );

  virtual bool isQuiet() const        = 0;
  virtual bool isVerbose() const      = 0;
  virtual bool isLevel( int l ) const = 0;
  virtual int  level() const          = 0;
};
#endif // ICOUNTERLEVEL_H
