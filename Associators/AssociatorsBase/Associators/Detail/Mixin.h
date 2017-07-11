#ifndef LINKERMIXIN_H
#define LINKERMIXIN_H

#include <GaudiKernel/LinkManager.h>
#include <GaudiKernel/ContainedObject.h>
#include <Event/LinksByKey.h>

#include "OutputLinks.h"
#include <Associators/InputLinks.h>

namespace {
   struct DefaultType {};
}

namespace Detail {

template <typename Source>
class LinkerMixin {
public:

   static std::string location(const std::string& location)
   {
      return "Link/" + (location.compare(0,7,"/Event/") == 0 ? location.substr(7)
                                                             : location);
   }

   // Bit of a trick here with enable if: if Target is specified,
   // so it's not DefaultType, we assume that the user wants
   // to use the given types for Source and Target
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if<!std::is_same<DefaultType, Target>::value, Target>::type* = nullptr>
   InputLinks<Src, Target> inputLinks(const LHCb::LinksByKey& links) const
   {
      return InputLinks<Src, Target>{links};
   }

   // Bit of a trick here with enable if: if Target is not specified,
   // so it's DefaultType, we assume that the user wants Source to be
   // ContainedObject and Targe to be Target, the remaining parameter
   // is therefore taken to be the target type.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if<std::is_same<DefaultType, Target>::value, Target>::type* = nullptr>
   InputLinks<ContainedObject, Src> inputLinks(const LHCb::LinksByKey& links) const
   {
      return InputLinks<ContainedObject, Src>{links};
   }

   // Bit of a trick here with enable if: if Target is not specified,
   // so it's DefaultType, we assume that the user wants Src to be
   // ContainedObject and Target to be Target, the remaining template
   // parameter (Src) is therefore taken to be the target type.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if<std::is_same<DefaultType, Target>::value, Target>::type* = nullptr>
   Detail::OutputLinks<Source, Src> outputLinks() const
   {
      return Detail::OutputLinks<Source, Src>{};
   }

   // Bit of a trick here with enable if: if Target is specified, so
   // it's not DefaultType, we assume that the user wants to use the
   // given types.
   template <typename Src, typename Target = DefaultType,
             typename std::enable_if<!std::is_same<DefaultType, Target>::value, Target>::type* = nullptr>
   Detail::OutputLinks<Src, Target> outputLinks() const
   {
      return Detail::OutputLinks<Src, Target>{};
   }

};

}

#endif
