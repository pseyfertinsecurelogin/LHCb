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
#ifndef KERNEL_IDETELEMFINDER_H 
#define KERNEL_IDETELEMFINDER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IInterface.h"

struct IDetectorElement;

/** @class IDetElemFinder IDetElemFinder.h Kernel/IDetElemFinder.h
 *  
 *  Interface for a service that provides associations from the path inside the 
 *  hierarchy of physical volumes to the corresponding detector element.
 *
 *  @author Marco Clemencic
 *  @date   2006-09-01
 */
struct IDetElemFinder : extend_interfaces<IInterface> {

  /// Return the interface ID
  DeclareInterfaceID( IDetElemFinder, 2, 0 );

  /// Find the detector element instance associated to a given physical volume path.
  /// Returns 0 if there is no suitable detector element.
  virtual const IDetectorElement * detectorElementForPath(const std::string &path) const = 0;


};
#endif // KERNEL_IDETELEMFINDER_H
