
#ifndef LHCBKERNEL_STLExtensions
#define LHCBKERNEL_STLExtensions 1

// This header contains methods that are likely to become parts of the STL
// at some point in the future, but currently are not available.

#include <array>
#include <type_traits>

namespace LHCb
{

  /// An implementation (cut down) of 'std::make_array' based on
  /// http://en.cppreference.com/w/cpp/experimental/make_array
  /// Once we target gcc 6.1 or newer only, this can be removed
  /// and <experimental/array> directly used instead.
  template <typename... T>
  constexpr decltype(auto) make_array( T && ... values )
  {
    return std::array< 
    typename std::decay< 
    typename std::common_type<T... >::type >::type, sizeof...(T) > 
    { std::forward<T>(values)... };
  }
  
}

#endif // LHCBKERNEL_STLExtensions
