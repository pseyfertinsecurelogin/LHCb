#ifndef LHCB_KERNEL_TAGGED_BOOL
#define LHCB_KERNEL_TAGGED_BOOL

#include "GaudiKernel/TaggedBool.h"

namespace xplicit {

template <typename Tag>
using tagged_bool  [[deprecated("please use Gaudi::tagged_bool instead")]] = Gaudi::tagged_bool<Tag>;

}

#endif

