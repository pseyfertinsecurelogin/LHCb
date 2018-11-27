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
/** @file module.cpp
 *
 *  Small Python module to use the CondDB (de)compression.
 *
 */

#include <boost/python.hpp>

#include "../component/CondDBCompression.h"

namespace {
  inline std::string defaultCompress(const std::string& s) {
    return CondDBCompression::compress(s);
  }
}

BOOST_PYTHON_MODULE(CondDBCompression)
{
    using namespace boost::python;
    def("compress", CondDBCompression::compress);
    def("compress", defaultCompress);
    def("decompress", CondDBCompression::decompress);
}
