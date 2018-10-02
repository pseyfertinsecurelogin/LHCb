#ifndef GIT_ENTITY_RESOLVER_HELPERS_H
#define GIT_ENTITY_RESOLVER_HELPERS_H

#include <algorithm>
#include <boost/filesystem.hpp>
#include <git2.h>
#include <string>
#include <mutex>

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

/// Helper class to allow on-demand connection to the git repository.
class git_repository_ptr {
public:
  using storage_t = std::unique_ptr<git_repository>;
  using factory_t = std::function<storage_t()>;
  using pointer = storage_t::pointer;
  using reference = std::add_lvalue_reference<storage_t::element_type>::type;

  git_repository_ptr(factory_t factory): m_factory( std::move( factory ) ) {}

  pointer get() const {
    {
      std::lock_guard guard(m_ptr_mutex);
      if ( !m_ptr ){
        m_ptr = m_factory();
        if ( !m_ptr ) throw std::runtime_error("unable create object");
      }
    }
    return m_ptr.get();
  }

  reference operator*() const {
    return *get();
  }

  pointer operator->() const {
    return get();
  }

  explicit operator bool() const {
    return get();
  }

  void reset() {
    std::lock_guard guard(m_ptr_mutex);
    m_ptr.reset();
  }

private:

  factory_t m_factory;
  mutable storage_t m_ptr;
  mutable std::mutex m_ptr_mutex;
};

namespace Git
{
  namespace Helpers
  {
    inline bool is_dir( const std::string& path ) { return boost::filesystem::is_directory( path ); }
    inline bool is_dir( const git_object_ptr& obj ) { return git_object_type( obj.get() ) == GIT_OBJ_TREE; }
    inline bool is_dir( const git_tree_entry* te ) { return git_tree_entry_type( te ) == GIT_OBJ_TREE; }
  }
}
#endif // GIT_ENTITY_RESOLVER_HELPERS_H
