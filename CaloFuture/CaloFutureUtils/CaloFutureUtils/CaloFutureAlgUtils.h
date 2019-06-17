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
// ============================================================================
#ifndef CALOFUTUREUTILS_CALOFUTUREALGUTILS_H
#define CALOFUTUREUTILS_CALOFUTUREALGUTILS_H 1
#include "Event/CaloHypo.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace LHCb {
  class CaloCluster;
  namespace CaloFutureAlgUtils {

    namespace details {
      // Try to find in the Haystack the Needle - ignore case
      inline bool contains_ci( std::string_view haystack, std::string_view needle ) {
        auto it = std::search( std::begin( haystack ), std::end( haystack ), std::begin( needle ), std::end( needle ),
                               []( char ch1, char ch2 ) { return std::toupper( ch1 ) == std::toupper( ch2 ); } );
        return it != std::end( haystack );
      }

      inline std::string to_string( std::string_view sr ) { return std::string{sr}; }

      inline std::string toUpper( std::string_view str ) {
        auto uStr = to_string( str );
        std::transform( uStr.begin(), uStr.end(), uStr.begin(), ::toupper );
        return uStr;
      }

      inline std::string operator+( const std::string& lhs, std::string_view rhs ) { return lhs + to_string( rhs ); }

    } // namespace details
    using details::toUpper;

    inline std::string CaloFutureNameFromAlg( std::string_view algName ) {
      // find tool name separator if any
      int  index = algName.find_last_of( "." ) + 1; // return 0 if '.' not found --> OK !!
      auto inst  = algName.substr( index );
      return details::contains_ci( inst, "ECAL" )
                 ? "Ecal"
                 : details::contains_ci( inst, "HCAL" )
                       ? "Hcal"
                       : details::contains_ci( inst, "PRS" )
                             ? "Prs"
                             : details::contains_ci( inst, "SPD" ) ? "Spd" : "Ecal"; // return Ecal as default
    }

    // Default location for CaloFutureObject as function of detector
    std::string              DeCaloFutureLocation( const std::string& name );
    std::string              CaloFutureRawBankReadoutStatusLocation( const std::string& name );
    std::string              CaloFutureAdcLocation( const std::string& name );
    std::string              CaloFutureUnfilteredDigitLocation( const std::string& name );
    std::string              CaloFutureDigitLocation( const std::string& name );
    std::string              CaloFutureClusterLocation( const std::string& name, const std::string& type = "" );
    std::string              CaloFutureSplitClusterLocation();
    std::string              CaloFutureHypoLocation( const std::string& type );
    std::string              CaloFutureIdLocation( const std::string& type );
    std::vector<std::string> TrackLocations();
    const LHCb::CaloCluster* ClusterFromHypo( const LHCb::CaloHypo* hypo, bool split = true );
    bool                     StringMatcher( const std::vector<std::string>& refs, const std::string& name );
    bool                     StringMatcher( const std::string& ref, const std::string& name );
  } // namespace CaloFutureAlgUtils
} // end of namespace LHCb

namespace Calo::Future::Utilities {}
#endif // CALOFUTUREUTILS_CALOFUTUREALGUTILS_H
