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
#include "GaudiAlg/GaudiTool.h"
#include "Kernel/IDQFilter.h" // IDQFilter
#include <algorithm>
#include <set>
#include <string>
#include <vector>

// ----------------------------------------------------------------------------
// Implementation file for class: BasicDQFilter
//
// 04/11/2011: Marco Clemencic
// ----------------------------------------------------------------------------
/** Basic implementation of a IDQFilter.
 *
 * Accept the list of flags only if none is set, except those specified in the
 * IgnoredFlags property.
 *
 * @author Marco Clemencic
 * @date 04/11/2011
 */
class BasicDQFilter final : public extends<GaudiTool, IDQFilter> {

public:
  /// Standard constructor
  using extends::extends;

  /// Accept the flags if there is not flag set, except the ignored ones
  /// (property IgnoredFlags).
  bool accept( const FlagsType& flags ) const override {
    return std::none_of( flags.begin(), flags.end(), [&]( const auto& f ) {
      // if the flag is not ignored
      if ( m_ignoredFlags.find( f.first ) != m_ignoredFlags.end() ) {
        if ( UNLIKELY( msgLevel( MSG::DEBUG ) ) ) debug() << "Ignoring flag: " << f.first << endmsg;
        return false;
      }
      return f.second != 0;
    } );
  }

private:
  struct cmpi {
    bool operator()( std::string_view a, std::string_view b ) const {
      return std::lexicographical_compare( begin( a ), end( a ), begin( b ), end( b ),
                                           []( char x, char y ) { return std::tolower( x ) < std::tolower( y ); } );
    }
  };

  /// Internal storage for ignored flags.
  std::set<std::string, cmpi> m_ignoredFlags;

  /// List of flag (names) to ignore during the filtering. The matching is
  /// case-insensitive. (property IgnoredFlags)
  Gaudi::Property<std::vector<std::string>> m_ignoredFlagsProp{
      this,
      "IgnoredFlags",
      {},
      [=]( auto& ) {
        m_ignoredFlags = {begin( m_ignoredFlagsProp ), end( m_ignoredFlagsProp )};
      },
      "List of flags to ignore when filtering events"};
};
DECLARE_COMPONENT( BasicDQFilter )

// ============================================================================
