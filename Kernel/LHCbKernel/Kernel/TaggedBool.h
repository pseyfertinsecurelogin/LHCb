#ifndef LHCB_KERNEL_TAGGED_BOOL
#define LHCB_KERNEL_TAGGED_BOOL

#include "GaudiKernel/TaggedBool.h"

namespace xplicit {

// [[deprecated("please use Gaudi::tagged_bool instead")]]
template <typename Tag>
using tagged_bool = Gaudi::tagged_bool<Tag>;

}

#endif

