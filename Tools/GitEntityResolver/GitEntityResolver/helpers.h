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
#ifndef GIT_ENTITY_RESOLVER_HELPERS_H
#define GIT_ENTITY_RESOLVER_HELPERS_H

#include "Kernel/SynchronizedValue.h"
#include <algorithm>
#include <boost/filesystem.hpp>
#include <git2.h>
#include <string>

namespace std {
  template <>
  struct default_delete<git_object> {
    void operator()( git_object* ptr ) const { git_object_free( ptr ); }
  };
  template <>
  struct default_delete<git_repository> {
    void operator()( git_repository* ptr ) const { git_repository_free( ptr ); }
  };
} // namespace std

using git_object_ptr = std::unique_ptr<git_object>;

/// Helper class to allow on-demand connection to the git repository.
class git_repository_ptr {
public:
  using storage_t = std::unique_ptr<git_repository>;
  using factory_t = std::function<storage_t()>;
  using pointer   = storage_t::pointer;
  using reference = std::add_lvalue_reference<storage_t::element_type>::type;

  git_repository_ptr( factory_t factory ) : m_factory( std::move( factory ) ) {}

  pointer get() const {
    return m_ptr.with_lock( [&]( storage_t& ptr ) {
      if ( !ptr ) {
        ptr = m_factory();
        if ( !ptr ) throw std::runtime_error( "unable create object" );
      }
      return ptr.get();
    } );
  }

  reference operator*() const { return *get(); }

  pointer operator->() const { return get(); }

  explicit operator bool() const { return get(); }

  void reset() {
    m_ptr.with_lock( []( storage_t& ptr ) { ptr.reset(); } );
  }

private:
  factory_t                                       m_factory;
  mutable LHCb::cxx::SynchronizedValue<storage_t> m_ptr;
};

namespace Git {
  namespace Helpers {
    inline bool is_dir( const std::string& path ) { return boost::filesystem::is_directory( path ); }
    inline bool is_dir( const git_object_ptr& obj ) { return git_object_type( obj.get() ) == GIT_OBJ_TREE; }
    inline bool is_dir( const git_tree_entry* te ) { return git_tree_entry_type( te ) == GIT_OBJ_TREE; }
  } // namespace Helpers
} // namespace Git
#endif // GIT_ENTITY_RESOLVER_HELPERS_H
