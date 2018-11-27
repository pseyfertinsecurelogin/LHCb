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

#include <string>

namespace Rich
{

  /** @enum Rich::RichPhDetConfigType
   *
   *  RICH photon detector type
   *
   *  @author Sajan Easo
   *  @date   17/11/2011
   */
  enum RichPhDetConfigType
  {
    InvalidPDConfig = -1, ///< Invalid PD config
    HPDConfig       = 0,  ///< HPD photodetectors
    PMTConfig       = 1   ///< PMT photodetectors
  };

  /** Text conversion for RichPhDetConfigType enumeration
   *
   *  @param conf RICH photon detector type
   *  @return RICH photon detector type as a string
   */
  std::string text( const Rich::RichPhDetConfigType conf );

} // namespace Rich
