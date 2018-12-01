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
#ifndef IOVLISTHELPERS_H
#define IOVLISTHELPERS_H
#include "DetCond/ICondDBReader.h"

namespace IOVListHelpers {
  /// Find the intervals in passed interval that are not covered by the provided.
  ICondDBReader::IOVList find_holes(const ICondDBReader::IOVList& data, const ICondDBReader::IOV& iov);
}

#endif
