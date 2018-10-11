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
// ============================================================================
#ifndef CALOUTILS_CALOALGUTILS_H
#define CALOUTILS_CALOALGUTILS_H 1
#include "Event/CaloHypo.h"
#include "Event/CaloCluster.h"
#include <string>
#include <cctype>
#include <algorithm>
#include <boost/utility/string_ref.hpp>

namespace LHCb{
  namespace CaloAlgUtils{

    namespace details {
        // Try to find in the Haystack the Needle - ignore case
       inline bool contains_ci(boost::string_ref haystack, boost::string_ref needle)
       {
          auto it = std::search( std::begin(haystack), std::end(haystack),
                                 std::begin(needle),   std::end(needle),
                      [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
          );
          return it != std::end(haystack);
       }

        inline std::string to_string( boost::string_ref sr) {
            return { sr.data(), sr.size() };
        }

        inline std::string toUpper(boost::string_ref str){
          auto uStr = to_string( str );
          std::transform( uStr.begin(), uStr.end(), uStr.begin(), ::toupper ) ;
          return uStr;
        }

        inline std::string operator+(const std::string& lhs, boost::string_ref rhs) {
            return lhs + to_string(rhs);
        }

    }
    using details::toUpper;

    inline bool hltContext( boost::string_ref context ){
      return details::contains_ci( context, "HLT");
    }

    inline std::string CaloNameFromAlg(boost::string_ref algName ){
      // find tool name separator if any
      int index = algName.find_last_of(".") +1 ; // return 0 if '.' not found --> OK !!
      auto inst = algName.substr(index);
      return details::contains_ci( inst, "ECAL") ? "Ecal"
           : details::contains_ci( inst, "HCAL") ? "Hcal"
           : details::contains_ci( inst, "PRS" ) ? "Prs"
           : details::contains_ci( inst, "SPD" ) ? "Spd"
           : "Ecal"; // return Ecal as default
    }

    inline std::string PathFromContext( boost::string_ref context , boost::string_ref def ){

      if( context.empty() || details::toUpper( context ) == "OFFLINE" ) return details::to_string( def );          // default standard Offline

      // non-standard locations
      auto index = def.find_first_of("/"); //FIXME: what if std::string::npos?
      using details::operator+;
      std::string loc =  details::to_string(context) + "/" + def.substr(index+1);
      // to be synchronized with ConfUtils
      //      if( 0 != context.find("/Event/") && 0 == context.find("/") )
      //        loc = loc.substr(1);
      return loc;
    }


    // Default location for CaloObject as function of detector and possibly context
    std::string DeCaloLocation( const std::string& name );
    std::string CaloDigitLocation( const std::string& name, const std::string& context="");
    std::string CaloClusterLocation( const std::string& name , const std::string& context="");
    std::string CaloSplitClusterLocation( const std::string& context="");
    std::string CaloHypoLocation( const std::string& type , const std::string& context="");
    std::string CaloIdLocation( const std::string& type, const std::string& context = "");
    std::vector<std::string> TrackLocations( const std::string& context="" );
    const LHCb::CaloCluster*  ClusterFromHypo(const LHCb::CaloHypo* hypo,bool split=true);
    bool StringMatcher(const std::vector<std::string>& refs, const std::string& name);
    bool StringMatcher(const std::string& ref, const std::string& name);
  } // end of namespace CaloUtils
}// end of namespace LHCb
#endif // CALOUTILS_CALOALGUTILS_H
