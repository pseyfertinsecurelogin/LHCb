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
#ifndef IARCHIVE_H
#define IARCHIVE_H

#include "boost/iostreams/filtering_stream.hpp"
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace {
  struct all_t {
    template <typename T>
    bool operator()( const T& ) const {
      return true;
    }
  };
  inline constexpr all_t all{};

  struct PrefixFilenameSelector {
    PrefixFilenameSelector( const std::string& _prefix ) : prefix( _prefix ) {}
    bool        operator()( const std::string& fname ) const { return fname.compare( 0, prefix.size(), prefix ) == 0; }
    std::string prefix;
  };
} // namespace

class IArchive {
public:
  virtual ~IArchive() = default;

  template <typename SELECTOR>
  std::vector<std::string> files( const SELECTOR& selector = all ) const {
    std::vector<std::string> f = contents();
    f.erase( std::remove_if( begin( f ), end( f ), [=]( const std::string& s ) { return !selector( s ); } ), end( f ) );
    return f;
  }

  template <typename T>
  std::optional<T> get( const std::string& name ) const {
    boost::iostreams::filtering_istream strm;
    if ( !setupStream( strm, name ) ) return {};
    T t;
    strm >> t;
    if ( strm.fail() ) return {};
    return t;
  }

  virtual bool operator!() const                                = 0;
  virtual bool writeable() const                                = 0;
  virtual bool append( const std::string&, std::stringstream& ) = 0;

protected:
  virtual bool setupStream( boost::iostreams::filtering_istream& s, const std::string& name ) const = 0;
  virtual std::vector<std::string> contents() const                                                 = 0;
};
#endif
