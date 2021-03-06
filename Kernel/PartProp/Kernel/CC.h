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
#ifndef PARTPROP_KERNEL_CC_H
#define PARTPROP_KERNEL_CC_H 1
// ============================================================================
// Include files
// ============================================================================
// STD & STL
// ============================================================================
#include <map>
#include <set>
#include <string>
// ============================================================================
namespace Decays {
  // ==========================================================================
  namespace CC {
    // ========================================================================
    /** @struct CmpCC
     *  a bit specific comparison of strings, useful for
     *  ordering according to the length as the primary parameter
     *  @author Vanya BELYAEV Ivan.Belyaev@nikhef.nl
     */
    struct CmpCC {
      /** the only one essential method
       *  The most long string is "less", otherwise the
       *  standard comparison is applicable.
       */
      inline bool operator()( const std::string& v1, const std::string& v2 ) const {
        const std::string::size_type s1 = v1.size();
        const std::string::size_type s2 = v2.size();
        return s1 < s2 ? false : s2 < s1 ? true : ( v1 < v2 );
      }
    };
    // ========================================================================
    /// the actual type of CC-map
    typedef std::map<std::string, std::string, CmpCC> MapCC; // CC-MAP
    /// the actual type of CC-set
    typedef std::set<std::string, CmpCC> SetCC; // CC-SET
    // ========================================================================
    /** simple function to make charge conjugated inside the original string.
     *  All substrings are subsutututed by their charge conjugates
     *  @param orig the original sring
     *  @param map_ the full map of substitutions
     *  @param pos the starting position
     *  @return charge-conjugated string
     */
    std::string cc( const std::string& decay, const MapCC& map_ );
    // ========================================================================
    /** simple function to make charge conjugated inside the original string.
     *  All substrings are subsutututed by their charge conjugates
     *  @param orig the original sring
     *  @param map_ the full map of substitutions
     *  @param pos the starting position
     *  @return charge-conjugated string
     */
    std::string cc( const std::string& decay, const std::map<std::string, std::string>& map_ );
    // ========================================================================
  } // namespace CC
  // ==========================================================================
} // end of namespace Decays
// ============================================================================
// The END
// ============================================================================
#endif // PARTPROP_KERNEL_CC_H
