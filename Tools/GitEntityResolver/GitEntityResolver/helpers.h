#ifndef GIT_ENTITY_RESOLVER_HELPERS_H
#define GIT_ENTITY_RESOLVER_HELPERS_H

#include <git2.h>
#include <boost/utility/string_ref.hpp>
#include <boost/filesystem.hpp>

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
    bool is_dir( boost::string_ref path ) { return boost::filesystem::is_directory( path.to_string() ); }
    bool is_dir( const git_object_ptr& obj ) { return git_object_type( obj.get() ) == GIT_OBJ_TREE; }
  }
}
#endif // GIT_ENTITY_RESOLVER_HELPERS_H
