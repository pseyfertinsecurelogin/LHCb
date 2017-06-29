#include <iomanip>
#include <iostream>
#include "GaudiKernel/StatusCode.h"


namespace details {
    struct OutputType_t final {
        bool adcOnTES;
        bool digitOnTES;
    };

    const char* toString(const OutputType_t& out);
    inline std::ostream& toStream(const OutputType_t& out, std::ostream& os)
    { return os << std::quoted(toString(out),'\''); }
    StatusCode parse(OutputType_t& result, const std::string& input );

    enum class source_t { from_adc, from_digit };
    const char* toString(const source_t& source);

    inline std::ostream& toStream(const source_t& source, std::ostream& os) {
        return os << std::quoted(toString(source),'\'');
    }
    StatusCode parse(source_t& result, const std::string& input );

}

