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


}

