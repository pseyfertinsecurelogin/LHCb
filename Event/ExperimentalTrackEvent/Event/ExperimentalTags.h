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
#pragma once

namespace experimental
{
  namespace Tag
  {

    namespace State
    {
      struct AssumeUnfiltered_tag {
      };
      struct AssumeFiltered_tag : AssumeUnfiltered_tag {
      };

      constexpr auto AssumeUnordered = AssumeUnfiltered_tag{};
      constexpr auto AssumeFiltered  = AssumeFiltered_tag{};
    } // namespace State

  } // namespace Tag
} // namespace experimental

