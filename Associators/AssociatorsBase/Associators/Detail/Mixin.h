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
#ifndef LINKERMIXIN_H
#define LINKERMIXIN_H

#include <GaudiKernel/LinkManager.h>
#include <GaudiKernel/ContainedObject.h>
#include <Event/LinksByKey.h>

#include <Associators/OutputLinks.h>
#include <Associators/InputLinks.h>

namespace {
   struct DefaultType {};
}

namespace Detail {

template <typename Source>
class LinkerMixin {
public:

   // Bit of a trick here with enable if: if Target is specified, so
   // it's not DefaultType, we assume that the user wants the Source
   // and Target types of the returned InputLinks to be the specified
   // types.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if_t<!std::is_same<DefaultType, Target>::value>* = nullptr>
   InputLinks<Src, Target> inputLinks(const LHCb::LinksByKey& links) const
   {
      return InputLinks<Src, Target>{links};
   }

   // Bit of a trick here with enable if: if Target is not specified,
   // so it's DefaultType, we assume that the user wants the Source
   // and Target types of the returned InputLinks to be
   // ContainedObject and the specified type, respectively.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if_t<std::is_same<DefaultType, Target>::value>* = nullptr>
   InputLinks<ContainedObject, Src> inputLinks(const LHCb::LinksByKey& links) const
   {
      return InputLinks<ContainedObject, Src>{links};
   }

   // Bit of a trick here with enable if: if Target is not specified,
   // so it's DefaultType, we assume that the user wants the Source
   // and Target types of the returned OutputLinks to be
   // ContainedObject and the specified type, respectively.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if_t<std::is_same<DefaultType, Target>::value>* = nullptr>
   OutputLinks<Source, Src> outputLinks() const
   {
      return OutputLinks<Source, Src>{};
   }

   // Bit of a trick here with enable if: if Target is specified, so
   // it's not DefaultType, we assume that the user wants the Source
   // and Target types of the returned OutputLinks to be the specified
   // types.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if_t<!std::is_same<DefaultType, Target>::value>* = nullptr>
   OutputLinks<Src, Target> outputLinks() const
   {
      return OutputLinks<Src, Target>{};
   }

};

}

#endif
