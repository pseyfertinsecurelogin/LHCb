/*****************************************************************************\
* (c) Copyright 2000-2019 CERN for the benefit of the LHCb Collaboration      *
*                                                                             *
* This software is distributed under the terms of the GNU General Public      *
* Licence version 3 (GPL Version 3), copied verbatim in the file "COPYING".   *
*                                                                             *
* In applying this licence, CERN does not waive the privileges and immunities *
* granted to it by virtue of its status as an Intergovernmental Organization  *
* or submit itself to any jurisdiction.                                       *
\*****************************************************************************/
#pragma once

#include "MDF/Buffer.h"

#include "Event/RawEvent.h"

#include "GaudiKernel/IInterface.h"

#include <memory>
#include <tuple>

#pragma once

namespace LHCb {

  /**
   * The interface implemented by any class making IO and reading RawEvent Data
   */
  class IIOSvc : virtual public IInterface {

  public:
    struct EndOfInput {};

  public:
    /// InterfaceID
    DeclareInterfaceID( IIOSvc, 1, 0 );

    /**
     * get next event from input
     * @return a pair RawEvent, shared_ptr<Buffer> where the second one holds the data pointed to
     * by the first one
     * @throws IIOSvc::EndOfInput
     */
    virtual std::tuple<RawEvent, std::shared_ptr<LHCb::MDF::Buffer>> next() = 0;
  };

} // End namespace LHCb
