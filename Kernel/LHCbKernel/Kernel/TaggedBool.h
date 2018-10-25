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
#ifndef LHCB_KERNEL_TAGGED_BOOL
#define LHCB_KERNEL_TAGGED_BOOL

#include "GaudiKernel/TaggedBool.h"

namespace xplicit {

template <typename Tag>
using tagged_bool  [[deprecated("please use Gaudi::tagged_bool instead")]] = Gaudi::tagged_bool<Tag>;

}

#endif

