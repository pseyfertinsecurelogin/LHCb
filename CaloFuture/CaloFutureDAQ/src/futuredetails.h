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
#include "GaudiKernel/StatusCode.h"
#include <iomanip>
#include <iostream>
#include <string_view>

namespace futuredetails {

  //-------------------------------------------------------------------------
  struct OutputType_t final {
    bool adcOnTES;
    bool digitOnTES;
  };

  const char*          toString( const OutputType_t& out );
  inline std::ostream& toStream( const OutputType_t& out, std::ostream& os ) {
    return os << std::quoted( toString( out ), '\'' );
  }
  StatusCode parse( OutputType_t& result, const std::string& input );
  //-------------------------------------------------------------------------

  //-------------------------------------------------------------------------
  enum class source_t { from_adc, from_digit };
  const char* toString( const source_t& source );

  inline std::ostream& toStream( const source_t& source, std::ostream& os ) {
    return os << std::quoted( toString( source ), '\'' );
  }
  StatusCode parse( source_t& result, const std::string& input );
  //-------------------------------------------------------------------------

  //-------------------------------------------------------------------------
  enum class DetectorName_t { Ecal = 0, Hcal, Prs, Spd, Unknown };
  const std::string&   toString( DetectorName_t );
  inline std::ostream& toStream( const DetectorName_t& name, std::ostream& os ) {
    return os << std::quoted( toString( name ), '\'' );
  }
  StatusCode parse( DetectorName_t& result, std::string_view input );

  DetectorName_t alg_name_to_detector( const std::string& s );

} // namespace futuredetails
