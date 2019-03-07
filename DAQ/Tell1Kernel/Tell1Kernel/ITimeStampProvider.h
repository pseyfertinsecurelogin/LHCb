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
#ifndef KERNEL_ITIMESTAMPPROVIDER_H
#define KERNEL_ITIMESTAMPPROVIDER_H 1

// Include files
// from STL
#include <string>

// from Gaudi
#include "GaudiKernel/IAlgTool.h"

namespace Velo {
  static const InterfaceID IID_ITimeStampProvider( "ITimeStampProvider", 1, 0 );

  /** @class ITimeStampProvider ITimeStampProvider.h Kernel/ITimeStampProvider.h
   *
   *  Interface for providers of time stamp string.
   *
   *  Time stamp strings are provided for initialization and current time.
   *
   *  The exact format of the time stamp strings is implementation specific.
   *
   *  @author Kurt Rinnert
   *  @date   2009-08-21
   */

  class ITimeStampProvider : virtual public IAlgTool {
  public:
    // Return the interface ID
    static const InterfaceID& interfaceID() { return IID_ITimeStampProvider; }

    virtual std::string initTime() = 0;

    virtual std::string now() = 0;
  };
} // namespace Velo
#endif // KERNEL_ITIMESTAMPPROVIDER_H
