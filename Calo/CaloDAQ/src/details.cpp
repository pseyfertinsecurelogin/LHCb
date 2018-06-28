#include "details.h"
#include <algorithm>
#include <regex>

namespace {
  const std::regex s_outTypeParse{"([\"']?)(?:(?:CALO)?(ADC|DIGIT)S?|(BOTH))\\1",
                                  std::regex_constants::icase};
}

namespace details {

    const char* toString(const OutputType_t& out) {
        if (  out.adcOnTES &&  out.digitOnTES ) return "BOTH";
        if (  out.adcOnTES && !out.digitOnTES ) return "ADCS";
        if ( !out.adcOnTES &&  out.digitOnTES ) return "Digits";
        throw "IMPOSSIBLE!";
    }

    StatusCode parse(OutputType_t& result, const std::string& input ) {
        std::string out( input );
        std::transform( input.begin() , input.end() , out.begin () , ::toupper ) ;

        std::smatch m;
        if ( std::regex_match(out, m, s_outTypeParse) ) {
            result.digitOnTES = m[3] == "BOTH" || m[2] == "DIGIT";
            result.adcOnTES = m[3] == "BOTH" || m[2] == "ADC";
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
        std::string out( input );
        std::transform( input.begin() ,input.end() , out.begin () , ::toupper ) ;

        std::smatch m;
        if ( std::regex_match(out, m, s_outTypeParse) ) {
          if ( m[2] == "ADC" ) {
            result = source_t::from_adc;
            return StatusCode::SUCCESS;
          } else if ( m[2] == "DIGIT" ) {
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

    StatusCode parse(DetectorName_t& result, const std::string& input ) {
        // remove optional quotes around the string
        std::string out;
        if (input.size() && input[0] == input[input.size()-1] && ( input[0] == '"' || input[0] == '\'' )) {
          out = input.substr(1, input.size()-2);
        } else out = input;

        result = DetectorName_t::Unknown;
        for (int i=0;i<4;++i) {
            auto dn = static_cast<DetectorName_t>( i );
            if (toString(dn)==out) { result = dn; return StatusCode::SUCCESS; };
        }
        return StatusCode::FAILURE;
    }

    DetectorName_t alg_name_to_detector(const std::string& s)
    {
      auto index = s.find_last_of(".") ;
      if (index==std::string::npos) { index = 0; }
      else { ++index; }
      if (s.compare(index,3, "Prs") == 0) return DetectorName_t::Prs;
      if (s.compare(index,3, "Spd") == 0) return DetectorName_t::Spd;
      auto t = s.substr(index,4);
      if ( t == "Hcal" ) return DetectorName_t::Hcal;
      if ( t == "Ecal" ) return DetectorName_t::Ecal;
      return DetectorName_t::Unknown;
    }
}
