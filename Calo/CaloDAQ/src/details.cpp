#include "details.h"
#include <algorithm>


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
        if (out == "BOTH" ) {
           result.digitOnTES = true;
           result.adcOnTES = true;
           return StatusCode::SUCCESS;
        }
        if( out == "DIGITS" ||  out == "CALODIGITS" ||
            out == "DIGIT"  ||  out == "CALODIGIT"  ) {
           result.digitOnTES = true;
           result.adcOnTES = false;
           return StatusCode::SUCCESS;
        }
        if(out == "ADCS" ||  out == "CALOADCS" ||
           out == "ADC"  ||  out == "CALOADC"  ) {
            result.digitOnTES = false;
            result.adcOnTES = true;
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

        if( out == "ADC" || out == "CALOADC" || out == "ADCS" || out == "CALOADCS") {
            result = source_t::from_adc;
            return StatusCode::SUCCESS;
        }
        if( out == "DIGIT" || out == "CALODIGIT" || out == "DIGITS" || out == "CALODIGITS")  {
            result = source_t::from_digit;
            return StatusCode::SUCCESS;
        }
        return StatusCode::FAILURE;
    }

    const std::string& toString( DetectorName_t dn ) {
        static const std::string labels[5] = { "Ecal","Hcal","Prs","Spd","Unknown" };
        auto i = static_cast<int>(dn);
        return labels[ i>4 ? 4 : i ];
    }

    StatusCode parse(DetectorName_t& result, const std::string& input ) {
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
      if (s.compare(index,3, "Prs") == 0) return DetectorName_t::Prs;
      if (s.compare(index,3, "Spd") == 0) return DetectorName_t::Spd;
      auto t = s.substr(index,4);
      if ( t == "Hcal" ) return DetectorName_t::Hcal;
      if ( t == "Ecal" ) return DetectorName_t::Ecal;
      return DetectorName_t::Unknown;
    }
}

