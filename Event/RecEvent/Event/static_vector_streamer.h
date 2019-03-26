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
#include "GaudiKernel/SerializeSTL.h"
#include "boost/container/static_vector.hpp"

namespace GaudiUtils {
  template <typename T, std::size_t C>
  std::ostream& operator<<( std::ostream& s, const boost::container::static_vector<T, C>& c ) {
    return details::ostream_joiner( s << '[', c, ", " ) << ']';
  }
} // namespace GaudiUtils

using GaudiUtils::operator<<;
