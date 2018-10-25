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
#include "futuredetails.h"
#include <algorithm>
#include <regex>
#include <boost/algorithm/string/predicate.hpp>

namespace {
  const std::regex s_outTypeParse{"([\"']?)(?:(?:CALOFUTURE)?(ADC|DIGIT)S?|(BOTH))\\1",
                                  std::regex_constants::icase};
}

namespace futuredetails {
    using boost::iequals;

    const char* toString(const OutputType_t& out) {
        if (  out.adcOnTES &&  out.digitOnTES ) return "BOTH";
        if (  out.adcOnTES && !out.digitOnTES ) return "ADCS";
        if ( !out.adcOnTES &&  out.digitOnTES ) return "Digits";
        throw "IMPOSSIBLE!";
    }

    StatusCode parse(OutputType_t& result, const std::string& input ) {
        if ( std::smatch m; std::regex_match(input, m, s_outTypeParse) ) {
            const bool both = iequals(m[3].str(), "BOTH");
            result.digitOnTES = both || iequals(m[2].str(), "DIGIT");
            result.adcOnTES = both || iequals(m[2].str(), "ADC");
            return StatusCode::SUCCESS;
        }

        return StatusCode::FAILURE;
    }

    const char* toString(const source_t& source) {
        switch (source) {
            case source_t::from_adc   : return "ADC";
            case source_t::from_digit : return "DIGIT";
            default: throw "IMPOSSIBLE VALUE"; return nullptr;
        }
    }

    StatusCode parse(source_t& result, const std::string& input ) {
        if ( std::smatch m; std::regex_match(input, m, s_outTypeParse) ) {
          if ( iequals(m[2].str(), "ADC") ) {
            result = source_t::from_adc;
            return StatusCode::SUCCESS;
          } else if ( iequals(m[2].str(), "DIGIT") ) {
            result = source_t::from_digit;
            return StatusCode::SUCCESS;
          }
        }
        return StatusCode::FAILURE;
    }

    const std::string& toString( DetectorName_t dn ) {
        static const std::string labels[5] = { "Ecal","Hcal","Prs","Spd","Unknown" };
        auto i = static_cast<int>(dn);
        return labels[ i>4 ? 4 : i ];
    }

    StatusCode parse(DetectorName_t& result, std::string_view input ) {
        // remove optional quotes around the string
        if (!input.empty() && ( input.front() == '"' || input.front() == '\'' ) && input.front() == input.back()) {
          input.remove_prefix(1);
          input.remove_suffix(1);
        }

        result = DetectorName_t::Unknown;
        for (int i=0;i<4;++i) {
            auto dn = static_cast<DetectorName_t>( i );
            if (toString(dn)==input) { result = dn; return StatusCode::SUCCESS; };
        }
        return StatusCode::FAILURE;
    }

    DetectorName_t alg_name_to_detector(const std::string& s)
    {
      auto index = s.find_last_of(".") ;
      if (index==std::string::npos) { index = 0; }
      else { ++index; }
      if (s.compare(index+6,3, "Prs") == 0) return DetectorName_t::Prs;
      if (s.compare(index+6,3, "Spd") == 0) return DetectorName_t::Spd;
      auto t = s.substr(index+6,4);
      if ( t == "Hcal" ) return DetectorName_t::Hcal;
      if ( t == "Ecal" ) return DetectorName_t::Ecal;
      return DetectorName_t::Unknown;
    }
}
