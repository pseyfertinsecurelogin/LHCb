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
#ifndef IReferenceRate_h
#define IReferenceRate_h

// Include files
#include "GaudiKernel/INamedInterface.h"

/** @class IReferenceRate IReferenceRate.h
 *
 *  functionality: provide a reference rate
 *
 *  @author Gerhard Raven and Wouter Hulsbergen
 *  @date   2009-01-01
 */


struct IReferenceRate : extend_interfaces<INamedInterface>
{
  DeclareInterfaceID( IReferenceRate, 2, 0 );

  virtual double rate() const = 0 ;
  virtual size_t tick() const = 0 ;
};
#endif // IReferenceRate_h
