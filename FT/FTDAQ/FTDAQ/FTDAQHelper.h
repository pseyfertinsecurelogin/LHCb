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
#ifndef FTDAQHELPER_H
#define FTDAQHELPER_H 1

#include "Kernel/STLExtensions.h"

namespace LHCb {

// forward declaration
class RawBank;

namespace FTDAQ {

/**
 * counts number of FT clusters in the given raw banks
 */
unsigned int nbFTClusters(LHCb::span<const RawBank *> banks);

} // namespace FTDAQ

} // namespace LHCb

#endif // FTDAQHELPER_H
