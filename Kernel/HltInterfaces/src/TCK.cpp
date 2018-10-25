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
#include "Kernel/TCK.h"
#include <string>
#include <iomanip>
#include "boost/regex.hpp"
#include "boost/algorithm/string/case_conv.hpp"
#include "GaudiKernel/StatusCode.h"
#include "GaudiKernel/GaudiException.h"
#include "GaudiKernel/ParsersFactory.h"

TCK&
TCK::set(const std::string& s) {
    static const boost::regex e("^(0x[0-9a-fA-F]{8})$");
    boost::smatch what;
    if(!boost::regex_match(s, what, e)) {
        throw GaudiException("Invalid TCK format",s,StatusCode::FAILURE);
        return *this;
    }
    m_unsigned = std::stoul(what[1],nullptr,0);
    m_stringRep = s;
    // canonical rep is lower case...
    boost::algorithm::to_lower(m_stringRep);
    return *this;
}

StatusCode parse(TCK& result, const std::string& input )
{
  std::stringstream ss(input);
  std::string s; ss >> std::quoted(s,'\'');
  try { result = TCK{s}; } catch ( GaudiException& e ) {
    return e.code();
  }
  return StatusCode::SUCCESS;
}
