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

#pragma once

// local
#include "RichFutureKernel/RichCommonBase.h"
#include "RichFutureKernel/RichConverter_Imp.h"

namespace Rich::Future
{

  /** @class Converter RichFutureKernel/RichConverter.h
   *
   *  Base class for RICH converters
   *
   *  @author Chris Jones
   *  @date   2009-07-07
   */
  class Converter : public CommonBase< Converter_Imp >
  {

  public:

    /// Standard constructor
    Converter( long storage_type, const CLID &class_type, ISvcLocator *svc = nullptr )
      : CommonBase< Converter_Imp >( storage_type, class_type, svc )
    {}
  };

} // namespace Rich::Future
