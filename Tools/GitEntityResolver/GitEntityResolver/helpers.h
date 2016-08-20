#ifndef GIT_ENTITY_RESOLVER_HELPERS_H
#define GIT_ENTITY_RESOLVER_HELPERS_H

#include <algorithm>
#include <boost/filesystem.hpp>
#include <git2.h>
#include <string>
#include <vector>

namespace std
{
  template <>
  struct default_delete<git_object> {
    void operator()( git_object* ptr ) const { git_object_free( ptr ); }
  };
  template <>
  struct default_delete<git_repository> {
    void operator()( git_repository* ptr ) const { git_repository_free( ptr ); }
  };
}

using git_object_ptr     = std::unique_ptr<git_object>;
using git_repository_ptr = std::unique_ptr<git_repository>;

namespace Git
{
  namespace Helpers
  {
    bool is_dir( const std::string& path ) { return boost::filesystem::is_directory( path ); }
    bool is_dir( const git_object_ptr& obj ) { return git_object_type( obj.get() ) == GIT_OBJ_TREE; }
    bool is_dir( const git_tree_entry* te ) { return git_tree_entry_type( te ) == GIT_OBJ_TREE; }
  }
}
#endif // GIT_ENTITY_RESOLVER_HELPERS_H
