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
#include "GaudiKernel/Property.h"
#include <algorithm>
#include <cctype>
#include <string>
#include <string_view>
#include <vector>

namespace LHCb {
  class CaloCluster;
  namespace Calo::Utilities {

    namespace details {
      // Try to find in the Haystack the Needle - ignore case
      inline bool contains_ci( std::string_view haystack, std::string_view needle ) {
        auto it = std::search( std::begin( haystack ), std::end( haystack ), std::begin( needle ), std::end( needle ),
                               []( char ch1, char ch2 ) { return std::toupper( ch1 ) == std::toupper( ch2 ); } );
        return it != std::end( haystack );
      }

      inline std::string toUpper( std::string_view str ) {
        auto uStr = std::string{str};
        std::transform( uStr.begin(), uStr.end(), uStr.begin(), ::toupper );
        return uStr;
      }

      // TODO: use fmt::format / std::format
      inline std::string operator+( std::string_view lhs, std::string_view rhs ) {
        return std::string{lhs}.append( rhs );
      }

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
    std::string              DeCaloFutureLocation( std::string_view name );
    std::string              CaloFutureRawBankReadoutStatusLocation( std::string_view name );
    std::string              CaloFutureAdcLocation( std::string_view name );
    std::string              CaloFutureUnfilteredDigitLocation( std::string_view name );
    std::string              CaloFutureDigitLocation( std::string_view name );
    std::string              CaloFutureClusterLocation( std::string_view name, std::string_view type = "" );
    std::string              CaloFutureSplitClusterLocation();
    std::string              CaloFutureHypoLocation( std::string_view type );
    std::string              CaloFutureIdLocation( std::string_view type );
    std::vector<std::string> TrackLocations();
    const CaloCluster*       ClusterFromHypo( const CaloHypo* hypo, bool split = true );
    bool                     StringMatcher( const std::vector<std::string>& refs, const std::string& name );
    bool                     StringMatcher( const std::string& ref, const std::string& name );

    inline void installDeCaloLocationHandler( Gaudi::Details::PropertyBase& p ) {
      p.declareUpdateHandler( []( Gaudi::Details::PropertyBase& p ) {
         auto s = p.toString();
         if ( !s.empty() && s.front() != '/' ) { p.fromString( DeCaloFutureLocation( s ) ).ignore(); }
       } ).useUpdateHandler();
    }

  } // namespace Calo::Utilities
} // end of namespace LHCb

namespace LHCb::CaloFutureAlgUtils {
  using namespace Calo::Utilities; // for backwards compatibility
}
#endif // CALOFUTUREUTILS_CALOFUTUREALGUTILS_H
