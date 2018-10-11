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

namespace LHCb {
  namespace Tag {

    namespace State {
      struct AssumeUnordered_tag { };
      struct AssumeSorted_tag : AssumeUnordered_tag { };

      constexpr auto AssumeUnordered = AssumeUnordered_tag{};
      constexpr auto AssumeSorted    = AssumeSorted_tag{};
    }

  }
}
