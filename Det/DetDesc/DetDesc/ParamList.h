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
#ifndef DETDESC_PARAMLIST_H
#define DETDESC_PARAMLIST_H 1

// Include files
#include "DetDesc/Param.h"
#include <map>
#include <memory>

#include "GaudiKernel/Map.h"

/** @class ParamList ParamList.h DetDesc/ParamList.h
 *
 *  Simple implementation of a polymorphic list of parameters identified by
 *  an std::string.
 *
 *  @author Marco CLEMENCIC
 *  @date   2005-02-22
 */
class ParamList final {

  std::map<std::string, std::unique_ptr<BasicParam>> m_map;

public:
  /// Standard constructor
  ParamList() = default;
  ParamList( const ParamList& );
  ParamList& operator      =( const ParamList& );
  ParamList( ParamList&& ) = default;
  ParamList& operator=( ParamList&& ) = default;

  const BasicParam* find( const std::string& key ) const {
    auto i = m_map.find( key );
    return i != m_map.end() ? i->second.get() : nullptr;
  }
  BasicParam* find( const std::string& key ) {
    auto i = m_map.find( key );
    return i != m_map.end() ? i->second.get() : nullptr;
  }

  /// Add a new parameter to the list (or replace if already there)
  template <class T>
  void add( const std::string& key, T val ) {
    auto i = m_map.find( key );
    if ( i != m_map.end() ) { // key already used
      i->second->set( std::move( val ) );
    } else {
      m_map.emplace( key, std::make_unique<Param<T>>( std::move( val ) ) );
    }
  }

  void addBasicParam( const std::string& key, const BasicParam& p ) { m_map.insert_or_assign( key, p.clone() ); }

  /// return a vector containing all the stored keys
  virtual std::vector<std::string> getKeys() const;

  /// Merge two lists (overwriting objects with the same name)
  ParamList& operator+=( const ParamList& pl );

  /// Remove all elements from the list, deleting the objects
  void clear();
};
#endif // DETDESC_PARAMLIST_H
