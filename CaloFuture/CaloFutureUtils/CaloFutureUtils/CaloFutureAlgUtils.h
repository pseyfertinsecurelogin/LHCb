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
#include "Event/CaloCluster.h"
#include "Event/CaloHypo.h"
#include <algorithm>
#include <boost/utility/string_ref.hpp>
#include <cctype>
#include <string>

namespace LHCb {
  namespace CaloFutureAlgUtils {

    namespace futuredetails {
      // Try to find in the Haystack the Needle - ignore case
      inline bool contains_ci( boost::string_ref haystack, boost::string_ref needle ) {
        auto it = std::search( std::begin( haystack ), std::end( haystack ), std::begin( needle ), std::end( needle ),
                               []( char ch1, char ch2 ) { return std::toupper( ch1 ) == std::toupper( ch2 ); } );
        return it != std::end( haystack );
      }

      inline std::string to_string( boost::string_ref sr ) { return {sr.data(), sr.size()}; }

      inline std::string toUpper( boost::string_ref str ) {
        auto uStr = to_string( str );
        std::transform( uStr.begin(), uStr.end(), uStr.begin(), ::toupper );
        return uStr;
      }

      inline std::string operator+( const std::string& lhs, boost::string_ref rhs ) { return lhs + to_string( rhs ); }

    } // namespace futuredetails
    using futuredetails::toUpper;

    inline bool hltContext( boost::string_ref context ) { return futuredetails::contains_ci( context, "HLT" ); }

    inline std::string CaloFutureNameFromAlg( boost::string_ref algName ) {
      // find tool name separator if any
      int  index = algName.find_last_of( "." ) + 1; // return 0 if '.' not found --> OK !!
      auto inst  = algName.substr( index );
      return futuredetails::contains_ci( inst, "ECAL" )
                 ? "Ecal"
                 : futuredetails::contains_ci( inst, "HCAL" )
                       ? "Hcal"
                       : futuredetails::contains_ci( inst, "PRS" )
                             ? "Prs"
                             : futuredetails::contains_ci( inst, "SPD" ) ? "Spd" : "Ecal"; // return Ecal as default
    }

    inline std::string PathFromContext( boost::string_ref context, boost::string_ref def ) {

      if ( context.empty() || futuredetails::toUpper( context ) == "OFFLINE" )
        return futuredetails::to_string( def ); // default standard Offline

      // non-standard locations
      auto                 index = def.find_first_of( "/" ); // FIXME: what if std::string::npos?
      using futuredetails::operator+;
      std::string          loc = futuredetails::to_string( context ) + "/" + def.substr( index + 1 );
      // to be synchronized with ConfUtils
      //      if( 0 != context.find("/Event/") && 0 == context.find("/") )
      //        loc = loc.substr(1);
      return loc;
    }

    // Default location for CaloFutureObject as function of detector and possibly context
    std::string DeCaloFutureLocation( const std::string& name );
    std::string CaloFutureRawBankReadoutStatusLocation( const std::string& name, const std::string& context = "" );
    std::string CaloFutureAdcLocation( const std::string& name, const std::string& context = "" );
    std::string CaloFutureUnfilteredDigitLocation( const std::string& name );
    std::string CaloFutureDigitLocation( const std::string& name );
    std::string CaloFutureClusterLocation( const std::string& name, const std::string& context = "",
                                           const std::string& type = "" );
    std::string CaloFutureSplitClusterLocation( const std::string& context = "" );
    std::string CaloFutureHypoLocation( const std::string& type, const std::string& context = "" );
    std::string CaloFutureIdLocation( const std::string& type, const std::string& context = "" );
    std::vector<std::string> TrackLocations( const std::string& context = "" );
    const LHCb::CaloCluster* ClusterFromHypo( const LHCb::CaloHypo* hypo, bool split = true );
    bool                     StringMatcher( const std::vector<std::string>& refs, const std::string& name );
    bool                     StringMatcher( const std::string& ref, const std::string& name );
  } // namespace CaloFutureAlgUtils
} // end of namespace LHCb
#endif // CALOFUTUREUTILS_CALOFUTUREALGUTILS_H
