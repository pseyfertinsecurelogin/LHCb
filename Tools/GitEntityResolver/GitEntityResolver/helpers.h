#ifndef GIT_ENTITY_RESOLVER_HELPERS_H
#define GIT_ENTITY_RESOLVER_HELPERS_H

#include <git2.h>

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

#endif // GIT_ENTITY_RESOLVER_HELPERS_H
